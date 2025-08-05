#include "stdafx.h"
#include "SocketObject.h"


#ifdef KNC_USE_IOCP

#endif




KSocketObject::KSocketObject() : m_sock( INVALID_SOCKET ), m_dwKey( 0 )
{
    m_sockaddr.sin_family   = AF_INET;

    m_ovlRecv.m_eIOMode     = KOVERLAPPED::IO_RECV;
    m_ovlSend.m_eIOMode     = KOVERLAPPED::IO_SEND;

    m_ovlRecv.hEvent        = NULL;
    m_ovlSend.hEvent        = NULL;

    ::InitializeCriticalSection( &m_csSend );
}

void KSocketObject::SetIOEvent( KOVERLAPPED::ENUM_IO_MODE eMode_, HANDLE hEvent_ )
{
    // 060227. florist. 이전엔 GetIOEvent라는 함수를 제공해 내부에서 이벤트 객체를 생성했었다.
    // 하지만 WaitFor...하는 부분은 SocketObject 외부에 있기 때문에, 종료처리시 스레드가 아직 돌 때
    // 이벤트를 먼저 지워버리면 문제가 생긴다. 이때문에 코드가 좀 길어지더라도 스레드가 있는 코드에서
    // 이벤트 객체를 생성 & 종료하도록 변경한다. SetIOEvent는 외부에서 새성된 Event를 받아오기만 한다.

    LPWSAOVERLAPPED povl    = NULL;

    switch( eMode_ )
    {
    case KOVERLAPPED::IO_SEND: povl = &m_ovlSend;   break;
    case KOVERLAPPED::IO_RECV: povl = &m_ovlRecv;   break;
    default:
        return;
    }

    povl->hEvent = hEvent_;
}


KSocketObject::~KSocketObject(void)
{
    CLOSE_SOCKET( m_sock );

    ::DeleteCriticalSection( &m_csSend );
}

void KSocketObject::CloseSocket()
{
    CLOSE_SOCKET( m_sock );
}

bool KSocketObject::Connect( const char* szIP_, 
                             unsigned short usPort_, 
                             bool bUseIocp_,
                             std::vector< std::pair<int,int> >* vecOpt_ )
{
    if( IsConnected() )
    {
        START_LOG( cwarn, L"Already Connected. Key : " << m_dwKey );
        return true;
    }

    int            ret;   

    m_sock = ::WSASocket( AF_INET, 
        SOCK_STREAM, 
        IPPROTO_TCP, 
        NULL,
        0, 
        WSA_FLAG_OVERLAPPED ); // overlapped I/O

    if( m_sock == INVALID_SOCKET )
    {
        START_LOG( cerr, GET_WSA_MSG )
            << BUILD_LOG( WSAGetLastError() )
            << END_LOG;
        return false;
    }

    if( vecOpt_ != NULL )
    {
        int sock_opt = 1;
        std::vector< std::pair<int,int> >::iterator vit;
        for( vit = vecOpt_->begin(); vit != vecOpt_->end(); vit++ )
        {
            if( ::setsockopt( m_sock, vit->first, vit->second, (char *)&sock_opt,	sizeof (sock_opt) ) == SOCKET_ERROR )
            {
                START_LOG( cerr, GET_WSA_MSG )
                    << BUILD_LOG( WSAGetLastError() )
                    << END_LOG;
                return false;
            }            
        }
    }

    { // locking scope
        
        if( szIP_ )         SetRemoteIP( szIP_ );
        if( usPort_ != 0 )  SetRemotePort( usPort_ );

        ret = ::connect( m_sock, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr) );

        if( SOCKET_ERROR == ret )
        {
            // MUST: 간혹 다른 컴(회의실)에서 WSAENOBUFS란 Error를 유발할 경우가 존재한다.
            // TCP의 buffer space가 모자랄 경우인 것 같다.
            // client가 100개 이면 뒤에 5개 정도가 이 error를 발생시킨다.
            // 나중에 수정되어야 할 것 이다.
            START_LOG( cerr, GET_WSA_MSG )
                << BUILD_LOG( WSAGetLastError() )
                << BUILD_LOG( m_dwKey )
                << BUILD_LOG( szIP_ )
                << BUILD_LOG( usPort_ )
                << END_LOG;

            CLOSE_SOCKET( m_sock );
            return false;
        }

        if( bUseIocp_ )
        {
#ifdef KNC_USE_IOCP
            _JIF( SiKIocp()->AssociateSocket( this ), return false );
#endif
        }

    } // locking scope

    _JIF( InitRecv(), return false );

    return  true;
}

