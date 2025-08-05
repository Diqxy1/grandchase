#include <Winsock2.h>   
#include "CenterSimLayer.h"
#include "CenterEvent.h"
#include "CenterNetLayer.h"
#include "DefaultFSM.h"
#include "CenterDBThread.h"
#include "CenterGash.h"
#include "CenterServer.h"
#include <boost/format.hpp>
#include "CnFBAuth.h"
#include "CenterLogManager.h"
#include "CnSoapAuth.h"
#include "NUSAuth.h"
#include "SHA1.h"
#include <boost/algorithm/string/trim.hpp>
#include "CnStatisticsManager.h"
#include "CnLoginOutStat.h"
#include "Log4.h"
#include "CnCYOUAuth.h"
#include <boost/lexical_cast.hpp>

#define MAX_FILE_SHA1_BUFFER 1024

ImplementSingleton(KCenterSimLayer);
NiImplementRTTI( KCenterSimLayer, KSimLayer );

KCenterSimLayer::KCenterSimLayer( )
:m_dwNUserWaitTime( 0 )
,m_nNewUserAge( 7 )
,m_nOldUserAge( 1 )
,m_bConnectCountEnable( false )
,m_dwConnectionCount( 0 )
,m_bBlockIPPassCheck( false )
,m_dwFBQueCheckTickGap(60000)
,m_nFBQueueThreshold(500)
,m_bDynamicResourceLoadEnable(false)
,m_bPingConfigChanged(false)
,m_bUseLoadCheckInServer(true)
,m_fFakeCCU(-1)
,m_bCheckOverlapped(false)
,m_bBlockIPUserLogEnable(false)
,m_nServerListAuthLevel(3)
,m_kLoginCCUTimer( 60000 )
,m_kDBQLogTimer( 60000 )
,m_dwUniqueNumber( 0 )
,m_nWrongPasswordLimit( 5 )
,m_bWrongPasswordBlock( false )
,m_kSyncTimeTimer( 60000 * 60 * 3 )
,m_kTimer_ServerHeartbeat( 1000 * 30 )
,uint32( 1000, 9999 )
,die(rng, uint32)
,m_nAccOnlineTimeTest(0)
,m_nUserHeartBeatError(0)
,m_bCYOUBillLogEnable(false)
,m_bAuthEnableForInternalTesop(true)
{
    m_dwLastConnectionTick = ::GetTickCount();
    m_dwDisconnectTypeTick = ::GetTickCount();
    m_dwFBQueCheckLastTick = ::GetTickCount();
    rng.seed( ::GetTickCount() ); // Seed 값 설정.
}

KCenterSimLayer::~KCenterSimLayer()
{
}

ImplToStringW( KCenterSimLayer )
{
    KLocker lock1( m_csNUserWaitTime );
    KLocker lock2( m_csBlockIPPassCheck );
    KLocker lock3( m_csDisconnectType );
    KLocker lock4( m_csBlockIPPassCheck );
    KLocker lock5( m_csServerListAuthLevel );
    KLocker lock6( m_csBlockIPUserLog );
    KLocker lock7( m_csCYOUBillLog );

    return START_TOSTRING_PARENTW( KSimLayer )
        << TOSTRINGW( m_dwNUserWaitTime )
        << TOSTRINGW( m_nNewUserAge )
        << TOSTRINGW( m_nOldUserAge )
        << TOSTRINGWb( m_bConnectCountEnable )
        << TOSTRINGWb( m_bBlockIPPassCheck )
        << TOSTRINGW( m_mapDisconnectType.size() )
        << TOSTRINGW( m_dwFBQueCheckTickGap )
        << TOSTRINGW( m_dwFBQueCheckLastTick )
        << TOSTRINGW( m_nFBQueueThreshold )
        << TOSTRINGWb( m_bDynamicResourceLoadEnable )
        << TOSTRINGWb( m_bUseLoadCheckInServer )
        << TOSTRINGW( m_fFakeCCU )
        << TOSTRINGW( m_nServerListAuthLevel )
        << TOSTRINGW( m_nAccOnlineTimeTest )
        << TOSTRINGW( m_nUserHeartBeatError )
        << TOSTRINGWb( m_bCYOUBillLogEnable )
        << TOSTRINGWb( m_bAuthEnableForInternalTesop )
        << std::boolalpha
        << L"      ST_CHECK_DUPL_LOGIN   : " << CheckTypeFlag(ST_CHECK_DUPL_LOGIN) << std::endl
        << L"      ST_INTERNAL_TESOP     : " << CheckTypeFlag(ST_INTERNAL_TESOP) << std::endl
        << std::noboolalpha;
}

