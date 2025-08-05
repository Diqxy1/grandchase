#include ".\gsgash.h"
#include "ThreadManager.h"
#include "GSSimLayer.h"
#include "GameServer.h"
#include "GSDBLayer.h"
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include "Socket/NetCommon.h"
#include <boost/algorithm/string.hpp> //to_lower
#include "BillingError.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

#define CLASS_TYPE KGSGash

KGSGash::KCommonData KGSGash::ms_ComnData = {
    KncCriticalSection(), std::string(), {/*0, 0,*/ 0}, std::string(), std::string()
};

KGSGash::KGSGash(void) : m_dwWaitTime( 8000 )
{
    m_nResult = -1;
    m_strOutErr.assign( "" );
    m_strOutLogin.assign( "" );
    m_strOutAccType.assign( "" );

    KLocker lock( ms_ComnData.m_cs );

    for( int i = 0; i < ACTION_NUM; i++ )
    {
        m_kSock[i].m_pkGash = this;
        m_kSock[i].SetKey( i );

        m_hEvents[ i * 2 + 0 ] = CreateEvent( NULL, false, false, NULL );
        m_hEvents[ i * 2 + 1 ] = CreateEvent( NULL, false, false, NULL );
        m_kSock[i].SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[ i * 2 + 0 ] );
        m_kSock[i].SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[ i * 2 + 1 ] );

        LIF( m_kSock[i].Connect( ms_ComnData.m_strGashIP.c_str(), ms_ComnData.m_usPort[i], false ) );

        m_hEndEvent[i] = CreateEvent( NULL, false, false, NULL ); // 완료시 이벤트 생성위해.
    }
}

KGSGash::~KGSGash(void)
{
    End( 15000 ); // 너무 빨리 끝내면 타임아웃시간 보다 짧아서 멤버를 참조하다 크래시 난다.
    // for loop에 등호(=)를 넣지 않는다. 맨 마지막 이벤트는 스레드 종료 감지 이벤트로, KThread에서 처리된다.
    for( int i = 0; i < ACTION_NUM * 2; i++ )
    {
        ::CloseHandle( m_hEvents[i] );
        m_hEvents[i] = NULL;
    }

    for( int i = 0 ; i < ACTION_NUM ; ++i )
    {
        ::CloseHandle( m_hEndEvent[i] );
        m_hEndEvent[i] = NULL;
    }

}


bool KGSGash::Begin()  // KThread의 이벤트 객체를 배열에 보관해, WaitMultipleObject를 사용한다.
{
    if( KThread::Begin() )
    {
        // note : 인덱스이므로 +1 하지 않는다.
        m_hEvents[ACTION_NUM * 2] = m_hKillEvent;
        return true;
    }

    return false;
}

void KGSGash::Run()
{
    DWORD ret       = NULL;
    bool bLoop      = true;

    while( bLoop )
    {
        ret = ::WaitForSingleObject( m_hEvents[ACTION_NUM * 2], 100 );

        switch( ret )
        {
        case WAIT_OBJECT_0: 
            bLoop = false;  
            return;  // 스레드 종료
        case WAIT_TIMEOUT: break;
        }

        //ret = ::WaitForMultipleObjects( ACTION_NUM * 2 + 1, m_hEvents, false, 100 );

        //switch( ret )
        //{
        //    //case AUTHEN * 2 + 0:    m_kSock[AUTHEN].OnIOCompleted( KOVERLAPPED::IO_RECV ); break;
        //    //case AUTHEN * 2 + 1:    m_kSock[AUTHEN].OnIOCompleted( KOVERLAPPED::IO_SEND ); break;
        //case AUTHOR * 2 + 0:    m_kSock[AUTHOR].OnIOCompleted( KOVERLAPPED::IO_RECV ); break;
        //case AUTHOR * 2 + 1:    m_kSock[AUTHOR].OnIOCompleted( KOVERLAPPED::IO_SEND ); break;
        //case ACCOUNT * 2 + 0:   m_kSock[ACCOUNT].OnIOCompleted( KOVERLAPPED::IO_RECV ); break;
        //case ACCOUNT * 2 + 1:   m_kSock[ACCOUNT].OnIOCompleted( KOVERLAPPED::IO_SEND ); break;

        //case ACTION_NUM * 2:    bLoop = false;  return;  // 스레드 종료
        //case WAIT_TIMEOUT:                      break;
        //}

        Tick();
    }
}

