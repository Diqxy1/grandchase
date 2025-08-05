#include "GSSimLayer.h" // #include <WinSock2.h>
#include "NMNetCafeManager.h"
#include "UserPacket.h"
#include <dbg/dbg.hpp>
#include "ThreadManager.h"
#include "KncUtil.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

#define CLASS_TYPE NetCafeManager

NetCafeManager::NetCafeManager(void)
:m_bInitialize(false)
{
}


NetCafeManager::~NetCafeManager(void)
{
    Cleanup();
    End();
}

NiImplementRootRTTI( NetCafeManager );
ImplOstmOperatorW2A( NetCafeManager );
ImplToStringW( NetCafeManager )
{
    KLocker lock( m_csNetCafe );
    return START_TOSTRINGW
        << TOSTRINGW( m_bInitialize )
        << TOSTRINGW( m_mapCnUserUID.size() );
}

bool NetCafeManager::Startup()
{
    // DLL 초기화
    if ( false == InitializeNMNetCafe( "c:\\NMNCAFE.config", this ) ) {
        START_LOG( cerr, L"NMNCafe Initialize Fail" ) << END_LOG;
        return false;
    }

    START_LOG( clog, L"NMNCafe Initialize Success" ) << END_LOG;

    // DLL 프로세스 시작
    if ( false == StartupNMNetCafe() ) {
        START_LOG( cerr, L"NMNCafe Startup Fail" ) << END_LOG;
        return false;
    }

    START_LOG( clog, L"NMNCafe Startup Success" ) << END_LOG;

    SetInitialize( true );
    return true;
}

void NetCafeManager::Cleanup()
{
    // DLL 프로세스 종료 및 자원할당 해제
    if ( IsInitialize() ) {
        CleanupNMNetCafe();
        FinalizeNMNetCafe();
    }
    START_LOG( clog, L"NMNCafe Cleanup" ) << END_LOG;
}

void NetCafeManager::RequestUserCheck( IN const std::wstring& strLogin_, IN const LPCSTR userCN_, IN const ULONG& ipAddress_ )
{
    // PC방 혜택 체크
    KLocker lock( m_csNetCafe );
    in_addr in;
    in.S_un.S_addr = ipAddress_;

    int nRet = lfNMNetCafeChkLogin( userCN_, ipAddress_ );

    _LOG_SUCCESS( nRet == ERROR_NMNCAFE_SUCCESS, L"User Check Login.. Result : " << nRet )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( KncUtil::toWideString( userCN_ ) )
        << BUILD_LOG( inet_ntoa(in) ) << END_LOG;
}

void NetCafeManager::RequestUserLogin( IN const std::wstring& strLogin_, IN const LPCSTR userCN_, IN const ULONG& ipAddress_, IN UINT playTime_/*=0*/ )
{
    // PC방 로그인 (혜택제공시점)
    KLocker lock( m_csNetCafe );
    in_addr in;
    in.S_un.S_addr = ipAddress_;

    int nRet = lfNMNetCafeLogin( userCN_, ipAddress_, playTime_ );

    _LOG_SUCCESS( nRet == ERROR_NMNCAFE_SUCCESS, L"User Login.. Result : " << nRet )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( KncUtil::toWideString( userCN_ ) )
        << BUILD_LOG( inet_ntoa(in) )
        << BUILD_LOG( playTime_ ) << END_LOG; // 초
}

int NetCafeManager::RequestUserLogout( IN const std::wstring& strLogin_, IN const LPCSTR userCN_, IN const ULONG& ipAddress_ )
{
    // PC방 로그아웃
    KLocker lock( m_csNetCafe );
    in_addr in;
    in.S_un.S_addr = ipAddress_;

    int nRet = lfNMNetCafeLogout( userCN_, ipAddress_ );

    _LOG_SUCCESS( nRet == ERROR_NMNCAFE_SUCCESS, L"User Logout.. Result : " << nRet )
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( KncUtil::toWideString( userCN_ ) )
        << BUILD_LOG( inet_ntoa(in) ) << END_LOG;

    return nRet;
}

