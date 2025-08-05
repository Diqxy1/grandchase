#include "GuildManager.h"

#include "GSSimLayer.h"
#include "GameServer.h"
#include <boost/algorithm/string.hpp> //to_lower
#include "CnConnector.h"
#include "GSDBLayer.h"
#include "Log4.h"

namespace
{
    // 길드명 정렬
    class KSortName
    {
    public:
        bool operator() ( const KGuildPtr ptA, const KGuildPtr ptB )
        {
            if( ptA == NULL || ptB == NULL) return false;
            return ptA->GetGuildInfo().m_strName < ptB->GetGuildInfo().m_strName;
        }
    };

    // 등급별 정렬
    class KSortGrade
    {
    public:
        bool operator() ( const KGuildPtr ptA, const KGuildPtr ptB )
        {
            if( ptA == NULL || ptB == NULL) return false;
            return ptA->GetGuildInfo().m_ucGrade > ptB->GetGuildInfo().m_ucGrade;
        }
    };

    // 길드포인트별 정렬
    class KSortPoint
    {
    public:
        bool operator() ( const KGuildPtr ptA, const KGuildPtr ptB )
        {
            if( ptA == NULL || ptB == NULL) return false;
            return ptA->GetGuildInfo().m_dwPoint > ptB->GetGuildInfo().m_dwPoint;
        }
    };

    // ID(=Date) 정렬
    class KSortIDdate
    {
    public:
        bool operator() ( const KGuildPtr ptA, const KGuildPtr ptB )
        {
            if( ptA == NULL || ptB == NULL) return false;
            return ptA->GetGuildInfo().m_dwUID < ptB->GetGuildInfo().m_dwUID;
        }
    };
}

ImplementSingleton( KGuildManager );

#define EVENT_TYPE  KUserEvent
#define CLASS_TYPE  KGuildManager

KGuildManager::KGuildManager()
:m_kGuildClearTimer(1000*60*10, false)
,m_kMemberClearTimer(1000*60*5, false)
,m_kPageBuildTimer(1000*60, false)
,m_bReservePageBuild(false)
,m_bBuildEnable(false)
,m_bGuildClearEnable(false)
,m_nTotalGuildPageNum(0)
{
    SetName( GUILD_MANAGER_NAME );

    m_mapGuild.clear();
    m_setLiveGuildUIDList.clear();
    m_mapSimplePageInfo.clear();
    m_mapGuildPage.clear();
    m_mapCompressedRankData.clear();
    m_vecGuild.clear();

    // DayCheck 변수 오늘 날짜로 초기화.
    CTime tDate = CTime::GetCurrentTime();
    m_kLastDayCheckDate.SetDate( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 6  );
} // KGuildManager()

KGuildManager::~KGuildManager()
{
} // ~KGuildManager()

ImplOstmOperatorW2A( KGuildManager );
ImplToStringW( KGuildManager )
{
    KLocker lock( m_csMapGuild );
    KLocker lock1( m_csPage );
    KLocker lock2( m_csGuildRank );

    START_TOSTRINGW
        << TOSTRINGW( m_mapGuild.size() )
        << TOSTRINGW( m_setLiveGuildUIDList.size() )
        << TOSTRINGWb( m_bReservePageBuild )
        << TOSTRINGW( m_mapSimplePageInfo.size() )
        << TOSTRINGW( m_mapGuildPage.size() )
        << TOSTRINGW( m_mapCompressedRankData.size() );

    std::map<UCHAR,std::vector<DWORD> >::const_iterator cmit;
    for ( cmit = m_mapSimplePageInfo.begin() ; cmit != m_mapSimplePageInfo.end() ; ++cmit ) {
        stm_ << L" PageType : " << (int)cmit->first << L", size : " << cmit->second.size() << std::endl;
    }
    return stm_;
}

bool KGuildManager::LoadScript()
{
    return _LoadScript(std::string());
}

bool KGuildManager::_LoadScript( OUT std::string& strScript_ )
{
    const std::string strScriptName = "InitGuildStore.lua";
    strScript_ = strScriptName;
    _JIF( m_kGuildStoreItems.LoadScript( strScriptName.c_str() ), return false );
    return true;
}

void KGuildManager::DumpStoreList()
{
    //std::cout << L"--- Dump Guild Store Item ---" << dbg::endl;

    //std::map<PAIR_USHORT_DWORD,KManufactureItem> mapItemList;
    //if ( false == m_kGuildStoreItems.GetCatalogs( mapItemList ) ) {
    //    std::cout << L"--- End Dump---" << dbg::endl;
    //    return;
    //}

    //std::cout << L"ItemList size : " << mapItemList.size() << dbg::endl;

    //std::map<PAIR_USHORT_DWORD,KManufactureItem>::iterator mit;
    //for ( mit = mapItemList.begin() ; mit != mapItemList.end() ; ++mit ) {
    //    std::cout << std::tab << mit->second.m_Index << L"(" << mit->first.first << L") | "
    //        << mit->second.m_ItemID << L"(" << mit->first.second << L") | "
    //        << mit->second.m_nDuration << L" | "
    //        << mit->second.m_nPeriod << L" | "
    //        << mit->second.m_nPrice << L" - ";

    //    std::vector<KDropItemInfo> vecMaterials;
    //    LIF( m_kGuildStoreItems.GetMaterials( mit->first, vecMaterials ) );

    //    std::vector<KDropItemInfo>::iterator vitList;
    //    for ( vitList = vecMaterials.begin() ; vitList != vecMaterials.end() ; ++vitList ) {
    //        std::cout << std::tab << L"{" << vitList->m_ItemID << L", "
    //            << vitList->m_nDuration << L", "
    //            << vitList->m_nPeriod << "}, ";
    //    }
    //    std::cout << dbg::endl;
    //}

    //std::cout << L"--- End Dump---" << dbg::endl;
}

void KGuildManager::Tick()
{
    KPerformer::Tick();

    IntervalTick();
    //GetGuildInfoFromDB();   // 일정 주기마다 DB의 정보를 받아와 caching 정보를 동기화 해준다.
} // Tick()

void KGuildManager::IntervalTick()
{
    if ( GetGuildClearEnable() ) {
        if ( m_kMemberClearTimer.CheckTime() ) {
            // 접속자가 없는 길드 맴버리스트 제거.
            ClearMemberList();
            m_kMemberClearTimer.Reset();
        }

        if ( m_kGuildClearTimer.CheckTime() ) {
            // 길드원 수가 0명인 길드 정보 제거.
            ClearGuildInfo();
            m_kGuildClearTimer.Reset();
        }
    }

    if ( m_kPageBuildTimer.CheckTime() ) {
        // 페이지 빌드 예약되어 있으면 빌드요청.
        if ( GetReservePageBuild() == true ) {
            std::vector<DWORD> vecData;
            GetGuildPageIDListSort( KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_ID_DATE, vecData );
            SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_GUILD_PAGE_BUILD_NOT, L"GuildManager", 0, 0, vecData );
            SetReservePageBuild(false);
        }
    }

    // 길드 페이지 수정작업
    // 하루 지났을 경우 데이터 갱신이 필요하다.
    // UID, 길드이름, 등급, 길드 포인트들의 재빌드를 위해서..,
    if( DayCheck() && GetBuildCheck() ) {
        SendUpdateGuildListReqToDB();
    }
}

bool KGuildManager::Add( IN const KNGuildInfo& kGuildInfo_ )
{
    KGuildPtr spGuild = GetGuild( kGuildInfo_.m_dwUID );

    // 이미 등록된 길드는 정보만 갱신
    if ( spGuild ) {
        spGuild->SetInfo( kGuildInfo_ );
        return true;
    }

    // 길드추가
    spGuild.reset( new KGuild );
    spGuild->SetInfo( kGuildInfo_ );

    // 길드 매니저에 추가
    {
        KLocker lock( m_csMapGuild );

        std::map<DWORD, KGuildPtr>::iterator mit = m_mapGuild.find( kGuildInfo_.m_dwUID );
        if ( mit != m_mapGuild.end() ) {
            START_LOG( cwarn, L"해당 ID의 길드가 이미 존재함. GuildUID : " << kGuildInfo_.m_dwUID ) << END_LOG;
            return false;
        }

        m_mapGuild[ kGuildInfo_.m_dwUID ] = spGuild;
        m_setLiveGuildUIDList.insert( kGuildInfo_.m_dwUID );
        m_vecGuild.push_back( spGuild );
    }
    START_LOG( clog, L"길드매니저에 길드 포인터 추가. GuildUID : " << kGuildInfo_.m_dwUID ) << END_LOG;

    // 길드Page에 신규 길드UID 추가
    AddPageNewGuild( kGuildInfo_.m_dwUID );
    return true;
}
/*
void KGuildManager::AddGuild( IN std::map<DWORD,KNGuildInfo>& mapAddGuildList_ )
{
    // 길드매니저에 길드 추가
    std::map<DWORD,KNGuildInfo>::const_iterator cmit;
    for ( cmit = mapAddGuildList_.begin() ; cmit != mapAddGuildList_.end() ; ++cmit ) {
        LIF( Add( cmit->second ) );
    }

    START_LOG( clog, L"길드매니저에 길드 추가. size : " << mapAddGuildList_.size() ) << END_LOG;
}
*/
void KGuildManager::AddGuild( IN std::map<DWORD,KNGuildInfo>& mapAddGuildList_ )
{
    std::map< DWORD, KNGuildInfo >::iterator mit;
    for ( mit = mapAddGuildList_.begin() ; mit != mapAddGuildList_.end() ; ++mit ) {
        // 길드 존재여부 확인
        DWORD dwGuildUID = mit->first;
        KGuildPtr spGuild = GetGuild( dwGuildUID );
        if ( spGuild ) {
            START_LOG( cwarn, L"이미 존재하는 길드입니다, GuildUID : " << dwGuildUID ) << END_LOG;
            continue;
        }

        // 길드정보 만들고
        spGuild.reset( new KGuild );
        spGuild->SetInfo( mit->second );

        {
            KLocker lock( m_csPage );
            // 매니저에 등록
            if ( !m_mapGuild.insert( std::make_pair( dwGuildUID, spGuild ) ).second ) {
                START_LOG( cwarn, L"이미 존재하는 길드입니다, GuildUID : " << dwGuildUID ) << END_LOG;
                continue;
            }
            m_vecGuild.push_back( spGuild );
        }
    }
    // guild page sorting
    BuildGuildPageList();
    // 미사용 길드 정보 삭제.
    ClearGuildInfo();
}

void KGuildManager::CreateGuild( IN const KNGuildInfo& kInfo_, IN std::vector<KNGuildNotice>& vecNotice_, IN std::map<DWORD,KNGuildUserInfo>& mapMember_, IN KUserPtr spUser_ )
{
    // 길드 첫생성시 부른다.
    KNOneGuildData kData;
    kData.m_kGuildInfo = kInfo_;
    kData.m_vecGuildNotice = vecNotice_;
    kData.m_mapGuildUserList = mapMember_;

    // 매니저에 등록
    LIF( Add( kInfo_ ) );
    LIF( SetGuildNotice( kInfo_.m_dwUID, vecNotice_ ) );
    LIF( SetGuildMemberList( kInfo_.m_dwUID, mapMember_ ) );

    // 센터 길드 매니저에 길드 생성정보를 알림
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_ADD_NEW_GUILD_NOT, kData);

    START_LOG( clog, L"길드매니저에 첫 생성된 길드정보 추가. GuildUID : " << kData.m_kGuildInfo.m_dwUID )
        << BUILD_LOG( kData.m_kGuildInfo.m_strName )
        << BUILD_LOG( kData.m_kGuildInfo.m_strMasterNick )
        << BUILD_LOG( kData.m_vecGuildNotice.size() )
        << BUILD_LOG( kData.m_mapGuildUserList.size() ) << END_LOG;
}

