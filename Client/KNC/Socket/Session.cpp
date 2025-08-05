#include "stdafx.h"
#include "Session.h"






#ifdef KNC_USE_IOCP

#endif

#define SEND_IMITATION_PACKET( id, buff ) \
do { \
    unsigned short      usEventID = id; \
    KSerBuffer          kbuffSend; \
    int                 nfrom = 0; \
    ks.BeginWriting( &kbuffSend ); \
    ks.Put( usEventID ); \
    ks.Put( buff ); \
    ks.Put( nfrom ); \
    ks.EndWriting(); \
    LIF( SendPacket( kbuffSend, L#id ) ); \
} while(false)

NiImplementRTTI( KSession, KPerformer );

#ifdef PROXY_ONLY
#undef PROXY_ONLY
#endif
#define PROXY_ONLY cassert( m_bIsProxy )(m_strName).fatal()

#ifdef UNPROXY_ONLY
#undef UNPROXY_ONLY
#endif
#define UNPROXY_ONLY cassert( !m_bIsProxy )(m_strName).fatal()

KSession::KMaxPacketInfo KSession::ms_kMaxPacketInfo = {
    KncCriticalSection(), L"초기상태", L"초기상태", 0, 0, CTime::GetCurrentTime()
};

DWORD   KSession::m_dwHBGap = 30000;

KSession::KSession( bool bIsProxy ) 
:
    m_bIsProxy          ( bIsProxy ),
    m_bUseIocp          ( true ),
    m_bDestroyReserved  ( false ),
    m_nSPIndex          ( 0 ),
    m_bAuthKeyRecved    ( false ),
    m_bCheckHBTick      ( true ),
    m_bZombieDestroy    ( false )
{
    m_dwHBTick = GetTickCount();

    m_kSockObj.m_pkSession = this;

    m_hEvents[EVENT_RECV_COMPLETED] = NULL;
    m_hEvents[EVENT_SEND_COMPLETED] = NULL;
}

KSession::~KSession(void)
{
    m_kSockObj.CloseSocket();

    if( m_bUseIocp )
    {
#ifdef KNC_USE_IOCP
        SiKIocp()->DeleteCompKey( &m_kSockObj );
#endif
    }
    else
    {   
        End();  // thread use

        ::CloseHandle( m_hEvents[EVENT_RECV_COMPLETED] );
        ::CloseHandle( m_hEvents[EVENT_SEND_COMPLETED] );
    }

    if( m_nSPIndex != 0 )
    {
        START_LOG( clog, L"SA 삭제. Name : " << m_strName << L", SPI : " << m_nSPIndex );
        KncSecurity::GetSADB().Delete( m_nSPIndex );
        m_nSPIndex = 0;
    }

}

ImplToStringW( KSession )
{
    return START_TOSTRING_PARENTW( KPerformer )
        << TOSTRINGW( m_dwHBTick )
        << TOSTRINGW( m_bDestroyReserved )
        << TOSTRINGW( m_nSPIndex )
        << TOSTRINGW( m_bIsProxy )
        << TOSTRINGW( m_bUseIocp )
        << TOSTRINGW( m_bCheckHBTick );
}

bool KSession::Init( bool bUseIocp_  )
{
    PROXY_ONLY;
    m_bUseIocp  = bUseIocp_;

    if( !m_bUseIocp )
    {
        m_hEvents[EVENT_RECV_COMPLETED] = CreateEvent( NULL, false, false, NULL );
        m_kSockObj.SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[EVENT_RECV_COMPLETED] );
        m_hEvents[EVENT_SEND_COMPLETED] = CreateEvent( NULL, false, false, NULL );
        m_kSockObj.SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[EVENT_SEND_COMPLETED] );
    }    

    return true;
}

