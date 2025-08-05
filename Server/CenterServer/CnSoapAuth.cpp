#include "CenterSimLayer.h"
#include "CnSoapAuth.h"
#include "CenterDBThread.h"
#include "soapH.h"
#include "Axeso5WSSoap.nsmap"
#include <KncUtil.h>
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

#define CLASS_TYPE KCnSoapAuth
#define GAMESERVICE_ID 2
#define USER_ID 32 // Billing의 UserID BYTE랑 맞춤( LIN의 UserID 최대 BYTE 24 )
#define USER_PASSWORD 50
#define IP_STRING_LENGTH 15

KCnSoapAuth::KCommonData KCnSoapAuth::ms_ComnData =
{
    KncCriticalSection(),
    std::string(),
    std::string(),
    std::string(),
};

KCnSoapAuth::KCnSoapAuth(void)
{
    p_gf_soap = NULL;
    ms_ComnData.m_strWsdlPath = "http://auth.axeso5.com/webservice/axeso5WSv3.asmx";
    ms_ComnData.m_strWebMethodPath = "http://www.axeso5.com/webservice/AuthenticateUserwc";
    ms_ComnData.m_strGameServiceID = "6";
}

KCnSoapAuth::~KCnSoapAuth(void)
{
}

bool KCnSoapAuth::Begin()
{
    return KThread::Begin();
}

void KCnSoapAuth::Run()
{
    std::cout << "KSubThread::Run(), thread start." << std::endl;

    GSoapInit(); // Soap Init.

    DWORD ret;
    while( true )
    {
        ret = ::WaitForSingleObject( m_hKillEvent, 1 );
        if( ret == WAIT_OBJECT_0 ) break;
        if( ret == WAIT_TIMEOUT )  Loop();
        else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;
    }// while
    GSoapDestory(); // Soap Destory.
}

void KCnSoapAuth::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff) );

    switch( kIntEvent_.m_usEventID )
    {
        INT_CASE( ENU_VERIFY_ACCOUNT_REQ );
    default:
        START_LOG( cerr, L"Invalid Event ID. ID : " << kIntEvent_.m_usEventID ) << END_LOG;
        break;
    }
    return;
}

void KCnSoapAuth::GSoapInit()
{
    p_gf_soap = soap_new();
    if( !p_gf_soap ) { // couldn't allocate: stop
        p_gf_soap = NULL;

        START_LOG( cerr, L"Soap_new Error" )
            << END_LOG;

        return;
    }
}

void KCnSoapAuth::GSoapDestory()
{
    if( p_gf_soap != NULL )
    {
        soap_destroy(p_gf_soap);
        soap_end(p_gf_soap);
        soap_free(p_gf_soap);
    }
}

void KCnSoapAuth::SetWsdlPath( IN const char* szWsdlPath_ )
{
    KLocker lock( ms_ComnData.m_csSoapAuth );
    ms_ComnData.m_strWsdlPath = szWsdlPath_;
}

void KCnSoapAuth::SetWebMethodPath( IN const char* szWebMethodPath_ )
{
    KLocker lock( ms_ComnData.m_csSoapAuth );
    ms_ComnData.m_strWebMethodPath = szWebMethodPath_;
}

void KCnSoapAuth::SetGameServiceID( IN const char* szGameServiceID_ )
{
    KLocker lock( ms_ComnData.m_csSoapAuth );
    ms_ComnData.m_strGameServiceID = szGameServiceID_;
}

