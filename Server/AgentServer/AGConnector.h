#pragma once
#include "AGProxy.h"
#include "KNCSingleton.h"
#include "KncTimer.h"

class KAGConnector
{
	DeclareSingleton(KAGConnector);
	DeclToStringW;

public:
	KAGConnector(void);
	virtual ~KAGConnector(void);

	void Init();
	void Tick();
	void ShutDown();

	bool InitConnect();
	inline bool Connect();

	void SetName(const std::wstring& strServerName);
	void SetRemoteAddress(const char* szIP, unsigned short usPort);
	void GetRemoteAddress(OUT std::string& strIP, OUT unsigned short& usPort);
	void KeepConnectionThread();

	template < typename T >
	bool SendPacket(unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false);

private:
	KAGProxyPtr                     m_spAgPtr;
	std::wstring                    m_strServerName;
	std::string                     m_strIP;
	unsigned short                  m_usPort;
	KncTimer                        m_kConnectTimer;

	std::shared_ptr< KTThread<KAGConnector> >  m_spThreadKeepConnect;
};
DefSingletonInline(KAGConnector);
DeclOstmOperatorA(KAGConnector);

template < typename T >
bool KAGConnector::SendPacket(unsigned short usEventID_, T& data_, bool bLogging_, bool bCompress_)
{
	__JIF(m_spAgPtr, return false);
	__JIF(m_spAgPtr->IsConnected(), return false);
	__JIF(m_spAgPtr->IsAfterVerify(), return false);
	return m_spAgPtr->SendPacket(usEventID_, data_, bLogging_, bCompress_);
}