void NetCafeManager::OnChkLogin( LPCSTR userCN_, UINT deductType_, UINT restPoint_, double expireDateTime_, LPVOID param_ )
{
    // use thread-safe code
    KLocker lock(m_csCallback);
    START_LOG( clog, L"------------- OnChkLogin -----------" << dbg::endl
               << L"userCn         : " << userCN_         << dbg::endl
               << L"deductType     : " << deductType_     << dbg::endl
               << L"restPoint      : " << restPoint_      << dbg::endl
               << L"expireDateTime : " << expireDateTime_ << dbg::endl
               << L"------------------------------------" ) << END_LOG;

    DWORD dwUID = 0;
    JIF( GetCnUserUID( userCN_, dwUID ) );

    KNmNetcafeParamAck kPacket;
    kPacket.m_UserCN = userCN_;
    kPacket.m_DeductType = deductType_;
    kPacket.m_RestPoint = restPoint_;

    START_LOG( clog, L"넷마블 PC방 체크 패킷 전달.. UserUID : " << dwUID )
        << BUILD_LOG( KncUtil::toWideString( kPacket.m_UserCN ) )
        << BUILD_LOG( kPacket.m_DeductType )
        << BUILD_LOG( kPacket.m_RestPoint ) << END_LOG;
}

void NetCafeManager::OnLogin( LPCSTR userCN_, UINT deductType_, UINT restPoint_, double expireDateTime_, LPVOID param_ )
{
    // use thread-safe code
    KLocker lock(m_csCallback);
    //std::clog2 << L"-------------- OnLogin -------------" << std::endl
    //           << L"userCn         : " << userCN_         << std::endl
    //           << L"deductType     : " << deductType_     << std::endl
    //           << L"restPoint      : " << restPoint_      << std::endl
    //           << L"expireDateTime : " << expireDateTime_ << std::endl
    //           << L"------------------------------------" << std::endl;

    DWORD dwUID = 0;
    JIF( GetCnUserUID( userCN_, dwUID ) );

    UINT uiDecPlayTime = 0;
    LIF( GetUserPlayTime( dwUID, uiDecPlayTime ) );

    KNmNetcafeParamAck kPacket;
    kPacket.m_UserCN = userCN_;
    kPacket.m_DeductType = deductType_;
    kPacket.m_RestPoint = restPoint_;
    kPacket.m_uiDecPlayTime = uiDecPlayTime;

    START_LOG( clog, L"넷마블 PC방 로그인 패킷 전달.. UserUID : " << dwUID )
        << BUILD_LOG( KncUtil::toWideString( kPacket.m_UserCN ) )
        << BUILD_LOG( kPacket.m_DeductType )
        << BUILD_LOG( kPacket.m_RestPoint )
        << BUILD_LOG( kPacket.m_uiDecPlayTime ) << END_LOG;

    __QUEUING_ACK_TO_USER( EVENT_NMNETCAFE_LOGIN_ACK, dwUID );
}

void NetCafeManager::OnExpire( UINT result_, LPCSTR userCN_, UINT deductType_, UINT restPoint_, double expireDateTime_ )
{
    // use thread-safe code
    KLocker lock(m_csCallback);
    //std::clog2 << L"-------------- OnExpire ------------" << std::endl
    //           << L"result         : " << result_         << std::endl
    //           << L"userCn         : " << userCN_         << std::endl
    //           << L"deductType     : " << deductType_     << std::endl
    //           << L"restPoint      : " << restPoint_      << std::endl
    //           << L"expireDateTime : " << expireDateTime_ << std::endl
    //           << L"------------------------------------" << std::endl;

    DWORD dwUID = 0;
    JIF( GetCnUserUID( userCN_, dwUID ) );

    KNmNetcafeParamAck kPacket;
    kPacket.m_UserCN = userCN_;
    kPacket.m_Result = result_;
    kPacket.m_DeductType = deductType_;
    kPacket.m_RestPoint = restPoint_;

    START_LOG( clog, L"넷마블 PC방 상품만료 패킷 전달.. UserUID : " << dwUID )
        << BUILD_LOG( KncUtil::toWideString( kPacket.m_UserCN ) )
        << BUILD_LOG( kPacket.m_Result )
        << BUILD_LOG( kPacket.m_DeductType )
        << BUILD_LOG( kPacket.m_RestPoint ) << END_LOG;

    __QUEUING_ACK_TO_USER( EVENT_NMNETCAFE_EXPIRE_NOT, dwUID );
}

void NetCafeManager::OnError( UINT errCode_ )
{
    // use thread-safe code
    KLocker lock(m_csCallback);
    _LOG_SUCCESS( errCode_ == 0 || errCode_ == 100, L" NMCafe OnError..  errCode : " << errCode_ ) << END_LOG;
}

void NetCafeManager::OnLogout( LPCSTR userCN, UINT result, LPVOID param )
{
    KLocker lock(m_csCallback);

    std::wstring strLogin_ = KncUtil::toWideString( userCN );
    

    START_LOG( clog, L"넷마블 로그아웃 응답 ")
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( result )
        << END_LOG;
    
}

