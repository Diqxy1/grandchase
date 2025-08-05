#pragma once
#include "NetLayer.h"
#include "KNCSingleton.h"

class KAGNetLayer : public KNetLayer
{
	DeclareSingleton(KAGNetLayer);
	DeclToStringW;
	NiDeclareRTTI;

public:
	KAGNetLayer(void);
	virtual ~KAGNetLayer(void);

	// derived from KNetLayer
	void ResetMaxData();
	virtual bool Init();
	virtual void ShutDown();
	virtual void RegToLua(std::shared_ptr<lua_State> L);

	void AddTRServerAddress(const char* szIP, USHORT usPort);
	bool GetTRServerAddress(IN int nKey, OUT DWORD& dwIP, OUT USHORT& usPort);
	bool DeleteTRServerAddress(IN unsigned int nIndex);

	void AddURServerAddress(const char* szIP, USHORT usPort);
	bool GetURServerAddress(IN int nKey, OUT DWORD& dwIP, OUT USHORT& usPort);
	bool DeleteURServerAddress(IN unsigned int nIndex);

protected:
	mutable KncCriticalSection  m_csRelayServer;
	std::vector<SOCKADDR_IN>    m_vecTRServerAddr;
	std::vector<SOCKADDR_IN>    m_vecURServerAddr;
};

DefSingletonInline(KAGNetLayer);