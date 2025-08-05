#include <WinSock2.h>
#include "CenterServer.h"
#include "CenterNetLayer.h"
#include "CenterSimLayer.h"
#include "CenterDBThread.h"
#include "BlockIP.h"
#include "CnDonationManager.h"
#include "CnRainBow.h"
#include "CnItemManager.h"
#include "CnClientContentsManager.h"
#include "CnCouponManager.h"
#include "CnMissionEvent.h"
#include "CnSubscriptionManager.h"
#include "CnNpcGift.h"
#include "CnClientScriptManager.h"
#include "CnDicePlay.h"
#include "CnSquareManager.h"
#include "CnGuildManager.h"
#include "CnQuizBingo.h"
#include "CnGWCSupport.h"
#include "CnNewDonationManager.h"
#include "CnSphinx.h"
#include "CnSurvey.h"
#include "CnPartyManager.h"
#include "CnShutdownUser.h"
#include "CnGWCEvent.h"
#include "Log4.h"
#include "CnStatisticsManager.h"
#include "CnAdventure.h"
#include "CnOlympicManager.h"
#include "CnBuffDonation.h"
#include "GCEnum.h"
#include "CnAccumulateAttendance.h"
#include "CnUpdatePlanManager.h"
#include "CnConnectionGiftBox.h"
#include "CnTongDonation.h"
#include "CnSocialCommerce.h"
#include "CnItemProvider.h"
#include "CnDungeonRankManager.h"

#define WM_PATCH ( WM_USER + 1 )
#define WM_LOGIN_PATCH ( WM_USER + 2 )

namespace
{
    class Sort_By_Score
    {
    public:
        bool operator () ( const KMiniGameRankInfo& a, const KMiniGameRankInfo& b )
        {
            if( a.m_dwScore > b.m_dwScore ) return true;
            return false;
        }

        bool operator () ( const DWORD a, const KMiniGameRankInfo& b )
        {
            if( a > b.m_dwScore ) return true;
            return false;
        }

        bool operator () ( const KMiniGameRankInfo& a, const DWORD b )
        {
            if( a.m_dwScore > b ) return true;
            return false;
        }
    };

    class Sort_By_UID
    {
    public:
        bool operator () ( const KMiniGameRankInfo& a, const KMiniGameRankInfo& b )
        {
            if( a.m_dwUserUID > b.m_dwUserUID ) return true;
            return false;
        }

        bool operator () ( const DWORD a, const KMiniGameRankInfo& b )
        {
            if( a > b.m_dwUserUID ) return true;
            return false;
        }

        bool operator () ( const KMiniGameRankInfo& a, const DWORD b )
        {
            if( a.m_dwUserUID > b ) return true;
            return false;
        }
    };

    class KRankNumbering
    {
    public:
        KRankNumbering():nNum(0){};
        void operator () ( KMiniGameRankInfo& kRank ) { kRank.m_nRank = ++nNum; };
    private:
        int nNum;
    };
}
NiImplementRTTI( KCenterServer, KBaseServer );
ImplementSingleton( KCenterServer );

ImplToStringW( KCenterServer )
{
    KLocker lock( m_csConnectionCount );
    START_TOSTRING_PARENTW( KBaseServer )
        << TOSTRINGW( GetServerListProtocol() )
        << TOSTRINGW( m_dwConnectionCount );
    Dump( stm_ );
    return stm_;
}

KCenterServer::KCenterServer(void)
:m_nServerListProtocol( 0 )
,m_dwConnectionCount(0)
,m_dwChannelNewsGap(60 * 1000)
,m_dwTotalServerUser(0)
,m_dwUserNumGap(0)
,m_hFirstSyncEvent(NULL)
,m_dwLoginStayLimitTime( 30 * 60 * 1000 )
,m_dwConnectStayLimitTime( 30 * 60 * 1000 )
{
    m_dwConnectionLastTick = ::GetTickCount();
    m_dwChannelNewsTick = ::GetTickCount();
    m_setPhoneNumber.clear();
    m_mapUniqueUserCount.clear();
    m_dwUniqueUserTick = ::GetTickCount();
}

KCenterServer::~KCenterServer(void)
{
    m_hFirstSyncEvent = NULL;
}

