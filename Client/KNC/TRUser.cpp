#include "stdafx.h"
#include "TRUser.h"





//#define DEF_SERVER_SIDE

#ifdef DEF_SERVER_SIDE
#   include "TRServer.h"
#   include "TRUserFSM.h"
#endif

#ifdef PROXY_ONLY
#undef PROXY_ONLY
#endif
#define PROXY_ONLY assert( m_bIsProxy );

#ifdef UNPROXY_ONLY
#undef UNPROXY_ONLY
#endif
#define UNPROXY_ONLY assert( !m_bIsProxy );


const int nRECONNECT_TIME = 1000;

NiImplementRootRTTI( KTRUser );

KSocketObject* KTRUser::Create()
{
#ifdef DEF_SERVER_SIDE
    KTRUserPtr spTRUser = SiKTRServer()->m_kTRUserManager.CreateTRUser();
    if( spTRUser != NULL)
        return spTRUser->GetSockObj();
#endif
    return NULL;
}

KTRUser::KTRUser( bool bIsProxy )
:
    m_bIsProxy( bIsProxy ),
    m_dwUID( 0L ),
    m_bDestroyReserved( false ),
    m_bUseIocp( false ),
    m_iClassID( 0 ),
    m_dwPing( 0 ),
    m_dwPingUID( 0 ),
    m_dwPingSendGap( 1000 ),
    m_dwPingTimeoutLimit( 10000 ),
    m_dwPingContinuousRTOCount( 0 ),
    m_bPingRecv( true ),
    m_bConnectCheck( false ),
    m_bReConnect( false )
{
    for( int i = 0; i < TICKS_NUM; i++ )
    {
        m_adwTickCount[i] = ::GetTickCount();
    }

    m_kSockObj.m_pkTRUser = this;

    m_hEvents[EVENT_RECV_COMPLETED] = NULL;
    m_hEvents[EVENT_SEND_COMPLETED] = NULL;
}

KTRUser::~KTRUser(void)
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
        End( INFINITE );

        ::CloseHandle( m_hEvents[EVENT_RECV_COMPLETED] );
        ::CloseHandle( m_hEvents[EVENT_SEND_COMPLETED] );
    }
}

bool KTRUser::Init( bool bUseIocp_ )
{
    m_bUseIocp = bUseIocp_;

    if( !m_bUseIocp )
    {
        m_hEvents[EVENT_RECV_COMPLETED] = CreateEvent( NULL, false, false, NULL );
        m_kSockObj.SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[EVENT_RECV_COMPLETED] );
        m_hEvents[EVENT_SEND_COMPLETED] = CreateEvent( NULL, false, false, NULL );
        m_kSockObj.SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[EVENT_SEND_COMPLETED] );
    }    

    return true;
}

bool KTRUser::Connect( const char* szIP_, unsigned short usPort_, DWORD dwUID_ )
{
    PROXY_ONLY;

    std::vector< std::pair<int,int> > vecOpt;
    vecOpt.push_back( std::make_pair( SOL_SOCKET, SO_REUSEADDR ) ); // 사용중인 address를 재사용할 수 있게 허용. 
    vecOpt.push_back( std::make_pair( IPPROTO_TCP, TCP_NODELAY ) ); // Nagle option을 제거. 데이터를 모아서 보내지 않고 그 즉시 전송하도록 한다.

    _JIF( m_kSockObj.Connect( szIP_, usPort_, m_bUseIocp, &vecOpt ), return false );

    if( !m_bUseIocp )   // no use iocp
    {
        Begin();    // thread run
    }

    // 접속 후 즉시 UID를 등록한다.
    m_dwUID = dwUID_;

    KncSecurity::KByteStream buff;
    buff.Append( sizeof(USHORT) + sizeof(char) + sizeof(m_dwUID), sizeof(USHORT) );
    buff.Append( 0, sizeof(char) );
    buff.Append( m_dwUID );

    int nErrorCode = 0;
    LIF( m_kSockObj.SendData( (const char*)buff.data(), buff.length(), nErrorCode ) );

    //START_LOG( clog, "Send UID : " << m_dwUID );

    m_bPingRecv = true;
    m_bConnectCheck = true;
    m_bReConnect = false;

    return true;
}

