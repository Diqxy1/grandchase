#pragma once
#include "Actor.h"
#include "ActorFactory.h"
#include <set>
#include <RTTI.h>
#include <dbg/dbg.hpp>
#include <Thread/Locker.h>
#include "IntEvent.h"

class KActorManager
{
	KActorManager(const KActorManager& right);              // do not use.
	KActorManager& operator=(const KActorManager& right);   // do not use.

public:
	KActorManager();
	virtual ~KActorManager();

	template< class T > std::shared_ptr<T> CreateActor();
	void ReserveDelete(const std::wstring& strName);
	void Tick();

	KActorPtr GetByName(const std::wstring& strName);
	KActorPtr GetByIndex(const size_t nIndex);
	KActorPtr GetByUID(const DWORD dwUID);

    template< class T > std::shared_ptr<T> GetByName( const std::wstring& strName );
    template< class T > std::shared_ptr<T> GetByUID( const DWORD dwUID );
	template< class T > T ForEach(T& functor);

	template< class T >
	void QueueingEventTo(const std::wstring& strName, const unsigned short usEventID, const T& data, const int nFrom = KEvent::FT_NONE);
	template< class T >
	void QueueingEventTo(const DWORD dwUID, const unsigned short usEventID, const T& data, const int nFrom = KEvent::FT_NONE);

	void QueueingEventTo(const DWORD dwUID, const KEventPtr& spEvent, const int nFrom = KEvent::FT_NONE);
	void QueueingEventTo(const std::wstring& strName, const KEventPtr& spEvent, const int nFrom = KEvent::FT_NONE);

	template< class T > bool SendTo(const DWORD dwUID,
		const unsigned short usEventID,
		const T& data);

	template< class T > bool SendTo(const std::wstring& strName,
		const unsigned short usEventID,
		const T& data);
	template< class T > void SendToAll(const unsigned short usEventID, const T& data, bool bCompress_ = false);
	template< class T > void SendToAllExMe(const std::wstring& strLogin, const unsigned short usEventID, const T& data);
	template< class T > void SendToAllExMe(const DWORD dwUID, const unsigned short usEventID, const T& data);
	template< class T > void QueueingToAll(const unsigned short usEventID, const T& data, const int nFrom = KEvent::FT_NONE);

	bool Rename(IN const std::wstring& strOldName_, const std::wstring& strNewName_);
	bool RegByUID(IN KActor& kActor);   // UID만 등록한다. name map이나 vector에는 들어가지 않음.

	size_t GetCount() const { KLocker lock(m_csAct); return m_mapActName.size(); }

	void SetActorFactory(IActorFactoryPtr spActorFactory) { m_spActorFactory = spActorFactory; }
	void SetCreateActorEnable(bool bEnable) { m_bCreateActorEnable = bEnable; }

	bool Add(const KActorPtr& spSob);
	void SetEventQueueSize(IN const int nQueueSize);
	bool GetEventQueueSize(OUT int& nQueueSize);
	std::map<std::wstring, KActorPtr> GetActMap() { return m_mapActName; }

protected:
	bool Delete(const std::wstring& strName);
	void DistributeEvent();
	bool SwapQueue(OUT std::deque<KIntEventPtr>& queEvents_);
	bool SwapDelReservedList(std::set<std::wstring>& setReservedList_);

	mutable KncCriticalSection          m_csAct;    // m_vecAct, m_mapActName을 동시에 보호한다.
	std::vector<KActorPtr>              m_vecAct;
	std::map<std::wstring, KActorPtr>   m_mapActName;
	//mutable KncCriticalSection          m_csActUID;     // m_mapActUID를 보호. 현재는 필요없으나 추후 스레드확장을 고려한다.
	std::map<DWORD, KActorPtr>           m_mapActUID;    // 객체의 생성과는 관계없기 때문에, 단일 스레드만 접근한다.

	mutable KncCriticalSection          m_csDel;
	std::set<std::wstring>              m_setDelReserved; // set 으로 교체 (중국에서 동일한 이름이 거의 무한대로 들어간 적이 있다.)

	mutable KncCriticalSection          m_csEventQueue;
	std::deque<KIntEventPtr>            m_queEvent;

	bool                    m_bCreateActorEnable;
	IActorFactoryPtr        m_spActorFactory;

	int                     m_nQueEventSize;
};

template< typename ActorType >
std::shared_ptr<ActorType> KActorManager::CreateActor()
{
	// 041206. 서버 종료 중에는 새 유저 객체를 생성하지 않는다.
	if (!m_bCreateActorEnable) return std::shared_ptr<ActorType>();

	KActorPtr spActor(m_spActorFactory->CreateActor());

	if (!spActor /*|| !Add( spActor )*/)
		return std::shared_ptr<ActorType>();

	START_LOG(clog, L"Name : " << spActor->GetName()) << END_LOG;

	return std::static_pointer_cast<ActorType>( spActor );
}

