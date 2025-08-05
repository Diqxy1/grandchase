#include "stdafx.h"
#include "MSProxy.h"
#include "NetError.h"

NiImplementRTTI( KMSProxy, KActorProxy );
ImplementSingleton( KMSProxy );
//ImplementProxy( KMSProxy );

#ifdef EVENT_TYPE
#undef EVENT_TYPE
#endif
#define EVENT_TYPE KMSEvent

#ifdef CLASS_TYPE
#undef CLASS_TYPE
#endif
#define CLASS_TYPE KMSProxy


KMSProxy::KMSProxy(void)
{
}

KMSProxy::~KMSProxy(void)
{
    End( INFINITE ); // 무한 대기 한다.
    // 말단 소멸 자에서 스레드 함수를 종료 시키도록 한다.
    // 부모에서 해주면 좋지 않다.
}

ImplToStringW( KMSProxy )
{
    return START_TOSTRING_PARENTW( KActorProxy )
        << TOSTRINGW( KncUtil::toWideString(GetIPStr()) )
        << TOSTRINGW( GetPort() )
        << TOSTRINGWb( IsConnected() );
}

void KMSProxy::OnSocketError( int nType )
{
    KSession::OnSocketError( nType );

    // 게임 클라이언트에게 연결이 끊어졌음을 알려야 한다.

	//     KEVENT_CLOSE_CONNECTION_NOT kNot;
	//     kNot.m_nCloseType = KEVENT_CLOSE_CONNECTION_NOT::CCT_NORMAL;
	//     kNot.m_strMessage = L"Connection Closed.";

	//     KSerializer ks;
	KEventPtr spEvent( CreateKEvent() );
	spEvent->m_usEventID = KMSEvent::EMS_CLOSE_CONNECTION_NOT;
	//     ks.BeginWriting( &spEvent->m_kbuff );
	//     ks.Put( kNot );
	//     ks.EndWriting();

    // windows msg sendmessage
    PostEvent( spEvent );
}

// Call to ProcessEventQueue (Client)
void KMSProxy::ProcessEvent( const KEventPtr& spEvent_ )
{
	PostEvent( spEvent_ );

	KSerializer     ks;
	ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    switch( spEvent_->m_usEventID )
    {
    CASE( EMS_VERIFY_ACCOUNT_ACK );
    CASE( EMS_S2_VERIFY_ACCOUNT_ACK );
   //_CASE( ECN_ADD_USER_ACK, KECN_REG_USER_DATA );
   // CASE( ECN_SERVER_MIGRATION_ACK );
   // CASE( ECN_KICK_USER_REQ );
    default:
        // 이곳에서 이벤트 핸들러는, 꼭 필요한 것만 정의된다. 정의되지 않은 이벤트가 있을 수 있다.
        //START_LOG( cerr , "이벤트 핸들러가 정의되지 않았음. " << kEvent_ );
        break;
    }
}

IMPL_ON_FUNC( EMS_VERIFY_ACCOUNT_ACK )
{
    m_nAckOK = kPacket_.m_nOK;

    SetEvent( m_spEventAck.get() );

    LOG_CONDITION( m_nAckOK == NetError::NET_OK, clog, cerr )
        << BUILD_LOG( m_nAckOK )
        << BUILD_LOG( NetError::GetErrStr( m_nAckOK ) );
}

IMPL_ON_FUNC( EMS_S2_VERIFY_ACCOUNT_ACK )
{
    m_nAckOK = kPacket_.m_nOK;

    SetEvent( m_spEventAck.get() );

    LOG_CONDITION( m_nAckOK == NetError::NET_OK, clog, cerr )
        << BUILD_LOG( m_nAckOK )
        << BUILD_LOG( NetError::GetErrStr( m_nAckOK ) );
}