void KCenterSimLayer::Init()
{

    if( CheckAuthTypeFlag( AT_GASH ) )
    {
        m_vecpThreadMgr.push_back( KTThreadManager<KCenterGash>::GetInstance() );
    }

    // NTreevUS 로그인 방식 수정 20090525 tgkwon
    if( CheckAuthTypeFlag( AT_NTREEV ) || CheckAuthTypeFlag( AT_SGI_EU ) ) // TODO : woosh. 임시로 묻어가기. 테스트 기간만...
    {
        m_vecpThreadMgr.push_back( KTThreadManager<KNUSAuth>::GetInstance() );
    }

    if( CheckAuthTypeFlag( AT_FUNBOX_TH ) )
    {
        m_vecpThreadMgr.push_back( KTThreadManager<KCnFBAuth>::GetInstance() );
        m_vecpThreadMgr.push_back( KTThreadManager<KCnPPAuth>::GetInstance() );
    }

    if( CheckAuthTypeFlag( AT_LIN ) )
    {
        m_vecpThreadMgr.push_back( KTThreadManager<KCnSoapAuth>::GetInstance() );
    }

    KSimLayer::Init();

    // UserFactory 생성. KUser가 아닌 다른 객체군을 생성한다면, 코드 구현 후 이곳 한줄만 변경하면 됨.
    // Abstract Factory Pattern.
    m_kActorManager.SetActorFactory( IActorFactoryPtr( new KActorFactory<KCenter, KDefaultFSM, KCenter::CenterPoolTag> ) );
    m_kActorManager2.SetActorFactory( IActorFactoryPtr( new KActorFactory<KNUser, KDefaultFSM, KNUser::NUserPoolTag> ) );

    // CenterServer LogManager 초기화.
    SiKCenterLogManager()->Init();
    //if( CheckAuthTypeFlag( AT_NEXON ) )
    //    ::NMLink_Init();    // 050726. florist. 넥슨 저팬 인증 라이브러리.
}


void KCenterSimLayer::Tick()
{
    KSimLayer::Tick();
    SiKCenterServer()->Tick(); // Center Server의 Tick
    
    if( m_bConnectCountEnable && ::GetTickCount() - m_dwLastConnectionTick > 60 * 60 * 1000 )
    {
        SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_CONNECTION_COUNT_RECORD_REQ, L"", 0, 0, m_dwConnectionCount );
        m_dwConnectionCount = 0;
        m_dwLastConnectionTick = ::GetTickCount();
    }

    if( ::GetTickCount() - m_dwDisconnectTypeTick > 60 * 60 * 1000 )
    {
        std::map<int,int> mapDisconnectType;
        GetDisconnectReson( mapDisconnectType );
        SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_DB_UPDATE_DISCONNECT_TYPE, L"", 0, 0, mapDisconnectType );
        m_dwDisconnectTypeTick = ::GetTickCount();
    }

    if ( CheckPingConfigChange() ) {
        KPingConfig kData;
        GetPingConfig( kData );
        m_kActorManager.SendToAll( KCenterEvent::ECN_CLIENT_PING_CONFIG_NOT, kData );
    }

    if( CheckAuthTypeFlag( AT_FUNBOX_TH ) ) CheckFBAutQueue();

    if ( m_kLoginCCUTimer.CheckTime() ) {
        DWORD dwCCU = m_kActorManager2.GetCount();
        SiKCenterDBMgr()->QueueingEvent( KCenterEvent::ECN_UPDATE_LOGIN_CCU, L"", 0, 0, dwCCU );

        KCnLoginOutStatPtr spStat = GET_STAT( KCnLoginOutStat, SI_LOGINOUT_STAT );
        if ( spStat ) {
            spStat->UpdateCCU( dwCCU );
        }

    }

    if ( m_kDBQLogTimer.CheckTime() ) {
        int nQueueCount = SiKCenterDBMgr()->GetQueueSize();

        std::wstringstream stm;
        stm << L"[" << KNC_TIME_STRING << L"], " << nQueueCount;
        
        SiKCenterLogManager()->Get( KCenterLogManager::LT_DB_QUEUE_SIZE ) << stm.str() << GLOG::endl;

        stm.clear();
        stm.str(L"");
    }

    if( m_kSyncTimeTimer.CheckTime() )
    {
        SiKCenterDBMgr()->QueueingID( KCenterEvent::ECN_DB_EVENT_TIME_SYNCRONIZE_NOT, L"", 0 );
    }

    if ( m_kTimer_ServerHeartbeat.CheckTime() ) {
        KCYOUServerInfo kPacket;
        GetAllServerCCU( kPacket.m_nCCU );
        
        GetBillConnIP( kPacket.m_strIP );
        if ( CheckTypeFlag( KCenterSimLayer::ST_KOG_AUTH_TEST) ) {
            kPacket.m_strIP = "14.45.79.140"; // TODO : 테스트 종료 후 제거.
        }        
        //KTThreadManager<KCnCYOUAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_SERVER_HEART_BEAT_REQ, L"", 0, 0, kPacket );
    }
}

