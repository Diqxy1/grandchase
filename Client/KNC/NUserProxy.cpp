#include "stdafx.h"
#pragma   warning( disable :4786 )
#include "NUserProxy.h"

NiImplementRTTI( KNUserProxy, KActorProxy );

#ifdef EVENT_TYPE
#undef EVENT_TYPE
#endif
#define EVENT_TYPE KNUserEvent

#ifdef CLASS_TYPE
#undef CLASS_TYPE
#endif
#define CLASS_TYPE KNUserProxy

KNUserProxy::KNUserProxy(void)
{
}

KNUserProxy::~KNUserProxy(void)
{
    End( INFINITE );
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
        //START_LOG( cerr , "이벤트 핸들러가 정의되지 않았음. " << kEvent_ );
        break;
    } 
}

IMPL_ON_FUNC( ENU_VERIFY_ACCOUNT_ACK )
{
    START_LOG_WITH_NAME( cout )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_strLogin )
        << BUILD_LOG( kPacket_.m_strNMPasswd )
        << BUILD_LOG( kPacket_.m_bMale )
        << BUILD_LOG( kPacket_.m_iAge )
        << BUILD_LOG( kPacket_.m_dwAuthTick );
}

_IMPL_ON_FUNC( ENU_WAIT_TIME_NOT, DWORD )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_ );
}
