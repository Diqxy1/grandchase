#include "Center.h"
#include <dbg/dbg.hpp>
#include "CnGuildManager.h"
#include "CenterDBThread.h"
#include "CenterSimLayer.h"
#include "CenterEvent.h"
#include <boost/mem_fn.hpp>
#include <boost/algorithm/string.hpp> //boost::to_lower
#include "Lua/KLuaManager.h"
#include <boost/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

namespace
{
    // 길드명 정렬
    class KSortName
    {
    public:
        bool operator() ( const KCnGuildPtr ptA, const KCnGuildPtr ptB )
        {
            if( ptA == NULL || ptB == NULL) return false;
            return ptA->GetGuildInfo().m_strName < ptB->GetGuildInfo().m_strName;
        }
    };

    // 등급별 정렬
    class KSortGrade
    {
    public:
        bool operator() ( const KCnGuildPtr ptA, const KCnGuildPtr ptB )
        {
            if( ptA == NULL || ptB == NULL) return false;
            return ptA->GetGuildInfo().m_ucGrade > ptB->GetGuildInfo().m_ucGrade;
        }
    };

    // 길드포인트별 정렬
    class KSortPoint
    {
    public:
        bool operator() ( const KCnGuildPtr ptA, const KCnGuildPtr ptB )
        {
            if( ptA == NULL || ptB == NULL) return false;
            return ptA->GetGuildInfo().m_dwPoint > ptB->GetGuildInfo().m_dwPoint;
        }
    };

    // ID(=Date) 정렬
    class KSortIDdate
    {
    public:
        bool operator() ( const KCnGuildPtr ptA, const KCnGuildPtr ptB )
        {
            if( ptA == NULL || ptB == NULL) return false;
            return ptA->GetGuildInfo().m_dwUID < ptB->GetGuildInfo().m_dwUID;
        }
    };
}

ImplementSingleton( KCnGuildManager );
NiImplementRootRTTI( KCnGuildManager );
ImplOstmOperatorW2A( KCnGuildManager );
ImplToStringW( KCnGuildManager )
{
    KLocker lock( m_csGuilds );
    KLocker lock2(m_csGuildRank);

    START_TOSTRINGW
        << TOSTRINGW( m_vecGuild.size() )
        << TOSTRINGW( m_mapGuild.size() )
        << TOSTRINGW( m_mapSimpleGuildRank.size() )
        << TOSTRINGW( m_mapCompressedRankData.size() )
        << TOSTRINGW( m_mapGuildPage.size() );

    std::map<UCHAR,std::vector<DWORD> >::const_iterator cmit;
    for ( cmit = m_mapGuildPage.begin() ; cmit != m_mapGuildPage.end() ; ++cmit ) {
        stm_ << L" PageType : " << (int)cmit->first << L", size : " << cmit->second.size() << std::endl;
    }

    return stm_;
}

KCnGuildManager::KCnGuildManager(void)
:m_kMinTimer(1000 * 60, false)
,m_kGuildClearTimer(1000 * 60 * 10, false)
,m_bChangeRank(false)
,m_bConfigChanged(false)
,m_dwChangeGuildTickGap(1000*60)
,m_dwUpdateGuildCount(50)
{
    // 페이지 설정 초기화
    std::vector<DWORD> vecBuf;
    vecBuf.clear();
    for ( UCHAR ucType = ST_GUILD_ID_DATE ; ucType < ST_MAX ; ++ucType ) {
        m_mapGuildPage[ ucType ] = vecBuf;
    }
    m_mapChangePointGuild.clear(); // Init.
    m_dwChangeGuildLastTick = ::GetTickCount();
}

KCnGuildManager::~KCnGuildManager(void)
{
}

void KCnGuildManager::Init()
{
    // DB에 최초 길드정보 요청.
    SendUpdateGuildListReqToDB();
}

void KCnGuildManager::Tick()
{
    DistributeEvent();
    EachGuildTick();
    IntervalTick();
    if ( IsConfigChanged() ) {
        KGuildConfig kPacket;
        GetGuildConfig( kPacket );
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_GUILD_CONFIG_NOT, kPacket, true );
    }
}

void KCnGuildManager::EachGuildTick()
{
    KLocker lock( m_csGuilds );
    std::for_each( m_vecGuild.begin(), m_vecGuild.end(), boost::mem_fn(&KCnGuild::Tick) );
}

void KCnGuildManager::IntervalTick()
{
    //{{ 1분 단위 처리
    if ( !m_kMinTimer.CheckTime() ) {
        return;
    }
    // 길드 정보 업데이트 체크
    SiKCenterDBMgr()->QueueingID( KCenterEvent::DB_ECN_UPDATE_GUILD_INFO_CHECK_REQ, L"", 0 );

    if ( IsRankChange() ) {
        SendGuildRankData();
        SetRankChange( false );
    }

    UpdateGuildPointTick(); // 길드 포인트 변화 체크.
    m_kMinTimer.Reset();
    //}}


    //{{ 1시간 단위 체크
    if ( !m_kGuildClearTimer.CheckTime() ) {
        return;
    }
    // 접속중인 맴버가 없으면 길드정보 삭제.
    ClearGuildInfoCheck();
    m_kGuildClearTimer.Reset();
    //}}

    if ( DayCheck() ) {
        SendUpdateGuildListReqToDB();
    }
}

bool KCnGuildManager::DayCheck()
{
    CTime tDate = CTime::GetCurrentTime();
    KSimpleDate kToday( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0 );

    // 마지막 검사 날짜와 현재 날짜가 다른가?
    if ( m_kLastDayCheckDate == kToday ) {
        return false;
    }

    // 마지막 검사 날짜 = 현재 날짜
    m_kLastDayCheckDate.SetDate( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0  );
    START_LOG( clog, L"Set LastDayCheck Date : " << m_kLastDayCheckDate.Str() ) << END_LOG;
    return true;
}

bool KCnGuildManager::IsPageEmpty()
{
    KLocker lock( m_csGuilds );
    // 페이지가 비어있는 경우.
    return ( m_mapGuildPage[ST_GUILD_ID_DATE].empty() &&
             m_mapGuildPage[ST_GUILD_NAME].empty() &&
             m_mapGuildPage[ST_GUILD_GRADE].empty() &&
             m_mapGuildPage[ST_GUILD_POINT].empty() );
}

bool KCnGuildManager::AddGuild( IN KNGuildInfo& kInfo_ )
{
    // 길드 존재여부 확인
    KCnGuildPtr spGuild = GetGuild( kInfo_.m_dwUID );
    if ( spGuild ) {
        START_LOG( cerr, L"이미 존재하는 길드입니다, 길드 ID : " << kInfo_.m_dwUID ) << END_LOG;
        return false;
    }

    KGuildConfig kConfig;
    GetGuildConfig( kConfig );

    // 길드정보 만들고
    spGuild.reset( new KCnGuild );
    spGuild->SetGuildInfo( kInfo_ );
    spGuild->SetConfig( kConfig );

    // 매니저에 등록
    {
        KLocker lock( m_csGuilds );

        if ( !m_mapGuild.insert( std::make_pair( kInfo_.m_dwUID, spGuild ) ).second ) {
            START_LOG( cerr, L"이미 존재하는 길드입니다, 길드 ID : " << kInfo_.m_dwUID ) << END_LOG;
            return false;
        }
        m_vecGuild.push_back(spGuild);
    }
    START_LOG( clog, L"길드매니저에 길드정보 추가. 길드 ID : " << kInfo_.m_dwUID ) << END_LOG;
    return true;
}

