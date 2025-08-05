#include "Center.h"
#include "CnGuild.h"
#include <boost/bind.hpp>
#include "CnGuildManager.h"
#include "CenterDBThread.h"
#include "CenterSimLayer.h"
#include "Log4.h"

NiImplementRTTI( KCnGuild, KPerformer );
#define EVENT_TYPE KCenterEvent
#define CLASS_TYPE KCnGuild

//FILE_NAME_FOR_LOG

namespace
{
    class KSortJoinDate
    {
    public:
        bool operator() ( const KNGuildUserInfo& a, const KNGuildUserInfo& b )
        {
            time_t tmA = KncUtil::TimeToInt( CTime( a.m_kJoinDate.m_sYear, a.m_kJoinDate.m_cMonth, a.m_kJoinDate.m_cDay, 0, 0, 0 ) );
            time_t tmB = KncUtil::TimeToInt( CTime( b.m_kJoinDate.m_sYear, b.m_kJoinDate.m_cMonth, b.m_kJoinDate.m_cDay, 0, 0, 0 ) );

            if ( tmA == tmB ) return false;
            return tmA < tmB;
        }
    };
}

ImplToStringW( KCnGuild )
{
    return START_TOSTRING_PARENTW( KPerformer );
}

KCnGuild::KCnGuild(void)
:m_kMinTimer(1000*60)
,m_kMemberClearTimer(1000 * 60 * 5)
,m_kPointTimer( 1000 * 60 * 3 )
,m_nMaxMemberNum(200)
,m_nMaxJoinerNum(200)
,m_dwGuildMasterUID(0)
,m_nOfficialGuildMemberNum(5)
,m_nJoinerRemoveTerm(15)
,m_nGuildRemoveTerm(30)
,m_kHourTimer(1000*60*60)
{
    m_kNGuildInfo.Init();

    // 최초 검사할 수 있도록 하루전 날짜로 설정.
    CTime tDate = CTime::GetCurrentTime();
    CTimeSpan tSpan(1,0,0,0);
    tDate -= tSpan;
    m_kLastDayCheckDate.SetDate( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0 );

    // 길드 가입신청자 자동 제거일 구하기
    SetRejectJoinerDate();
}

KCnGuild::~KCnGuild(void)
{
}

void KCnGuild::Tick()
{
    KPerformer::Tick();
    if ( m_kMinTimer.CheckTime() ) {
        DayCheck();
        AutoUpgradeGuildGrade();
        IntervalTick();
    }

    if ( m_kMemberClearTimer.CheckTime() ) {
        // 맴버리스트 클리어 체크
        MemberListClearCheck();
    }
/*
    if ( m_kPointTimer.CheckTime() ) {
        UpdatePointTick();
    }
*/
}

void KCnGuild::IntervalTick()
{
    if ( GetLiveUserCount() <= 0 ) {
        if ( false == m_kHourTimer.CheckTime() ) {
            return;
        }
    }

    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_GUILD_MARK_INFO_UPDATE_REQ, L"", GetUID() );

}

void KCnGuild::UpdatePointTick()
{
    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_GUILD_POINT_UPDATE_REQ, L"", GetUID() );
}

void KCnGuild::DayCheck()
{
    CTime tDate = CTime::GetCurrentTime();
    KSimpleDate kToday( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0 );

    // 마지막 검사 날짜와 현재 날짜가 다른가?
    if ( m_kLastDayCheckDate == kToday ) {
        return;
    }

    // 마지막 검사 날짜 = 현재 날짜
    m_kLastDayCheckDate.SetDate( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0  );

    // 길드가입자 신청기간 체크하여 자동 거절 처리
    SetRejectJoinerDate();
    AutoRejectJoinerCheck();
};

void KCnGuild::MemberListClearCheck()
{
    if ( GetOnlineUserCount() > 0 || GetJoinerOnlineCount() > 0 ) {
        START_LOG( clog, L"접속중인 길드원 있음. GuildUID : " << m_kNGuildInfo.m_dwUID )
            << BUILD_LOG( GetOnlineUserCount() )
            << BUILD_LOG( GetJoinerOnlineCount() ) << END_LOG;
        return;
    }

    // 맴버리스트 삭제
    {
        KLocker lock( m_csJoiner );
        m_mapOnlineUsers.clear();
        m_mapOfflineUsers.clear();
        m_mapJoiner.clear();
        m_vecJoinerByJoinDate.clear();
    }
    // 충분한 검증이 끝나면 그때 로그레벨 clog2 로 변경할 것.
    START_LOG( cwarn, L"접속유저가 없는 길드의 맴버리스트 초기화함.. GuildUID : " << m_kNGuildInfo.m_dwUID ) << END_LOG;
}

void KCnGuild::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );
    switch( spEvent_->m_usEventID ) {
       _CASE( ECN_UPDATE_GUILD_NAME_NOT, std::wstring );
        CASE( ECN_JOIN_GUILD_ACK );
       _CASE( ECN_UPDATE_GUILD_URL_NOT, KNGuildURL );
       _CASE( ECN_GUILD_MARK_INFO_UPDATE_ACK, KNGuildMarkInfo );
        CASE( ECN_CANCEL_JOIN_GUILD_ACK );
        CASE( ECN_CHANGE_GUILD_MEMBER_LEVEL_ACK );
        CASE( ECN_SELF_DRUMOUT_GUILD_ACK );
        CASE( ECN_ACCEPT_GUILD_JOINER_ACK );
        CASE( ECN_REJECT_GUILD_JOINER_ACK );
       _CASE( ECN_CHANGE_GUILD_JOIN_SETTING_ACK, KCnNGuildJoinPolicy );
       _CASE( ECN_UPGRADE_GUILD_GRADE_ACK, int );
        CASE( ECN_UPGRADE_GUILD_GRADE_GIFT_ACK );
       _CASE( ECN_SET_GUILD_RANK_NOT, KSimpleGuildRank );
       _CASE( ECN_UPDATE_GUILD_POINT_REQ, KGuildPointBroad );
        CASE_NOPARAM( ECN_NEWBIE_GUILD_DESTROY_CHECK_NOT );
       _CASE( ECN_UPDATE_GUILD_MEMBER_NICK_NOT, KGuildUserName );
       _CASE( ECN_GUILD_MEMBER_LEVELUP_NOT, KGuildUserCharacter );
       _CASE( ECN_DB_GUILD_POINT_UPDATE_ACK, KGuildPointBroad );

        default:
            START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString() ) << END_LOG;
    }
}

void KCnGuild::SetGuildInfo( IN KNGuildInfo& kInfo_ )
{
    SetUID( (DWORD)kInfo_.m_dwUID );
    SetName( kInfo_.m_strName );
    m_kNGuildInfo = kInfo_;

    // 최초 정보 로드되었을때도
    // 신규길드인데, 정식길드가 될 조건이 되는지 확인.
    AutoUpgradeGuildGrade();
    // 신생길드 삭제날짜 설정
    SetNewbieGuildRemoveDate( kInfo_ );
    // 삭제 체크
    DayCheck();
}

bool KCnGuild::AddMember( IN KNGuildUserInfo& kUserInfo_ )
{
    KLocker lock( m_csJoiner );

    // 신규 유저 추가(신규유저는 무조건 오프라인 유저로 시작하고, 접속처리는 로컬에서 진행한다)
    if ( m_mapOfflineUsers.insert( std::make_pair(kUserInfo_.m_dwUserUID, kUserInfo_) ).second == false ) {
        START_LOG( cwarn, L"이미 맴버 리스트에 있는 유저. GuildUID : " << m_kNGuildInfo.m_dwUID )
            << BUILD_LOG( kUserInfo_.m_dwUserUID )
            << BUILD_LOG( kUserInfo_.m_strLogin ) << END_LOG;
    }

    // 길드맴버수 갱신
    m_kNGuildInfo.m_nNumMembers = GetMemberCount();
    return true;
}

bool KCnGuild::AddJoiner( IN KNGuildUserInfo& kUserInfo_ )
{
    KLocker lock( m_csJoiner );

    // 가입대기자 추가
    if ( m_mapJoiner.insert( std::make_pair(kUserInfo_.m_dwUserUID, kUserInfo_) ).second == false ) {
        // 이미 존재하는 유저
        return false;
    }

    m_vecJoinerByJoinDate.push_back( kUserInfo_ );
    return true;
}

bool KCnGuild::RemoveMember( IN const DWORD dwUserUID_ )
{
    if ( RemoveOnlineUser( dwUserUID_ ) == true ) return true;
    if ( RemoveOfflineUser( dwUserUID_ ) == true ) return true;

    START_LOG( cerr, L"삭제대상 정식길드원이 없음. UserUID : " << dwUserUID_ ) << END_LOG;
    return false;
}

bool KCnGuild::RemoveOnlineUser( IN const DWORD dwUserUID_ )
{
    char cServerUID = 0;

    {
        KLocker lock( m_csJoiner );

        std::map< DWORD, KNGuildUserInfo >::iterator mit;
        mit = m_mapOnlineUsers.find( dwUserUID_ );
        if ( mit == m_mapOnlineUsers.end() ) {
            START_LOG( cerr, L"삭제대상 온라인유저가 없음. UserUID : " << dwUserUID_ ) << END_LOG;
            return false;
        }
        cServerUID = mit->second.m_cServerUID;

        m_mapOnlineUsers.erase( mit );
        START_LOG( clog, L"온라인유저 제거함. UserUID : " << dwUserUID_ ) << END_LOG;

        // 길드맴버수 갱신
        m_kNGuildInfo.m_nNumMembers = GetMemberCount();
    }
    // 서버 정보에서 유저제거(유저카운트 감소)
    RemoveServer( cServerUID );

    return true;
}

