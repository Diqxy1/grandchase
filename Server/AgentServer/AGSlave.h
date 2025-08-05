#pragma once
#include "Actor.h"
#include "SobManager.h"
#include "AgentPacket.h"
#include <windows.h>    // ::GetTickCount()
#include "AgentEvent.h"

SmartPointer(KAGSlave);

class KAGSlave : public KActor
{
	NiDeclareRTTI;
	DeclToStringW;
	DeclareException;

public:
	struct AGUserPoolTag {};
	KAGSlave(void);
	virtual ~KAGSlave(void);

	static KAGSlavePtr Create();
	static void AddManager(KAGSlavePtr spAGSlave);

	KAGSlavePtr GetThisPtr() { return std::static_pointer_cast<KAGSlave>((*this).shared_from_this()); }

	// override
	virtual void Tick();
	virtual KEvent* CreateKEvent() { return new KAgentEvent; }

protected:
	//override.
	virtual void ProcessEvent(const KEventPtr& spEvent);
	virtual void OnDestroy();   // LOGINED 상태로 빠져나오고, DB data update를 신청한다

protected:  // Packet Handling
	_DECL_ON_FUNC(EAGS_VERIFY_ACCOUNT_REQ, KAgentServerInfo);
	_DECL_ON_FUNC(EAGS_SET_AGIT_SID_ACK, KEnterAgitSIDAck);
	_DECL_ON_FUNC(EAGS_CLEAR_AGIT_SID_NOT, DWORD);
	_DECL_ON_FUNC(EAGS_UPDATE_SLAVE_STATE_NOT, PAIR_INT_BOOL);
	_DECL_ON_FUNC(EAGS_USER_CONNECT_STATUS_REQ, KUserConnectStatus);

protected:
	KAgentServerInfo    m_kSlaveInfo;
};