bool KSession::Connect( const char* szIP_, unsigned short usPort_, bool bNagleAlgOn )
{
    PROXY_ONLY;

    std::vector< std::pair<int,int> > vecOpt;
    vecOpt.push_back( std::make_pair( SOL_SOCKET, SO_REUSEADDR ) ); // 사용중인 address를 재사용할 수 있게 허용. 

    if( !bNagleAlgOn )
        vecOpt.push_back( std::make_pair( IPPROTO_TCP, TCP_NODELAY ) ); // Nagle option을 제거. 데이터를 모아서 보내지 않고 그 즉시 전송하도록 한다.

    START_LOG( clog, L"Connect To : " << szIP_ << L":" << usPort_ );

    _JIF( m_kSockObj.Connect( szIP_, usPort_, m_bUseIocp, &vecOpt ), return false );

    if( !m_bUseIocp )   // no use iocp
    {
        Begin();    // thread run
    }

    // 050520. 키를 받아 설정할 때까지 대기한다.
    DWORD dwWaitStartTick = ::GetTickCount();

    Sleep( 20 );

    // 대기시간을 초과하지 않았고 받은 메세지가 아무것도 없는 동안은
    while( ::GetTickCount() - dwWaitStartTick < SECURE_KEY_WAITING_TIME && !m_bAuthKeyRecved )
        Sleep( 20 );    // 계속 기다린다.

    if( !m_bAuthKeyRecved )  // 키를 받지 못했다면
    {
        START_LOG( cerr, L"서버에서 보안 키를 받지 못했습니다. name:" << m_strName << L", 대기시간:" 
            << ::GetTickCount() - dwWaitStartTick );
        ReserveDestroy();
        return false;
    }

    return true;
}

void KSession::OnAcceptConnection()
{
    UNPROXY_ONLY;

    const KncSecurity::KSecurityAssociation* pkSA = NULL;
    SpiType nSpiNew = 0;
    pkSA = KncSecurity::GetSADB().CreateNewSA( nSpiNew );

    KSerializer ks;
    KSerBuffer  kbuff;
    ks.BeginWriting( &kbuff );
    ks.Put( nSpiNew );
    ks.Put( *pkSA );
    ks.EndWriting();

    SEND_IMITATION_PACKET( EVENT_ACCEPT_CONNECTION_NOT, kbuff );    // 상수키로 보낸 후,
    m_nSPIndex = nSpiNew;   // 새 키로 변경해준다.
}

void KSession::OnDestroy()
{
    m_kSockObj.CloseSocket();
    m_bAuthKeyRecved    = false;    // 070307. kkurrung. 소켓을 끊을때 인증키 받은 정보를 초기화 해준다. 다음에 다시 연결 할수 있도록.
    m_bDestroyReserved  = false;    // 이전에 종료가 예약 되었든 아니든, 한번 Destory 호출이 성공하면 없어진다.

    // 050323. floris. IOCP 사용 안할 경우, ReserveDestroy 호출하면 Thread 멈춘다.
    if( !m_bUseIocp ) 
    {
        SetEvent( m_hKillEvent );
    }

    if( !m_queEvent.empty() )   // 050328. 아직 처리되지 않은 이벤트들은 처리해준다.
        KPerformer::Tick();
}