//IMPL_ON_FUNC( ECN_KICK_USER_REQ )
//{
//    KPacketNameOK kPacket;
//    kPacket.m_strName   = kPacket_.m_strUserName;
//    kPacket.m_nOK       = NetError::ERR_UNKNOWN;
//
//    START_LOG_WITH_NAME( cwarn )
//        << BUILD_LOG( kPacket_.m_strUserName )
//        << BUILD_LOG( kPacket_.m_strMsg )
//        << BUILD_LOG( kPacket_.m_bStrongCheck );
//
//    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( kPacket_.m_strUserName ) );
//
//    if( spUser == NULL )
//    {
//        if( kPacket_.m_bStrongCheck )
//        {
//            SET_ERROR( ERR_INTERNAL_02 );
//            PRINT_LASTERROR;
//        }
//    }
//    else
//    {
//        // 연결이 끊기기 전 메세지를 보낸다.
//        KEVENT_CLOSE_CONNECTION_NOT kNot;
//        kNot.m_bAbnormal    = true;
//        kNot.m_strMessage   = kPacket_.m_strMsg;
//        spUser->SendPacket( KUserEvent::EVENT_CLOSE_CONNECTION_NOT, kNot );
//        
//        spUser->ReserveDestroy();
//
//        SET_ERROR( NET_OK );
//    }
//
//    kPacket.m_nOK = NetError::GetLastError();
//
//    SendPacket( KMSEvent::ECN_KICK_USER_ACK, kPacket );
//    START_LOG( cerr, kPacket_.m_strMsg << L" Name : " << kPacket_.m_strUserName );
//}
//
//_IMPL_ON_FUNC( ECN_ADD_USER_ACK, KECN_REG_USER_DATA )
//{
//    START_LOG( clog, L"Name : " << kPacket_.m_strName );
//
//    //성공 실패를 유저에게 전달한다.
//    
//
//    // 접속된 유저에게 메시지 전송 후 연결 종료 시킴 
//    KUserPtr spUser( SiKGSSimLayer()->m_kActorManager.GetByName<KUser>( kPacket_.m_strName ) );
//    if( !spUser)
//    {
//        START_LOG( cerr2, L"해당 유저를 찾을수 없습니다.: " << kPacket_.m_strName );
//        return;
//    }
//
//    if( kPacket_.m_nOK == NetError::NET_OK )
//    {
//        spUser->QueueingEvent( KUserEvent::ECN_ADD_USER_ACK, kPacket_ );
//        START_LOG( clog, L"정상접속 : " << kPacket_.m_strName );
//    }
//    else
//    {
//        // 연결이 끊기기 전 메세지를 보낸다.
//        KEVENT_CLOSE_CONNECTION_NOT kNot;
//        kNot.m_bAbnormal    = true;
//        kNot.m_strMessage   = SiKGameServer()->m_stringTable.GetValue( L"str_02" );
//
//        spUser->SendPacket( KUserEvent::EVENT_CLOSE_CONNECTION_NOT, kNot );
//
//        spUser->ReserveDestroy();
//
//        START_LOG( cerr, L"타 서버간 이중접속. Name : " << kPacket_.m_strName );
//    }   
//}
//
//IMPL_ON_FUNC( ECN_SERVER_MIGRATION_ACK )
//{
//    //START_LOG( clog, L"Name : " << kPacket_.m_strUserName );
//    //std::wstring strLogin_ = kPacket_.m_strUserName;
//
//    int kPacket;
//    kPacket.m_nOK = kPacket_.m_nOK;
//    //QUEUING_ACK_TO_USER( EVENT_SERVER_MIGRATION_ACK );
//}
//
//_IMPL_ON_FUNC( ECN_DEL_USER_ACK, KECN_REG_USER_DATA )
//{
//    if( kPacket_.m_bServerMigration )
//    {
//        int kPacket;
//        kPacket.m_nOK = kPacket_.m_nOK;
//        std::wstring strLogin_ = kPacket_.m_strName;
//        QUEUING_ACK_TO_USER( KUserEvent::EVENT_SYNC_CACHED_USERDATA_ACK );
//    }
//    
//}
//
//_IMPL_ON_FUNC( EVENT_NOT_MSG_NOT, std::wstring )
//{
//    START_LOG( clog, L"" )
//        << BUILD_LOG( kPacket_.m_strMsg );
//
//    SiKChannelManager()->Send_NotMsg( kPacket_.m_strMsg.c_str() );
//}