void KCnGuildManager::AddGuild( IN std::map< DWORD, KNGuildInfo >& mapGuildList_ )
{
    // 전체 길드정보 설정.
    std::map< DWORD, KNGuildInfo >::iterator mit;
    KGuildConfig kConfig;
    GetGuildConfig( kConfig );

    for ( mit = mapGuildList_.begin() ; mit != mapGuildList_.end() ; ++mit ) {

        // 길드 존재여부 확인
        DWORD dwGuildUID = mit->first;
        KCnGuildPtr spGuild = GetGuild( dwGuildUID );
        if ( spGuild ) {
            START_LOG( cwarn, L"이미 존재하는 길드입니다, GuildUID : " << dwGuildUID ) << END_LOG;
            continue;
        }

        // 길드정보 만들고
        spGuild.reset( new KCnGuild );
        spGuild->SetGuildInfo( mit->second );
        spGuild->SetConfig( kConfig );

        {
            KLocker lock( m_csGuilds );
            // 매니저에 등록
            if ( !m_mapGuild.insert( std::make_pair( dwGuildUID, spGuild ) ).second ) {
                START_LOG( cwarn, L"이미 존재하는 길드입니다, GuildUID : " << dwGuildUID ) << END_LOG;
                continue;
            }
            m_vecGuild.push_back(spGuild);
        }
        //START_LOG( clog, L"길드매니저에 길드정보 추가. 길드 ID : " << mit->second.m_dwUID ) << END_LOG;
    }
    START_LOG( clog, L"길드매니저에 길드정보들 추가됨. 추가된 길드수 : " << mapGuildList_.size() ) << END_LOG;

    // 소팅 & 페이지 빌드
    BuildGuildPageList();
    // Page정보 전송
    //SendGuildPageNot(); // 자정(0시)에 게임서버에 영향 주는지 확인
    // 빌드후 미사용 길드 정보 바로 삭제.
    ClearGuildInfoCheck();
}

void KCnGuildManager::RemoveGuild( IN DWORD dwGuildUID_ )
{
    std::vector<KCnGuildPtr>::iterator vit;
    std::map<DWORD, KNGuildUserInfo> mapUserList;

    KCnGuildPtr spGuild = GetGuild( dwGuildUID_ );
    if ( spGuild != NULL ) {
        spGuild->GetMemberList( mapUserList );

        std::map<DWORD, KNGuildUserInfo>::iterator mitUser;
        for ( mitUser = mapUserList.begin() ; mitUser != mapUserList.end() ; ++mitUser ) {
            DelUserInGuildInfo( mitUser->first );
        }
    }

    {
        KLocker lock( m_csGuilds );
        std::map<DWORD,KCnGuildPtr>::iterator mit = m_mapGuild.find( dwGuildUID_ );
        if ( mit != m_mapGuild.end() ) {
            // 포인터에서 삭제
            vit = std::find( m_vecGuild.begin(), m_vecGuild.end(), mit->second );
            if ( vit != m_vecGuild.end() ) {
                m_vecGuild.erase( vit );
                START_LOG( clog, L"m_vecGuild에서 길드정보 제거. GuildUID : " << dwGuildUID_ ) << END_LOG;
            }

            m_mapGuild.erase( mit );
            START_LOG( clog, L"m_mapGuild에서 길드정보 제거. GuildUID : " << dwGuildUID_ ) << END_LOG;
        }
    }

    // Page에 길드UID 제거
    RemovePageGuild( dwGuildUID_ );

    // 각 게임서버로 길드삭제 알림
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_REMOVE_GUILD_BROAD_NOT, dwGuildUID_ );
}

KCnGuildPtr KCnGuildManager::GetGuild( DWORD dwGuildID_ )
{
    std::map<DWORD,KCnGuildPtr>::iterator mit;
    KLocker lock( m_csGuilds );
    mit = m_mapGuild.find( dwGuildID_ );

    if ( mit == m_mapGuild.end() )
        return KCnGuildPtr();

    return mit->second;
}

bool KCnGuildManager::AddUser( IN const DWORD dwGuildUID_, IN KNGuildUserInfo& kUserInfo_, IN KCenterPtr spCenter_ )
{
    KCnGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( spGuild == NULL ) {
        START_LOG( cerr, L"없는 길드에 유저를 추가하려고 함" )
            << BUILD_LOG( dwGuildUID_ )
            << BUILD_LOG( kUserInfo_.m_strLogin )
            << BUILD_LOGc( kUserInfo_.m_cMemberLevel ) << END_LOG;
        return false;
    }

    // 길드에 유저 정보 추가
    spGuild->SetOnline( kUserInfo_, spCenter_ );
    AddUserInGuildInfo( kUserInfo_.m_dwUserUID, kUserInfo_.m_dwGuildUID );
    return true;
}

bool KCnGuildManager::DrumoutUser( IN const DWORD dwGuildUID_, IN const std::set<DWORD>& setUserUID_, IN const char cOutType_ )
{
    // 길드원이 탈퇴된 경우 불린다.
    std::set<DWORD>::const_iterator sit;

    KCnGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( spGuild == NULL ) {
        START_LOG( cerr, L"없는 길드에 유저를 탈퇴하려고 함" )
            << BUILD_LOG( dwGuildUID_ )
            << BUILD_LOG( setUserUID_.size() ) << END_LOG;
        return false;
    }

    // 길드에 유저 정보 삭제
    for ( sit = setUserUID_.begin() ; sit != setUserUID_.end() ; ++sit ) {
        // 해당 길드에서 유저 탈퇴처리(제거)
        LIF( spGuild->DrumoutUser( *sit, cOutType_ ) );

        // 유저UID별 길드UID정보 제거
        DelUserInGuildInfo( *sit );
    }

    START_LOG( clog, L"길드매니저에 유저정보 삭제. GuildUID : " << dwGuildUID_ << L", size : " << setUserUID_.size() ) << END_LOG;
    return true;
}