ImplOstmOperatorW2A( KCenterServer );

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KCenterServer::RegToLua()
{
    KBaseServer::RegToLua();

    luabind::module( m_sptlua.get() )
    [
        luabind::def( "GetCenterServer", &KCenterServer::GetInstance ),
        luabind::class_<KCenterServer, KBaseServer>( "KCenterServer" )
            .def( luabind::tostring(luabind::self) )
            .def( "SetServerListProtocol", &KCenterServer::SetServerListProtocol )
            .def( "SetRepetitionInfo", &KCenterServer::SetRepetitionInfo )
            .def( "GetChecksumManager", &KCenterServer::GetChecksumManager )
            .def( "SetChannelNewsGap", &KCenterServer::SetChannelNewsGap )
            .def( "SetProblemMsg", &KCenterServer::SetProblemMsg )
            .def( "SetUserNumGap", &KCenterServer::SetUserNumGap )
            .def( "AddPhoneNumber", &KCenterServer::AddPhoneNumber )
            .def( "DumpServerList", &KCenterServer::DumpServerList )
            .def( "SetLoginStayLimitTime", &KCenterServer::SetLoginStayLimitTime )
            .def( "SetConnectStayLimitTime", &KCenterServer::SetConnectStayLimitTime ),

        luabind::class_<KChecksumManager>("KChecksumManager")
            .def( luabind::tostring(luabind::self) )
            .def( "UpdateNow", &KChecksumManager::UpdateNow )
            .def( "Clear", &KChecksumManager::Clear )
            .def( "SetCheckEnable", &KChecksumManager::SetCheckEnable )
            .def( "RunTest", &KChecksumManager::RunTest )
            .def( "DumpChecksum", &KChecksumManager::DumpChecksum ),

        luabind::def( "GetCnDonationManager", &KCnDonationManager::GetInstance ),
        luabind::class_<KCnDonationManager>("KCnDonationManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnDonationManager::LoadScript )
            .def( "PrintRank", &KCnDonationManager::PrintRank ),

        luabind::def( "GetIPBlockList", &KBlockIPList::GetInstance ),
        luabind::class_<KBlockIPList>( "KBlockIPList" )
            .def( luabind::tostring(luabind::self) )
            .def( "SetCheck", &KBlockIPList::SetCheck )
            .def( "DumpIPList", &KBlockIPList::DumpIPList )
            .def( "LoadScript", &KBlockIPList::LoadScript )
            .def( "TestIPList", &KBlockIPList::TestIPList ),

        luabind::def( "GetCnRainBow", &KCnRainBow::GetInstance ),
        luabind::class_<KCnRainBow>( "KCnRainBow" )
            .def( luabind::tostring(luabind::self) )
            .def( "ReloadAll", &KCnRainBow::ReloadAll ),

        luabind::def( "GetCnItemManager", &KCnItemManager::GetInstance ),
        luabind::class_<KCnItemManager>( "KCnItemManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "SetBuyItemInfoEnable", &KCnItemManager::SetBuyItemInfoEnable )
            .def( "LoadScript", &KCnItemManager::LoadScript )
            .def( "CheckDBUpdate", &KCnItemManager::QueuingCheckDBUp )
            .def( "DumpBuyLimitItem", &KCnItemManager::DumpBuyLimitItem )
            .def( "SendPackageListChangedNot", &KCnItemManager::SendPackageListChangedNot ),

        luabind::def( "GetCnContentsManager", &KCnClientContentsManager::GetInstance ),
        luabind::class_<KCnClientContentsManager>( "KCnClientContentsManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnClientContentsManager::LoadScript ),

        luabind::def( "GetCnMissionEvent", KCnMissionEvent::GetInstance ),
        luabind::class_<KCnMissionEvent>( "KCnMissionEvent" )
            .def( luabind::tostring(luabind::self) ),

        luabind::def( "GetCnSubscriptManager", KCnSubscriptionManager::GetInstance ),
        luabind::class_<KCnSubscriptionManager>( "KCnSubscriptionManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "DumpList", &KCnSubscriptionManager::DumpList ),

        luabind::def( "GetCnNpcGift", KCnNpcGift::GetInstance ),
        luabind::class_<KCnNpcGift>( "KCnNpcGift" )
            .def( luabind::tostring(luabind::self) )
            .def( "DumpList", &KCnNpcGift::DumpList ),

        luabind::def( "GetCnClientScriptManager", KCnClientScriptManager::GetInstance ),
        luabind::class_<KCnClientScriptManager>( "KCnClientScriptManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnClientScriptManager::LoadScript )
            .def( "DumpMonsterInfo", &KCnClientScriptManager::DumpMonsterInfo ),

        luabind::def( "GetCnSquareManager", KCnSquareManager::GetInstance ),
        luabind::class_<KCnSquareManager>( "KCnSquareManger" )
            .def( luabind::tostring(luabind::self) )
            .def( "AddSquare", &KCnSquareManager::AddSquare )
            .def( "DumpSquareList", &KCnSquareManager::DumpSquareList )
            .def( "AddGuildSquareSetInfo", &KCnSquareManager::AddGuildSquareSetInfo ),

        luabind::class_<KSquareSetInfo>( "KSquareSetInfo" )
            .def( luabind::constructor<>() )
            .def( "SetInfo", &KSquareSetInfo::SetInfo )
            .def( "SetAddress", &KSquareSetInfo::SetAddress ),

        luabind::def( "GetGuildManager", KCnGuildManager::GetInstance ),
        luabind::class_<KCnGuildManager>( "KCnGuildManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnGuildManager::LoadScript )
            .def( "PrintGuild", &KCnGuildManager::PrintGuild )
            .def( "PrintUser", &KCnGuildManager::PrintUser )
            .def( "UpdateList", &KCnGuildManager::SendUpdateGuildListReqToDB ),

        luabind::def( "GetCnCouponManager", &KCnCouponManager::GetInstance ),
        luabind::class_<KCnCouponManager>( "KCnCouponManager" )
            .def( luabind::tostring(luabind::self) ),
        luabind::def( "GetCnQuizBingo", KCnQuizBingo::GetInstance ),
        luabind::class_<KCnQuizBingo>( "KCnQuizBingo" )
            .def( luabind::tostring(luabind::self) ),
        luabind::def( "GetCnGWCSupport", KCnGWCSupport::GetInstance ),
        luabind::class_<KCnGWCSupport>( "KCnGWCSupport" )
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnGWCSupport::LoadScript ),
        luabind::def( "GetCnNewDonationManager", &KCnNewDonationManager::GetInstance ),
        luabind::class_<KCnNewDonationManager>("KCnNewDonationManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnNewDonationManager::LoadScript ),

        //luabind::def( "GetOneaday", &KCnOneaday::GetInstance ),
        //luabind::class_<KCnOneaday>("KCnOneaday")
        //    .def( luabind::tostring(luabind::self) ),

        luabind::def( "GetCnSphinx", &KCnSphinx::GetInstance ),
        luabind::class_<KCnSphinx>("KCnSphinx")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnSphinx::LoadScript ),

        luabind::def( "GetPartyManager", &KCnPartyManager::GetInstance ),
            luabind::class_<KCnPartyManager>("KCnPartyManager")
            .def( luabind::tostring(luabind::self) )
            .def( "PrintParty", &KCnPartyManager::PrintParty )
            .def( "PrintAll", &KCnPartyManager::PrintAll ),

        luabind::def( "GetShutdownUser", &KCnShutdownUser::GetInstance ),
        luabind::class_<KCnShutdownUser>("KCnShutdownUser")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnShutdownUser::LoadScript ),

        luabind::def( "GetCnSurvey", &KCnSurvey::GetInstance ),
        luabind::class_<KCnSurvey>("KCnSurvey")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnSurvey::LoadScript ),

        luabind::def( "GetCnGWCEvent", &KCnGWCEvent::GetInstance ),
        luabind::class_<KCnGWCEvent>("KCnGWCEvent")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnGWCEvent::LoadScript ),

        luabind::def( "GetCnStatisticsManager", KCnStatisticsManager::GetInstance ),
        luabind::class_<KCnStatisticsManager>( "KCnStatisticsManager" )
            .def( luabind::tostring(luabind::self) )
            .def( "AddStatistics", &KCnStatisticsManager::AddStatistics )
            .def( "ModifyStatistics", &KCnStatisticsManager::ModifyStatistics )
            .enum_( "STATISTICS_ID" )
            [
                luabind::value( "SI_LOGINOUT_STAT", KCnStatisticsManager::SI_LOGINOUT_STAT )
            ],

        luabind::def( "GetCnAdventure", &KCnAdventure::GetInstance ),
        luabind::class_<KCnAdventure>("KCnAdventure")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnAdventure::LoadScript ),

        luabind::def( "GetCnOlympicManager", &KCnOlympicManager::GetInstance ),
        luabind::class_<KCnOlympicManager>("KCnOlympicManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnOlympicManager::LoadScript ),

        luabind::def( "GetCnBuffDonation", &KCnBuffDonation::GetInstance ),
        luabind::class_<KCnBuffDonation>("KCnBuffDonation")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnBuffDonation::LoadScript ),

        luabind::def( "GetCnUpdatePlanManager", &KCnUpdatePlanManager::GetInstance ),
        luabind::class_<KCnUpdatePlanManager>("KCnUpdatePlanManager")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnUpdatePlanManager::LoadScript ),

        luabind::def( "GetCnConnectionGiftBox", &KCnConnectionGiftBox::GetInstance ),
        luabind::class_<KCnConnectionGiftBox>("KCnConnectionGiftBox")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnConnectionGiftBox::LoadScript ),

        luabind::def( "GetCnTongDonation", &KCnTongDonation::GetInstance ),
        luabind::class_<KCnTongDonation>("KCnTongDonation")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnTongDonation::LoadScript ),

        luabind::def( "GetCnSocialCommerce", &KCnSocialCommerce::GetInstance ),
        luabind::class_<KCnSocialCommerce>("KCnSocialCommerce")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnSocialCommerce::LoadScript ),

        luabind::def( "GetKCnItemProvider", &KCnItemProvider::GetInstance ),
        luabind::class_<KCnItemProvider>("KCnItemProvider")
            .def( luabind::tostring(luabind::self) )
            .def( "LoadScript", &KCnItemProvider::LoadScript ),

        luabind::def( "GetCnDungeonRankManager", &KCnDungeonRankManager::GetInstance ),
        luabind::class_<KCnDungeonRankManager>("KCnDungeonRankManager")
            .def( luabind::tostring(luabind::self) )
            .def( "DoForceSeasonReset", &KCnDungeonRankManager::DoForceSeasonReset )
    ];
}