template< typename ActorType >
std::shared_ptr<ActorType> KActorManager::GetByName(const std::wstring& strName_)
{
	KActorPtr spActor(GetByName(strName_));

	if (spActor == NULL) return std::shared_ptr<ActorType>();

	if (!NiIsExactKindOf(ActorType, spActor.get()))
		return std::shared_ptr<ActorType>();

	return std::static_pointer_cast<ActorType>( spActor );
}

template< typename ActorType >
std::shared_ptr<ActorType> KActorManager::GetByUID(const DWORD dwUID_)
{
	KActorPtr spActor(GetByUID(dwUID_));

	if (spActor == NULL) return std::shared_ptr<ActorType>();

	if (!NiIsExactKindOf(ActorType, spActor.get()))
		return std::shared_ptr<ActorType>();

	return std::static_pointer_cast<ActorType>( spActor );
}

template<typename Func>
Func KActorManager::ForEach(Func& functor)
{
	KLocker lock(m_csAct);
	return std::for_each(m_vecAct.begin(), m_vecAct.end(), functor);
}

template< class T >
void KActorManager::QueueingEventTo(const std::wstring& strName_,
	const unsigned short usEventID_,
	const T& data_,
	const int nFrom_)
{
	KIntEventPtr spEvent(new KIntEvent);
	spEvent->m_nFrom = nFrom_;
	spEvent->m_usEventID = usEventID_;
	spEvent->m_strSender = strName_;

	KSerializer         ks;
	// serialize - only data
	ks.BeginWriting(&spEvent->m_kbuff);
	if (!ks.Put(data_)) {
		START_LOG(cerr, L"Queueing Fail!!..") << END_LOG;
		return;
	}
	ks.EndWriting();

	KLocker lock(m_csEventQueue);
	m_queEvent.push_back(spEvent);
}

template< class T >
bool KActorManager::SendTo(const std::wstring& strName_,
	const unsigned short usEventID_,
	const T& data_)
{
	KActorPtr spActor(GetByName(strName_));

	if (!spActor) return false;

	return spActor->SendPacket(usEventID_, data_);
}

template< class T >
void KActorManager::QueueingToAll(const unsigned short usEventID_, const T& data_, const int nFrom_)
{
	KLocker lock(m_csAct);

	std::vector<KActorPtr>::iterator vit;
	for (vit = m_vecAct.begin(); vit != m_vecAct.end(); vit++)
	{
		(*vit)->QueueingEvent(usEventID_, data_, nFrom_);
	}
}

template< class T >
void KActorManager::SendToAll(const unsigned short usEventID_, const T& data_, bool bCompress_)
{
	KLocker lock(m_csAct);

	std::vector<KActorPtr>::iterator vit;
	for (vit = m_vecAct.begin(); vit != m_vecAct.end(); vit++)
	{
		(*vit)->SendPacket(usEventID_, data_, bCompress_);
	}
}

template< class T >
void KActorManager::SendToAllExMe(const std::wstring& strLogin_, const unsigned short usEventID_, const T& data_)
{
	KLocker lock(m_csAct);

	std::vector<KActorPtr>::iterator vit;
	for (vit = m_vecAct.begin(); vit != m_vecAct.end(); vit++)
	{
		if ((*vit)->GetName() != strLogin_)
			(*vit)->SendPacket(usEventID_, data_);
	}
}

template< class T >
void KActorManager::SendToAllExMe(const DWORD dwUID_, const unsigned short usEventID_, const T& data_)
{
	KLocker lock(m_csAct);

	std::vector<KActorPtr>::iterator vit;
	for (vit = m_vecAct.begin(); vit != m_vecAct.end(); vit++)
	{
		if ((*vit)->GetUID() != dwUID_)
			(*vit)->SendPacket(usEventID_, data_);
	}
}

template< class T >
bool KActorManager::SendTo(const DWORD dwUID_,
	const unsigned short usEventID_,
	const T& data_)
{
	KActorPtr spActor(GetByUID(dwUID_));

	if (!spActor) return false;

	return spActor->SendPacket(usEventID_, data_);
}

template< class T >
void KActorManager::QueueingEventTo(const DWORD dwUID_,
	const unsigned short usEventID_,
	const T& data_,
	const int nFrom_)
{
	KIntEventPtr spEvent(new KIntEvent);
	spEvent->m_nFrom = nFrom_;
	spEvent->m_usEventID = usEventID_;
	spEvent->m_dwSenderUID = dwUID_;

	KSerializer         ks;
	// serialize - only data
	ks.BeginWriting(&spEvent->m_kbuff);
	if (!ks.Put(data_)) {
		START_LOG(cerr, L"Queueing Fail!!..") << END_LOG;
		return;
	}
	ks.EndWriting();

	KLocker lock(m_csEventQueue);
	m_queEvent.push_back(spEvent);
}
