#include "CenterSimLayer.h"
#include "CnFBAuth.h"
#include "CenterDBThread.h"
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

#define TIME_OUT_ELAPSED_TIME_LIMIT 8000
#define CLASS_TYPE KCnFBAuth

KCnFBAuth::KCommonData KCnFBAuth::ms_ComnData =
{
    KncCriticalSection(),
    std::string(),
    std::vector<unsigned short>(),
};

KCnFBAuth::KCnFBAuth(void)
:m_dwWaitTime(TIME_OUT_ELAPSED_TIME_LIMIT)
,m_hEndEvent(0)
,m_dwThreadID(0)
{
    //쩌???째첬 ??쨘짜?짰 ??짹창?짯.

    m_strIP = GetIPString();
    m_usPort = GetPort();

    m_kSock.m_pkFBAuth = this; // 
    m_kSock.SetKey( 0 );
    m_kSock.m_dwLastHeartBitTime = ::GetTickCount();

    m_hEvents[0] = CreateEvent( NULL, false, false, NULL ); // IO_RECV
    m_kSock.SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[0] );

    m_hEvents[1] = CreateEvent( NULL, false, false, NULL ); // IO_SEND
    m_kSock.SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[1] );

    LIF( m_kSock.Connect( m_strIP.c_str(), m_usPort, false ) );
}

KCnFBAuth::~KCnFBAuth(void)
{
    End( 15000 ); // 쩍쨘쨌쨔쨉책 ?쩐쨌찼쨍짝 15?? 짹창쨈?쨌????
    int i = 0;
    for( i = 0; i < 2; i++ )
    {
        ::CloseHandle( m_hEvents[i] ); // 2째쨀?? ??쨘짜?짰
        m_hEvents[i] = NULL;
    }
    ::CloseHandle( m_hEndEvent ); // 1째쨀?? ?쩐쨌찼 ??쨘짜?짰?
    m_hEndEvent = NULL;
}

void KCnFBAuth::SetAuthIP( IN const char* szIP_ )
{
    KLocker lock( ms_ComnData.m_csData );
    ms_ComnData.m_strServerIP   = szIP_;
}

void KCnFBAuth::AddAuthPort ( IN unsigned short usPort_ )
{
    KLocker lock( ms_ComnData.m_csData );
    ms_ComnData.m_vecPorts.push_back( usPort_ );
}

void KCnFBAuth::DumpData()
{
    KLocker lock( ms_ComnData.m_csData );
    std::cout << "IP : " << ms_ComnData.m_strServerIP << std::endl;
    std::cout << "Port : ";
    std::copy( ms_ComnData.m_vecPorts.begin(), ms_ComnData.m_vecPorts.end(),
        std::ostream_iterator<int>(std::cout, ", " ) );
    std::cout << std::endl;
}

unsigned short KCnFBAuth::GetPort()
{
    unsigned short usPort = 0;
    KLocker lock( ms_ComnData.m_csData );

    if( ms_ComnData.m_vecPorts.empty() ) return usPort;
    usPort = ms_ComnData.m_vecPorts.back();
    ms_ComnData.m_vecPorts.pop_back();

    return usPort;
}

std::string KCnFBAuth::GetIPString()
{
    KLocker lock( ms_ComnData.m_csData );
    return ms_ComnData.m_strServerIP;
}

void KCnFBAuth::KSocketFBAuth::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {
        START_LOG( clog, L"closed by remote machine" ) << END_LOG;
        OnSocketError();
        return;
    }
    START_LOG( clog, L"???쨋 쨔??쩍. Size : " << dwTransfered_ ) << END_LOG;
    if( dwTransfered_ >= 128 )
    {
        START_LOG( clog, L"128 ??째첬??쨈? ???쨋 쨔??쩍. Size : " << dwTransfered_ ) << END_LOG;
        return;
    }
    m_ovlRecv.m_dwLeft += dwTransfered_;

    char buff[128];
    ::memset( buff, 0, sizeof( buff ) );
    ::memcpy( buff, m_ovlRecv.m_pBuffer, dwTransfered_ ); // 쩔짤짹창쩌짯 ???쨘째? 째째?쨘쨉짜 

    m_pkFBAuth-> InitResultData();
    m_pkFBAuth->m_strResult.append( buff, dwTransfered_ );
    boost::trim( m_pkFBAuth->m_strResult );

    m_ovlRecv.m_dwLeft = 0;
    SetEvent( m_pkFBAuth->m_hEndEvent );
    LIF( InitRecv() );
}