bool KCnGuild::RemoveOfflineUser( IN const DWORD dwUserUID_ )
{
    KLocker lock( m_csJoiner );
    
    std::map< DWORD, KNGuildUserInfo >::iterator mit;
    mit = m_mapOfflineUsers.find( dwUserUID_ );
    if ( mit == m_mapOfflineUsers.end() ) {
        START_LOG( cerr, L"삭제대상 오프라인유저가 없음. UserUID : " << dwUserUID_ ) << END_LOG;
        return false;
    }

    m_mapOfflineUsers.erase( mit );
    START_LOG( clog, L"오프라인유저 제거함. UserUID : " << dwUserUID_ ) << END_LOG;

    // 길드맴버수 갱신
    m_kNGuildInfo.m_nNumMembers = GetMemberCount();
    return true;
}

bool KCnGuild::RemoveJoiner( IN const DWORD dwUserUID_ )
{
    std::map< DWORD, KNGuildUserInfo >::iterator mit;
    std::vector< KNGuildUserInfo >::iterator vit;
    char cServerUID = -1;

    {
        KLocker lock( m_csJoiner );

        mit = m_mapJoiner.find( dwUserUID_ );
        if ( mit == m_mapJoiner.end() ) {
            START_LOG( cerr, L"삭제대상 가입대기자가 없음. UserUID : " << dwUserUID_ ) << END_LOG;
            return false;
        }

        cServerUID = mit->second.m_cServerUID;

        m_mapJoiner.erase( mit );
        RemoveJoinerVector( dwUserUID_ );
    }

    // 서버 정보에서 유저제거(유저카운트 감소)
    if ( cServerUID != -1 ) {
        RemoveServer( cServerUID );
    }

    START_LOG( clog, L"가입대기자 제거함. UserUID : " << dwUserUID_ << L", ServerUID : " << (int)cServerUID ) << END_LOG;
    return true;
}

void KCnGuild::SetOnline( IN KNGuildUserInfo& kUserInfo_, IN KCenterPtr spCenter_ )
{
    // 오프라인 유저가 온라인 유저가 될때, 정보갱신 처리
    std::map<DWORD,KNGuildUserInfo>::iterator mit;

    {
        KLocker lock( m_csJoiner );

        // 유저 리스트 검색
        if ( kUserInfo_.m_cMemberLevel == KNGuildUserInfo::GL_JOINER ) {
            // 가입대기자의 경우.
            mit = m_mapJoiner.find( kUserInfo_.m_dwUserUID );
            JIF( mit != m_mapJoiner.end() );

            // 정보갱신만 해준다.
            mit->second = kUserInfo_;

        } 
        else {
            // 정식 길드원의 경우,

            // 오프라인->온라인으로 처리
            mit = m_mapOfflineUsers.find( kUserInfo_.m_dwUserUID );
            if ( mit != m_mapOfflineUsers.end() ) {
                // 오프라인 리스트에 있으면, 목록에서 제거
                m_mapOfflineUsers.erase( mit );
            }
            // 온라인 리스트에 추가.
            kUserInfo_.m_bIsConnect = true;
            m_mapOnlineUsers[ kUserInfo_.m_dwUserUID ] = kUserInfo_;
        }
    }

    // 해당 서버의 길드원 카운트 증가
    AddServer( spCenter_ );

    START_LOG( clog, L"유저 접속 정보 추가/갱신")
        << BUILD_LOGc( kUserInfo_.m_cServerUID )
        << BUILD_LOG( kUserInfo_.m_dwUserUID )
        << BUILD_LOG( kUserInfo_.m_strLogin )
        << BUILD_LOGc( kUserInfo_.m_cMemberLevel )
        << BUILD_LOG( GetOnlineUserCount() )
        << BUILD_LOG( GetOfflineUserCount() )
        << BUILD_LOG( GetJoinerCount() ) << END_LOG;

    // 길드원이 존재하는 게임서버들에 해당 정보 알림.
    KECN_ADD_GUILD_USER_BROAD_NOT kPacket;
    kPacket.m_dwGuildUID = kUserInfo_.m_dwGuildUID;
    kPacket.m_kGuildUserInfo = kUserInfo_;
    SendToAll( KCenterEvent::ECN_ADD_GUILD_USER_BROAD_NOT, kPacket );
}

bool KCnGuild::SetOffline( IN DWORD dwUserUID_ )
{
    // 온라인 유저를 오프라인 상태로 변경.
    std::map<DWORD,KNGuildUserInfo>::iterator mit;
    KNGuildUserInfo kUserInfo;
    CTime tDate( ::CTime::GetCurrentTime() );

    {
        KLocker lock( m_csJoiner );
        
        // 가입대기자 처리
        mit = m_mapJoiner.find( dwUserUID_ );

        if ( mit != m_mapJoiner.end() ) {
            // 접속 종료된 유저인지 체크
            if ( mit->second.m_bIsConnect == false ) {
                START_LOG( cwarn, L"이미 종료처리된 유저. Login : " << mit->second.m_strLogin ) << END_LOG;
                return false;
            }

            // 오프라인 처리/게임서버에 보낼 데이터 설정
            mit->second.LogOff( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0 );
            kUserInfo = mit->second;

        } 
        else {
            // 정식길드원 처리
            mit = m_mapOnlineUsers.find( dwUserUID_ );
            if ( mit == m_mapOnlineUsers.end() ) {
                // 이미 오프라인 처리 된 경우.
                START_LOG( cwarn, L"오프라인 처리하려는 유저정보가 온라인유저 목록에 없음. UserUID : " << dwUserUID_ ) << END_LOG;
                return false;
            }

            // 오프라인 처리/게임서버에 보낼 데이터 설정
            mit->second.LogOff( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0 );
            kUserInfo = mit->second;

            m_mapOnlineUsers.erase( mit ); // 온라인 유저목록에서 제거
            m_mapOfflineUsers.insert( std::make_pair( kUserInfo.m_dwUserUID, kUserInfo ) ); // 오프라인 유저목록에 추가
        }
    }

    // 서버 정보에서 유저제거(유저카운트 감소)
    RemoveServer( static_cast<DWORD>(kUserInfo.m_cServerUID) );

    START_LOG( clog, L"유저 오프라인 처리됨.")
        << BUILD_LOGc( kUserInfo.m_cServerUID )
        << BUILD_LOG( kUserInfo.m_dwUserUID )
        << BUILD_LOG( kUserInfo.m_strLogin )
        << BUILD_LOGc( kUserInfo.m_cMemberLevel )
        << BUILD_LOG( GetOnlineUserCount() )
        << BUILD_LOG( GetOfflineUserCount() )
        << BUILD_LOG( GetJoinerCount() ) << END_LOG;

    // 길드원이 나간것을 게임서버로 알림
    PAIR_DWORD kPacket;
    kPacket.first = dwUserUID_;
    kPacket.second = kUserInfo.m_dwGuildUID;
    SendToAll( KCenterEvent::ECN_OFFLINE_GUILD_USER_BROAD, kPacket );

    return true;
}

bool KCnGuild::DrumoutUser( IN DWORD dwUserUID_, IN const char cOutType_ )
{
    // 길드원이 탈퇴하거나 강퇴된 경우 불린다.
    std::map<DWORD,KNGuildUserInfo>::iterator mit;
    std::vector<KNGuildUserInfo>::iterator vit;

    bool bIsEraseUser = false; // 정상적으로 제거했는지?
    char cServerUID = -1;

    // 길드원 정보에서 유저제거(확인차 모두다 돌자)
    // 가입대기자 처리

    { // lock 의 수명을 짧게 유지하기 위한 괄호치기
        KLocker lock( m_csJoiner );
        mit = m_mapJoiner.find( dwUserUID_ );
        if ( mit != m_mapJoiner.end() ) {
            // 대기자 목록에서 제거
            cServerUID = mit->second.m_cServerUID;
            m_mapJoiner.erase( mit );
            RemoveJoinerVector( dwUserUID_ );

            bIsEraseUser = true;
        }

        // 정식 길드원 처리
        mit = m_mapOnlineUsers.find( dwUserUID_ );
        if ( mit != m_mapOnlineUsers.end() ) {
            // 온라인목록에서 삭제
            cServerUID = mit->second.m_cServerUID;
            m_mapOnlineUsers.erase( mit );

            bIsEraseUser = true;
        }

        mit = m_mapOfflineUsers.find( dwUserUID_ );
        if ( mit != m_mapOfflineUsers.end() ) {
            // 오프라인목록에서 삭제
            cServerUID = mit->second.m_cServerUID;
            m_mapOfflineUsers.erase( mit );

            bIsEraseUser = true;
        }
    }

    if ( bIsEraseUser == false ) {
        START_LOG( cerr, L"탈퇴처리를 하려는 유저정보가 없음. UserUID : " << dwUserUID_ ) << END_LOG;
        return false;
    }

    // 서버 정보에서 유저제거(유저카운트 감소)
    RemoveServer( cServerUID );
    // 길드맴버수 갱신
    m_kNGuildInfo.m_nNumMembers = GetMemberCount();

    KECN_DRUMOUT_GUILD_USER_BROAD_NOT kPacket;
    kPacket.m_dwGuildUID = m_kNGuildInfo.m_dwUID;
    kPacket.m_dwUserUID = dwUserUID_;
    kPacket.m_cOutType = cOutType_;

    // 길드원이 탈퇴한것을 전 서버로 알림
    SendToAll( KCenterEvent::ECN_DRUMOUT_GUILD_USER_BROAD_NOT, kPacket );
    return true;
}

bool KCnGuild::IsInGuild( IN const DWORD dwUserUID_ )
{
    // 길드원 목록에 있는지 여부
    return ( IsOnlineUser( dwUserUID_ ) == true ||
             IsOfflineUser( dwUserUID_ ) == true ||
             IsJoiner( dwUserUID_ ) == true );
}

bool KCnGuild::IsOnlineUser( IN const DWORD dwUserUID_ )
{
    KLocker lock( m_csJoiner );
    // 온라인 유저인가?
    return ( m_mapOnlineUsers.find( dwUserUID_ ) != m_mapOnlineUsers.end() );
}