void KSession::KSkSession::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {                    
        START_LOG( clog, L"closed by remote machine" );
        OnSocketError( KEVENT_CLOSE_CONNECTION_NOT::SOCKET_ERROR_RECVCOMP );
        return;
    }

    USHORT          usPacketLen = 0;

    m_ovlRecv.m_dwLeft += dwTransfered_;
    
    //if( m_ovlRecv.m_dwLeft >= MAX_PACKET_SIZE )
    //{
    //    START_LOG( cwarn, "Recv Buffer Full. " )
    //        << BUILD_LOG( MAX_PACKET_SIZE )
    //        << BUILD_LOG( m_ovlRecv.m_dwLeft )
    //        << BUILD_LOG( m_strName );
    //    return;
    //} 
    
    // 패킷 길이 진행량보다 크다면 계속 새 패킷 가능성을 체크한다.
    while( m_ovlRecv.m_dwLeft >= sizeof(usPacketLen) )
    {
        // 패킷 길이를 알아낸다.
        memcpy( &usPacketLen, m_ovlRecv.m_pBuffer, sizeof(usPacketLen) );

        // 버퍼에 받아둔 데이터로 완전한 packet이 가능한 경우 다음으로 진행한다.
        if( m_ovlRecv.m_dwLeft >= usPacketLen )
        {
            // 길이를 제외한 데이터만 버퍼에 담는다.
            KncSecurity::KSecureBuffer kSecBuff( m_pkSession->m_nSPIndex, m_ovlRecv.m_pBuffer + sizeof(usPacketLen),
                usPacketLen - sizeof(usPacketLen) );
            //std::cout << "RECV " << kSecBuff << std::endl;
            if( !kSecBuff.IsAuthentic() )
            {
                START_LOG( cerr, "packet authentication failed." )
                    << BUILD_LOG( kSecBuff.GetSize() )
                    << BUILD_LOG( m_pkSession->GetName() )
                    << BUILD_LOG( m_pkSession->m_nSPIndex );
                return;
            }

            // 인증 및 복호화 처리를 마치고 순수 payload만을 얻어낸다.
            KncSecurity::KByteStream payload;
            JIF( kSecBuff.GetPayload( payload ) );

            m_pkSession->OnRecvCompleted( payload );

            // buffer management
            m_ovlRecv.m_dwLeft -= usPacketLen;
            memmove(&m_ovlRecv.m_pBuffer[0], &m_ovlRecv.m_pBuffer[usPacketLen], m_ovlRecv.m_dwLeft );            

        } // if( m_ovlRecv.m_dwLeft >= dwHeadPacketLen )    // 완전한 패킷이 가능한 경우

        else break;     // 온전한 패킷을 구성하기에 충분한 데이터가 없음. loop를 빠져나간다.
    
    } // while( m_ovlRecv.m_dwLeft >= sizeof(DWORD) )

    // recv function call
    LIF( InitRecv() );
}

void KSession::OnRecvCompleted( IN const KncSecurity::KByteStream& payload_ )
{
    KSerializer     ks;
    KSerBuffer      kbuff;

    // payload를 KEvent 형식으로 전환한다.
    kbuff.Write( payload_.data(), payload_.length() );
    KEventPtr spEvent( CreateKEvent() );
    ks.BeginReading( &kbuff );
    ks.Get( *spEvent );
    ks.EndReading();

    // 압축 된 경우만 자동으로 해제한다.
    spEvent->m_kbuff.UnCompress();

    switch( spEvent->m_usEventID )
    {
    case EVENT_HEART_BIT:                   // heart bit filtering - no queueing
        UNPROXY_ONLY;
        break;
    case EVENT_ACCEPT_CONNECTION_NOT:
        {
            PROXY_ONLY;

            KncSecurity::KSecurityAssociation kSA;

            ks.BeginReading( &spEvent->m_kbuff );
            ks.Get( m_nSPIndex );
            ks.Get( kSA );
            ks.EndReading();

            KncSecurity::GetSADB().Insert( m_nSPIndex, kSA );
            START_LOG( clog, m_strName << L", 보안키 설정. spi:" << m_nSPIndex  );

            m_bAuthKeyRecved = true;
        }
        break;
    default:
        QueueingEvent( spEvent, KEvent::FT_OUTTER );
    }

    // renew m_dwHBRecvTick
    if( !m_bIsProxy )
        m_dwHBTick = ::GetTickCount();
}