bool KTRUser::Begin()
{
    if( KThread::Begin() )
    {
        m_hEvents[EVENT_DISCONNECTED] = m_hKillEvent;
        return true;
    }

    return false;
}

void KTRUser::Run()
{
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
        case EVENT_DISCONNECTED + WAIT_OBJECT_0:
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
        }

        Tick();
    }

}

void KTRUser::OnDestroy()
{
    m_kSockObj.CloseSocket();
    m_bDestroyReserved = false;     // 이전에 종료가 예약 되었든 아니든, 한번 Destory 호출이 성공하면 없어진다.

#ifdef DEF_SERVER_SIDE
    if( !m_bIsProxy )
    {
        switch( GetStateID() ) {
        case KTRUserFSM::STATE_INIT:
            StateTransition( KTRUserFSM::INPUT_CONNECT );
            StateTransition( KTRUserFSM::INPUT_EXIT );
            break;
        case KTRUserFSM::STATE_CONNECTED:
            StateTransition( KTRUserFSM::INPUT_EXIT );
        	break;
        case KTRUserFSM::STATE_REGISTERING:
            StateTransition( KTRUserFSM::INPUT_REG_UID_FAILED );
            StateTransition( KTRUserFSM::INPUT_EXIT );
            break;
        case KTRUserFSM::STATE_REGISTERED:
            StateTransition( KTRUserFSM::INPUT_EXIT );
            break;
        case KTRUserFSM::STATE_EXIT:
            break;
        }
    }
#endif

    if( !m_bUseIocp )    // 050323. florist. IOCP 사용 안할 경우, ReserveDestroy 호출하면 Thread 멈춘다.
    {
        SetEvent( m_hKillEvent );
    }
}

void KTRUser::Tick_Heartbit()
{
    if( ::GetTickCount() - GetTick( HB_TICK ) <= 15000 ) // 15s 단위로 heart-bit 전송
        return;

    //START_LOG( clog, L" Heart-bit " << m_dwUID << L"  :" << ::GetTickCount() - GetTick( HB_TICK ) );
    SetTick( HB_TICK );

    KncSecurity::KByteStream buff;
    buff.Append( sizeof(USHORT) + sizeof(char), sizeof(USHORT) );
    buff.Append( EVENTID_HEART_BIT, sizeof(char) );

    int nErrorCode = 0;
    LIF( m_kSockObj.SendData( (const char*)buff.data(), buff.length(), nErrorCode ) );
}

void KTRUser::Tick_Ping()
{
    // 틱이 지나야 체크를 한다.
    if( m_bPingRecv == false ) {
        DWORD dwCurrPing = ::GetTickCount() - GetTick( PING_TICK );
        if( dwCurrPing > m_dwPingTimeoutLimit ) {
            m_bPingRecv = true;
            SetPing( dwCurrPing );
            ++m_dwPingContinuousRTOCount;
        }
        else {
            return;
        }
    }

    if( ::GetTickCount() - GetTick( PING_TICK ) <= m_dwPingSendGap ) // 1s 단위로 heart-bit 전송
        return;

    ////START_LOG( clog, L" PING " << m_dwUID << L"  :" << ::GetTickCount() - GetTick( PING_TICK ) );
    SetTick( PING_TICK );

    SendPing();
    ////START_LOG( clog, "Send Ping : " << m_dwPingUID );
}

