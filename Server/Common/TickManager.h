#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <KncTimer.h>
#include "GenericLogfile.h"

class KTickManager
{
	DeclareSingleton(KTickManager);
	NiDeclareRootRTTI(KTickManager);
	DeclToStringW;

public:
	KTickManager(void);
	~KTickManager(void);

	void UpdateActorEvent(DWORD dwQueueCount, std::wstring strName, DWORD dwElapsedTime);
	void Tick();
	void UpdateAllActorTick(DWORD dwElapsedTime, DWORD dwKernelTime, DWORD dwUserTime);
	void UpdateSimLayerTick(DWORD dwElapsedTime, DWORD dwKernelTime, DWORD dwUserTime);
	void UpdateEventTime(std::wstring& strKey, DWORD dwElapsedTime);
	void SetTickGap(IN DWORD dwTick_);
	void SetMinTickGap(IN DWORD dwTick_);
	void UpdateAccumulateData(IN DWORD& dwAccumulateTick_, IN int& nAccumulateEvent_);
	void UpdateDBThreadTime(IN const USHORT usEventID_, IN const DWORD dwElapsedTime_);

private:
	void GetDBProcessTime(std::wstringstream& stm_);
	void GetDBProcessTime_except_EVENT_VERIFY_ACCOUNT_REQ(std::wstringstream& stm_);
protected:
	KncCriticalSection m_csQueueCount;

	KncTimer m_kTimer;    // 10분 Tick
	KncTimer m_kMinTimer; // 1분 Tick
	DWORD m_dwMaxQueueOver; // 특정 N개 큐를 가진 유저 체크위한 MAX 변수.
	DWORD m_dwQueueOverCount; // 특정 N개 이상 큐를 가진 유저 수 Counting.

	//KGenericLogfile m_logFile;
	std::map< std::wstring, std::vector<DWORD> > m_mapElapsedTime;

	struct KMaxQueueCount {
		std::wstring m_strName;
		time_t m_Time;
		DWORD m_dwQueueCount;
		DWORD m_dwElapsedTime;

		void Init() {
			m_strName.clear();
			m_Time = 0;
			m_dwQueueCount = 0;
			m_dwElapsedTime = 0;
		}
	} m_kMaxQueueCount;

	struct KMaxElapsedData {
		time_t m_Time;
		DWORD m_dwElapsedTime;
		DWORD m_dwKernelTime;
		DWORD m_dwUserTime;

		void Init() {
			m_Time = 0;
			m_dwElapsedTime = 0;
			m_dwKernelTime = 0;
			m_dwUserTime = 0;
		}
	};

	struct KAccumulateElapsedData {
		time_t m_Time;
		DWORD m_dwAccumulateTick;
		int m_nAccumulateEvent;

		void Init() {
			m_Time = 0;
			m_dwAccumulateTick = 0;
			m_nAccumulateEvent = 0;
		}
	};

	struct KMaxDBThreadTime {
		std::wstring m_strEventString;
		time_t m_Time;
		DWORD m_dwElapsedTime;

		KMaxDBThreadTime() :
			m_strEventString(std::wstring()),
			m_Time(0),
			m_dwElapsedTime(0) {}

		void Init() {
			m_strEventString = std::wstring();
			m_Time = 0;
			m_dwElapsedTime = 0;
		}
	};

	KMaxQueueCount m_kMaxElapsedTime;

	KMaxElapsedData m_kMaxSimLayerElapsed;
	KMaxElapsedData m_kActorManagerElapsed;

	KncCriticalSection m_csAccumulate;
	KAccumulateElapsedData m_kAccumulateElapsed;

	KncCriticalSection m_csDBThreadTime;
	KMaxDBThreadTime m_kDBThreadTime;
};

DefSingletonInline(KTickManager);
DeclOstmOperatorA(KTickManager);

class Checker
{
public:
	Checker(std::wstring& strKey_, DWORD dwMinTime_)
		: strKey(strKey_)
		, dwElapsedTime(0)
		, dwMinTime(dwMinTime_)
	{
		dwElapsedTime = ::GetTickCount();
	}

	~Checker()
	{
		dwElapsedTime = ::GetTickCount() - dwElapsedTime;
		if (dwMinTime < dwElapsedTime) {
			SiKTickManager()->UpdateEventTime(strKey, dwElapsedTime);
		}
	}

private:
	std::wstring strKey;
	DWORD dwElapsedTime;
	DWORD dwMinTime;
};