void KCnSoapAuth::test()
{
    /*
    2012. 10. 25. by lucidust
    남미 PC방 혜택 추가 개발

    ID: GCcyber1 ~ GCcyber10, testergc01
    Pass: tester123 -- 패스워드는 md5계산을 해야 테스트 페이지에서 사용할 수 있다
    md5(tester123) = 8e607a4752fa2e59413e5790536f2b42

    경험치 보너스 Player IP:
    200.1.1.1 : 10%
    200.2.2.2 : 15%
    190.3.3.3 : 20%
    192.4.4.4 : 25%

    테섭용: http://auth.tst.axeso5.com/webservice/axeso5WSv3.asmx
    본섭용: http://auth.axeso5.com/webservice/axeso5WSv3.asmx
    */
    struct _ns1__AuthenticateUser GSoapInput; // GSoapInput struct.
    struct _ns1__AuthenticateUserResponse GSoapOutput; // GSoapOutput struct.

    GSoapInput.login = "testergc01";
    GSoapInput.password = "8e607a4752fa2e59413e5790536f2b42";
    GSoapInput.game = "6";

    if( !p_gf_soap ) {
        return;
    }

    // GSoap 함수 호출을 통해서 유저 인증 체크.
    int nResult = soap_call___ns1__AuthenticateUser( p_gf_soap,
        ms_ComnData.m_strWsdlPath.c_str(), ms_ComnData.m_strWebMethodPath.c_str(),
        &GSoapInput, &GSoapOutput );

    // GSoap 호출 체크.
    if( 0 != nResult ) {
        START_LOG( cerr, L"GSoap 호출 에러" ) << END_LOG;
        return;
    }

    // GSoapOutput OUTPUT Value Check
    // 1 : 인증 성공, -1 : 인증 실패.
    if( 1 != GSoapOutput.AuthenticateUserResult ) {
        START_LOG( cerr, L"인증 실패" << GSoapOutput.AuthenticateUserResult ) << END_LOG;
        return;
    }
}

