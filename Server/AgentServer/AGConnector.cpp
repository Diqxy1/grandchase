#include "AGConnector.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton(KAGConnector);

KAGConnector::KAGConnector(void)
	:m_kConnectTimer(1000 * 10)
{
	m_spAgPtr.reset();
	m_strIP.clear();
	m_usPort = 0;
}

KAGConnector::~KAGConnector(void)
{
	ShutDown();
}

ImplOstmOperatorW2A(KAGConnector);
ImplToStringW(KAGConnector)
{
	m_spAgPtr->ToString(stm_);
	return stm_;
}

void KAGConnector::Init()
{
	m_spAgPtr.reset(new KAGProxy);
}

void KAGConnector::Tick()
{
	if (m_spAgPtr == NULL) {
		return;
	}

	m_spAgPtr->Tick();
}

bool KAGConnector::InitConnect()
{
	m_spThreadKeepConnect = std::shared_ptr< KTThread<KAGConnector> >(new KTThread<KAGConnector>(*this, &KAGConnector::KeepConnectionThread, 10000));

	if (m_spThreadKeepConnect) {
		m_spThreadKeepConnect->Begin();
		START_LOG(cerr, L"AgConnector 재접속 Thread 시작") << END_LOG;
		return true;
	}

	START_LOG(cerr, L"AgConnector 재접속 Thread 생성 실패") << END_LOG;
	return false;
}

bool KAGConnector::Connect()
{
	_JIF(m_spAgPtr, return false);
	__JIF(!m_strIP.empty(), return false);
	__JIF(m_usPort > 0, return false);
	__JIF(!m_spAgPtr->IsConnected(), return true);

	m_spAgPtr->SetName(m_strServerName);
	if (false == m_spAgPtr->Connect(m_strIP, m_usPort)) {
		START_LOG(cerr, L"에이전트 마스터 서버로 접속 실패.. IP : "
			<< KncUtil::toWideString(m_strIP) << L", Port : " << m_usPort)
			<< END_LOG;
		return false;
	}

	return true;
}

void KAGConnector::ShutDown()
{
	// 일단 재접속 Thread부터 죽인다.
	if (m_spThreadKeepConnect) {
		m_spThreadKeepConnect->End(3000);
	}

	if (m_spAgPtr) {
		m_spAgPtr->OnDestroy();
		m_spAgPtr.reset();
	}
}

void KAGConnector::SetName(const std::wstring& strServerName_)
{
	m_strServerName = strServerName_;
}

void KAGConnector::SetRemoteAddress(const char* szIP_, unsigned short usPort_)
{
	m_strIP = szIP_;
	m_usPort = usPort_;
}

void KAGConnector::GetRemoteAddress(OUT std::string& strIP_, OUT unsigned short& usPort_)
{
	strIP_ = m_strIP;
	usPort_ = m_usPort;
}

void KAGConnector::KeepConnectionThread()
{
	if (m_spAgPtr->IsConnected()) {
		return;
	}

	if (false == m_kConnectTimer.CheckTime()) {
		return;
	}

	m_spAgPtr.reset(new KAGProxy);
	if (Connect()) {
		START_LOG(cerr, L" Reconnect To Agent Master.") << END_LOG;
	}
}