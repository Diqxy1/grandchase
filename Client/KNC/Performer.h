#pragma once
#include "Event.h"
#include "SimObject.h"
//#include <queue>
#include <Serializer/SerBuffer.h>

//#include <windows.h>    // CRITICAL_SECTION
#include "Thread/Locker.h"

class KPerformer : public KSimObject
{
    NiDeclareRTTI;
    DeclToStringW;

public:
    KPerformer(void);
    virtual ~KPerformer(void);

    // 이곳에 위치한 virtual은 모두 최초 정의. 상속 아님
//{ 061205. kkurrung. // 순수 가상 함수호출시 오류를 남기도록 변경
    virtual KEvent* CreateKEvent();
//} 061205. kkurrung.

    template < class T >
    void    QueueingEvent( unsigned short usEventID, const T& data, const int nFrom = KEvent::FT_NONE ); 
    void    QueueingEvent( IN KSerBuffer& kBuff, const int nFrom = KEvent::FT_NONE );
    void    QueueingEvent( const KEventPtr& spEvent, const int nFrom = KEvent::FT_NONE );
    size_t  GetQueueSize()
    {
        KLocker lock( m_csEventQueue );
        return m_queEvent.size();
    }

protected:
//{ 061205. kkurrung. // 순수 가상 함수호출시 오류를 남기도록 변경
    virtual void    ProcessEvent( const KEventPtr& );
//} 061205. kkurrung.
    virtual void    Tick();     // ProcessEventQueue ( queue consumming )
    bool GetEvent( KEventPtr& spEvent );

protected:
    mutable KncCriticalSection                  m_csEventQueue;
    std::queue<KEventPtr>                       m_queEvent;

};

template< class T >
void KPerformer::QueueingEvent( unsigned short usEventID_, const T& data_, const int nFrom_ )
{
    // make event object
    KEventPtr spEvent( CreateKEvent() );

//{ 061205. kkurrung.  // 이벤트 생성 실패시 로그를 남기도록(가상 한수 호출이 일어나면 로그를 남기는것과 같다.)
    if( !spEvent.get() )
    {
        START_LOG( cout, L"Create Event Instance Fail...!!" );
        return;
    }
//} 061205. kkurrung.

    spEvent->m_usEventID = usEventID_;

    KSerializer         ks;
    // serialize - only data
    ks.BeginWriting( &spEvent->m_kbuff );
    if( !ks.Put( data_ ) )
    {
        START_LOG( cerr, L"Queueing Fail!!.." );
        return;
    }
    ks.EndWriting();

    QueueingEvent( spEvent, nFrom_ );
}

#define CASE_BODY( type, id ) \
{ \
    type kPacket; \
    if( spEvent_->m_kbuff.GetLength() == 0 ) \
    { \
        START_LOG( cerr, L#id L" 버퍼가 비었음. name:" << m_strName ) \
            << BUILD_LOG( GetQueueSize() ) \
            << BUILD_LOG( spEvent_->GetEventIDString() ) \
            << END_LOG; \
    } \
    else if( !ks.Get(kPacket) ) \
    { \
        START_LOG( cerr, L#type L" deserialize 실패. name:" << m_strName ) \
            << BUILD_LOG( spEvent_->GetEventIDString() ) \
            << END_LOG; \
    } \
    else \
    { \
        dbg::clog << L"ⓗ " L#id L" (name:" << m_strName << L")" << END_LOG; \
        ON_##id(kPacket, spEvent_->m_nFrom ); \
    } \
} \
break

#define  CASE(id)           case EVENT_TYPE::##id: CASE_BODY( K##id, id )
#define _CASE(id,packet)    case EVENT_TYPE::##id: CASE_BODY( packet, id )
#define  CASE_NOPARAM(id)   case EVENT_TYPE::##id: \
                            dbg::clog << L"ⓗ " L#id L" (name:" << m_strName << L")" << END_LOG; \
                            ON_##id( spEvent_->m_nFrom ); \
                            break 

#define  IMPL_ON_FUNC(id)           void CLASS_TYPE::ON_##id( K##id& kPacket_, const int nFrom_ )
#define _IMPL_ON_FUNC(id, packet)   void CLASS_TYPE::ON_##id( packet& kPacket_, const int nFrom_ )
#define  IMPL_ON_FUNC_NOPARAM(id)   void CLASS_TYPE::ON_##id( const int nFrom_ )

#define  DECL_ON_FUNC(id)           void ON_##id( K##id& kPacket, const int nFrom )
#define _DECL_ON_FUNC(id, packet)   void ON_##id( packet& kPacket, const int nFrom )
#define  DECL_ON_FUNC_NOPARAM(id)   void ON_##id( const int nFrom )
