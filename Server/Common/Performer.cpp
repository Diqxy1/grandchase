#include "Performer.h"
#include <Serializer/Serializer.h>
#include <dbg/dbg.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

NiImplementRTTI(KPerformer, KSimObject);

KPerformer::KPerformer(void)
{
}

KPerformer::~KPerformer(void)
{
	KLocker lock(m_csEventQueue);

	if (!m_queEvent.empty())
	{
		START_LOG(cwarn, L"Object destroy with un-processed Events. Name : " << m_strName)
			<< BUILD_LOG(m_queEvent.size())
			<< END_LOG;
	}
}

ImplToStringW(KPerformer)
{
	KLocker lock(m_csEventQueue);

	return START_TOSTRING_PARENTW(KSimObject)
		<< TOSTRINGW(m_queEvent.size());
}

/*virtual*/KEvent* KPerformer::CreateKEvent()
{
	START_LOG(cerr, L"Virtual Function Call... Check Please....") << END_LOG;
	return NULL;
}

/*virtual*/void KPerformer::ProcessEvent(const KEventPtr& spEvent_)
{
	START_LOG(cerr, L"Virtual Function Call... Check Please....") << END_LOG;
	return;
}

bool KPerformer::GetEvent(KEventPtr& spEvent_)
{
	spEvent_.reset();
	{ // locking scope
		KLocker lock(m_csEventQueue);

		if (!m_queEvent.empty())
		{
			spEvent_ = m_queEvent.front();
			m_queEvent.pop_front();
			return true;
		}
	} // locking scope

	return false;
}

bool KPerformer::SwapEvent(OUT std::deque<KEventPtr>& queEvent_)
{
	queEvent_.clear();
	KLocker lock(m_csEventQueue);
	if (!m_queEvent.empty()) {
		queEvent_.swap(m_queEvent);
		return true;
	}
	return false;
}

void KPerformer::Tick()
{
	//////////////////////////////////////////////////////////////////////////
	// consume event queue
	std::deque<KEventPtr> queEvent;
	if (!SwapEvent(queEvent)) // queue 가 비어 있으면 바로 리턴
		return;

	std::deque<KEventPtr>::iterator qit;
	for (qit = queEvent.begin(); qit != queEvent.end(); ++qit) {
		if (*qit != NULL)
			ProcessEvent(*qit);
	}

	//// traverse all event
	//KEventPtr spEvent;
	//while( GetEvent( spEvent ) )
	//{
	//    // 050220. 이벤트 포인터가 NULL인지 감지한다.
	//    if( spEvent != NULL )
	//    {
	//        // 2. call event handler
	//        ProcessEvent( spEvent );
	//    }
	//}
}

void KPerformer::QueueingEvent(const KEventPtr& spEvent_, const int nFrom_)
{
	spEvent_->m_nFrom = nFrom_;
	QueueingSPEvent(spEvent_);
}

void KPerformer::QueueingEvent(IN KSerBuffer& kBuff_, const int nFrom_)
{
	KEventPtr spEvent(CreateKEvent());
	spEvent->m_nFrom = nFrom_;

	if (!spEvent) {
		START_LOG(cerr, L"Create Event Instance Fail...!!") << END_LOG;
		return;
	}

	KSerializer ks;
	ks.BeginReading(&kBuff_);
	ks.Get(*spEvent);
	ks.EndReading();

	QueueingSPEvent(spEvent);
}

void KPerformer::QueueingSPEvent(const KEventPtr& spEvent_)
{
	START_LOG(clog, L"ⓡ " << spEvent_->GetEventIDString() << L" (name:" << m_strName << L")") << END_LOG;
	KLocker lock(m_csEventQueue);
	m_queEvent.push_back(spEvent_);
}

void KPerformer::QueueingIntEvent(IN OUT KIntEventPtr& spEvent_)
{
	// make event object
	KEventPtr spEvent(CreateKEvent());
	if (!spEvent) {
		START_LOG(cerr, L"Create Event Instance Fail...!!") << END_LOG;
		return;
	}

	spEvent->m_kbuff.Swap(spEvent_->m_kbuff);
	spEvent->m_nFrom = spEvent_->m_nFrom;
	spEvent->m_usEventID = spEvent_->m_usEventID;

	QueueingSPEvent(spEvent);
}