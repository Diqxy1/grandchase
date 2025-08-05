#pragma once
#include "SimLayer.h"
#include "AGConnector.h"

class KAGSimLayer : public KSimLayer
{
	DeclareSingleton(KAGSimLayer);
	NiDeclareRTTI;
	DeclToStringW;

public:

	KAGSimLayer(void);
	virtual ~KAGSimLayer(void);

	// derived from KSimLayer
	virtual void Init();
	virtual void Tick();
	virtual void ShutDown();
	virtual void RegToLua(std::shared_ptr<lua_State> L);
	bool DestroyAllUsers();

	void SetUseAgentMaster(IN const bool& bVal) { KLocker lock(m_csMasterSlave); m_bUseAgentMaster = bVal; }
	bool IsUseAgentMaster() { KLocker lock(m_csMasterSlave); return m_bUseAgentMaster; }
	void SetAgentSlave(IN const bool& bVal) { KLocker lock(m_csMasterSlave); m_bAgentSlave = bVal; }
	bool IsAgentSlave() { KLocker lock(m_csMasterSlave); return m_bAgentSlave; }
	void SetAgentMaster(IN const bool& bVal) { KLocker lock(m_csMasterSlave); m_bAgentMaster = bVal; }
	bool IsAgentMaster() { KLocker lock(m_csMasterSlave); return m_bAgentMaster; }

public: // for lua..
	DWORD                           m_dwShutdownTimeLimit;  // 서버 종료시 DB처리를 위한 대기시간.
	DWORD                           m_dwSendTimeLimit; // AGUser 에서 내 정보를 보고하는 시간 간격.

	mutable KncCriticalSection      m_csMasterSlave;
	bool                            m_bUseAgentMaster; // 에이전트 서버 중앙통제기능 사용여부
	bool                            m_bAgentSlave; // 중앙통제용 에이전트에 접속하는 하위 에이전트인가
	bool                            m_bAgentMaster; // 중앙통제용 에이전트인가
};

DefSingletonInline(KAGSimLayer);