bool KCnGuild::IsOfflineUser( IN const DWORD dwUserUID_ )
{
    KLocker lock( m_csJoiner );
    // 오프라인 유저인가?
    return ( m_mapOfflineUsers.find( dwUserUID_ ) != m_mapOfflineUsers.end() );
}

bool KCnGuild::IsJoiner( IN const DWORD dwUserUID_ )
{
    KLocker lock( m_csJoiner );
    // 가입대기자 인가?
    return ( m_mapJoiner.find( dwUserUID_ ) != m_mapJoiner.end() );
}

bool KCnGuild::IsUserListEmpty()
{
    KLocker lock( m_csJoiner );
    // 길드원 리스트가 있는가?
    return ( m_mapOnlineUsers.empty() && m_mapOfflineUsers.empty() && m_mapJoiner.empty() );
}

void KCnGuild::AddServer( IN KCenterPtr spCenter_ )
{
    KLocker lock( m_csServers );

    std::map<DWORD,std::pair<KCenterPtr,int>>::iterator mitServer = m_mapServers.find( spCenter_->GetUID() );
    // 유저 접속한 서버의 길드원 카운트값 증가
    if ( mitServer == m_mapServers.end() ) {
        mitServer = m_mapServers.insert(std::map<DWORD,std::pair<KCenterPtr,int>>::value_type(spCenter_->GetUID(),std::pair<KCenterPtr,int>(spCenter_,0)) ).first;
    }

    ++(mitServer->second.second);
    START_LOG( clog, L"서버 길드원 수 증가. GuildUID : " << m_kNGuildInfo.m_dwUID << L",ServerUID : " << mitServer->first << L",UserNum : " << mitServer->second.second ) << END_LOG;
}

void KCnGuild::RemoveServer( IN const DWORD dwUID_ )
{
    KLocker lock( m_csServers );

    std::map<DWORD,std::pair<KCenterPtr,int>>::iterator mitServer = m_mapServers.find( dwUID_ );
    if ( mitServer == m_mapServers.end() ) {
        // 이미 처리된 경우.
        START_LOG( cwarn, L"없는 서버정보(=해당 서버UID의 서버 포인터 없음). ServerUID : " << dwUID_ ) << END_LOG;
        return;
    }

    // 나간 유저가 있던 서버의 길드 맴버수 카운트를 감소
    --(mitServer->second.second);
    START_LOG( clog, L"GuildUID : " << m_kNGuildInfo.m_dwUID << L",ServerUID : " << dwUID_ << L",Member Num :" << mitServer->second.second ) << END_LOG;

    // 모든 유저가 나간 서버는 제거
    if ( mitServer->second.second <= 0 ) {
        m_mapServers.erase( mitServer );
        START_LOG( clog, L"길드원이 없는 서버정보 제거. GuildUID : " << m_kNGuildInfo.m_dwUID << L",ServerUID : " << dwUID_ ) << END_LOG;
    }
}

bool KCnGuild::GetUserInfo( IN DWORD dwUserUID_, OUT KNGuildUserInfo& kUserInfo_ )
{
    // 유저 정보 가져오기
    if ( GetOnlineUserInfo( dwUserUID_, kUserInfo_ ) == true ) {
        return true;
    }
    if ( GetOffineUserInfo( dwUserUID_, kUserInfo_ ) == true ) {
        return true;
    }
    if ( GetJoinerInfo( dwUserUID_, kUserInfo_ ) == true ) {
        return true;
    }

    START_LOG( cwarn, L"해당 유저의 길드원 정보가 없음. GuildUID : " << m_kNGuildInfo.m_dwUID << L",UserUID : " << dwUserUID_ ) << END_LOG;
    return false;
}

bool KCnGuild::GetOnlineUserInfo( IN const DWORD dwUserUID_, OUT KNGuildUserInfo& kUserInfo_ )
{
    KLocker lock( m_csJoiner );

    std::map<DWORD,KNGuildUserInfo>::iterator mit = m_mapOnlineUsers.find( dwUserUID_ );
    if ( mit == m_mapOnlineUsers.end() ) {
        return false;
    }
    kUserInfo_ = mit->second;
    return true;
}

bool KCnGuild::GetOffineUserInfo( IN const DWORD dwUserUID_, OUT KNGuildUserInfo& kUserInfo_ )
{
    KLocker lock( m_csJoiner );

    std::map<DWORD,KNGuildUserInfo>::iterator mit = m_mapOfflineUsers.find( dwUserUID_ );
    if ( mit == m_mapOfflineUsers.end() ) return false;
    kUserInfo_ = mit->second;
    return true;
}
bool KCnGuild::GetJoinerInfo( IN const DWORD dwUserUID_, OUT KNGuildUserInfo& kUserInfo_ )
{
    KLocker lock( m_csJoiner );

    std::map<DWORD,KNGuildUserInfo>::iterator mit = m_mapJoiner.find( dwUserUID_ );
    if ( mit == m_mapJoiner.end() ) return false;
    kUserInfo_ = mit->second;
    return true;
}

void KCnGuild::SetNotice( IN std::vector<KNGuildNotice>& vecNotice_ )
{
    KLocker lock( m_csJoiner );

    m_vecNGuildNotice = vecNotice_;
    START_LOG( clog, L"길드 공지사항 설정됨. GuildUID : " << m_kNGuildInfo.m_dwUID << L",size : " << m_vecNGuildNotice.size() ) << END_LOG;
}

void KCnGuild::GetNotice( OUT std::vector<KNGuildNotice>& vecNotice_ )
{
    KLocker lock( m_csJoiner );

    vecNotice_ = m_vecNGuildNotice;
}

void KCnGuild::ModifyNotice( IN const std::vector<KNGuildNotice>& vecNotice_ )
{
    KLocker lock( m_csJoiner );

    std::vector<KNGuildNotice>::const_iterator cvit;
    std::vector<KNGuildNotice>::iterator vit;

    for ( cvit = vecNotice_.begin() ; cvit != vecNotice_.end() ; ++cvit ) {

        vit = std::find_if( m_vecNGuildNotice.begin(), m_vecNGuildNotice.end(),
            boost::bind(&KNGuildNotice::m_nMsgID,_1) == cvit->m_nMsgID );

        if ( vit == m_vecNGuildNotice.end() ) {
            m_vecNGuildNotice.push_back( *cvit );
            START_LOG( clog, L"공지 새로 추가함. MsgID : " << cvit->m_nMsgID )
                << BUILD_LOG( cvit->m_strMsg ) << END_LOG;
            continue;
        }

        vit->m_strMsg = cvit->m_strMsg;
        START_LOG( clog, L"공지 수정함. MsgID : " << cvit->m_nMsgID )
            << BUILD_LOG( cvit->m_strMsg ) << END_LOG;

        // 길드 소개면 길드정보에도 적용
        if ( cvit->m_nMsgID == KNGuildNotice::GN_COMMENT ) {
            m_kNGuildInfo.m_strComment = cvit->m_strMsg;
            START_LOG( clog, L"길드소개 수정됨. MsgID : " << cvit->m_nMsgID )
                << BUILD_LOG( cvit->m_strMsg ) << END_LOG;
        }
    }

    START_LOG( clog, L"수정된 공지사항 적용됨. Msg size : " << vecNotice_.size() ) << END_LOG;
}

// 길드 유저 중 길드마스터를 찾아서 길드 정보에 넣기
bool KCnGuild::SetGuildMaster()
{
    std::map< DWORD, KNGuildUserInfo >::const_iterator mitOnline;
    std::map< DWORD, KNGuildUserInfo >::const_iterator mitOffline;
    
    KLocker lock( m_csJoiner );

    for( mitOnline = m_mapOnlineUsers.begin() ; mitOnline != m_mapOnlineUsers.end() ; ++mitOnline ) {
        if ( mitOnline->second.m_cMemberLevel == KNGuildUserInfo::GL_MASTER ) {
            m_kNGuildInfo.m_strMasterNick = mitOnline->second.m_strNickName;
            m_kNGuildInfo.m_strMasterNickColor = mitOnline->second.m_strNickColor;
            return true;
        }
    }

    for( mitOffline = m_mapOfflineUsers.begin() ; mitOffline != m_mapOfflineUsers.end() ; ++mitOffline ) {
        if( mitOffline->second.m_cMemberLevel == KNGuildUserInfo::GL_MASTER ) {
            m_kNGuildInfo.m_strMasterNick = mitOffline->second.m_strNickName;
            m_kNGuildInfo.m_strMasterNickColor = mitOffline->second.m_strNickColor;
            return true;
        }
    }

    return false;
}

void KCnGuild::SetMemberList( IN std::map<DWORD, KNGuildUserInfo>& mapUserList_ )
{
    KLocker lock( m_csJoiner );
    // 길드원 리스트 설정
    std::map<DWORD, KNGuildUserInfo>::iterator mit;
    std::map<DWORD, KNGuildUserInfo> mapOnlineList; // 온라인 유저
    std::map<DWORD, KNGuildUserInfo> mapOfflineList;// 오프라인 유저
    std::map<DWORD, KNGuildUserInfo> mapJoinerList; // 가입대기자

    for ( mit = mapUserList_.begin() ; mit != mapUserList_.end() ; ++mit ) {

        // 길드마스터UID 저장
        if ( mit->second.m_cMemberLevel == KNGuildUserInfo::GL_MASTER ) {
            SetMasterUID( mit->second.m_dwUserUID );
        }

        // 가입대기자
        if ( mit->second.m_cMemberLevel == KNGuildUserInfo::GL_JOINER ) {
            mapJoinerList[ mit->first ] = mit->second;
            m_vecJoinerByJoinDate.push_back( mit->second );
            continue;
        }

        // 오프라인
        if ( mit->second.m_bIsConnect == false ) {
            mapOfflineList[ mit->first ] = mit->second;
            continue;
        }

        // 온라인
        mapOnlineList[ mit->first ] = mit->second;
    }

    // 유저리스트 설정(swap)
    SetOnlineMemberList( mapOnlineList );
    SetOfflineMemberList( mapOfflineList );
    SetJoinerList( mapJoinerList );

    // 가입신청자 자동거절
    AutoRejectJoinerCheck();

    START_LOG( clog, L"길드원 리스트 설정됨. GuildUID : " << m_kNGuildInfo.m_dwUID << L", size : " << mapUserList_.size() )
        << BUILD_LOG( GetOnlineUserCount() )
        << BUILD_LOG( GetOfflineUserCount() )
        << BUILD_LOG( GetJoinerCount() ) << END_LOG;
}

