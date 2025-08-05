//#define NOMINMAX
#include "SocketObject.h"
#include "Socket/NetCommon.h"

#ifdef KNC_USE_IOCP
#include "Iocp.h"
#endif

#include <dbg/dbg.hpp>
#include <thread/Locker.h>
#include "Log4.h"
//FILE_NAME_FOR_LOG

KSocketObject::KMaxSizeInfo KSocketObject::ms_kMaxSizeInfo = {
	KncCriticalSection(), 0, 0, CTime::GetCurrentTime()
};

void KSocketObject::KMaxSizeInfo::Check(int iDataSize, int iQueueSize)
{
	KLocker lock(m_cs);

	if (iDataSize > m_iMaxDataSize)
	{
		m_iMaxDataSize = iDataSize;
		m_iQueueSize = iQueueSize;
		m_cTime = CTime::GetCurrentTime();

		START_LOG(cerr, L"Send-Queue log update.")
			<< BUILD_LOG(m_iMaxDataSize)
			<< BUILD_LOG(m_iQueueSize)
			<< BUILD_LOG((LPCTSTR)m_cTime.Format(KNC_TIME_FORMAT)) << END_LOG;
	}
}

void KSocketObject::KMaxSizeInfo::Dump(std::wostream& stm)
{
	KLocker lock(m_cs);

	stm << L" -> Send Q. log " << std::endl
		<< TOSTRINGW(m_iMaxDataSize)
		<< TOSTRINGW(m_iQueueSize)
		<< TOSTRINGW((LPCTSTR)m_cTime.Format(KNC_TIME_FORMAT));
}

KSocketObject::KSocketObject() : m_sock(INVALID_SOCKET), m_dwKey(0)
{
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_addr.s_addr = ::inet_addr("192.0.0.1");
	m_sockaddr.sin_port = ::htons(0);

	for (int i = 0; i < 8; ++i) {
		m_sockaddr.sin_zero[i] = 0;
	}

	m_ovlRecv.m_eIOMode = KOVERLAPPED::IO_RECV;
	m_ovlSend.m_eIOMode = KOVERLAPPED::IO_SEND;

	m_ovlRecv.hEvent = NULL;
	m_ovlSend.hEvent = NULL;
	m_bSendBufferFull = false;
}

void KSocketObject::SetIOEvent(KOVERLAPPED::ENUM_IO_MODE eMode_, HANDLE hEvent_)
{
	// 060227. florist. 이전엔 GetIOEvent라는 함수를 제공해 내부에서 이벤트 객체를 생성했었다.
	// 하지만 WaitFor...하는 부분은 SocketObject 외부에 있기 때문에, 종료처리시 스레드가 아직 돌 때
	// 이벤트를 먼저 지워버리면 문제가 생긴다. 이때문에 코드가 좀 길어지더라도 스레드가 있는 코드에서
	// 이벤트 객체를 생성 & 종료하도록 변경한다. SetIOEvent는 외부에서 새성된 Event를 받아오기만 한다.

	LPWSAOVERLAPPED povl = NULL;

	switch (eMode_)
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
	CLOSE_SOCKET(m_sock);
}

void KSocketObject::CloseSocket()
{
	CLOSE_SOCKET(m_sock);
}