void KGSGash::KSocketGash::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {                    
        START_LOG( clog, L"closed by remote machine" ) << END_LOG;
        OnSocketError();
        return;
    }

    m_ovlRecv.m_dwLeft += dwTransfered_;

    DWORD dwIndex = 0;

    // 길이가 최소 4바이트 이상이고, 아직 버퍼의 끝까지 도달하지 않은 경우
    // nIndex + 1 : 현재 가리키는 곳 다음 바이트까지 접근해야 하므로 +1
    // m_ovlRecv.m_dwLeft - 1 : array의 index처럼 접근되므로 길이에서 -1
    while( m_ovlRecv.m_dwLeft > 4 && dwIndex + 1 <= m_ovlRecv.m_dwLeft - 1 )
    {
        // 온전한 데이터 하나를 분리해서 처리한다.
        if( m_ovlRecv.m_pBuffer[dwIndex] == '\r' && m_ovlRecv.m_pBuffer[dwIndex+1] == '\n' )
        {
            char buff[512];
            memset( buff, 0, 512 ); //초기화.
            // index + 1 까지 끊어낸다.
            ::memcpy( buff, m_ovlRecv.m_pBuffer, dwIndex + 2 );
            {
                KLocker lock( m_pkGash->m_csResult );

                std::string strTmp( buff );    
                typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
                boost::char_separator<char> sep(";\n\r");
                tokenizer tokens( strTmp, sep);
                tokenizer::iterator tok_iter = tokens.begin();

                do{
                    if( tok_iter == tokens.end() ) break;
                    m_pkGash->m_nResult = ::atoi( tok_iter->c_str() );

                    ++tok_iter;
                    if( tok_iter == tokens.end() ) break;
                    m_pkGash->m_strOutErr = *tok_iter;

                    ++tok_iter;
                    if( tok_iter == tokens.end() ) break;
                    m_pkGash->m_strOutLogin = *tok_iter;

                    ++tok_iter;
                    if( tok_iter == tokens.end() ) break;
                    m_pkGash->m_strOutAccType = *tok_iter;                    

                }while( false );
            }
            // 버퍼에 남아있는 데이터를 앞으로 이동한다.
            m_ovlRecv.m_dwLeft -= dwIndex + 2;
            ::memmove( m_ovlRecv.m_pBuffer, &m_ovlRecv.m_pBuffer[dwIndex + 2], m_ovlRecv.m_dwLeft );
            dwIndex = 0;

            switch( m_dwKey ) {
                //case AUTHOR:    SetEvent( m_pkGash->m_hEndEvent[AUTHOR] ); break; // m_pkGash->OnRecvAuthor( nResult, szOutput );    break;
                case ACCOUNT:   SetEvent( m_pkGash->m_hEndEvent[ACCOUNT] ); break; //m_pkGash->OnRecvAccount( nResult, szOutput );   break;
            }
        }
        else
        {
            dwIndex++;
        }
    }

    LIF( InitRecv() );
}

void KGSGash::InitGashInfo( const char* szIP_, 
                         unsigned short usPortAuthen_, 
                         unsigned short usPortAuthor_, 
                         unsigned short usPortAccount_, 
                         int iServiceCode_, 
                         const char* szRegionCode_ )
{
    KLocker lock( ms_ComnData.m_cs );

    char buff[32] = {0};

    ms_ComnData.m_strGashIP         = szIP_;
    //ms_ComnData.m_usPort[AUTHEN]    = usPortAuthen_;
    //ms_ComnData.m_usPort[AUTHOR]    = usPortAuthor_;
    ms_ComnData.m_usPort[ACCOUNT]   = usPortAccount_;
    ms_ComnData.m_strServiceCode    = ::itoa( iServiceCode_, buff, 10 );
    ms_ComnData.m_strRegionCode     = szRegionCode_;
}