bool KCnGuildManager::SetGuildMemberInfo( IN const DWORD dwGuildUID_, IN std::map<DWORD, KNGuildUserInfo>& mapGuildUserList_ )
{
    KCnGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( spGuild == NULL ) {
        START_LOG( cerr, L"없는 길드에 유저 리스트를 추가하려고 함. GuildUID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    // 유저UID, 길드UID 정보 저장
    std::map<DWORD, KNGuildUserInfo>::iterator mit;
    for ( mit = mapGuildUserList_.begin() ; mit != mapGuildUserList_.end() ; ++mit ) {
        AddUserInGuildInfo( mit->first, mit->second.m_dwGuildUID );
    }
    START_LOG( clog, L"길드매니저에 유저정보 추가함. 유저정보 개수 : " << mapGuildUserList_.size() ) << END_LOG;

    // 해당 길드에도 전달
    spGuild->SetMemberList( mapGuildUserList_ );
    spGuild->SetGuildMaster(); // 길드마스터 찾아서 길드 정보에 넣기.
    return true;
}

bool KCnGuildManager::OfflineUser( IN DWORD dwUserUID_ )
{
    // 길드유저 접속종료 처리
    // 유저의 길드ID를 찾는다.
    DWORD dwGuildUID = GetUserGuildID( dwUserUID_ );
    if ( dwGuildUID == 0 ) {
        START_LOG( cwarn, L"삭제되었거나, 길드유저가 아니다. UserUID : " << dwUserUID_ ) << END_LOG;
        return false;
    }
    DelUserInGuildInfo( dwUserUID_ );

    // 유저의 길드 포인터를 찾는다.
    KCnGuildPtr spGuild = GetGuild( dwGuildUID );
    if( spGuild == NULL )
    {
        START_LOG( cerr, L"길드 포인터 없음. UserUID : " << dwUserUID_ << L",GuildUID : " << dwGuildUID ) << END_LOG;
        return false;
    }

    // 해당길드에서 유저 접속종료 처리
    if( spGuild->SetOffline( dwUserUID_ ) == false ) {
        START_LOG( cwarn, L"이미 접속종료 처리됨. UserUID : " << dwUserUID_ << L",GuildUID : " << dwGuildUID ) << END_LOG;
        return false;
    }

    // 길드에 접속중인 유저 머릿수 체크
    if( spGuild->GetLiveUserCount() == 0 )
    {
        //RemoveGuild( spGuild->GetUID() ); // 길드제거.정책상 안함.
        _LOG_CONDITION( spGuild.use_count() == 1, clog, cwarn, L"Ref Count : " << spGuild.use_count() )  << END_LOG;
    }

    START_LOG( clog, L"길드정보에서 유저 오프라인 처리. UserUID : " << dwUserUID_ << L",GuildUID : " << dwGuildUID ) << END_LOG;
    return true;
}

bool KCnGuildManager::BroadGuildUserOnline( IN DWORD dwUserUID_ )
{
    KCnGuildPtr spGuild = GetUserGuild( dwUserUID_ );

    if( spGuild == NULL )
    {
        START_LOG( cerr, L"유저에 해당하는 길드정보가 없음. UserUID : " << dwUserUID_ ) << END_LOG;
        return false;
    }

    KNGuildUserInfo kUserInfo;
    spGuild->GetUserInfo( dwUserUID_, kUserInfo );

    KECN_ADD_GUILD_USER_BROAD_NOT kPacket;
    kPacket.m_dwGuildUID = kUserInfo.m_dwGuildUID;
    kPacket.m_kGuildUserInfo = kUserInfo;
    spGuild->SendToAll( KCenterEvent::ECN_ADD_GUILD_USER_BROAD_NOT, kPacket );

    return true;
}

bool KCnGuildManager::BroadGuildUserOffline( IN DWORD dwUserUID_ )
{
    KCnGuildPtr spGuild = GetUserGuild( dwUserUID_ );

    if( spGuild == NULL )
    {
        START_LOG( cwarn, L"유저에 해당하는 길드정보가 없음. UserUID : " << dwUserUID_ ) << END_LOG;
        return false;
    }

    if( spGuild->SetOffline( dwUserUID_ ) == false ) {
        START_LOG( cwarn, L"이미 오프라인 길드유저. UserUID : " << dwUserUID_ ) << END_LOG;
        return true;
    }

    return true;
}

KCnGuildPtr KCnGuildManager::GetUserGuild( IN DWORD dwUserUID_ )
{
    DWORD dwGuildUID = 0;
    {
        KLocker lock( m_csUserMap );
        std::map<DWORD,DWORD>::iterator mitUser = m_mapUserInGuild.find( dwUserUID_ );

        if( mitUser == m_mapUserInGuild.end() )
        {
            START_LOG( cwarn, L"유저에 해당하는 길드정보가 없음. UserUID : " << dwUserUID_ ) << END_LOG;
            return KCnGuildPtr();
        }
        dwGuildUID = mitUser->second;
    }

    return GetGuild( dwGuildUID );
}

bool KCnGuildManager::GuildChatProc( IN KChatData kChat_ )
{
    KCnGuildPtr spGuild = GetUserGuild( kChat_.m_dwSenderUID );
    if ( spGuild == NULL ) {

        // 유저UID로 못찾은 경우, 길드UID로 다시 검색.
        spGuild = GetGuild( kChat_.m_iParam );
        if ( spGuild == NULL ) {
            START_LOG( cerr, L"채팅을 하려는 길드가 없음" )
                << BUILD_LOG( kChat_.m_dwSenderUID )
                << BUILD_LOGc( kChat_.m_cChatType )
                << BUILD_LOG( kChat_.m_strSenderNick )
                << BUILD_LOG( kChat_.m_strChatMsg ) << END_LOG;
            return false;
        }

        AddUserInGuildInfo( kChat_.m_dwSenderUID, kChat_.m_iParam );
    }

    kChat_.m_cChatType = KChatData::MSG_GUILD;
    kChat_.m_iParam = spGuild->GetUID();

    KECN_CHAT_REQ kPacket;
    kPacket.m_kChatData = kChat_;
    spGuild->SendToAll( KCenterEvent::ECN_CHAT_BROAD, kPacket );

    return true;
}

bool KCnGuildManager::AddGuildNotice( IN const DWORD dwGuildUID_, IN std::vector<KNGuildNotice>& vecNotice_ )
{
    KCnGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if( spGuild == NULL )
    {
        START_LOG( cerr, L"없는 길드에 공지를 추가하려고 함" )
            << BUILD_LOG( dwGuildUID_ )
            << BUILD_LOG( vecNotice_.size() ) << END_LOG;
        return false;
    }

    spGuild->SetNotice( vecNotice_ );
    return true;
}

bool KCnGuildManager::ChangeGuildNotice( IN const DWORD dwGuildUID_, IN const std::vector<KNGuildNotice>& vecNotice_ )
{
    // 길드 공지사항이 변경된 경우, 게임서버가 보냄.
    KCnGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if( spGuild == NULL )
    {
        START_LOG( cerr, L"없는 길드에 공지를 수정하려고 함. GuildUID : " << dwGuildUID_ )
            << BUILD_LOG( vecNotice_.size() ) << END_LOG;
        return false;
    }

    // 길드에 공지 수정.
    spGuild->ModifyNotice( vecNotice_ );

    // 각 게임서버로 정보 전달
    KECN_UPDATE_GUILD_NOTICE_NOT kPacket;
    kPacket.m_dwGuildUID = dwGuildUID_;
    kPacket.m_vecNotice = vecNotice_;

    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_UPDATE_GUILD_NOTICE_NOT, kPacket );

    START_LOG( clog, L"전체 게임서버에 바뀐 길드공지 전달. GuildUID : " << dwGuildUID_ )
        << BUILD_LOG( vecNotice_.size() ) << END_LOG;
    return true;
}

void KCnGuildManager::SendGuildMemberListNot( IN const DWORD dwGuildUID_, IN std::map<DWORD, KNGuildUserInfo>& mapGuildUserList_ )
{
    KECN_GUILD_MEMEBER_LIST_NOT kPacket;
    kPacket.m_dwGuildUID = dwGuildUID_;
    kPacket.m_mapGuildUserList = mapGuildUserList_;

    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_GUILD_MEMEBER_LIST_NOT, kPacket, true );

    START_LOG( clog, L"전체 게임서버에 길드 맴버 리스트 보냄. " )
        << BUILD_LOG( kPacket.m_dwGuildUID )
        << BUILD_LOG( kPacket.m_mapGuildUserList.size() ) << END_LOG;
}

bool KCnGuildManager::ModifyMemberComment( IN const DWORD dwGuildUID_, IN const DWORD dwUserUID_, IN const std::wstring& strUserComment_ )
{
    // 길드원이 자기소개를 변경한 경우, 게임서버가 보냄.
    KCnGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( spGuild == NULL ) {
        START_LOG( cerr, L"해당하는 길드가 없음." )
            << BUILD_LOG( dwGuildUID_ )
            << BUILD_LOG( dwUserUID_ ) << END_LOG;
        return false;
    }

    // 길드에 유저 소개 변경
    LIF( spGuild->ModifyMemberComment( dwUserUID_, strUserComment_ ) );

    // 각 게임서버로 정보 전달
    KECN_UPDATE_GUILD_MEMBER_COMMENT_NOT kPacket;
    kPacket.m_dwGuildUID = dwGuildUID_;
    kPacket.m_dwUserUID = dwUserUID_;
    kPacket.m_strUserComment = strUserComment_;

    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_UPDATE_GUILD_MEMBER_COMMENT_NOT, kPacket, true );

    START_LOG( clog, L"전체 게임서버에 바뀐 길드원 소갯글 전달. " )
        << BUILD_LOG( dwGuildUID_ )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( strUserComment_ ) << END_LOG;
    return true;
}