bool KSocketObject::Connect(const char* szIP_,
	unsigned short usPort_,
	bool bUseIocp_,
	std::vector< std::pair<int, int> >* vecOpt_)
{
	if (IsConnected())
	{
		START_LOG(cwarn, L"Already Connected. Key : " << m_dwKey) << END_LOG;
		return true;
	}

	int            ret;

	m_sock = ::WSASocket(AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP,
		NULL,
		0,
		WSA_FLAG_OVERLAPPED); // overlapped I/O

	if (m_sock == INVALID_SOCKET)
	{
		START_LOG(cerr, GET_WSA_MSG)
			<< BUILD_LOG(WSAGetLastError())
			<< END_LOG;
		return false;
	}

	if (vecOpt_ != NULL)
	{
		int sock_opt = 1;
		std::vector< std::pair<int, int> >::iterator vit;
		for (vit = vecOpt_->begin(); vit != vecOpt_->end(); vit++)
		{
			if (::setsockopt(m_sock, vit->first, vit->second, (char *)&sock_opt, sizeof(sock_opt)) == SOCKET_ERROR)
			{
				START_LOG(cerr, GET_WSA_MSG)
					<< BUILD_LOG(WSAGetLastError())
					<< END_LOG;
				return false;
			}
		}
	}

	{ // locking scope
		if (szIP_)         SetRemoteIP(szIP_);
		if (usPort_ != 0)  SetRemotePort(usPort_);

		ret = ::connect(m_sock, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr));

		if (SOCKET_ERROR == ret)
		{
			// MUST: 간혹 다른 컴(회의실)에서 WSAENOBUFS란 Error를 유발할 경우가 존재한다.
			// TCP의 buffer space가 모자랄 경우인 것 같다.
			// client가 100개 이면 뒤에 5개 정도가 이 error를 발생시킨다.
			// 나중에 수정되어야 할 것 이다.
			START_LOG(cerr, GET_WSA_MSG)
				<< BUILD_LOG(WSAGetLastError())
				<< BUILD_LOG(m_dwKey)
				//<< BUILD_LOG( szIP_ )
				<< BUILD_LOG(usPort_)
				<< END_LOG;

			CLOSE_SOCKET(m_sock);
			return false;
		}

		// IOCP를 사용하지 않으면 (Gash, NCash...)
		// 연결 즉시 InitRecv를 호출하도록 한다.
		// IOCP 를 사용하면 Key 등록 먼저 하고, InitRecv호출하도록 한다.
		if (!bUseIocp_)
		{
			_JIF(InitRecv(), return false);
		}
	} // locking scope

	return  true;
}

bool KSocketObject::InitRecv()
{
	if (m_sock == INVALID_SOCKET) return false;

	DWORD dwRead = 0;
	DWORD dwFlag = 0;

	// Note: Win2000에서는 아래의 WSARecv() 대신 ReadFile()을 사용할 수 있다.
	//       Win98에서는 ReadFile()을 사용할 수 없다.

	m_ovlRecv.Clear();

	m_ovlRecv.m_wsaBuffer.buf = &m_ovlRecv.m_pBuffer[m_ovlRecv.m_dwLeft];
	m_ovlRecv.m_wsaBuffer.len = MAX_PACKET_SIZE - m_ovlRecv.m_dwLeft;

	int ret = ::WSARecv(m_sock,                    // socket
		&m_ovlRecv.m_wsaBuffer,     // buffer pointer, size
		1,                          // lpBuffers 배열에 있는 WSABUF 구조체의 수.
		&dwRead,                    // I/O 작업이 끝나자마자 보내진 바이트 수
		&dwFlag,                    // [in,out] Option Flag
		&m_ovlRecv,                 // struct LPWSAOVERLAPPED
		NULL);                      // Recv 완료시 호출될 함수 포인터

	if (SOCKET_ERROR == ret)
	{
		switch (::GetLastError()) {
		case WSA_IO_PENDING:    // the overlapped operation has been successfully initiated
			return true;

		case WSAECONNRESET:     // The virtual circuit was reset by the remote side.
			START_LOG(cwarn, L"The virtual circuit was reset by the remote side.") << END_LOG;
			OnSocketError();
			return false;

		default:
			START_LOG(cerr, L"WSARecv() Failed.")
				<< BUILD_LOG(m_dwKey)
				<< BUILD_LOG(GET_WSA_MSG)
				<< BUILD_LOG(ret) << END_LOG;
			OnSocketError();
			return false;
		}
	} // if

	// If no error occurs and the receive operation has completed immediately, WSARecv returns zero.
	if (ret == 0)
	{
		return true;
	}
	return false;
}

