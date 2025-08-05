#pragma   warning( disable :4786 )
#include "NUserProxy.h"
#include "NUserEvent.h"
#include "UserEvent.h"
#include "NUserPacket.h"

#include "Socket/NetCommon.h"
#include <KncSecurity/KncSecurity.h>
//FILE_NAME_FOR_LOG

NiImplementRTTI( KNUserProxy, KActorProxy );

#define EVENT_TYPE KNUserEvent
#define CLASS_TYPE KNUserProxy

KNUserProxy::KNUserProxy(void)
{
}

KNUserProxy::~KNUserProxy(void)
{
}

KEvent* KNUserProxy::CreateKEvent()
{
    return new KNUserEvent;
}

ImplToStringW( KNUserProxy )
{
    return START_TOSTRING_PARENTW( KActorProxy );
}

void KNUserProxy::ProcessEvent( const KEventPtr& spEvent_ )
{
    PostEvent( spEvent_ );

    KSerializer     ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    switch( spEvent_->m_usEventID )
    {
        CASE( ENU_VERIFY_ACCOUNT_ACK );
       _CASE( ENU_WAIT_TIME_NOT, DWORD );

    default:     
        // 이곳에서 이벤트 핸들러는, 꼭 필요한 것만 정의된다. 정의되지 않은 이벤트가 있을 수 있다.
        //START_LOG( cerr , "이벤트 핸들러가 정의되지 않았음. " << kEvent_ ) << END_LOG;
        break;
    } 
}

IMPL_ON_FUNC( ENU_VERIFY_ACCOUNT_ACK )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_strLogin )
        << BUILD_LOG( kPacket_.m_bMale )
        << BUILD_LOG( kPacket_.m_iAge )
        << BUILD_LOG( kPacket_.m_dwAuthTick )
        << END_LOG;
}

_IMPL_ON_FUNC( ENU_WAIT_TIME_NOT, DWORD )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_ )
        << END_LOG;
}
