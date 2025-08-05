#include "TRUser.h"
#include <cassert>
#include <KncSecurity/KncSecurity.h>
#include "TREvent.h"
#include "RelayData.h"

#define DEF_SERVER_SIDE

#ifdef DEF_SERVER_SIDE
#   include "TRServer.h"
#   include "TRUserFSM.h"
#endif



#define PROXY_ONLY assert( m_bIsProxy );
#define UNPROXY_ONLY assert( !m_bIsProxy );

NiImplementRootRTTI( KTRUser );

int KTRUser::m_nLimitPacketSendCount = 1000;

KTRUserPtr KTRUser::Create()
{
#ifdef DEF_SERVER_SIDE
    KTRUserPtr spTRUser = SiKTRServer()->m_kTRUserManager.CreateTRUser();
    if( spTRUser != NULL) {
        return spTRUser;
    }
#endif
    return KTRUserPtr();
}


void KTRUser::AddManager( KTRUserPtr spTRUser_ )
{

}

KTRUser::KTRUser( bool bIsProxy ) : KSession ( false ),

m_bIsProxy( bIsProxy ),
m_dwUID( 0L ),
m_bDestroyReserved( false ),
m_bUseIocp( false ),
m_iClassID( 0 ),
m_nPacketSendCount( 0 ),
m_dwTickGap( 1000 * 60 )
{
    for( int i = 0; i < TICKS_NUM; i++ )
    {
        m_adwTickCount[i] = ::GetTickCount();
    }

    //m_kSockObj = GetSockObj();
    m_kSockObj.m_pkTRUser = this;

    m_hEvents[EVENT_RECV_COMPLETED] = NULL;
    m_hEvents[EVENT_SEND_COMPLETED] = NULL;
    SetSendQueueSize( 512 * 1024 ); // 32k 추가 버퍼 사용.
    m_dwConnectTime = ::timeGetTime();
    m_dwConnectTick = ::GetTickCount(); // 첫 생성시 Tick
}