bool KCenterServer::Init( const wchar_t* szFileName_ )
{
    m_pkNetLayer    = SiKCenterNetLayer();
    m_pkSimLayer    = SiKCenterSimLayer();
    m_pkDBLayer     = SiKCenterDBMgr();

    // 070319. kkurrung. 한시간 이벤트 데이터 초기화.
    InitRepetitionInfo();

    _JIF( KBaseServer::Init( szFileName_ ), return false );

    _JIF( LoadScript(), return false ); // <---  이 함수 안에서 스크립트 로드 해주세요.. ^^
    _JIF( SiKCnSquareManager()->LoadScript( m_sptlua ), return false );

    SiKCnDonationManager()->InitProcess();
    SiKCnSubscriptionManager()->SendDBSubscriptionInfoUpdateReq();
    SiKCnDicePlay()->InitProcess();     // 주사위 이벤트 데이터 세팅.
    SiKCnNewDonationManager()->InitProcess();   // 신년 이벤트 세팅.

    // 길드 정보 받아오기
    SiKCnGuildManager()->Init();

    // 초기 구동시 db에서 서버 리스트를 한번 얻어온다.
    m_hFirstSyncEvent   = ::CreateEvent( NULL, false, false, NULL );
    SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_EVENT_FIRST_INIT_REQ, 
        m_strServerName.c_str(), 0, 0, GetServerListProtocol() );

    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_NPC_GIFTS_UPDATE_REQ, L"", 0 );

    //SiKCnOneaday()->Init();

    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_EVENT_TIME_SYNCRONIZE_NOT, L"", 0 );
    SiKCnOlympicManager()->InitProcess(); // 올림픽 이벤트 데이터 가져오기.

    {
        // 발렌타인 버프 기부 이벤트 데이터 가져오기
        KECN_BUFF_DONATION_TOTAL_DATA_NOT kPacket;
        kPacket.m_nBuffStateFlag = FLAG_SYNC_ONLY_GAME_SERVER;
        SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_BUFF_DONATION_TOTAL_DATA_REQ, L"", 0, -1, kPacket );
    }

    // 던전 랭킹 시스템 데이터 가져오기
    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_DUNGEON_RANK_SEASON_DELIMIT_TIME_REQ, L"", 0 );
    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_DUNGEON_RANK_CURRENT_SEASON_TOP_RANK_REQ, L"", 0 );
    SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_DUNGEON_RANK_PREVIOUS_SEASON_TOP_RANK_REQ, L"", 0 );

    //SiKCenterSimLayer()->InitCYOUBilling( m_strServerName, 0 );

    m_dwServerInfoUpdateTick = ::GetTickCount();

    switch( ::WaitForSingleObject( m_hFirstSyncEvent, 30000 ) ) {
    case WAIT_OBJECT_0:
        {
            std::cout << "**** Server Initionize Finished.. ****" << std::endl;
            CloseHandle( m_hFirstSyncEvent );
            m_hFirstSyncEvent = NULL;
        }
        break;
    case WAIT_TIMEOUT:
        START_LOG( cerr, L"Getting Server Info from DB failed." ) << END_LOG;
        CloseHandle( m_hFirstSyncEvent );
        return false;
        break;
    default:
        START_LOG( cerr, L"Unexpected Return value from ::WaitForSingleObject()." ) << END_LOG;
        CloseHandle( m_hFirstSyncEvent );
        return false;
    }

    SiKCenterSimLayer()->SetCreateActorEnable( true );
    START_LOG( cerr, L"Create Actor Enable!!" ) << END_LOG;

    return true;
}

