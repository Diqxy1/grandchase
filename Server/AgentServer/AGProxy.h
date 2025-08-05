#pragma once
#include "ActorProxy.h"
#include <windows.h>
#include "Thread/Thread.h"
#include "AgentEvent.h"
#include "AgentPacket.h"
#include <ToString.h>

SmartPointer(KAGProxy);

class KAGSimLayer;
class KAgentEvent;
class KAGConnector;
class KAGProxy : public KActorProxy
{
	typedef KAGSimLayer SLayerType;
	typedef KAgentEvent IDVenderType;
	NiDeclareRTTI;
	DeclToStringW;

public:
	KAGProxy(void);
	virtual ~KAGProxy(void);

	// derived from KPerformer
	virtual void ProcessEvent(const KEventPtr&);
	virtual KEvent* CreateKEvent() { return new KAgentEvent; }

	bool Connect(std::string strIP, unsigned short usPort);
	bool IsAfterVerify() { return m_bAfterVerify; };
	bool CheckRecvUser(IN const std::map<DWORD, std::vector<DWORD> >& mapServerUser);

protected:
	HANDLE          m_hVerifyEvent;
	int             m_nSavedResult;
	bool            m_bAfterVerify;

	friend class    KAGConnector;

protected:
	_DECL_ON_FUNC(EAGS_VERIFY_ACCOUNT_ACK, int);
	_DECL_ON_FUNC(EAGS_SET_AGIT_SID_REQ, KEnterAgitSIDReq);
	_DECL_ON_FUNC(EAGS_USER_CONNECT_NOT, KUserTimeParam);
	_DECL_ON_FUNC(EAGS_USER_DISCONNECT_NOT, KUserTimeParam);
	_DECL_ON_FUNC(EAGS_SET_DESTROY_AGIT_NOT, bool);
	_DECL_ON_FUNC(EAGS_USER_CONNECT_STATUS_ACK, KUserConnectStatus);
	_DECL_ON_FUNC(EAGS_REMOVE_AGIT_INFO_NOT, DWORD);
};
DeclOstmOperatorA(KAGProxy);