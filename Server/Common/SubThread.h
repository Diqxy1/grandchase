#pragma once
#include <thread/Thread.h>
#include <atltime.h>

class KIntEvent;

// 가장 일반적인 경우의 loop, dispatch가 구현되어 있다.
// 이벤트 아이디에 대한 핸들러 함수만 구현하고자 한다면
// KSubThread에서 상속받고 핸들러만 추가한다.
template < typename IDEvent >
class KSubThread : public KThread
{
	typedef IDEvent IDVenderType;
protected:  // 직접 객체를 생성하지는 않는다.
	KSubThread() {}
public:
	virtual ~KSubThread(void) {}

protected:
	inline void Loop()
	{
		KIntEventPtr spEvent;

		if (!m_pkThreadMgr->GetIntEvent(spEvent))
			return;

		if (spEvent == NULL)
		{
			std::cout << "Sub-Thread Terminated." << std::endl;
			SetEvent(m_hKillEvent);
			return;
		}

		DWORD dwElapTime = ::GetTickCount();  // 040428. 처리 소요시간을 구한다.

		ProcessIntEvent(*spEvent);

		dwElapTime = ::GetTickCount() - dwElapTime;

		m_pkThreadMgr->CheckMaxProcessingInfo<IDVenderType>(dwElapTime, *spEvent);
	}

	virtual void Run()     // derived from KThread
	{
		std::cout << "KSubThread::Run(), thread start." << std::endl;

		DWORD ret;

		while (true)
		{
			ret = ::WaitForSingleObject(m_hKillEvent, 1);

			if (ret == WAIT_OBJECT_0) break;

			if (ret == WAIT_TIMEOUT) Loop();

			else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;
		}// while
		//START_LOG( clog, L"Leave Run()" ) << END_LOG;    // 041229. 가끔 exception 발생
	}

	virtual void ProcessIntEvent(const KIntEvent& kIntEvent) = 0;
};

#define INT_CASE_BODY( type, id ) type kPacket; \
                                if( ks.Get(kPacket) ) \
                                { \
                                    START_LOG( clog, L"DB_ⓗ " L#id ) << END_LOG; \
                                    ON_##id( kIntEvent_.m_strSender, kIntEvent_.m_dwSenderUID, kIntEvent_.m_cCharType, kPacket );\
                                } \
                                else \
                                { \
                                    START_LOG( cerr, L#type L" deserialize 실패. opcode:" << kIntEvent_.m_usEventID ) \
                                        << BUILD_LOG( IDVenderType::GetEventIDString( kIntEvent_.m_usEventID ) ) \
                                        << END_LOG; } \
                                } \
                                break

#define  INT_CASE(id)           case IDVenderType::##id: { INT_CASE_BODY( K##id, id )
#define _INT_CASE(id,packet)    case IDVenderType::##id: { INT_CASE_BODY( packet, id )
#define  INT_CASE_NOPARAM( id ) case IDVenderType::##id: \
                                START_LOG( clog, L"DB_ⓗ " L#id L" (name:" << kIntEvent_.m_strSender << L")" ) << END_LOG; \
                                ON_##id( kIntEvent_.m_strSender, kIntEvent_.m_dwSenderUID, kIntEvent_.m_cCharType );\
                                break

#define  INT_IMPL_ON_FUNC(id)           void CLASS_TYPE##::ON_##id( const std::wstring& strLogin_, DWORD dwUID_, char cCharType_, K##id& kPacket_ )
#define _INT_IMPL_ON_FUNC(id, packet)   void CLASS_TYPE##::ON_##id( const std::wstring& strLogin_, DWORD dwUID_, char cCharType_, packet& kPacket_ )
#define  INT_IMPL_ON_FUNC_NOPARAM(id)   void CLASS_TYPE##::ON_##id( const std::wstring& strLogin_, DWORD dwUID_, char cCharType_ )

#define  INT_DECL_ON_FUNC(id)           void ON_##id( const std::wstring& strLogin, DWORD dwUID, char cCharType_, K##id& kPacket )
#define _INT_DECL_ON_FUNC(id, packet)   void ON_##id( const std::wstring& strLogin, DWORD dwUID, char cCharType_, packet& kPacket )
#define  INT_DECL_ON_FUNC_NOPARAM(id)   void ON_##id( const std::wstring& strLogin, DWORD dwUID, char cCharType_ )

#define QUEUEING_ACK_TO_GUILD_MANAGER( id ) SiKGuildManager()->QueueingEvent( IDVenderType::id, kPacket, IDVenderType::FT_INNER )
#define _QUEUING_ACK_TO_USER( id, kPacket ) SLayerType::GetInstance()->m_kActorManager.QueueingEventTo( strLogin_, IDVenderType::id, kPacket, IDVenderType::FT_INNER )
#define QUEUING_ACK_TO_USER( id )           SLayerType::GetInstance()->m_kActorManager.QueueingEventTo( strLogin_, IDVenderType::id, kPacket, IDVenderType::FT_INNER )
#define QUEUING_ACK_TO_USER_UID( id )       SLayerType::GetInstance()->m_kActorManager.QueueingEventTo( dwUID_, IDVenderType::id, kPacket, IDVenderType::FT_INNER )
#define QUEUING_ACK_TO_SPECIFIC_USER_UID( uid, id, packet )	SLayerType::GetInstance()->m_kActorManager.QueueingEventTo( uid, IDVenderType::id, packet, IDVenderType::FT_INNER )
#define __QUEUING_ACK_TO_USER( id, uid )    SLayerType::GetInstance()->m_kActorManager.QueueingEventTo( uid, IDVenderType::id, kPacket, IDVenderType::FT_INNER )

#define _QUEUING_ACK_TO_NUSER( id, kPacket ) SLayerType::GetInstance()->m_kActorManager2.QueueingEventTo( strLogin_, KNUserEvent::id, kPacket )
#define QUEUING_ACK_TO_NUSER( id )           SLayerType::GetInstance()->m_kActorManager2.QueueingEventTo( strLogin_, KNUserEvent::id, kPacket )
#define QUEUING_ACK_TO_NUSER_UID( id )           SLayerType::GetInstance()->m_kActorManager2.QueueingEventTo( dwUID_, KNUserEvent::id, kPacket )

#define TQUEUEING_EVENT_TO_DB( obj, id, name, uid, chartype, data )       KTThreadManager<obj>::GetInstance()->QueueingEvent( id, name.c_str(), uid, chartype, data )
#define TQUEUEING_ID_TO_DB( obj, id, name, chartype, uid )                KTThreadManager<obj>::GetInstance()->QueueingID( id, name.c_str(), chartype, uid )

#define _QUEUEING_ACK_TO_CENTER( id, kPacket ) SLayerType::GetInstance()->m_kActorManager.QueueingEventTo( strLogin_, KCenterEvent::id, kPacket )
#define QUEUEING_ACK_TO_CENTER( id ) SLayerType::GetInstance()->m_kActorManager.QueueingEventTo( strLogin_, KCenterEvent::id, kPacket, IDVenderType::FT_INNER )
#define QUEUEING_ACK_TO_CENTER_UID( id ) SLayerType::GetInstance()->m_kActorManager.QueueingEventTo( dwUID_, KCenterEvent::id, kPacket, IDVenderType::FT_INNER )