void KCnFBAuth::KSocketFBAuth::OnSocketError()
{
    CLOSE_SOCKET( m_sock );
    m_ovlSend.Clear();
    m_ovlRecv.Clear();
    m_ovlSend.m_dwLeft      = 0;
    m_ovlRecv.m_dwLeft      = 0;
    m_ovlSend.m_bSending    = false;
    m_ovlRecv.m_bSending    = false;

    // 쨀?쨔짬 쨔짬??쨘챰??째? reconnect ??쨈?.
    ::Sleep( 1000 );

    START_LOG( clog, L"PayLetter 쩌??? 짼첨쩐챤?체... Thread ID : " << m_pkFBAuth->m_dwThreadID ) << END_LOG;
    LIF( Connect( NULL, NULL, false ) );
}

bool KCnFBAuth::Begin()
{
    return KThread::Begin();
}

void KCnFBAuth::Run()
{
    std::cout << "KSubThread::Run(), thread start." << std::endl;
    m_dwThreadID = ::GetCurrentThreadId();
    DWORD ret;
    while( true )
    {
        ret = ::WaitForSingleObject( m_hKillEvent, 1 );
        if( ret == WAIT_OBJECT_0 ) break;
        if( ret == WAIT_TIMEOUT )  Loop();
        else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;
    }// while
}

bool KCnFBAuth::WaitEvents( IN const std::wstring& strLogin_ )
{
    DWORD dwRet;
    DWORD dwTotalElapsedTime = ::GetTickCount();

    // Send Complete
    do{
        dwRet = WaitForSingleObject( m_hEvents[1], m_dwWaitTime ); // IO Send
        if( dwRet == WAIT_TIMEOUT )
            break;
        m_kSock.OnIOCompleted( KOVERLAPPED::IO_SEND );
    }while( m_kSock.IsSending() ); //send째징 쩔?쨌찼쨉? 쨋짠짹챤?철 쨉쨌쨈?.

    if( dwRet == WAIT_TIMEOUT ) goto end_proc;

    // Recv Complete
    do {
        dwRet = WaitForSingleObject( m_hEvents[0], m_dwWaitTime ); // IO_RECV
        if( dwRet == WAIT_TIMEOUT )
            break;

        m_kSock.OnIOCompleted( KOVERLAPPED::IO_RECV );

        dwRet = WaitForSingleObject( m_hEndEvent, 1 );

        if( dwRet == WAIT_OBJECT_0 ) break;

    }while( dwRet == WAIT_TIMEOUT );

    if( dwRet == WAIT_TIMEOUT ) goto end_proc;

    if( ::GetTickCount() - dwTotalElapsedTime > TIME_OUT_ELAPSED_TIME_LIMIT )
    {
        dwRet = WAIT_TIMEOUT;
    }

end_proc:
    dwTotalElapsedTime = ::GetTickCount() - dwTotalElapsedTime;

    if( dwRet == WAIT_TIMEOUT ) // 쩍?째짙 ??째첬
    {
        START_LOG( cerr, L"Event Processing - time over. elapsed : " << dwTotalElapsedTime )
            << BUILD_LOG( strLogin_ ) << END_LOG;

        // Event reset.
        ResetEvent( m_hEvents[1] );
        ResetEvent( m_hEvents[0] );
        ResetEvent( m_hEndEvent );
        m_kSock.CancelIo();
        m_kSock.OnSocketError();

        START_LOG( cerr, L"Reconnec To Auth Server..." ) << END_LOG;
        return false;
    }

    return true;
}

void KCnFBAuth::ProcessIntEvent( const KIntEvent& kIntEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &kIntEvent_.m_kbuff) );

    switch( kIntEvent_.m_usEventID )
    {
        INT_CASE( ENU_VERIFY_ACCOUNT_REQ );
       _INT_CASE( ENU_LOG_OUT_NOT, KECN_LOG_OUT_NOT );

    default:
        START_LOG( cerr, L"Invalid Event ID. ID : " << kIntEvent_.m_usEventID ) << END_LOG;
        break;
    }
    return;
}