void KCnGuild::SetOnlineMemberList( IN std::map<DWORD, KNGuildUserInfo>& mapUserList_ )
{
    // 온라인유저 리스트 설정
    KLocker lock( m_csJoiner );

    m_mapOnlineUsers.swap( mapUserList_ );
}

void KCnGuild::SetOfflineMemberList( IN std::map<DWORD, KNGuildUserInfo>& mapUserList_ )
{
    // 오프라인유저 리스트 설정
    KLocker lock( m_csJoiner );

    m_mapOfflineUsers.swap( mapUserList_ );
}

void KCnGuild::SetJoinerList( IN std::map<DWORD, KNGuildUserInfo>& mapUserList_ )
{
    // 가입대기자 리스트 설정
    KLocker lock( m_csJoiner );

    m_mapJoiner.swap( mapUserList_ );
}

void KCnGuild::GetMemberList( OUT std::map<DWORD, KNGuildUserInfo>& mapUserList_ )
{
    // 온라인/오프라인/가입대기자 모두 합쳐진 유저리스트를 반환.
    std::map<DWORD, KNGuildUserInfo>::const_iterator cmit;

    KLocker lock( m_csJoiner );

    // 온라인
    for ( cmit = m_mapOnlineUsers.begin() ; cmit != m_mapOnlineUsers.end() ; ++cmit ) {
        mapUserList_[ cmit->first ] = cmit->second;
    }
    // 오프라인
    for ( cmit = m_mapOfflineUsers.begin() ; cmit != m_mapOfflineUsers.end() ; ++cmit ) {
        mapUserList_[ cmit->first ] = cmit->second;
    }
    // 가입대기자
    for ( cmit = m_mapJoiner.begin() ; cmit != m_mapJoiner.end() ; ++cmit ) {
        mapUserList_[ cmit->first ] = cmit->second;
    }

    START_LOG( clog, L"mapUserList_.size() : " << mapUserList_.size() )
        << BUILD_LOG( GetOnlineUserCount() )
        << BUILD_LOG( GetOfflineUserCount() )
        << BUILD_LOG( GetJoinerCount() ) << END_LOG;
}

void KCnGuild::GetOnlineUserList( OUT std::map<DWORD, KNGuildUserInfo>& mapUserList_ )
{
    KLocker lock( m_csJoiner );

    mapUserList_ = m_mapOnlineUsers;
}

void KCnGuild::GetOfflineUserList( OUT std::map<DWORD, KNGuildUserInfo>& mapUserList_ )
{
    KLocker lock( m_csJoiner );

    mapUserList_ = m_mapOfflineUsers;
}

void KCnGuild::GetJoinerList( OUT std::map<DWORD, KNGuildUserInfo>& mapUserList_ )
{
    KLocker lock( m_csJoiner );

    mapUserList_ = m_mapJoiner;
}

void KCnGuild::GetJoinerListVector( OUT std::vector<KNGuildUserInfo>& vecUserList_ )
{
    KLocker lock( m_csJoiner );

    vecUserList_ = m_vecJoinerByJoinDate;
}

void KCnGuild::GetJoinerUIDListByJoinDate( IN const int nUserCount_, OUT std::set<DWORD>& setUserList_ )
{
    std::vector<KNGuildUserInfo>::iterator vit;
    std::vector<KNGuildUserInfo> vecJoiner;

    GetJoinerListVector( vecJoiner );
    std::sort( vecJoiner.begin(), vecJoiner.end(), KSortJoinDate() );

    for ( vit = vecJoiner.begin() ; vit != vecJoiner.end() ; ++vit ) {
        // 가입일순 정렬로 제한수만큼 인원정보 가져오기.
        if ( setUserList_.size() >= (UINT)nUserCount_ ) {
            break;
        }

        setUserList_.insert( vit->m_dwUserUID );
    }
    START_LOG( clog, L"가입신청일순 가입대기자 목록 얻기. size : " << setUserList_.size() << L" / " << nUserCount_ ) << END_LOG;
}

bool KCnGuild::ModifyMemberComment( IN const DWORD dwUserUID_, IN const std::wstring& strUserComment_ )
{
    // 유저 소개글 변경
    if ( ModifyOnlineUserComment( dwUserUID_, strUserComment_ ) == true ) return true;
    if ( ModifyOfflineUserComment( dwUserUID_, strUserComment_ ) == true ) return true;
    if ( ModifyJoinerComment( dwUserUID_, strUserComment_ ) == true ) return true;

    START_LOG( cerr, L"해당 유저정보가 없음. GuildUID : " << m_kNGuildInfo.m_dwUID << L",UserUID : " << dwUserUID_ ) << END_LOG;
    return false;
}

bool KCnGuild::ModifyOnlineUserComment( IN const DWORD dwUserUID_, IN const std::wstring& strUserComment_ )
{
    KLocker lock( m_csJoiner );

    std::map<DWORD, KNGuildUserInfo>::iterator mit = m_mapOnlineUsers.find( dwUserUID_ );
    if ( mit == m_mapOnlineUsers.end() ) return false;
    mit->second.m_strMyComment = strUserComment_;
    return true;
}

bool KCnGuild::ModifyOfflineUserComment( IN const DWORD dwUserUID_, IN const std::wstring& strUserComment_ )
{
    KLocker lock( m_csJoiner );

    std::map<DWORD, KNGuildUserInfo>::iterator mit = m_mapOfflineUsers.find( dwUserUID_ );
    if ( mit == m_mapOfflineUsers.end() ) return false;
    mit->second.m_strMyComment = strUserComment_;
    return true;
}

bool KCnGuild::ModifyJoinerComment( IN const DWORD dwUserUID_, IN const std::wstring& strUserComment_ )
{
    KLocker lock( m_csJoiner );

    std::map<DWORD, KNGuildUserInfo>::iterator mit = m_mapJoiner.find( dwUserUID_ );
    if ( mit == m_mapJoiner.end() ) return false;
    mit->second.m_strMyComment = strUserComment_;
    return true;
}

void KCnGuild::SetMemeberLevel( IN const char cMemberLevel_, IN const std::set<DWORD>& setUID_ )
{
    std::map<DWORD, KNGuildUserInfo>::iterator mit;
    std::set<DWORD>::const_iterator sit;

    KLocker lock( m_csJoiner );

    // 맴버레벨 변경
    for ( sit = setUID_.begin() ; sit != setUID_.end() ; ++sit ) {

        // 온라인 유저
        mit = m_mapOnlineUsers.find( *sit );
        if ( mit != m_mapOnlineUsers.end() ) {
            mit->second.m_cMemberLevel = cMemberLevel_;
            START_LOG( clog, L"온라인유저 맴버레벨 변경." )
                << BUILD_LOG( mit->second.m_strLogin )
                << BUILD_LOGc( cMemberLevel_ ) << END_LOG;
            continue;
        }

        // 오프라인 유저
        mit = m_mapOfflineUsers.find( *sit );
        if ( mit != m_mapOfflineUsers.end() ) {
            mit->second.m_cMemberLevel = cMemberLevel_;
            START_LOG( clog, L"오프라인유저 맴버레벨 변경." )
                << BUILD_LOG( mit->second.m_strLogin )
                << BUILD_LOGc( cMemberLevel_ ) << END_LOG;
            continue;
        }

        // 가입대기자
        mit = m_mapJoiner.find( *sit );
        if ( mit == m_mapJoiner.end() ) {
            START_LOG( cerr, L"유저목록에 없는 유저. UserUID : " << *sit ) << END_LOG;
            continue;
        }
        // 가입대기자는 일반길드원이 될수밖에 없다.
        // 가입거절 처리는 탈퇴 처리로 가기때문에 이쪽으로 안온다.
        if ( cMemberLevel_ != KNGuildUserInfo::GL_NORMAL ) {
            START_LOG( cerr, L"가입대기자인데 변경하려는 맴버레벨이 일반길드원이 아님." )
                << BUILD_LOG( mit->second.m_strLogin )
                << BUILD_LOGc( cMemberLevel_ ) << END_LOG;
            continue;
        }

        // 맴버 레벨 변경
        mit->second.m_cMemberLevel = cMemberLevel_;

        // 온오프라인 상태에 따라 유저를 이동.
        if ( mit->second.m_bIsConnect == true ) {
            m_mapOnlineUsers[ mit->first ] = mit->second;
            START_LOG( clog, L"온라인유저 맴버레벨 변경. 가입대기자에서 일반으로 이동함." )
                << BUILD_LOG( mit->second.m_strLogin )
                << BUILD_LOGc( cMemberLevel_ ) << END_LOG;
        } else {
            m_mapOfflineUsers[ mit->first ] = mit->second;
            START_LOG( clog, L"오프라인유저 맴버레벨 변경. 가입대기자에서 일반으로 이동함." )
                << BUILD_LOG( mit->second.m_strLogin )
                << BUILD_LOGc( cMemberLevel_ ) << END_LOG;
        }

        // 대기자목록에서 제거
        m_mapJoiner.erase( mit );
        RemoveJoinerVector( *sit );

        START_LOG( clog, L"일반길드원이 된 유저를 가입대기자 목록에서 제거." )
            << BUILD_LOG( mit->second.m_strLogin )
            << BUILD_LOGc( cMemberLevel_ ) << END_LOG;
    }

    START_LOG( clog, L"맴버레벨 변경됨. MemberLevel : " << (int)cMemberLevel_ << L",size :" << setUID_.size() ) << END_LOG;
}