KTRUser::~KTRUser(void)
{
    m_kSockObj.CloseSocket();

    if( m_bUseIocp )
    {
#ifdef KNC_USE_IOCP
        //SiKIocp()->DeleteCompKey( m_kSockObj.KSocketObject::GetKey() );
        //DeleteCompKey();
#endif
    }
    else
    {   
        End();

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

    _JIF( m_kSockObj.KSocketObject::Connect( szIP_, usPort_, m_bUseIocp, &vecOpt ), return false );

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
    LIF( m_kSockObj.SendData( (const char*)buff.data(), buff.length() ) );

    START_LOG( clog, L"Send UID : " << m_dwUID ) << END_LOG;

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
    KSession::OnDestroy();
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
    //ResetTRUser();
    if( m_kSockObj.GetKey() != 0 )
    {
        SiKIocp()->DeleteCompKey( m_kSockObj.GetKey() );
        m_kSockObj.SetKey( 0 );
    }
    DWORD dwDurationTime = ( ::timeGetTime() - m_dwConnectTime ) / 1000; // 초로 환산.
    START_LOG( clog, L"TRUser PacketCnt : " << m_nPacketSendCount )
        << BUILD_LOG( dwDurationTime )
        << END_LOG;

    
}

void KTRUser::Tick()
{
#ifdef DEF_SERVER_SIDE
    if( m_bIsProxy == false && GetStateID() == KTRUserFSM::STATE_EXIT )
    {
        START_LOG( cerr, L"종료 상태에서 다시 Tick 으로 들어 왔다" ) << END_LOG;
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
        if( m_kSockObj.KSocketObject::IsConnected() == false )    // UserProxy의 경우, 연결이 없어도 살아있거나 혹은 재접속 할 수도 있다 (Robot)
            return;

        if( ::GetTickCount() - GetTick( HB_TICK ) <= 15000 ) // 15s 단위로 heart-bit 전송
            return;

        START_LOG( clog, L" Heart-bit " << m_dwUID << L"  :" << ::GetTickCount() - GetTick( HB_TICK ) ) << END_LOG;
        SetTick( HB_TICK );

        KncSecurity::KByteStream buff;
        buff.Append( sizeof(USHORT) + sizeof(char), sizeof(USHORT) );
        buff.Append( EVENTID_HEART_BIT, sizeof(char) );
        LIF( m_kSockObj.SendData( (const char*)buff.data(), buff.length() ) );
    }

    else    // un-proxy heart-bit check
    {
#ifdef DEF_SERVER_SIDE
        switch( GetStateID() )
        {
        case KTRUserFSM::STATE_CONNECTED:

            if( ::GetTickCount() - GetTick( CONNECT_TICK ) > 30000 ) // Connect 이후 5초간 UID등록이 없다.
            {
                START_LOG( cwarn, L"Regard " << m_dwUID 
                    << L" as a Zombie.(Reason : Not Regist UID) (tick: " 
                    << ::GetTickCount() - GetTick( CONNECT_TICK ) 
                    << L" )" ) 
                    << END_LOG;

                ReserveDestroy();
                SiKTRServer()->m_kTRUserManager.IncreaseTypeStatCount( KTRUser::STAT_FAIL_SEND_USERUID, 1 );
            }
            break;

        case KTRUserFSM::STATE_REGISTERING:

            if( !SiKTRServer()->m_kTRUserManager.Regist( GetThisPtr() ) )     // 등록한다.
            {
                StateTransition( KTRUserFSM::INPUT_REG_UID_FAILED );    // 등록 실패시 Connected 상태로 되돌아감.왜 돌아가지??
                ReserveDestroy();                                     // 종료 예약처리 일단 false. 기존 연결된 것이 Tick에서 처리될때까지 기다린다.
                SiKTRServer()->m_kTRUserManager.IncreaseTypeStatCount( KTRUser::STAT_FAIL_UID_REGISTERED, 1 );
            }
            else
                StateTransition( KTRUserFSM::INPUT_END_REG_UID );
            break;

        case KTRUserFSM::STATE_REGISTERED:

            if( ::GetTickCount() - GetTick( HB_TICK ) > 120000 ) // 120s 단위로 heart-bit check
            {
                START_LOG( cwarn, L"Regard " << m_dwUID 
                    << L" as a Zombie. (tick: " 
                    << ::GetTickCount() - GetTick( HB_TICK ) 
                    << L")" )
                    << END_LOG;

                ReserveDestroy();
                SiKTRServer()->m_kTRUserManager.IncreaseTypeStatCount( KTRUser::STAT_USER_ZOMBIE_STATE, 1 );
            }

            break;

        case KTRUserFSM::STATE_EXIT:
            break;
        } // end switch
#endif

    } //end if .. else
}

void KTRUser::OnSocketError()
{
    KSession::OnSocketError();
    m_bDestroyReserved = true;
}

void KTRUser::OnAcceptConnection()
{
    UNPROXY_ONLY;
    SetTick( CONNECT_TICK );
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
            ::PostMessage( m_hWnd, WM_APP + m_iClassID, 0, reinterpret_cast<LPARAM>(pBuff) );
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
                START_LOG( cerr, L"Duplicate UID regist. Current UID : " << m_dwUID 
                    << L"Recv UID : " << dwUID ) << END_LOG;
                break;
            }

            if( dwUID <= 0 )
            {
                START_LOG( cerr, L"Recv Invalid UID: " << dwUID ) << END_LOG; 
                break;
            }

            m_dwUID = dwUID;
            START_LOG( clog, L"Regist UID : " << m_dwUID ) << END_LOG;

#ifdef DEF_SERVER_SIDE
            StateTransition( KTRUserFSM::INPUT_START_REG_UID );
#endif
        }        
        break;
    default:
        if( cRecvCnt_ < 0 ) // 위에서 필터링 되지 않은 음수 영역. 문제가 있다.
        {
            START_LOG( cerr, L"Invalide cRecvCnt field value. data size : " << buff_.GetLength() )
                << BUILD_LOGc( cRecvCnt_ )
                << END_LOG;
            return;
        }
