#include "AGDBLayer.h"
#include "AGDBThread.h"
#include "odbc/Odbc.h"

ImplOstmOperatorW2A(KAGDBLayer);
NiImplementRTTI(KAGDBLayer, KDBLayer);
ImplementSingleton(KAGDBLayer);

KAGDBLayer::KAGDBLayer(void)
{
}

KAGDBLayer::~KAGDBLayer(void)
{
}

ImplToStringW(KAGDBLayer)
{
	return START_TOSTRING_PARENTW(KDBLayer);
}

std::shared_ptr<KThread> KAGDBLayer::CreateThread()
{
	std::shared_ptr<KAGDBThread> spThread(new KAGDBThread);
	spThread->SetThreadManager(this);
	return spThread;
}

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KAGDBLayer::RegToLua(std::shared_ptr<lua_State> sptlua)
{
	KDBLayer::RegToLua(sptlua);

	luabind::module(sptlua.get())
		[
			luabind::def("GetDBLayer", KAGDBLayer::GetInstance),
			luabind::class_<KAGDBLayer, KThreadManager>("KAGDBLayer")
			.def(luabind::tostring(luabind::self))
		.def("Init_ODBC", &KAGDBLayer::Init_ODBC)
		];
}

void KAGDBLayer::Init_ODBC(const char* szMainODBC_, const char* szStatODBC_)
{
	KAGDBThread::InitFileDSN(szMainODBC_);
	KAGDBThread::InitFileDSNStat(szStatODBC_);
}