void KGSGash::Tick()
{
    m_spEvent.reset();

    // 새 이벤트를 처리하고자 하면, 현재 전송중인 데이터가 없어야 한다.
    //JIF( !m_kSock[AUTHEN].IsSending() );
    //JIF( !m_kSock[AUTHOR].IsSending() );
    JIF( !m_kSock[ACCOUNT].IsSending() );

    if( !m_pkThreadMgr->GetIntEvent( m_spEvent ) )
        return;

    if( m_spEvent == NULL )
    {
        std::cout << "Thread Terminated." << std::endl;
        SetEvent( m_hKillEvent );
        return;
    }

    KSerializer ks;
    ks.BeginReading( &m_spEvent->m_kbuff );
    KIntEvent& kIntEvent_ = *m_spEvent;  // _INT_CASE 를 사용하기 위해서 필요하다.

    // 한개의 데이터 처리, 지금부터 시작.
    DWORD dwElapTime = ::GetTickCount();

    // 이벤트의 종류를 확인 (1. Authentication 2. Authorization 3. Accounting )
    switch( m_spEvent->m_usEventID ) {
       _INT_CASE( EVENT_VERIFY_ACCOUNT_REQ, KDB_EVENT_VERIFY_ACCOUNT_REQ );
       _INT_CASE( DB_EVENT_EXIT_GAME_NOT, KGASH_ACCOUNTING );
        default:
            START_LOG( cerr, L"invalid event id. id : " << m_spEvent->m_usEventID  ) << END_LOG;
            break;
    }
    dwElapTime = ::GetTickCount() - dwElapTime;
    m_pkThreadMgr->CheckMaxProcessingInfo<IDVenderType>( dwElapTime, *m_spEvent );

}

void KGSGash::KSocketGash::OnSocketError()
{
    CLOSE_SOCKET( m_sock );
    m_ovlSend.Clear();
    m_ovlRecv.Clear();
    m_ovlSend.m_dwLeft      = 0;
    m_ovlRecv.m_dwLeft      = 0;
    m_ovlSend.m_bSending    = false;
    m_ovlRecv.m_bSending    = false;

    LIF( Connect( NULL, NULL, false ) );
}

bool KGSGash::WaitEvents( IN const ENUM_ACTION_TYPE eAction_, IN const std::wstring& strLogin_  )
{

    DWORD dwRet;
    DWORD dwTotalElpasedTime = ::GetTickCount();
    do{

        do{
            dwRet = WaitForSingleObject( m_hEvents[ eAction_ * 2 + 1 ], 3000 );
            if( dwRet == WAIT_TIMEOUT )
                break;
            m_kSock[eAction_].OnIOCompleted( KOVERLAPPED::IO_SEND );
        }while( m_kSock[eAction_].IsSending() ); //send가 완료할때까지 돈다.

        if( dwRet  == WAIT_TIMEOUT ) break;

        do{
            dwRet = WaitForSingleObject( m_hEvents[ eAction_ * 2 ], m_dwWaitTime );
            if( dwRet == WAIT_TIMEOUT )
                break;
            m_kSock[eAction_].OnIOCompleted( KOVERLAPPED::IO_RECV );

            dwRet = WaitForSingleObject( m_hEndEvent[ eAction_ ], 1 );

            if( dwRet == WAIT_OBJECT_0 ) break;

        }while( dwRet  == WAIT_TIMEOUT );

        if( dwRet  == WAIT_TIMEOUT ) break;

        if( ::GetTickCount() - dwTotalElpasedTime > 10000 )
        {
            dwRet  = WAIT_TIMEOUT;
            break;
        }

    }while(false);

    dwTotalElpasedTime = ::GetTickCount() - dwTotalElpasedTime;

    if( dwRet == WAIT_TIMEOUT ) // 시간 초과
    {
        START_LOG( cerr, L"Event processing - time over. elapsed : " << m_dwWaitTime )
            << BUILD_LOG( strLogin_ )
            << END_LOG;

        //이벤트 리셋.
        ResetEvent( m_hEvents[ eAction_ * 2 + 1 ] );
        ResetEvent( m_hEvents[ eAction_ * 2 ] );
        ResetEvent( m_hEndEvent[ eAction_ ] );

        for( int i = 0; i < ACTION_NUM; i++ )
        {
            m_kSock[i].CancelIo();
            m_kSock[i].OnSocketError();
        }
        InitResultData();
        START_LOG( cerr, L"Gash ReConnection" ) << END_LOG;
        return false;
    }
    return true;
}

