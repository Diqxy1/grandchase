#include ".\nuser.h"
#include <dbg/dbg.hpp>
#include "DefaultFSM.h"
#include "CenterSimLayer.h"
#include "CenterGash.h"
#include "CenterServer.h"
#include <cctype>
#include "CenterDBThread.h"
#include <boost/tokenizer.hpp>
#include "BlockIP.h"
#include "CnItemManager.h"
#include "CnClientScriptManager.h"
#include "CnGuildManager.h" 
#include "CnClientContentsManager.h"
#include "CnClientScriptManager.h"
#include "NUSAuth.h"
#include "CnFBAuth.h"
#include "CenterLogManager.h"
#include "CnSoapAuth.h"
#include "CnShutdownUser.h"
#include "Log4.h"
#include "CnLoginOutStat.h"
#include "CnStatisticsManager.h"
#include "CnClientContentsInfo.h"
#include "CnUpdatePlanManager.h"
#include "CnCYOUAuth.h"

#pragma comment( lib, "NMCrypt" ) // for DecryptString()
#include "../GameServer/CPSSOLib.h" // 넷마블 인증

namespace {

    struct KIsConnect
    {
        bool    m_bDuplicate;
        DWORD   m_dwUID;

        KIsConnect ( DWORD dwUID ):m_dwUID(dwUID), m_bDuplicate(false){};
        void operator() ( const KActorPtr spActor )
        {
            if( !spActor ) return;
            KCenterPtr spCenter = std::static_pointer_cast<KCenter>( spActor );
            if( spCenter->IsConnectedUser( m_dwUID ) )
            {
                m_bDuplicate = true;
            }
        }
        operator bool() const { return m_bDuplicate; }
    };
}

NiImplementRTTI( KNUser, KActor );
ImplementException( KNUser );

#define CLASS_TYPE  KNUser
#define EVENT_TYPE  KNUserEvent

KNUser::KNUser(void)
:   m_strArrangedServer( L"" ),
    m_dwAuthType( 0x00000000 ),
    m_cAuthLevel( 0 ),
    m_nWrongpasswordInputTime( 0 )
{
    m_strBillingAccount.clear();
    SetTick( KNUser::SEND_SERVER_LIST );
    SetTick( KNUser::AUTHEN_TICK );
    SetTick( KNUser::SEND_CHANNEL_NEWS );
    SetTick( KNUser::CONNECT_TICK ); // 마지막에 있는것이 만약을 위해서 좋겠다.
}

KNUser::~KNUser(void)
{
}

ImplToStringW( KNUser )
{
    return START_TOSTRING_PARENTW( KActor );
}


KNUserPtr KNUser::Create()
{
    return SiKCenterSimLayer()->m_kActorManager2.CreateActor<KNUser>();
}

void KNUser::AddManager( KNUserPtr spNuser_ )
{
    JIF( spNuser_ );
    JIF( SiKCenterSimLayer()->m_kActorManager2.Add( spNuser_ ) );
}

void KNUser::UpdateTicksData()
{
    switch( GetStateID() )
    {
    case KDefaultFSM::STATE_LOGINED:

        if( ::GetTickCount() - GetTick( KNUser::AUTHEN_TICK ) >= ( SiKCenterServer()->GetLoginStayLimitTime() )  )
        {
            SEND_ID( ENU_STAY_SERVER_TIMEOUT );
            ReserveDestroy();

            START_LOG( cwarn, L"로그인 상태로 오래 머물러서 종료 예약됨" )
                << BUILD_LOG( m_strName )
                << BUILD_LOG( ::GetTickCount() )
                << BUILD_LOG( GetTick( KNUser::AUTHEN_TICK ) ) << END_LOG;
            return;
        }

        if ( SiKCenterSimLayer()->CheckTypeFlag( KCenterSimLayer::ST_INTERNAL_TESOP ) ) {
            if ( ::GetTickCount() - GetTick( KNUser::SEND_SERVER_LIST ) >= 10 * 1000 ) {
                UpdateServerList();
            }
        }
        // 일정 주기로 Server List 를 BroadCasting 한다.
        if( ::GetTickCount() - GetTick( KNUser::SEND_SERVER_LIST ) >= KBaseServer::ms_dwSendServerListGap )
        {
            UpdateServerList();
        }
        
        if( ::GetTickCount() - GetTick( KNUser::SEND_CHANNEL_NEWS ) >= SiKCenterServer()->GetChannelNewsGap() )
        {
            UpdateChannelNews();
        }
        break;

    case KDefaultFSM::STATE_CONNECTED:
        if( ::GetTickCount() - GetTick( KNUser::CONNECT_TICK ) >= ( SiKCenterServer()->GetConnectStayLimitTime() ) ) //
        {
            SEND_ID( ENU_STAY_SERVER_TIMEOUT );
            ReserveDestroy();

            START_LOG( cwarn, L"접속 상태로 오래 머물러서 종료 예약됨" )
                << BUILD_LOG( m_strName )
                << BUILD_LOG( ::GetTickCount() )
                << BUILD_LOG( GetTick( KNUser::CONNECT_TICK ) ) << END_LOG;
        }
        break;
       
    default:
        //디폴트 처리는 하지 마삼.. Tick에서 하니까.
        break;
    } //end switch

}

void KNUser::UpdateServerList()
{
    std::vector< KServerInfo > kServerInfoList;
    SiKCenterServer()->GetServerList( kServerInfoList );
    CheckServerListByAuthLevel( kServerInfoList );

    KCnClientContentsInfo kUsingICC;
    SiKCnUpdatePlanManager()->GetUsingICC( kUsingICC );

    kUsingICC.SetServerBenefitMsg( kServerInfoList );

    SendPacket( KNUserEvent::ENU_SERVER_LIST_NOT, kServerInfoList, false );
    SetTick( KNUser::SEND_SERVER_LIST );
}

void KNUser::Tick()
{
    KActor::Tick();

    UpdateTicksData();

    switch( GetStateID() )
    {
    case KDefaultFSM::STATE_INIT:
    case KDefaultFSM::STATE_CONNECTED:
    case KDefaultFSM::STATE_LOGINED:
        break;

    case KDefaultFSM::STATE_EXIT:
        SiKCenterSimLayer()->m_kActorManager2.ReserveDelete( m_strName );
        m_bDestroyReserved = true;
        break;

    default:
        START_LOG( cerr, L"있을수 없는 상황이다." ) //Tick 에서 로그를 남기는 것은 좋지 않은것 같다.
            << BUILD_LOG( m_strName )
            << BUILD_LOG( GetStateIDString() ) << END_LOG;
        break;
    }//end switch
}

void KNUser::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

    switch( spEvent_->m_usEventID )
    {
        CASE( ENU_VERIFY_ACCOUNT_REQ );
        CASE( ENU_VERIFY_ACCOUNT_ACK );
        CASE( EVENT_CLOSE_CONNECTION_NOT );
        //_CASE( ENU_CONNECT_TO_SERVER_REQ, std::wstring );
       _CASE( ENU_OVERLAP_FILE_INFO, std::vector<KOverlapFile> );
        CASE_NOPARAM( ENU_GUIDE_BOOK_LIST_REQ );
       _CASE( ENU_GUIDE_BOOK_LIST_ACK, MAP_SHORT_SHORT );
        CASE_NOPARAM( ENU_TEXTURE_DYNAMIC_LOAD_REQ );
        CASE_NOPARAM( ENU_LOADING_IMAGE_REQ );
        CASE_NOPARAM( ENU_CLIENT_CONTENTS_FIRST_INIT_INFO_REQ );
        CASE_NOPARAM( ENU_CLIENT_PING_CONFIG_REQ );
        CASE_NOPARAM( ENU_SHAFILENAME_LIST_REQ );
        CASE_NOPARAM( ENU_USER_AGREEMENT_REQ );
        CASE_NOPARAM( ENU_CLIENT_CONTENTS_FIRST_INIT_ADDITIONAL_INFO_REQ );
        CASE(ENU_ACCOUNT_REGISTER_REQ);
        CASE(ENU_ACCOUNT_REGISTER_ACK);

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음." )
            << BUILD_LOG( GetName() )
            << BUILD_LOG( spEvent_->GetEventIDString() ) << END_LOG;
        break;
    }

    dwElapTime = ::GetTickCount() - dwElapTime;
    if( dwElapTime > 500 )
    {
        START_LOG_WITH_NAME( cwarn )
            << BUILD_LOG( spEvent_->GetEventIDString() )
            << BUILD_LOG( spEvent_->m_usEventID )
            << BUILD_LOG( dwElapTime ) << END_LOG;
    }

}


void KNUser::OnAcceptConnection()
{
    KSession::OnAcceptConnection();
    DWORD dwWaitTime = 0;
    SiKCenterSimLayer()->GetNUserWaitTime( dwWaitTime );
    SendPacket( KNUserEvent::ENU_WAIT_TIME_NOT, dwWaitTime );
}

