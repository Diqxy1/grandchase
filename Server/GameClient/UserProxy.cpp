#include "UserProxy.h"
#include "UserEvent.h"
#include "UserPacket.h"
#include "NetError.h"
#include <KncSecurity/KncSecurity.h>
//FILE_NAME_FOR_LOG

NiImplementRTTI( KUserProxy, KActorProxy );
ImplementSingleton( KUserProxy )

#define EVENT_TYPE KUserEvent
#define CLASS_TYPE KUserProxy

KUserProxy::KUserProxy(void)
{
}

KUserProxy::~KUserProxy(void)
{
}

void KUserProxy::InitForClient()
{
    NetCommon::InitWinSock();
    KncSecurity::InitSecurity();
}

void KUserProxy::ReleaseForClient()
{
    KncSecurity::ReleaseSecurity();
}

KEvent* KUserProxy::CreateKEvent()
{
    return new KUserEvent;
}

ImplToStringW( KUserProxy )
{
    return START_TOSTRING_PARENTW( KActorProxy );
}

bool KUserProxy::RequestServerMigration( IN const KServerInfo& kInfo_, DWORD dwTimeOut_ /* = 3000  */)
{
    _JIF( SendPacket( KUserEvent::EVENT_SERVER_MIGRATION_REQ, kInfo_.m_strName ), false );

    m_nAckOK = -99; // unused-error code.

    switch( ::WaitForSingleObject( m_spEventAck.get(), dwTimeOut_ ) ) {
        case WAIT_OBJECT_0: // ack를 제대로 받은 경우.
            return (m_nAckOK == 0);
            break;
        case WAIT_TIMEOUT: // 시간 초과
            START_LOG( cerr, L"verify account timeout." ) << END_LOG;
            break;
        default:
            START_LOG( cerr, L"invalide event object." ) << END_LOG;
    }

    return false;
}


void KUserProxy::OnSocketError()
{    
    KSession::OnSocketError();

    // 게임 클라이언트에게 연결이 끊어졌음을 알려야 한다.

    KEVENT_CLOSE_CONNECTION_NOT kNot;
    kNot.m_nCloseType = KEVENT_CLOSE_CONNECTION_NOT::CCT_NORMAL;
    kNot.m_strMessage = L"Connection Closed.";

    KSerializer ks;
    KEventPtr spEvent( CreateKEvent() );
    spEvent->m_usEventID = KUserEvent::EVENT_CLOSE_CONNECTION_NOT;
    ks.BeginWriting( &spEvent->m_kbuff );
    ks.Put( kNot );
    ks.EndWriting();

    // windows msg sendmessage
    PostEvent( spEvent );
}

void KUserProxy::ProcessEvent( const KEventPtr& spEvent_ )
{
    PostEvent( spEvent_ );

    KSerializer     ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    switch( spEvent_->m_usEventID )
    {
        //CASE( EVENT_VERIFY_ACCOUNT_ACK );
        CASE( EVENT_GMTOOL_AUTHENTICATION_ACK );
       _CASE( EVENT_SERVER_MIGRATION_ACK, int );
        //CASE( EVENT_CREATE_ROOM_ACK );

       // CASE( EVENT_WHERE_ACK );
       // CASE( EVENT_WHISPER_ACK );
       // CASE( EVENT_INVITE_NOT );
       // CASE( EVENT_OPEN_USER_INFO_ACK );

    default:     
        // 이곳에서 이벤트 핸들러는, 꼭 필요한 것만 정의된다. 정의되지 않은 이벤트가 있을 수 있다.
        //START_LOG( cerr , "이벤트 핸들러가 정의되지 않았음. " << kEvent_ ) << END_LOG;
        break;
    } 
}

_IMPL_ON_FUNC( EVENT_SERVER_MIGRATION_ACK, int )
{
    m_nAckOK = kPacket_;

    SetEvent( m_spEventAck.get() );
}

IMPL_ON_FUNC( EVENT_GMTOOL_AUTHENTICATION_ACK )
{
    m_nAckOK = kPacket_.m_nOK;

    SetEvent( m_spEventAck.get() );

    if( kPacket_.m_nOK != NetError::NET_OK )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOGc( kPacket_.m_nOK )
            << END_LOG;
        return;
    }

    START_LOG_WITH_NAME( clog )
        << BUILD_LOGc( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_strLogin )
        << BUILD_LOGc( kPacket_.m_cAuthLevel )
        << END_LOG;

    SetName( kPacket_.m_strLogin );
}
/*
IMPL_ON_FUNC( EVENT_VERIFY_ACCOUNT_ACK )
{
    m_nAckOK = kPacket_.m_ucOK;

    SetEvent( m_spEventAck.get() );

    if( kPacket_.m_ucOK != 0 )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOGc( kPacket_.m_ucOK )
            << END_LOG;

        if( kPacket_.m_ucOK != 11 ) // 대만 gash 인증 실패시, 접속을 끊지 않는다.
        {
            ReserveDestroy();
        }
        return;
    }

    assert( !kPacket_.m_mapDifficulty.empty() );
    
    START_LOG_WITH_NAME( clog )
        << BUILD_LOGc( kPacket_.m_ucOK )
        << BUILD_LOG( kPacket_.m_strServerName )
        << BUILD_LOG( kPacket_.m_strLogin )
        //<< BUILD_LOG( kPacket_.m_iGamePoint )
        << BUILD_LOG( NetCommon::CalcIP( kPacket_.m_dwIP ) )
        //<< BUILD_LOG( kPacket_.m_kGuildInfo.m_iId )
        << BUILD_LOG( kPacket_.m_dwServerType )
        << BUILD_LOG( kPacket_.m_mapDifficulty.size() )
        << END_LOG;

    SetName( kPacket_.m_strLogin );
}*/

//IMPL_ON_FUNC( EVENT_CREATE_ROOM_ACK )
//{
//    m_nAckOK = kPacket_.m_nOK;
//    SetEvent( m_spEventAck.get() );
//
//    m_usRoomID = kPacket_.m_kRoomInfo.m_usRoomID;
//}