void KCenterServer::Run( bool bRunInterpreter_ )
{
    KBaseServer::Run( bRunInterpreter_ ); // lua interpreter 시작
}

void KCenterServer::ShutDown()
{
    KBaseServer::ShutDown();

    KCenterSimLayer::ReleaseInstance();
    KCenterNetLayer::ReleaseInstance();
    KTDBLayer<KCenterDBThread>::ReleaseInstance();
    KCnDonationManager::ReleaseInstance();
    KBlockIPList::ReleaseInstance();
    KCnRainBow::ReleaseInstance();
    KCnSubscriptionManager::ReleaseInstance();
    KCnItemManager::ReleaseInstance();
    KCnClientContentsManager::ReleaseInstance();
    KCnCouponManager::ReleaseInstance();
    KCnMissionEvent::ReleaseInstance();
    KCnNpcGift::ReleaseInstance();
    KCnClientScriptManager::ReleaseInstance();
    KCnDicePlay::ReleaseInstance();
    KCnSquareManager::ReleaseInstance();
    KCnGuildManager::ReleaseInstance();
    KCnGWCSupport::ReleaseInstance();
    KCnNewDonationManager::ReleaseInstance();
    KCnSphinx::ReleaseInstance();
    KCnPartyManager::ReleaseInstance();
    KCnShutdownUser::ReleaseInstance();
    KCnSurvey::ReleaseInstance();
    KCnGWCEvent::ReleaseInstance();
    KCnAdventure::ReleaseInstance();	
    KCnOlympicManager::ReleaseInstance();
    KCnBuffDonation::ReleaseInstance();
    KCnAccumulateAttendanceManager::ReleaseInstance();
    KCnUpdatePlanManager::ReleaseInstance();
    KCnConnectionGiftBox::ReleaseInstance();
    KCnTongDonation::ReleaseInstance();
    KCnSocialCommerce::ReleaseInstance();
    KCnItemProvider::ReleaseInstance();
    KCnDungeonRankManager::ReleaseInstance();
}