bool NetCafeManager::OnAlive( LPCSTR userCN, ULONG ipAddress )
{
    START_LOG( clog, L"NetCafeManager::OnAlive 호출 확인용 로그" ) << END_LOG;
    {
        KLocker lock(m_csCallback);
        DWORD dwUserUID;
        if ( GetCnUserUID( userCN, dwUserUID )  ) {
            return true;
        }
    }

    START_LOG( cwarn, L"서버에 없는 넷마블 유저 Alive 체크 요청" )
        << BUILD_LOG( userCN )
        << END_LOG;
    
    return false;
}

void NetCafeManager::SetCnUserUID( IN const std::string& UserCN_, IN const DWORD& dwUserUID_ )
{
    KLocker lock( m_csNetCafe );
    m_mapCnUserUID[ UserCN_ ] = dwUserUID_;
}

bool NetCafeManager::GetCnUserUID( IN const std::string& UserCN_, OUT DWORD& dwUserUID_ )
{
    dwUserUID_ = 0;

    KLocker lock( m_csNetCafe );

    std::map<std::string,DWORD>::iterator mit;
    mit = m_mapCnUserUID.find( UserCN_ );
    if ( mit == m_mapCnUserUID.end() ) {
        START_LOG( cwarn , L"등록되지않은 유저CN값.. : " << KncUtil::toWideString( UserCN_ ) ) << END_LOG;
        return false;
    }

    dwUserUID_ = mit->second;
    START_LOG( clog, L"UserCN : " << KncUtil::toWideString( UserCN_ ) << L", UserUID : " << dwUserUID_ ) << END_LOG;
    return true;
}

void NetCafeManager::DelCnUserUID( IN const std::string& UserCN_ )
{
    KLocker lock( m_csNetCafe );
    m_mapCnUserUID.erase( UserCN_ );
}

void NetCafeManager::SetUserPlayTime( IN const DWORD& dwUserUID_, IN const UINT& PlayTime_ )
{
    KLocker lock( m_csNetCafe );
    m_mapUserPlayTime[dwUserUID_] = PlayTime_;
}

bool NetCafeManager::GetUserPlayTime( IN const DWORD& dwUserUID_, OUT UINT& PlayTime_ )
{
    PlayTime_ = 0;

    KLocker lock( m_csNetCafe );
    std::map<DWORD,UINT>::iterator mit;
    mit = m_mapUserPlayTime.find( dwUserUID_ );
    if ( mit == m_mapUserPlayTime.end() ) {
        return false;
    }

    PlayTime_ = mit->second;
    return true;
}

void NetCafeManager::DelUserPlayTime( IN const DWORD& dwUserUID_ )
{
    KLocker lock( m_csNetCafe );
    m_mapUserPlayTime.erase( dwUserUID_ );
}

void NetCafeManager::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff ) );

    switch( kIntEvent_.m_usEventID )
    {
        INT_CASE_NOPARAM( EVENT_NMNETCAFE_STARTUP_NOT );
       _INT_CASE( EVENT_NMNETCAFE_LOGIN_REQ, KNmNetcafeParam );
       _INT_CASE( EVENT_NMNETCAFE_LOGOUT_REQ, KNmNetcafeParam );
    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " 
            << IDVenderType::GetEventIDString( kIntEvent_.m_usEventID ) ) << END_LOG;
    }
}

INT_IMPL_ON_FUNC_NOPARAM( EVENT_NMNETCAFE_STARTUP_NOT )
{
    LIF( Startup() );
}

_INT_IMPL_ON_FUNC( EVENT_NMNETCAFE_LOGIN_REQ, KNmNetcafeParam )
{
    SetCnUserUID( kPacket_.m_UserCN, dwUID_ );
    SetUserPlayTime( dwUID_, kPacket_.m_PlayTime );
    RequestUserLogin( strLogin_, kPacket_.m_UserCN.c_str(), kPacket_.m_IpAddress, kPacket_.m_PlayTime );
}

_INT_IMPL_ON_FUNC( EVENT_NMNETCAFE_LOGOUT_REQ, KNmNetcafeParam )
{
    int kPacket = -99;
    kPacket = RequestUserLogout( strLogin_, kPacket_.m_UserCN.c_str(), kPacket_.m_IpAddress );
    DWORD dwUserUID;
    GetCnUserUID( kPacket_.m_UserCN, dwUserUID ); // 유저의 CN 데이터를 삭제하기 전에 UID 를 가져온다.
    DelCnUserUID( kPacket_.m_UserCN );
    DelUserPlayTime( dwUserUID );

    _QUEUING_ACK_TO_USER( EVENT_NMNETCAFE_LOGOUT_ACK, kPacket );
}