//#pragma warning( disable : 4800 )
#include <Winsock2.h>
#include "SimLayer.h"
#include "NetLayer.h"
#include "Actor.h"
#include <KncLua.h>
#include "Thread/Locker.h"
#include <sstream>  // std::ostringstream
#include <boost/bind.hpp>
#include "Log4.h"

//ImplementSingleton(KSimLayer);
NiImplementRootRTTI(KSimLayer);
ImplementException(KSimLayer);

KSimLayer::KSimLayer()
	:m_dwServerType(0x00)
	, m_dwAuthType(0x00)
	, m_dwBillingType(BT_WEB)
	, m_dwMaxElapsedTick(0)
	, m_dwElapsedTick(0)
	, m_dwKernelTick(0)
	, m_dwUserTick(0)
	, m_nCurrentIndex(0)
	, m_dwAccumulateTick(0)
	, m_nAccumulateEvent(0)
{
	m_mapAuthTypeStrings[AT_NETMARBLE] = L"AT_NETMARBLE";
	m_mapAuthTypeStrings[AT_GASH] = L"AT_GASH";
	m_mapAuthTypeStrings[AT_LEVELUP] = L"AT_LEVELUP";
	m_mapAuthTypeStrings[AT_NTREEV] = L"AT_NTREEV";
	m_mapAuthTypeStrings[AT_LEVELUP_PH] = L"AT_LEVELUP_PH";
	m_mapAuthTypeStrings[AT_FUNBOX_TH] = L"AT_FUNBOX_TH";
	m_mapAuthTypeStrings[AT_MEGAXUS] = L"AT_MEGAXUS";
	m_mapAuthTypeStrings[AT_LIN] = L"AT_LIN";
	m_mapAuthTypeStrings[AT_SGI_EU] = L"AT_SGI_EU";
	m_mapAuthTypeStrings[AT_CYOU] = L"AT_CYOU";

	for (size_t i = 0; i < m_aTickInfo.max_size(); ++i) {
		m_aTickInfo[i] = 0;
	}
}

KSimLayer::~KSimLayer()
{
	std::cout << "~KSimLayer" << std::endl;
	ShutDown();
}

void KSimLayer::Init()
{
	// run thread manager.
	std::for_each(m_vecpThreadMgr.begin(), m_vecpThreadMgr.end(),
		boost::bind(&KThreadManager::BeginThread, _1));
}

const std::wstring KSimLayer::GetAuthTypeStr(DWORD dwFlag_) const
{
	std::map< DWORD, std::wstring >::const_iterator mit;
	std::wstringstream stm;
	stm << L"-- Auth Type --" << std::endl;
	for (mit = m_mapAuthTypeStrings.begin(); mit != m_mapAuthTypeStrings.end(); ++mit)
	{
		if (mit->first & dwFlag_)
		{
			stm << boost::wformat(L" 0x%08x = %s \n") % mit->first % mit->second;
		}
	}

	return stm.str();
}

const wchar_t* KSimLayer::GetBillingTypeStr() const
{
	const static wchar_t* szBillingType[] = {
		L"BT_WEB",
		L"BT_GASH",
		L"BT_BR_PAYLETTER",
		L"BT_PAYLETTER",
		L"BT_TH_PAYLETTER",
		L"BT_ID_PAYLETTER",
		L"BT_LA_NCASH",
		L"BT_CN_CYOU",
	};

	return szBillingType[m_dwBillingType];
}

ImplToStringW(KSimLayer)
{
	START_TOSTRINGW
		<< TOSTRINGW(m_kActorManager.GetCount())
		<< TOSTRINGW(m_kActorManager2.GetCount())
		<< GetAuthTypeStr(m_dwAuthType)
		<< TOSTRINGW(GetBillingTypeStr())
		<< TOSTRINGW(GetElapsedTick()) // average tick
		<< TOSTRINGW(m_dwMaxElapsedTick); // max tick

#ifdef _KNC_NO_CRASH_
	KSimLayer::DumpExceptionInfo(stm_);
#endif
	return stm_;
}

void KSimLayer::Run()
{
	START_LOG(clog, L"Enter Run()") << END_LOG;
	srand((unsigned)time(NULL));
	DWORD ret;

	while (true)
	{
		ret = ::WaitForSingleObject(m_hKillEvent, 1);

		if (ret == WAIT_OBJECT_0) break;

		if (ret == WAIT_TIMEOUT) Tick();

		else    START_LOG(cerr, L"*** WaitForSingleObject() - return : " << ret) << END_LOG;
	}
}