IMPL_ON_FUNC( ENU_VERIFY_ACCOUNT_REQ )
{
    KCnLoginOutStatPtr spStat = GET_STAT( KCnLoginOutStat, SI_LOGINOUT_STAT );
    if ( spStat ) {
        spStat->AddStat( KCnLoginOutStat::LOGIN );
    }

    kPacket_.m_strIP = KncUtil::toWideString( GetIPStr() );  // m_strIP 변수는 클라이언트에서 담지 않는다. 받자마자 채워놓는다.
    SET_ERROR( ERR_UNKNOWN );

    KENU_VERIFY_ACCOUNT_ACK kPacket;    
    kPacket.m_nOK = NetError::ERR_UNKNOWN;

    kPacket_.m_bExpAccount = false;
    kPacket_.m_dwChannellingType = KNUser::USER_CT_PUBLISHER_1;   // 기본적으로 채널링Type은 1로 초기화.( 1은 해당 퍼블리셔 )

    KShutdownUserConfig kShutdownConfig;
    SiKCnShutdownUser()->GetConfig( kShutdownConfig );



    START_LOG( clog, L"ENU_VERIFY_ACCOUNT_REQ" )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strLogin ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) )
        << BUILD_LOG( kPacket_.m_strIP )
        << BUILD_LOG( kPacket_.m_nProtocolVer )
        << BUILD_LOG( kPacket_.m_strServiceTicket )
        << BUILD_LOG( kPacket_.m_dwAuthType )
        << BUILD_LOG( kPacket_.m_bMale )
        << BUILD_LOG( kPacket_.m_iAge )
        << BUILD_LOG( kPacket_.m_uiSessionKeyHigh )
        << BUILD_LOG( kPacket_.m_uiSessionKeyLow )
        << END_LOG;

    // 프로토콜 버전 확인
    if( kPacket_.m_nProtocolVer != SiKCenterServer()->m_nProtocolVer2 )
    {
        START_LOG( cerr, L"Invalid Protocol Version" )
            << L"Wanted : " << SiKCenterServer()->m_nProtocolVer2 << dbg::endl
            << L"Current :" << kPacket_.m_nProtocolVer  << END_LOG;
        ReserveDestroy();
        SET_ERR_GOTO( ERR_NUSER_VERIFY_00, err_proc );
    }

    //등록된 인증 타입인가?
    if( !SiKCenterSimLayer()->CheckAuthTypeFlag( kPacket_.m_dwAuthType ) )
    {
        ReserveDestroy();
        SET_ERR_GOTO( ERR_NUSER_VERIFY_05, err_proc );
    }

    if ( false == kShutdownConfig.m_bEnable ) {
        kPacket_.m_bMale = false;
        kPacket_.m_iAge = 20;
        START_LOG( clog, L"셧다운제 미사용시, 기본값 설정." ) << END_LOG;
    }

    START_LOG( clog, L"유저나이 : " << kPacket_.m_iAge ) << END_LOG;

    // 테스트 서버일때의 처리.
    if ( SiKCenterSimLayer()->CheckTypeFlag( KCenterSimLayer::ST_INTERNAL_TESOP ) ) {
        // 중국 외 국가 처리
        if ( SiKCenterSimLayer()->GetBillingType() != KSimLayer::BT_CN_CYOU ) {
            SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_ENU_VERIFY_ACCOUNT_REQ, m_strName.c_str(), 0, 0, kPacket_ );

            return;
        }

        // 중국은 현지 테스트를 위해 사내에서도 인증서버를 통과하도록 한다
        if ( false == SiKCenterSimLayer()->GetAuthEnableForInternalTesop() ) {
            SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_ENU_VERIFY_ACCOUNT_REQ, m_strName.c_str(), 0, 0, kPacket_ );

            return;
        }
    }

    switch( kPacket_.m_dwAuthType ) {
        case KSimLayer::AT_GASH:
            // 빈펀 유저의 경우 ChannellingType를 KNUser::USER_CT_PUBLISHER_2로 세팅.
            if( kPacket_.m_bBeanfunUser ) {
                kPacket_.m_dwChannellingType = KNUser::USER_CT_PUBLISHER_2;
            }
            KTThreadManager<KCenterGash>::GetInstance()->QueueingEvent( KNUserEvent::ENU_VERIFY_ACCOUNT_REQ, m_strName.c_str(), 0, 0, kPacket_ );
            return;

        case KSimLayer::AT_NETMARBLE:
            {
                // 넷마블 새 인증 방식.
                char szUserID[256]      = {0};
                char szUniID[256]       = {0}; // 넷마블CN
                char szSex[256]         = {0};
                char szAge[256]         = {0};
                char szCPTS[256]        = {0};
                char szState[256]       = {0}; // 체험 계정
                char szGameCode[256]    = {0}; // 체험 계정
                char szEndDate[256]     = {0}; // 체험 계정
                char szCertificate[256] = {0}; // 체험 계정
                char szCCode[256]       = {0}; // CCode값.
                int nCCode = 0;                // CCode값을 받아서 parsing하기 위한 변수.

                if( kPacket_.m_strPasswd.find("UserID=") != std::string::npos &&
                    GetCPCookieValue( kPacket_.m_strPasswd.c_str(), "UserID", szUserID) == true &&
                    GetCPCookieValue( kPacket_.m_strPasswd.c_str(), "UniID", szUniID) == true &&
                    GetCPCookieValue( kPacket_.m_strPasswd.c_str(), "Sex", szSex) == true &&
                    GetCPCookieValue( kPacket_.m_strPasswd.c_str(), "Age", szAge) == true &&
                    GetCPCookieValue( kPacket_.m_strPasswd.c_str(), "CPTS", szCPTS) == true &&
                    GetCPCookieValue( kPacket_.m_strPasswd.c_str(), "CCode", szCCode) == true )
                {
                    kPacket_.m_strLogin     = CharLowerA(szUserID);
                    kPacket_.m_strPasswd    = szUniID ;
                    std::string strSex      = szSex ;
                    kPacket_.m_bMale        = (strSex.compare( "1" ) == 0) ? true : false;
                    kPacket_.m_iAge         = atoi( szAge );
                    nCCode = atoi( szCCode );

                    if( kPacket_.m_strSpardParam.empty() ) {
                        START_LOG( cerr, L"전달 받은 SpareParam이 비었습니다." )
                            << BUILD_LOG( kPacket_.m_strLogin.c_str() ) << END_LOG;
                    }

                    // 넘어오는 CCode값의 확인을 위해서 로그 추가.
                    START_LOG( clog, L"전달받은 CCode값 : " << nCCode ) << END_LOG;

                    // 채널링 추가 
                    // 채널 타입 체크에 대해서 추가.( 넷마블/투니버스/네이트/네이버 )
                    if( KNUser::CCODE_PUBLISHER_2 == nCCode ) {
                        kPacket_.m_dwChannellingType = KNUser::USER_CT_PUBLISHER_2;
                    } else if( KNUser::CCODE_PUBLISHER_3 == nCCode ) {
                        kPacket_.m_dwChannellingType = KNUser::USER_CT_PUBLISHER_3;
                    } else if( KNUser::CCODE_PUBLISHER_4 == nCCode ) {
                        kPacket_.m_dwChannellingType = KNUser::USER_CT_PUBLISHER_4;
                    } else {
                        kPacket_.m_dwChannellingType = KNUser::USER_CT_PUBLISHER_1;
                    }
/*
                    if( kPacket_.m_strLogin.find("@tooni") != std::string::npos ) {
                        kPacket_.m_dwChannellingType = KNUser::USER_CT_PUBLISHER_2;
                        START_LOG( cwarn, L"투니버스 유저계정입니다." )
                            << BUILD_LOG( kPacket_.m_strLogin ) << END_LOG;
                    } else {
                        kPacket_.m_dwChannellingType = KNUser::USER_CT_PUBLISHER_1;
                    }
*/
                    if( kPacket_.m_strPasswd.find("EX") != std::string::npos )
                    {
                        kPacket_.m_bExpAccount = true;
                        START_LOG( cwarn, L"체험계정입니다." )
                            << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strLogin ) ) << END_LOG;
                    }

                    if( CheckNMCPTS( std::string(szCPTS) ) )
                    {
                        SiKCenterServer()->GetDBLayer()->QueueingEvent( KCenterEvent::DB_ENU_VERIFY_ACCOUNT_REQ, m_strName.c_str(), 0, 0, kPacket_ );
                        return;
                    }
                    else
                    {
                        START_LOG( cerr, L"CPTS 오류 Name : " << KncUtil::toWideString( kPacket_.m_strLogin ) << L", IP : " << kPacket_.m_strIP ) << END_LOG;
                    }
                }

                ReserveDestroy();
                SET_ERR_GOTO( ERR_NUSER_VERIFY_06, err_proc );
            }
            break;
        case KSimLayer::AT_LEVELUP: // 바로 DB로 넘긴다.
            SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_ENU_VERIFY_ACCOUNT_REQ, m_strName.c_str(), 0, 0, kPacket_ );
            return;
        case KSimLayer::AT_NTREEV:
            if ( !CheckValidate( kPacket_.m_strLogin ) ) {
                SET_ERR_GOTO( ERR_NUSER_VERIFY_07, err_proc );
            }
            kPacket_.m_nSGIGamecode = KENU_VERIFY_ACCOUNT_REQ::GAMECODE_GC_US;
            KTThreadManager<KNUSAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_VERIFY_ACCOUNT_REQ, m_strName.c_str(), 0, 0, kPacket_ );
            return;

        case KSimLayer::AT_SGI_EU:
            if ( !CheckValidate( kPacket_.m_strLogin ) ) {
                SET_ERR_GOTO( ERR_NUSER_VERIFY_07, err_proc );
            }
            kPacket_.m_nSGIGamecode = KENU_VERIFY_ACCOUNT_REQ::GAMECODE_GC_EU;
            // NTreevUS 로그인 방식 수정 20090525 tgkwon
            KTThreadManager<KNUSAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_VERIFY_ACCOUNT_REQ, m_strName.c_str(), 0, 0, kPacket_ );
            return;

        case KSimLayer::AT_LEVELUP_PH:
            if( !CheckValidate( kPacket_.m_strLogin ) )
            {
                SET_ERR_GOTO( ERR_NUSER_VERIFY_07, err_proc );
            }
            SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_ENU_VERIFY_ACCOUNT_REQ, m_strName.c_str(), 0, 0, kPacket_ );
            return;

        case KSimLayer::AT_FUNBOX_TH:
            {
                if( kPacket_.m_nFunBoxPostfix == 0 )
                    KTThreadManager<KCnFBAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_VERIFY_ACCOUNT_REQ, m_strName.c_str(), 0, 0, kPacket_ );
                else
                    KTThreadManager<KCnPPAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_VERIFY_ACCOUNT_REQ, m_strName.c_str(), 0, 0, kPacket_ );
            }
            return;

        case KSimLayer::AT_MEGAXUS:
            if( !CheckValidate( kPacket_.m_strLogin ) )
            {
                SET_ERR_GOTO( ERR_NUSER_VERIFY_07, err_proc );
            }
            SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_ENU_VERIFY_ACCOUNT_REQ, m_strName.c_str(), 0, 0, kPacket_ );
            return;

        case KSimLayer::AT_LIN:
            if( !CheckValidate( kPacket_.m_strLogin ) ) {
                SET_ERR_GOTO( ERR_NUSER_VERIFY_07, err_proc );
            }
            KTThreadManager<KCnSoapAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_VERIFY_ACCOUNT_REQ, m_strName.c_str(), 0, 0, kPacket_ );
            return;

        case KSimLayer::AT_CYOU:
            //KTThreadManager<KCnCYOUAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_VERIFY_ACCOUNT_REQ, m_strName.c_str(), 0, 0, kPacket_ );
			throw new std::exception("AT_CYOU not supported");
            return;

        default:
            START_LOG( cerr, L"undefined Authenticate type." )
                //<< BUILD_LOG( SiKCenterSimLayer()->GetAuthTypeStr( kPacket_.m_dwAuthType ) )
                << BUILD_LOG( kPacket_.m_dwAuthType )
                << END_LOG;
            ReserveDestroy();
            SET_ERR_GOTO( ERR_NUSER_VERIFY_05, err_proc );
    }