bool KCnGuild::ChangeMemberLevel( IN const std::map<char,std::set<DWORD> >& mapChangeMemberLevel_ )
{
    // 길드맴버레벨 변경
    std::map<char,std::set<DWORD> >::const_iterator mit;
    std::set<DWORD>::const_iterator sit;

    if ( mapChangeMemberLevel_.empty() ) return false;

    for ( mit = mapChangeMemberLevel_.begin() ; mit != mapChangeMemberLevel_.end() ; ++mit ) {
        // 탈퇴처리는 여기서 하지 않는다.
        if ( mit->first == KNGuildUserInfo::GL_DRUMOUT ) {
            continue;
        }

        // 일반길드원으로 설정
        if ( mit->first == KNGuildUserInfo::GL_NORMAL ) {
            SetMemeberLevel( KNGuildUserInfo::GL_NORMAL, mit->second );
            continue;
        }

        // 제너럴로 설정
        if ( mit->first == KNGuildUserInfo::GL_GENERAL ) {
            SetMemeberLevel( KNGuildUserInfo::GL_GENERAL, mit->second );
            continue;
        }

        // 길드마스터로 설정
        if ( mit->first == KNGuildUserInfo::GL_MASTER ) {

            if ( mit->second.size() > 1 ) {
                START_LOG( cerr, L"길드마스터는 1명밖에 존재할 수 없음. size : " << mit->second.size() ) << END_LOG;
                continue;
            }

            SetMemeberLevel( KNGuildUserInfo::GL_MASTER, mit->second );
            // 길드마스터 UID 변경
            SetMasterUID( *(mit->second.begin()) );
            SetGuildMaster();
            continue;
        }

        START_LOG( cerr, L"정의되지않은 맴버레벨. MemberLevel : " << (int)(mit->first) << L",size : " << mit->second.size() ) << END_LOG;
    }

    // 길드맴버수 갱신
    KLocker lock( m_csJoiner );

    m_kNGuildInfo.m_nNumMembers = GetMemberCount();

    START_LOG( clog, L"맴버레벨 변경 적용됨. size :" << mapChangeMemberLevel_.size() ) << END_LOG;
    return true;
}

void KCnGuild::GetGuildMarkInfo( OUT KNGuildMarkInfo& kMarkInfo_ )
{
    KLocker lock( m_csJoiner );

    kMarkInfo_.m_dwGuildUID = m_kNGuildInfo.m_dwUID;
    kMarkInfo_.m_ucMarkStatus = m_kNGuildInfo.m_ucMarkStatus;
    kMarkInfo_.m_nMarkRevision = m_kNGuildInfo.m_nMarkRevision;
    kMarkInfo_.m_strFileName = m_kNGuildInfo.m_strFileName;
}

void KCnGuild::SetGuildMarkStatus( IN const UCHAR ucMarkStatus_ )
{
    KLocker lock( m_csJoiner );

    m_kNGuildInfo.m_ucMarkStatus = ucMarkStatus_;
}

DWORD KCnGuild::GetLiveUserCount()
{
    return GetOnlineUserCount() + GetJoinerOnlineCount();
}

bool KCnGuild::GuildMarkUpdate( IN const KNGuildMarkInfo& kMarkInfo_ )
{
    KLocker lock( m_csJoiner );

    if ( kMarkInfo_.m_ucMarkStatus != m_kNGuildInfo.m_ucMarkStatus ||
         kMarkInfo_.m_nMarkRevision != m_kNGuildInfo.m_nMarkRevision ||
         kMarkInfo_.m_strFileName != m_kNGuildInfo.m_strFileName )
    {
        m_kNGuildInfo.m_ucMarkStatus = kMarkInfo_.m_ucMarkStatus;
        m_kNGuildInfo.m_nMarkRevision = kMarkInfo_.m_nMarkRevision;
        m_kNGuildInfo.m_strFileName = kMarkInfo_.m_strFileName;

        START_LOG( clog, L"길드마크 상태 변경됨." )
            << BUILD_LOGc( kMarkInfo_.m_ucMarkStatus )
            << BUILD_LOG( kMarkInfo_.m_nMarkRevision )
            << BUILD_LOG( kMarkInfo_.m_strFileName ) << END_LOG;
        return true;
    }
    return false;
}

void KCnGuild::AutoJoinSettingCheck()
{
    if ( GetMemberCount() < (UINT)GetMaxMemberNum() ) {
        START_LOG( clog, L"길드원수가 최대치보다 적음." )
            << BUILD_LOG( m_kNGuildInfo.m_dwUID )
            << BUILD_LOG( GetMemberCount() )
            << BUILD_LOG( GetMaxMemberNum() ) << END_LOG;
        return;
    }

    KLocker lock( m_csJoiner );
    // 현재 정식길드원수가 최대치이면, 가입중지 상태로 만든다.
    m_kNGuildInfo.m_ucJoinSetting = KNGuildInfo::JS_STOP_JOIN;

    // DB에도 변경요청.
    AutoChangeGuildJoinSettingNot();

    // 남은 가입대기자는 자동으로 가입거절 처리.
    std::set<DWORD> setRejectJoinerUserUID;
    std::vector< KNGuildUserInfo > vecJoiner;
    GetJoinerListVector( vecJoiner );
    std::vector< KNGuildUserInfo >::iterator vit;
    for ( vit = vecJoiner.begin() ; vit != vecJoiner.end() ; ++vit ) {
        setRejectJoinerUserUID.insert( vit->m_dwUserUID );
    }

    // DB에 삭제신청
    AutoRejectGuildJoinerReq( setRejectJoinerUserUID );
}

void KCnGuild::AutoChangeGuildJoinSettingNot()
{
    KCnNGuildJoinPolicy kPacket;
    kPacket.m_dwUserUID = GetMasterUID();
    kPacket.m_kPolicy.m_dwUID = m_kNGuildInfo.m_dwUID;
    kPacket.m_kPolicy.m_ucMethod = KNGuildInfo::JS_STOP_JOIN;

    SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_AUTO_CHANGE_GUILD_JOIN_SETTING_NOT, NULL, 0, 0, kPacket );

    START_LOG( clog, L"길드원수가 최대치를 넘어, 길드가입설정 가입중지로 자동변경." )
        << BUILD_LOG( m_kNGuildInfo.m_dwUID )
        << BUILD_LOG( GetMemberCount() )
        << BUILD_LOG( GetMaxMemberNum() ) << END_LOG;
}

void KCnGuild::SetMasterUID( IN const DWORD dwUID_ )
{
    KLocker lock( m_csJoiner );

    m_dwGuildMasterUID = dwUID_;
    START_LOG( clog, L"길드마스터 UID 설정됨. UserUID : " << dwUID_ ) << END_LOG;
}

void KCnGuild::RemoveJoinerVector( IN const DWORD dwUserUID_ )
{
    // 외부에서 lock 걸어주세요.
    std::vector<KNGuildUserInfo>::iterator vit;
    vit = std::find_if( m_vecJoinerByJoinDate.begin(), m_vecJoinerByJoinDate.end(),
        boost::bind(&KNGuildUserInfo::m_dwUserUID,_1) == dwUserUID_ );
    if ( vit != m_vecJoinerByJoinDate.end() ) {
        m_vecJoinerByJoinDate.erase( vit );
    }
}

void KCnGuild::AutoUpgradeGuildGrade()
{
    // 신규길드 조건만족시 정식길드로 등급을 바꾼다.
    if ( m_kNGuildInfo.m_ucGrade != KNGuildInfo::GG_NEWBIE ) {
        return;
    }

    // 조건체크
    if ( GetMemberCount() < (UINT)m_nOfficialGuildMemberNum ) {
        return;
    }

    // DB에도 변경요청.
    SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_UPGRADE_GUILD_GRADE_REQ, NULL, 0, 0, m_kNGuildInfo.m_dwUID );

    START_LOG( clog, L"DB에 길드등급 공식길드로 변경요청. GuildUID : " << m_kNGuildInfo.m_dwUID )
        << BUILD_LOG( GetMemberCount() )
        << BUILD_LOG( m_nOfficialGuildMemberNum ) << END_LOG;
}

void KCnGuild::SetNewbieGuildRemoveDate( IN KNGuildInfo& kInfo_ )
{
    // 신생길드만 체크.
    if ( kInfo_.m_ucGrade != KNGuildInfo::GG_NEWBIE ) {
        return;
    }

    // 길드 만든 날짜.
    CTime tRemoveDate( kInfo_.m_kFoundingDate.m_sYear, kInfo_.m_kFoundingDate.m_cMonth, kInfo_.m_kFoundingDate.m_cDay, 0, 0, 0 );
    // 만료기간
    CTimeSpan tSpan( m_nGuildRemoveTerm ,0,0,0);
    tRemoveDate += tSpan;

    // 길드 삭제일 설정.
    m_kGuilRemovedDate.SetDate( tRemoveDate.GetYear(), tRemoveDate.GetMonth(), tRemoveDate.GetDay(), 0 );

    START_LOG( clog, L"신생길드. 길드삭제일 설정됨. GuildUID : " << kInfo_.m_dwUID
        << L"창립날짜   : " << kInfo_.m_kFoundingDate.m_sYear << L"-" << (int)kInfo_.m_kFoundingDate.m_cMonth << L"-" << (int)kInfo_.m_kFoundingDate.m_cDay << dbg::endl
        << L"삭제예정일 : " << m_kGuilRemovedDate.m_sYear << L"-" << (int)m_kGuilRemovedDate.m_cMonth << L"-" << (int)m_kGuilRemovedDate.m_cDay ) << END_LOG;
}