bool KSession::SendPacket( KSerBuffer& kbuff_, const wchar_t* szDataDesc_ )
{
    if( m_bDestroyReserved ) return true;       ///< 종료가 예약되었을때 send 시도. 이런 경우가 다분하므로, true 인정.
    _JIF( m_kSockObj.IsConnected(), return false );    ///< 소켓이 유효하지 않음.

    // 암호화한다.
    KncSecurity::KByteStream    bsbuff;
    KncSecurity::KSecureBuffer  kSecBuff( m_nSPIndex );
    
    bsbuff.assign( (byte*)kbuff_.GetData(), kbuff_.GetLength() );   // 실제 전송할 데이터 넣기.
    _JIF( kSecBuff.Create( bsbuff ), return false );                       // 암호화 및 인증 절차 처리.

    // bsbuff를 재활용해, 암호화된 데이터 앞에 총 길이를 덧붙인 byte stream을 만든다.
    bsbuff.clear();
    bsbuff.reserve( kSecBuff.GetSize() + sizeof(USHORT) );   // 패킷 총 사이즈[4] + 전송할 데이터[가변]
    bsbuff.Assign( kSecBuff.GetSize() + sizeof(USHORT), sizeof(USHORT) );
    bsbuff.append( (byte*)kSecBuff.GetDataPtr(), kSecBuff.GetSize() );

    {   // locking scope

        KLocker lock2( ms_kMaxPacketInfo.m_cs );

        if( ms_kMaxPacketInfo.m_nPacketSize < bsbuff.length() )
        {
            ms_kMaxPacketInfo.m_strName     = m_strName;
            // 050930. florist. std::wstring에 NULL이 assign되면 출력시 runtime-error가 발생함을 테스트 코드로 확인했다.
            ms_kMaxPacketInfo.m_strDataDesc = ( szDataDesc_ != NULL ) ? szDataDesc_ : L"NULL" ;
            ms_kMaxPacketInfo.m_cTime       = CTime::GetCurrentTime();
            ms_kMaxPacketInfo.m_nDataSize   = kbuff_.GetLength();
            ms_kMaxPacketInfo.m_nPacketSize = bsbuff.length();
            START_LOG( clog2, L"패킷 최대사이즈 갱신. Name : " << m_strName )
                << BUILD_LOG( ms_kMaxPacketInfo.m_strDataDesc )
                << BUILD_LOG( ms_kMaxPacketInfo.m_nDataSize )
                << BUILD_LOG( ms_kMaxPacketInfo.m_nPacketSize );
        }
    
    }   // locking scope

    int nErrorCode = 0;
    return m_kSockObj.SendData( (const char*)bsbuff.data(), (int)bsbuff.length(), nErrorCode );
}


bool KSession::Begin()  // KThread의 이벤트 객체를 배열에 보관해, WaitMultipleObject를 사용한다.
{
    if( KThread::Begin() )
    {
        m_hEvents[EVENT_DISCONNECTED] = m_hKillEvent;
        return true;
    }

    return false;
}

void KSession::Run()
{
#if defined( BUG_TRAP )
	BT_SetTerminate();
#endif

    PROXY_ONLY;

    DWORD ret       = NULL;
    bool bLoop      = true;

    while( bLoop )
    {
        ret = ::WaitForMultipleObjects( EVENT_MAX_VALUE,
            m_hEvents, 
            false, 
            100 );     // 0.1s 간격

        switch( ret )
        {
        case EVENT_DISCONNECTED + WAIT_OBJECT_0:    // 스레드 종료
            bLoop = false;
            break;
        case EVENT_RECV_COMPLETED + WAIT_OBJECT_0:
            {
                m_kSockObj.OnIOCompleted( KOVERLAPPED::IO_RECV );
            }
            break;
        case EVENT_SEND_COMPLETED + WAIT_OBJECT_0:
            {
                m_kSockObj.OnIOCompleted( KOVERLAPPED::IO_SEND );
            }
            break;
        case WAIT_TIMEOUT:
            break;

        case WAIT_FAILED:
            cassert( false )( GET_WSA_MSG ).fatal();
            break;
        }

        Tick(); // 050214. IOCP를 사용하지 않을 경우, Tick을 내부에서 처리한다.

    } // while

}