bool KCnFBAuth::GetLoginResult( OUT SFBLogin& fbLogin_ )
{
    fbLogin_.Clear();
    std::string strResult = m_strResult;
    if( strResult.empty() ) {
        START_LOG( cerr, L"???천쩌짯쨔철쨌?쨘??? 쨔??쨘 ?짚쨘쨍째징 쩐첩쨀?." ) << END_LOG;
        return false;
    }

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(",;");
    tokenizer tokens( strResult, sep);
    tokenizer::iterator tok_iter = tokens.begin();

    if( tok_iter == tokens.end() ) return false;
    fbLogin_.m_strLoginID = *tok_iter;

    // Email ID 쨔?쩐?쩔?쨈? 쨘?쨘? ??째징.
    ++tok_iter;
    if( tok_iter == tokens.end() ) return false;
    fbLogin_.m_strEmailID = *tok_iter;

    ++tok_iter;
    if( tok_iter == tokens.end() ) return false;
    fbLogin_.m_strState = *tok_iter;

    ++tok_iter;
    if( tok_iter == tokens.end() ) return false;
    fbLogin_.m_nAccount = ::atoi( tok_iter->c_str() );
//    fbLogin_.m_bAccount = (::atoi( tok_iter->c_str() ) == 1 ? true:false);

    ++tok_iter;
    if( tok_iter == tokens.end() ) return false;
    fbLogin_.m_bAccessAllow = (::atoi( tok_iter->c_str() ) == 0 ? true:false);

    ++tok_iter;
    if( tok_iter == tokens.end() ) return false;
    fbLogin_.m_nBenefit = ::atoi( tok_iter->c_str() );

    return true;
}

bool KCnFBAuth::GetLogoutResult()
{
    if( m_strResult.empty() ) return false;
    START_LOG( cwarn, L"?? 쨔?쩍쨘 쨌?짹?쩐?쩔척 : " <<  KncUtil::toWideString( m_strResult ) ) << END_LOG;

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(",;");
    tokenizer tokens( m_strResult, sep);
    tokenizer::iterator tok_iter = tokens.begin();
    std::string strLogin;
    std::string strState;

    if( tok_iter == tokens.end() ) return false;
    strLogin = tok_iter->c_str();

    ++tok_iter;
    if( tok_iter == tokens.end() ) return false;
    strState = tok_iter->c_str();

    ++tok_iter;
    if( tok_iter == tokens.end() ) return false;

    return (::atoi( tok_iter->c_str() ) == 1);
}

bool KCnFBAuth::GetPostFix( IN const int nPF_, OUT std::string& strPF_ )
{
    strPF_.clear();
    switch ( nPF_ )
    {
    case FBPF_FUN_BOX:
        strPF_ = "FB";
        return true;
    case FBPF_PLAY_PARK:
        strPF_ = "PP";
        return true;
    }
    return false;
}

bool KCnFBAuth::GetPF_Login( IN std::string& strLoginPacket_, OUT std::string& strLogin_, OUT std::string& strPF_ )
{
    std::string strPacketLogin = strLoginPacket_;
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(".");
    tokenizer tokens( strPacketLogin, sep);
    tokenizer::iterator tok_iter = tokens.begin();

    if( tok_iter == tokens.end() ) return false;
    strLogin_ = tok_iter->c_str();

    ++tok_iter;
    if( tok_iter == tokens.end() ) return false;
    strPF_ = tok_iter->c_str();
    return true;
}

bool KCnFBAuth::CheckCorrectID( IN const std::string& strSendLogin_, IN const std::string& strPacketLogin_, IN const std::string& strAuthLogin_, IN const std::string& m_strEmailID_ )
{
    return ( boost::to_lower_copy( strSendLogin_ ) == boost::to_lower_copy( strAuthLogin_ ) ||
                boost::to_lower_copy( strSendLogin_ ) == boost::to_lower_copy( m_strEmailID_ ) ||
                    boost::to_lower_copy( strPacketLogin_ ) == boost::to_lower_copy( strAuthLogin_ ) ||
                        boost::to_lower_copy( strPacketLogin_ ) == boost::to_lower_copy( m_strEmailID_ ) );
}

