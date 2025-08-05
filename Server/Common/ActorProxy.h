#pragma once
#include "Socket/Session.h"
#include <windows.h>
#include <Queue>

struct KServerInfo;
class KActorProxy : public KSession
{
	NiDeclareRTTI;
	DeclToStringW;

protected:
	KActorProxy();

public:
	virtual ~KActorProxy(void) {}

	void SetHwnd(HWND hWnd);
	void SetThrowWinMsg(bool bThrowWinMsg);
	bool GetThrowWinMsg() { return m_bThrowWinMsg; }

	// 060720. 인증 처리까지 기다린다. KUserProxy, KMSProxy가 이를 사용한다.
	int ConnectNAuth(IN const KServerInfo& kServerInfo, IN const KEventPtr spEvent, IN DWORD dwTimeOut);

	void SetClassID(int iClassID) { m_iClassID = iClassID; }  // 윈도우 메세지를 구성할때 사용할 id 할당.
	int GetClassID() { return m_iClassID; }

	int GetProxyID() { return m_iProxyID; }
	void SetProxyID(int iProxyID) { m_iProxyID = iProxyID; }

protected:
	void PostEvent(KEventPtr spEvent);    // 이벤트를 윈도우 메시지로 하거나 내부 큐에 큐잉한다.

	HWND                        m_hWnd;
	bool                        m_bThrowWinMsg;     ///< false이면 받은 메시지를 큐에 넣어두기만 한다.

	std::shared_ptr<void>     m_spEventAck;
	int                         m_nAckOK;
	int                         m_iClassID;

	int                         m_iProxyID;

public:

	KncCriticalSection          m_csRecvMsg;
	std::queue<KEventPtr>       m_queRecvMsg;       ///< m_bThrowWinmsg = false일때 사용.
};