void KCenterSimLayer::GetAllServerCCU( OUT int& nTotalCCU_ )
{
    std::vector< KServerInfo > kServerInfoList;
    SiKCenterServer()->GetServerList( kServerInfoList );

    nTotalCCU_ = 0;

    std::vector< KServerInfo >::iterator vit;
    for ( vit = kServerInfoList.begin() ; vit != kServerInfoList.end() ; ++vit ) {
        nTotalCCU_ += vit->m_iUserNum;
    }
}

void KCenterSimLayer::ShutDown()
{
    //if( CheckAuthTypeFlag( AT_NEXON ) )
    //    ::NMLink_Terminate();    // 050726. florist. 넥슨 저팬 인증 라이브러리.

    KSimLayer::ShutDown();

    KTThreadManager<KCenterDBThread>::ReleaseInstance();
    KTThreadManager<KCenterGash>::ReleaseInstance();    // flag 체크없이 항상 해준다. Release 여러번 해서 나쁠거 없다.
    KTThreadManager<KNUSAuth>::ReleaseInstance();
    KTThreadManager<KCnFBAuth>::ReleaseInstance();
    KTThreadManager<KCnPPAuth>::ReleaseInstance();
    //KTThreadManager<KCnCYOUAuth>::ReleaseInstance();
}
void KCenterSimLayer::GetNUserWaitTime( DWORD& dwNUserWaitTime_ )
{
    KLocker lock( m_csNUserWaitTime );

    dwNUserWaitTime_ = m_dwNUserWaitTime;
}

void KCenterSimLayer::SetNUserWaitTime( DWORD dwNUserWaitTime_ )
{
    KLocker lock( m_csNUserWaitTime );
    m_dwNUserWaitTime = dwNUserWaitTime_;
}


#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"
ImplOstmOperatorW2A( KCenterSimLayer );
ImplOstmOperatorW2A( KTThreadManager<KCenterDBThread> );