void KCenterServer::Tick()
{
    if ( SiKCenterSimLayer()->CheckTypeFlag( KCenterSimLayer::ST_INTERNAL_TESOP ) ) {
        if( ::GetTickCount() - m_dwServerInfoUpdateTick > 10 * 1000 )
        {
            SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_EVENT_SERVER_LIST_REQ, 
                m_strServerName.c_str(), 0, 0, GetServerListProtocol() );
            m_dwServerInfoUpdateTick = ::GetTickCount();
        }
    }

    // 서버 리스트를 가져와서 베이스 서버에 저장한다.
    if( ::GetTickCount() - m_dwServerInfoUpdateTick > KBaseServer::ms_dwSendServerListGap )
    {
        SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_EVENT_SERVER_LIST_REQ, 
            m_strServerName.c_str(), 0, 0, GetServerListProtocol() );

        m_dwServerInfoUpdateTick = ::GetTickCount();
    }

    if( ::GetTickCount() - m_dwConnectionLastTick > 1000 * 60 * 60 ) // 1시간
    {
        DWORD dwConnectionCount = GetConnectionCount();
        m_dwConnectionLastTick = ::GetTickCount();
        ClearConnectionCount();

        if( dwConnectionCount > 0 )
            SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_UPDATE_CONNECTION_COUNT_REQ, m_strServerName.c_str(), 0, 0, dwConnectionCount );
    }

    SiKCnDonationManager()->Tick();
    if( m_kChecksumManager.CheckUpdate() )
    {
        SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_UPDATE_SCRIPT_CHECKSUM_REQ,
        m_strServerName.c_str(), 0, 0, GetServerListProtocol() );
    }

    if( ::GetTickCount() - m_dwChannelNewsTick > m_dwChannelNewsGap )
    {
        SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_UPDATE_CHANNEL_NEWS_REQ, m_strServerName.c_str(), 0, 0, GetServerListProtocol() );
        m_dwChannelNewsTick = ::GetTickCount();
    }

    if( ::GetTickCount() - m_dwUniqueUserTick > 1000 * 60 * 60 * 24 ) // 1일
    {
        m_dwUniqueUserTick = ::GetTickCount();
        std::map< DWORD, std::set<DWORD> > mapUniqueUserCount;
        GetUniqueUserStat( mapUniqueUserCount );

        SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_UPDATE_UNIQUE_USER_STAT_REQ, m_strServerName.c_str(), 0, 0, mapUniqueUserCount );
    }

    SiKCnRainBow()->Tick();
    SiKCnSubscriptionManager()->Tick();
    SiKCnItemManager()->Tick();
    SiKCnCouponManager()->Tick();
    SiKCnMissionEvent()->Tick();
    SiKCnNpcGift()->Tick();
    SiKCnDicePlay()->Tick();
    SiKCnGuildManager()->Tick();
    SiKCnGWCSupport()->Tick();
    SiKCnNewDonationManager()->Tick();
    SiKCnSquareManager()->Tick();
    SiKCnQuizBingo()->Tick();
    SiKCnSphinx()->Tick();
    SiKCnSurvey()->Tick();
    SiKCnGWCEvent()->Tick();
    SiKCnStatisticsManager()->Tick();
    SiKCnAdventure()->Tick();
    SiKCnOlympicManager()->Tick();
    SiKCnBuffDonation()->Tick();
    SiKCnAccumulateAttendanceManager()->Tick();
    SiKCnUpdatePlanManager()->Tick();
    SiKCnSocialCommerce()->Tick();
    SiKCnItemProvider()->Tick();
    SiKCnDungeonRankManager()->Tick();
}

void KCenterServer::InitRepetitionInfo()
{
    m_dwLastRepetitionUID   = 0;
    m_nRequestCount         = 0;
    m_nMaxRequestCount      = 3;
    m_nServerUserLimit      = 100;
    m_nCommentID            = -1;
    m_tmGetEventTime        = CTime::GetCurrentTime();
}

void KCenterServer::SetRepetitionInfo( const int nMaxRequestCount_, const int nServerUserLimit_ )
{
    JIF( nMaxRequestCount_ > 0 && nServerUserLimit_ > 0 );

    KLocker lock( m_csRepetitionEvent );
    m_nMaxRequestCount = nMaxRequestCount_;
    m_nServerUserLimit = nServerUserLimit_;
}

void KCenterServer::Dump( std::wostream& stm_ ) const
{
    stm_ << L" -- Repetition Event Info -- ";
    KLocker lock( m_csRepetitionEvent );
    stm_ << (LPCTSTR)m_tmGetEventTime.Format( KNC_TIME_FORMAT ) << std::endl
        << L"  Event ID : " << m_dwLastRepetitionUID
        << L", Comment ID : " << m_nCommentID
        << L", Request Count : " << m_nRequestCount << L" / " << m_nMaxRequestCount
        << L", Server User Limit : " << m_nServerUserLimit 
        << L", Channel News Gap : " << m_dwChannelNewsGap << std::endl;
}

void KCenterServer::UpdateRepetitionData( DWORD dwRepetitionUID_, int nCommentID_ )
{
    JIF( dwRepetitionUID_ > 0 && nCommentID_ > 0 );

    KLocker lock( m_csRepetitionEvent );
    m_tmGetEventTime        = CTime::GetCurrentTime();
    m_dwLastRepetitionUID   = dwRepetitionUID_;
    m_nCommentID            = nCommentID_;
    m_nRequestCount         = 0;
}

void KCenterServer::ProcessRepetitionEvent()
{
    std::vector< std::wstring > vecServerNames;
    vecServerNames.clear();

    KLocker lock( m_csRepetitionEvent );

    // 시도 횟수가 허용되면
    JIF( m_nRequestCount <= m_nMaxRequestCount );

    // 해당 센터를 찾고
    {
        KLocker lock1( m_csServerInfo );
        std::vector< KServerInfo >::const_iterator vit;
        for( vit = m_vecServerInfoList.begin() ; vit != m_vecServerInfoList.end() ; ++ vit )
        {
            if( vit->m_iUserNum >= m_nServerUserLimit )
            {
                vecServerNames.push_back( vit->m_strName );
            }
        }
    }

    if( vecServerNames.empty() )
    {
        START_LOG( cwarn, L"이벤트 대상 서버가 존재하지 않음" )
            << BUILD_LOGtm( m_tmGetEventTime )
            << BUILD_LOG( m_dwLastRepetitionUID )
            << BUILD_LOG( m_nCommentID )
            << BUILD_LOG( m_nServerUserLimit ) << END_LOG;
        return;
    }

    std::random_shuffle( vecServerNames.begin(), vecServerNames.end() );
    std::vector< std::wstring >::const_iterator vit;
    KCenterPtr spCenter;
    for( vit = vecServerNames.begin() ; vit != vecServerNames.end() ; ++vit )
    {
        spCenter = SiKCenterSimLayer()->m_kActorManager.GetByName<KCenter>( *vit );
        if( spCenter )
            break;
    }

    JIF( spCenter != NULL );
    // 해당 센터에게 보낸다.
    KECN_REPETITION_PROVIDE_REQ kPacket;
    kPacket.m_dwEventUID = m_dwLastRepetitionUID;
    kPacket.m_nCommentID = m_nCommentID;
    LIF( spCenter->SendPacket( KCenterEvent::ECN_REPETITION_PROVIDE_REQ, kPacket ) );
    ++m_nRequestCount;
    START_LOG( clog, L"한시간 이벤트 Send to CenterProxy.." )
        << BUILD_LOG( kPacket.m_dwEventUID )
        << BUILD_LOG( kPacket.m_nCommentID ) << END_LOG;
    // 없다면 로그로 남긴다.
}