bool KSocketObject::InitRecv()
{
    if( m_sock == INVALID_SOCKET ) return false;

    DWORD dwRead = 0;
    DWORD dwFlag = 0;    

    // Note: Win2000에서는 아래의 WSARecv() 대신 ReadFile()을 사용할 수 있다.
    //       Win98에서는 ReadFile()을 사용할 수 없다.

    m_ovlRecv.Clear();

    m_ovlRecv.m_wsaBuffer.buf = &m_ovlRecv.m_pBuffer[m_ovlRecv.m_dwLeft];
    m_ovlRecv.m_wsaBuffer.len = MAX_PACKET_SIZE - m_ovlRecv.m_dwLeft;

    int ret = ::WSARecv( m_sock,                    // socket
        &m_ovlRecv.m_wsaBuffer,     // buffer pointer, size
        1,                          // lpBuffers 배열에 있는 WSABUF 구조체의 수.
        &dwRead,                    // I/O 작업이 끝나자마자 보내진 바이트 수
        &dwFlag,                    // [in,out] Option Flag
        &m_ovlRecv,                 // struct LPWSAOVERLAPPED
        NULL);                      // Recv 완료시 호출될 함수 포인터

    if( SOCKET_ERROR == ret )
    {
        switch( ::GetLastError() ) {
        case WSA_IO_PENDING:    // the overlapped operation has been successfully initiated            
            return true;

        case WSAECONNRESET:     // The virtual circuit was reset by the remote side.
            START_LOG( cwarn, L"The virtual circuit was reset by the remote side." );
            OnSocketError( KEVENT_CLOSE_CONNECTION_NOT::SOCKET_ERROR_WSAECONNRESET );
            return false;

        default:
            START_LOG( cerr, L"WSARecv() Failed." )
                << BUILD_LOG( m_dwKey )
                << BUILD_LOG( GET_WSA_MSG )
                << BUILD_LOG( ret );
            OnSocketError( KEVENT_CLOSE_CONNECTION_NOT::SOCKET_ERROR_DEFAULT );
            return false;
        }
    } // if

    // If no error occurs and the receive operation has completed immediately, WSARecv returns zero.
    if( ret == 0 )
    {        
        return true;
    }
    return false;
}

void KSocketObject::OnIOCompleted( KOVERLAPPED::ENUM_IO_MODE eMode_ )
{
    DWORD dwTransfered      = 0;
    DWORD dwFlag            = 0;
    LPWSAOVERLAPPED povl    = NULL;

    switch( eMode_ )
    {
    case KOVERLAPPED::IO_SEND: povl = &m_ovlSend;   break;
    case KOVERLAPPED::IO_RECV: povl = &m_ovlRecv;   break;
    default:
        return;
    }

    ::WSAGetOverlappedResult( m_sock, povl, &dwTransfered, FALSE, &dwFlag );

    if( eMode_ == KOVERLAPPED::IO_SEND )
        OnSendCompleted( dwTransfered );
    else
        OnRecvCompleted( dwTransfered );
}

void KSocketObject::OnSendCompleted( DWORD dwTransfered_ )
{
    if( m_sock == INVALID_SOCKET ) return;

    KLocker lock( m_csSend );

    m_ovlSend.m_bSending    = false;    // 050127. 다 보냈는지 여부를 떠나서 항상 false. 재전송할 경우 아래에서 다시 true해줌.

    if( dwTransfered_ >= m_ovlSend.m_dwLeft )   // 요청된 데이터를 모두 다 보냄
    {
        m_ovlSend.m_dwLeft      = 0;        
        return;
    } 

    int ret = SOCKET_ERROR;

    // 데이터를 덜 보냄 : 다시 전송을 신청 
    m_ovlSend.m_dwLeft -= m_ovlSend.InternalHigh;
    memmove( &m_ovlSend.m_pBuffer[0], &m_ovlSend.m_pBuffer[dwTransfered_], m_ovlSend.m_dwLeft );

    DWORD dwWrite = 0;
    m_ovlSend.m_wsaBuffer.len = m_ovlSend.m_dwLeft;

    ret = ::WSASend( m_sock,                     // socket
        &m_ovlSend.m_wsaBuffer,     // 버퍼 포인터
        1,                          // WSABUF의 갯수
        &dwWrite,                   // [out] 즉시 전송된 량
        0,                          // Flag
        &m_ovlSend,                 // OVERLAPPED
        NULL );

    if( ret == SOCKET_ERROR && ::WSAGetLastError() == ERROR_IO_PENDING )    // 현재 pending 중..
    {
        m_ovlSend.m_bSending   = true;            
        return;
    } 

    if( ret != SOCKET_ERROR ) // 호출 하자 마자 다 보냄.
    {
        m_ovlSend.m_bSending   = true;
        return;
    }

    START_LOG( cwarn, L"WSASend Failed. LastError : " << GET_WSA_MSG );

    OnSocketError( KEVENT_CLOSE_CONNECTION_NOT::SOCKET_ERROR_SEND );

    return;
}

