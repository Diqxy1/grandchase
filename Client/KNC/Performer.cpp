#include "stdafx.h"
#include "Performer.h"



NiImplementRTTI( KPerformer, KSimObject );

KPerformer::KPerformer(void)
{
}

KPerformer::~KPerformer(void)
{
    KLocker lock( m_csEventQueue );

    if( !m_queEvent.empty() )
    {
        START_LOG( cwarn, L"Object destroy with un-processed Events. Name : " << m_strName )
            << BUILD_LOG( m_queEvent.size() )
            << END_LOG;
    }
}

ImplToStringW( KPerformer )
{
    KLocker lock( m_csEventQueue );

    return START_TOSTRING_PARENTW( KSimObject )
        << TOSTRINGW( m_queEvent.size() );
}

/*virtual*/KEvent* KPerformer::CreateKEvent()
{
    START_LOG( cout, L"Virtual Function Call... Check Please...." );
    return NULL;
}

/*virtual*/void KPerformer::ProcessEvent( const KEventPtr& spEvent_)
{
    START_LOG( cout, L"Virtual Function Call... Check Please...." );
    return;
}

bool KPerformer::GetEvent( KEventPtr& spEvent_ )
{
    spEvent_.reset();
    { // locking scope
        KLocker lock( m_csEventQueue );

        if( !m_queEvent.empty() )
        {
            spEvent_ = m_queEvent.front();
            m_queEvent.pop();
            return true;
        }
    } // locking scope

    return false;
}
void KPerformer::Tick()
{
    //////////////////////////////////////////////////////////////////////////
    // consume event queue

    // traverse all event
    KEventPtr spEvent;
    while( GetEvent( spEvent ) )
    {
        // 050220. 이벤트 포인터가 NULL인지 감지한다.
        if( spEvent != NULL )
        {
            // 2. call event handler
            ProcessEvent( spEvent );
        }
    }
}

void KPerformer::QueueingEvent( const KEventPtr& spEvent_, const int nFrom_ )
{
    dbg::clog << L"ⓡ " << spEvent_->GetEventIDString() << L" (name:" << m_strName << L")" << END_LOG;

    KLocker lock( m_csEventQueue );

    spEvent_->m_nFrom = nFrom_;
    m_queEvent.push( spEvent_ );
}

void KPerformer::QueueingEvent( IN KSerBuffer& kBuff_, const int nFrom_ )
{
    KEventPtr spEvent( CreateKEvent() );

//{ 061205. kkurrung. 
    if( !spEvent )
    {
        dbg::cout << L"Create Event Instance Fail...!!" << END_LOG;
        return;
    }
//} 061205. kkurrung.

    KSerializer ks;
    ks.BeginReading( &kBuff_ );
    ks.Get( *spEvent );
    ks.EndReading();

    QueueingEvent( spEvent, nFrom_ );
}