err_proc:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK,              kPacket.m_nOK, 0 );      // 성공
        ERR_CASE( ERR_NUSER_VERIFY_00, kPacket.m_nOK, 1 );      // Protocol Version이 틀림
        ERR_CASE( ERR_NUSER_VERIFY_05, kPacket.m_nOK, 2 );      // 아직 행동이 정의되지 않은 인증방식
        ERR_CASE( ERR_NUSER_VERIFY_06, kPacket.m_nOK, 3 );      // 인증 실패
        ERR_CASE( ERR_NUSER_VERIFY_01, kPacket.m_nOK, 4 );      // rename 실패
        ERR_CASE( ERR_NUSER_VERIFY_02, kPacket.m_nOK, 5 );      // 이중접속 감지
        ERR_CASE( ERR_NUSER_VERIFY_03, kPacket.m_nOK, 6 );      // Center Gash 에서 Login 을 얻을수 없다
        ERR_CASE( ERR_NUSER_VERIFY_07, kPacket.m_nOK, 7 );      // ID or Password에 이상한 문자들어가있음
        ERR_CASE( ERR_NUSER_VERIFY_08, kPacket.m_nOK, 8 );      // 요청한 유저와 반환 유저가 다름_Gash_
        ERR_CASE( ERR_NUSER_VERIFY_09, kPacket.m_nOK, 9 );      // Login 길이가 0 이다
        ERR_CASE( ERR_NUSER_VERIFY_10, kPacket.m_nOK, 10 );     // 틀린 Password
        ERR_CASE( ERR_NUSER_VERIFY_11, kPacket.m_nOK, 11 );     // 존재하지 않는 계정
        ERR_CASE( ERR_NUSER_VERIFY_12, kPacket.m_nOK, 12 );     // 신규 유저 등록 실패
        ERR_CASE( ERR_NUSER_VERIFY_13, kPacket.m_nOK, 13 );     // 연령 제한
        ERR_CASE( ERR_NUSER_VERIFY_14, kPacket.m_nOK, 14 );     // 접속 아이피 제한 
        ERR_CASE( ERR_NUSER_VERIFY_15, kPacket.m_nOK, 15 );     // 스크립트 체크섬 오류. 
        ERR_CASE( ERR_NUSER_VERIFY_16, kPacket.m_nOK, 16 );     // 패치시간 중엔 접속할 수 없음.
        ERR_CASE( ERR_NUSER_VERIFY_17, kPacket.m_nOK, 17 );     // 등록되지 않은 Admin IP.
        ERR_CASE( ERR_NUSER_VERIFY_18, kPacket.m_nOK, 18 );     // 빈펀, 게쉬 OTP 오류
        ERR_CASE( ERR_NUSER_VERIFY_19, kPacket.m_nOK, 19 );     // 빈펀 전환 유저가 기존방식으로 접속함.
        ERR_CASE( ERR_NUSER_VERIFY_20, kPacket.m_nOK, 20 );     // 미국 웹 인증 실패 .
        ERR_CASE( ERR_NUSER_VERIFY_21, kPacket.m_nOK, 21 );     // 블럭 된 유저.
        ERR_CASE( ERR_NUSER_VERIFY_22, kPacket.m_nOK, 22 );     // 알수 없는 PostFix.
        ERR_CASE( ERR_NUSER_VERIFY_23, kPacket.m_nOK, 23 );     // 중복된 이메일.
        ERR_CASE( ERR_NUSER_VERIFY_24, kPacket.m_nOK, 24 );     // 유저 정보 없음.
        ERR_CASE( ERR_NUSER_VERIFY_25, kPacket.m_nOK, 25 );     // 남미 인증서버 오류.
        ERR_CASE( ERR_NUSER_VERIFY_26, kPacket.m_nOK, 26 );     // 남미 인증 실패.
        ERR_CASE( ERR_NUSER_VERIFY_27, kPacket.m_nOK, 27 );     // 알수 없는 WebService.
        ERR_CASE( ERR_NUSER_VERIFY_28, kPacket.m_nOK, 28 );     // 블럭된 유저정보.

    default:
        kPacket.m_nOK = 4; break; //알수 없는 Error
    }

    SEND_PACKET( ENU_VERIFY_ACCOUNT_ACK );

    START_LOG( cerr, L"ENU_VERIFY_ACCOUNT_REQ" )
        << BUILD_LOG( NetError::GetLastNetError() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strLogin ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) )
        << BUILD_LOG( kPacket_.m_strIP )
        << BUILD_LOG( kPacket_.m_nProtocolVer )
        << BUILD_LOG( kPacket_.m_strServiceTicket )
        << BUILD_LOG( kPacket_.m_dwAuthType )
        << BUILD_LOG( kPacket_.m_bMale )
        << BUILD_LOG( kPacket_.m_iAge )
        << BUILD_LOG( kPacket_.m_uiSessionKeyHigh )
        << BUILD_LOG( kPacket_.m_uiSessionKeyLow )
        << END_LOG;
}

