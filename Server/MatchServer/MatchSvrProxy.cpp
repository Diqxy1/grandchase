#include "MatchSvrProxy.h"
#include "NetError.h"
#include "MatchSimLayer.h"
#include "MatchEvent.h"
#include "MatchPacket.h"
#include "MatchUser.h"
#include "Log4.h"
//FILE_NAME_FOR_LOG

NiImplementRTTI( KMatchSvrProxy, KActorProxy );

#define EVENT_TYPE KMatchEvent
#define CLASS_TYPE KMatchSvrProxy

KMatchSvrProxy::KMatchSvrProxy(void)
: m_iID( 0 )
{
    SetSendQueueSize( 1024 * 512 );
}

KMatchSvrProxy::~KMatchSvrProxy(void)
{
}

ImplToStringW( KMatchSvrProxy )
{
    return START_TOSTRING_PARENTW( KActorProxy )
        << TOSTRINGW( KncUtil::toWideString(GetIPStr()) )
        << TOSTRINGW( GetPort() )
        << TOSTRINGWb( IsConnected() );
}

// Call to ProcessEventQueue (Client)
void KMatchSvrProxy::ProcessEvent( const KEventPtr& spEvent_ )
{
     KSerializer     ks;
     KSerBuffer      kbuff;

     ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    switch( spEvent_->m_usEventID )
    {

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString() ) << END_LOG;
    }
}

void KMatchSvrProxy::OnDestroy()
{
    KActorProxy::OnDestroy();
}

void KMatchSvrProxy::OnSocketError()
{
    KSession::OnSocketError();

    START_LOG(cerr, L"Call Socket Err, Queue Size : " << GetQueueSize() ) << END_LOG;
}

void KMatchSvrProxy::RemoveIOCPKey()
{
    DeleteCompKey();
}