_INT_IMPL_ON_FUNC( EVENT_VERIFY_ACCOUNT_REQ, KDB_EVENT_VERIFY_ACCOUNT_REQ )
{
    //InitResultData();
    //ResetEvent( m_hEndEvent[AUTHOR] );
    //boost::format fmter( "%s'%s'%s'%s'1'I'0'%s'\r\n" );

    //{ // locking scope

    //    KLocker lock( ms_ComnData.m_cs );

    //    fmter % ms_ComnData.m_strServiceCode 
    //        % ms_ComnData.m_strRegionCode
    //        % kPacket_.m_kPacket.m_strLogin 
    //        % kPacket_.m_kPacket.m_strPasswd 
    //        % KncUtil::toNarrowString( kPacket_.m_kPacket.m_strGetIP );

    //} // locking scope

    //std::string strData = fmter.str();

    //START_LOG( clog, L"ACCOUNT. msg : " << strData ) << END_LOG;

    //LIF( m_kSock[AUTHOR].SendData( strData.c_str(), strData.size() ) );

    //LIF( WaitEvents( AUTHOR, strLogin_ ) ); //어떤값이든지 유저에게 반환해줘야 한다.

    //int nResult;
    //std::string strOutput;
    //std::string strOutLogin;
    //std::string strOutAccType;
    //{
    //    KLocker lock( m_csResult );
    //    nResult = m_nResult;
    //    strOutput = m_strOutErr;
    //    strOutLogin = m_strOutLogin;
    //    strOutAccType = m_strOutAccType;

    //}

    //if( !strOutAccType.empty() && ::strstr( strOutAccType.c_str(), "YH" ) != NULL )
    //{
    //    kPacket_.m_nPubEvent = 1; // 061204. kkurrung. 대만 야후 이벤트
    //}

    //if( nResult == 1 && strOutLogin == kPacket_.m_kPacket.m_strLogin )
    //{
    //    if( kPacket_.m_kPacket.m_strLogin.size() > 4 )  // 4까지 포함하면 4자일 경우 xor 할때 모두 0이 됨.
    //    {
    //    std::string strLogin = boost::to_lower_copy( kPacket_.m_kPacket.m_strLogin );
    //    boost::trim(strLogin);

    //        unsigned int dwPrefix   = 0;
    //        unsigned int dwsuffix   = 0;

    //        // 앞뒤에서 4바이트씩 떼어내서 xor 한다.
    //        ::memcpy( &dwPrefix, strLogin.substr( 0, 4 ).c_str(), sizeof(dwPrefix) );
    //        ::memcpy( &dwsuffix, strLogin.substr( strLogin.size() - 4 ).c_str(), sizeof(dwsuffix) );

    //        kPacket_.m_kPacket.m_strPasswd = boost::str( boost::format( "NM%010d" ) % (dwPrefix^dwsuffix) );
    //    }

    //    SiKGameServer()->GetDBLayer()->QueueingEvent( KUserEvent::EVENT_VERIFY_ACCOUNT_REQ, strLogin_.c_str(), 0, kPacket_ );
    //}
    //else // 실패한 경우, 유저에게 실패 ack를 보낸다.
    //{
    //    KEVENT_DB_VERIFY_ACCOUNT_ACK kPacket;
    //    std::wstring& strLogin_ = m_spEvent->m_strSender;   // 매크로를 활용하기 위해 reference 사용.

    //    kPacket.m_kVerifyPacket.m_ucOK = 11;    // 실패 원인마다 분류된 ok를 던지려면 스트링을 모두 읽어야 한다.
    //    kPacket.m_kVerifyPacket.m_strServerName = KncUtil::toWideString( strOutput );

    //    SiKBillingError()->ErrorDetect( strOutput );

    //    QUEUING_ACK_TO_USER( EVENT_VERIFY_ACCOUNT_ACK );
    //}

    //if( kPacket_.m_kPacket.m_strLogin != strOutLogin )
    //{
    //    START_LOG( cerr, L" Not Eq Login " << kPacket_.m_kPacket.m_strLogin << L" != " << strOutLogin << END_LOG ) << END_LOG;
    //}
    //
    //LOG_CONDITION( nResult == 1 && strOutLogin == kPacket_.m_kPacket.m_strLogin, Info, Error2 )

    //    << BUILD_LOG( strLogin_ )
    //    << BUILD_LOG( strOutLogin )
    //    << BUILD_LOG( strOutAccType )
    //    << BUILD_LOG( nResult )
    //    << BUILD_LOG( strOutput )
    //    << BUILD_LOG( strData ) << END_LOG;
}