IMPL_ON_FUNC( ENU_VERIFY_ACCOUNT_ACK )
{
    DWORD dwIP = ::ntohl( GetIP() );
    KNUserPtr spUser;
    spUser.reset();

    // DB인증 성공유무에 관한 에러값을 넣어서 전달.
    NetError::SetLastNetError( kPacket_.m_nOK );

    if ( SiKCenterSimLayer()->GetUseBlockWrongPassword() && NetError::GetLastNetError() == NetError::ERR_NUSER_VERIFY_10 ) { // 틀린 Password
        ++m_nWrongpasswordInputTime;
        if ( IsExceedWrongpasswordTime() == true ) {
            SET_ERR_GOTO( ERR_NUSER_VERIFY_30, end_proc );
        }
    }

    if ( !IS_CORRECT( NET_OK ) && 
         !IS_CORRECT( ERR_NUSER_VERIFY_37 ) &&
         !IS_CORRECT( ERR_NUSER_VERIFY_02 ) ) {
        START_LOG( cerr, L"DB Update Failed ")
            << BUILD_LOG( kPacket_.m_nOK ) << END_LOG;
        goto end_proc;
    }

    // #NATION_CODE
    SiKBlockIPList()->CheckUserCountry( dwIP, kPacket_.m_strNationCode ); 
    START_LOG( clog , L" UserCounty : " << KncUtil::toWideString( kPacket_.m_strNationCode ) ) << END_LOG;
    if( !SiKBlockIPList()->CheckBlockCountry( kPacket_.m_strNationCode ) ) {
        START_LOG( cerr , L" Block Check Error Login : " << kPacket_.m_strLogin )
            << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strNationCode ) )
            << BUILD_LOG( dwIP ) << END_LOG;
        SET_ERR_GOTO( ERR_NUSER_VERIFY_14, end_proc );
    }

    // 패치 타임일 때 허용 유저 ID인지 체크.
    if( !SiKBlockIPList()->CheckExceptUserID( kPacket_.m_strLogin ) ) {
        START_LOG( cerr , L" Block Login : " << kPacket_.m_strLogin )
            << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strNationCode ) )
            << BUILD_LOG( dwIP )
            << END_LOG;
        SET_ERR_GOTO( ERR_NUSER_VERIFY_14, end_proc );
    }

    // 셧다운제 체크
    bool bResult = SiKCnShutdownUser()->CheckAge( kPacket_.m_iAge, dwIP );
    if ( false == bResult ) {
        START_LOG( cerr, L"Shutdown Target User.. Login : " << kPacket_.m_strLogin << L", Age : " << kPacket_.m_iAge ) << END_LOG;
        SET_ERR_GOTO( ERR_NUSER_VERIFY_29, end_proc );
    }

    // 이중 접속 확인 - NUser 와의 중복여부
    // 이시점에서 이중접속 확인이되면 자신은 자동으로 끊어내고, 상대방을 강제종료해야한다.
    if ( NetError::GetLastNetError() == NetError::ERR_NUSER_VERIFY_02 ) {
        // 중국 CYOU 인증용 중복 유저 로그아웃 처리
        //CheckDuplicationUserForCYOU( kPacket_.m_strLogin, kPacket_.m_dwUID );
        SET_ERR_GOTO( ERR_NUSER_VERIFY_02, end_proc );
    }
    else {
        // 일반적인 중복 유저 로그아웃 처리
        if ( true == CheckDuplicationUser( kPacket_.m_strLogin, kPacket_.m_dwUID ) ) {
            SET_ERR_GOTO( ERR_NUSER_VERIFY_02, end_proc );
        }
    }

    if( !SiKCenterSimLayer()->m_kActorManager2.Rename( GetName(), kPacket_.m_strLogin ) )
    {
        SET_ERR_GOTO( ERR_NUSER_VERIFY_01, end_proc );
    }
    SetUID( kPacket_.m_dwUID );
    LIF( SiKCenterSimLayer()->m_kActorManager2.RegByUID( *this ) );

    SetTick( KNUser::AUTHEN_TICK );//인증 받은 시각
    StateTransition( KDefaultFSM::INPUT_VERIFICATION_OK );
    SetAuthLevel( kPacket_.m_cAuthLevel );
    UpdateServerList();
    kPacket_.m_strMarkServerURL = SiKCnGuildManager()->GetMarkURL();
    kPacket_.m_bUseLoadCheckInServer = SiKCenterSimLayer()->GetUseLoadCheckInServer();
    kPacket_.m_fFakeCCU = SiKCenterSimLayer()->GetFakeCCU();
    SiKCenterServer()->SetUniqueUserStat( kPacket_.m_dwChannellingType, kPacket_.m_dwUID ); // 유닉유저 통계.

    m_strBillingAccount = kPacket_.m_strBillingAccount;

end_proc:
    switch( NetError::GetLastNetError() )
    {
    case NetError::NET_OK:              kPacket_.m_nOK = 0; break;      // 성공
    case NetError::ERR_NUSER_VERIFY_00: kPacket_.m_nOK = 1; break;      // Protocol Version이 틀림
    case NetError::ERR_NUSER_VERIFY_05: kPacket_.m_nOK = 2; break;      // 아직 행동이 정의되지 않은 인증방식
    case NetError::ERR_NUSER_VERIFY_06: kPacket_.m_nOK = 3; break;      // 인증 실패
    case NetError::ERR_NUSER_VERIFY_01: kPacket_.m_nOK = 4; break;      // rename 실패
    case NetError::ERR_NUSER_VERIFY_02: kPacket_.m_nOK = 5; break;      // 이중접속 감지
    case NetError::ERR_NUSER_VERIFY_03: kPacket_.m_nOK = 6; break;      // Center Gash 에서 Login 을 얻을수 없다
    case NetError::ERR_NUSER_VERIFY_07: kPacket_.m_nOK = 7; break;      // ID or Password에 이상한 문자들어가있음
    case NetError::ERR_NUSER_VERIFY_08: kPacket_.m_nOK = 8; break;      // 요청한 유저와 반환 유저가 다름_Gash_
    case NetError::ERR_NUSER_VERIFY_09: kPacket_.m_nOK = 9; break;      // Login 길이가 0 이다
    case NetError::ERR_NUSER_VERIFY_10: kPacket_.m_nOK = 10; break;     // 틀린 Password
    case NetError::ERR_NUSER_VERIFY_11: kPacket_.m_nOK = 11; break;     // 존재하지 않는 계정
    case NetError::ERR_NUSER_VERIFY_12: kPacket_.m_nOK = 12; break;     // 신규 유저 등록 실패
    case NetError::ERR_NUSER_VERIFY_13: kPacket_.m_nOK = 13; break;     // 연령 제한
    case NetError::ERR_NUSER_VERIFY_14: kPacket_.m_nOK = 14; break;     // 접속 아이피 제한 
    case NetError::ERR_NUSER_VERIFY_15: kPacket_.m_nOK = 15; break;     // 스크립트 체크섬 오류.
    case NetError::ERR_NUSER_VERIFY_16: kPacket_.m_nOK = 16; break;     // 패치시간 중엔 접속할 수 없음.
    case NetError::ERR_NUSER_VERIFY_17: kPacket_.m_nOK = 17; break;     // 등록되지 않은 Admin IP.
    case NetError::ERR_NUSER_VERIFY_18: kPacket_.m_nOK = 18; break;     // 빈펀, 게쉬 OTP 오류
    case NetError::ERR_NUSER_VERIFY_19: kPacket_.m_nOK = 19; break;     // 빈펀 전환 유저가 기존방식으로 접속함.
    case NetError::ERR_NUSER_VERIFY_20: kPacket_.m_nOK = 20; break;     // 미국 웹 인증 실패.
    case NetError::ERR_NUSER_VERIFY_21: kPacket_.m_nOK = 21; break;     // 블럭 된 유저
    case NetError::ERR_NUSER_VERIFY_22: kPacket_.m_nOK = 22; break;     // 알수 없는 PostFix.
    case NetError::ERR_NUSER_VERIFY_23: kPacket_.m_nOK = 23; break;     // 중복 Email ID.
    case NetError::ERR_NUSER_VERIFY_24: kPacket_.m_nOK = 24; break;     // 유저 정보 없음.
    case NetError::ERR_NUSER_VERIFY_25: kPacket_.m_nOK = 25; break;     // 남미 인증서버 오류.
    case NetError::ERR_NUSER_VERIFY_26: kPacket_.m_nOK = 26; break;     // 남미 인증 실패.
    case NetError::ERR_NUSER_VERIFY_27: kPacket_.m_nOK = 27; break;     // 알수 없는 WebService.
    case NetError::ERR_NUSER_VERIFY_28: kPacket_.m_nOK = 28; break;     // 블럭된 유저정보.
    case NetError::ERR_NUSER_VERIFY_29: kPacket_.m_nOK = 29; break;     // 셧다운제 대상유저
    case NetError::ERR_NUSER_VERIFY_30: kPacket_.m_nOK = 30; break;     // 패스워드 틀린 횟수 기준치 초과
    case NetError::ERR_NUSER_VERIFY_31: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_31; break; // 국가나 지역이 지원되지 않음
    case NetError::ERR_NUSER_VERIFY_32: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_32; break; // 중국 창유 인증 실패
    case NetError::ERR_NUSER_VERIFY_34: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_34; break; // 중국 창유 비활성화 계정
    case NetError::ERR_NUSER_VERIFY_35: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_35; break; // 중국 창유 계정 실명 미인증
    case NetError::ERR_NUSER_VERIFY_36: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_36; break; // 중국 창유 인증패킷 조립 실패
    case NetError::ERR_NUSER_VERIFY_37: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_37; break; // 중국 창유 유저 동의 안함.
    case NetError::ERR_NUSER_VERIFY_38: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_38; break; // 중국 창유 인증 에러. 위에 없는 종류.
    case NetError::ERR_NUSER_VERIFY_39: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_39; break; // 중국 창유 미가입 계정
    case NetError::ERR_NUSER_VERIFY_40: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_40; break; // 중국 창유 고정 IP 아닌 에러
    case NetError::ERR_NUSER_VERIFY_41: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_41; break; // 중국 창유 휴대폰 고정 계정
    case NetError::ERR_NUSER_VERIFY_42: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_42; break; // 중국 보안카드 미신청
    case NetError::ERR_NUSER_VERIFY_43: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_43; break; // 중국 보안카드 정보 입력하고 다시 로그인
    case NetError::ERR_NUSER_VERIFY_44: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_44; break; // 중국 보안카드 비밀번호 틀림
    case NetError::ERR_NUSER_VERIFY_45: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_45; break; // 중국 Mac값 검사 실패. 비고정 Mac 경로
    case NetError::ERR_NUSER_VERIFY_46: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_46; break; // 중국 같은 신분증으로 다른 계정이  이미 로그인 된 상태
    case NetError::ERR_NUSER_VERIFY_47: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_47; break; //
    case NetError::ERR_NUSER_VERIFY_48: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_48; break; //
    case NetError::ERR_NUSER_VERIFY_49: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_49; break; //
    case NetError::ERR_NUSER_VERIFY_50: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_50; break; //
    case NetError::ERR_NUSER_VERIFY_51: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_51; break; //
    case NetError::ERR_NUSER_VERIFY_52: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_52; break; //
    case NetError::ERR_NUSER_VERIFY_53: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_53; break; //
    case NetError::ERR_NUSER_VERIFY_54: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_54; break; //
    case NetError::ERR_NUSER_VERIFY_55: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_55; break; //
    case NetError::ERR_NUSER_VERIFY_63: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_63; break; //
    case NetError::ERR_NUSER_VERIFY_64: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_64; break; //
    case NetError::ERR_NUSER_VERIFY_65: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_65; break; //
    case NetError::ERR_NUSER_VERIFY_66: kPacket_.m_nOK = NetError::ERR_NUSER_VERIFY_66; break; //

    default:
        kPacket_.m_nOK = -99;
        START_LOG( cerr, L"ENU_VERIFY_ACCOUNT_ACK. 알수 없는 오류 , Name : " << kPacket_.m_strLogin )
            << BUILD_LOG( NetError::GetLastNetError() )
            << BUILD_LOG( NetError::GetLastNetErrMsg() ) << END_LOG;
    }

    //접속 성공하면 카운트를 가지고 있는다.
    if( IS_CORRECT( NET_OK ) )
    {
        SiKCenterSimLayer()->AddConnectionCount();
        UpdateChannelNews();
        SendItemInfo();
        SendNewContentsInfo();
        SendSocketTableNot();
        SendCashbackRatioInfoNot();
    }

    if ( SiKCenterSimLayer()->m_nAccOnlineTimeTest != 0 ) {
        kPacket_.m_nAccOnlineTime = SiKCenterSimLayer()->m_nAccOnlineTimeTest;
    }

    SEND_COMPRESS_PACKET( ENU_VERIFY_ACCOUNT_ACK, kPacket_ );

    if( IS_CORRECT( ERR_NUSER_VERIFY_15 ) )
    {
        SiKCenterSimLayer()->AddDisconnectReson( DRW_SCRIPT_CHKSUM );
    }

    if ( IS_CORRECT( ERR_NUSER_VERIFY_30 ) ) {
        ReserveDestroy();
    }

    _LOG_SUCCESS( kPacket_.m_nOK == 0 || kPacket_.m_nOK == 10 || kPacket_.m_nOK == NetError::ERR_NUSER_VERIFY_37 , L" MSG : " << NetError::GetLastNetErrMsg() << L", Login : " << kPacket_.m_strLogin )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_iAge )
        << BUILD_LOG( kPacket_.m_strLogin )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strNMPasswd ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strBillingAccount ) )
        << BUILD_LOG( kPacket_.m_bExpAccount )
        << BUILD_LOG( kPacket_.m_bBlockIPPass)
        << BUILD_LOG( kPacket_.m_kGuildInfo.m_dwUID )
        << BUILD_LOG( kPacket_.m_kGuildInfo.m_strName )
        << BUILD_LOG( kPacket_.m_kGuildInfo.m_nNumMembers )
        << BUILD_LOG( kPacket_.m_kGuildUserInfo.m_dwGuildUID )
        << BUILD_LOGc( kPacket_.m_kGuildUserInfo.m_cMemberLevel )
        << BUILD_LOG( kPacket_.m_strMarkServerURL )
        << BUILD_LOG( kPacket_.m_biUniqueKey )
        << BUILD_LOG( kPacket_.m_nAccOnlineTime ) // 중국 CYOU 전체 게임 온라인 누적 시간.몰입방지용
        << BUILD_LOGc( kPacket_.m_cFatigueState )
        << END_LOG;
}