void KCnGuild::NewbieGuildDestroyCheck()
{
    // 신생길드 삭제 체크
    if ( m_kNGuildInfo.m_ucGrade != KNGuildInfo::GG_NEWBIE ) {
        return;
    }

    // 최종 삭제전에 정식길드 조건 갖췄는지 마지막 검사.
    if ( GetMemberCount() >= (UINT)m_nOfficialGuildMemberNum ) {
        // 조건이 맞으면 공식길드로 변경 요청
        AutoUpgradeGuildGrade();
        return;
    }

    // DB에 길드 삭제 요청
    SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_NEWBIE_GUILD_DESTROY_NOT, NULL, 0, 0, m_kNGuildInfo.m_dwUID );

    START_LOG( cerr, L"신생길드 삭제예정일 지났음. DB에 삭제처리 요청함. GuildUID : " << m_kNGuildInfo.m_dwUID
        << L"삭제예정일 : " << m_kGuilRemovedDate.Str() ) << END_LOG;
}

void KCnGuild::SetRank( IN const int nWeeklyRank_, IN const int nMonthlyRank_, IN const int nTotalRank_ )
{
    m_kNGuildInfo.m_nWeeklyRank = nWeeklyRank_;
    m_kNGuildInfo.m_nMonthlyRank = nMonthlyRank_;
    m_kNGuildInfo.m_nTotalRank = nTotalRank_;

    START_LOG( clog, L"랭킹정보 설정. GuildUID : " << m_kNGuildInfo.m_dwUID )
        << BUILD_LOG( nWeeklyRank_ )
        << BUILD_LOG( nMonthlyRank_ )
        << BUILD_LOG( nTotalRank_ ) << END_LOG;
}

void KCnGuild::SetRejectJoinerDate()
{
    // 길드 가입신청자 자동 제거일 구하기
    CTime tDate = CTime::GetCurrentTime();
    CTimeSpan tJoinerRemoveTerm( m_nJoinerRemoveTerm,0,0,0);
    tDate -= tJoinerRemoveTerm;
    m_kRejectJoinerDate.SetDate( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 0 );
}

void KCnGuild::AutoRejectJoinerCheck()
{
    // 가입신청한 기간에 따라 자동 가입거절
    std::set<DWORD> setRejectJoinerUserUID;
    std::vector< KNGuildUserInfo > vecJoiner;
    GetJoinerListVector( vecJoiner );
    setRejectJoinerUserUID.clear();
    
    if ( vecJoiner.empty() ) {
        //START_LOG( clog, L"가입대기자 없음." ) << END_LOG;
        return;
    }

    std::vector< KNGuildUserInfo >::iterator vit;
    for ( vit = vecJoiner.begin() ; vit != vecJoiner.end() ; ++vit ) {
        // 신청기간이 삭제일 이전인가?
        if ( vit->m_kJoinDate < m_kRejectJoinerDate ) {
            setRejectJoinerUserUID.insert( vit->m_dwUserUID );
        }
    }

    // DB에 삭제신청
    AutoRejectGuildJoinerReq( setRejectJoinerUserUID );
}

void KCnGuild::AutoRejectGuildJoinerReq( IN std::set<DWORD>& setUserUID_ )
{
    KECN_REJECT_GUILD_JOINER_REQ kPacket;
    kPacket.m_dwGuildUID = m_kNGuildInfo.m_dwUID;
    kPacket.m_dwUserUID = GetMasterUID();
    kPacket.m_setUserUID.swap( setUserUID_ );

    START_LOG( clog, L"가입신청 유예기간이 지나서 자동 가입거절처리됨. GuildUID : " << m_kNGuildInfo.m_dwUID )
        << BUILD_LOG( kPacket.m_setUserUID.size() )
        << BUILD_LOG( m_kRejectJoinerDate.Str() ) << END_LOG;

    // DB에 삭제신청
    SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_AUTO_REJECT_GUILD_JOINER_REQ, NULL, 0, 0, kPacket );
}

void KCnGuild::SetConfig( IN KGuildConfig& kConfig_ )
{
    {
        KLocker lock( m_csConfig );
        m_kConfig = kConfig_;
    }
    m_kMemberClearTimer.SetInterval( kConfig_.m_dwMemberListClearTerm );
    m_kPointTimer.SetInterval( kConfig_.m_dwUpdatePointTerm );
}

void KCnGuild::UpgradeGuildGradeGiftReq()
{
    // 공식길드 변경시,길마에게 길드마크변경권 넣어주기.
    /* 캐릭터 분리 작업으로 동작이 아래와 같이 변경됨
    센터 서버
    If 승급 {
        SDGA 값 저장: 길마 UID, 길드 UID(time_t), data(1)
        If 길마 로그온 {
            해당 게임 서버에 알림
        }
    }

    게임 서버
    If 마크 변경권 지급 알림 {
        SDGA 값 확인
        If 1 이면 {
            현재 플레이 중인 캐릭터 인벤에 지급
            지급 후 data(0) 저장
            지급 성공 후 메시지로 알림(이벤트 분리)
        }
    }

    게임 서버
    If 접속시(event verify account). 길드장인 경우 {
        SDGA 값 확인
        If 1 이면 {
            마지막 플레이한 캐릭터 인벤에 지급
            지급 후 data(0) 저장
            지급 성공 후 메시지로 알림(이벤트 분리)
        }
    }
    */
    KECN_UPGRADE_GUILD_GRADE_GIFT_REQ kPacket;
    kPacket.m_dwGuildUID = m_kNGuildInfo.m_dwUID;
    kPacket.m_dwGuildMasterUID = GetMasterUID();
    SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_UPGRADE_GUILD_GRADE_GIFT_REQ, NULL, 0, 0, kPacket );

    START_LOG( clog, L"공식길드 승격에 따른 아이템 요청함. GuildUID : " << m_kNGuildInfo.m_dwUID )
        << BUILD_LOG( kPacket.m_dwGuildMasterUID )
        << END_LOG;
}

size_t KCnGuild::GetJoinerOnlineCount()
{
    std::map< DWORD, KNGuildUserInfo >::iterator mit;
    size_t stOnlineCount = 0;

    KLocker lock( m_csJoiner );
    for ( mit = m_mapJoiner.begin() ; mit != m_mapJoiner.end() ; ++mit ) {
        if ( mit->second.m_bIsConnect == true ) {
            ++stOnlineCount;
        }
    }
    // 온라인인 가입대기자수 구하기
    return stOnlineCount;
}

void KCnGuild::AutoAcceptJoinerList()
{
    // 바로가입으로 변경시에만 처리.
    if ( m_kNGuildInfo.m_ucJoinSetting != KNGuildInfo::JS_AUTO_ACCEPT ) {
        return;
    }

    if ( GetMemberCount() >= (UINT)(GetMaxMemberNum()) ) {
        START_LOG( clog, L"맴버수가 이미 최대치. Guild UID : " << m_kNGuildInfo.m_dwUID )
            << BUILD_LOG( GetMemberCount() )
            << BUILD_LOG( GetMaxMemberNum() ) << END_LOG;
        return;
    }

    if ( GetJoinerCount() <= 0 ) {
        START_LOG( clog, L"길드가입 대기자 없음. Guild UID : " << m_kNGuildInfo.m_dwUID ) << END_LOG;
        return;
    }

    // 대기자 바로가입 처리
    KECN_CHANGE_GUILD_MEMBER_LEVEL_REQ kPacket;
    kPacket.m_dwGuildUID = m_kNGuildInfo.m_dwUID;
    kPacket.m_dwGuildMasterUID = GetMasterUID();
    kPacket.m_mapChangeMemberLevel.clear();

    // 자동가입설정으로 가입가능한 인원수
    int nAutoAcceptJoinerCount = GetMaxMemberNum() - GetMemberCount();

    std::set<DWORD> setUserUID;
    GetJoinerUIDListByJoinDate( nAutoAcceptJoinerCount, setUserUID );
    kPacket.m_mapChangeMemberLevel[ KNGuildUserInfo::GL_NORMAL ] = setUserUID;

    // DB 큐잉 queuing
    SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_CHANGE_GUILD_MEMBER_LEVEL_REQ, NULL, 0, 0, kPacket );

    START_LOG( clog, L"자동가입설정으로 대기자 자동가입신청. Guild UID : " << m_kNGuildInfo.m_dwUID )
        << BUILD_LOG( nAutoAcceptJoinerCount )
        << BUILD_LOG( setUserUID.size() ) << END_LOG;
}

void KCnGuild::PrintUserListInfo()
{
    KLocker lock( m_csJoiner );
    START_LOG( cerr, L"-- Print User List Info (GuildUID:" << m_kNGuildInfo.m_dwUID << L") --" << dbg::endl
              << L"m_mapOnlineUsers.size() : " << m_mapOnlineUsers.size() << dbg::endl
              << L"m_mapOfflineUsers.size() : " << m_mapOfflineUsers.size() << dbg::endl
              << L"m_mapJoiner.size() : " << m_mapJoiner.size() << dbg::endl
              << L"m_vecJoinerByJoinDate.size() : " << m_vecJoinerByJoinDate.size() ) << END_LOG;
}

// 패킷 처리
_IMPL_ON_FUNC( ECN_UPDATE_GUILD_NAME_NOT, std::wstring )
{
    m_kNGuildInfo.m_strName = kPacket_;

    KECN_UPDATE_GUILD_NAME_NOT kPacket;
    kPacket.m_dwGuildUID = m_kNGuildInfo.m_dwUID;
    kPacket.m_strGuildName = m_kNGuildInfo.m_strName;

    START_LOG( clog, L"길드 이름 변경. Guild UID : " << m_kNGuildInfo.m_dwUID )
        << BUILD_LOG( m_kNGuildInfo.m_strName ) << END_LOG;

    SendToAll( KCenterEvent::ECN_UPDATE_GUILD_NAME_NOT, kPacket );
}

