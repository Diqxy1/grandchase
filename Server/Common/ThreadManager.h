#pragma once

#include "IntEvent.h"
#include <Queue>
#include <ToString.h>
#include <RTTI.h>
#include <boost/shared_ptr.hpp>
#include <thread/Locker.h>
#include <thread/thread.h>
#include <atltime.h> // CTime
#include <dbg/dbg.hpp>
#include "Log4.h"

class KThreadManager
{
	DeclToStringW;
	NiDeclareRootRTTI(KThreadManager);

protected:
	KThreadManager(void);
public:
	virtual ~KThreadManager(void);

public:
	void Init(int nThreadNum);
	void BeginThread();
	virtual void EndThread();
	size_t GetThreadNum() const { KLocker lock(m_csVecThread); return m_vecThread.size(); }
	void SetThreadNum(size_t nThreadNum); // 실시간에 스레드 갯수를 조절하려고 할 때 호출.

	template<typename T>
	void QueueingEvent(unsigned short usEventID, const wchar_t* szLogin, DWORD dwUID, char cCharType, T& data);
	void QueueingEvent(KIntEventPtr spEvent) { KLocker lock(m_csEventQueue); m_queEvent.push(spEvent); }
	void QueueingID(unsigned short usEventID, const wchar_t* szLogin, DWORD dwUID, char cCharType = -1);

	// 큐잉된 이벤트를 얻어옴. 큐가 비어있으면 이벤트 없이 즉시 리턴.
	bool GetIntEvent(KIntEventPtr& spEvent);

	void ClearQueue();  // 초 위급상황 대비. 평상시 절대 쓰지 말것.
	void SetProcessing(bool bProcessing) { m_bProcessingEvent = bProcessing; }

	int GetQueueSize()
	{
		KLocker lock(m_csEventQueue);
		return m_queEvent.size();
	}

	void DumpMaxProcessInfo(std::wostream& stm) const;
	void ResetMaxProcessingInfo();

	template < typename T >
	void CheckMaxProcessingInfo(DWORD dwElapTime, const KIntEvent& kIntEvent)
	{
		KLocker lock(m_kMaxProcessingInfo.m_cs);

		if (dwElapTime > m_kMaxProcessingInfo.m_dwTerm)
		{
			m_kMaxProcessingInfo.m_strName = kIntEvent.m_strSender;
			m_kMaxProcessingInfo.m_strEventID = T::GetEventIDString(kIntEvent.m_usEventID);
			m_kMaxProcessingInfo.m_cTime = CTime::GetCurrentTime();
			m_kMaxProcessingInfo.m_dwTerm = dwElapTime;

			START_LOG(clog, L"이벤트 처리 최대 소요시간 갱신. Name : " << kIntEvent.m_strSender)
				<< BUILD_LOG(m_kMaxProcessingInfo.m_strEventID)
				<< BUILD_LOG(m_kMaxProcessingInfo.m_dwTerm) << END_LOG;
		}
	}

protected:
	// 060113. florist. Factory Method 패턴을 활용한 객체 생성. 이를 위해 생성할 객체마다
	// KThreadManager를 상속받는 새 클래스를 정의해야 하지만, 이것으로 인해 독립성이 강해져
	// singleton으로 매니저 객체를 사용한다거나 하는 등에 유용하다. template을 활용해 상속없이
	// thread 객체를 생성하게 했었으나, 다시 상속을 활용한 예전방식으로 회귀하였다. 매번 상속하는게 낫다.
	virtual std::shared_ptr<KThread> CreateThread();

	mutable CRITICAL_SECTION            m_csEventQueue;
	std::queue<KIntEventPtr>            m_queEvent;

	bool                                m_bTerminateReserved;
	bool                                m_bProcessingEvent; // 위급상황이 아닌 한, 항상 true.
	size_t                              m_nTerminateCount;  // 종료된 스레드 갯수를 센다.
	std::vector< std::shared_ptr<KThread> >   m_vecThread;
	mutable KncCriticalSection                  m_csVecThread;

	struct KMaxProcessingInfo {  // 처리시간이 가장 오래 걸린 이벤트에 대한 정보를 기억.
		mutable KncCriticalSection  m_cs;
		std::wstring                m_strName;
		std::wstring                m_strEventID;
		DWORD                       m_dwTerm;       // 이벤트 하나를 처리완료하기까지 걸린 시간.
		CTime                       m_cTime;
	} m_kMaxProcessingInfo;
};

DeclOstmOperatorW(KThreadManager);

template< typename T >
void KThreadManager::QueueingEvent(unsigned short usEventID_, const wchar_t* szLogin_, DWORD dwUID_, char cCharType_, T& data_)
{
	if (!m_bProcessingEvent) return;

	KIntEventPtr spEvent(new KIntEvent);

	spEvent->m_usEventID = usEventID_;
	spEvent->m_dwSenderUID = dwUID_;
	spEvent->m_cCharType = cCharType_;
	if (szLogin_ != NULL)
		spEvent->m_strSender = szLogin_;

	KSerializer ks;
	ks.BeginWriting(&spEvent->m_kbuff);
	ks.Put(data_);
	ks.EndWriting();

	QueueingEvent(spEvent);
}

template< typename T >
class KTThreadManager : public KThreadManager
{
	NiDeclareRTTI;
	DeclToStringW;

protected:
	KTThreadManager(void) {}
public:
	virtual ~KTThreadManager(void) {}

	static KTThreadManager<T>* GetInstance()
	{
		if (ms_selfInstance == NULL)
			ms_selfInstance = new KTThreadManager<T>;
		return ms_selfInstance;
	}
	static void ReleaseInstance()
	{
		if (ms_selfInstance != NULL)
		{
			delete ms_selfInstance;
			ms_selfInstance = NULL;
		}
	}

protected:
	// util function
	virtual std::shared_ptr<KThread> CreateThread()
	{
		std::shared_ptr<T> spThread(new T);
		spThread->SetThreadManager(this);
		return spThread;
	}

	static KTThreadManager<T>*   ms_selfInstance;
};

template < typename T > KTThreadManager<T>* KTThreadManager<T>::ms_selfInstance = NULL;
template < typename T > NiImplementRTTI(KTThreadManager<T>, KThreadManager);
template < typename T > ImplOstmOperatorW2A(KTThreadManager<T>);
template < typename T > ImplToStringW(KTThreadManager<T>)
{
	return START_TOSTRING_PARENTW(KThreadManager);
}