void KCenterSimLayer::RegToLua( std::shared_ptr<lua_State> L )
{
    KSimLayer::RegToLua( L );

    luabind::module( L.get() )
    [
        luabind::def( "GetSimLayer", &KCenterSimLayer::GetInstance ),
        luabind::class_<KCenterSimLayer, KSimLayer>( "KCenterSimLayer" )
            .def( luabind::tostring(luabind::self) )
            .def( "SetNUserWaitTime", &KCenterSimLayer::SetNUserWaitTime )
            .def( "SetBlockIPPassCheck", &KCenterSimLayer::SetBlockIPPassCheck )
            .def( "SetFBAuthQ",             &KCenterSimLayer::SetFBAuthQ )
            .def( "SetPingConfig",          &KCenterSimLayer::SetPingConfig )
            .def( "SetUseLoadCheckInServer", &KCenterSimLayer::SetUseLoadCheckInServer )
            .def( "SetFakeCCU",              &KCenterSimLayer::SetFakeCCU )
            .def_readwrite( "m_nNewUserAge", &KCenterSimLayer::m_nNewUserAge )
            .def_readwrite( "m_nOldUserAge", &KCenterSimLayer::m_nOldUserAge )
            .def_readwrite( "m_bConnectCountEnable", &KCenterSimLayer::m_bConnectCountEnable )
            .def_readwrite( "m_bDynamicResourceLoadEnable", &KCenterSimLayer::m_bDynamicResourceLoadEnable )
            .def_readwrite( "m_nServerListAuthLevel", &KCenterSimLayer::m_nServerListAuthLevel )
            .def_readwrite( "m_nAccOnlineTimeTest", &KCenterSimLayer::m_nAccOnlineTimeTest )
            .def_readwrite( "m_nUserHeartBeatError", &KCenterSimLayer::m_nUserHeartBeatError )
            .def( "SetCheckOverlapped", &KCenterSimLayer::SetCheckOverlapped )
            .def( "KickUser", &KCenterSimLayer::SendKickUserNot )
            .def( "SetBlockIPUserLogEnable", &KCenterSimLayer::SetBlockIPUserLogEnable )
            .def( "SetWrongPasswordLimit", &KCenterSimLayer::SetWrongPasswordLimit )
            .def( "SetUseBlockWrongPassword", &KCenterSimLayer::SetUseBlockWrongPassword )
            .def( "testcyou", &KCenterSimLayer::CYOUChangeCash )
            .def( "SetBillConnIP",          &KCenterSimLayer::SetBillConnIP )
            .def( "SetCYOUBillLogEnable", &KCenterSimLayer::SetCYOUBillLogEnable )
            .def( "SetAuthEnableForInternalTesop", &KCenterSimLayer::SetAuthEnableForInternalTesop )
            .enum_( "AuthType" )
            [
                luabind::value( "ST_CHECK_DUPL_LOGIN",  ST_CHECK_DUPL_LOGIN ),
                luabind::value( "ST_INTERNAL_TESOP",    ST_INTERNAL_TESOP ),
                luabind::value( "ST_KOG_AUTH_TEST",     ST_KOG_AUTH_TEST ),
                luabind::value( "ST_FORCE_DWORD",       ST_FORCE_DWORD )
            ],

        luabind::def( "Init_CenterDB", &KCenterDBThread::InitFileDSN ),
        luabind::def( "Init_MainDB", &KCenterDBThread::InitFileDSNMain ),
        luabind::def( "GetDBHandler", KTDBLayer<KCenterDBThread>::GetInstance ),
        luabind::class_<KTDBLayer<KCenterDBThread>, KDBLayer>( "KCenterDBHandler" )
            .def( luabind::tostring(luabind::self) ),

        luabind::def( "GetGashManager", &KTThreadManager<KCenterGash>::GetInstance ),
        luabind::def( "Init_GashInfo", &KCenterGash::InitGashInfo ),
        luabind::class_<KTThreadManager<KCenterGash>, KThreadManager>( "KGashManager" )
            .def( luabind::tostring(luabind::self ) ),

        luabind::def( "Init_NUSAuth", &KNUSAuth::InitFileDSN ),
        luabind::def( "GetNUSAuthManager", &KTThreadManager<KNUSAuth>::GetInstance ),
        luabind::class_<KTThreadManager<KNUSAuth>, KThreadManager>( "KNUSAuthManager" )
            .def( luabind::tostring(luabind::self ) ),

        luabind::def( "GetFBAuthManager", &KTThreadManager<KCnFBAuth>::GetInstance ),
        luabind::def( "SetAuthIP", &KCnFBAuth::SetAuthIP ),
        luabind::def( "AddAuthPort", &KCnFBAuth::AddAuthPort ),
        luabind::class_<KTThreadManager<KCnFBAuth>, KThreadManager>( "KCnFBAuthManager" )
            .def( luabind::tostring(luabind::self ) ),

        luabind::def( "GetPPAuthManager", &KTThreadManager<KCnPPAuth>::GetInstance ),
        luabind::class_<KTThreadManager<KCnPPAuth>, KThreadManager>( "KCnPPAuthManager" )
            .def( luabind::tostring(luabind::self ) ),

        luabind::def( "GetSoapManager", &KTThreadManager<KCnSoapAuth>::GetInstance ),
        luabind::def( "SetWsdlPath", &KCnSoapAuth::SetWsdlPath ),
        luabind::def( "SetWebMethodPath", &KCnSoapAuth::SetWebMethodPath ),
        luabind::def( "SetGameServiceID", &KCnSoapAuth::SetWebMethodPath ),
        luabind::class_<KTThreadManager<KCnSoapAuth>, KThreadManager>( "KCnSoapAuthManager" )
            .def( luabind::tostring(luabind::self ) )/*,

        luabind::def( "GetCYOUAuth", &KTThreadManager<KCnCYOUAuth>::GetInstance ),
        luabind::def( "InitCYOUBillingInfo", &KCnCYOUAuth::InitCYOUBillingInfo ),
        luabind::class_<KTThreadManager<KCnCYOUAuth>, KThreadManager>( "KCnCYOUAuth" )
            .def( luabind::tostring(luabind::self ) )*/

    ];
}