void KTRUser::Tick()
{
#ifdef DEF_SERVER_SIDE
    if( m_bIsProxy == false && GetStateID() == KTRUserFSM::STATE_EXIT )
    {
        //START_LOG( cout, "종료 상태에서 다시 Tick 으로 들어 왔다" );
        return ;
    }
#endif
    if( m_bDestroyReserved )    // 종료가 예약된 상태라면 다시한번 종료처리를 시도한다.
    {
        OnDestroy();            // derived class call
        return;
    }

    //////////////////////////////////////////////////////////////////////////
    // check heart bit
    if( m_bIsProxy )
    {
        if( m_kSockObj.IsConnected() == false )    // UserProxy의 경우, 연결이 없어도 살아있거나 혹은 재접속 할 수도 있다 (Robot)
            return;

        Tick_Ping();
        Tick_Heartbit();
    }

    else    // un-proxy heart-bit check
    {
#ifdef DEF_SERVER_SIDE
        switch( GetStateID() )
        {
        case KTRUserFSM::STATE_CONNECTED:

            if( ::GetTickCount() - GetTick( ENUM_TICKS::CONNECT_TICK ) > 5000) // Connect 이후 5초간 UID등록이 없다.
            {
                //START_LOG( cwarn, L"Reguard " << m_dwUID 
                    << L" as a Zombie.(Reason : Not Regist UID) (tick: " 
                    << ::GetTickCount() - GetTick( ENUM_TICKS::CONNECT_TICK ) 
                    << L" )" );

                ReserveDestroy();
            }
            break;

        case KTRUserFSM::STATE_REGISTERING:

            if( !SiKTRServer()->m_kTRUserManager.Regist( this ) )     // 등록한다.
            {
                StateTransition( KTRUserFSM::INPUT_REG_UID_FAILED );    // 등록 실패시 Connected 상태로 되돌아감.왜 돌아가지??
                ReserveDestroy();                                       // 왜돌아 가지?? 여기서 종료 예약 하면 되는데..(로그도 않남고.)
            }
            else
                StateTransition( KTRUserFSM::INPUT_END_REG_UID );
            break;

        case KTRUserFSM::STATE_REGISTERED:
            if( ::GetTickCount() - GetTick( ENUM_TICKS::HB_TICK ) > 30000 ) // 30s 단위로 heart-bit check
            {
                //START_LOG( cwarn, L"Reguard " << m_dwUID 
                    //<< L" as a Zombie. (tick: " 
                    //<< ::GetTickCount() - GetTick( ENUM_TICKS::HB_TICK ) 
                    //<< L")" );

                ReserveDestroy(); 
            }

            break;

        case KTRUserFSM::STATE_EXIT:
            break;
        } // end switch
#endif

    } //end if .. else
}

void KTRUser::OnSocketError( int nType )
{
    m_bDestroyReserved = true;
}

void KTRUser::OnAcceptConnection()
{
    UNPROXY_ONLY;
    SetTick( CONNECT_TICK );
}

void KTRUser::ProcSubtype( IN KSerBuffer* pBuff )
{
    DWORD dwSubtype = 0;
    memcpy( &dwSubtype, pBuff->GetData(), sizeof(dwSubtype) );

    switch( dwSubtype ) {
        case SUBTYPE_PING:
            {
                DWORD dwTime = 0;
                DWORD dwPingUID = 0;
                memcpy( &dwPingUID, (BYTE*)pBuff->GetData() + sizeof(DWORD), sizeof(DWORD) );
                memcpy( &dwTime, (BYTE*)pBuff->GetData() + sizeof(DWORD)*2, sizeof(DWORD) );

                if( dwPingUID < m_dwPingUID - 1 ) {
                    ////START_LOG( cerr, "tcp relay ping : "<<::timeGetTime() - dwTime << ", ping UID : " << dwPingUID << " ( Request Timed Out )" );
                }
                else {
                    SetPing( ::timeGetTime() - dwTime );
                    ////START_LOG( clog, "tcp relay ping : "<<GetPing()<< ", ping UID : " << dwPingUID );
                    m_bPingRecv = true;
                    m_dwPingContinuousRTOCount = 0;
                }
            }
            break;
        default:
            {
                ::PostMessage( m_hWnd, WM_APP + m_iClassID, 0, reinterpret_cast<LPARAM>(pBuff) );
            }
            break;
    }
}

