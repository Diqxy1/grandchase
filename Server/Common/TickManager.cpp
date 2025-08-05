#include "../GameServer/GSSimLayer.h"
#include "Socket/Session.h"
#include "TickManager.h"
#include <dbg/dbg.hpp>
#include "odbc/Odbc.h"
#include "UserPacket.h"
#include "ThreadManager.h"
#include "../GameServer/Worker.h"

ImplementSingleton(KTickManager);
ImplOstmOperatorW2A(KTickManager);
NiImplementRootRTTI(KTickManager);

KTickManager::KTickManager(void)
	:m_kTimer(5 * 60 * 1000)
	, m_kMinTimer(60 * 1000)
	, m_dwMaxQueueOver(50)
	, m_dwQueueOverCount(0)
{
	m_kMaxElapsedTime.Init();
	m_kMaxQueueCount.Init();
	m_kMaxSimLayerElapsed.Init();
	m_kActorManagerElapsed.Init();
}

KTickManager::~KTickManager(void)
{
}

ImplToStringW(KTickManager)
{
	return stm_;
}

void KTickManager::UpdateActorEvent(DWORD dwQueueCount_, std::wstring strName_, DWORD dwElapsedTime_)
{
	KLocker lock(m_csQueueCount);

	if (dwQueueCount_ > m_dwMaxQueueOver) {
		++m_dwQueueOverCount;
	}

	if (dwQueueCount_ > m_kMaxQueueCount.m_dwQueueCount) {
		m_kMaxQueueCount.m_dwElapsedTime = dwElapsedTime_;
		m_kMaxQueueCount.m_strName = strName_;
		m_kMaxQueueCount.m_dwQueueCount = dwQueueCount_;
		m_kMaxQueueCount.m_Time = KncUtil::TimeToInt(CTime::GetCurrentTime());
		return;
	}

	if (dwElapsedTime_ > m_kMaxElapsedTime.m_dwElapsedTime) {
		m_kMaxElapsedTime.m_dwElapsedTime = dwElapsedTime_;
		m_kMaxElapsedTime.m_strName = strName_;
		m_kMaxElapsedTime.m_dwQueueCount = dwQueueCount_;
		m_kMaxElapsedTime.m_Time = KncUtil::TimeToInt(CTime::GetCurrentTime());
		return;
	}
}