IMPL_ON_FUNC( EVENT_CLOSE_CONNECTION_NOT )
{
    //nuser테이블에서 제거하고,Nuser도 강제종료 시킨다.

    KEVENT_CLOSE_CONNECTION_NOT kNot;
    kNot.m_nCloseType   = KEVENT_CLOSE_CONNECTION_NOT::CCT_DUPLICATION;
    kNot.m_strMessage   = SiKCenterServer()->m_stringTable.GetValue( L"str_02" );
    SEND_RECEIVED_PACKET( EVENT_CLOSE_CONNECTION_NOT );
    ReserveDestroy();

    START_LOG( cerr, kNot.m_strMessage )
        << BUILD_LOG( m_strName ) << END_LOG;
}

_IMPL_ON_FUNC( ENU_OVERLAP_FILE_INFO, std::vector<KOverlapFile> )
{
    // 로그 파일에 기록하기.
    if( SiKCenterSimLayer()->GetCheckOverlapped() ) {

        std::wstringstream stm;
        stm << L"|UserID|" << m_strName                           // UserID.
            << L"|UserIP|" << KncUtil::toWideString( GetIPStr() ); // UserIP.

        std::vector<KOverlapFile>::iterator vit;
        for( vit = kPacket_.begin(); vit != kPacket_.end(); ++vit ) {
            stm << L"|PreviousFile|" << KncUtil::toWideString( vit->m_strPreviousFile )        // 이전파일.
                << L"|CurrentFile|" << KncUtil::toWideString( vit->m_strCurrentFile )          // 현재파일.
                << L"|OverlapFile|" << KncUtil::toWideString( vit->m_strOverlapFile );         // 중복파일.
        }
        stm << L"|Time|" << KNC_TIME_STRING;

        SiKCenterLogManager()->Get( KCenterLogManager::LT_OVERLAPPED_FILE ) << stm.str() << GLOG::endl;
        stm.clear();        // STL 권고사항으로 해당 버퍼의 clear()를 호출.
        stm.str(L"");       // 사용한 wstringstream 변수의 버퍼를 비우자.

    START_LOG( clog, L"클라이언트 중복파일 발견" ) << END_LOG;
    }
    // 중복파일로 로그인 실패한 유저에 대해서 통계를 남김.
    SiKCenterSimLayer()->AddDisconnectReson( DRW_DUPLICATE_FILE );
}

IMPL_ON_FUNC_NOPARAM( ENU_GUIDE_BOOK_LIST_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    QUEUEING_CN_ID_TO_DB( ECN_GUIDE_BOOK_LIST_REQ );
}