void KTRUser::OnRecvCompleted( IN const char cRecvCnt_, IN KSerBuffer& buff_ )
{
    switch( cRecvCnt_ ) {
    case EVENTID_RELAYED:   // relay. 윈도우 메시지로 데이터를 던진다.
        PROXY_ONLY;

        if( IsWindow( m_hWnd ) )
        {

            // SendMessage하면 메세지가 처리될때까지 기다리지만, PostMessage이므로 큐잉만 한다.
            // cmd : WM_APP + class ID
            // wParam : not use (0)
            // lParam : data 의 포인터

            KSerBuffer* pBuff = new KSerBuffer( buff_ );
            ProcSubtype( pBuff );
        }
        break;
    case EVENTID_HEART_BIT: // heart bit.
        UNPROXY_ONLY;
    	break;
    case EVENTID_REG_UID:   // Regist UID. TRUserManager에 등록도 해야 한다.
        {
            UNPROXY_ONLY;
            DWORD dwUID;
            buff_.Read( &dwUID, sizeof( DWORD) );

            if( m_dwUID != 0 )
            {
                //START_LOG( cerr, L"Duplicate UID regist. Current UID : " << m_dwUID 
                    //<< L"Recv UID : " << dwUID << dbg::endl );
                break;
            }

            if( dwUID <= 0 )
            {
                //START_LOG( cerr, L"Recv Invalid UID: " << dwUID ); 
                break;
            }

            m_dwUID = dwUID;
            //START_LOG( clog, "Regist UID : " << m_dwUID );

#ifdef DEF_SERVER_SIDE
            StateTransition( KTRUserFSM::INPUT_START_REG_UID );
#endif
        }        
        break;
    default:
        if( cRecvCnt_ < 0 ) // 위에서 필터링 되지 않은 음수 영역. 문제가 있다.
        {
            //START_LOG( cerr, "Invalide cRecvCnt field value. data size : " << buff_.GetLength() )
                //<< BUILD_LOGc( cRecvCnt_ )
                //<< END_LOG;
            return;
        }
#ifdef DEF_SERVER_SIDE
        if( buff_.GetLength() < cRecvCnt_ * sizeof(DWORD) )
        {
            //START_LOG( cerr, "Invalide packet received. data size : " << buff_.GetLength() )
                //<< BUILD_LOGc( cRecvCnt_ )
                //<< END_LOG;
            OnSocketError( KEVENT_CLOSE_CONNECTION_NOT::SOCKET_ERROR_TRUSER );
        }

        KRelayDataPtr spData( new KRelayData );
        spData->m_cRecvCnt = cRecvCnt_;
        
        // cRecvCnt_에 담긴 수만큼 DWORD를 읽어낸다.
        spData->m_vecUID.resize( cRecvCnt_ );
        for( int i = 0 ; i < cRecvCnt_ ; ++i )
        {
            buff_.Read( &spData->m_vecUID[i], sizeof(DWORD) );
        }

        spData->m_buff.resize( buff_.GetReadLength() );
        buff_.Read( &spData->m_buff[0], buff_.GetReadLength() );

        SiKTRServer()->QueueingRelayData( spData );
#endif
    }

    // renew m_dwHBRecvTick
    if( !m_bIsProxy )
        SetTick( HB_TICK );
}


void KTRUser::KSkTRUser::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {                    
        //START_LOG( clog, L"closed by remote machine" );
        OnSocketError( KEVENT_CLOSE_CONNECTION_NOT::SOCKET_ERROR_TRUSER );
        return;
    }

    USHORT usPacketLen = 0;

    m_ovlRecv.m_dwLeft += dwTransfered_;
    
    // 패킷 길이 진행량보다 크다면 계속 새 패킷 가능성을 체크한다.
    while( m_ovlRecv.m_dwLeft >= sizeof(usPacketLen) )
    {
        // 패킷 길이를 알아낸다.
        memcpy( &usPacketLen, m_ovlRecv.m_pBuffer, sizeof(usPacketLen) );

        // 패킷은 최소 3byte 이상의 길이를 가져야 한다.
        if( usPacketLen < MINIMUM_PACKET_SIZE )
        {
            //START_LOG( cerr, "invalid packet size - " << usPacketLen );
            OnSocketError( KEVENT_CLOSE_CONNECTION_NOT::SOCKET_ERROR_TRUSER );
            return;
        }

        // 버퍼에 받아둔 데이터로 완전한 packet이 가능한 경우 다음으로 진행한다.
        if( m_ovlRecv.m_dwLeft >= usPacketLen )
        {
            // get recvCnt
            char cRecvCnt = -99; // 사용하지 않는 값으로 초기화.
            memcpy( &cRecvCnt, &m_ovlRecv.m_pBuffer[ sizeof(usPacketLen) ], sizeof(cRecvCnt) );

            // get (recvUIDn, data)
            KSerBuffer buffer;
            buffer.Write( &m_ovlRecv.m_pBuffer[MINIMUM_PACKET_SIZE], usPacketLen - MINIMUM_PACKET_SIZE );

            m_pkTRUser->OnRecvCompleted( cRecvCnt, buffer );

            // buffer management
            m_ovlRecv.m_dwLeft -= usPacketLen;
            memmove(&m_ovlRecv.m_pBuffer[0], &m_ovlRecv.m_pBuffer[usPacketLen], m_ovlRecv.m_dwLeft );            

        } // if( m_ovlRecv.m_dwLeft >= dwHeadPacketLen )    // 완전한 패킷이 가능한 경우

        else break;     // 온전한 패킷을 구성하기에 충분한 데이터가 없음. loop를 빠져나간다.

    } // while( m_ovlRecv.m_dwLeft >= sizeof(DWORD) )

    // recv function call
    LIF( InitRecv() );
}