bool KCnGuildManager::ChangeGuildMemberLevel( IN const DWORD dwGuildUID_, IN const std::map<char,std::set<DWORD> >& mapChangeMemberLevel_ )
{
    // 길드원 맴버레벨이 변경된 경우, 게임서버가 보냄.
    KCnGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( spGuild == NULL ) {
        START_LOG( cerr, L"해당하는 길드가 없음." )
            << BUILD_LOG( dwGuildUID_ )
            << BUILD_LOG( mapChangeMemberLevel_.size() ) << END_LOG;
        return false;
    }

    // 길드에 유저 맴버레벨 변경
    LIF( spGuild->ChangeMemberLevel( mapChangeMemberLevel_ ) );

    // 각 게임서버로 정보 전달
    KECN_CHANGE_GUILD_MEMBER_LEVEL_NOT kPacket;
    kPacket.m_dwGuildUID = dwGuildUID_;
    kPacket.m_mapChangeMemberLevel = mapChangeMemberLevel_;

    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_CHANGE_GUILD_MEMBER_LEVEL_NOT, kPacket );

    START_LOG( clog, L"전체 게임서버에 바뀐 길드원 맴버레벨 정보 전달. " )
        << BUILD_LOG( dwGuildUID_ )
        << BUILD_LOG( mapChangeMemberLevel_.size() ) << END_LOG;
    return true;
}

void KCnGuildManager::BuildGuildPageList()
{
    // Sort Type별로 정렬해서 GuildUID 리스트 작성
    std::vector<DWORD> vecGuildUIDList;

    KLocker lock( m_csGuilds );

    std::sort( m_vecGuild.begin(), m_vecGuild.end(), KSortIDdate() );
    GetGuildUIDList( m_vecGuild, vecGuildUIDList );
    m_mapGuildPage[ST_GUILD_ID_DATE] = vecGuildUIDList;

    std::sort( m_vecGuild.begin(), m_vecGuild.end(), KSortName() );
    GetGuildUIDList( m_vecGuild, vecGuildUIDList );
    m_mapGuildPage[ST_GUILD_NAME] = vecGuildUIDList;

    std::sort( m_vecGuild.begin(), m_vecGuild.end(), KSortGrade() );
    GetGuildUIDList( m_vecGuild, vecGuildUIDList );
    m_mapGuildPage[ST_GUILD_GRADE] = vecGuildUIDList;

    std::sort( m_vecGuild.begin(), m_vecGuild.end(), KSortPoint() );
    GetGuildUIDList( m_vecGuild, vecGuildUIDList );
    m_mapGuildPage[ST_GUILD_POINT] = vecGuildUIDList;
}

void KCnGuildManager::AddNewGuildInfo( IN KNGuildInfo& kInfo_, std::vector<KNGuildNotice>& mapNotice_, IN std::map<DWORD, KNGuildUserInfo>& mapUserList_ )
{
    // 길드정보 추가.
    AddGuild( kInfo_ );
    LIF( AddGuildNotice( kInfo_.m_dwUID, mapNotice_ ) );
    LIF( SetGuildMemberInfo( kInfo_.m_dwUID, mapUserList_ ) );

    START_LOG( clog, L"게임서버에서 새로 만들어진 길드정보 받음. GuildUID : " << kInfo_.m_dwUID )
        << BUILD_LOG( kInfo_.m_strName )
        << BUILD_LOG( kInfo_.m_strMasterNick )
        << BUILD_LOG( mapNotice_.size() )
        << BUILD_LOG( mapUserList_.size() ) << END_LOG;

    // 설정된 길드포인터 받아옴.
    KCnGuildPtr spGuild = GetGuild( kInfo_.m_dwUID );
    if ( !spGuild ) {
        START_LOG( cerr, L"바로 위에서 생성했는데, 존재하지 않는 길드, 길드 ID : " << kInfo_.m_dwUID ) << END_LOG;
        return;
    }

    std::map<DWORD, KNGuildUserInfo>::iterator mit;
    for ( mit = mapUserList_.begin() ; mit != mapUserList_.end() ; ++mit ) {
        AddUserInGuildInfo( mit->first, kInfo_.m_dwUID );
    }

    KNOneGuildData kPacket;
    kPacket.m_kGuildInfo = spGuild->GetGuildInfo();
    spGuild->GetNotice( kPacket.m_vecGuildNotice );
    spGuild->GetMemberList( kPacket.m_mapGuildUserList );

    // 각 게임서버로 새로 추가된 길드의 정보 전달.
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_ADD_NEW_GUILD_NOT, kPacket, true );

    START_LOG( clog, L"각 게임서버로 새로 추가된 길드의 정보 전달. GuildUID : " << kPacket.m_kGuildInfo.m_dwUID )
        << BUILD_LOG( kPacket.m_kGuildInfo.m_strName )
        << BUILD_LOG( kPacket.m_kGuildInfo.m_strMasterNick )
        << BUILD_LOG( kPacket.m_vecGuildNotice.size() )
        << BUILD_LOG( kPacket.m_vecGuildNotice.size() )
        << BUILD_LOG( kPacket.m_mapGuildUserList.size() ) << END_LOG;

    // Page에 신규 길드UID 추가
    AddPageNewGuild( kPacket.m_kGuildInfo.m_dwUID );
}

void KCnGuildManager::QueueingID( unsigned short usEventID_, DWORD dwUID_ )
{
    KIntEventPtr spEvent( new KIntEvent );

    spEvent->m_usEventID    = usEventID_;
    spEvent->m_dwSenderUID  = dwUID_;

    KLocker lock( m_csEventQueue );
    m_queEvent.push_back( spEvent );
}

void KCnGuildManager::DistributeEvent()
{
    std::deque<KIntEventPtr> queEvent;
    {
        KLocker lock( m_csEventQueue );
        m_queEvent.swap( queEvent );
    }

    if ( queEvent.empty() ) return;

    std::deque<KIntEventPtr>::iterator qit;
    for ( qit = queEvent.begin() ; qit != queEvent.end() ; ++qit ) {
        KCnGuildPtr spGuild = GetGuild( (*qit)->m_dwSenderUID );
        if ( !spGuild ) continue;

        KEventPtr spEvent ( new KCenterEvent );
        spEvent->m_usEventID = (*qit)->m_usEventID;
        spEvent->m_kbuff = (*qit)->m_kbuff;

        spGuild->QueueingEvent( spEvent );
    }
}

void KCnGuildManager::SetGuildRankData( IN std::map< DWORD, std::vector<KNGuildRankInfo> >& mapGuildRank_ )
{
    std::map< DWORD, KSerBuffer > mapRankData;
    std::map< DWORD, std::vector<KNGuildRankInfo> >::iterator mit;

    for ( mit = mapGuildRank_.begin() ; mit != mapGuildRank_.end() ; ++mit ) {
        KSerBuffer kBuff;
        KSerializer ks;
        // 시리얼 라이즈
        ks.BeginWriting( &kBuff );
        LIF( ks.Put( mit->second ) );
        ks.EndWriting();

        // 압축.
        kBuff.Compress();

        mapRankData.insert( std::map< DWORD, KSerBuffer >::value_type( mit->first, kBuff ) );
    }

    START_LOG( cerr, L"길드 랭킹 데이터 업데이트. Size : " << mapRankData.size() ) << END_LOG;

    {
        KLocker lock( m_csGuildRank );
        m_mapCompressedRankData.swap( mapRankData );
    }
    SetRankChange( true );
}

