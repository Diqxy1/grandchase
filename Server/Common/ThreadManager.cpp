#include "dbg/dbg.hpp"
#include "ThreadManager.h"
#include "odbc/Odbc.h"

#include <boost/bind.hpp>
#include "Log4.h"

ImplOstmOperatorW(KThreadManager);
NiImplementRootRTTI(KThreadManager);
ImplToStringW(KThreadManager)
{
	{ // locking scope
		KLocker lock(m_csEventQueue);
		KLocker lock2(m_csVecThread);

		START_TOSTRINGW
			<< TOSTRINGWb(m_bProcessingEvent)
			<< TOSTRINGW(m_queEvent.size())
			<< TOSTRINGW(m_vecThread.size());
	} // locking scope

	DumpMaxProcessInfo(stm_);

	return stm_;
}

KThreadManager::KThreadManager(void)
	:
	m_bTerminateReserved(false),
	m_nTerminateCount(0),
	m_bProcessingEvent(true)
{
	InitializeCriticalSection(&m_csEventQueue);

	m_kMaxProcessingInfo.m_strEventID = L"Initial state.";
	m_kMaxProcessingInfo.m_strName = L"Initial state.";
	m_kMaxProcessingInfo.m_dwTerm = 0;
	m_kMaxProcessingInfo.m_cTime = CTime::GetCurrentTime();
}

KThreadManager::~KThreadManager(void)
{
	DeleteCriticalSection(&m_csEventQueue);
}

void KThreadManager::DumpMaxProcessInfo(std::wostream& stm_) const
{
	KLocker lock(m_kMaxProcessingInfo.m_cs);

	stm_ << L" -- Max processing time -- " << std::endl
		<< L"  user name : " << m_kMaxProcessingInfo.m_strName << std::endl
		<< L"  event id  : " << m_kMaxProcessingInfo.m_strEventID << std::endl
		<< L"  duration  : " << m_kMaxProcessingInfo.m_dwTerm << std::endl
		<< L"  executed time : " << (const wchar_t*)m_kMaxProcessingInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S")) << std::endl;
}

void KThreadManager::ResetMaxProcessingInfo()
{
	KLocker lock(m_kMaxProcessingInfo.m_cs);

	m_kMaxProcessingInfo.m_strEventID.clear();
	m_kMaxProcessingInfo.m_dwTerm = 0;
	m_kMaxProcessingInfo.m_cTime = CTime::GetCurrentTime();
	m_kMaxProcessingInfo.m_strName = (const wchar_t*)m_kMaxProcessingInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	m_kMaxProcessingInfo.m_strName.append(L" 에 데이터 초기화 되었습니다.");
}

bool KThreadManager::GetIntEvent(KIntEventPtr& spEvent_)
{
	{ // locking scope
		KLocker lock(m_csEventQueue);

		if (!m_queEvent.empty())
		{
			spEvent_ = m_queEvent.front();
			m_queEvent.pop();
			return true;
		}

		else if (m_bTerminateReserved) // 종료가 예약되었고, 처리할 데이터가 더이상 남지 않았다.
		{
			//spEvent_ = NULL;    // 확실하게 다시한번! NULL을 리턴받으면 스레드는 종료한다.
			spEvent_.reset();
			m_nTerminateCount++;
			return true;
		}
	} // locking scope

	return false;
}

void KThreadManager::Init(int nThreadNum_)
{
	// create I/O thread
	KLocker lock(m_csVecThread);

	for (int i = 0; i < nThreadNum_; i++)
	{
		m_vecThread.push_back(CreateThread());
	}
}

void KThreadManager::BeginThread()
{
	KLocker lock(m_csVecThread);

	std::for_each(m_vecThread.begin(), m_vecThread.end(), boost::bind(&KThread::Begin, _1));
}

void KThreadManager::EndThread()
{
	KLocker lock(m_csVecThread);

	if (m_vecThread.empty())
		return;

	m_bTerminateReserved = true;

	START_LOG(cerr, L"모든 스레드가 종료되도록 대기합니다. ** 남은 Queuing Event 개수 : " << GetQueueSize()) << END_LOG;

	DWORD dwWaitTick = ::GetTickCount();
	while (m_nTerminateCount < m_vecThread.size()) //&& ::GetTickCount() - dwWaitTick < 10000 )
	{
		::Sleep(20);
	}

	START_LOG(cerr, L"대기 후 남은 스레드 수 : " << m_vecThread.size() - m_nTerminateCount)
		<< BUILD_LOG(m_vecThread.size())
		<< BUILD_LOG(m_nTerminateCount) << END_LOG;

	m_vecThread.clear();
}

void KThreadManager::QueueingID(unsigned short usEventID_, const wchar_t* szLogin_, DWORD dwUID_, char cCharType_)
{
	if (!m_bProcessingEvent) return;

	KIntEventPtr spEvent(new KIntEvent);

	spEvent->m_usEventID = usEventID_;
	spEvent->m_dwSenderUID = dwUID_;
	spEvent->m_cCharType = cCharType_;
	if (szLogin_ != NULL)
		spEvent->m_strSender = szLogin_;

	KLocker lock(m_csEventQueue);
	m_queEvent.push(spEvent);
}

void KThreadManager::SetThreadNum(size_t nThreadNum_)
{
	KLocker lock(m_csVecThread);

	if (nThreadNum_ == m_vecThread.size())
	{
		std::cout << " 현재 " << nThreadNum_ << "개의 스레드가 동작 중입니다." << std::endl;
		return;
	}

	std::cout << " 스레드 수 조절. " << m_vecThread.size() << " -> " << nThreadNum_ << std::endl;

	if (nThreadNum_ < m_vecThread.size())    // 스레드를 줄여야 한다.
	{
		m_vecThread.resize(nThreadNum_);
	}

	else    // 스레드를 늘려야 한다.
	{
		for (size_t i = m_vecThread.size(); i < nThreadNum_; i++)
		{
			std::shared_ptr<KThread> spThread(CreateThread());
			spThread->Begin();
			m_vecThread.push_back(spThread);
		}
	}

	std::cout << "최종 스레드 갯수 : " << m_vecThread.size() << std::endl;
}

void KThreadManager::ClearQueue()
{
	KLocker lock(m_csEventQueue);

	START_LOG(cerr, L"이벤트 큐 정리. size : " << m_queEvent.size()) << END_LOG;

	while (!m_queEvent.empty())
	{
		KIntEventPtr spEvent;

		spEvent = m_queEvent.front();
		m_queEvent.pop();
	}
}

std::shared_ptr<KThread> KThreadManager::CreateThread()
{
	START_LOG(cerr, L"Pure Virtual Function Call... Check Please....") << END_LOG;
	return std::shared_ptr<KThread>((KThread*)(NULL));
}