INT_IMPL_ON_FUNC( ENU_VERIFY_ACCOUNT_REQ )
{
    DWORD dwElapsedTime = ::GetTickCount();
    InitResultData();
    // GameID, Login/out, LoginID, Password, IP, PostFix
    //(gc, i, funbox,ecasxcvveebexfjlopu,127.0.0.1,fb) ???천쩌짯쨔철쨌? 쨘쨍쨀쨩쨈? ?짚쨘쨍.
    // tester001.PP, i, 1, 0, 0  ???천쩌짯쨔철쨌?쨘??? 쨔?쨈? ?짚쨘쨍.
    // 쨔?쨈? ?짚쨘쨍 쩌첩쩌짯쨈챘쨌? ?짚??.
    // GameID
    // State ( Loing/Logout )
    // m_nAccount 째챔?짚 ?쨍?챌??쨈??철 쩐???쨈??철쩔징 쨈챘?? ?쩌?짤,
    // m_bAccessAllow 쨘챠쨌째/?찾쩔챘
    // m_nBenefit ?첵?? 1, 2

    boost::format fmterAuth( "gc,i,%s,%s,%s,%s;" );
    KENU_VERIFY_ACCOUNT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_strLogin = KncUtil::toWideString( kPacket_.m_strLogin );
    std::string strAuthString;
    SFBLogin    fbLogin;
    std::string strPF;
    std::wstring wstrAuthString;
    std::string strSendLogin;

    SET_ERROR( ERR_UNKNOWN );

    if( !GetPostFix( kPacket_.m_nFunBoxPostfix, strPF ) )
    {
        SET_ERR_GOTO( ERR_NUSER_VERIFY_22, end_proc ); // Post Fix 쩔?쨌첫.
    }

    // Request 쨔짰??쩔짯 쨩첵쩌쨘.
    {
        KLocker lock( ms_ComnData.m_csData );
        fmterAuth % kPacket_.m_strLogin
            % kPacket_.m_strPasswd
            % KncUtil::toNarrowString( kPacket_.m_strIP )
            % strPF;
    }
    strAuthString = fmterAuth.str();
    wstrAuthString = KncUtil::toWideString( strAuthString );
    START_LOG( clog, L" Auth String : " << wstrAuthString ) << END_LOG;

    // Request 쨔짰??쩔짯 ?체쩌?.
    ResetEvent( m_hEndEvent );
    LIF( m_kSock.SendData( strAuthString.c_str(), strAuthString.size() ) );
    // Ack 짹창쨈?쨍짼.
    LIF( WaitEvents( KncUtil::toWideString( kPacket_.m_strLogin ) ) );
    // ??쨈채 쩐챵쩐챤쩔?.
    if( !GetLoginResult( fbLogin ) )
    {
        SET_ERR_GOTO( ERR_NUSER_VERIFY_06, end_proc ); // ???천 쩍???
    }

    // ???천?쨀쨍짰 ok째짧쩔징 쨈챘?? ?쨀쨍짰.
    switch( fbLogin.m_nAccount ) {
        case 0:
            SET_ERR_GOTO( ERR_NUSER_VERIFY_10, end_proc ); // ?짼쨍째 Password.
            break;
        case 1:
            START_LOG( clog, L"TH Auth Success ok value : " << fbLogin.m_nAccount ) << END_LOG; // ???천 쩌쨘째첩.
            break;
        case -800:
            SET_ERR_GOTO( ERR_NUSER_VERIFY_23, end_proc ); // ??쨘쨔쨉? ??쨍???
            break;
        default:
            SET_ERR_GOTO( ERR_NUSER_VERIFY_11, end_proc ); // ?쨍?챌???철 쩐?쨈? 째챔?짚
            break;
    }

    if( !fbLogin.m_bAccessAllow ) {
        SET_ERR_GOTO( ERR_NUSER_VERIFY_21, end_proc ); // 쨘챠쨌째 쨉? ?짱?첬.
    }
    strSendLogin = kPacket_.m_strLogin;
    kPacket_.m_strLogin +=".";
    kPacket_.m_strLogin += strPF;

    if( !CheckCorrectID( strSendLogin, kPacket_.m_strLogin, fbLogin.m_strLoginID, fbLogin.m_strEmailID ) ) {
        // Event reset.
        ResetEvent( m_hEvents[1] );
        ResetEvent( m_hEvents[0] );
        ResetEvent( m_hEndEvent );
        m_kSock.CancelIo();
        m_kSock.OnSocketError();

        SET_ERR_GOTO( ERR_NUSER_VERIFY_06, end_proc ); // ???천 쩍???
    }

    kPacket_.m_strLogin = fbLogin.m_strLoginID; // ???천 쩌쨘째첩??쨍챕 ???천쩌짯쨔철쩔징쩌짯 쨔?쩐?쩔? 째짧?쨍쨌? 쨘짱째챈.
    kPacket_.m_nFunBoxBonus = fbLogin.m_nBenefit;

    SET_ERROR( NET_OK );
end_proc:
    kPacket.m_nOK = NetError::GetLastNetError();
    if( IS_CORRECT( NET_OK ) )
    {
        SiKCenterDBMgr()->QueueingEvent( KCenterEvent::DB_ENU_VERIFY_ACCOUNT_REQ, strLogin_.c_str(), 0, 0, kPacket_ );
    }
    else
    {
       SLayerType::GetInstance()->m_kActorManager2.QueueingEventTo( strLogin_, IDVenderType::ENU_VERIFY_ACCOUNT_ACK, kPacket );
    }

    dwElapsedTime = ::GetTickCount() - dwElapsedTime;

    START_LOG( clog, L"Fun Box Auth... Msg : " << NetError::GetErrStr( kPacket.m_nOK ) )
        << BUILD_LOG( KncUtil::toWideString( kPacket_.m_strLogin ) )
        << BUILD_LOG( kPacket.m_nOK )
        << BUILD_LOG( KncUtil::toWideString( fbLogin.m_strLoginID ) )
        << BUILD_LOG( KncUtil::toWideString( fbLogin.m_strEmailID ) )
        << BUILD_LOG( KncUtil::toWideString( fbLogin.m_strState ) )
        << BUILD_LOG( fbLogin.m_nAccount )
        << BUILD_LOG( fbLogin.m_bAccessAllow )
        << BUILD_LOG( fbLogin.m_nBenefit )
        << BUILD_LOG( KncUtil::toWideString( m_strResult ) )
        << BUILD_LOG( dwElapsedTime ) << END_LOG;
}