void KCenterServer::SetMiniGameRank( std::map< char, std::vector<KMiniGameRankInfo> >& mapRank_ )
{
    m_mapMiniGameRank = mapRank_;

    std::map< char, std::vector<KMiniGameRankInfo> >::iterator mit;
    std::vector<KMiniGameRankInfo>::iterator vit;

    for( mit = m_mapMiniGameRank.begin(); mit != m_mapMiniGameRank.end(); ++mit )
    {     
        BuildRankByScore( mit->second );
    }
    //DumpMiniRank();
}

void KCenterServer::AddMiniGameRank( IN std::map< char, std::vector<KMiniGameRankInfo> > mapRank_ )
{
    KLocker lock( m_csMiniGameRank );
    std::map< char, std::vector<KMiniGameRankInfo> >::iterator mit;
    std::map< char, std::vector<KMiniGameRankInfo> >::iterator mit2;

    std::vector<KMiniGameRankInfo>::iterator vit;

    for( mit = mapRank_.begin(); mit != mapRank_.end(); ++mit )
    {
        mit2 = m_mapMiniGameRank.find( mit->first );
        
        if( mit2 == m_mapMiniGameRank.end() )
        {            
            std::pair< std::map<char, std::vector<KMiniGameRankInfo> >::iterator, bool > prib;
            prib = m_mapMiniGameRank.insert( std::make_pair(mit->first,std::vector<KMiniGameRankInfo>()) );
            JIF( prib.second );
            mit2 = prib.first;

            //continue; // 해당 미니게임 종류가 없음
        }

        for( vit = mit->second.begin(); vit != mit->second.end() ; ++vit )
        {
            AddGameRank( mit2->second, *vit );
        }

        // 정렬
        BuildRankByScore( mit2->second );
       
        Clipping( 100, mit2->second );
    }
}

void KCenterServer::Clipping( IN DWORD dwSize_, IN OUT std::vector<KMiniGameRankInfo>& vecRank_ )
{
    if( vecRank_.empty() ) return;

    if( vecRank_.size() <= dwSize_ ) return;

    vecRank_.erase( vecRank_.begin() + dwSize_ , vecRank_.end() );

}

void KCenterServer::GetMiniGameRank( OUT std::map< char, std::vector<KMiniGameRankInfo> >& mapRank_ )
{
    KLocker lock( m_csMiniGameRank );

    mapRank_ = m_mapMiniGameRank;
}

void KCenterServer::BuildRankByScore( IN OUT std::vector<KMiniGameRankInfo>& vecRank_ )
{
    std::sort( vecRank_.begin(), vecRank_.end(), Sort_By_Score() );
    std::for_each( vecRank_.begin(), vecRank_.end(), KRankNumbering() ); // 득점으로 넘버링을 한다.
}