void KTickManager::Tick()
{
	// 1분 Tick.
	if (m_kMinTimer.CheckTime()) {
		size_t userCount = SiKGSSimLayer()->m_kActorManager.GetCount();
		{
			std::wstringstream stm;
			int nQueueSize;
			// 실제 게임서버에서 접근하는 아이를 가져다가 쓰자.
			SiKGSSimLayer()->m_kActorManager.GetEventQueueSize(nQueueSize);

			stm << L"[" << KNC_TIME_STRING << L"] ";
			// write log.
			stm << L"QueueSize: " << nQueueSize << std::endl;

			TQUEUEING_EVENT_TO_DB(KWorker, KUserEvent::EVENT_TICK_MANAGER_QUEUE_SIZE, std::wstring(L""), 0, 0, stm.str());

			stm.clear();
			stm.str(L"");
		}

		{
			std::map<USHORT, __int64> mapTemp;
			KSession::GetPacketCount(mapTemp);
			TQUEUEING_EVENT_TO_DB(KWorker, KUserEvent::EVENT_LT_PACKET_COUNT, std::wstring(L""), 0, 0, mapTemp);
		}

		{
			KLocker lock(m_csAccumulate);

			std::wstringstream stm;
			stm << L"[" << KNC_TIME_STRING << L"] ";
			stm << L"Time: " << m_kAccumulateElapsed.m_dwAccumulateTick
				<< "\tEvents: " << m_kAccumulateElapsed.m_nAccumulateEvent
				<< "\tUserCount: " << userCount << std::endl;

			TQUEUEING_EVENT_TO_DB(KWorker, KUserEvent::EVENT_USER_EVENT_PROCESS_TIME, std::wstring(L""), 0, 0, stm.str());

			m_kAccumulateElapsed.m_dwAccumulateTick = 0;
			m_kAccumulateElapsed.m_nAccumulateEvent = 0;
			stm.clear();
			stm.str(L"");
		}
		// 유저 객체 생성/소멸 수를 비교해서 메모리 문제 찾으려고 했으나, 기록 결과는 10명 차이가 안나서
		// 문제 없는 것으로 결론내리고 관련 로그는 주석으로 남겨둠. 2012-05-15. Luke
		//{
		//    std::wstringstream stm;
		//    stm << L"[" << KNC_TIME_STRING << L"] ";
		//    stm << L"Total New count : " << SiKGSSimLayer()->GetCreateUserCount()
		//        << L",Total Delete count : " << SiKGSSimLayer()->GetDeleteUserCount()
		//        << L", Current User Count : " << userCount << std::endl;

		//    TQUEUEING_EVENT_TO_DB( KWorker, KUserEvent::EVENT_NEW_DELETE_USERCOUNT, std::wstring(L""), 0, stm.str() );
		//    stm.clear();
		//    stm.str(L"");
		//}
	}

	// 5분 Tick.
	if (m_kTimer.CheckTime()) {
		KLocker lock(m_csQueueCount);

		std::wstringstream stm;

		stm << L"[" << KNC_TIME_STRING << L"]" << std::endl;
		// write log
		stm << L"Max Queue Count, [" << (LPCTSTR)CTime(m_kMaxQueueCount.m_Time).Format(KNC_TIME_FORMAT)
			<< L"] Name : " << m_kMaxQueueCount.m_strName
			<< L" | Queue count : " << m_kMaxQueueCount.m_dwQueueCount
			<< L" | Elapsed time : " << m_kMaxQueueCount.m_dwElapsedTime << L" " << std::endl;

		stm << L" Max Elapsed Time, [" << (LPCTSTR)CTime(m_kMaxElapsedTime.m_Time).Format(KNC_TIME_FORMAT)
			<< L"] Name : " << m_kMaxElapsedTime.m_strName
			<< L" | Queue count : " << m_kMaxElapsedTime.m_dwQueueCount
			<< L" | Elapsed time : " << m_kMaxElapsedTime.m_dwElapsedTime << L" " << std::endl;

		stm << L" ActorManagerElapsed time : " << m_kActorManagerElapsed.m_dwElapsedTime
			<< L" ( " << m_kActorManagerElapsed.m_dwKernelTime
			<< L" , " << m_kActorManagerElapsed.m_dwUserTime
			<< L" ) , Time : " << (LPCTSTR)CTime(m_kActorManagerElapsed.m_Time).Format(KNC_TIME_FORMAT)
			<< std::endl;

		stm << L" , Max SimLayer Tick : " << m_kMaxSimLayerElapsed.m_dwElapsedTime
			<< L" ( " << m_kActorManagerElapsed.m_dwKernelTime
			<< L" , " << m_kActorManagerElapsed.m_dwUserTime
			<< L" ) , Time : " << (LPCTSTR)CTime(m_kMaxSimLayerElapsed.m_Time).Format(KNC_TIME_FORMAT)
			<< std::endl;

		stm << L" , User Queue Over Count : " << m_dwQueueOverCount
			<< std::endl;

		std::map< std::wstring, std::vector<DWORD> >::const_iterator mit;
		for (mit = m_mapElapsedTime.begin(); mit != m_mapElapsedTime.end(); ++mit) {
			stm << L"Event Name : " << mit->first << L" | ";
			std::vector<DWORD>::const_iterator vit;
			for (vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
				stm << *vit << ", ";
			}
			stm << std::endl;
		}

		GetDBProcessTime(stm);

		GetDBProcessTime_except_EVENT_VERIFY_ACCOUNT_REQ(stm);

		TQUEUEING_EVENT_TO_DB(KWorker, KUserEvent::EVENT_TICK_MANAGER_LOG, std::wstring(L""), 0, 0, stm.str());

		m_kMaxQueueCount.Init();
		m_kMaxElapsedTime.Init();
		m_kActorManagerElapsed.Init();
		m_kMaxSimLayerElapsed.Init();
		m_mapElapsedTime.clear();
		m_kDBThreadTime.Init();
		m_dwQueueOverCount = 0;

		std::vector<std::wstring> overRecvEvent;
		KSession::GetOverRecvEvent(overRecvEvent);

		if (!overRecvEvent.empty()) {
			TQUEUEING_EVENT_TO_DB(KWorker, KUserEvent::EVENT_LT_RECV_PACKET, std::wstring(L""), 0, 0, overRecvEvent);
		}

		stm.clear();
		stm.str(L"");
	}
}