bool KSocketObject::SendData( const char* szData_, int iSize_, int& nErrorCode )
{
    nErrorCode = EM_SEND_SUCCESS;

    _JIF( m_sock != INVALID_SOCKET, nErrorCode = EM_SEND_FAIL_CONNECT; return false );    // 소켓이 유효하지 않음.
    _JIF( iSize_ > 0, nErrorCode = EM_SEND_FAIL_SIZE; return false );                  // 0 byte 전송 시도도 에러처리.
    _JIF( szData_ != NULL, nErrorCode = EM_SEND_FAIL_NODATA; return false );

    KLocker lock( m_csSend );

    if( m_ovlSend.m_dwLeft + iSize_ > MAX_PACKET_SIZE )
    {
        START_LOG( cwarn, L"Send Buffer Full. data size : " << iSize_
            << L", left : " << m_ovlSend.m_dwLeft
            << L", bSending : " << m_ovlSend.m_bSending );

        // 데이터를 보내지 않았음. 다시 보내려면 SendPakcet 한번 더 호출해야 함.

        m_ovlSend.m_dwLeft = 0;
        m_ovlSend.m_bSending = false;

        OnSocketError( KEVENT_CLOSE_CONNECTION_NOT::SOCKET_ERROR_BUFFSIZE );
        
        nErrorCode = EM_SEND_FAIL_BUFFSIZE;


        return false;
    }

    ::memmove(&m_ovlSend.m_pBuffer[m_ovlSend.m_dwLeft], szData_, iSize_ );
    m_ovlSend.m_dwLeft += iSize_;

    // 전송중이지 않으면 다시한번 WsaSend를 호출해준다.
    if( m_ovlSend.m_bSending == false )
    {
        DWORD dwWrite = 0;
        m_ovlSend.m_wsaBuffer.len = m_ovlSend.m_dwLeft;

        int ret = ::WSASend( m_sock,                     // socket
            &m_ovlSend.m_wsaBuffer,     // 버퍼 포인터
            1,                          // WSABUF의 갯수
            &dwWrite,                   // [out] 즉시 전송된 량
            0,                          // Flag
            &m_ovlSend,                 // OVERLAPPED
            NULL );

        //std::cout << "raw length : " << kbuff_.GetLength() << std::endl << "SEND " << bsbuff << std::endl;

        if( ret == SOCKET_ERROR && ::WSAGetLastError() == ERROR_IO_PENDING )    // 현재 pending 중..
        {
            m_ovlSend.m_bSending   = true;            
            return true;
        } 

        if( ret != SOCKET_ERROR ) // 호출 하자 마자 다 보냄.
        {
            m_ovlSend.m_bSending   = true;
            return true;
        }

        START_LOG( cwarn, L"WSASend Failed. LastError : " << GET_WSA_MSG );

        OnSocketError( KEVENT_CLOSE_CONNECTION_NOT::SOCKET_ERROR_SEND );

        nErrorCode = EM_SEND_FAIL_SENDERROR;
        return false;
    }   

    return true;
}

void KSocketObject::OnRecvCompleted( DWORD dwTransfered )
{
	START_LOG( cout, L"Pure Virtual Function Call... Check Please...." );
}

void KSocketObject::OnSocketError( int nType )
{
	START_LOG( cout, L"Pure Virtual Function Call... Check Please...." );
}