INT_IMPL_ON_FUNC( ENU_VERIFY_ACCOUNT_REQ )
{
    KENU_VERIFY_ACCOUNT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_strLogin = KncUtil::toWideString( kPacket_.m_strLogin );
    struct _ns1__AuthenticateUser GSoapInput; // GSoapInput struct.
    struct _ns1__AuthenticateUserResponse GSoapOutput; // GSoapOutput struct.

    SET_ERROR( ERR_UNKNOWN );

    // ID, PW가 비었는지 체크.
    if( kPacket_.m_strLogin.empty() || kPacket_.m_strPasswd.empty() ) {
        SET_ERROR( ERR_NUSER_VERIFY_09 );
        goto err_proc;
    }

    if( !p_gf_soap ) { // gsoap 모듈이 정상적으로 생성되었는지에 대한 NULL 체크.
        return;
    }

    // 마지막 NULL값 보정.
    char cUserName[USER_ID+1] = {0,};
    char cUserPassword[USER_PASSWORD+1] = {0,};
    char cGameServiceID[GAMESERVICE_ID+1] = {0,};
    char cIPStr[IP_STRING_LENGTH+1] = {0,};     // PC방 혜택을 위해 추가됨. 2012. 10. 24. lucidust
    cUserName[USER_ID] = NULL;
    cUserPassword[USER_PASSWORD] = NULL;
    cGameServiceID[GAMESERVICE_ID] = NULL;
    cIPStr[IP_STRING_LENGTH] = NULL;

    strncpy_s( cUserName, kPacket_.m_strLogin.c_str(), USER_ID );
    strncpy_s( cUserPassword, kPacket_.m_strPasswd.c_str(), USER_PASSWORD );
    strncpy_s( cGameServiceID, ms_ComnData.m_strGameServiceID.c_str(), GAMESERVICE_ID );
    strncpy_s( cIPStr, KncUtil::toNarrowString( kPacket_.m_strIP ).c_str(), IP_STRING_LENGTH );

    // 로그인 ID, PW 입력.
    GSoapInput.login = cUserName;
    GSoapInput.password = cUserPassword;
    GSoapInput.game = cGameServiceID;
    GSoapInput.PlayersIp = cIPStr;

    // GSoap 함수 호출을 통해서 유저 인증 체크.
    int nResult = soap_call___ns1__AuthenticateUser( p_gf_soap,
                    ms_ComnData.m_strWsdlPath.c_str(), ms_ComnData.m_strWebMethodPath.c_str(),
                    &GSoapInput, &GSoapOutput );

    // GSoap 호출 체크.
    if( 0 != nResult ) {
        START_LOG( cerr, L"soap_call___ns1__AuthenticateUser func call Error." ) << END_LOG;
        SET_ERR_GOTO( ERR_NUSER_VERIFY_24, err_proc );
    }

    // GSoapOutput OUTPUT Value Check
    // 1 : 인증 성공, -1 : 인증 실패.
    switch( GSoapOutput.AuthenticateUserResult )
    {
        case 1:
            START_LOG( clog, L"SOAP Auth Check Success Result : " << GSoapOutput.AuthenticateUserResult ) << END_LOG;
            SET_ERROR( NET_OK );
            break;
        case 101:   // PC방 혜택 1
        case 102:   // PC방 혜택 2
        case 103:   // PC방 혜택 3
        case 104:   // PC방 혜택 4
            START_LOG( clog, L"SOAP Auth Check Success(CIBER CAFE benefits) Result : " << GSoapOutput.AuthenticateUserResult ) << END_LOG;
            kPacket_.m_nLinBonus = GSoapOutput.AuthenticateUserResult;
            SET_ERROR( NET_OK );
            break;
        case -1:
            START_LOG( cerr, L"UnKnown User Error Result : " << GSoapOutput.AuthenticateUserResult ) << END_LOG;
            SET_ERROR( ERR_NUSER_VERIFY_24 );
            break;
        case -2:
            START_LOG( cerr, L"Web Services Internal Error. Result : " << GSoapOutput.AuthenticateUserResult ) << END_LOG;
            SET_ERROR( ERR_NUSER_VERIFY_25 );
            break;
        case -3:
            START_LOG( cerr, L" Password Incorrect Result : " << GSoapOutput.AuthenticateUserResult ) << END_LOG;
            SET_ERROR( ERR_NUSER_VERIFY_10 );
            break;
        case -4:
            START_LOG( cerr, L" Maintenance Time Result : " << GSoapOutput.AuthenticateUserResult ) << END_LOG;
            SET_ERROR( ERR_NUSER_VERIFY_16 );
            break;
        case -5:
            START_LOG( cerr, L" SOAP Login Error Result : " << GSoapOutput.AuthenticateUserResult ) << END_LOG;
            SET_ERROR( ERR_NUSER_VERIFY_26 );
            break;
        case -6:
            START_LOG( cerr, L" Unknown Service Error Result : " << GSoapOutput.AuthenticateUserResult ) << END_LOG;
            SET_ERROR( ERR_NUSER_VERIFY_27 );
            break;
        case -7:
            START_LOG( cerr, L"SOAP Auth Check Fail Result : " << GSoapOutput.AuthenticateUserResult ) << END_LOG;
            SET_ERROR( ERR_NUSER_VERIFY_28 );
            break;
        case -8:
            START_LOG( cerr, L"User has not confirmed his axeso5.com account yet. Result : " << GSoapOutput.AuthenticateUserResult ) << END_LOG;
            SET_ERROR( ERR_NUSER_VERIFY_26 );
            break;
        case -9:
            START_LOG( cerr, L"Application Data Layer error. Result : " << GSoapOutput.AuthenticateUserResult ) << END_LOG;
            SET_ERROR( ERR_NUSER_VERIFY_26 );
            break;
        case -10:
            START_LOG( cerr, L"Country or Region not supported. Result : " << GSoapOutput.AuthenticateUserResult ) << END_LOG;
            SET_ERROR( ERR_NUSER_VERIFY_31 );
            break;
        default:
            START_LOG( cerr, L"Undefined Error Result : " << GSoapOutput.AuthenticateUserResult ) << END_LOG;
            SET_ERROR( ERR_NUSER_VERIFY_26 );
            break;
    }

    if( NetError::GetLastNetError() != NetError::NET_OK ) {
        START_LOG( cerr, L"SOAP Auth Check Fail Result : " << GSoapOutput.AuthenticateUserResult ) << END_LOG;
    }

    //SET_ERROR( NET_OK );

err_proc:
    kPacket.m_nOK = NetError::GetLastNetError();
    if( IS_CORRECT( NET_OK ) ) {
        SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_ENU_VERIFY_ACCOUNT_REQ, strLogin_.c_str(), 0, 0, kPacket_ );
    } 
    else {
        KCenterSimLayer::GetInstance()->m_kActorManager2.QueueingEventTo( strLogin_, IDVenderType::ENU_VERIFY_ACCOUNT_ACK, kPacket );
    }

    LOG_CONDITION( kPacket.m_nOK == 0, clog, cerr )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( KncUtil::toWideString( ms_ComnData.m_strWsdlPath ) )
        << BUILD_LOG( KncUtil::toWideString( ms_ComnData.m_strWebMethodPath ) )
        << BUILD_LOG( KncUtil::toWideString( ms_ComnData.m_strGameServiceID ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strLogin ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) )
        << BUILD_LOG( kPacket_.m_strIP ) << END_LOG;
}