void KCenterSimLayer::AddDisconnectReson( IN const int nType_ )
{
    KLocker lock( m_csDisconnectType );
    m_mapDisconnectType[nType_] += 1;
}

void KCenterSimLayer::GetDisconnectReson( OUT std::map<int,int>& mapDisconnectType_ )
{
    mapDisconnectType_.clear();
    KLocker lock( m_csDisconnectType );
    m_mapDisconnectType.swap( mapDisconnectType_ );
}



void KCenterSimLayer::CheckFBAutQueue()
{
    KLocker lock( m_csBlockIPPassCheck );
    if( ::GetTickCount() - m_dwFBQueCheckLastTick <= m_dwFBQueCheckTickGap ) return;

    START_LOG( cerr, L" Thai Auth Queue Check...  ") << END_LOG;

    int nQueueSize = KTThreadManager<KCnFBAuth>::GetInstance()->GetQueueSize();
    if( nQueueSize > m_nFBQueueThreshold )
    {
        KTThreadManager<KCnFBAuth>::GetInstance()->ClearQueue();
        START_LOG( cerr, L" KCnFBAuth Queue Clear : " << nQueueSize ) << END_LOG;
    }

    nQueueSize = KTThreadManager<KCnPPAuth>::GetInstance()->GetQueueSize();
    if( nQueueSize > m_nFBQueueThreshold )
    {
        KTThreadManager<KCnPPAuth>::GetInstance()->ClearQueue();
        START_LOG( cerr, L" KCnFBAuth Queue Clear : " << nQueueSize ) << END_LOG;
    }
    m_dwFBQueCheckLastTick = ::GetTickCount();

}

void KCenterSimLayer::SetFBAuthQ( const DWORD dwTickGap_, const int nThreshold_ )
{
    KLocker lock( m_csBlockIPPassCheck );
    m_dwFBQueCheckTickGap = dwTickGap_;
    m_nFBQueueThreshold = nThreshold_;
}

void KCenterSimLayer::GetPingConfig( OUT KPingConfig& kPingConfig_ )
{
    KLocker lock( m_csPingInfo );
    kPingConfig_ = m_kPingConfig;
}

void KCenterSimLayer::SetPingConfig( IN const DWORD dwPingCheckGap_, IN const DWORD dwPingLimitGap_, IN const DWORD dwPingLimitValue_, IN const int nRepetitionCount_, IN const bool bEnable_, IN const int nJoinRoomLimitPing_ )
{
    KLocker lock( m_csPingInfo );
    m_kPingConfig.m_dwPingCheckGap = dwPingCheckGap_;
    m_kPingConfig.m_dwPingLimitGap = dwPingLimitGap_;
    m_kPingConfig.m_dwPingLimitValue = dwPingLimitValue_;
    m_kPingConfig.m_nRepetitionCount = nRepetitionCount_;
    m_kPingConfig.m_bEnable = bEnable_;
    m_bPingConfigChanged = true;
    m_kPingConfig.m_nJoinRoomLimitPing = nJoinRoomLimitPing_;
}