#ifdef DEF_SERVER_SIDE
        if( buff_.GetLength() < cRecvCnt_ * sizeof(DWORD) )
        {
            START_LOG( cerr, L"Invalide packet received. data size : " << buff_.GetLength() )
                << BUILD_LOGc( cRecvCnt_ )
                << END_LOG;
            OnSocketError();
        }

        KRelayDataPtr spData( new KRelayData );
        spData->m_cRecvCnt = cRecvCnt_;

        // cRecvCnt_에 담긴 수만큼 DWORD를 읽어낸다.
        spData->m_vecUID.resize( cRecvCnt_ );
        for( int i = 0 ; i < cRecvCnt_ ; ++i )
        {
            buff_.Read( &spData->m_vecUID[i], sizeof(DWORD) );
            SiKTRServer()->m_kTRUserManager.IncreaseTypeStatCount( KTRUser::STAT_RELAY_COUNT, 1 ); // Increase Relay Count.
        }

        spData->m_buff.resize( buff_.GetReadLength() );
        buff_.Read( &spData->m_buff[0], buff_.GetReadLength() );

        SiKTRServer()->QueueingRelayData( spData );
        ++m_nPacketSendCount;
        CheckPacketSendCount(); // 분당 패킷 보내는 부분 체크.
        SiKTRServer()->m_kTRUserManager.IncreaseTypeStatCount( KTRUser::STAT_RECV_COUNT, 1 ); // Increase Recv Count.
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
        START_LOG( clog, L"closed by remote machine" ) << END_LOG;
        OnSocketError();
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
        if( usPacketLen < KTRUser::MINIMUM_PACKET_SIZE ) //if( usPacketLen <= MINIMUM_PACKET_SIZE )
        {
            START_LOG( cerr, L"invalid packet size - " << usPacketLen ) << END_LOG;
            OnSocketError();
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
            if ( usPacketLen - KTRUser::MINIMUM_PACKET_SIZE > 0 ) {
                buffer.Write( &m_ovlRecv.m_pBuffer[KTRUser::MINIMUM_PACKET_SIZE], usPacketLen - KTRUser::MINIMUM_PACKET_SIZE );
            }


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

bool KTRUser::Send( IN const std::vector<unsigned char>& buff_ )
{
    UNPROXY_ONLY;

    KncSecurity::KByteStream buff;
    // 패킷 총 사이즈 + 릴레이 패킷을 의미하는 recvCnt + 전송할 데이터
    buff.Append( sizeof(USHORT) + sizeof(char) + buff_.size(), sizeof(USHORT) );
    buff.Append( EVENTID_RELAYED, sizeof(char) );
    buff.append( &buff_[0] , buff_.size() );

    return m_kSockObj.SendData( (const char*)buff.data(), buff.length() );
}

bool KTRUser::Send( IN const std::vector<DWORD> vecUID_, IN const void* pvData_, IN const size_t dwDataSize_ )
{
    PROXY_ONLY;

    if( vecUID_.empty() ) return false;
    if( dwDataSize_ > MAXIMUM_PACKET_SIZE ) return false;

    KncSecurity::KByteStream buff;
    buff.Append( sizeof(USHORT)                 // 패킷 총 사이즈
        + sizeof(char)                          // recvCnt (전송대상자 수)
        + sizeof(m_dwUID) * vecUID_.size()      // 전송받은 상대의 UIDs...
        + dwDataSize_                           // 전송할 데이터
        , sizeof(USHORT) );

    buff.Append( vecUID_.size(), sizeof(char) );

    std::vector<DWORD>::const_iterator vit;
    for( vit = vecUID_.begin(); vit != vecUID_.end(); ++vit )
    {
        buff.Append( *vit );
    }

    buff.append( (byte*)pvData_, dwDataSize_ );

    return m_kSockObj.SendData( (const char*)buff.data(), buff.length() );
}

void KTRUser::SetKey( DWORD dwKey )
{ 
    m_kSockObj.SetKey( dwKey ); 
}

void KTRUser::CheckPacketSendCount()
{
    if( m_nPacketSendCount < m_nLimitPacketSendCount ) {
        return;
    }

    if( ::GetTickCount() - m_dwConnectTick < m_dwTickGap ) {
        SiKTRServer()->m_kTRUserManager.IncreaseTypeStatCount( KTRUser::STAT_PACKET_COUNT_OVER, 1 );
    }
    // 통계후 변수 초기화.
    m_nPacketSendCount = 0;
    m_dwConnectTick = ::GetTickCount();
}