IMPL_ON_FUNC( ECN_JOIN_GUILD_ACK )
{
    // 길드 가입성공시 불린다.
    KECN_ADD_GUILD_USER_BROAD_NOT kPacket;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_kGuildUserInfo = kPacket_.m_kGuildUserInfo;

    if ( kPacket_.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_JOINER ) {
        // 가입대기자 추가
        LIF( AddJoiner( kPacket_.m_kGuildUserInfo ) );
    } else {
        // 신규맴버 추가
        LIF( AddMember( kPacket_.m_kGuildUserInfo ) );
    }

    // 맴버수 체크에 의한 가입설정변경
    AutoJoinSettingCheck();
    // 정식길드 변경 체크
    AutoUpgradeGuildGrade();

    START_LOG( clog, L"신규 유저 추가. GuildUID : " << kPacket_.m_dwGuildUID )
        << BUILD_LOG( m_kNGuildInfo.m_strName )
        << BUILD_LOG( kPacket_.m_kGuildUserInfo.m_dwUserUID )
        << BUILD_LOGc( kPacket_.m_kGuildUserInfo.m_cMemberLevel )
        << BUILD_LOGc( kPacket_.m_kGuildUserInfo.m_cServerUID ) << END_LOG;

    SendToAll( KCenterEvent::ECN_ADD_GUILD_USER_BROAD_NOT, kPacket );
}

