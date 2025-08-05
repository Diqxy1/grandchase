#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

#include "AGSimLayer.h"
#include <iomanip>  // std::setiosflags, std::resetiosflags
#include "defaultFSM.h"
#include "AGDBLayer.h"
#include "AGDBThread.h"
#include "AgentServer.h"
#include "Agent.h"
#include "DBLayer.h"
#include "SlangFilter.h"
#include "AGConnector.h"
#include "AGSlave.h"

ImplementSingleton(KAGSimLayer);
NiImplementRTTI(KAGSimLayer, KSimLayer);

KAGSimLayer::KAGSimLayer(void)
	:m_dwShutdownTimeLimit(30000)
	, m_dwSendTimeLimit(10000)
	, m_bUseAgentMaster(false)
	, m_bAgentSlave(false)
	, m_bAgentMaster(false)
{
}

KAGSimLayer::~KAGSimLayer(void)
{
}

ImplToStringW(KAGSimLayer)
{
	KLocker lock(m_csMasterSlave);

	START_TOSTRING_PARENTW(KSimLayer)
		<< TOSTRINGW(m_dwShutdownTimeLimit)
		<< TOSTRINGW(m_dwSendTimeLimit)
		<< TOSTRINGWb(m_bUseAgentMaster)
		<< TOSTRINGWb(m_bAgentSlave);

	return stm_;
}

ImplOstmOperatorW2A(KAgent);
ImplOstmOperatorW2A(KAGSimLayer);

void KAGSimLayer::RegToLua(std::shared_ptr<lua_State> L)
{
	KSimLayer::RegToLua(L);

	luabind::module(L.get())
		[
			luabind::def("GetSimLayer", &KAGSimLayer::GetInstance),
			luabind::class_<KAGSimLayer, KSimLayer>("KAGSimLayer")
			.def(luabind::tostring(luabind::self))
		.def_readwrite("m_dwShutdownTimeLimit", &KAGSimLayer::m_dwShutdownTimeLimit)
		.def_readwrite("m_dwSendTimeLimit", &KAGSimLayer::m_dwSendTimeLimit)
		.def("SetUseAgentMaster", &KAGSimLayer::SetUseAgentMaster)
		.def("SetAgentSlave", &KAGSimLayer::SetAgentSlave)
		.def("SetAgentMaster", &KAGSimLayer::SetAgentMaster)
		];
}

void KAGSimLayer::Init()
{
	KSimLayer::Init();

	// UserFactory 생성. KUser가 아닌 다른 객체군을 생성한다면, 코드 구현 후 이곳 한줄만 변경하면 됨.
	// Abstract Factory Patern.
	m_kActorManager.SetActorFactory(IActorFactoryPtr(new KActorFactory<KAgent, KDefaultFSM, KAgent::AGUserPoolTag>));
	m_kActorManager2.SetActorFactory(IActorFactoryPtr(new KActorFactory<KAGSlave, KDefaultFSM, KAGSlave::AGUserPoolTag>));
}

void KAGSimLayer::Tick()
{
	BEGIN_TRY_BLOCK;

	KSimLayer::Tick();

	SiKAGConnector()->Tick();
	SiKAgentServer()->Tick();

	END_TRY_BLOCK("");
}

void KAGSimLayer::ShutDown()
{
	KSimLayer::ShutDown();
	KSlangFilter::ReleaseInstance();
}

bool KAGSimLayer::DestroyAllUsers()
{
	m_kActorManager.SetCreateActorEnable(false);
	m_kActorManager.ForEach(boost::bind(&KActor::ReserveDestroy, _1, 0));
	return false;
}