void KSession::Tick()
{
    KPerformer::Tick();

    //////////////////////////////////////////////////////////////////////////
    // 종료 처리
    if( m_bDestroyReserved )    // 종료가 예약된 상태라면 다시한번 종료처리를 시도한다.
    {
        OnDestroy();            // derived class call
        return;
    }

    //////////////////////////////////////////////////////////////////////////
    // check heart bit
    if( m_bIsProxy )
    {
        if( IsConnected() == false )    // UserProxy의 경우, 연결이 없어도 살아있거나 혹은 재접속 할 수도 있다 (Robot)
            return;

        if( ::GetTickCount() - m_dwHBTick <= 15000 ) // 15s 단위로 heart-bit 전송
            return;

        if( !m_bAuthKeyRecved )   // 아직 보안키를 받기 이전이다. 이땐 heart-bit를 보내지 않는다.
            return;

        //START_LOG( clog, L" Heart-bit " << m_strName << L"  :" << ::GetTickCount() - m_dwHBTick );
        m_dwHBTick = ::GetTickCount();

        KSerializer ks;
        KSerBuffer  kbuff;
        SEND_IMITATION_PACKET( EVENT_HEART_BIT, kbuff );
    }

    else    // un-proxy heart-bit check
    {
        if( m_bCheckHBTick && ::GetTickCount() - m_dwHBTick > KSession::m_dwHBGap ) // 30s 단위로 heart-bit check
        {
            START_LOG( cwarn, L"Reguard " << m_strName 
                << L" as a Zombie. (tick: " 
                << ::GetTickCount() - m_dwHBTick 
                << L" )" );

            m_bZombieDestroy = true; // 060913. 순서는 OnDestroy 이전에 세세팅
            OnDestroy();
        }
    }
}

void KSession::OnSocketError( int nType )
{    
    m_bDestroyReserved = true;
}

void KSession::DumpMaxSendData( std::wostream& stm_ )
{
    KLocker lock( ms_kMaxPacketInfo.m_cs );

    stm_ << L" -- Max size Data sending -- " << std::endl
         << L"  user name   : " << ms_kMaxPacketInfo.m_strName << std::endl
         << L"  data desc   : " << ms_kMaxPacketInfo.m_strDataDesc << std::endl
         << L"  data size   : " << ms_kMaxPacketInfo.m_nDataSize 
         << L" (" << ms_kMaxPacketInfo.m_nDataSize / 1024.0f << " Kb)" << std::endl
         << L"  packet size : " << ms_kMaxPacketInfo.m_nPacketSize
         << L" (" << ms_kMaxPacketInfo.m_nPacketSize / 1024.0f << " Kb)" << std::endl
         << L"  send time   : " << (LPCWSTR)ms_kMaxPacketInfo.m_cTime.Format( KNC_TIME_FORMAT ) << std::endl;
}

void KSession::ResetMaxSendData()
{
    KLocker lock( ms_kMaxPacketInfo.m_cs );

    ms_kMaxPacketInfo.m_strDataDesc.clear();
    ms_kMaxPacketInfo.m_nDataSize = 0;
    ms_kMaxPacketInfo.m_nPacketSize = 0;
    ms_kMaxPacketInfo.m_cTime = CTime::GetCurrentTime();
    ms_kMaxPacketInfo.m_strName = (LPCWSTR)ms_kMaxPacketInfo.m_cTime.Format( KNC_TIME_FORMAT );
    ms_kMaxPacketInfo.m_strName.append( L" 에 데이터 초기화 되었습니다." );
}

bool KSession::SendID( unsigned short usEventID_ )
{
    // check valide state
    KEventPtr spEvent( CreateKEvent() );
    spEvent->m_usEventID = usEventID_;

    // make event object
    KSerializer         ks;

    // serialize - full event object
    KSerBuffer          kbuff;
    ks.BeginWriting( &kbuff );
    ks.Put( *spEvent );
    ks.EndWriting();

    // send packet
    if( ! KSession::SendPacket( kbuff, spEvent->GetEventIDString() ) )
        return false;

    dbg::clog << L"ⓢ " << spEvent->GetEventIDString() << L" (name:" << m_strName << L")" << END_LOG;

    return true;
}

bool KSession::CheckExceedRefCount( int nCount_ )
{
    // 입력받은 수와 같거나 적은 경우는 문제를 일으키지 않으므로, true를 리턴한다.
    // false가 리턴된 경우, 로그출력 등의 추가작업을 위해선 child 클래스에서 상속받아 구현한다.
    return ( GetRefCount() <= nCount_ );
}