_IMPL_ON_FUNC( ENU_GUIDE_BOOK_LIST_ACK, MAP_SHORT_SHORT )
{
    SEND_COMPRESS_PACKET( ENU_GUIDE_BOOK_LIST_ACK, kPacket_ );
    START_LOG( clog, L"가이드북 정보 보냄.. Name : " << m_strName << L", Size : " << kPacket_.size() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( ENU_TEXTURE_DYNAMIC_LOAD_REQ )
{
    bool kPacket = SiKCenterSimLayer()->m_bDynamicResourceLoadEnable;
    SEND_PACKET( ENU_TEXTURE_DYNAMIC_LOAD_ACK );
}

IMPL_ON_FUNC_NOPARAM( ENU_LOADING_IMAGE_REQ )
{
    KENU_LOADING_IMAGE_ACK kPacket;
    SiKCnClientContentsManager()->GetChannelNewsImageNumber( kPacket.m_vecNewsNumber );
    SiKCnClientContentsManager()->GetLoadingImage( kPacket.m_vecLoadingImageName );
    SiKCnClientContentsManager()->GetPVPLoadingImage( kPacket.m_mapPVPLoadingImageName );
    SiKCnClientContentsManager()->GetClientScriptName( kPacket.m_mapScriptName );
    SiKCnClientContentsManager()->GetExceptionMotionID( kPacket.m_vecExceptionMotionID );
    SiKCnClientContentsManager()->GetEventBannerInfo( kPacket.m_vecEventBannerInfo );
    SiKCnClientContentsManager()->GetCharSkillInfo( kPacket.m_setCharSkillEnable );
    SiKCnClientContentsManager()->GetDLLBlackList( kPacket.m_setDLLBlackList );
    SiKCnClientContentsManager()->GetChampionRatio( kPacket.m_mapChampionRatio );
    SiKCnClientContentsManager()->GetEventOpenInfo( kPacket.m_vecEventOpenInfo );
    SiKCnClientContentsManager()->GetSmartPanelInfo( kPacket.m_bSmartPanelMailListEnable, kPacket.m_vecSmartPanelMailList );

    SEND_PACKET( ENU_LOADING_IMAGE_ACK );

    START_LOG( clog, L"로딩이미지, 채널 뉴스, 클라이언트 스크립트 이름 전송" )
        << BUILD_LOG( kPacket.m_vecNewsNumber.size() )
        << BUILD_LOG( kPacket.m_vecLoadingImageName[0] )
        << BUILD_LOG( kPacket.m_vecLoadingImageName[1] )
        << BUILD_LOG( kPacket.m_vecLoadingImageName[2] )
        << BUILD_LOG( kPacket.m_mapScriptName.size() )
        << BUILD_LOG( kPacket.m_vecExceptionMotionID.size() )
        << BUILD_LOG( kPacket.m_vecEventBannerInfo.size() )
        << BUILD_LOG( kPacket.m_setCharSkillEnable.size() )
        << BUILD_LOG( kPacket.m_setDLLBlackList.size() )
        << BUILD_LOG( kPacket.m_mapChampionRatio.size() )
        << BUILD_LOG( kPacket.m_vecEventOpenInfo.size() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( ENU_CLIENT_CONTENTS_FIRST_INIT_INFO_REQ )
{
    KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT kPacket;
    kPacket.m_nSendType = KEVENT_CLIENT_CONTENTS_FIRST_INIT_INFO_NOT::ECCFII_CONNECTED;

    for ( int i = 0 ; i < KCCUsingFilesInfo::EV_MAX ; i++ )
        kPacket.m_prUsingFilesInfo.first.push_back( i );
    for ( int i = 0 ; i < KCCHackCheckInfo::EV_MAX ; i++ )
        kPacket.m_prHackCheckInfo.first.push_back( i );

    KCnClientContentsInfo kUsingICC;
    SiKCnUpdatePlanManager()->GetUsingICC( kUsingICC );

    kUsingICC.GetChannelNewsImageNumber( kPacket.m_prUsingFilesInfo.second.m_vecNewsNumber );
    kUsingICC.GetLoadingImage( kPacket.m_prUsingFilesInfo.second.m_mapLoadingImageName );
    kUsingICC.GetPVPLoadingImage( kPacket.m_prUsingFilesInfo.second.m_mapPVPLoadingImageName );
    kUsingICC.GetClientScriptName( kPacket.m_prUsingFilesInfo.second.m_mapScriptName );
    kUsingICC.GetEventBannerInfo( kPacket.m_prUsingFilesInfo.second.m_vecEventBannerInfo );

    kUsingICC.GetExceptionMotionID( kPacket.m_prHackCheckInfo.second.m_vecExceptionMotionID );
    kUsingICC.GetDLLBlackList( kPacket.m_prHackCheckInfo.second.m_setDLLBlackList );

    SEND_COMPRESS_PACKET( ENU_CLIENT_CONTENTS_FIRST_INIT_INFO_ACK, kPacket );

    START_LOG( clog, L"로딩이미지, 채널 뉴스, 클라이언트 스크립트 이름 전송" )
        << BUILD_LOG( kPacket.m_nSendType )
        << BUILD_LOG( kPacket.m_prUsingFilesInfo.second.m_vecNewsNumber.size() )
        << BUILD_LOG( kPacket.m_prUsingFilesInfo.second.m_mapLoadingImageName.size() )
        << BUILD_LOG( kPacket.m_prUsingFilesInfo.second.m_mapScriptName.size() )
        << BUILD_LOG( kPacket.m_prHackCheckInfo.second.m_vecExceptionMotionID.size() )
        //<< BUILD_LOG( kPacket.m_bSmartPanelMailListEnable )
        //<< BUILD_LOG( kPacket.m_vecSmartPanelMailList.size() )
        << BUILD_LOG( kPacket.m_prHackCheckInfo.second.m_setDLLBlackList.size() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( ENU_CLIENT_PING_CONFIG_REQ )
{
    KPingConfig kPacket;
    SiKCenterSimLayer()->GetPingConfig( kPacket );
    SEND_PACKET( ENU_CLIENT_PING_CONFIG_ACK );
    START_LOG( clog, L"Check Gap : " << kPacket.m_dwPingCheckGap
        << L", Limit : " << kPacket.m_dwPingLimitValue
        << L", Repetition : " << kPacket.m_nRepetitionCount )  << END_LOG;
}
// 클라이언트에게 SHA File Name List 전송.
IMPL_ON_FUNC_NOPARAM( ENU_SHAFILENAME_LIST_REQ )
{
    KENU_SHAFILENAME_LIST_ACK kPacket;
    SiKCenterServer()->GetSHAFileNameList( kPacket );
    SEND_PACKET( ENU_SHAFILENAME_LIST_ACK );
    START_LOG( clog, L"SHA FileName List Size : " << kPacket.size() ) << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( ENU_USER_AGREEMENT_REQ )
{
    KCYOUAuthUserInfo kData;
    kData.m_strLogin = m_strBillingAccount;
    kData.m_dwUID = GetUID();
    kData.m_strIP = GetIPStr();
    //kData.m_strServerIP = NetCommon::GetLocalIP();
    SiKCenterSimLayer()->GetBillConnIP( kData.m_strServerIP );

    //KTThreadManager<KCnCYOUAuth>::GetInstance()->QueueingEvent( KNUserEvent::ENU_USER_AGREEMENT_REQ, GetName().c_str() , GetUID(), 0, kData );

    START_LOG( clog, L"중국 유저 동의" )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( KncUtil::toWideString( GetIPStr() ) )
        << BUILD_LOG( KncUtil::toWideString( kData.m_strIP ) )
        << END_LOG;
}

IMPL_ON_FUNC_NOPARAM( ENU_CLIENT_CONTENTS_FIRST_INIT_ADDITIONAL_INFO_REQ )
{
    KENU_CLIENT_CONTENTS_FIRST_INIT_ADDITIONAL_INFO_ACK kPacket;

    KCnClientContentsInfo kUsingICC;
    SiKCnUpdatePlanManager()->GetUsingICC( kUsingICC );

    kUsingICC.GetSmartPanelInfo( kPacket.m_bSmartPanelMailListEnable, kPacket.m_vecSmartPanelMailList );
    kUsingICC.GetChannelImageDownloadURLList( kPacket.m_mapChannelImageDownloadURLList );

    SEND_COMPRESS_PACKET( ENU_CLIENT_CONTENTS_FIRST_INIT_ADDITIONAL_INFO_ACK, kPacket );

    START_LOG( clog, L"스마트 패널 메일 도메인, 채널 이미지 다운로드 URL 리스트 전송" )
        << BUILD_LOG( kPacket.m_bSmartPanelMailListEnable )
        << BUILD_LOG( kPacket.m_vecSmartPanelMailList.size() )
        << BUILD_LOG( kPacket.m_mapChannelImageDownloadURLList.size() )
        << END_LOG;
}

void KNUser::OnDestroy()
{
    KSession::OnDestroy();

    AddLogoutStat();

    switch( m_pkCurrentState->GetID() )
    {
    case KDefaultFSM::STATE_EXIT:
        return;
    case KDefaultFSM::STATE_INIT:
    case KDefaultFSM::STATE_CONNECTED:
    case KDefaultFSM::STATE_LOGINED:
        StateTransition( KDefaultFSM::INPUT_EXIT_GAME );    // 상태를 Exit로 변경하여 Tick에서 삭제 리스트에 추가한다.
        return;
    }// end switch

    // 만약을 위한 처리일뿐 있으면 안되는 상황.
    START_LOG( cerr, L"종료처리 - 있으면 안되는 상태임" )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( GetStateIDString() )
        << END_LOG;

    SiKCenterSimLayer()->m_kActorManager2.ReserveDelete( m_strName );
}
bool KNUser::CheckValidate( std::string& strData_ )
{
    char chNum[2]   = {'0','9'};
    char loAlpha[2] = {'a','z'};
    char upAlpha[2] = {'A','Z'};
    char underBar[1] = { '_' };

    std::string::iterator sit;

    for( sit = strData_.begin() ; sit != strData_.end() ; ++sit )
    {
        if( !(  ( chNum[0] <= *sit && chNum[1] >= *sit ) ||
            ( loAlpha[0] <= *sit && loAlpha[1] >= *sit ) ||
            ( upAlpha[0] <= *sit && upAlpha[1] >= *sit ) ||
            ( underBar[0] == *sit ) )
            )
        {
            return false;
        }
    }
    return true;
}

void KNUser::UpdateChannelNews()
{
    KENU_CHANNEL_NEWS_NOT kChannelNews;
    SiKCenterServer()->GetChannelNews( kChannelNews );

    SendPacket( KNUserEvent::ENU_CHANNEL_NEWS_NOT, kChannelNews, false );
    SetTick( KNUser::SEND_CHANNEL_NEWS );
}

bool KNUser::CheckNMCPTS( std::string strCPTS_ )
{
    if( strCPTS_.empty() ) return false;
    CTime tmCurrent = CTime::GetCurrentTime();

    std::string strTime;
    std::string strMin;
    CTime tmOriginCpts;
    CTime tmReconcileCpts;
    {
        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep("|");
        tokenizer tokens( strCPTS_, sep);
        tokenizer::iterator tok_iter = tokens.begin();
        if( tok_iter == tokens.end() ) return false;
        strTime = *tok_iter;
        ++tok_iter;
        if( tok_iter == tokens.end() ) return false;
        strMin = *tok_iter;
    }

    {
        //std::cout << TOSTRINGA( strTime );
        if( strTime.length() != 14 ) return false;
        int nSec    = atoi( strTime.substr( 0, 2 ).c_str() );
        int nMin    = atoi( strTime.substr( 2, 2 ).c_str() );
        int nhour   = atoi( strTime.substr( 4, 2 ).c_str() );
        int nDay    = atoi( strTime.substr( 6, 2 ).c_str() );
        int nMonth  = atoi( strTime.substr( 8, 2 ).c_str() );
        int nYear   = atoi( strTime.substr( 10, 4 ).c_str() );
        tmOriginCpts = CTime( nYear, nMonth, nDay, nhour, nMin, nSec );
        //std::wcout << TOSTRINGWt( tmOriginCpts );
        int nOTPMin = atoi( strMin.c_str() );
        tmReconcileCpts = tmOriginCpts + CTimeSpan( 0, 0, nOTPMin, 0 );
        //std::wcout << TOSTRINGWt( tmReconcileCpts );
    }
    //std::wcout << TOSTRINGWt( tmCurrent );
    //std::wcout << TOSTRINGWb( (bool)(tmCurrent <= tmReconcileCpts && tmCurrent >= tmOriginCpts) );
    return (tmCurrent <= tmReconcileCpts && tmCurrent >= tmOriginCpts);
}

void KNUser::SendItemInfo()
{
    KItemBuyInfo kPacket;
    if( !SiKCnItemManager()->GetBuyItemInfo( kPacket.m_bBuyEnable, kPacket.m_vecItemRange ) ) return;
    SiKCnItemManager()->GetItemRecommInfo( kPacket.m_mapRecommInfo );

    
    START_LOG( cwarn, L"접속시 Item 구매 가능 정보 전달.."
        << L" BuyEnable : " << (kPacket.m_bBuyEnable?L"True":L"False")
        << L", Size : " << kPacket.m_vecItemRange.size() ) << END_LOG;
    SEND_COMPRESS_PACKET( ENU_ITEM_BUY_INFO_NOT, kPacket );
}

void KNUser::SendContentsInfo() // 미사용. 아래의 SendNewContentsInfo 사용하도록 하고 안정화되면 요거는 삭제할 예정
{
    KENU_CLIENT_CONTENTS_OPEN_NOT kPacket;
    kPacket.m_nMaxLevel = SiKCnClientContentsManager()->GetMaxLevel();
    
    SiKCnClientContentsManager()->GetEventMonsterAbility( kPacket.m_mapMonsterAbilityList );

    if( SiKCnClientContentsManager()->GetContentsInfo( kPacket.m_vecChannelCategories, kPacket.m_vecCategoryModes,
                                                        kPacket.m_vecCategoryInfo, kPacket.m_mapCharactes, kPacket.m_vecCharSP4OpenInfo,
                                                        kPacket.m_mapCharOpenType, kPacket.m_mapCharCashGoods, kPacket.m_vecDisableMenuList,
                                                        kPacket.m_SlotItem ) ) {
        SEND_COMPRESS_PACKET( ENU_CLIENT_CONTENTS_OPEN_NOT, kPacket );
    }
    else {
        START_LOG( cwarn, L"클라이언트 컨텐츠 오픈 정보가 비어 있음.." ) << END_LOG;
    }
}

void KNUser::SendNewContentsInfo()
{
    KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT kPacket;
    kPacket.m_nSendType = KEVENT_CLIENT_CONTENTS_OPEN_INFO_NOT::ECCOI_CONNECTED;

    for ( int i = 0 ; i < KCCGameCategoryInfo::EV_MAX ; i++ )
        kPacket.m_prGameCategoryInfo.first.push_back( i );
    for ( int i = 0 ; i < KCCGameCharInfo::EV_MAX ; i++ )
        kPacket.m_prGameCharInfo.first.push_back( i );
    for ( int i = 0 ; i < KCCMonsterInfo::EV_MAX ; i++ )
        kPacket.m_prMonsterInfo.first.push_back( i );
    for ( int i = 0 ; i < KCCMiniEventnInfo::EV_MAX ; i++ )
        kPacket.m_prMiniEventInfo.first.clear();

    KCnClientContentsInfo kUsingICC;
    SiKCnUpdatePlanManager()->GetUsingICC( kUsingICC );

    kPacket.m_prGameCharInfo.second.m_nMaxLevel = kUsingICC.GetMaxLevel();
    kUsingICC.GetEventMonsterAbility( kPacket.m_prMonsterInfo.second.m_mapMonsterAbility );
    kUsingICC.GetChampionRatio( kPacket.m_prMonsterInfo.second.m_mapChampionRatio );
    kUsingICC.GetCharSkillInfo( kPacket.m_prGameCharInfo.second.m_setCharSkillEnable );
    kUsingICC.GetEventOpenInfo( kPacket.m_prMiniEventInfo.second.m_vecEventOpenInfo );

    if ( kUsingICC.GetContentsInfo( kPacket.m_prGameCategoryInfo.second.m_vecChannelCategories,
                                                        kPacket.m_prGameCategoryInfo.second.m_vecCategoryModes,
                                                        kPacket.m_prGameCategoryInfo.second.m_vecCategoryInfo,
                                                        kPacket.m_prGameCharInfo.second.m_mapCharactes,
                                                        kPacket.m_prGameCharInfo.second.m_vecCharSP4OpenInfo,
                                                        kPacket.m_prGameCharInfo.second.m_mapCharOpenType,
                                                        kPacket.m_prGameCharInfo.second.m_mapCharCashGoods,
                                                        kPacket.m_prMiniEventInfo.second.m_vecDisableMenuList ) ) {
        SEND_COMPRESS_PACKET( ENU_NEW_CLIENT_CONTENTS_OPEN_NOT, kPacket );

        START_LOG( clog, L"접속시 클라이언트에 ICC 일부 정보 전송" )
            << BUILD_LOG( kPacket.m_nSendType )

            << BUILD_LOG( kPacket.m_prGameCategoryInfo.second.m_vecChannelCategories.size() )
            << BUILD_LOG( kPacket.m_prGameCategoryInfo.second.m_vecCategoryModes.size() )
            << BUILD_LOG( kPacket.m_prGameCategoryInfo.second.m_vecCategoryInfo.size() )

            << BUILD_LOG( kPacket.m_prGameCharInfo.second.m_nMaxLevel )
            << BUILD_LOG( kPacket.m_prGameCharInfo.second.m_mapCharactes.size() )
            << BUILD_LOG( kPacket.m_prGameCharInfo.second.m_vecCharSP4OpenInfo.size() )
            << BUILD_LOG( kPacket.m_prGameCharInfo.second.m_mapCharOpenType.size() )
            << BUILD_LOG( kPacket.m_prGameCharInfo.second.m_mapCharCashGoods.size() )
            << BUILD_LOG( kPacket.m_prGameCharInfo.second.m_setCharSkillEnable.size() )

            << BUILD_LOG( kPacket.m_prMonsterInfo.second.m_mapChampionRatio.size() )
            << BUILD_LOG( kPacket.m_prMonsterInfo.second.m_mapMonsterAbility.size() )

            << BUILD_LOG( kPacket.m_prMiniEventInfo.second.m_vecDisableMenuList.size() )
            << BUILD_LOG( kPacket.m_prMiniEventInfo.second.m_vecEventOpenInfo.size() )
            
            << END_LOG;
    }
    else {
        START_LOG( cerr, L"클라이언트 컨텐츠 오픈 정보가 비어 있음.." ) << END_LOG;
    }
}

void KNUser::SendClientScriptInfoNot() // 미사용, 삭제할 예정, 클라에서도 사용하지 않는 것으로 보임
{
    KENU_CLIENT_SCRIPT_INFO_NOT kPacket;
    SiKCnClientScriptManager()->GetMonsterInfo( kPacket.m_mapMonsterInfo );
    //SiKCnClientScriptManager()->GetDamageInfo( kPacket.m_mapDamageInfo );
    SEND_COMPRESS_PACKET( ENU_CLIENT_SCRIPT_INFO_NOT, kPacket );
}

void KNUser::SendSocketTableNot()
{
    KENU_SOCKET_TABLE_INFO_NOT kPacket;
    SiKCnItemManager()->m_kSocketConsume.GetTableInfo( kPacket );
    SEND_COMPRESS_PACKET( ENU_SOCKET_TABLE_INFO_NOT, kPacket );
}

void KNUser::CheckServerListByAuthLevel( IN OUT std::vector< KServerInfo >& vecList_ )
{
    int nCheckAuthLevel = SiKCenterSimLayer()->GetServerListAuthLevel();
    bool IsPassAuthLevel = false;
    int nEnableServerPart = 1; // 일반계정 접근 가능한 서버part

    // 허용계정등급인가?
    if ( GetAuthLevel() >= nCheckAuthLevel ) {
        IsPassAuthLevel = true;
    }

    // 허용계정등급이 접근가능한 서버Part 설정
    if ( IsPassAuthLevel ) {
        nEnableServerPart = -1;
    }

    // 허용계정등급이고, 운영자면 모두 전달
    if ( IsPassAuthLevel && GetAuthLevel() >= 3 ) {
        return;
    }

    // 허용계정등급이 아니면 ServerPart 0 이하의 서버는 보이지 않음.
    std::vector< KServerInfo > vecEnableList;
    vecEnableList.reserve( vecList_.size() );

    std::vector< KServerInfo >::iterator vit;
    for ( vit = vecList_.begin() ; vit != vecList_.end() ; ++vit ) {
        if ( vit->m_iServerPart >= nEnableServerPart ) {
            vecEnableList.push_back( *vit );
        }
    }

    vecList_.swap( vecEnableList );
}

void KNUser::AddLogoutStat()
{
    KCnLoginOutStatPtr spStat = GET_STAT( KCnLoginOutStat, SI_LOGINOUT_STAT );
    if ( spStat ) {
        spStat->AddStat( KCnLoginOutStat::LOGOUT );
    }
}

bool KNUser::IsExceedWrongpasswordTime() // 틀린 패스워드 횟수가 기준 초과인지 체크
{
    bool bRet = false;

    if ( m_nWrongpasswordInputTime >= SiKCenterSimLayer()->GetWrongPasswordLimit() ) {
        bRet = true;
    }
    
    return bRet;
}

void KNUser::SendCashbackRatioInfoNot()
{
    KCashbackRatioInfo kPacket;
    SiKCnItemManager()->GetCashbackRatioInfo( kPacket );
    SEND_COMPRESS_PACKET( ENU_CASHBACK_RATIO_INFO_NOT, kPacket );
}

void KNUser::CheckDuplicationUserForCYOU( IN const std::wstring& wstrLogin_, IN const DWORD dwUID_ )
{
    if ( false == SiKCenterSimLayer()->CheckAuthTypeFlag( KSimLayer::AT_CYOU ) ) {
        return;
    }

    /* 중국 인증 서버에서 중복 로그인 응답을 주는 경우
       게임 서버에 해당 유저가 로그인되어 있다면 게임 서버에 알려서접속 종료를 시키고
       다시 센터 서버에 알려서 센터 서버에 해당 유저가 로그인되어 있다면 접속 종료를 시킨 다음
       중국 인증 서버에 A4(로그아웃) 프로토콜을 전송한다
       이 경우 게임 서버에서의 구분을 위해 Disconnect reason을 다르게 설정한다 */

    // A3를 보낸 유저만 A4를 보내면 되기 때문에 게임 서버에 접속중인지를 확인한다
    bool bIsUserConnected = false;
    if ( SiKCenterSimLayer()->m_kActorManager.ForEach( KIsConnect( dwUID_ ) ) ) {
        KECN_KICK_USER_NOT kNot;
        kNot.m_wstrLogin        = wstrLogin_;
        kNot.m_dwDestUserUID    = dwUID_;
        kNot.m_nReason          = KEVENT_CLOSE_CONNECTION_NOT::CCT_DUPLICATION_CYOU;
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_KICK_USER_NOT, kNot );

        bIsUserConnected = true;
    }

    if ( false == bIsUserConnected ) {
        // 게임 서버에 접속중이지 않기 때문에 캐릭터 정보를 알 수 없으므로 중국 CYOU 인증 서버에 A4 패킷을 바로 보낸다
        KCYOUAuthUserInfo kCnPacket;
        kCnPacket.m_strLogin = KncUtil::toNarrowString( wstrLogin_ );
        kCnPacket.m_cLogoutType = 0x03; // 강제 로그아웃

        SiKCenterSimLayer()->m_kActorManager.QueueingToAll( KCenterEvent::ECN_CYOU_USER_LOGOUT, kCnPacket );
    }
}

bool KNUser::CheckDuplicationUser( IN const std::wstring& wstrLogin_, IN const DWORD dwUID_ )
{
    // @return: true: 중복 로그인 감지됨, false: 중복로그인 아님

    KNUserPtr spUser = KNUserPtr( SiKCenterSimLayer()->m_kActorManager2.GetByName< KNUser >( wstrLogin_ ) );
    if( spUser )  // 현재 서버에 똑같은 이름의 접속 유저가 있다
    {
        KEVENT_CLOSE_CONNECTION_NOT kNot;
        kNot.m_nCloseType   = KEVENT_CLOSE_CONNECTION_NOT::CCT_DUPLICATION;
        kNot.m_strMessage   = SiKCenterServer()->m_stringTable.GetValue( L"str_02" );
        spUser->SendPacket( KNUserEvent::EVENT_CLOSE_CONNECTION_NOT, kNot );
        spUser->ReserveDestroy();

        KECN_DEL_USER_REQ kCnPacket;
        kCnPacket.m_dwUserUID = dwUID_;
        kCnPacket.m_nReason = KECN_DEL_USER_REQ::DR_DUPLICATION_CENTER;
        SiKCenterSimLayer()->m_kActorManager.QueueingToAll( KCenterEvent::ECN_DEL_USER_REQ, kCnPacket );

        return true;
    }

    if( SiKCenterSimLayer()->m_kActorManager.ForEach( KIsConnect( dwUID_ ) ) ) {
        // 센터 서버에는 없지만 게임 서버에 연결중인 유저들이 접속종료 될 수 있도록 게임 서버에 알림
        KECN_KICK_USER_NOT kNot;
        kNot.m_dwDestUserUID    = dwUID_;
        kNot.m_nReason          = KEVENT_CLOSE_CONNECTION_NOT::CCT_DUPLICATION;
        SiKCenterSimLayer()->m_kActorManager.SendToAll( KCenterEvent::ECN_KICK_USER_NOT, kNot );

        KECN_DEL_USER_REQ kCnPacket;
        kCnPacket.m_dwUserUID = dwUID_;
        kCnPacket.m_nReason = KECN_DEL_USER_REQ::DR_DUPLICATION_GAME;
        SiKCenterSimLayer()->m_kActorManager.QueueingToAll( KCenterEvent::ECN_DEL_USER_REQ, kCnPacket );

        return true;
    }

    return false;
}

IMPL_ON_FUNC( ENU_ACCOUNT_REGISTER_REQ )
{
    kPacket_.m_strIP = KncUtil::toWideString( GetIPStr() );
    SET_ERROR( ERR_UNKNOWN );
    KENU_ACCOUNT_REGISTER_ACK kPacket;
    kPacket.m_nOK = NetError::ERR_UNKNOWN;
    START_LOG( clog, L"ENU_ACCOUNT_REGISTER_REQ" )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strLogin ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strEmail ) )
        << BUILD_LOG( kPacket_.m_strIP )
        << BUILD_LOG( kPacket_.m_nProtocolVer )
        << END_LOG;
    if( kPacket_.m_nProtocolVer != SiKCenterServer()->m_nProtocolVer2 )
    {
        START_LOG( cerr, L"Invalid Protocol Version" )
            << L"Wanted : " << SiKCenterServer()->m_nProtocolVer2 << dbg::endl
            << L"Current :" << kPacket_.m_nProtocolVer  << END_LOG;
        ReserveDestroy();
        SET_ERR_GOTO( ERR_NUSER_VERIFY_00, err_proc );
    }
    SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_ENU_ACCOUNT_REGISTER_REQ, m_strName.c_str(), 0, 0, kPacket_ );
    SET_ERROR( NET_OK );
err_proc:
    switch( NetError::GetLastNetError() )
    {
        ERR_CASE( NET_OK, kPacket.m_nOK, 0 );
    default:
        kPacket.m_nOK = -99; break;
    }
    SEND_PACKET( ENU_ACCOUNT_REGISTER_ACK );
    START_LOG( cerr, L"ENU_ACCOUNT_REGISTER_REQ" )
        << BUILD_LOG( NetError::GetLastNetError() )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strLogin ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strEmail ) )
        << BUILD_LOG( kPacket_.m_strIP )
        << BUILD_LOG( kPacket_.m_nProtocolVer )
        << END_LOG;
}

IMPL_ON_FUNC( ENU_ACCOUNT_REGISTER_ACK )
{
    START_LOG( clog, L"ENU_ACCOUNT_REGISTER_ACK. 알수 없는 오류 , iOK : " << kPacket_.m_nOK ) << END_LOG;
    SEND_COMPRESS_PACKET( ENU_ACCOUNT_REGISTER_ACK, kPacket_ );
    _LOG_SUCCESS( kPacket_.m_nOK == 0 , L" MSG : " << NetError::GetLastNetErrMsg() << L", iOK : " << kPacket_.m_nOK ) << END_LOG;
}