void KSimLayer::Tick()
{
	// elapsed time  start
	FILETIME ftKernelTimeStart, ftKernelTimeEnd, ftUserTimeStart, ftUserTimeEnd;
	FILETIME ftDummy;

	::GetThreadTimes(::GetCurrentThread(), &ftDummy, &ftDummy, &ftKernelTimeStart, &ftUserTimeStart);
	m_dwElapsedTick = ::GetTickCount();

	/* lucidust, 2011. 11. 16
		1분당 처리하는 유저 이벤트 수와
		해당 이벤트들의 처리시간 합을 기록함.
		(1), (2) 주석을 풀어 세팅하고 User_Event_Process_Time 폴더에 기록을 확인한다.
	*/
	//(1)
	int nAccumulateEvent;
	m_kActorManager.GetEventQueueSize(nAccumulateEvent);
	m_nAccumulateEvent = nAccumulateEvent;

	m_kActorManager.Tick();
	m_kActorManager2.Tick();

	// end of elapsed time
	m_dwElapsedTick = ::GetTickCount() - m_dwElapsedTick;
	::GetThreadTimes(::GetCurrentThread(), &ftDummy, &ftDummy, &ftKernelTimeEnd, &ftUserTimeEnd);

	__int64 qwKernelTimeElapsed = (::Int64ShllMod32(ftKernelTimeEnd.dwHighDateTime, 32) | ftKernelTimeEnd.dwLowDateTime) - (::Int64ShllMod32(ftKernelTimeStart.dwHighDateTime, 32) | ftKernelTimeStart.dwLowDateTime);
	__int64 qwUserTimeElapsed = (::Int64ShllMod32(ftUserTimeEnd.dwHighDateTime, 32) | ftUserTimeEnd.dwLowDateTime) - (::Int64ShllMod32(ftUserTimeStart.dwHighDateTime, 32) | ftUserTimeStart.dwLowDateTime);
	m_dwKernelTick = static_cast<DWORD>(qwKernelTimeElapsed / 10000);
	m_dwUserTick = static_cast<DWORD>(qwUserTimeElapsed / 10000);

	//(2)
	m_dwAccumulateTick = m_dwElapsedTick;

	SetMaxElasedTick(m_dwElapsedTick);
	SetAvgElapsedTick(m_dwElapsedTick);
	// avg elapsed time in  recent 10-20  and max elapsed time.
}

void KSimLayer::ShutDown()
{
	End();

	std::for_each(m_vecpThreadMgr.begin(), m_vecpThreadMgr.end(),
		boost::bind(&KThreadManager::EndThread, _1));

	m_vecpThreadMgr.clear();
}

#define LUABIND_NO_RTTI
#include "luabind/luabind.hpp"
#include "luabind/operator.hpp"

void KSimLayer::RegToLua(std::shared_ptr<lua_State> L)
{
	luabind::module(L.get())
		[
			luabind::class_<KSimLayer>("KSimLayer")
			.def("AddTypeFlag", &KSimLayer::AddTypeFlag)
		.def("CheckTypeFlag", &KSimLayer::CheckTypeFlag)
		.def("DeleteTypeFlag", &KSimLayer::DeleteTypeFlag)
		.def("AddAuthTypeFlag", &KSimLayer::AddAuthTypeFlag)
		.def("CheckAuthTypeFlag", &KSimLayer::CheckAuthTypeFlag)
		.def("DeleteAuthTypeFlag", &KSimLayer::DeleteAuthTypeFlag)
		.def("GetAuthTypeStr", &KSimLayer::GetAuthTypeStr)
		.enum_("AuthType")
		[
			luabind::value("AT_NETMARBLE", AT_NETMARBLE),
			luabind::value("AT_GASH", AT_GASH),
			luabind::value("AT_LEVELUP", AT_LEVELUP),
			luabind::value("AT_NTREEV", AT_NTREEV),
			luabind::value("AT_LEVELUP_PH", AT_LEVELUP_PH),
			luabind::value("AT_FUNBOX_TH", AT_FUNBOX_TH),
			luabind::value("AT_MEGAXUS", AT_MEGAXUS),
			luabind::value("AT_LIN", AT_LIN),
			luabind::value("AT_SGI_EU", AT_SGI_EU),
			luabind::value("AT_CYOU", AT_CYOU)
		]
	.def("SetBillingType", &KSimLayer::SetBillingType)
		.def("GetBillingType", &KSimLayer::GetBillingType)
		.def("GetBillingTypeStr", &KSimLayer::GetBillingTypeStr)
		.enum_("BillingType")
		[
			luabind::value("BT_WEB", BT_WEB),
			luabind::value("BT_GASH", BT_GASH),
			luabind::value("BT_BR_PAYLETTER", BT_BR_PAYLETTER),
			luabind::value("BT_PAYLETTER", BT_PAYLETTER),
			luabind::value("BT_TH_PAYLETTER", BT_TH_PAYLETTER),
			luabind::value("BT_ID_PAYLETTER", BT_ID_PAYLETTER),
			luabind::value("BT_LA_NCASH", BT_LA_NCASH),
			luabind::value("BT_CN_CYOU", BT_CN_CYOU)
		]
		];
}

void KSimLayer::SetMaxElasedTick(DWORD dwElapsedTick_)
{
	if (dwElapsedTick_ > m_dwMaxElapsedTick) {
		m_dwMaxElapsedTick = dwElapsedTick_;
	}
}

void KSimLayer::SetAvgElapsedTick(DWORD dwElapsedTick_)
{
	m_aTickInfo[m_nCurrentIndex] = dwElapsedTick_;

	m_nCurrentIndex = ++m_nCurrentIndex % MAX_TICK_COUNT;
}

const DWORD KSimLayer::GetElapsedTick() const
{
	DWORD dwResult = 0;
	boost::array<DWORD, MAX_TICK_COUNT>::const_iterator ait;
	for (ait = m_aTickInfo.begin(); ait != m_aTickInfo.end(); ++ait) {
		dwResult += *ait;
	}

	return (dwResult / MAX_TICK_COUNT);
}