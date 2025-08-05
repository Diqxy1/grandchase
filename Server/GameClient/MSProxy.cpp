#include "MSProxy.h"
#include "NetError.h"

NiImplementRTTI( KMSProxy, KActorProxy );
ImplementSingleton( KMSProxy );


#define EVENT_TYPE KMSEvent
#define CLASS_TYPE KMSProxy


KMSProxy::KMSProxy(void)
{
}

KMSProxy::~KMSProxy(void)
{
}

ImplToStringW( KMSProxy )
{
    return START_TOSTRING_PARENTW( KActorProxy )
        << TOSTRINGW( KncUtil::toWideString(GetIPStr()) )
        << TOSTRINGW( GetPort() )
        << TOSTRINGWb( IsConnected() );
}

void KMSProxy::OnSocketError()
{
    KSession::OnSocketError();

    // 게임 클라이언트에게 연결이 끊어졌음을 알려야 한다.

    KEVENT_CLOSE_CONNECTION_NOT kNot;
    kNot.m_nCloseType = KEVENT_CLOSE_CONNECTION_NOT::CCT_NORMAL;
    kNot.m_strMessage = L"Connection Closed.";

    KSerializer ks;
    KEventPtr spEvent( CreateKEvent() );
    spEvent->m_usEventID = KMSEvent::EMS_CLOSE_CONNECTION_NOT;
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( kNot );
    ks.EndWriting();

    // windows msg sendmessage
    PostEvent( spEvent );
}

// Call to ProcessEventQueue (Client)
void KMSProxy::ProcessEvent( const KEventPtr& spEvent_ )
{
	PostEvent( spEvent_ );

	KSerializer     ks;
	ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    //switch( spEvent_->m_usEventID )
    //{
    // //CASE( EMS_VERIFY_ACCOUNT_ACK );
    ////_CASE( ECN_ADD_USER_ACK, KECN_REG_USER_DATA );
    //// CASE( ECN_SERVER_MIGRATION_ACK );
    //// CASE( ECN_KICK_USER_REQ );
    // default:
    //     // 이곳에서 이벤트 핸들러는, 꼭 필요한 것만 정의된다. 정의되지 않은 이벤트가 있을 수 있다.
    //     //START_LOG( cerr , "이벤트 핸들러가 정의되지 않았음. " << kEvent_ ) << END_LOG;
    //     break;
    //}
}