void KGuildManager::Remove( IN const DWORD dwGuildUID_ )
{
    KLocker lock( m_csMapGuild );
    // 길드 삭제시
    std::map<DWORD, KGuildPtr>::iterator mit = m_mapGuild.find( dwGuildUID_ );
    if ( mit != m_mapGuild.end() ) {
        std::vector<KGuildPtr>::iterator vit = std::find( m_vecGuild.begin(), m_vecGuild.end(), mit->second );
        if( vit != m_vecGuild.end() ) { // std::vector 삭제되는 값에 대한 보정 처리.
            m_vecGuild.erase( vit );
        }
        m_mapGuild.erase( mit );
        START_LOG( clog, L"길드 삭제. GuildUID : " << dwGuildUID_ ) << END_LOG;
    }
    m_setLiveGuildUIDList.erase( dwGuildUID_ );
} // Remove()

bool KGuildManager::RemoveGuild( IN const DWORD dwGuildUID_ )
{
    // 길드삭제를 통보받으면 불린다.
    KGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( !spGuild ) {
        // 시간차로 이미 지워진 경우나, 정보가 로드안된 길드의 경우.
        START_LOG( cwarn, L"없는 길드ID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    // 길드원들에게 길드해산 알림
    PAIR_UCHAR kPacket;
    kPacket.first = spGuild->GetNGuildInfo().m_ucGrade; // 이전 상태
    kPacket.second = KNGuildInfo::GG_BREAKUP;           // 바뀌는 상태

    spGuild->QueueingToAll( KUserEvent::EVENT_UPDATE_GUILD_STATE_NOT, kPacket );

    // 길드매니저에서 삭제
    Remove( dwGuildUID_ );
    START_LOG( clog, L"길드매니저에서 길드 삭제. GuildUID : " << dwGuildUID_ ) << END_LOG;

    _LOG_CONDITION( spGuild.use_count() == 1, clog, cwarn, L"Ref Count : " << spGuild.use_count() ) << END_LOG;

    // 길드 페이지에서 제거
    RemovePageGuild( dwGuildUID_ );

    return true;
}

KGuildPtr KGuildManager::GetGuild( IN const DWORD dwGuildUID_ )
{
    KLocker lock( m_csMapGuild );

    std::map<DWORD, KGuildPtr>::iterator mit = m_mapGuild.find( dwGuildUID_ );
    if ( mit != m_mapGuild.end() ) {
        return mit->second;
    }

    START_LOG( cwarn, L"서버에 존재하지 않는 길드 UID :" << dwGuildUID_ ) << END_LOG;
    return KGuildPtr();
}

bool KGuildManager::GetGuildInfo( IN const DWORD dwGuildUID_, OUT KNGuildInfo& kGuildInfo_ )
{
    kGuildInfo_.Init();

    if ( dwGuildUID_ == 0 ) {
        START_LOG( cwarn, L"유효하지 않은 길드ID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    KGuildPtr spGuild = GetGuild( dwGuildUID_ );
    if ( !spGuild ) {
        START_LOG( cwarn, L"없는 길드ID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    kGuildInfo_ = spGuild->GetNGuildInfo();

    _JIF( !kGuildInfo_.m_strName.empty(), return false );

    return true;
}

bool KGuildManager::SetGuildNotice( IN const DWORD dwGuildUID_, IN std::vector<KNGuildNotice>& vecNotice_ )
{
    KGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( !spGuild ) {
        START_LOG( cwarn, L"없는 길드ID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    START_LOG( clog, L"센터서버에서 전달한 길드공지를 해당 길드에 전달. GuildUID :" << dwGuildUID_ << L", vecNotice_.size() : " << vecNotice_.size() ) << END_LOG;

    // 센터서버에서 전달한 길드 공지를 해당 길드에 전달
    spGuild->SetGuildNotice( vecNotice_ );
    return true;
}

bool KGuildManager::GetNotice( IN const DWORD dwGuildUID_, OUT std::vector<KNGuildNotice>& vecNotice_ )
{
    KGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( !spGuild ) {
        START_LOG( cwarn, L"없는 길드ID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    spGuild->GetNGuildNotice( vecNotice_ );
    return true;
}

bool KGuildManager::ModifyNotice( IN const DWORD dwGuildUID_, IN std::vector<KNGuildNotice>& vecNotice_ )
{
    KGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( !spGuild ) {
        START_LOG( cwarn, L"없는 길드ID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    // 센터서버에서 전달한 길드 공지를 해당 길드에 전달
    spGuild->ModifyGuildNotice( vecNotice_ );
    return true;
}

bool KGuildManager::ModifyGuildNotice( IN const DWORD dwGuildUID_, IN std::vector<KNGuildNotice>& vecNotice_ )
{
    // 로컬서버에서 수정된 길드 공지를 해당 길드에 전달
    _JIF( ModifyNotice( dwGuildUID_, vecNotice_ ), return false );

    // 바뀐 공지사항 센터 길드매니저에도 알림
    _JIF( SendModifyGuildNoticeNot( dwGuildUID_, vecNotice_ ), return false );
    return true;
}

bool KGuildManager::SendModifyGuildNoticeNot( IN const DWORD dwGuildUID_, IN std::vector<KNGuildNotice>& vecNotice_ )
{
    KECN_UPDATE_GUILD_NOTICE_NOT kPacket;
    kPacket.m_dwGuildUID = dwGuildUID_;
    kPacket.m_vecNotice = vecNotice_;

    _JIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_UPDATE_GUILD_NOTICE_NOT, kPacket ), return false );

    START_LOG( clog, L"센터서버에 길드공지사항 변경 알림, GuildUID : " << dwGuildUID_ << L",size : " << vecNotice_.size() ) << END_LOG;
    return true;
}

bool KGuildManager::SetGuildMemberList( IN const DWORD dwGuildUID_, IN std::map< DWORD, KNGuildUserInfo >& mapMemberList_ )
{
    KGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( !spGuild ) {
        START_LOG( cwarn, L"길드 매니저에 등록되지 않은 길드. GuildUID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    // 해당길드에 맴버 리스트 전달
    spGuild->SetNGuildMemberList( mapMemberList_ );
    return true;
}

void KGuildManager::SetGuildPageID( IN const UCHAR ucSortType_, IN const KSerBuffer& kBuf_ )
{
    std::vector<DWORD> vecPageID;
    vecPageID.clear();

    KSerializer ks;
    KSerBuffer kBuffer = kBuf_;
    kBuffer.UnCompress();
    ks.BeginReading( &kBuffer );
    ks.Get( vecPageID );
    ks.EndReading();

    START_LOG( clog, L"Sort Type : " << (int)ucSortType_ << L",PageID size : " << vecPageID.size() ) << END_LOG;

    {
        KLocker lock( m_csPage );
        m_mapSimplePageInfo[ucSortType_].swap( vecPageID );
    }

    // 길드 페이지 빌드 예약
    SetReservePageBuild(true);
}

void KGuildManager::UpdateGuildPage( IN const UCHAR ucSortType_, IN const UCHAR ucOrderBy_,IN const int nPageNum_, IN const KSerBuffer& kPageBuf_ )
{
    if ( ucSortType_ > KEVENT_SEARCH_GUILD_LIST_REQ::ST_MAX ) {
        START_LOG( cerr, L"정의되지 않은 정렬타입 : " << (int)ucSortType_ ) << END_LOG;
        return;
    }

    {
        KLocker lock( m_csPage );
        m_mapGuildPage[ std::make_pair(ucSortType_,ucOrderBy_) ][nPageNum_] = kPageBuf_;
    }
}

bool KGuildManager::IsGuildPageEmpty()
{
    KLocker lock( m_csPage );
    return m_mapGuildPage.empty();
}

bool KGuildManager::GetGuildPage( IN OUT UCHAR& ucSortType_, IN OUT UCHAR& ucOrderBy_, IN OUT int& nPageNum_, OUT KSerBuffer& kPage_, OUT int& nTotalPageNum_ )
{
    if ( IsGuildPageEmpty() ) {
        return true;
    }

    if ( ucSortType_ >= KEVENT_SEARCH_GUILD_LIST_REQ::ST_MAX ) {
        START_LOG( cerr, L"정의되지 않은 정렬타입 : " << (int)ucSortType_ ) << END_LOG;
        ucSortType_ = KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_ID_DATE; // 기본형으로 변경함.
    }

    if ( ucOrderBy_ >= EOB_MAX ) {
        START_LOG( cerr, L"정의되지 않은 차순 : " << (int)ucOrderBy_ ) << END_LOG;
        ucOrderBy_ = EOB_ASC; // 기본형으로 변경함.
    }

    return GetCurrentPage( ucSortType_, ucOrderBy_, nPageNum_, kPage_, nTotalPageNum_ );
}

bool KGuildManager::GetCurrentPage( IN const UCHAR ucSortType_, IN const UCHAR ucOrderBy_, IN OUT int& nPageNum_, OUT KSerBuffer& kPage_, OUT int& nTotalPageNum_ )
{
    std::map<int,KSerBuffer> mapPage;
    {
        KLocker lock( m_csPage );
        mapPage = m_mapGuildPage[ std::make_pair(ucSortType_,ucOrderBy_) ];
    }

    kPage_.Clear();
    nTotalPageNum_ = GetTotalGuildPageNum();

    // 페이지가 없는경우(=길드가 하나도 없는 경우)
    if ( mapPage.empty() ) {
        START_LOG( cwarn, L"길드 페이지가 없음. size : " << mapPage.size() << L", ucSortType_ : " << (int)ucSortType_ << L", OrderBy : " << (int)ucOrderBy_ ) << END_LOG;
        nPageNum_ = 0;
        return true;
    }

    // 페이지수 보장
    nPageNum_ = std::max<int>( 1, nPageNum_ );

    // 최대 페이지보다 많은 경우, 1페이지로 돌려줌.
    if ( nPageNum_ > nTotalPageNum_ ) {
        START_LOG( cwarn, L"요청한 길드페이지가 최대 페이지보다 크다. PageNum : " << nPageNum_ ) << END_LOG;
        nPageNum_ = mapPage.begin()->first;
        kPage_ = mapPage.begin()->second;
    }

    std::map<int,KSerBuffer>::const_iterator cmit;
    cmit = mapPage.find( nPageNum_ );

    // 요청한 페이지가 정상적으로 있는 경우.
    if ( cmit != mapPage.end() ) {
        kPage_ = cmit->second;
        return true;
    }

    // 빌드가 안된 Page를 요청했음.
    START_LOG( cwarn, L"빌드가 안된 Page를 요청했음. PageNum : " << nPageNum_ << L",TotalPageNum : " <<nTotalPageNum_ ) << END_LOG;
    return false;
}

void KGuildManager::GetGuildSearchPage( IN const int nPageNum_, IN std::vector<DWORD>& vecGuildUID_, OUT std::vector<KNGuildInfo>& vecGuildInfo_ )
{
    // 길드검색의 경우, 페이지 요청을 처리
    std::vector<DWORD>::iterator vit;
    std::map<DWORD, KGuildPtr>::iterator mit;
    int nPageSize = 0;
    int nCurrentPage = 1;
    vecGuildInfo_.clear();

    if ( vecGuildUID_.empty() ) {
        START_LOG( cwarn, L"길드 포인터 리스트가 비어있음." ) << END_LOG;
        return;
    }

    // 길드포인터를 돌면서 일치하는 길드 정보를 받아옴.
    for ( vit = vecGuildUID_.begin() ; vit != vecGuildUID_.end() ; ++vit ) {

        // 한페이지 정보를 다 넣었는가?
        if ( vecGuildInfo_.size() >= (UINT)GetGuildPageSize() )
            break;

        // 길드UID 검색
        {
            KLocker lock( m_csMapGuild );

            mit = m_mapGuild.find( *vit );
            if ( mit == m_mapGuild.end() )
                continue;

            // 페이지 사이즈증가
            ++nPageSize;

            // 페이지 사이즈가 다차면 현재 페이지 1증가
            if ( nPageSize > GetGuildPageSize() ) {
                ++nCurrentPage;
                nPageSize = 0;
            }

            // 요청한 페이지와 현재 페이지 확인
            if ( nCurrentPage < nPageNum_ )
                continue;

            // 여기서부터 요청한 페이지의 정보이니 넣어주자.
            vecGuildInfo_.push_back( mit->second->GetNGuildInfo() );
        }

    }

    START_LOG( clog, L"검색한 길드 페이지 정보를 다 담았음. size : " << vecGuildInfo_.size() ) << END_LOG;
}

bool KGuildManager::AddUser( IN const KNGuildUserInfo& kUserInfo_ )
{
    // 길드매니저에 유저 정보 추가
    KGuildPtr spGuild = GetGuild( kUserInfo_.m_dwGuildUID );
    if ( !spGuild ) {
        START_LOG( cwarn, L"길드 매니저에 길드 없음. GuildUID : " << kUserInfo_.m_dwGuildUID ) << END_LOG;
            return false;
        }

    // 길드에 유저추가
    if ( spGuild->AddNewUser( kUserInfo_ ) == false ) {
        START_LOG( cwarn, L"이미 등록된 길드원. GuildUID : " << kUserInfo_.m_dwGuildUID )
            << BUILD_LOG( kUserInfo_.m_strLogin )
            << BUILD_LOGc( kUserInfo_.m_cMemberLevel ) << END_LOG;
        return false;
    }

    START_LOG( clog, L"길드매니저에 새 길드원 정보 추가함." )
        << BUILD_LOG( kUserInfo_.m_dwGuildUID )
        << BUILD_LOG( kUserInfo_.m_strLogin )
        << BUILD_LOGc( kUserInfo_.m_cServerUID ) << END_LOG;

    // 일반 길드원 이상 가입시 인원수 정보 페이지 갱신
    if ( kUserInfo_.m_cMemberLevel > KNGuildUserInfo::GL_JOINER ) {
        UpdateGuildPageInfo( kUserInfo_.m_dwGuildUID );
    }

    return true;
}

bool KGuildManager::Register( IN const DWORD dwGuildUID_, IN KUserPtr spUser_ )
{
    // 유저가 게임서버 접속시, 호출된다.
    KGuildPtr spGuild = GetGuild( dwGuildUID_ );
    // 센터에서 이미 다 가지고 온 상태니까, 있어야 한다.
    if ( !spGuild ) {
        START_LOG( cwarn, L"길드 매니저에 길드 없음. GuildUID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    // 길드에 길드원 입장.
    if ( spGuild->Enter( spUser_ ) == false ) {
        START_LOG( cwarn, L"길드에 길드원 입장 실패. GuildUID : " << dwGuildUID_ )
            << BUILD_LOG( spUser_->GetName() ) << END_LOG;
        return false;
    }

    // 성공하면 센터서버에 알림
    KECN_ADD_GUILD_USER_REQ kCnPacket;
    spGuild->GetNGuildUserInfo( spUser_->GetUID(), kCnPacket.m_kUserInfo );
    kCnPacket.m_dwGuildUID = dwGuildUID_;
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_ADD_GUILD_USER_REQ, kCnPacket );

    START_LOG( clog, L"길드매니저에 새 길드원 정보 추가함." )
        << BUILD_LOG( dwGuildUID_ )
        << BUILD_LOG( spUser_->GetName() ) << END_LOG;

    return true;
} // Register()

void KGuildManager::Unregister( IN KUserPtr spUser_ )
{
    // 유저가 게임서버 종료시, 호출된다.
    if ( !spUser_ )
        return;

    // 유저의 길드원 정보를 불러옴.
    KNGuildUserInfo kGuildUserInfo;
    spUser_->GetGuildUserInfo( kGuildUserInfo );

    if ( kGuildUserInfo.m_dwGuildUID == 0 )
    {
        START_LOG( clog, L"Guild에 가입되지 않은 사용자. Login : " << spUser_->GetName() ) << END_LOG;
        return;
    }

    KGuildPtr spGuild;
    spGuild = GetGuild( kGuildUserInfo.m_dwGuildUID );
    if ( !spGuild )
    {
        START_LOG( cwarn, L"Unregister하려는 사용자의 길드가 길드 매니저에 없음." )
            << BUILD_LOG( kGuildUserInfo.m_dwGuildUID )
            << BUILD_LOG( spUser_->GetName() )
            << END_LOG;
        return;
    }

    START_LOG( clog, L"Guild유저 접속종료처리. Login : " << spUser_->GetName() ) << END_LOG;

    // 해당 길드에서 유저의 포인터 삭제(정보갱신)
    spGuild->Leave( spUser_ ); 

    if ( spGuild->IsEmpty() )
    {
        //Remove( kGuildUserInfo.m_dwGuildUID ); // 정책상 생성된 길드 정보는 유지한다.
        // 해당 User의 OnDestroy에서 센터로 자신이 나갔음을 알려서 길드 정보를 갱신한다.
    }
} // Unregister()

bool KGuildManager::LogOffUser( IN const DWORD dwGuildUID_, IN const DWORD dwUserUID_ )
{
    KGuildPtr spGuild;
    spGuild = GetGuild( dwGuildUID_ );
    if ( !spGuild )
    {
        START_LOG( cwarn, L"로그오프 하려는 사용자의 길드가 길드 매니저에 없음." )
            << BUILD_LOG( dwGuildUID_ )
            << BUILD_LOG( dwUserUID_ )
            << END_LOG;
        return false;
    }

    // 길드에 유저 로그오프 요청
    spGuild->LogOffUser( dwUserUID_ );
    START_LOG( clog, L"길드매니저에서 유저 로그오프 통보받음. GuildUID : " << dwGuildUID_ << L",UserUID : " << dwUserUID_ ) << END_LOG;
    return true;
}

bool KGuildManager::DeleteUser( IN const DWORD dwGuildUID_, IN const DWORD dwUserUID_, IN const char cOutType_ )
{
    // 길드원 정보 영구삭제.
    KGuildPtr spGuild;
    spGuild = GetGuild( dwGuildUID_ );
    if ( !spGuild )
    {
        START_LOG( cwarn, L"삭제하려는 사용자의 길드가 길드 매니저에 없음." )
            << BUILD_LOG( dwGuildUID_ )
            << BUILD_LOG( dwUserUID_ )
            << BUILD_LOGc( cOutType_ )
            << END_LOG;
        return false;
    }

    // 해당길드에서 유저정보 삭제
    spGuild->DeleteUser( dwUserUID_, cOutType_ );
    // 길드 페이지 정보 갱신
    UpdateGuildPageInfo( dwGuildUID_ );

    START_LOG( clog, L"매니저에서 해당 유저UID의 길드정보 삭제." )
        << BUILD_LOG( dwGuildUID_ )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOGc( cOutType_ ) << END_LOG;
    return true;
}

void KGuildManager::DrumoutUser( IN const DWORD dwGuildUID_, IN std::set<DWORD>& setUserUID_, IN const char cOutType_ )
{
    // 길드탈퇴 혹은 길드에서 추방 되면 부른다.
    if ( setUserUID_.empty() ) return;

    std::set<DWORD>::iterator sit;
    for ( sit = setUserUID_.begin() ; sit != setUserUID_.end() ; ++sit ) {
        LIF( DeleteUser( dwGuildUID_, *sit, cOutType_ ) );
    }

    // 센터서버에 알림
    LIF( SendDrumoutGuildUserNot( dwGuildUID_, setUserUID_, cOutType_ ) );
}

bool KGuildManager::SendDrumoutGuildUserNot( IN const DWORD dwGuildUID_, IN std::set<DWORD>& setUID_, IN const char cOutType_ )
{
    KECN_DRUMOUT_GUILD_USER_NOT kPacket;
    kPacket.m_dwGuildUID = dwGuildUID_;
    kPacket.m_setUserUID = setUID_;
    kPacket.m_cOutType = cOutType_;

    _JIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_DRUMOUT_GUILD_USER_NOT, kPacket ), return false );

    START_LOG( clog, L"센터서버에 길드원 탈퇴/추방 알림, GuildUID : " << dwGuildUID_ << L",size : " << setUID_.size() << L",OutType : " << (int)cOutType_ ) << END_LOG;
    return true;
}

bool KGuildManager::ChangeGuildMemberLevel( IN const DWORD dwGuildUID_, IN const std::map<char,std::set<DWORD> >& mapChangeMemberLevel_ )
{
    // 길드 맴버레벨 변경
    std::map<char,std::set<DWORD> >::const_iterator mit;

    KGuildPtr spGuild;
    spGuild = GetGuild( dwGuildUID_ );
    if ( !spGuild )
    {
        START_LOG( cwarn, L"대상 길드가 길드 매니저에 없음." )
            << BUILD_LOG( dwGuildUID_ )
            << END_LOG;
        return false;
    }

    // 맴버레벨 변경
    mit = mapChangeMemberLevel_.find( KNGuildUserInfo::GL_NORMAL );
    if ( mit != mapChangeMemberLevel_.end() ) {
        if ( !( mit->second.empty() ) ) {
            // 일반길드원으로 변경
            spGuild->ChangeMemberLevel( KNGuildUserInfo::GL_NORMAL, mit->second );
            START_LOG( clog, L"일반유저로 맴버레벨 변경. size : " << mit->second.size() ) << END_LOG;
        }
    }

    mit = mapChangeMemberLevel_.find( KNGuildUserInfo::GL_GENERAL );
    if ( mit != mapChangeMemberLevel_.end() ) {
        if ( !( mit->second.empty() ) ) {
            // 제너럴로 변경
            spGuild->ChangeMemberLevel( KNGuildUserInfo::GL_GENERAL, mit->second );
            START_LOG( clog, L"제너럴로 맴버레벨 변경. size : " << mit->second.size() ) << END_LOG;
        }
    }

    mit = mapChangeMemberLevel_.find( KNGuildUserInfo::GL_MASTER );
    if ( mit != mapChangeMemberLevel_.end() ) {
        // 길드마스터는 1명만 존재할 수 있다.
        if ( !( mit->second.empty() ) && mit->second.size() == 1 ) {
            // 마스터로 변경
            spGuild->ChangeMemberLevel( KNGuildUserInfo::GL_MASTER, mit->second );
            START_LOG( clog, L"마스터로 맴버레벨 변경. size : " << mit->second.size() ) << END_LOG;
        }
    }

    START_LOG( clog, L"유저의 맴버레벨 변경됨. GuildUID : " << dwGuildUID_ ) << END_LOG;
    return true;
}

void KGuildManager::ChangeMemberLevel( IN const DWORD dwGuildUID_, IN const std::map<char,std::set<DWORD> >& mapChangeMemberLevel_ )
{
    // 길드원 맴버레벨 변경시 부른다.
    LIF( ChangeGuildMemberLevel( dwGuildUID_, mapChangeMemberLevel_ ) );

    // 센터서버에 알림
    LIF( SendChangeGuildMemberLevelNot( dwGuildUID_, mapChangeMemberLevel_ ) );
}

bool KGuildManager::SendChangeGuildMemberLevelNot( IN const DWORD dwGuildUID_, IN const std::map<char,std::set<DWORD> >& mapChangeMemberLevel_ )
{
    // 센터서버에 길드원 맴버 변경을 알린다.
    KECN_CHANGE_GUILD_MEMBER_LEVEL_NOT kPacket;
    kPacket.m_dwGuildUID = dwGuildUID_;
    kPacket.m_mapChangeMemberLevel = mapChangeMemberLevel_;

    _JIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_CHANGE_GUILD_MEMBER_LEVEL_NOT, kPacket ), return false );

    START_LOG( clog, L"센터서버에 길드원 맴버레벨 변경 알림, GuildUID : " << dwGuildUID_ << L",size : " << mapChangeMemberLevel_.size() ) << END_LOG;
    return true;
}

//bool KGuildManager::UpdateGuildPoint( IN const DWORD dwGuildUID_, IN const DWORD dwPoint_ )
//{
//    if ( !SiKGSSimLayer()->CheckTypeFlag( ST_GUILD ) ) return true;
//
//    // 060614. florist. note : update 되는 point를 caching하지 않고 바로 DB에 적게 되있다.
//    // 처음부터 잘못 작성되었다... 나중엔 GuildManager가 아닌 Guild 각자가 Performer가 되어야 한다.
//
//    KGuildPtr spGuild = GetGuild( dwGuildUID_ );
//
//    if ( !spGuild ) {
//        START_LOG( cerr, L"guild point를 갱신하려는 길드가 길드 매니저에 없음." )
//            << BUILD_LOG( dwGuildUID_ )
//            << END_LOG;
//
//        return false;
//    }
//
//    if ( dwPoint_ == 0 ) {
//        START_LOG( cerr, L"길드 포인트 증가값이 없음." )
//            << BUILD_LOG( dwGuildUID_ )
//            << BUILD_LOG( dwPoint_ )
//            << END_LOG;
//
//        return false;
//    }
//
//    KGuildGameResult kResult;
//    kResult.m_dwGuildUID  = dwGuildUID_;
//    kResult.m_dwPoint    = dwPoint_;
//    SiKGameServer()->GetDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_UPDATE_GUILD_POINT_REQ, m_strName.c_str(), 0, kResult );
//
//    return true;
//}

bool KGuildManager::UpdateGuildPoint( IN const DWORD dwGuildUID_, IN const DWORD dwPoint_, IN const DWORD dwUserUID_, IN const int nWin_, IN const int nLose_ )
{
    //if ( !SiKGSSimLayer()->CheckTypeFlag( ST_GUILD ) ) {
    //    return true;
    //}
    // 060614. florist. note : update 되는 point를 caching하지 않고 바로 DB에 적게 되있다.
    // 처음부터 잘못 작성되었다... 나중엔 GuildManager가 아닌 Guild 각자가 Performer가 되어야 한다.

    KGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( !spGuild ) {
        START_LOG( cerr, L"guild point를 갱신하려는 길드가 길드 매니저에 없음." )
            << BUILD_LOG( dwGuildUID_ )
            << END_LOG;

        return false;
    }

    if ( dwPoint_ == 0 ) {
        START_LOG( cwarn, L"길드 포인트 증가값이 없음." )
            << BUILD_LOG( dwGuildUID_ )
            << BUILD_LOG( dwPoint_ )
            << END_LOG;

        return false;
    }

    spGuild->AddGuildPoint( dwPoint_ );
    spGuild->UpdateMemberGuildPoint( dwUserUID_, dwPoint_ );

    KGuildGameResult kResult;
    kResult.m_dwGuildUID  = dwGuildUID_;
    kResult.m_dwPoint    = dwPoint_;
    kResult.m_nWin = nWin_;
    kResult.m_nLose = nLose_;
    kResult.m_dwUserUID = dwUserUID_;
    kResult.m_nBPointDiff = 0;

    SiKGameServer()->GetDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_UPDATE_GUILD_POINT_REQ, m_strName.c_str(), 0, 0, kResult );

    return true;
}

void KGuildManager::UpdateBattlePoint( IN const DWORD dwGuildUID_, IN const int point_ )
{
    KLocker lock( m_csMapGuild );

    std::map<DWORD, KGuildPtr>::iterator mit = m_mapGuild.find( dwGuildUID_ );
    if ( mit != m_mapGuild.end() ) {

        int appliedPoint = point_;
        int bPoint = static_cast<int>( mit->second->GetBattlePoint() );
        if ( bPoint + appliedPoint < 0 ) { // 점수(결과)가 마이너스 되는 것을 막아보자
            appliedPoint = -bPoint;
        }

        if ( bPoint + appliedPoint > GetMaxBattlePoint() ) { // 최대 배틀 포인트 초과를 막아보자
            appliedPoint = GetMaxBattlePoint() - bPoint;
        }

        KGuildGameResult kResult;
        kResult.m_dwGuildUID = dwGuildUID_;
        kResult.m_nBPointDiff = appliedPoint;
        SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_UPDATE_GUILD_BATTLE_POINT_REQ, L"GuildManager", 0, 0, kResult );

        mit->second->UpdateBattlePoint( appliedPoint );
    }
}


void KGuildManager::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    DWORD dwElapTime = ::GetTickCount();

    switch( spEvent_->m_usEventID )
    {
   _CASE( DB_EVENT_UPDATE_GUILD_POINT_ACK, KGuildGameResult );

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString() ) << END_LOG;
    }

    dwElapTime = ::GetTickCount() - dwElapTime;
    if ( dwElapTime > 500 ) {
        START_LOG_WITH_NAME( cwarn )
            << BUILD_LOG( spEvent_->GetEventIDString() )
            << BUILD_LOG( spEvent_->m_usEventID )
            << BUILD_LOG( dwElapTime )
            << END_LOG;
    }
} // ProcessEvent()

void KGuildManager::BroadChat( IN const DWORD dwGuildUID_, IN KChatData& kChat_ )
{
    KGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( spGuild == NULL ) {
        START_LOG( cwarn, L"길드 정보가 없는데, 채팅 요청함")
            << BUILD_LOG( dwGuildUID_ )
            << BUILD_LOG( kChat_.m_dwSenderUID )
            << BUILD_LOG( kChat_.m_strSenderNick ) << END_LOG;
        return;
    }

    spGuild->SendToAllexJoiner( KUserEvent::EVENT_CHAT_NOT, kChat_ );

    START_LOG( clog, L"길드 채팅")
        << BUILD_LOG( dwGuildUID_ )
        << BUILD_LOG( kChat_.m_dwSenderUID )
        << BUILD_LOG( kChat_.m_strSenderNick ) << END_LOG;

}

_IMPL_ON_FUNC( DB_EVENT_UPDATE_GUILD_POINT_ACK, KGuildGameResult )
{
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( !IS_CORRECT( NET_OK) ) {
        START_LOG( cerr, L"길드 포인트 업데이트 실패")
            << BUILD_LOG( NetError::GetLastNetErrMsg() )
            << BUILD_LOG( kPacket_.m_dwGuildUID )
            << BUILD_LOG( kPacket_.m_dwUserUID ) << END_LOG;
        return;
    }

    KGuildPtr spGuild = GetGuild( kPacket_.m_dwGuildUID );
    if ( !spGuild ) {
        START_LOG( cwarn, L"guild point를 갱신한 길드가 길드 매니저에 없음." )
            << BUILD_LOG( kPacket_.m_dwGuildUID ) << END_LOG;
    }
    else {
        KGuildPointBroad kPacket;
        kPacket.m_dwDiffPoint = kPacket_.m_dwPoint;
        kPacket.m_dwUserUID = kPacket_.m_dwUserUID;
        kPacket.m_GuildUID = kPacket_.m_dwGuildUID;
        kPacket.m_dwBattlePoint = spGuild->GetBattlePoint();
        kPacket.m_dwGuildPoint = spGuild->GetGuildPoint();
        kPacket.m_dwUserGuildPoint = spGuild->GetMemberGuildPoint( kPacket_.m_dwUserUID );
        kPacket.m_nBattlePointDiff = kPacket_.m_nBPointDiff;

        SiKCnConnector()->SendPacket( KCenterEvent::ECN_UPDATE_GUILD_POINT_REQ, kPacket );
    }
}

bool KGuildManager::CheckGuildMarkExt( IN const std::wstring& strExt_ )
{
    KLocker lock( m_csConfig );
    return m_kGuildConfig.m_setExt.find( strExt_ ) != m_kGuildConfig.m_setExt.end();
}

bool KGuildManager::CheckGuildMarkSize( IN const DWORD dwMarkSize_ )
{
    KLocker lock( m_csConfig );
    return (m_kGuildConfig.m_prMarkSizeLimit.first < dwMarkSize_ && m_kGuildConfig.m_prMarkSizeLimit.second < dwMarkSize_);
}

GCITEMID KGuildManager::GetGuildMarkItemID()
{
    KLocker lock( m_csConfig );
    return m_kGuildConfig.m_dwMarkUploadItemID;
}

int KGuildManager::GetGuildMarkItemFactor()
{
    KLocker lock( m_csConfig );
    return m_kGuildConfig.m_nMarkUploadItemFactor;
}

GCITEMID KGuildManager::GetGuildMarkCashItemID()
{
    KLocker lock( m_csConfig );
    return m_kGuildConfig.m_dwMarkUploadCashItemID;
}

void KGuildManager::MakeGuildMarkName( IN const DWORD dwGuildUID_, IN const int nRevision_,
                                       IN const std::wstring& strExt_, OUT std::wstring& strFileName_ )
{
    if ( nRevision_ != 0 ) {
        std::wstringstream stm;
        stm << dwGuildUID_ << L"_" << nRevision_ << L"." << strExt_;
        strFileName_ = stm.str();
        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.
    }
    else {
        KLocker lock( m_csConfig );
        strFileName_ = m_kGuildConfig.m_strDefaultMarkName;
    }
    
    boost::to_lower( strFileName_ );
}

bool KGuildManager::SetMemberComment( IN const DWORD dwGuildUID_, IN const DWORD dwUserUID_, IN const std::wstring& strMyComment_ )
{
    // 길드원이 자기 소갯글 변경시 불린다.
    KGuildPtr spGuild = GetGuild( dwGuildUID_ );
    if ( !spGuild ) {
        START_LOG( cwarn, L"길드 매니저에 길드 없음. GuildUID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    // 길드에 정보 적용
    LIF( spGuild->ModifyMemberComment( dwUserUID_, strMyComment_ ) );
    return true;
}

bool KGuildManager::ModifyMemberComment( IN const DWORD dwGuildUID_, IN const DWORD dwUserUID_, IN const std::wstring& strMyComment_ )
{
    _JIF( SetMemberComment( dwGuildUID_, dwUserUID_, strMyComment_ ), return false );

    // 센터서버에도 알린다.
    KECN_UPDATE_GUILD_MEMBER_COMMENT_NOT kPacket;
    kPacket.m_dwGuildUID = dwGuildUID_;
    kPacket.m_dwUserUID = dwUserUID_;
    kPacket.m_strUserComment = strMyComment_;
    SiKCnConnector()->SendPacket( KCenterEvent::ECN_UPDATE_GUILD_MEMBER_COMMENT_NOT, kPacket );

    START_LOG( clog, L"길드매니저에 길드원 소갯말 적용됨." )
        << BUILD_LOG( dwGuildUID_ )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOG( strMyComment_ ) << END_LOG;
    return true;
}

bool KGuildManager::ModifyGuildName( IN const DWORD dwGuildUID_, IN std::wstring& strGuildName_ )
{
    // 로컬서버에서 수정된 길드 이름을 해당 길드에 전달
    _JIF( ModifyName( dwGuildUID_, strGuildName_ ), return false );

    // 바뀐 길드 이름 센터 길드매니저에도 알림
    _JIF( SendModifyGuildNameNot( dwGuildUID_, strGuildName_ ), return false );
    return true;
}

bool KGuildManager::ModifyName( IN const DWORD dwGuildUID_, IN std::wstring& strGuildName_ )
{
    KGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( spGuild == NULL ) {
        START_LOG( cwarn, L"길드명을 변경하려는 대상 길드가 없음. GuildUID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    // 센터서버에서 전달한 길드이름을 해당 길드에 전달
    spGuild->ModifyGuildName( strGuildName_ );
    // 길드 정보 갱신
    UpdateGuildPageInfo( dwGuildUID_ );
    return true;
}

bool KGuildManager::SendModifyGuildNameNot( IN const DWORD dwGuildUID_, IN std::wstring& strGuildName_ )
{
    KECN_UPDATE_GUILD_NAME_NOT kPacket;
    kPacket.m_dwGuildUID = dwGuildUID_;
    kPacket.m_strGuildName = strGuildName_;

    _JIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_UPDATE_GUILD_NAME_NOT, kPacket ), return false );

    START_LOG( clog, L"센터서버에 길드이름 변경 알림, GuildUID : " << dwGuildUID_ << L", Name : " << strGuildName_ ) << END_LOG;
    return true;
}

void KGuildManager::SetGuildRankData( IN std::map< DWORD, KSerBuffer >& mapGuildRank_ )
{
    KLocker lock( m_csGuildRank );
    m_mapCompressedRankData.swap( mapGuildRank_ );
}

void KGuildManager::GetGuildRankData( IN const int nType, OUT KSerBuffer& buffGuildRank_ )
{
    std::map< DWORD, KSerBuffer >::const_iterator mit;
    KLocker lock( m_csGuildRank );
    mit = m_mapCompressedRankData.find( nType );
    if ( mit == m_mapCompressedRankData.end() ) {
        return;
    }
    buffGuildRank_ = mit->second;
}

bool KGuildManager::ChangeURLToCenter( KNGuildURL& kGuildURL_ )
{
    // 로컬에서 수정된 길드 URL 을 해당 길드에 전달.
    _JIF( ChangeURL( kGuildURL_ ), return false );

    // 바뀐 URL 을 센터 길드매니저에 알림
    _JIF( SendChangeGuildURLNot( kGuildURL_ ), return false );
    return true;
}

bool KGuildManager::ChangeURL( KNGuildURL& kGuildURL_ )
{
    KGuildPtr spGuild = GetGuild( kGuildURL_.m_dwUID );

    if ( !spGuild ) {
        START_LOG( cwarn, L"없는 길드ID : " << kGuildURL_.m_dwUID ) << END_LOG;
        return false;
    }

    spGuild->ChangeURL( kGuildURL_.m_strURL );
    // 길드 페이지 정보 갱신
    UpdateGuildPageInfo( kGuildURL_.m_dwUID );
    return true;
}

bool KGuildManager::SendChangeGuildURLNot( KNGuildURL& kGuildURL_ )
{
    _JIF( SiKCnConnector()->SendPacket( KCenterEvent::ECN_UPDATE_GUILD_URL_NOT, kGuildURL_ ), return false );

    START_LOG( clog, L"센터서버에 길드 URL 변경 알림, Guild UID :" << kGuildURL_.m_dwUID )
        << BUILD_LOG( kGuildURL_.m_strURL ) << END_LOG;

    return true;
}

bool KGuildManager::ChangeJoinPolicy( KNGuildJoinPolicy& kPacket_ )
{
    KGuildPtr spGuild = GetGuild( kPacket_.m_dwUID );

    if ( spGuild == NULL ) {
        START_LOG( cwarn, L"가입설정을 변경하려는 대상 길드가 없음. GuildUID : " << kPacket_.m_dwUID ) << END_LOG;
        return false;
    }

    spGuild->ChangeJoinPolicy( kPacket_ );
    // 길드 페이지 정보 갱신
    UpdateGuildPageInfo( kPacket_.m_dwUID );
    return true;
}


void KGuildManager::SetGuildConfig( IN const KGuildConfig& kConfig_ )
{
    {
        KLocker lock( m_csConfig );
        m_kGuildConfig = kConfig_;
    }
    m_kGuildClearTimer.SetInterval( kConfig_.m_dwGuildInfoClearTerm );
    m_kMemberClearTimer.SetInterval( kConfig_.m_dwMemberListClearTerm );
}

bool KGuildManager::ChangeGuildGrade( IN const DWORD dwGuildUID_, IN const UCHAR ucGrade_ )
{
    KGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( !spGuild ) {
        START_LOG( cwarn, L"없는 길드ID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    spGuild->ChangeGrade( ucGrade_ );
    // 길드 페이지 정보 갱신
    UpdateGuildPageInfo( dwGuildUID_ );

    START_LOG( clog, L"길드등급 변경 알림. GuildUID : " << dwGuildUID_ << L",Grade : " << (int)ucGrade_ ) << END_LOG;
    return true;
}

bool KGuildManager::GetGuildMemberList( IN const DWORD dwGuildUID_, OUT std::map< DWORD, KNGuildUserInfo >& mapUserList_ )
{
    KGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( !spGuild ) {
        START_LOG( cwarn, L"없는 길드ID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    spGuild->GetNGuildMemberList( mapUserList_ );

    START_LOG( clog, L"길드 유저 리스트  Guild UID : " << dwGuildUID_ )
        << BUILD_LOG( mapUserList_.size() ) << END_LOG;
    return true;
}

void KGuildManager::PrintGuild( DWORD dwGuildUID_ )
{
    KNGuildInfo kGuildInfo;
    if ( !GetGuildInfo( dwGuildUID_, kGuildInfo ) ) {
        std::cout << "Not Exits Guild UID : " << dwGuildUID_ << std::endl;
        return;
    }

    //std::cout << "-- Print Guild Info (GuildUID:" << dwGuildUID_ << L") --" << dbg::endl
    //    << std::tab << L"m_strName :" << kGuildInfo.m_strName << dbg::endl
    //    << std::tab << L"m_ucMarkStatus : " << (int)kGuildInfo.m_ucMarkStatus << dbg::endl
    //    << std::tab << L"m_nMarkRevision : " << kGuildInfo.m_nMarkRevision << dbg::endl
    //    << std::tab << L"m_strFileName : " << kGuildInfo.m_strFileName << dbg::endl
    //    << std::tab << L"m_strComment : " << kGuildInfo.m_strComment << dbg::endl
    //    << std::tab << L"m_ucGrade : " << (int)kGuildInfo.m_ucGrade << dbg::endl
    //    << std::tab << L"m_kFoundingDate : " << kGuildInfo.m_kFoundingDate.Str() << dbg::endl
    //    << std::tab << L"m_strMasterNick : " << kGuildInfo.m_strMasterNick << dbg::endl
    //    << std::tab << L"m_strURL : " << kGuildInfo.m_strURL << dbg::endl
    //    << std::tab << L"m_dwExp : " << kGuildInfo.m_dwExp << dbg::endl
    //    << std::tab << L"m_dwPoint : " << kGuildInfo.m_dwPoint << dbg::endl
    //    << std::tab << L"m_nNumMembers : " << kGuildInfo.m_nNumMembers << dbg::endl
    //    << std::tab << L"m_nTotalRank : " << kGuildInfo.m_nTotalRank << dbg::endl
    //    << std::tab << L"m_nMonthlyRank : " << kGuildInfo.m_nMonthlyRank << dbg::endl
    //    << std::tab << L"m_nWeeklyRank : " << kGuildInfo.m_nWeeklyRank << dbg::endl
    //    << std::tab << L"m_ucJoinSetting : " << (int)kGuildInfo.m_ucJoinSetting << dbg::endl
    //    << std::tab << L"m_BattlePoint : " << kGuildInfo.m_GuildBattlePoint << dbg::endl;

}

void KGuildManager::GetUnicode( std::vector< std::pair<DWORD,DWORD> >& vecUnicode_ )
{
    KLocker lock( m_csConfig );
    vecUnicode_ = m_kGuildConfig.m_vecUnicode;
}

void KGuildManager::UpdateGuildPointFromCenter( IN std::map<GCGuildUID, KGuildPointBroad>& kPacket_ )
{
    std::map<GCGuildUID, KGuildPointBroad>::iterator mit;
    for( mit = kPacket_.begin(); mit != kPacket_.end(); ++mit ) {
        KGuildPtr spGuild = GetGuild( mit->first );
        if ( !spGuild ) {
            START_LOG( cwarn, L"guild point를 동기화 할 길드가 길드 매니저에 없음." )
                << BUILD_LOG( mit->first )
                << END_LOG;
            continue;
        }
        else {
            START_LOG( clog, L"길드전 포인트 더하기 전 point : " << spGuild->GetGuildPoint() )
                << BUILD_LOG( mit->second.m_dwDiffPoint )
                << BUILD_LOG( mit->second.m_dwBattlePoint )
                << BUILD_LOG( spGuild->GetBattlePoint() )
                << END_LOG;

            spGuild->SetGuildPoint( mit->second.m_dwTotalPoint );
            spGuild->SetBattlePoint( mit->second.m_dwBattlePoint );

            if ( spGuild->GetMemberGuildPoint( mit->second.m_dwUserUID ) < mit->second.m_dwUserGuildPoint ) {
                spGuild->SetMemberGuildPoint( mit->second.m_dwUserUID, mit->second.m_dwUserGuildPoint );
            }

            START_LOG( clog, L"길드전 포인트 더한 후 point : " << spGuild->GetGuildPoint() )
                << BUILD_LOG( mit->second.m_dwBattlePoint )
                << BUILD_LOG( spGuild->GetBattlePoint() ) << END_LOG;
        }
    }
}

void KGuildManager::ClearGuildInfo()
{
    std::map<DWORD, KGuildPtr>::iterator mit;
    std::vector<DWORD>::iterator vit;
    std::vector<DWORD> vecClearGuildList;
    std::vector<KGuildPtr>::iterator vitGuild;

    KLocker lock( m_csMapGuild );

    for ( mit = m_mapGuild.begin() ; mit != m_mapGuild.end() ; ++mit ) {
        if ( mit->second == NULL ) {
            START_LOG( cwarn, L"해당 길드포인터가 비었음." ) << END_LOG;
            continue;
        }

        if ( mit->second->IsMemberListEmpty() == false ) {
            continue;
        }

        // 접속중인 길드원수가 0이면 삭제 리스트에 넣자.
        vecClearGuildList.push_back( mit->first );
        // 해당 길드 정보를 삭제시켜 주자.
        vitGuild = std::find( m_vecGuild.begin(), m_vecGuild.end(), mit->second );
        if( vitGuild != m_vecGuild.end() ) {
            m_vecGuild.erase( vitGuild );
        }
    }

    for ( vit = vecClearGuildList.begin() ; vit != vecClearGuildList.end() ; ++vit ) {
        m_setLiveGuildUIDList.erase( *vit );
        m_mapGuild.erase( *vit );
        START_LOG( clog, L"접속중인 길드원이 없는 길드정보 clear. GuildUID : " << *vit ) << END_LOG;
    }

    START_LOG( clog, L"Clear Guild Info." )
        << BUILD_LOG( m_setLiveGuildUIDList.size() )
        << BUILD_LOG( m_mapGuild.size() )
        << BUILD_LOG( vecClearGuildList.size() ) << END_LOG;
}

void KGuildManager::ClearMemberList()
{
    std::map<DWORD, KGuildPtr>::iterator mit;

    KLocker lock( m_csMapGuild );

    for ( mit = m_mapGuild.begin() ; mit != m_mapGuild.end() ; ++mit ) {
        KGuildPtr spGuild = mit->second;
        if ( spGuild == NULL ) {
            START_LOG( cwarn, L"대상 길드가 없음. GuildUID : " << mit->first ) << END_LOG;
            continue;
        }
        // 접속중인 유저가 없으면 맴버리스트 클리어
        spGuild->MemberListClearCheck();
        START_LOG( clog, L"접속중인 길드원이 없는 길드 맴버리스트 clear. GuildUID : " << mit->first ) << END_LOG;
    }
}

void KGuildManager::GetGuildUIDList( OUT std::set<DWORD>& setGuildUID_ )
{
    KLocker lock( m_csMapGuild );
    setGuildUID_ = m_setLiveGuildUIDList;
}

void KGuildManager::GetGuildPageIDList( OUT std::map<UCHAR,std::vector<DWORD> >& mapData_ )
{
    KLocker lock( m_csPage );
    mapData_ = m_mapSimplePageInfo;
}

bool KGuildManager::GetGuildPageIDListSort( IN const UCHAR ucSortType_, OUT std::vector<DWORD>& vecData_ )
{
    vecData_.clear();

    KLocker lock( m_csPage );

    std::map<UCHAR,std::vector<DWORD> >::const_iterator cmit;
    cmit = m_mapSimplePageInfo.find( ucSortType_ );
    if ( cmit == m_mapSimplePageInfo.end() ) {
        START_LOG( cerr, L"해당 SortType이 없음. SortType : " << (int)ucSortType_ ) << END_LOG;
        return false;
    }

    vecData_ = cmit->second;

    if ( vecData_.empty() ) {
        START_LOG( cerr, L"해당 SortType의 Data가 없음. SortType : " << (int)ucSortType_ ) << END_LOG;
        return false;
    }

    return true;
}

void KGuildManager::BuildGuildPage()
{
    int nPageSize = GetGuildPageSize(); // 1페이지에 표시되는 길드수
    int nMaxBuildPage = GetMaxPageSize(); // 빌드할 최대 Page수

    if ( nPageSize < 0 || nMaxBuildPage < 0 ) {
        START_LOG( cerr, L"길드 config 정보 이상. 로긴서버 연결 확인바람" ) << END_LOG;
        return;
    }

    std::map<int,KSerBuffer> mapPage;
    std::map<UCHAR,std::vector<DWORD> > mapGuildPageIDList;
    GetGuildPageIDList( mapGuildPageIDList );

    std::map<UCHAR,std::vector<DWORD> >::iterator mit;
    for ( mit = mapGuildPageIDList.begin() ; mit != mapGuildPageIDList.end() ; ++mit ) {
        // 페이지 빌드/등록
        BuildPage( EOB_ASC, nPageSize, nMaxBuildPage, mit->second, mapPage );
        SetGuildPage( std::make_pair(mit->first,EOB_ASC), mapPage );

        BuildPage( EOB_DESC, nPageSize, nMaxBuildPage, mit->second, mapPage );
        SetGuildPage( std::make_pair(mit->first,EOB_DESC), mapPage );
    }

    // 총 페이지수 구하기
    SetTotalGuildPageNum();

    START_LOG( clog, L"길드리스트 페이지 빌드." )
        << BUILD_LOG( mapGuildPageIDList.size() )
        << BUILD_LOG( mapPage.size() )
        << BUILD_LOG( nPageSize )
        << BUILD_LOG( nMaxBuildPage )
        << BUILD_LOG( GetTotalGuildPageNum() )
        << BUILD_LOG( GetCurrentGuildCount() ) << END_LOG;
}

void KGuildManager::BuildPage( IN const UCHAR ucOrderBy_, IN const int nPageSize_, IN const int nMaxPage_, IN std::vector<DWORD> vecGuildUID_, OUT std::map<int,KSerBuffer>& mapPage_ )
{
    // 길드 페이지 작성
    std::vector<KNGuildInfo> vecPage;
    int nPageNum = 1;

    mapPage_.clear();
    vecPage.reserve( nPageSize_ );

    if ( ucOrderBy_ == EOB_ASC ) {

        std::vector<DWORD>::iterator vit;
        for ( vit = vecGuildUID_.begin() ; vit != vecGuildUID_.end() ; ++vit ) {
            BuildPageBuffer( nPageSize_, *vit, nPageNum, vecPage, mapPage_ );

            if ( nPageNum > nMaxPage_ )
                break;
        }
    }
    else if ( ucOrderBy_ == EOB_DESC ) {

        std::vector<DWORD>::reverse_iterator rvit;
        for ( rvit = vecGuildUID_.rbegin() ; rvit != vecGuildUID_.rend() ; ++rvit ) {
            BuildPageBuffer( nPageSize_, *rvit, nPageNum, vecPage, mapPage_ );

            if ( nPageNum > nMaxPage_ )
                break;
        }
    }

    // 남은 개수가 있으면 페이지 만들기
    KSerBuffer kBuff;
    if ( CompressPage( vecPage, kBuff ) == true ) {
        mapPage_[ nPageNum ] = kBuff; // 페이지정보 저장.
    }
}

void KGuildManager::BuildOnePage( IN const UCHAR ucOrderBy_, IN std::vector<DWORD>& vecGuildUID_, OUT KSerBuffer& kBuff_ )
{
    std::vector<KNGuildInfo> vecPage;
    KNGuildInfo kInfo;
    vecPage.clear();
    kBuff_.Clear();

    // 길드정보 뽑기(정렬별)
    if ( ucOrderBy_ == EOB_ASC ) {
        std::vector<DWORD>::iterator vit;
        for ( vit = vecGuildUID_.begin() ; vit != vecGuildUID_.end() ; ++vit ) {
            kInfo.Init();
            if ( GetGuildInfo( *vit, kInfo ) == false ) 
                continue;
            vecPage.push_back( kInfo );
            START_LOG( clog, L"EOB_ASC. GuildUID : " << *vit ) << END_LOG;
        }
    }
    else if ( ucOrderBy_ == EOB_DESC ) {
        std::vector<DWORD>::reverse_iterator rvit;
        for ( rvit = vecGuildUID_.rbegin() ; rvit != vecGuildUID_.rend() ; ++rvit ) {
            kInfo.Init();
            if ( GetGuildInfo( *rvit, kInfo ) == false ) 
                continue;
            vecPage.push_back( kInfo );
            START_LOG( clog, L", EOB_DESC. GuildUID : " << *rvit ) << END_LOG;
        }
    }

    // 압축
    LIF( CompressPage( vecPage, kBuff_ ) );
}

void KGuildManager::BuildPageBuffer( IN const int nPageSize_, IN const DWORD dwGuildUID_, IN OUT int& nPageNum_, IN OUT std::vector<KNGuildInfo>& vecPage_, IN OUT std::map<int,KSerBuffer>& mapPage_ )
{
    KNGuildInfo kInfo;
    kInfo.Init();

    if ( GetGuildInfo( dwGuildUID_, kInfo ) == false )
        return;

    vecPage_.push_back( kInfo );

    // 1페이지 개수 채우고,
    if ( vecPage_.size() < (UINT)nPageSize_ )
        return;

    // 압축
    KSerBuffer kBuff;
    LIF( CompressPage( vecPage_, kBuff ) );

    // 페이지정보 저장.
    mapPage_[ nPageNum_ ] = kBuff;

    // 버퍼 비움.
    kBuff.Clear();
    vecPage_.clear();
    ++nPageNum_;
}

void KGuildManager::SetGuildPage( IN const std::pair<UCHAR,UCHAR> prKey_, IN const std::map<int,KSerBuffer>& mapPage_ )
{
    KLocker lock( m_csPage );
    m_mapGuildPage[prKey_] = mapPage_;
}

void KGuildManager::SetTotalGuildPageNum()
{
    std::vector<DWORD> vecPageIDList;
    GetGuildPageIDListSort( KEVENT_SEARCH_GUILD_LIST_REQ::ST_GUILD_ID_DATE, vecPageIDList );
    int nPageSize = GetGuildPageSize();
    // 길드 목록을 페이징 하기 위해서
    // 65개 길드를 페이징 하려면 나누기 연산을 통해서 8개의 페이징을 생성하고..,
    int nTotalPage = vecPageIDList.size() / std::max<int>( 1, nPageSize );
    // 나누기 연산을 통해서 1개의 페이징 생성을 해서 전체 9개의 페이지를 생성.
    if ( vecPageIDList.size() % std::max<int>( 1, nPageSize ) > 0 ) {
        nTotalPage += 1;
    }

    KLocker lock( m_csPage );
    m_nTotalGuildPageNum = nTotalPage;
}

void KGuildManager::AddPageNewGuild( IN const DWORD dwGuildUID_ )
{
    KLocker lock( m_csPage );

    // 신규길드UID를 Page에 추가
    std::map<UCHAR,std::vector<DWORD> >::iterator mit;
    std::vector<DWORD>::iterator vit;
    for ( mit = m_mapSimplePageInfo.begin() ; mit != m_mapSimplePageInfo.end() ; ++mit ) {
        vit = std::find( mit->second.begin(), mit->second.end(), dwGuildUID_ );
        if ( vit != mit->second.end() ) {
            //START_LOG( cwarn, L"이미 Page에 있는 길드UID. GuildUID : " << dwGuildUID_ << L", SortType : " << (int)mit->first ) << END_LOG;
            continue;
        }

        mit->second.push_back( dwGuildUID_ );

        START_LOG( clog, L"신규 길드UID page에 추가. GuildUID : " << dwGuildUID_ << L", SortType : " << (int)mit->first ) << END_LOG;
    }

    // 최대 길드페이지수 갱신
    SetTotalGuildPageNum();

    int nTotalPage = GetTotalGuildPageNum();
    int nMaxBuildPage = GetMaxPageSize();
    if ( nTotalPage <= nMaxBuildPage ) {
        // 현재 길드페이지수가 빌드최대수 이하이면, 해당 페이지만 빌드요청
        SendOneGuildPageBuildNotice( nTotalPage );
    }
}

void KGuildManager::SendOneGuildPageBuildNotice( IN const int nPageNum_ )
{
    // 특정 1개 Page만 빌드 요청
    std::map<UCHAR,std::vector<DWORD> > mapTemp;
    std::map<UCHAR,std::vector<DWORD> > mapData;
    std::vector<DWORD> vecTemp;
    mapData.clear();

    GetGuildPageIDList( mapTemp );

    std::map<UCHAR,std::vector<DWORD> >::iterator mit;
    for ( mit = mapTemp.begin() ; mit != mapTemp.end() ; ++mit ) {
        GetCurrentPageIDList( nPageNum_, mit->second, vecTemp );
        mapData[mit->first] = vecTemp;
    }

    KDB_EVENT_ONE_GUILD_PAGE_BUILD_NOT kDBPacket;
    kDBPacket.m_nPagenum = nPageNum_;
    kDBPacket.m_mapGuildUID.swap( mapData );

    SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_ONE_GUILD_PAGE_BUILD_NOT, L"GuildManager", 0, 0, kDBPacket );

    START_LOG( clog, L"One GuildPage Build Notice To DB." )
        << BUILD_LOG( kDBPacket.m_nPagenum )
        << BUILD_LOG( kDBPacket.m_mapGuildUID.size() ) << END_LOG;
}

void KGuildManager::GetCurrentPageIDList( IN const int nPageNum_, IN const std::vector<DWORD>& vecGuildUID_, OUT std::vector<DWORD>& vecData_ )
{
    vecData_.clear();

    int nPageSize = SiKGuildManager()->GetGuildPageSize();
    int nGuildCount = 0;
    int nCurrentPageNum = 1;

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = vecGuildUID_.begin() ; cvit != vecGuildUID_.end() ; ++cvit ) {
        // 빌드해야 할 페이지 번호가 맞으면 GuildUID 채워넣기.
        if ( nCurrentPageNum == nPageNum_ ) {
            vecData_.push_back( *cvit );

            if ( vecData_.size() < (UINT)nPageSize ) {
                continue;
            }
            break;
        }

        ++nGuildCount;
        if ( nGuildCount < nPageSize ) {
            continue;
        }

        // 페이지 채우면 페이지번호 증가
        ++nCurrentPageNum;
        nGuildCount = 0;
    }
}

int KGuildManager::GetCurrentGuildCount()
{
    // 길드 개수 구하기.
    KLocker lock( m_csPage );
    if ( m_mapSimplePageInfo.empty() )
        return 0;

    return (m_mapSimplePageInfo.begin()->second).size();
}

void KGuildManager::DumpGuildPage( IN const int nSortType_, IN const int nOrderBy_, IN const int nPageNum_ )
{
    KLocker lock( m_csPage );

    std::map< std::pair<UCHAR,UCHAR>, std::map<int,KSerBuffer> >::const_iterator cmit;
    cmit = m_mapGuildPage.find( std::make_pair(nSortType_,nOrderBy_) );
    if ( cmit == m_mapGuildPage.end() ) {
        START_LOG( cerr, L"해당 정렬타입이 없음. nSortType_ : " << nSortType_ << L",OrderBy : " << nOrderBy_ ) << END_LOG;
        return;
    }

    std::map<int,KSerBuffer>::const_iterator cmitPage;
    cmitPage = cmit->second.find( nPageNum_ );
    if ( cmitPage == cmit->second.end() ) {
        START_LOG( cerr, L"해당 페이지가 없음. nPageNum_ : " << nPageNum_ ) << END_LOG;
        return;
    }

    std::vector<KNGuildInfo> vecPage;
    ReadPage( cmitPage->second, vecPage );

    //std::cout << L"-- Dump Page " << nSortType_ << L"," << nOrderBy_ << L"," << nPageNum_ << L"(size:" << vecPage.size() <<L") --" << dbg::endl;
    //std::vector<KNGuildInfo>::const_iterator cvit;
    //for ( cvit = vecPage.begin() ; cvit != vecPage.end() ; ++cvit ) {
    //    std::cout << cvit->m_dwUID << L","
    //              << cvit->m_strName << L","
    //              << (int)cvit->m_ucGrade << L","
    //              << cvit->m_kFoundingDate.Str() << L","
    //              << cvit->m_strMasterNick << L","
    //              << cvit->m_strURL << L","
    //              << cvit->m_nNumMembers << L","
    //              << (int)cvit->m_ucJoinSetting << dbg::endl;
    //}
    //std::cout << L"-- End Dump Page --" << dbg::endl;
}

void KGuildManager::RemovePageGuild( IN const DWORD dwGuildUID_ )
{
    KLocker lock( m_csPage );

    // 삭제된 길드UID를 Page에 제거
    std::map<UCHAR,std::vector<DWORD> >::iterator mit;
    std::vector<DWORD>::iterator vit;
    for ( mit = m_mapSimplePageInfo.begin() ; mit != m_mapSimplePageInfo.end() ; ++mit ) {
        vit = std::find( mit->second.begin(), mit->second.end(), dwGuildUID_ );
        if ( vit != mit->second.end() ) {
            mit->second.erase( vit );
        }
    }

    // 길드 페이지 빌드 예약
    SetReservePageBuild(true);
}

bool KGuildManager::UpdateMemberNick( IN const DWORD dwGuildUID_, IN const DWORD dwUserUID_, IN const std::wstring& strNickName_, IN const std::wstring& strNickColor_ )
{
    // 변경된 길드원 닉네임 정보갱신
    KGuildPtr spGuild = GetGuild( dwGuildUID_ );
    if ( spGuild == NULL ) {
        START_LOG( cwarn, L"길드원 닉네임 갱신할 대상 길드가 없음. GuildUID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    _JIF( spGuild->ChangeMemberNick( dwUserUID_, strNickName_, strNickColor_ ), return false );
    return true;
}

bool KGuildManager::SendMemberLevelupNotice( IN const DWORD dwGuildUID_, IN const DWORD dwUserUID_, IN const char cCharType_, IN const char cLevel_ )
{
    // 길드맴버 레벨업 알림
    KGuildPtr spGuild = GetGuild( dwGuildUID_ );
    if ( spGuild == NULL ) {
        START_LOG( cwarn, L"길드원 레벨업 알릴 대상 길드가 없음. GuildUID : " << dwGuildUID_ ) << END_LOG;
        return false;
    }

    KGuildUserCharacter kPacket;
    kPacket.m_dwGuildUID = dwGuildUID_;
    kPacket.m_dwUserUID = dwUserUID_;
    kPacket.m_cCharType = cCharType_;
    kPacket.m_cLevel = cLevel_;

    spGuild->SendToAllexJoiner( KUserEvent::EVENT_GUILD_MEMBER_CHARACTER_LEVELUP_NOT, kPacket );

    START_LOG( clog, L"길드원 레벨업 알림. GuildUID : " << dwGuildUID_ )
        << BUILD_LOG( dwUserUID_ )
        << BUILD_LOGc( cCharType_ )
        << BUILD_LOGc( cLevel_ ) << END_LOG;
    return true;
}

void KGuildManager::DumpGuildPageSort( IN const int nSortType_ )
{
    std::map<UCHAR,std::vector<DWORD> >::const_iterator cmit;
    std::vector<DWORD>::const_iterator cvit;

    KLocker lock( m_csPage );

    cmit = m_mapSimplePageInfo.find( nSortType_ );
    if ( cmit == m_mapSimplePageInfo.end() ) {
        START_LOG( cerr, L"해당 SortType이 없음. SortType : " << nSortType_ ) << END_LOG;
        return;
    }

    //int nCount = 0;

    //std::cout << L"-- Dump Page SortType:" << nSortType_ << L" (size:" << cmit->second.size() << L") --" << dbg::endl;
    //for ( cvit = cmit->second.begin() ; cvit != cmit->second.end() ; ++cvit ) {
    //    if ( nCount >= 8 ) {
    //        nCount = 0;
    //        std::cout << dbg::endl;
    //    }

    //    std::cout << *cvit << "  ";
    //    ++nCount;
    //}
    //std::cout << L"-- End Dump Page Sort --" << dbg::endl;
}

bool KGuildManager::CompressPage( IN std::vector<KNGuildInfo> vecPage_, OUT KSerBuffer& kBuff_ )
{
    kBuff_.Clear();

    if ( vecPage_.empty() ) {
        //START_LOG( cwarn, L"Page List Empty." ) << END_LOG;
        return false;
    }

    KSerializer ks;
    ks.BeginWriting( &kBuff_ );
    ks.Put( vecPage_ );
    ks.EndWriting();
    kBuff_.Compress();

    //START_LOG( clog, L"Page Compress..Size : " << vecPage_.size() ) << END_LOG;
    return true;
}

void KGuildManager::ReadPage( IN const KSerBuffer kBuff_, OUT std::vector<KNGuildInfo>& vecPage_ )
{
    vecPage_.clear();

    KSerializer ks;
    KSerBuffer kBuffer = kBuff_;
    kBuffer.UnCompress();
    ks.BeginReading( &kBuffer );
    ks.Get( vecPage_ );
    ks.EndReading();

    START_LOG( clog, L"Page Read..Size : " << vecPage_.size() ) << END_LOG;
}

bool KGuildManager::UpdatePageInfo( IN const DWORD dwGuildUID_ )
{
    // 길드 정보 업데이트시, 해당 길드의 Page 위치 확인
    // mapUpdateInfo_; map< pair<SortType,OrderBy>, PageNum>
    std::map< std::pair<UCHAR,UCHAR>, int > mapUpdateInfo;
    mapUpdateInfo.clear();

    {
        KLocker lock( m_csPage );

        std::map<UCHAR,std::vector<DWORD> >::const_iterator cmit;
        for ( cmit = m_mapSimplePageInfo.begin() ; cmit != m_mapSimplePageInfo.end() ; ++cmit ) {
            // Sort,OrderBy별로 Guild정보가 몇 페이징에 있는지 체크
            GetCurrentPageNumOrderByAsc( dwGuildUID_, cmit->first, cmit->second, mapUpdateInfo );
            GetCurrentPageNumOrderByDesc( dwGuildUID_, cmit->first, cmit->second, mapUpdateInfo );
        }
    }

    if ( mapUpdateInfo.empty() ) {
        START_LOG( cwarn, L"길드 정보 수정할 대상 Page가 없음." ) << END_LOG;
        return false;
    }

    // 수정할 길드 정보를 새로 뽑아, 해당 Page들만 빌드
    std::map< std::pair<UCHAR,UCHAR>, int >::const_iterator cmit;
    for ( cmit = mapUpdateInfo.begin() ; cmit != mapUpdateInfo.end() ; ++cmit ) {

        KLocker lock( m_csPage );

        std::map< std::pair<UCHAR,UCHAR>, std::map<int,KSerBuffer> >::iterator mitPage;
        mitPage = m_mapGuildPage.find( cmit->first );
        if ( mitPage == m_mapGuildPage.end() ) {
            START_LOG( cwarn, L"키값과 일치하는 항목이 없음. GuildUID : " << dwGuildUID_ << L",Key : " << (int)cmit->first.first << L"," << (int)cmit->first.second ) << END_LOG;
            continue;
        }

        std::map<int,KSerBuffer>::iterator mitPageInfo;
        mitPageInfo = mitPage->second.find( cmit->second );
        if ( mitPageInfo == mitPage->second.end() ) {
            START_LOG( cwarn, L"일치하는 페이지가 없음. GuildUID : " << dwGuildUID_ << L",PageNum : " << cmit->second ) << END_LOG;
            continue;
        }

        // 해당 페이지 정보 임시 저장
        std::vector<KNGuildInfo> vecPage;
        ReadPage( mitPageInfo->second, vecPage );

        // 길드 정보 갱신
        std::vector<KNGuildInfo>::iterator vit;
        for ( vit = vecPage.begin() ; vit != vecPage.end() ; ++vit ) {
            if ( vit->m_dwUID != dwGuildUID_ ) {
                continue;
            }

            KNGuildInfo kGuild;
            if ( GetGuildInfo( dwGuildUID_, kGuild ) == true ) {
                *vit = kGuild;
            }
            break;
        }

        // 재압축
        KSerBuffer kBuff;
        if ( CompressPage( vecPage, kBuff ) == true ) {
            mitPageInfo->second = kBuff; // 페이지 정보 갱신
            START_LOG( clog, L"길드 페이지 갱신됨. GuildUID : " << dwGuildUID_ )
                << BUILD_LOGc( cmit->first.first )
                << BUILD_LOGc( cmit->first.second )
                << BUILD_LOG( cmit->second )
                << BUILD_LOG( mitPageInfo->first ) << END_LOG;
        }
    }

    return true;
}

void KGuildManager::GetCurrentPageNumOrderByAsc( IN const DWORD dwGuildUID_, IN const UCHAR ucSortType_, IN const std::vector<DWORD>& vecGuild_, OUT std::map< std::pair<UCHAR,UCHAR>, int >& mapUpdatePage_ )
{
    // Sort별로 길드 정보가 몇 페이지에 있는지 확인
    int nGuildCount = 0;
    int nCurrentPage = 1;

    std::vector<DWORD>::const_iterator cvit;
    for ( cvit = vecGuild_.begin() ; cvit != vecGuild_.end() ; ++cvit ) {
        if ( *cvit == dwGuildUID_ ) {
            mapUpdatePage_[ std::make_pair( ucSortType_, EOB_ASC ) ] = nCurrentPage;
            break;
        }

        ++nGuildCount;

        if ( nGuildCount >= GetMaxPageSize() ) {
            nGuildCount = 0;
            ++nCurrentPage;
        }
    }

    START_LOG( clog, L"Current Page Num OrderBy Asc. GuildUID : " << dwGuildUID_ )
        << BUILD_LOGc( ucSortType_ )
        << BUILD_LOG( vecGuild_.size() )
        << BUILD_LOG( mapUpdatePage_.size() ) << END_LOG;
}

void KGuildManager::GetCurrentPageNumOrderByDesc( IN const DWORD dwGuildUID_, IN const UCHAR ucSortType_, IN const std::vector<DWORD>& vecGuild_, OUT std::map< std::pair<UCHAR,UCHAR>, int >& mapUpdatePage_ )
{
    // Sort별로 길드 정보가 몇 페이지에 있는지 확인
    int nGuildCount = 0;
    int nCurrentPage = 1;

    std::vector<DWORD>::const_reverse_iterator crvit;
    for ( crvit = vecGuild_.rbegin() ; crvit != vecGuild_.rend() ; ++crvit ) {
        if ( *crvit == dwGuildUID_ ) {
            mapUpdatePage_[ std::make_pair( ucSortType_, EOB_DESC ) ] = nCurrentPage;
            break;
        }

        ++nGuildCount;

        if ( nGuildCount >= GetMaxPageSize() ) {
            nGuildCount = 0;
            ++nCurrentPage;
        }
    }

    START_LOG( clog, L"Current Page Num OrderBy Desc. GuildUID : " << dwGuildUID_ )
        << BUILD_LOGc( ucSortType_ )
        << BUILD_LOG( vecGuild_.size() )
        << BUILD_LOG( mapUpdatePage_.size() ) << END_LOG;
}

void KGuildManager::UpdateGuildPageInfo( IN const DWORD dwGuildUID_ )
{
    KNGuildInfo kInfo;
    if ( GetGuildInfo( dwGuildUID_, kInfo ) == false ) {
        SiKGSDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_UPDATE_GUILD_PAGE_INFO_NOT, L"GuildManager", 0, 0, dwGuildUID_ );
        START_LOG( cwarn, L"페이지 수정할 길드 정보가 매니저에 없음. GuildUID : " << dwGuildUID_ ) << END_LOG;
        return;
    }

    // 길드 Page정보 갱신
    LIF( UpdatePageInfo( dwGuildUID_ ) );
}

char KGuildManager::DivideChannelGrade( IN const DWORD dwBattlePoint_ )
{
    char resultGrade = 0;
    std::map< char, std::pair< DWORD, DWORD > >::reverse_iterator mit;
    for ( mit = m_kGuildConfig.m_mapChannelGrade.rbegin() ; mit != m_kGuildConfig.m_mapChannelGrade.rend(); ++mit ) {

        if ( mit->second.first <= dwBattlePoint_ && dwBattlePoint_ <= mit->second.second ) {
            resultGrade = mit->first;
            break;
        }
        else if ( dwBattlePoint_ > mit->second.second ) { // 배틀포인트가 10000 이상일 때
            resultGrade = mit->first; // TODO : m_dwBattlePoint 가 9999 초과일 때 20 그레이드 세팅되는지 테스트 필요
        }
        else {
            continue;
        }
    }

    return resultGrade;
}

int KGuildManager::GetMaxBattlePoint()
{
    int maxPoint = 0;
    std::map< char, std::pair< DWORD, DWORD > >::iterator mit;
    for ( mit = m_kGuildConfig.m_mapChannelGrade.begin() ; mit != m_kGuildConfig.m_mapChannelGrade.end(); ++mit ) {
        if ( static_cast<DWORD>(maxPoint) < mit->second.second ) {
            maxPoint = mit->second.second;
        }
    }

    return maxPoint;
}

DWORD KGuildManager::GetGuildBattlePoint( IN const DWORD dwGuildUID_ )
{
    KLocker lock( m_csMapGuild );

    std::map<DWORD, KGuildPtr>::iterator mit = m_mapGuild.find( dwGuildUID_ );
    if ( mit != m_mapGuild.end() ) {
        return mit->second->GetBattlePoint();
    }

    START_LOG( cwarn, L"서버에 존재하지 않는 길드 UID :" << dwGuildUID_ ) << END_LOG;
    return 0;
}


USHORT KGuildManager::GetGuildLv( IN const int& dwPoint_ )
{
    USHORT usGuildLv = 0;

    KLocker lock(m_csConfig);

    std::map<USHORT,int>::iterator mit;
    for ( mit = m_kGuildConfig.m_mapGuildLvPoint.begin() ; mit != m_kGuildConfig.m_mapGuildLvPoint.end() ; ++mit ) {

        if ( dwPoint_ < mit->second ) {
            break;
        }

        usGuildLv = mit->first;
    }

    return usGuildLv;
}

bool KGuildManager::CheckStoreTabByGuildLv( IN const USHORT& usGuildLv_, IN const int& nTabID_ )
{
    // 길드레벨에 이용가능한 길드상점 탭인지 체크.
    bool bCheck = false;

    KLocker lock(m_csConfig);

    std::map<USHORT,std::pair<int,float> >::iterator mit;
    for ( mit = m_kGuildConfig.m_mapGuildLvAdvantage.begin() ; mit != m_kGuildConfig.m_mapGuildLvAdvantage.end() ; ++mit ) {

        if ( usGuildLv_ < mit->first ) {
            break;
        }

        if ( mit->second.first != KGuildConfig::GA_STORE_TAB ) {
            continue;
        }

        if ( nTabID_ == mit->second.second ) {
            bCheck = true;
            break;
        }
    }

    return bCheck;
}

float KGuildManager::GetGuildBonus( IN const int& nAdvType_, IN const USHORT& usGuildLv_ )
{
    // 길드레벨에 따른 보너스률
    float fExpBonus = 0.f;

    // 길드탭은 별도 관리
    if ( nAdvType_ == KGuildConfig::GA_STORE_TAB ) {
        return 0.f;
    }

    KLocker lock(m_csConfig);

    std::map<USHORT,std::pair<int,float> >::iterator mit;
    for ( mit = m_kGuildConfig.m_mapGuildLvAdvantage.begin() ; mit != m_kGuildConfig.m_mapGuildLvAdvantage.end() ; ++mit ) {

        if ( usGuildLv_ < mit->first ) {
            break;
        }

        if ( mit->second.first != nAdvType_ ) {
            continue;
        }

        fExpBonus += mit->second.second;
    }

    return fExpBonus;
}

USHORT KGuildManager::GetUserContributeGuildLv( IN const int& nContributePoint_ )
{
    // 유저기여도에 따른 길드 혜택레벨
    USHORT usUserContributeGuildLv = 0;

    KLocker lock(m_csConfig);

    std::map<int,USHORT>::iterator mit;
    for ( mit = m_kGuildConfig.m_mapContributeAdventageLv.begin() ; mit != m_kGuildConfig.m_mapContributeAdventageLv.end() ; ++mit ) {
        if ( nContributePoint_ < mit->first ) {
            break;
        }

        usUserContributeGuildLv = mit->second;
    }

    return usUserContributeGuildLv;
}

void KGuildManager::SetBattlePoint( IN const DWORD dwGuildUID_, IN const DWORD dwPoint_ )
{
    KLocker lock( m_csMapGuild );

    std::map<DWORD, KGuildPtr>::iterator mit = m_mapGuild.find( dwGuildUID_ );
    if ( mit != m_mapGuild.end() ) {
        mit->second->SetBattlePoint( dwPoint_ );
    }
}

int KGuildManager::GetGuildPointDiff( IN const int type_, IN const bool bWin_ /*= true */ )
{
    int result;
    if ( bWin_ ) {
        result = m_kGuildConfig.m_mapGuildPoint[type_].first;
    }
    else {
        result = m_kGuildConfig.m_mapGuildPoint[type_].second;
    }

    return result;
}

void KGuildManager::AddGuildPoint( IN const DWORD dwGuildUID_, IN const DWORD dwUserUID_, IN const int type_, IN const bool bWin_ /*= true */ )
{
    KGuildPtr spGuild = GetGuild( dwGuildUID_ );

    if ( spGuild ) {

        DWORD dwPoint = GetGuildPointDiff( type_, bWin_ );
        spGuild->AddGuildPoint( dwPoint );

        spGuild->UpdateMemberGuildPoint( dwUserUID_, dwPoint );

        KGuildGameResult kResult;
        kResult.m_dwGuildUID  = dwGuildUID_;
        kResult.m_dwPoint    = dwPoint;
        kResult.m_nWin = 0;
        kResult.m_nLose = 0;
        kResult.m_dwUserUID = dwUserUID_;
        kResult.m_nBPointDiff = 0;

        SiKGameServer()->GetDBLayer()->QueueingEvent( KUserEvent::DB_EVENT_UPDATE_GUILD_POINT_REQ, m_strName.c_str(), 0, 0, kResult );
    }

}

void KGuildManager::Init()
{
    // DB에 최초 길드정보 요청.
    SendUpdateGuildListReqToDB();
}

void KGuildManager::SendUpdateGuildListReqToDB()
{
    // 길드 전체정보 DB에 요청.
    SiKGSDBLayer()->QueueingID( KUserEvent::DB_UPDATE_GUILD_LIST_REQ, L"", 0, 0 );
}

void KGuildManager::BuildGuildPageList()
{
    // Sort Type별로 정렬해서 GuildUID 리스트 작성
    std::vector<DWORD> vecGuildUIDList;

    KLocker lock( m_csPage );

    std::sort( m_vecGuild.begin(), m_vecGuild.end(), KSortIDdate() );
    GetGuildUIDList( m_vecGuild, vecGuildUIDList );
    m_mapSimplePageInfo[ST_GUILD_ID_DATE] = vecGuildUIDList;

    std::sort( m_vecGuild.begin(), m_vecGuild.end(), KSortName() );
    GetGuildUIDList( m_vecGuild, vecGuildUIDList );
    m_mapSimplePageInfo[ST_GUILD_NAME] = vecGuildUIDList;

    std::sort( m_vecGuild.begin(), m_vecGuild.end(), KSortGrade() );
    GetGuildUIDList( m_vecGuild, vecGuildUIDList );
    m_mapSimplePageInfo[ST_GUILD_GRADE] = vecGuildUIDList;

    std::sort( m_vecGuild.begin(), m_vecGuild.end(), KSortPoint() );
    GetGuildUIDList( m_vecGuild, vecGuildUIDList );
    m_mapSimplePageInfo[ST_GUILD_POINT] = vecGuildUIDList;

    SetReservePageBuild(true);
}

bool KGuildManager::DayCheck()
{
    CTime tDate = CTime::GetCurrentTime();
    KSimpleDate kToday( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 6 );

    // 마지막 검사 날짜와 현재 날짜가 다른가?
    if ( m_kLastDayCheckDate == kToday ) {
        return false;
    }

    // 마지막 검사 날짜 = 현재 날짜
    m_kLastDayCheckDate.SetDate( tDate.GetYear(), tDate.GetMonth(), tDate.GetDay(), 6 );
    START_LOG( clog, L"Set LastDayCheck Date : " << m_kLastDayCheckDate.Str() ) << END_LOG;
    return true;
}

void KGuildManager::GetGuildUIDList( IN const std::vector<KGuildPtr>& vecGuild_, OUT std::vector<DWORD>& vecGuildUID_ )
{
    // 길드정보에서 GuildUID만 뽑아 vector에 담는다.
    vecGuildUID_.clear();

    std::vector<KGuildPtr>::const_iterator cvit;
    for ( cvit = vecGuild_.begin() ; cvit != vecGuild_.end() ; ++cvit ) {
        vecGuildUID_.push_back( (*cvit)->GetGuildUID() );
    }
}

void KGuildManager::UpdateGuildInfoByLua( IN const DWORD dwGuildUID_ )
{
    SiKGSDBLayer()->QueueingEvent( KUserEvent::EVENT_GET_GUILD_INFO_FROM_DB_REQ, L"GuildManager", 0, 0, dwGuildUID_ );
};

void KGuildManager::SendToGuildInfoCenter() // GuildPoint Test Func.
{
    for( int i = 1; i < 100; i++ ) {
        KGuildPointBroad kPacket;
        kPacket.m_dwDiffPoint = 0;
        kPacket.m_dwUserUID = i+100;
        kPacket.m_GuildUID = i;
        kPacket.m_dwBattlePoint = 100;
        kPacket.m_dwGuildPoint = 100;
        kPacket.m_dwUserGuildPoint = 0;
        kPacket.m_nBattlePointDiff = 0;

        SiKCnConnector()->SendPacket( KCenterEvent::ECN_UPDATE_GUILD_POINT_REQ, kPacket );
    }
}

void KGuildManager::GetGuildMarkStringName( IN const DWORD& dwGuildUID_, OUT std::wstring& strName_ )
{
    std::map<DWORD, KGuildPtr>::iterator mit;
    mit = m_mapGuild.find( dwGuildUID_ );
    if ( mit == m_mapGuild.end() )
        return;

    strName_ = mit->second->GetGuildInfo().m_strFileName;
}