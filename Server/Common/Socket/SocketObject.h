#pragma once
#include <WinSock2.h>
#include "Socket/Overlapped.h"
#include <atltime.h>    // CTime
#include "CircularQueue.h" // vector, CircularQueue
#include <Thread/Locker.h>
#include <streambuf>
#include <iostream>

// 060116. florist. 현재 무조건 event 객체를 생성하도록 되어있다.
// 이를 선택 가능하게 수정 한 후, KSockBase로 rename해서
// 모든 소켓 작업에 활용 가능하도록 확장해 나가면 좋을 것 같다.
// (확장 : IO모델 선택 가능, TCP/UDP 선택 가능, 세부 socket option 설정 가능...)
class KSocketObject
{
public:
	KSocketObject(const KSocketObject& right);              // do not use (no implement.)
	KSocketObject& operator=(const KSocketObject& right);   // do not use (no implement.)

	KSocketObject(void);
	virtual ~KSocketObject(void);

public:
	void CloseSocket();
	bool Connect(const char* szIP = NULL,
		unsigned short usPort = 0,
		bool bUseIocp = true,
		std::vector< std::pair<int, int> >* vecOpt = NULL);
	bool SendData(const char* szData, int iSize);
	bool InitRecv();
	void OnIOCompleted(KOVERLAPPED::ENUM_IO_MODE eMode); // IOCP를 사용하지 않는 경우.
	void OnSendCompleted(DWORD dwTransfered);

	// 접속할 원격지 주소를 미리 지정해 줄 수도 있고, Connect 함수 호출시에 지정해 줄 수도 있다.
	void SetRemoteIP(const char* szIP) { m_sockaddr.sin_addr.s_addr = inet_addr(szIP); }
	void SetRemotePort(u_short usPort) { m_sockaddr.sin_port = htons(usPort); }
	void SetSocketInfo(SOCKET socket, SOCKADDR_IN& sockAddr) { m_sock = socket; m_sockaddr = sockAddr; }

	virtual void OnRecvCompleted(DWORD dwTransfered);
	virtual void OnSocketError();
	virtual void OnAcceptConnection() {} // accept 되자마자 호출.

	bool    IsConnected() const { return m_sock != INVALID_SOCKET; }
	bool    IsSending() const { return m_ovlSend.m_bSending; }
	void    SetKey(DWORD dwKey) { m_dwKey = dwKey; }
	DWORD   GetKey() const { return m_dwKey; }
	void    SetIOEvent(KOVERLAPPED::ENUM_IO_MODE eMode, HANDLE hEvent);   // 외부에서 생성한 이벤트를 설정.
	BOOL    CancelIo() { return ::CancelIo((HANDLE)m_sock); }
	void    SetSendQueueSize(int nSize) { m_kSendQueue.Resize(nSize); } // 포지션도 초기화 되어 버린다. 생성시에만 쓰자.

	unsigned int GetIP() const { return m_sockaddr.sin_addr.S_un.S_addr; }
	const char* GetIPStr() const { return inet_ntoa(m_sockaddr.sin_addr); }
	unsigned short GetPort() const { return ntohs(m_sockaddr.sin_port); }

	bool                m_bSendBufferFull;

	static void DumpSendQueueLog(std::wostream& stm) { ms_kMaxSizeInfo.Dump(stm); }

protected:
	inline void MovDataQue2Buf(OUT int& nBeforeSize, OUT int& nAfterSize);   // send-queue에 대기중인 데이터가 있다면 버퍼로 옮긴다.

protected:
	DWORD               m_dwKey;
	SOCKADDR_IN         m_sockaddr;
	KOVERLAPPED         m_ovlRecv;
	KOVERLAPPED         m_ovlSend;
	SOCKET              m_sock;
	KncCriticalSection  m_csSock;   // sendbuff뿐 아니라, m_ovlSend중 EVENT, eIOMode정도만을 제외한 거의 대부분을 보호해야 한다.

	// 보조 버퍼
	KCircularQueue      m_kSendQueue;
	KncCriticalSection  m_csSendQueue;

	static struct KMaxSizeInfo {
		mutable KncCriticalSection  m_cs;
		int                         m_iMaxDataSize;
		int                         m_iQueueSize;
		CTime                       m_cTime;
		void Check(int iDataSize, int iQueueSize);
		void Dump(std::wostream& stm);
	} ms_kMaxSizeInfo;

	friend class KIocp;
};

#define KNC_USE_IOCP