bool KTRUser::Send( IN const std::vector<unsigned char>& buff_, int& nErrorCode )
{
    UNPROXY_ONLY;

    KncSecurity::KByteStream buff;
    // 패킷 총 사이즈 + 릴레이 패킷을 의미하는 recvCnt + 전송할 데이터
    buff.Append( sizeof(USHORT) + sizeof(char) + buff_.size(), sizeof(USHORT) );
    buff.Append( EVENTID_RELAYED, sizeof(char) );
    buff.append( &buff_[0] , buff_.size() );

    return m_kSockObj.SendData( (const char*)buff.data(), buff.length(), nErrorCode );
}

bool KTRUser::Send( IN const std::vector<DWORD>& vecUID_, IN const void* pvData_, IN const size_t dwDataSize_, int& nErrorCode, DWORD dwSubType /*= SUBTYPE_NORMAL*/ )
{
    PROXY_ONLY;

    if( vecUID_.empty() ) 
    {
        nErrorCode = KSocketObject::EM_SEND_FAIL_TARGET;
        return false;
    }
    //if( dwDataSize_ > KSkTRUser::MAXIMUM_PACKET_SIZE ) return false;

    KncSecurity::KByteStream buff;
    buff.Append( sizeof(USHORT)                 // 패킷 총 사이즈
        + sizeof(char)                          // recvCnt (전송대상자 수)
        + sizeof(m_dwUID) * vecUID_.size()      // 전송받은 상대의 UIDs...
        + sizeof(DWORD)                         // SubType
        + dwDataSize_                           // 전송할 데이터
        , sizeof(USHORT) );
    
    buff.Append( vecUID_.size(), sizeof(char) );

    std::vector<DWORD>::const_iterator vit;
    for( vit = vecUID_.begin(); vit != vecUID_.end(); vit++ )
    {
        buff.Append( *vit );
    }

    buff.Append( dwSubType );
    buff.append( (byte*)pvData_, dwDataSize_ );

    return m_kSockObj.SendData( (const char*)buff.data(), buff.length(), nErrorCode );
}

bool KTRUser::SendPing()
{
    DWORD dwTime = ::timeGetTime();
    //START_LOG( clog, "Send Ping : " << m_dwUID );
    KncSecurity::KByteStream buff;
    buff.Append( sizeof(USHORT)                 // 패킷 총 사이즈
        + sizeof(char)                          // recvCnt (전송대상자 수)
        + sizeof(m_dwUID)                       // 전송받은 상대의 UIDs...
        + sizeof(DWORD)                         // SubType
        + sizeof(DWORD)*2                         // 전송할 데이터
        , sizeof(USHORT) );

    buff.Append( 1, sizeof(char) );
    buff.Append( m_dwUID );

    DWORD dwSubType = SUBTYPE_PING;
    buff.Append( dwSubType );
    buff.Append( m_dwPingUID++ );
    buff.Append( dwTime );

    int nErrorCode = 0;
    bool bRet = m_kSockObj.SendData( (const char*)buff.data(), buff.length(), nErrorCode );

    if( bRet ) {
        m_bPingRecv = false;
    }

    return bRet;
}

bool KTRUser::IsConnectCheck()
{
    // 최초 한번만 체크하기 위해서
    if( !m_bConnectCheck )
        return false;

    m_bConnectCheck = false;
    m_bReConnect = true;

    m_dwDisConnectTick = GetTickCount();

    return true;
}

bool KTRUser::IsReConnectCheck()
{
    if ( !m_bReConnect )
        return false;

    DWORD dwTick = GetTickCount();

    //  1초 지나야지 재접하게 한다. 
    if ( dwTick - m_dwDisConnectTick >= nRECONNECT_TIME )
    {
        m_bReConnect = false;
        return true;
    }

    return false;
}