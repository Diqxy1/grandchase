#include "CenterGash.h"
#include "ThreadManager.h"
#include "CenterSimLayer.h"
//#include "GSDBHandler.h"
#include "CenterDBThread.h"
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <dbg/dbg.hpp>
#include <boost/algorithm/string.hpp> //to_lower
#include "Log4.h"
//FILE_NAME_FOR_LOG

#define CLASS_TYPE KCenterGash

KCenterGash::KCommonData KCenterGash::ms_ComnData = {
    KncCriticalSection(), std::string(), {/*0, 0,*/ 0}, std::string(), std::string()
};

KCenterGash::KCenterGash(void) :m_dwWaitTime(8000)
{
    m_strOutLogin.assign( "" );
    m_strOutErr.assign( "" );
    m_strOutAccType.assign( "" );
    m_nResult       = 0;

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

KCenterGash::~KCenterGash(void)
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


bool KCenterGash::Begin()  // KThread의 이벤트 객체를 배열에 보관해, WaitMultipleObject를 사용한다.
{
    if( KThread::Begin() )
    {
        // note : 인덱스이므로 +1 하지 않는다.
        m_hEvents[ACTION_NUM * 2] = m_hKillEvent;
        return true;
    }

    return false;
}

void KCenterGash::Run()
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

        Tick();
    }
}

void KCenterGash::KSocketGash::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {                    
        START_LOG( clog, L"closed by remote machine" ) << END_LOG;
        OnSocketError();
        return;
    }

    m_ovlRecv.m_dwLeft += dwTransfered_;
    if( m_ovlRecv.m_dwLeft != dwTransfered_ )
    {
        START_LOG( cerr, L"Not eq" )
            << BUILD_LOG( m_ovlRecv.m_dwLeft )
            << BUILD_LOG( dwTransfered_ ) << END_LOG;
    }

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
            //m_ovlRecv.m_dwLeft -= dwIndex + 2;
            //::memmove( m_ovlRecv.m_pBuffer, &m_ovlRecv.m_pBuffer[dwIndex + 2], m_ovlRecv.m_dwLeft );
            m_ovlRecv.m_dwLeft = 0;

            dwIndex = 0;

            switch( m_dwKey ) {
                case AUTHEN:    SetEvent( m_pkGash->m_hEndEvent[AUTHEN] ); break;//m_pkGash->OnRecvAuthen( nResult, szOutput );    break;
                case AUTHOR:    SetEvent( m_pkGash->m_hEndEvent[AUTHOR] ); break; // m_pkGash->OnRecvAuthor( nResult, szOutput );    break;
            }
        }
        else
        {
            dwIndex++;
        }
    }

    LIF( InitRecv() );
}

void KCenterGash::InitGashInfo( const char* szIP_, 
                          unsigned short usPortAuthen_, 
                          unsigned short usPortAuthor_, 
                          unsigned short usPortAccount_, 
                          int iServiceCode_, 
                          const char* szRegionCode_ )
{
    KLocker lock( ms_ComnData.m_cs );

    char buff[32] = {0};

    ms_ComnData.m_strGashIP         = szIP_;
    ms_ComnData.m_usPort[AUTHEN]    = usPortAuthen_;
    ms_ComnData.m_usPort[AUTHOR]    = usPortAuthor_;
    ms_ComnData.m_strServiceCode    = ::itoa( iServiceCode_, buff, 10 );
    ms_ComnData.m_strRegionCode     = szRegionCode_;
}

void KCenterGash::Tick()
{
    m_spEvent.reset();

    // 새 이벤트를 처리하고자 하면, 현재 전송중인 데이터가 없어야 한다.
    JIF( !m_kSock[AUTHEN].IsSending() );

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
        INT_CASE( ENU_VERIFY_ACCOUNT_REQ );
        default:
            START_LOG( cerr, L"invalid event id. id : " << m_spEvent->m_usEventID  ) << END_LOG;
            break;
    }
    dwElapTime = ::GetTickCount() - dwElapTime;
    m_pkThreadMgr->CheckMaxProcessingInfo<IDVenderType>( dwElapTime, *m_spEvent );

}

void KCenterGash::KSocketGash::OnSocketError()
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

bool KCenterGash::WaitEvents( IN const ENUM_ACTION_TYPE eAction_, IN const std::wstring& strLogin_  )
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