void KTickManager::GetDBProcessTime(std::wstringstream& stm_)
{
	std::wstring query_;
	DWORD term_;
	CTime time_;

	KODBC::GetDumpMaxTime(query_, term_, time_);
	KODBC::ResetMaxTime();

	stm_ << L"DB Queue process time, Query : " << query_
		<< L", Elapsed : " << term_
		<< L", Time : " << (LPCTSTR)(time_.Format(KNC_TIME_FORMAT)) << L" " << std::endl;
}

void KTickManager::UpdateAllActorTick(DWORD dwElapsedTime_, DWORD dwKernelTime_, DWORD dwUserTime_)
{
	KLocker lock(m_csQueueCount);

	if (dwElapsedTime_ > m_kActorManagerElapsed.m_dwElapsedTime) {
		m_kActorManagerElapsed.m_dwElapsedTime = dwElapsedTime_;
		m_kActorManagerElapsed.m_dwKernelTime = dwKernelTime_;
		m_kActorManagerElapsed.m_dwUserTime = dwUserTime_;
		m_kActorManagerElapsed.m_Time = KncUtil::TimeToInt(CTime::GetCurrentTime());
	}
}

void KTickManager::UpdateSimLayerTick(DWORD dwElapsedTime_, DWORD dwKernelTime_, DWORD dwUserTime_)
{
	KLocker lock(m_csQueueCount);

	if (dwElapsedTime_ > m_kMaxSimLayerElapsed.m_dwElapsedTime) {
		m_kMaxSimLayerElapsed.m_dwElapsedTime = dwElapsedTime_;
		m_kActorManagerElapsed.m_dwKernelTime = dwKernelTime_;
		m_kActorManagerElapsed.m_dwUserTime = dwUserTime_;
		m_kMaxSimLayerElapsed.m_Time = KncUtil::TimeToInt(CTime::GetCurrentTime());
	}
}

void KTickManager::UpdateEventTime(std::wstring& strKey_, DWORD dwElapsedTime_)
{
	m_mapElapsedTime[strKey_].push_back(dwElapsedTime_);

	// 주기적으로 파일(로그)에 쓰고, 데이터는 clear 하기.
}

void KTickManager::SetTickGap(IN DWORD dwTick_)
{
	m_kTimer.SetInterval(dwTick_);
}

void KTickManager::SetMinTickGap(IN DWORD dwTick_)
{
	m_kMinTimer.SetInterval(dwTick_);
}

void KTickManager::UpdateAccumulateData(IN DWORD& dwAccumulateTick_, IN int& nAccumulateEvent_)
{
	KLocker lock(m_csAccumulate);

	m_kAccumulateElapsed.m_dwAccumulateTick += dwAccumulateTick_;
	m_kAccumulateElapsed.m_nAccumulateEvent += nAccumulateEvent_;
}

void KTickManager::UpdateDBThreadTime(IN const USHORT usEventID_, IN const DWORD dwElapsedTime_)
{
	if (KUserEvent::EVENT_VERIFY_ACCOUNT_REQ == usEventID_) {
		return;
	}

	KLocker lock(m_csDBThreadTime);

	if (dwElapsedTime_ > m_kDBThreadTime.m_dwElapsedTime) {
		m_kDBThreadTime.m_strEventString = KUserEvent::GetEventIDString(usEventID_);
		m_kDBThreadTime.m_dwElapsedTime = dwElapsedTime_;
		m_kDBThreadTime.m_Time = KncUtil::TimeToInt(CTime::GetCurrentTime());
	}
}

void KTickManager::GetDBProcessTime_except_EVENT_VERIFY_ACCOUNT_REQ(std::wstringstream& stm_)
{
	KLocker lock(m_csDBThreadTime);

	stm_ << L"DB Event Process Time except (event_verify_account_req), Event : " << m_kDBThreadTime.m_strEventString
		<< L", Elapsed : " << m_kDBThreadTime.m_dwElapsedTime
		<< L", Time : " << (LPCTSTR)CTime(m_kDBThreadTime.m_Time).Format(KNC_TIME_FORMAT) << L" " << std::endl;
}