void KSocketObject::OnIOCompleted(KOVERLAPPED::ENUM_IO_MODE eMode_)
{
	DWORD dwTransfered = 0;
	DWORD dwFlag = 0;
	LPWSAOVERLAPPED povl = NULL;

	switch (eMode_)
	{
	case KOVERLAPPED::IO_SEND: povl = &m_ovlSend;   break;
	case KOVERLAPPED::IO_RECV: povl = &m_ovlRecv;   break;
	default:
		return;
	}

	::WSAGetOverlappedResult(m_sock, povl, &dwTransfered, FALSE, &dwFlag);

	if (eMode_ == KOVERLAPPED::IO_SEND)
		OnSendCompleted(dwTransfered);
	else
		OnRecvCompleted(dwTransfered);
}

void KSocketObject::MovDataQue2Buf(OUT int& nBeforeSize_, OUT int& nAfterSize_)
{
	KLocker lock(m_csSock);
	KLocker lock2(m_csSendQueue);

	nBeforeSize_ = 0;
	nAfterSize_ = 0;
	int nSize = MAX_PACKET_SIZE - m_ovlSend.m_dwLeft;
	nSize = m_kSendQueue.Pop(nSize, &m_ovlSend.m_pBuffer[m_ovlSend.m_dwLeft]);
	nBeforeSize_ = m_ovlSend.m_dwLeft; // 버퍼
	m_ovlSend.m_dwLeft += nSize;
	nAfterSize_ = m_ovlSend.m_dwLeft;
}

void KSocketObject::OnSendCompleted(DWORD dwTransfered_)
{
	KLocker lock(m_csSock);

	if (m_sock == INVALID_SOCKET) return;

	m_ovlSend.m_bSending = false;    // 050127. 다 보냈는지 여부를 떠나서 항상 false. 재전송할 경우 아래에서 다시 true해줌.

	if (dwTransfered_ >= m_ovlSend.m_dwLeft)   // 요청된 데이터를 모두 다 보냄
	{
		m_ovlSend.m_dwLeft = 0;
	}
	else
	{
		m_ovlSend.m_dwLeft -= m_ovlSend.InternalHigh;
		memmove(&m_ovlSend.m_pBuffer[0], &m_ovlSend.m_pBuffer[dwTransfered_], m_ovlSend.m_dwLeft);
	}

	int nBeforeSize, nAfterSize; // 로그 추가를 위하여 변수 추가.
	MovDataQue2Buf(nBeforeSize, nAfterSize);   // send-queue에 대기중인 데이터가 있다면 버퍼로 옮긴다.

	if (m_ovlSend.m_dwLeft == 0)   // 보내야 할 데이터가 더이상 남아있지 않다면 리턴.
		return;

	m_ovlSend.m_wsaBuffer.len = m_ovlSend.m_dwLeft;

	DWORD dwWrite = 0;

	int ret = ::WSASend(m_sock,                     // socket
		&m_ovlSend.m_wsaBuffer,     // 버퍼 포인터
		1,                          // WSABUF의 갯수
		&dwWrite,                   // [out] 즉시 전송된 량
		0,                          // Flag
		&m_ovlSend,                 // OVERLAPPED
		NULL);

	if (ret == SOCKET_ERROR && ::WSAGetLastError() == ERROR_IO_PENDING)    // 현재 pending 중..
	{
		m_ovlSend.m_bSending = true;
		return;
	}

	if (ret != SOCKET_ERROR) // 호출 하자 마자 다 보냄.
	{
		m_ovlSend.m_bSending = true;
		return;
	}

	START_LOG(cwarn, L"WSASend Failed. LastError : " << GET_WSA_MSG) << END_LOG;

	OnSocketError();

	return;
}