INT_IMPL_ON_FUNC( ENU_VERIFY_ACCOUNT_REQ )
{
    std::string strData;
    //boost::format fmter( "%s'%s'%s'%s'0'%s'0'\r\n" ); // authentication
    boost::format fmterAuthor( "%s'%s'%s'%s'1'I'0'%s'0'\r\n" ); // authorization

    KENU_VERIFY_ACCOUNT_ACK    kPacket;
    kPacket.m_nOK = -1;
    SET_ERROR( ERR_UNKNOWN );

    if( !CheckValidate( kPacket_.m_strPasswd ) || !CheckValidate( kPacket_.m_strLogin ) )
    {
        SET_ERROR( ERR_NUSER_VERIFY_07 );
        goto err_proc;
    }

    InitResultData();
    ResetEvent( m_hEndEvent[AUTHOR] );

    { // locking scope
        KLocker lock( ms_ComnData.m_cs );

        fmterAuthor % ms_ComnData.m_strServiceCode
            % ms_ComnData.m_strRegionCode
            % kPacket_.m_strLogin
            % kPacket_.m_strPasswd
            % KncUtil::toNarrowString( kPacket_.m_strIP );
    } // locking scope

    strData = fmterAuthor.str();

    START_LOG( clog, L"AUTHOR. msg : " << KncUtil::toWideString( strData ) ) << END_LOG;
    LIF( m_kSock[AUTHOR].SendData( strData.c_str(), strData.size() ) );
    LIF( WaitEvents( AUTHOR, strLogin_ ) ); //어떤값이든지 유저에게 반환해줘야 한다.

    if( !m_strOutAccType.empty() && ::strstr( m_strOutAccType.c_str(), "YH" ) != NULL )
    {
        //kPacket_.m_nPubEvent = 1; // 061204. kkurrung. 대만 야후 이벤트
    } // 로긴서버, 게임서버의  야후 이벤트 처리  수정필요...

    if( m_strOutErr == "Wrong_OTP" || m_strOutErr == "Wrong_EventOTP" )
    {
        if( kPacket_.m_bBeanfunUser )
        {
            SET_ERROR( ERR_NUSER_VERIFY_18 );
            goto err_proc;
        }
        else
        {
            SET_ERROR( ERR_NUSER_VERIFY_19 );
            goto err_proc;
        }

    }
    if( m_nResult != 1 && m_strOutErr == "Wrong_ID_or_Password")
    {
        SET_ERROR( ERR_NUSER_VERIFY_10 );
        goto err_proc;
    }

    if( CheckAuthResult( m_strOutLogin, kPacket_.m_strLogin, m_nResult ) == false )
    {
        goto err_proc;
    }

    MakePasswordXOR( kPacket_.m_strLogin, kPacket_.m_strPasswd ); // 패스워드 생성..게쉬 인증 의존하면 필요없지 않을까...

    SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_ENU_VERIFY_ACCOUNT_REQ, strLogin_.c_str(), 0, 0, kPacket_ );

    START_LOG( clog, L"AUTHOR 인증 처리 완료" )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strLogin ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) )
        << BUILD_LOG( m_nResult )
        << BUILD_LOG( KncUtil::toWideString( m_strOutErr ) )
        << BUILD_LOG( KncUtil::toWideString( m_strOutLogin ) )
        << BUILD_LOG( KncUtil::toWideString( m_strOutAccType )) << END_LOG;
    return;

err_proc:

    kPacket.m_nOK = NetError::GetLastNetError();
    SLayerType::GetInstance()->m_kActorManager2.QueueingEventTo( strLogin_, IDVenderType::ENU_VERIFY_ACCOUNT_ACK, kPacket );
    START_LOG( cerr, NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( m_nResult )
        << BUILD_LOG( KncUtil::toWideString( m_strOutErr ) )
        << BUILD_LOG( KncUtil::toWideString( m_strOutLogin ) )
        << BUILD_LOG( KncUtil::toWideString( m_strOutAccType ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strLogin ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strPasswd ) )
        << BUILD_LOG( kPacket_.m_strIP )
        << BUILD_LOG( KncUtil::toWideString( strData ) ) << END_LOG;
}

bool KCenterGash::CheckValidate( std::string& strData )
{
    char chNum[2]   = {'0','9'};
    char loAlpha[2] = {'a','z'};
    char upAlpha[2] = {'A','Z'};

    std::string::iterator sit;

    for( sit = strData.begin() ; sit != strData.end() ; ++sit )
    {
        if( !(  ( chNum[0] <= *sit && chNum[1] >= *sit ) ||
            ( loAlpha[0] <= *sit && loAlpha[1] >= *sit ) ||
            ( upAlpha[0] <= *sit && upAlpha[1] >= *sit )    )
            )
        {
            return false;
        }

    }

    return true;
}

bool KCenterGash::CheckAuthResult( std::string& strOutLogin_, std::string& strInputLogin_, int nResult_ )
{
    if( nResult_ != 1 )
    {
        SET_ERROR( ERR_NUSER_VERIFY_06 ); // 인증 실패.
        return false;
    }

    //if( strOutLogin_ != strInputLogin_ )
    //{
    //    START_LOG( cerr, L" Not Eq Login "  << KncUtil::toWideString( strInputLogin_ ) << L" != "
    //        << KncUtil::toWideString( strOutLogin_ ) << END_LOG );
    //    SET_ERROR( ERR_NUSER_VERIFY_08 );
    //    return false;
    //}

    return true;
}

void KCenterGash::MakePasswordXOR( std::string& strLogin_, std::string& strPasswd_ )
{
    if( strLogin_.size() > 4 )  // 4까지 포함하면 4자일 경우 xor 할때 모두 0이 됨.
    {
        std::string strLogin = boost::to_lower_copy( strLogin_ );
        boost::trim(strLogin);

        unsigned int dwPrefix   = 0;
        unsigned int dwsuffix   = 0;

        // 앞뒤에서 4바이트씩 떼어내서 xor 한다.
        ::memcpy( &dwPrefix, strLogin.substr( 0, 4 ).c_str(), sizeof(dwPrefix) );
        ::memcpy( &dwsuffix, strLogin.substr( strLogin.size() - 4 ).c_str(), sizeof(dwsuffix) );

        strPasswd_ = boost::str( boost::format( "NM%010d" ) % (dwPrefix^dwsuffix) );
    }
}