_IMPL_ON_FUNC( ECN_UPDATE_GUILD_URL_NOT, KNGuildURL )
{
    m_kNGuildInfo.m_strURL = kPacket_.m_strURL;

    START_LOG( clog, L"길드 URL 변경. Guild UID : " << m_kNGuildInfo.m_dwUID )
        << BUILD_LOG( m_kNGuildInfo.m_strURL ) << END_LOG;

    SendToAll( KCenterEvent::ECN_UPDATE_GUILD_URL_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ECN_GUILD_MARK_INFO_UPDATE_ACK, KNGuildMarkInfo )
{
    if ( !GuildMarkUpdate( kPacket_ ) ) {
        START_LOG( clog, L"길드 마크 정보 변경 없음.. UID : " << GetUID() ) << END_LOG;
        // 길드 마크 변경이 없으므로 여기서 빠이..
        return;
    }
    GetGuildMarkInfo( kPacket_ );
    SendToAll( KCenterEvent::ECN_GUILD_MARK_STATUS_NOT, kPacket_ );
}

IMPL_ON_FUNC( ECN_CANCEL_JOIN_GUILD_ACK )
{
    // 가입대기자가 길드 가입취소시 불린다.
    KECN_REMOVE_GUILD_USER_BROAD_NOT kPacket;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
    kPacket.m_cOutType = KNGuildUserInfo::OT_CANCEL_JOIN;

    // 대기자명단에서 제거
    LIF( RemoveJoiner( kPacket_.m_dwUserUID ) );

    START_LOG( clog, L"가입대기자 제거. GuildUID : " << kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;

    SendToAll( KCenterEvent::ECN_REMOVE_GUILD_USER_BROAD_NOT, kPacket );
}

IMPL_ON_FUNC( ECN_CHANGE_GUILD_MEMBER_LEVEL_ACK )
{
    // 길드맴버 변경시 불린다.
    std::map< char, std::set<DWORD> >::iterator mit;
    std::set<DWORD>::iterator sit;

    KECN_CHANGE_GUILD_MEMBER_LEVEL_BROAD_NOT kPacket;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_mapChangeMemberLevel = kPacket_.m_mapChangeMemberLevel;
    kPacket.m_cOutType = KNGuildUserInfo::OT_DRUMOUT;

    // 탈퇴처리
    mit = kPacket_.m_mapChangeMemberLevel.find( KNGuildUserInfo::GL_DRUMOUT );
    if ( mit != kPacket_.m_mapChangeMemberLevel.end() ) {

        for ( sit = mit->second.begin() ; sit != mit->second.end() ; ++sit ) {
            // 이 패킷에서는 가입대기자는 처리대상이 아니다.
            LIF( RemoveMember( *sit ) );
        }

        START_LOG( clog, L"길드원 강제탈퇴 처리. size : " << mit->second.size() ) << END_LOG;
    }

    // 맴버레벨 변경
    LIF( ChangeMemberLevel( kPacket_.m_mapChangeMemberLevel ) );

    // 맴버수 체크에 의한 가입설정변경
    AutoJoinSettingCheck();
    // 정식길드 변경 체크
    AutoUpgradeGuildGrade();
    // 길드맴버수 갱신
    m_kNGuildInfo.m_nNumMembers = GetMemberCount();

    SendToAll( KCenterEvent::ECN_CHANGE_GUILD_MEMBER_LEVEL_BROAD_NOT, kPacket );
}

IMPL_ON_FUNC( ECN_SELF_DRUMOUT_GUILD_ACK )
{
    // 길드원이 자진탈퇴시 불린다.
    KECN_REMOVE_GUILD_USER_BROAD_NOT kPacket;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
    kPacket.m_cOutType = KNGuildUserInfo::OT_SELF_OUT;

    // 대기자명단에서 제거
    LIF( RemoveMember( kPacket_.m_dwUserUID ) );

    START_LOG( clog, L"자진탈퇴로 유저 제거. GuildUID : " << kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;

    SendToAll( KCenterEvent::ECN_REMOVE_GUILD_USER_BROAD_NOT, kPacket );
}

IMPL_ON_FUNC( ECN_ACCEPT_GUILD_JOINER_ACK )
{
    // 길드원 가입신청 승인시 불린다.
    // 가입대기자 에서 일반길드원으로 맴버레벨 변경된다.
    KECN_CHANGE_GUILD_MEMBER_LEVEL_BROAD_NOT kPacket;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_mapChangeMemberLevel[ KNGuildUserInfo::GL_NORMAL ] = kPacket_.m_setUserUID;
    kPacket.m_cOutType = -1;

    // 맴버레벨 변경
    LIF( ChangeMemberLevel( kPacket.m_mapChangeMemberLevel ) );

    START_LOG( clog, L"길드원 가입신청 승인처리. size : " << kPacket_.m_setUserUID.size() ) << END_LOG;

    // 맴버수 체크에 의한 가입설정변경
    AutoJoinSettingCheck();
    // 정식길드 변경 체크
    AutoUpgradeGuildGrade();

    SendToAll( KCenterEvent::ECN_CHANGE_GUILD_MEMBER_LEVEL_BROAD_NOT, kPacket );
}

IMPL_ON_FUNC( ECN_REJECT_GUILD_JOINER_ACK )
{
    // 길드원 가입신청 거절시 불린다.
    // 탈퇴처리된다.
    std::set<DWORD>::iterator sit;

    KECN_CHANGE_GUILD_MEMBER_LEVEL_BROAD_NOT kPacket;
    kPacket.m_dwGuildUID = kPacket_.m_dwGuildUID;
    kPacket.m_mapChangeMemberLevel[ KNGuildUserInfo::GL_DRUMOUT ] = kPacket_.m_setUserUID;
    kPacket.m_cOutType = KNGuildUserInfo::OT_REJECT_JOIN;

    // 탈퇴처리
    for ( sit = kPacket_.m_setUserUID.begin() ; sit != kPacket_.m_setUserUID.end() ; ++sit ) {
        // 이 패킷에서는 가입대기자는 처리대상이 아니다.
        LIF( RemoveJoiner( *sit ) );
    }

    START_LOG( clog, L"길드원 가입신청거절 처리. size : " << kPacket_.m_setUserUID.size() ) << END_LOG;

    // 이거 수정되야할것 같다.
    // 맴버레벨변경 패킷이랑
    // 맴버삭제 패킷이랑 두종류로 나눠서 보내도록..
    SendToAll( KCenterEvent::ECN_CHANGE_GUILD_MEMBER_LEVEL_BROAD_NOT, kPacket );
}

_IMPL_ON_FUNC( ECN_CHANGE_GUILD_JOIN_SETTING_ACK, KCnNGuildJoinPolicy )
{
    // 길드가입 승인정책 변경시 불린다.
    START_LOG( clog, L"길드원 가입신청 설정 변경 전, Guild UID : " << m_kNGuildInfo.m_dwUID )
        << BUILD_LOGc( m_kNGuildInfo.m_ucJoinSetting ) << END_LOG;

    m_kNGuildInfo.m_ucJoinSetting = kPacket_.m_kPolicy.m_ucMethod;

    START_LOG( clog, L"길드원 가입신청 설정 변경 후, Guild UID : " << m_kNGuildInfo.m_dwUID )
        << BUILD_LOGc( m_kNGuildInfo.m_ucJoinSetting ) << END_LOG;

    SendToAll( KCenterEvent::ECN_CHANGE_GUILD_JOIN_SETTING_NOT, kPacket_.m_kPolicy );

    // 가입설정이 바로가입으로 변경시, 가입대기자 바로가입 승인.
    AutoAcceptJoinerList();
}

_IMPL_ON_FUNC( ECN_UPGRADE_GUILD_GRADE_ACK, int )
{
    // 공식길드로 만들어주자.
    m_kNGuildInfo.m_ucGrade = KNGuildInfo::GG_OFFICIAL;
    START_LOG( clog, L"길드등급 공식길드로 변경됨. GuildUID : " << m_kNGuildInfo.m_dwUID ) << END_LOG;

    // 길드승격 아이템요청
    UpgradeGuildGradeGiftReq();

    // 서버에 알림
    SendToAll( KCenterEvent::ECN_UPGRADE_GUILD_GRADE_BROAD_NOT, m_kNGuildInfo.m_dwUID );
}

IMPL_ON_FUNC( ECN_UPGRADE_GUILD_GRADE_GIFT_ACK )
{
    // DB에서 길마에게 변경권 지급여부 세팅 성공.
    char cServerUID = -1;

    {
        KLocker lock( m_csJoiner );
        // 길마 접속중인가?
        std::map< DWORD, KNGuildUserInfo >::iterator mit;
        mit = m_mapOnlineUsers.find( kPacket_.m_dwGuildMasterUID );
        if ( mit == m_mapOnlineUsers.end() ) {
            START_LOG( clog, L"길마가 접속중이 아님. GuildUID : " << kPacket_.m_dwGuildUID )
                << BUILD_LOG( kPacket_.m_dwGuildMasterUID ) << END_LOG;
            return;
        }
        cServerUID = mit->second.m_cServerUID;
    }


    // 길마가 접속해있으면 알려주자.
    std::map<DWORD,std::pair<KCenterPtr,int>>::iterator mitServer;
    {
        KLocker lock( m_csServers );
        mitServer = m_mapServers.find( (UCHAR)cServerUID );
        if ( mitServer == m_mapServers.end() ) {
            START_LOG( cerr, L"길마가 접속해있는 서버 정보가 없음. GuildUID : " << kPacket_.m_dwGuildUID )
                << BUILD_LOG( kPacket_.m_dwGuildMasterUID )
                << BUILD_LOGc( cServerUID ) << END_LOG;
            return;
        }
    }

    // 길마 접속 서버에 패킷 전달.
    mitServer->second.first->SendPacket( KCenterEvent::ECN_UPGRADE_GUILD_GRADE_GIFT_ACK, kPacket_ );

    START_LOG( clog, L"길마 접속서버에 길드승격에 따른 보상 아이템 받음 알림. GuildUID : " << kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwGuildMasterUID )
        << BUILD_LOGc( cServerUID ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_SET_GUILD_RANK_NOT, KSimpleGuildRank )
{
    // 길드 랭킹 데이터 설정.
    m_kNGuildInfo.m_nWeeklyRank = kPacket_.m_nWeeklyRank;
    m_kNGuildInfo.m_nMonthlyRank = kPacket_.m_nMonthlyRank;
    m_kNGuildInfo.m_nTotalRank = kPacket_.m_nTotalRank;

    START_LOG( clog, L"SetRank. GuildUID:" << kPacket_.m_dwGuildUID << L",Weekly:" << kPacket_.m_nWeeklyRank << L",Monthly:" << kPacket_.m_nMonthlyRank << L",Total:" << kPacket_.m_nTotalRank ) << END_LOG;
}

_IMPL_ON_FUNC( ECN_UPDATE_GUILD_POINT_REQ, KGuildPointBroad )
{
    // 해당 길드의 전체  길드 포인트 업데이트
    START_LOG( clog, L"길드의 전체 포인트에 더하기 전 , Point : " << m_kNGuildInfo.m_dwPoint )
        << BUILD_LOG( m_kNGuildInfo.m_GuildBattlePoint )
        << BUILD_LOG( kPacket_.m_dwDiffPoint )
        << BUILD_LOG( kPacket_.m_dwBattlePoint )
        << BUILD_LOG( kPacket_.m_nBattlePointDiff ) << END_LOG;

    {
        KLocker lock( m_csJoiner );

        m_kNGuildInfo.m_dwPoint += kPacket_.m_dwDiffPoint;
        m_kNGuildInfo.m_GuildBattlePoint += kPacket_.m_nBattlePointDiff;

        START_LOG( clog, L"길드의 전체 포인트에 더하기 후 , Point : " << m_kNGuildInfo.m_dwPoint )
            << BUILD_LOG( m_kNGuildInfo.m_GuildBattlePoint ) 
            << END_LOG;
        // 길드원의 길드 포인트 업데이트
        std::map< DWORD, KNGuildUserInfo >::iterator mit;
        mit = m_mapOnlineUsers.find( kPacket_.m_dwUserUID );

        if ( mit != m_mapOnlineUsers.end() ) {

            if ( kPacket_.m_dwUserGuildPoint > mit->second.m_dwContributePoint ) {
            }
            START_LOG( clog, L"길드원 포인트에 덮어쓰기 전 , Point: " << mit->second.m_dwContributePoint )
                << BUILD_LOG( kPacket_.m_dwUserGuildPoint )
                << END_LOG;

            mit->second.m_dwContributePoint = kPacket_.m_dwUserGuildPoint;

            START_LOG( clog, L"길드원 포인트에 덮어쓰기 후 , Point: " << mit->second.m_dwContributePoint ) << END_LOG;

        }
        else {
            START_LOG( cerr, L"길드 포인트 업데이트 하려는 유저가 온라인 목록에 없음, UserUID:" << kPacket_.m_dwUserUID ) << END_LOG;
        }
    }

    KGuildPointBroad kBroad;
    kBroad.m_GuildUID = kPacket_.m_GuildUID;
    kBroad.m_dwUserUID = kPacket_.m_dwUserUID;
    kBroad.m_dwDiffPoint = kPacket_.m_dwDiffPoint;
    kBroad.m_dwTotalPoint = m_kNGuildInfo.m_dwPoint;
    kBroad.m_dwBattlePoint = m_kNGuildInfo.m_GuildBattlePoint;
    kBroad.m_dwUserGuildPoint = kPacket_.m_dwUserGuildPoint;
    kBroad.m_nBattlePointDiff = kPacket_.m_nBattlePointDiff;

//    SendToAll( KCenterEvent::ECN_UPDATE_GUILD_POINT_BROAD, kBroad );
}

IMPL_ON_FUNC_NOPARAM( ECN_NEWBIE_GUILD_DESTROY_CHECK_NOT )
{
    // 신생길드 삭제 체크
    NewbieGuildDestroyCheck();
}

_IMPL_ON_FUNC( ECN_UPDATE_GUILD_MEMBER_NICK_NOT, KGuildUserName )
{
    // 맴버 닉네임 변경 알림
    KLocker lock( m_csJoiner );

    // 닉네임 정보 갱신
    std::vector< KNGuildUserInfo >::iterator vit;
    vit = std::find_if( m_vecJoinerByJoinDate.begin(), m_vecJoinerByJoinDate.end(),
        boost::bind(&KNGuildUserInfo::m_dwUserUID,_1) == kPacket_.m_dwUserUID );
    if ( vit != m_vecJoinerByJoinDate.end() ) {
        vit->m_strNickName = kPacket_.m_strNickName;
        vit->m_strNickColor = kPacket_.m_strNickColor;
    }

    std::map< DWORD, KNGuildUserInfo >::iterator mitJoiner;
    mitJoiner = m_mapJoiner.find( kPacket_.m_dwUserUID );
    if ( mitJoiner != m_mapJoiner.end() ) {
        mitJoiner->second.m_strNickName = kPacket_.m_strNickName;
        mitJoiner->second.m_strNickColor = kPacket_.m_strNickColor;
    }

    std::map< DWORD, KNGuildUserInfo >::iterator mitOnline;
    mitOnline = m_mapOnlineUsers.find( kPacket_.m_dwUserUID );
    if ( mitOnline != m_mapOnlineUsers.end() ) {
        mitOnline->second.m_strNickName = kPacket_.m_strNickName;
        mitOnline->second.m_strNickColor = kPacket_.m_strNickColor;
    }

    std::map< DWORD, KNGuildUserInfo >::iterator mitOffline;
    mitOffline = m_mapOfflineUsers.find( kPacket_.m_dwUserUID );
    if ( mitOffline != m_mapOfflineUsers.end() ) {
        mitOffline->second.m_strNickName = kPacket_.m_strNickName;
        mitOffline->second.m_strNickColor = kPacket_.m_strNickColor;
    }

    START_LOG( clog, L"길드원 닉네임 변경. Guild UID : " << kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_strNickName ) << END_LOG;

    // 길마면 길드정보도 갱신
    if ( kPacket_.m_dwUserUID == GetMasterUID() ) {
        m_kNGuildInfo.m_strMasterNick = kPacket_.m_strNickName;
        m_kNGuildInfo.m_strMasterNickColor = kPacket_.m_strNickColor;

        START_LOG( clog, L"길드마스터 닉네임 변경. Guild UID : " << kPacket_.m_dwGuildUID )
            << BUILD_LOG( kPacket_.m_dwUserUID )
            << BUILD_LOG( kPacket_.m_strNickName ) << END_LOG;
    }

    SendToAll( KCenterEvent::ECN_UPDATE_GUILD_MEMBER_NICK_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ECN_GUILD_MEMBER_LEVELUP_NOT, KGuildUserCharacter )
{
    // 맴버 레벨업 알림.
    START_LOG( clog, L"길드원 레벨업. Guild UID : " << kPacket_.m_dwGuildUID )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOGc( kPacket_.m_cCharType )
        << BUILD_LOGc( kPacket_.m_cLevel ) << END_LOG;

    SendToAll( KCenterEvent::ECN_GUILD_MEMBER_LEVELUP_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ECN_DB_GUILD_POINT_UPDATE_ACK, KGuildPointBroad )
{
    // 해당 길드의 전체  길드 포인트 업데이트
    START_LOG( clog, L"길드 포인트 DB 동기화 전 , Point : " << m_kNGuildInfo.m_dwPoint )
        << BUILD_LOG( m_kNGuildInfo.m_GuildBattlePoint )
        << BUILD_LOG( kPacket_.m_dwBattlePoint ) << END_LOG;

    {
        KLocker lock( m_csJoiner );

        m_kNGuildInfo.m_dwPoint = kPacket_.m_dwGuildPoint;
        m_kNGuildInfo.m_GuildBattlePoint = kPacket_.m_dwBattlePoint;
    }

    START_LOG( clog, L"길드 포인트 DB 동기화 후 , Point : " << m_kNGuildInfo.m_dwPoint )
        << BUILD_LOG( m_kNGuildInfo.m_GuildBattlePoint ) << END_LOG;

    KGuildPointBroad kBroad;
    kBroad.m_GuildUID = GetGuildUID();
    kBroad.m_dwTotalPoint = m_kNGuildInfo.m_dwPoint;
    kBroad.m_dwBattlePoint = m_kNGuildInfo.m_GuildBattlePoint;

//    SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_UPDATE_GUILD_POINT_BROAD, kBroad );
    //SendToAll( KCenterEvent::ECN_UPDATE_GUILD_POINT_BROAD, kBroad );
}