void KCenterServer::DumpMiniRank()
{
    std::map< char, std::vector<KMiniGameRankInfo> >::iterator mit;
    std::vector<KMiniGameRankInfo>::iterator vit;
    
    for( mit = m_mapMiniGameRank.begin(); mit != m_mapMiniGameRank.end() ; ++mit )
    {
        for( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
        {
            START_LOG( cwarn, L"GameMode : " << static_cast<int>(vit->m_cGameMode) << L", Nick : " << vit->m_strNick
                << L", Score : " << vit->m_dwScore << L", Rank : " << vit->m_nRank ) << END_LOG;
        }
    }
}

bool KCenterServer::AddGameRank( IN OUT std::vector<KMiniGameRankInfo>& vecRank_, IN OUT KMiniGameRankInfo& kInfo_ )
{
    kInfo_.m_nRank = -1;

    if( !vecRank_.empty() )
    {
        std::vector<KMiniGameRankInfo>::iterator vit = vecRank_.end() -1;

        //START_LOG( clog, L"랭크 마지막 순위의 유저 : " << vit->m_strNick << ", 점수 : " << vit->m_dwScore << " , Rank :  " << vit->m_nRank  ) << END_LOG;

        if( vit->m_dwScore >= kInfo_.m_dwScore && vecRank_.size() >= 100 )
        {
            //START_LOG( clog, L"점수가 꼴등보다 못해서 랭킹에 못들어간다"  ) << END_LOG;
            return false;
        }

        vit = std::find_if( vecRank_.begin(), vecRank_.end(), 
            boost::bind(&KMiniGameRankInfo::m_dwUserUID, _1 ) == kInfo_.m_dwUserUID ); // 같은 유저의 기록이 있다면, 동일 유저(지) 최고 기록보다 더 높은 점수여야 갱신이 됨

        if( vit != vecRank_.end() ) // 동일 유저가 있는가
        {
            if( vit->m_dwScore >= kInfo_.m_dwScore ) // 기존 점수를 초과했는가
            {
                //START_LOG( clog, L"미니게임 랭킹의 기존유저, 기존 기록 이하... " << kInfo_.m_strNick  ) << END_LOG;
                return false;
            }

            *vit = kInfo_; // 초과했다면 데이터 교체
            //START_LOG( clog, L"미니게임 랭킹의 기존유저, 기록 갱신함, Nick: " << kInfo_.m_strNick  ) << END_LOG;

        }
        else
        {
            //START_LOG( clog, L"새로운 유저 데이터를 미니게임 랭킹에 추가, " << kInfo_.m_strNick  ) << END_LOG;
            vecRank_.push_back( kInfo_ ); // 기존 랭킹에 없는 유저일 때 
        }
    }
    else //if( vecRank_.size() < 100 )
    {
        vecRank_.push_back( kInfo_ ); // 최초 1회만 불려질거 같음
    }

    //BuildRankByScore( vecRank_ ); // 점수로 정렬해서, 등수 부여
    return true;
}

void KCenterServer::DumpRank( std::map< char, std::vector<KMiniGameRankInfo> >& mapRank_ )
{
    std::map< char, std::vector<KMiniGameRankInfo> >::iterator mit;
    std::vector<KMiniGameRankInfo>::iterator vit;

    for( mit = mapRank_.begin(); mit != mapRank_.end() ; ++mit )
    {
        for( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
        {
            START_LOG( cwarn, L"GameMode : " << static_cast<int>(vit->m_cGameMode) << L", Nick : " << vit->m_strNick
                << L", Score : " << vit->m_dwScore << ", Rank : " << vit->m_nRank ) << END_LOG;
        }
    }
}
void KCenterServer::UpdateChannelNews( IN const KENU_CHANNEL_NEWS_NOT& vecChannelNews_ )
{
    KLocker lock( m_csChannelNews );
    m_vecChannelNews = vecChannelNews_;    
}

void KCenterServer::GetChannelNews( OUT KENU_CHANNEL_NEWS_NOT& vecChannelNews_ )
{
    KLocker lock( m_csChannelNews );
    vecChannelNews_ = m_vecChannelNews;
}

bool KCenterServer::CheckScriptChecksum( KODBC& kOdbc_, IN const std::map<std::wstring,KScriptCheckInfo>& mapScript_, const std::wstring& strLogin_, OUT std::vector< std::pair<std::wstring,bool> >& vecFileCheck_ )
{
    if ( !m_kChecksumManager.GetCheckEnable() ) return true;
    std::set< std::wstring > setScript;
    std::set< std::wstring >::iterator sit;
    std::map<std::wstring,KScriptCheckInfo>::const_iterator cmit;
    vecFileCheck_.clear();
    bool bResult = true; // 스크립트 검사 결과

    // DB에 설정된 스크립트 파일 이름 얻어오기.
    m_kChecksumManager.GetHashCheckList( setScript );
    START_LOG( clog, L"Server Hash Size : " << setScript.size()
        << L", Client Hash Size : " << mapScript_.size() )  << END_LOG;
    vecFileCheck_.reserve( setScript.size() );

    for ( sit = setScript.begin() ; sit != setScript.end() ; ++sit ) {
        
        // 설정된 스크립트 파일명 보내줬는지 검사
        cmit = mapScript_.find( *sit );
        if ( cmit == mapScript_.end() ) {
            bResult = false;
            vecFileCheck_.push_back( std::make_pair( *sit, false ) );
            START_LOG( cerr, L"설정된 Checksum의 FileName이 없음. script name : " << *sit ) << END_LOG;
            continue;
        }

        // 체크섬 검사

        if( !m_kChecksumManager.CheckHash( kOdbc_, strLogin_, cmit->first, cmit->second.m_strHash, cmit->second.m_dwFileSize ) )
        {
            bResult = false;
            START_LOG( cerr, L"Hash Check err.. Name : " << strLogin_
                << L", File : " << cmit->first
                << L", Hash : " << cmit->second.m_strHash
                << L", Size : " << cmit->second.m_dwFileSize ) << END_LOG;
            vecFileCheck_.push_back( std::make_pair( cmit->first, false ) );
            continue;
        }

        vecFileCheck_.push_back( std::make_pair( cmit->first, true ) );
        // 클라이언트에서 File Size가 틀린 파일들을 지워주기 위해서 File Size 체크 결과값을 담아주자.

        START_LOG(clog, L"Script Check Success.. Name : " << strLogin_
            << L", File : " << cmit->first
            << L", Hash : " << cmit->second.m_strHash
            << L", Size : " << cmit->second.m_dwFileSize ) << END_LOG;
    }

    return bResult;
}

bool KCenterServer::LoadScript()
{
    _JIF( SiKBlockIPList()->LoadScript(), return false );
    _JIF( SiKCnDonationManager()->LoadScript(), return false );
    _JIF( SiKCnClientScriptManager()->LoadScript(), return false );
    _JIF( SiKCnClientContentsManager()->LoadScript(), return false );
    _JIF( SiKCnDicePlay()->LoadScript(), return false );
    _JIF( SiKCnGuildManager()->LoadScript(), return false );
    _JIF( SiKCnGWCSupport()->LoadScript(), return false );
    _JIF( SiKCnNewDonationManager()->LoadScript(), return false );
    _JIF( SiKCnItemManager()->m_kSocketConsume.LoadScript(), return false );
    _JIF( SiKCnItemManager()->m_kSocketConsume.LoadScript_GP(), return false );
    _JIF( SiKCnItemManager()->m_kSocketConsume.LoadScript_Item(), return false );
    _JIF( SiKCnSphinx()->LoadScript(), return false );
    SiKCnItemManager()->QueuingCheckDBUp();
    _JIF( SiKCnShutdownUser()->LoadScript(), return false );
    _JIF( SiKCnSurvey()->LoadScript(), return false );
    _JIF( SiKCnGWCEvent()->LoadScript(), return false );
    _JIF( SiKCnAdventure()->LoadScript(), return false );
    _JIF( SiKCnOlympicManager()->LoadScript(), return false );
    _JIF( SiKCnBuffDonation()->LoadScript(), return false );
    _JIF( SiKCnUpdatePlanManager()->LoadScript(), return false );
    _JIF( SiKCnConnectionGiftBox()->LoadScript(), return false );
    _JIF( SiKCnTongDonation()->LoadScript(), return false );
    _JIF( SiKCnSocialCommerce()->LoadScript(), return false );
    _JIF( SiKCnItemProvider()->LoadScript(), return false );

    return true;
}

bool KCenterServer::ReLoadScript()
{
    _JIF( LoadScript(), return false );

    HWND    m_hRemote;
    m_hRemote = FindWindow( NULL, L"KEsdRemoteController_Server" );
    if( m_hRemote != NULL )
    {
        START_LOG( cerr, L"패치 성공 & 리모콘 떠있음" ) << END_LOG;
        SendMessage( m_hRemote, WM_LOGIN_PATCH, NULL, NULL );
    }
    return true;
}

void KCenterServer::GetSHAFileNameList( OUT std::set<std::wstring>& setSHAFileNameList_ )
{
    std::set< std::wstring > setScript;
    // DB에 설정된 스크립트 파일 이름 얻어오기.
    m_kChecksumManager.GetHashCheckList( setScript );
    setSHAFileNameList_.swap( setScript );
}

void KCenterServer::SetProblemMsg( IN const char* cProblemMsg_, IN const char* cCurrentMsg_ )
{
    KLocker lock( m_csTotalServerUser );
    std::string strProblemMsg;
    std::string strCurrentMsg;
    strProblemMsg = cProblemMsg_;
    strCurrentMsg = cCurrentMsg_;
    m_wstrProblemMsg = m_stringTable.GetValue( KncUtil::toWideString( strProblemMsg ).c_str() );
    m_wstrCurrentMsg = m_stringTable.GetValue( KncUtil::toWideString( strCurrentMsg ).c_str() );
}

void KCenterServer::AddPhoneNumber( IN const char* cPhoneNumber_ )
{
    KLocker lock( m_csTotalServerUser );
    std::string strMsg;
    std::wstring wstrMsg;
    strMsg = cPhoneNumber_;
    wstrMsg = m_stringTable.GetValue( KncUtil::toWideString( strMsg ).c_str() );
    m_setPhoneNumber.insert( wstrMsg );
}

void KCenterServer::GetProblemMsg( OUT std::wstring& wstrProblemMsg_, OUT std::wstring& wstrCurrentMsg_ )
{
    KLocker lock( m_csTotalServerUser );
    wstrProblemMsg_ = m_wstrProblemMsg;
    wstrCurrentMsg_ = m_wstrCurrentMsg;
}

void KCenterServer::DumpServerList()
{
    std::vector<KServerInfo> vecList;
    GetServerList(vecList);

    START_LOG( cerr, L"=== Dump ServerList (size:" << vecList.size() << L") ==="  ) << END_LOG;
    std::vector<KServerInfo>::const_iterator cvit;
    for ( cvit = vecList.begin() ; cvit != vecList.end() ; ++cvit ) {
        START_LOG( cerr, cvit->Str()  ) 
            << BUILD_LOG( KncUtil::toWideString( cvit->StrIP() ) ) 
            << BUILD_LOG( cvit->m_usPort )
            << END_LOG;
    }
    START_LOG( cerr, L"=== End Dump ==="  ) << END_LOG;
}

void KCenterServer::SetUniqueUserStat( IN const DWORD dwChannelType_, IN const DWORD dwUID_ )
{
    KLocker lock( m_csUniqueUser );
    std::set<DWORD> setUIDList;
    setUIDList.clear();

    std::map< DWORD, std::set<DWORD> >::iterator mit;
    std::set<DWORD>::iterator sit;

    mit = m_mapUniqueUserCount.find( dwChannelType_ );
    if( mit == m_mapUniqueUserCount.end() ) {
        setUIDList.insert( dwUID_ );
        m_mapUniqueUserCount.insert( std::make_pair( dwChannelType_, setUIDList ) );
        return;
    }
    sit = mit->second.find( dwUID_ );
    if( sit != mit->second.end() ) return;
    mit->second.insert( dwUID_ );
}

void KCenterServer::GetUniqueUserStat( OUT std::map< DWORD, std::set<DWORD> >& mapUniqueUserCount_ )
{
    mapUniqueUserCount_.clear();

    KLocker lock( m_csUniqueUser );
    mapUniqueUserCount_.swap( m_mapUniqueUserCount );
}



DWORD KCenterServer::GetLoginStayLimitTime()
{
    KLocker lock ( m_csStayLimitTime );
    return m_dwLoginStayLimitTime;
}

DWORD KCenterServer::GetConnectStayLimitTime()
{
    KLocker lock ( m_csStayLimitTime );
    return m_dwConnectStayLimitTime;
}