bool KSocketObject::SendData(const char* szData_, int iSize_)
{
	KLocker lock(m_csSock);

	_JIF(m_sock != INVALID_SOCKET, return false);    // 소켓이 유효하지 않음.
	_JIF(iSize_ > 0, return false);                  // 0 byte 전송 시도도 에러처리.
	_JIF(szData_ != NULL, return false);

	int nBeforeSize, nAfterSize; // 로그 추가를 위하여 변수 추가.
	MovDataQue2Buf(nBeforeSize, nAfterSize);   // send-queue에 대기중인 데이터가 있다면 버퍼로 옮긴다.

	// input data의 처리 - 1. 버퍼에 빈 공간이 있다면 우선 복사 시도.
	int iFreeSpace = MAX_PACKET_SIZE - m_ovlSend.m_dwLeft;
	if (iFreeSpace > 0)
	{
		int iCopySize = std::min<int>(iFreeSpace, iSize_);
		::memcpy(&m_ovlSend.m_pBuffer[m_ovlSend.m_dwLeft], szData_, iCopySize);
		m_ovlSend.m_dwLeft += iCopySize;
		szData_ += iCopySize;
		iSize_ -= iCopySize;
	}

	// input data의 처리 - 2. 아직 데이터가 남아 있다면 SendQueue에 복사 시도.
	if (iSize_ > 0)
	{
		KLocker lock2(m_csSendQueue);

		iSize_ -= m_kSendQueue.Push(szData_, iSize_);

		ms_kMaxSizeInfo.Check(m_kSendQueue.GetDataSize(), m_kSendQueue.size());
	}

	// input data의 처리 - 3. 그래도 아직 복사하지 못한 데이터가 남아 있다면 buffer full.
	if (iSize_ > 0)
	{
		START_LOG(cwarn, L"Send Buffer Full. data size : " << iSize_
			<< L", left : " << m_ovlSend.m_dwLeft
			<< L", bSending : " << m_ovlSend.m_bSending
			<< L", BeforeData : " << nBeforeSize
			<< L", AfterData : " << nAfterSize)
			<< END_LOG;

		START_LOG(cerr, L"Data")
			<< BUILD_LOG(KncUtil::toWideString(szData_))
			<< END_LOG;
		// 데이터를 보내지 않았음. 다시 보내려면 SendPakcet 한번 더 호출해야 함.
		m_ovlSend.m_dwLeft = 0;
		m_ovlSend.m_bSending = false;

		m_bSendBufferFull = true; // 070429. kkurrung. Send Buffer Full의 카운트를 체크 한다.
		OnSocketError();

		return false;
	}

	// 전송중이지 않으면 다시한번 WsaSend를 호출해준다.
	if (m_ovlSend.m_bSending == false)
	{
		DWORD dwWrite = 0;
		m_ovlSend.m_wsaBuffer.len = m_ovlSend.m_dwLeft;

		int ret = ::WSASend(m_sock,                     // socket
			&m_ovlSend.m_wsaBuffer,     // 버퍼 포인터
			1,                          // WSABUF의 갯수
			&dwWrite,                   // [out] 즉시 전송된 량
			0,                          // Flag
			&m_ovlSend,                 // OVERLAPPED
			NULL);

		//std::cout << "raw length : " << kbuff_.GetLength() << std::endl << "SEND " << bsbuff << std::endl;

		if (ret == SOCKET_ERROR && ::WSAGetLastError() == ERROR_IO_PENDING)    // 현재 pending 중..
		{
			m_ovlSend.m_bSending = true;
			return true;
		}

		if (ret != SOCKET_ERROR) // 호출 하자 마자 다 보냄.
		{
			m_ovlSend.m_bSending = true;
			return true;
		}

		START_LOG(cwarn, L"WSASend Failed. LastError : " << GET_WSA_MSG) << END_LOG;

		OnSocketError();
		return false;
	}

	return true;
}

void KSocketObject::OnRecvCompleted(DWORD dwTransfered)
{
	START_LOG(cerr, L"Pure Virtual Function Call... Check Please....") << END_LOG;
}

void KSocketObject::OnSocketError()
{
	START_LOG(cerr, L"Pure Virtual Function Call... Check Please....") << END_LOG;
}