_INT_IMPL_ON_FUNC( DB_EVENT_EXIT_GAME_NOT, KGASH_ACCOUNTING )
{
    InitResultData();
    ResetEvent( m_hEndEvent[ACCOUNT] );
    boost::format fmter( "%s'%s'%s'R'''%s'0'\r\n" );

    { // locking scope

        KLocker lock( ms_ComnData.m_cs );

        fmter   % ms_ComnData.m_strServiceCode 
                % ms_ComnData.m_strRegionCode
                % kPacket_.m_strLogin 
                % kPacket_.m_strIP;

    } // locking scope

    std::string strData = fmter.str();

    START_LOG( clog, L"[ACCOUNTING] Log Out. msg : " << KncUtil::toWideString( strData ) ) << END_LOG;

    LIF( m_kSock[ACCOUNT].SendData( strData.c_str(), strData.size() ) );

    LIF( WaitEvents( ACCOUNT, strLogin_ ) ); //로그아웃시, 서버간 이동시 항상 유저에게 반환해줘야 한다.

    int nResult;
    std::string strOutput;
    std::string strOutLogin;
    std::string strOutAccType;
    {
        KLocker lock( m_csResult );
        nResult         = m_nResult;
        strOutput       = m_strOutErr;
        strOutLogin     = m_strOutLogin;
        strOutAccType   = m_strOutAccType;
    }
    
   if( !kPacket_.m_strWantedServerName.empty() ) // 서버간 이동을 위해 미리 logout 처리한 경우.
    {
        START_LOG( clog, L"Server Migration" )
            << BUILD_LOG( kPacket_.m_strWantedServerName ) << END_LOG;

        // 060508. kkurrung. 현재는 서버 마이그레이션을 센터를 통해 하지 않는다.
        int kAck = 0;
        _QUEUING_ACK_TO_USER( EVENT_SERVER_MIGRATION_ACK, kAck );
    }

    if( kPacket_.m_strLogin != strOutLogin )
    {
        START_LOG( cerr, L" Not Eq Login " << KncUtil::toWideString( kPacket_.m_strLogin ) << L" != " << KncUtil::toWideString( strOutLogin  )) << END_LOG;
    }

    if( nResult != 1 )
    {
        SiKBillingError()->ErrorDetect( strOutput );
    }
    LOG_CONDITION( nResult == 1 && strOutLogin == kPacket_.m_strLogin, clog, cerr )
        << L" [Gash Authrization]"
        << BUILD_LOG( strLogin_ )
        << BUILD_LOG( KncUtil::toWideString( strOutLogin ))
        << BUILD_LOG( KncUtil::toWideString( strOutAccType ) )
        << BUILD_LOG( nResult )
        << BUILD_LOG( KncUtil::toWideString( strOutput ) )
        << BUILD_LOG( KncUtil::toWideString( strData ) ) << END_LOG;
}