void KCnGuildManager::SetRankInfoInGuildList( IN std::map<DWORD,KSimpleGuildRank>& mapRank_ )
{
    std::map< DWORD, KSimpleGuildRank >::iterator mit;
    std::map< DWORD, KNGuildInfo >::iterator mitGuild;

    for ( mit = mapRank_.begin() ; mit != mapRank_.end() ; ++mit ) {
        // 길드에 랭킹데이터 입력
        if ( SetGulidRankInfo( mit->first, mit->second.m_nWeeklyRank, mit->second.m_nMonthlyRank, mit->second.m_nTotalRank ) == false )
            continue;

        START_LOG( clog, L"길드리스트 정보에 랭킹데이터 설정. GuildUID : " << mit->first )
            << BUILD_LOG( mit->second.m_nWeeklyRank )
            << BUILD_LOG( mit->second.m_nMonthlyRank )
            << BUILD_LOG( mit->second.m_nTotalRank ) << END_LOG;
    }

    // 랭킹 데이터 저장.
    SetSimpleGuildRank( mapRank_ );
}

bool KCnGuildManager::SetGulidRankInfo( IN const DWORD dwGuildUID_, IN const int nWeeklyRank_, IN const int nMonthlyRank_, IN const int nTotalRank_ )
{
    KCnGuildPtr spGuild = GetGuild( dwGuildUID_ );
    if ( spGuild == NULL ) {
        START_LOG( cwarn, L"랭킹데이터를 입력할 대상 길드가 없음. GuildUID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    // 길드에 랭킹정보 설정.
    spGuild->SetRank( nWeeklyRank_, nMonthlyRank_, nTotalRank_ );

    START_LOG( clog, L"길드에 랭킹정보 설정함. GuildUID : " << dwGuildUID_ ) << END_LOG;
    return true;
}

void KCnGuildManager::GetGuildRankData( OUT std::map< DWORD, KSerBuffer >& mapGuildRank_ )
{
    mapGuildRank_.clear();
    KLocker lock( m_csGuildRank );
    mapGuildRank_ = m_mapCompressedRankData;
}

void KCnGuildManager::GetGuildRank( IN const DWORD dwGuildUID_, OUT KSimpleGuildRank& kRank_ )
{
    std::map< DWORD, KSimpleGuildRank >::iterator mit;
    std::map< DWORD, KSimpleGuildRank > mapSimpleRank;

    GetSimpleGuildRank( mapSimpleRank );

    mit = mapSimpleRank.find( dwGuildUID_ );
    if ( mit == mapSimpleRank.end() ) {
        START_LOG( cwarn, L"해당 길드의 랭킹 데이터가 없음. GuildUID : " << dwGuildUID_ ) << END_LOG;
        return;
    }

    kRank_ = mit->second;
}

void KCnGuildManager::SendGuildRankData()
{
    std::map< DWORD, KSerBuffer > kPacket;
    GetGuildRankData( kPacket );
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_GUILD_RANK_NOT, kPacket );
}

void KCnGuildManager::MakeGuildMarkName( IN const DWORD dwGuildUID_, IN const int nRevision_,
                                      IN const std::wstring& strExt_, OUT std::wstring& strFileName_ )
{
    if ( nRevision_ != 0 ) {
        std::wstringstream stm;
        stm << dwGuildUID_ << L"_" << nRevision_ << L"." << strExt_;
        strFileName_ = stm.str();
    }
    else {
        KLocker lock( m_csConfig );
        strFileName_ = m_kGuildConfig.m_strDefaultMarkName;
    }

    boost::to_lower( strFileName_ );
}

bool KCnGuildManager::LoadScript()
{
    KLuaManager kLuaMng;
    KGuildConfig kConfig;
    std::string strTmp;
    DWORD dwChangeGuildTickGap;
    DWORD dwUpdateGuildCount;

    _JIF( kLuaMng.DoFile( "InitGuildConfig.lua" ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MarkServerURL", strTmp ) == S_OK, return false );
    kConfig.m_strMarkServerURL = KncUtil::toWideString( strTmp );

    _JIF( kLuaMng.BeginTable( "MarkImageExt" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        if ( kLuaMng.GetValue( i, strTmp ) != S_OK )
            break;
        kConfig.m_setExt.insert( KncUtil::toWideString( strTmp ) );
    }
    _JIF( kLuaMng.EndTable() == S_OK, return false );

    _JIF( kLuaMng.GetValue( "MarkDefaultName", strTmp ) == S_OK, return false );
    kConfig.m_strDefaultMarkName = KncUtil::toWideString( strTmp );

    _JIF( kLuaMng.GetValue( "ImageBuffMinSize", kConfig.m_prMarkSizeLimit.first ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "ImageBuffMaxSize", kConfig.m_prMarkSizeLimit.second ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GuildMarkUploadItemID", kConfig.m_dwMarkUploadItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GuildMarkUploadItemFactor", kConfig.m_nMarkUploadItemFactor ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GuildMarkUploadItemType", kConfig.m_nMarkUploadItemType ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GuildMarkUploadCashItemID", kConfig.m_dwMarkUploadCashItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GuildChangeNameItemID", kConfig.m_dwChangGuildNameItemID ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GuildPageSize", kConfig.m_nGuildPageSize ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GuildNameMinLength", kConfig.m_prGuildNameLength.first ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GuildNameMaxLength", kConfig.m_prGuildNameLength.second ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GuildMasterMinLv", kConfig.m_dwGuildMasterMinLevel ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GuildURLMinLength", kConfig.m_prGuildURLLength.first ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GuildURLMaxLength", kConfig.m_prGuildURLLength.second ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GuildCreateGP", kConfig.m_nGuildCreateGP) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "OfficailGuildMemberNum", kConfig.m_nOfficailGuildMemberNum ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "NewbieGuildRemoveTerm", kConfig.m_nNewbieGuildRemoveTerm ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "KeywordMinLength", kConfig.m_nKeywordMinLength ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "JoinerRemoveTerm", kConfig.m_nJoinerRemoveTerm ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "AllowUnicode", kConfig.m_bAllowUnicode ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "GuildInfoClearTerm", kConfig.m_dwGuildInfoClearTerm ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MemberListClearTerm", kConfig.m_dwMemberListClearTerm ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "MaxPageSize", kConfig.m_nMaxPageSize ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "UpdatePointTerm", kConfig.m_dwUpdatePointTerm ) == S_OK, return false );
    _JIF( LoadUnicode( kLuaMng, kConfig.m_vecUnicode ), return false );
    _JIF( LoadGuildLvPoint( kLuaMng, kConfig.m_mapGuildLvPoint ), return false );
    _JIF( LoadGuildLvAdvantage( kLuaMng, kConfig.m_mapGuildLvAdvantage ), return false );
    _JIF( LoadContributePointAdventageLv( kLuaMng, kConfig.m_mapContributeAdventageLv ), return false );
    _JIF( LoadChannelGradeByBattlePoint( kLuaMng, kConfig.m_mapChannelGrade ), return false );
    _JIF( LoadGuildPoint( kLuaMng, kConfig.m_mapGuildPoint ), return false );
    
    _JIF( kLuaMng.GetValue( "GuildUpdateCount", dwUpdateGuildCount ) == S_OK, return false );
    _JIF( kLuaMng.GetValue( "ChangeGuildTickGap", dwChangeGuildTickGap ) == S_OK, return false );

    START_LOG( cerr, L"길드 관련 설정 읽기 완료." ) << END_LOG;

    {
        KLocker lock( m_csConfig );
        m_kGuildConfig = kConfig;
        m_dwChangeGuildTickGap = dwChangeGuildTickGap;
        m_dwUpdateGuildCount = dwUpdateGuildCount;
        m_bConfigChanged = true;
    }
    m_kGuildClearTimer.SetInterval( kConfig.m_dwGuildInfoClearTerm );
    return true;
}

void KCnGuildManager::GetGuildConfig( OUT KGuildConfig& kConfig_ )
{
    KLocker lock( m_csConfig );
    kConfig_ = m_kGuildConfig;
}

bool KCnGuildManager::IsConfigChanged()
{
    bool bRet = false;
    KLocker lock( m_csConfig );
    if ( m_bConfigChanged ) {
        bRet = true;
        m_bConfigChanged = false;
    }

    return bRet;
}

void KCnGuildManager::PrintGuild( DWORD dwGuildUID_ )
{
    KNGuildInfo kGuildInfo;
    kGuildInfo.Init();

    if( !GetGuildInfo( dwGuildUID_, kGuildInfo ) ) {
        std::cout << "Not exist GuildUID : " << dwGuildUID_ << std::endl;
        return;
    }

    START_LOG( cerr,  L"----------------- Print Guild Info -------------------" << dbg::endl
              << L" Guild UID: " << kGuildInfo.m_dwUID << dbg::endl
              << L" Name :" << kGuildInfo.m_strName << dbg::endl
              << L" m_ucMarkStatus : " << (int)kGuildInfo.m_ucMarkStatus << dbg::endl
              << L" m_nMarkRevision : " << kGuildInfo.m_nMarkRevision << dbg::endl
              << L" m_strFileName : " << kGuildInfo.m_strFileName << dbg::endl
              << L" m_strComment : " << kGuildInfo.m_strComment << dbg::endl
              << L" m_ucGrade : " << (int)kGuildInfo.m_ucGrade << dbg::endl
              << L" m_kFoundingDate (Y-M-D) : " << kGuildInfo.m_kFoundingDate.Str() << dbg::endl
              << L" m_strMasterNick : " << kGuildInfo.m_strMasterNick << dbg::endl
              << L" m_strURL : " << kGuildInfo.m_strURL << dbg::endl
              << L" m_dwExp : " << kGuildInfo.m_dwExp << dbg::endl
              << L" m_dwPoint : " << kGuildInfo.m_dwPoint << dbg::endl
              << L" m_nNumMembers : " << kGuildInfo.m_nNumMembers << dbg::endl
              << L" m_nTotalRank : " << kGuildInfo.m_nTotalRank << dbg::endl
              << L" m_nMonthlyRank : " << kGuildInfo.m_nMonthlyRank << dbg::endl
              << L" m_nWeeklyRank : " << kGuildInfo.m_nWeeklyRank << dbg::endl
              << L" m_ucJoinSetting : " << (int)kGuildInfo.m_ucJoinSetting << dbg::endl
              << L" m_GuildBattlePoint" << kGuildInfo.m_GuildBattlePoint << dbg::endl
              << L"---------------------- End Info ----------------------" ) << END_LOG;

    // 길드 유저리스트 정보 출력
    PrintGuildUserListInfo( dwGuildUID_ );
}

void KCnGuildManager::PrintUser( DWORD dwUserUID_ )
{
    DWORD dwGuildUID = GetUserGuildID( dwUserUID_ );
    KNGuildUserInfo kUserInfo;
    kUserInfo.Init();

    if ( dwGuildUID == 0 ) {
        START_LOG( cerr, L"Not Guild User. UserUID : " << dwUserUID_  ) << END_LOG;
        return;
    }

    KCnGuildPtr spGuild = GetGuild( dwGuildUID );
    if ( !spGuild ) {
        START_LOG( cerr, L"Not exist Guild Info. UserUID : " << dwUserUID_ << L", GuildUID : " << dwGuildUID  ) << END_LOG;
        return;
    }

    if ( spGuild->GetUserInfo( dwUserUID_, kUserInfo ) == false ) {
        START_LOG( cerr, L"Not exist GuildUser Info. UserUID : " << dwUserUID_ << L", GuildUID : " << dwGuildUID  ) << END_LOG;
        return;
    }

    START_LOG( cerr, L"-------------- Print Guild User Info -----------------" << dbg::endl
              << L" m_dwUserUID : " << kUserInfo.m_dwUserUID << dbg::endl
              << L" m_strLogin : " << kUserInfo.m_strLogin << dbg::endl
              << L" m_strNickName : " << kUserInfo.m_strNickName << dbg::endl
              << L" m_dwGuildUID : " << kUserInfo.m_dwGuildUID << dbg::endl
              << L" m_cMemberLevel : " << (int)kUserInfo.m_cMemberLevel << dbg::endl
              << L" m_ucCharacterGrade : " << (int)kUserInfo.m_ucCharacterGrade << dbg::endl
              << L" m_dwContributePoint : " << kUserInfo.m_dwContributePoint << dbg::endl
              << L" m_nWinCount : " << kUserInfo.m_nWinCount << dbg::endl
              << L" m_nLoseCount : " << kUserInfo.m_nLoseCount << dbg::endl
              << L" m_strMyComment : " << kUserInfo.m_strMyComment << dbg::endl
              << L" m_kJoinDate (Y-M-D) : " << kUserInfo.m_kJoinDate.Str() << dbg::endl
              << L" m_bIsConnect : " << kUserInfo.m_bIsConnect << dbg::endl
              << L" m_cServerUID : " << (int)kUserInfo.m_cServerUID << dbg::endl
              << L" m_strLocation : " << kUserInfo.m_strLocation << dbg::endl
              << L" m_kLastLoginDate (Y-M-D) : " << kUserInfo.m_kLastLoginDate.Str() << dbg::endl
              << L"---------------------- End Info ----------------------" ) << END_LOG;
}

DWORD KCnGuildManager::GetUserGuildID( IN const DWORD dwUserUID_ )
{
    KLocker lock( m_csUserMap );

    std::map<DWORD,DWORD>::iterator mit = m_mapUserInGuild.find( dwUserUID_ );
    if ( mit == m_mapUserInGuild.end() ) return 0;
    return mit->second;
}

bool KCnGuildManager::GetGuildInfo( IN const DWORD dwGuildUID_, OUT KNGuildInfo& kGuildInfo_ )
{
    KCnGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( !spGuild ) {
        START_LOG( cerr, L"없는 길드ID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    kGuildInfo_ = spGuild->GetGuildInfo();
    return true;
}

bool KCnGuildManager::LoadUnicode( IN KLuaManager& kLuaMng_, OUT std::vector< std::pair<DWORD,DWORD> >& vecAllowedUnicode_ )
{
    vecAllowedUnicode_.clear();
    _JIF( kLuaMng_.BeginTable( "Unicode" ) == S_OK, return false );
    for( int i = 1 ; ; ++i )
    {
        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        std::pair<DWORD,DWORD> prData;
        _JIF( kLuaMng_.GetValue( 1, prData.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( 2, prData.second ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        vecAllowedUnicode_.push_back( prData );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

void KCnGuildManager::CheckRankInfo( IN OUT KNGuildInfo& kInfo_ )
{
    KSimpleGuildRank kRank;

    if ( kInfo_.m_nTotalRank != 0 && kInfo_.m_nMonthlyRank != 0 && kInfo_.m_nWeeklyRank != 0 ) {
        // 길드 랭킹데이터 정상적으로 설정되어 있음.
        return;
    }

    // 랭킹 데이터가 제대로 설정되어 있지 않은 경우.
    GetGuildRank( kInfo_.m_dwUID, kRank );

    // 랭킹 설정
    kInfo_.m_nWeeklyRank = kRank.m_nWeeklyRank;
    kInfo_.m_nMonthlyRank = kRank.m_nMonthlyRank;
    kInfo_.m_nTotalRank = kRank.m_nTotalRank;

    START_LOG( clog, L"길드랭킹 설정됨. GuildUID : " << kInfo_.m_dwUID )
        << BUILD_LOG( kInfo_.m_nWeeklyRank )
        << BUILD_LOG( kInfo_.m_nMonthlyRank )
        << BUILD_LOG( kInfo_.m_nTotalRank ) << END_LOG;

    // 변경된 데이터가 있으면 해당 길드에도 랭킹 데이터 전송
    KSimpleGuildRank kPacket;
    kPacket.m_dwGuildUID = kInfo_.m_dwUID;
    kPacket.m_nWeeklyRank = kInfo_.m_nWeeklyRank;
    kPacket.m_nMonthlyRank = kInfo_.m_nMonthlyRank;
    kPacket.m_nTotalRank = kInfo_.m_nTotalRank;

    QueueingEvent( KCenterEvent::ECN_SET_GUILD_RANK_NOT, kPacket.m_dwGuildUID, kPacket );
}

void KCnGuildManager::ClearGuildInfoCheck()
{
    std::map<DWORD,KCnGuildPtr>::iterator mit;
    std::vector<KCnGuildPtr>::iterator vit;

    KLocker lock( m_csGuilds );

    START_LOG( clog, L"before clear. guild list size : " << m_vecGuild.size() << L", " << m_mapGuild.size() ) << END_LOG;

    vit = m_vecGuild.begin();
    while ( vit != m_vecGuild.end() ) {

        if ( m_vecGuild.empty() ) {
            break;
        }

        // 접속중인 길드원이 1명도 없으면 길드 정보 삭제.
        if ( (*vit)->GetLiveUserCount() <= 0 ) {
            DWORD dwGuildUID = (*vit)->GetGuildInfo().m_dwUID;
            //START_LOG( cwarn, L"Erase Guild Info. GuildUID : " << dwGuildUID ) << END_LOG;
            vit = m_vecGuild.erase( vit );

            mit = m_mapGuild.find( dwGuildUID );
            if ( mit != m_mapGuild.end() ) {
                m_mapGuild.erase( mit );
            }
            continue;
        }

        //START_LOG( clog, L"GuildUID : " << (*vit)->GetGuildInfo().m_dwUID << L", OnlineUser Size : " << (*vit)->GetLiveUserCount() ) << END_LOG;
        ++vit;
    }

    START_LOG( clog, L"after clear. guild list size : " << m_vecGuild.size() << L", " << m_mapGuild.size() ) << END_LOG;
}

void KCnGuildManager::PrintGuildUserListInfo( IN const DWORD dwGuildUID_ )
{
    KCnGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( !spGuild ) {
        START_LOG( cerr, L"없는 길드ID : " << dwGuildUID_ ) << END_LOG;
        return;
    }

    spGuild->PrintUserListInfo();
}

void KCnGuildManager::GetGuildUIDList( IN const std::vector<KCnGuildPtr>& vecGuild_, OUT std::vector<DWORD>& vecGuildUID_ )
{
    // 길드정보에서 GuildUID만 뽑아 vector에 담는다.(게임서버 전달용)
    vecGuildUID_.clear();

    std::vector<KCnGuildPtr>::const_iterator cvit;
    for ( cvit = vecGuild_.begin() ; cvit != vecGuild_.end() ; ++cvit ) {
        vecGuildUID_.push_back( (*cvit)->GetGuildUID() );
    }
}

void KCnGuildManager::SendGuildPageNot()
{
    KECN_GUILD_PAGE_NOT kPacket;
    std::map<UCHAR, KSerBuffer > mapPageInfo;
    SiKCnGuildManager()->GetGuildPageIDList( mapPageInfo );

    std::map<UCHAR, KSerBuffer >::iterator mit;
    for ( mit = mapPageInfo.begin() ; mit != mapPageInfo.end() ; ++mit ) {
        kPacket.m_ucSortType = mit->first;
        kPacket.m_kPageBuffer = mit->second;

        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_GUILD_PAGE_NOT, kPacket, true );
        START_LOG( clog, L"Page정보 각 게임서버에 전송. SortType : " << kPacket.m_ucSortType ) << END_LOG;
    }
}

void KCnGuildManager::GetGuildPageIDList( OUT std::map<UCHAR,KSerBuffer>& mapData_ )
{
    KLocker lock( m_csGuilds );

    std::map<UCHAR,std::vector<DWORD> >::iterator mit;
    for ( mit = m_mapGuildPage.begin() ; mit != m_mapGuildPage.end() ; ++mit ) {
        KSerBuffer kBuff;
        KSerializer ks;
        // 시리얼 라이즈
        ks.BeginWriting( &kBuff );
        LIF( ks.Put( mit->second ) );
        ks.EndWriting();

        // 압축.
        kBuff.Compress();

        mapData_.insert( std::map< UCHAR, KSerBuffer >::value_type( mit->first, kBuff ) );
    }
}

void KCnGuildManager::AddPageNewGuild( IN const DWORD dwGuildUID_ )
{
    // 페이지에 신규 길드UID만 추가.
    // 페이지Sort 갱신은 좀더 고민해보자.
    KLocker lock( m_csGuilds );
    for ( UCHAR ucType = ST_GUILD_ID_DATE ; ucType < ST_MAX ; ++ucType ) {
        m_mapGuildPage[ ucType ].push_back( dwGuildUID_ );
    }
}

void KCnGuildManager::RemovePageGuild( IN const DWORD dwGuildUID_ )
{
    // 페이지에 삭제된 길드UID 제거
    // 페이지Sort 갱신은 좀더 고민해보자.
    std::map<UCHAR,std::vector<DWORD> >::iterator mit;
    std::vector<DWORD>::iterator vit;

    KLocker lock( m_csGuilds );
    for ( mit = m_mapGuildPage.begin() ; mit != m_mapGuildPage.end() ; ++mit ) {
        vit = std::find( mit->second.begin(), mit->second.end(), dwGuildUID_ );
        if ( vit != mit->second.end() ) {
            mit->second.erase( vit );
            START_LOG( clog, L"Page에서 GuildUID 제거. SortType : " << (UINT)mit->first << L",GuildUID : " << dwGuildUID_ ) << END_LOG;
        }
    }
}

void KCnGuildManager::SendUpdateGuildListReqToDB()
{
    // 길드 전체정보 DB에 요청.
    SiKCenterDBMgr()->QueueingID( KCenterEvent::DB_ECN_UPDATE_GUILD_LIST_REQ, L"", 0 );
}

void KCnGuildManager::DelUserInGuildInfo( IN const DWORD dwUserUID_ )
{
    // 유저UID별 길드UID정보 제거
    KLocker lock( m_csUserMap );
    m_mapUserInGuild.erase( dwUserUID_ );
}

void KCnGuildManager::AddUserInGuildInfo( IN const DWORD dwUserUID_, IN const DWORD dwGuildUID_ )
{
    KLocker lock( m_csUserMap );
    m_mapUserInGuild[dwUserUID_] = dwGuildUID_;
}

bool KCnGuildManager::LoadGuildStoreTabUserPoint( IN KLuaManager& kLuaMng_, OUT std::map<int,DWORD>& mapData_ )
{
    mapData_.clear();

    _JIF( kLuaMng_.BeginTable( "GuildStoreTabUserPoint" ) == S_OK, return false );
    for(int i = 1; ; i += 2 ) {
        int nTab = 0;
        DWORD dwPoint = 0;

        if ( kLuaMng_.GetValue( i, nTab ) != S_OK ) break;
        if ( kLuaMng_.GetValue( i+1, dwPoint ) != S_OK ) break;

        _JIF( mapData_.insert( std::make_pair( nTab, dwPoint ) ).second, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnGuildManager::LoadGuildLvPoint( IN KLuaManager& kLuaMng_, OUT std::map<USHORT,int>& mapData_ )
{
    mapData_.clear();

    _JIF( kLuaMng_.BeginTable( "GuildLvPoint" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        USHORT usGuildLv = 0;
        int nGPoint = 0;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        if ( kLuaMng_.GetValue( 1, usGuildLv ) != S_OK ) break;
        if ( kLuaMng_.GetValue( 2, nGPoint ) != S_OK ) break;
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( mapData_.insert( std::make_pair( usGuildLv, nGPoint ) ).second, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnGuildManager::LoadGuildLvAdvantage( IN KLuaManager& kLuaMng_, OUT std::map<USHORT,std::pair<int,float> >& mapData_ )
{
    mapData_.clear();

    _JIF( kLuaMng_.BeginTable( "GuildLvAdvantage" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        USHORT usGuildLv = 0;
        std::pair<int,float> prAdvantage;
        prAdvantage.first = 0;
        prAdvantage.second = 0;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        if ( kLuaMng_.GetValue( 1, usGuildLv ) != S_OK ) break;
        if ( kLuaMng_.GetValue( 2, prAdvantage.first ) != S_OK ) break;
        if ( kLuaMng_.GetValue( 3, prAdvantage.second ) != S_OK ) break;
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( mapData_.insert( std::make_pair( usGuildLv, prAdvantage ) ).second, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnGuildManager::LoadContributePointAdventageLv( IN KLuaManager& kLuaMng_, OUT std::map<int,USHORT>& mapData_ )
{
    mapData_.clear();

    _JIF( kLuaMng_.BeginTable( "ContributePointAdventageLv" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        int nContributePoint = 0;
        USHORT usAdvantageGuildLv = 0;

        if ( kLuaMng_.BeginTable( i ) != S_OK ) break;
        if ( kLuaMng_.GetValue( 1, nContributePoint ) != S_OK ) break;
        if ( kLuaMng_.GetValue( 2, usAdvantageGuildLv ) != S_OK ) break;
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        _JIF( mapData_.insert( std::make_pair( nContributePoint, usAdvantageGuildLv ) ).second, return false );
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );

    return true;
}

bool KCnGuildManager::LoadChannelGradeByBattlePoint( IN KLuaManager& kLuaMng_, OUT std::map< char, std::pair< DWORD, DWORD > >& mapData_ )
{
    mapData_.clear();

    _JIF( kLuaMng_.BeginTable( "ChannelGrade" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        int type;
        std::pair< DWORD, DWORD > prBPoint;
        _JIF( kLuaMng_.GetValue( "ChannelType", type ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "BeginPoint", prBPoint.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "EndPoint", prBPoint.second ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( !mapData_.insert( std::make_pair( static_cast<char>(type), prBPoint ) ).second ) {
            START_LOG( cerr, L"중복된 길드 채널 정보가 있음. type : " << static_cast<int>(type) ) << END_LOG;
        }
        
    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

bool KCnGuildManager::LoadGuildPoint( IN KLuaManager& kLuaMng_, std::map< int, std::pair< int, int > >& mapData_ )
{
    mapData_.clear();

    _JIF( kLuaMng_.BeginTable( "GuildPointReward" ) == S_OK, return false );
    for(int i = 1; ; ++i ) {
        if( kLuaMng_.BeginTable( i ) != S_OK ) break;

        int type;
        std::pair< int, int > prWinLose;
        _JIF( kLuaMng_.GetValue( "Type", type ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Win", prWinLose.first ) == S_OK, return false );
        _JIF( kLuaMng_.GetValue( "Lose", prWinLose.second ) == S_OK, return false );
        _JIF( kLuaMng_.EndTable() == S_OK, return false );

        if( !mapData_.insert( std::make_pair( type, prWinLose ) ).second ) {
            START_LOG( cerr, L"중복된 길드 포인트 혜택 정보가 있음. type : " << type ) << END_LOG;
        }

    }
    _JIF( kLuaMng_.EndTable() == S_OK, return false );
    return true;
}

void KCnGuildManager::UpdateChangeGuildInfo( IN const GCGuildUID& guildUID_, IN KGuildPointBroad& kGuildPointBroad_ )
{
    std::map<GCGuildUID, KGuildPointBroad>::iterator mit;

    KCnGuildPtr spGuild = GetGuild( guildUID_ );
    if ( !spGuild ) {
        START_LOG( cerr, L"Not exist Guild Info. GuildUID : " << guildUID_ )
            <<END_LOG;
        return;
    }

    KNGuildInfo kInfo;
    kInfo = spGuild->GetGuildInfo(); // 길드정보 받아오기

    // 최종 길드 포인트 정보로 갱신.
    kGuildPointBroad_.m_dwTotalPoint = kInfo.m_dwPoint;
    kGuildPointBroad_.m_dwBattlePoint = kInfo.m_GuildBattlePoint;

    KLocker lock( m_csChangeGuild );
    mit = m_mapChangePointGuild.find( guildUID_ );
    if( mit == m_mapChangePointGuild.end() ) {
        m_mapChangePointGuild[guildUID_] = kGuildPointBroad_;
    } else {
        mit->second = kGuildPointBroad_;
    }

    // 변경된 길드 size 체크.
    if( m_dwUpdateGuildCount <= GetChangePointGuildCount() ) {
        std::map<GCGuildUID, KGuildPointBroad> mapChangePointGuild;
        GetChangeGuildInfo( mapChangePointGuild );
        SendChangeGuildInfo( mapChangePointGuild );
    }
}

void KCnGuildManager::GetChangeGuildInfo( OUT std::map<GCGuildUID, KGuildPointBroad>& mapChangePointGuild_ )
{
    mapChangePointGuild_.clear();

    KLocker lock( m_csChangeGuild );
    mapChangePointGuild_.swap( m_mapChangePointGuild );
}

void KCnGuildManager::SendChangeGuildInfo( IN std::map<GCGuildUID, KGuildPointBroad>& mapChangePointGuild_ )
{
    KECN_UPDATE_GUILD_POINT_BROAD kPacket;
    kPacket = mapChangePointGuild_;
    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_UPDATE_GUILD_POINT_BROAD, kPacket );
    m_dwChangeGuildLastTick = ::GetTickCount();
}

void KCnGuildManager::UpdateGuildPointTick()
{
    if ( ::GetTickCount() - m_dwChangeGuildLastTick < m_dwChangeGuildTickGap ) {
        return;
    }

    if( GetChangePointGuildCount() > 0 ) { // 1분 Tick을 돌면서 길드 배틀포인터 데이터 게임서버 broadcast.
        std::map<GCGuildUID, KGuildPointBroad> mapChangePointGuild;
        GetChangeGuildInfo( mapChangePointGuild );
        SendChangeGuildInfo( mapChangePointGuild );
    }

    m_dwChangeGuildLastTick = ::GetTickCount();
}