bool KCenterSimLayer::CheckPingConfigChange()
{
    KLocker lock( m_csPingInfo );
    if ( m_bPingConfigChanged ) {
        m_bPingConfigChanged = false;
        return true;
    }

    return false;
}

void KCenterSimLayer::SendKickUserNot( IN const DWORD& dwUserUID_ )
{
    KECN_KICK_USER_NOT kNot;
    kNot.m_dwDestUserUID    = dwUserUID_;
    kNot.m_nReason          = KEVENT_CLOSE_CONNECTION_NOT::CCT_DETECT_HACK;
    m_kActorManager.SendToAll( KCenterEvent::ECN_KICK_USER_NOT, kNot );
}

std::string KCenterSimLayer::MakeSHA1( IN std::wstring& strInput_ )
{
    boost::trim( strInput_ );
    std::string strSourceSHA = KncUtil::toNarrowString( strInput_ );

    char strSHACheckSum[MAX_FILE_SHA1_BUFFER] = {0,};
    CSHA1 cSHA1;
    cSHA1.Update( (unsigned char*)strSourceSHA.c_str(), strSourceSHA.length() );
    cSHA1.Final();
    cSHA1.ReportHash(strSHACheckSum, CSHA1::REPORT_HEX, true);

    std::string strRet = strSHACheckSum;
    boost::trim( strRet );
    START_LOG( clog, L"SHA1 length : " << strRet.length() ) << END_LOG;

    return strRet;
}

int KCenterSimLayer::GetWrongPasswordLimit()
{
    KLocker lock( m_csWrongPasswordLimit );
    return m_nWrongPasswordLimit;
}

void KCenterSimLayer::SetWrongPasswordLimit( IN const int nWrongPasswordTime_ )
{
    KLocker lock( m_csWrongPasswordLimit );
    m_nWrongPasswordLimit = nWrongPasswordTime_;
}

void KCenterSimLayer::SetUseBlockWrongPassword( IN bool bUseBlockBruteForceAttack_ )
{ 
    KLocker lock( m_csWrongPasswordLimit ); 
    m_bWrongPasswordBlock = bUseBlockBruteForceAttack_; 
};

bool KCenterSimLayer::GetUseBlockWrongPassword()
{
    KLocker lock( m_csWrongPasswordLimit ); 
    return m_bWrongPasswordBlock;
}

void KCenterSimLayer::InitCYOUBilling( std::wstring& strServerName_, DWORD dwUID_ )
{
}

void KCenterSimLayer::CYOUChangeCash()
{
	/*
    KCYOUCashData kPacket;
    kPacket.m_strServerIP = "14.45.79.140";
    kPacket.m_strLogin = "sh2fcm0004@changyou.com";
    kPacket.m_strUserIP = "192.168.61.29";
    kPacket.m_strCharNickname = "Elesis";
    kPacket.m_strSerialNumber = "800000000000000000000";
    kPacket.m_strItemInfo = "999,1,100";
    
    KTThreadManager<KCnCYOUAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_CYOU_CHANGE_CASH_REQ, L"" , 0, 0, kPacket );

    KTThreadManager<KCnCYOUAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_CYOU_GET_REMAIN_CASH_REQ, L"" , 0, 0, kPacket );
    
    KTThreadManager<KCnCYOUAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_CYOU_BUY_LOG_REQ, L"" , 0, 0, kPacket );


    KCYOUAuthUserInfo kData;
    kData.m_strServerIP = "14.45.79.140";
    kData.m_strIP = "192.168.61.29";
    kData.m_strLogin = "sh2fcm0004@changyou.com";

    KTThreadManager<KCnCYOUAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_USER_AGREEMENT_REQ, L"" , 0, 0, kData );
	*/

}

void KCenterSimLayer::MakeCYOUSerialNumber( OUT std::string& strSerialNumber_ )
{
}

void KCenterSimLayer::SetBillConnIP( IN const char* szIP_ )
{
    KLocker lock( m_csBillConnIP );
    m_strBillConnIP = szIP_;
}
