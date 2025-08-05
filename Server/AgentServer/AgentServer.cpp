#include <WinSock2.h>
#include <sstream>
#include <iomanip>
#include "AgentServer.h"
#include "AGSimLayer.h"
#include "AGNetLayer.h"
#include "AGDBLayer.h"
#include "AgentEvent.h"
#include "AgitManager.h"
#include "AGConnector.h"
#include "SwitchUserSID.h"
#include "Log4.h"

#ifdef _DEBUG
#pragma comment( lib, "log4cxxD" ) // log4cxx
#else
#pragma comment( lib, "log4cxx" ) // log4cxx
#endif

//FILE_NAME_FOR_LOG

NiImplementRTTI(KAgentServer, KBaseServer);
ImplementSingleton(KAgentServer);

ImplToStringW(KAgentServer)
{
	return START_TOSTRING_PARENTW(KBaseServer);
}

KAgentServer::KAgentServer(void)
{
}

KAgentServer::~KAgentServer(void)
{
	m_hFirstSyncEvent = NULL;
}

ImplOstmOperatorW2A(KAgentServer);

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KAgentServer::RegToLua()
{
	KBaseServer::RegToLua();

	luabind::module(m_sptlua.get())
		[
			luabind::def("GetAgentServer", &KAgentServer::GetInstance),
			luabind::class_<KAgentServer, KBaseServer>("KAgentServer")
			.def(luabind::tostring(luabind::self))
		.def("SetServerProtocol", &KAgentServer::SetServerProtocol)
		.def("DumpInfo", &KAgentServer::DumpInfo)
		.def("SetSID", &KAgentServer::SetSID),

		luabind::def("GetAgitManager", &KAgitManager::GetInstance),
		luabind::class_<KAgitManager>("KAgitServer")
		.def(luabind::tostring(luabind::self))
		.def("LoadScript", &KAgitManager::LoadScript)
		.def("LoadConfig", &KAgitManager::LoadConfig)
		.def("LoadMapInfo", &KAgitManager::LoadMapInfo)
		.def("LoadFlowerPot", &KAgitManager::LoadFlowerPot)
		.def("LoadFairyTree", &KAgitManager::LoadFairyTree)
		.def("DumpAgit", &KAgitManager::DumpAgit)
		.def("TestFunc_CreateAgit", &KAgitManager::TestFunc_CreateAgit)
		.def("UpdateRank", &KAgitManager::UpdateAgitRankFromDB),

		luabind::def("GetAGConnector", &KAGConnector::GetInstance),
		luabind::class_<KAGConnector>("KAGConnector")
		.def(luabind::tostring(luabind::self))
		.def("SetRemoteAddress", &KAGConnector::SetRemoteAddress),

		luabind::def("GetSwitchUserSID", &KSwitchUserSID::GetInstance),
		luabind::class_<KSwitchUserSID>("KSwitchUserSID")
		.def(luabind::tostring(luabind::self))
		.def("DumpAgentSlave", &KSwitchUserSID::DumpAgentSlave)
		.def("DumpUser", &KSwitchUserSID::DumpUser)
		];
}

bool KAgentServer::Init(const wchar_t* szFileName_)
{
	m_pkNetLayer = SiKAGNetLayer();
	m_pkSimLayer = SiKAGSimLayer();
	m_pkDBLayer = SiKAGDBLayer();

	_JIF(KBaseServer::Init(szFileName_), return false);
	_JIF(LoadScript(), return false);

	m_hFirstSyncEvent = ::CreateEvent(NULL, false, false, NULL);
	SiKAGDBLayer()->QueueingEvent(KAgentEvent::DB_EVENT_FIRST_INIT_REQ, m_strServerName.c_str(), 0, -1, GetServerProtocol());
	SiKAgitManager()->UpdateAgitRankFromDB();

	{
		std::wstringstream stm;
		stm << L"AgentServer_" << SiKAGNetLayer()->m_usPortMaster << L"_" << SiKAGNetLayer()->m_usPortSlave;
		SetServerName(stm.str().c_str());
		stm.clear();
	}

	switch (::WaitForSingleObject(m_hFirstSyncEvent, 3000)) {
	case WAIT_OBJECT_0:
	{
		std::cout << "**** Server Initialize Finished.. ****" << std::endl;

		// connect to AgentMaster
		if (SiKAGSimLayer()->IsUseAgentMaster()) {
			if (SiKAGSimLayer()->IsAgentSlave()) {
				KAGConnector* pkAGProxy = SiKAGConnector();
				pkAGProxy->Init();
				pkAGProxy->SetName(m_strServerName);
				LIF(pkAGProxy->InitConnect());
			}
		}

		CloseHandle(m_hFirstSyncEvent);
		m_hFirstSyncEvent = NULL;
	}
	break;
	case WAIT_TIMEOUT:
		START_LOG(cerr, L"Getting Server Info from DB failed.") << END_LOG;
		CloseHandle(m_hFirstSyncEvent);
		return false;
		break;
	default:
		START_LOG(cerr, L"Unexpected Return value from ::WaitForSingleObject().") << END_LOG;
		CloseHandle(m_hFirstSyncEvent);
		return false;
	}

	SiKAGSimLayer()->SetCreateActorEnable(true);
	START_LOG(cerr, L"Create Actor Enable!!") << END_LOG;
	return true;
}

void KAgentServer::Run(bool bRunInterpreter_)
{
	KBaseServer::Run(bRunInterpreter_); // lua interpreter 시작
}

void KAgentServer::ShutDown()
{
	// 종료처리 하기 전, 모든 아지트의 캐싱 데이터를 업데이트 해야 한다.
	SiKAgitManager()->UpdateCashingData();

	KBaseServer::ShutDown();
	KAGConnector::ReleaseInstance();
	KAGSimLayer::ReleaseInstance();
	KAGNetLayer::ReleaseInstance();
	KAGDBLayer::ReleaseInstance();
	KAgitManager::ReleaseInstance();
	KSwitchUserSID::ReleaseInstance();
}

void KAgentServer::Tick()
{
	if (::GetTickCount() - m_dwServerInfoUpdateTick > KBaseServer::ms_dwSendServerListGap) {
		SiKAGDBLayer()->QueueingEvent(KAgentEvent::DB_EVENT_SERVER_LIST_REQ, m_strServerName.c_str(), 0, -1, GetServerProtocol());
		m_dwServerInfoUpdateTick = ::GetTickCount();
	}

	// 주요 매니저들 Tick
	SiKAgitManager()->Tick();

	if (SiKAGSimLayer()->IsAgentMaster()) {
		SiKSwitchUserSID()->Tick();
	}
}

bool KAgentServer::LoadScript()
{
	// lua 스크립트 로드부분
	_JIF(SiKAgitManager()->LoadScript(), return false);
	return true;
}

const int& KAgentServer::GetServerProtocol()
{
	KLocker lock(m_csServerProtocol);
	return m_nServerProtocol;
}

void KAgentServer::SetServerProtocol(IN const int& nProtocol_)
{
	KLocker lock(m_csServerProtocol);
	m_nServerProtocol = nProtocol_;
}

void KAgentServer::DumpInfo()
{
	// 서버정보 로그로 출력
	DumpServerList();
}

void KAgentServer::SetSID(int nSID_)
{
	SetServerID(nSID_);
}