_INT_IMPL_ON_FUNC( ENU_LOG_OUT_NOT, KECN_LOG_OUT_NOT )
{
    InitResultData();
    std::string strLogin;
    std::string strPassword = KncUtil::toNarrowString(kPacket_.m_strPassword);
    std::string strIP       = KncUtil::DWORDToIPStr(kPacket_.m_dwIPNum);
    std::string strPF;
    std::string strLogoutString;
    bool bResult = false;
    
    if( !GetPF_Login( KncUtil::toNarrowString(kPacket_.m_strLogin), strLogin, strPF ) )
    {
		START_LOG( cerr, L"濡洹몄?? PF? Login ? ?살? 紐삵?.")
            << BUILD_LOG( kPacket_.m_strLogin )
            << BUILD_LOG( kPacket_.m_strPassword )
            << BUILD_LOG( kPacket_.m_dwUID )
            << BUILD_LOG( kPacket_.m_dwIPNum )
            << BUILD_LOG( kPacket_.m_dwAuthType ) << END_LOG;
        return;
    }

    //gc, o, funbox,ecasxcvveebexfjlopu,127.0.0.1,fb
    boost::format fmterAuth( "gc,o,%s,%s,%s,%s" );

    // Request 쨔짰??쩔짯 쨩첵쩌쨘.
    {
        KLocker lock( ms_ComnData.m_csData );
        fmterAuth % strLogin
            % strPassword
            % strIP
            % strPF;
    }
    strLogoutString = fmterAuth.str();
    ResetEvent( m_hEndEvent );
    LIF( m_kSock.SendData( strLogoutString.c_str(), strLogoutString.size() ) );
    LIF( WaitEvents( kPacket_.m_strLogin ) );
    bResult = GetLogoutResult();

    _LOG_CONDITION( bResult, clog, cwarn, L"쨌?짹? 쩐?쩔척 .. Name : " << kPacket_.m_strLogin )
        << BUILD_LOG( bResult )
        << BUILD_LOG( KncUtil::toWideString( m_strResult ) ) << END_LOG;
}