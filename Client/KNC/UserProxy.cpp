#include "stdafx.h"
#include "UserProxy.h"
#include "NetError.h"

#ifndef NO_GAMEGUARD
#include "NPGameLib.h"
#endif

#include "../GlobalVariables.h"

#if defined(USE_XTRAP) && !defined(_DEBUG)
#include "Xtrap_C_Interface.h"
#endif

NiImplementRTTI( KUserProxy, KActorProxy );
ImplementSingleton( KUserProxy )

#ifdef EVENT_TYPE
#undef EVENT_TYPE
#endif
#define EVENT_TYPE KUserEvent

#ifdef CLASS_TYPE
#undef CLASS_TYPE
#endif
#define CLASS_TYPE KUserProxy

KUserProxy::KUserProxy(void)
{
}

KUserProxy::~KUserProxy(void)
{
    End( INFINITE );
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
            START_LOG( cerr, L"verify account timeout." );
            break;
        default:
            START_LOG( cerr, L"invalide event object." );
    }

    return false;
}


void KUserProxy::OnSocketError( int nType )
{    
    KSession::OnSocketError( nType );

    // 게임 클라이언트에게 연결이 끊어졌음을 알려야 한다.

	KEVENT_CLOSE_CONNECTION_NOT kNot;
	kNot.m_nCloseType = KEVENT_CLOSE_CONNECTION_NOT::CCT_NORMAL;
    kNot.m_nSocketErr = static_cast<KEVENT_CLOSE_CONNECTION_NOT::EM_SOCKET_ERROR_TYPE>( nType );
	//     kNot.m_strMessage = L"Connection Closed.";

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
        CASE( EVENT_VERIFY_ACCOUNT_ACK );
        CASE( EVENT_GMTOOL_AUTHENTICATION_ACK );
       _CASE( EVENT_SERVER_MIGRATION_ACK, int );

    default:     
        // 이곳에서 이벤트 핸들러는, 꼭 필요한 것만 정의된다. 정의되지 않은 이벤트가 있을 수 있다.
        //START_LOG( cerr , "이벤트 핸들러가 정의되지 않았음. " << kEvent_ );
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

    START_LOG_WITH_NAME( cout )
        << BUILD_LOGc( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_strLogin )
        << BUILD_LOGc( kPacket_.m_cAuthLevel );

    SetName( kPacket_.m_strLogin );
}

IMPL_ON_FUNC( EVENT_VERIFY_ACCOUNT_ACK )
{
    m_nAckOK = kPacket_.m_ucOK;

    SetEvent( m_spEventAck.get() );

    if( kPacket_.m_ucOK != 0 )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOGc( kPacket_.m_ucOK )
            << END_LOG;

        if( kPacket_.m_ucOK != 11 && kPacket_.m_ucOK != 14 ) // 대만 gash 인증 실패시, 접속을 끊지 않는다.
        {
            ReserveDestroy();
        }
        return;
    }

    assert( !kPacket_.m_mapDifficulty.empty() );
    
    START_LOG_WITH_NAME( cout )
        << BUILD_LOGc( kPacket_.m_ucOK )
        << BUILD_LOG( kPacket_.m_strServerName )
        << BUILD_LOG( kPacket_.m_strLogin )
        //<< BUILD_LOG( kPacket_.m_iGamePoint )
        << BUILD_LOG( NetCommon::CalcIP( kPacket_.m_dwIP ) )
        //<< BUILD_LOG( kPacket_.m_kGuildInfo.m_iId )
        << BUILD_LOG( kPacket_.m_dwServerType )
        << BUILD_LOG( kPacket_.m_mapDifficulty.size() );

    SetName( kPacket_.m_strLogin );
}

//IMPL_ON_FUNC( EVENT_CHANNEL_LIST_NOT )
//{
//
//    START_LOG( cout, L"....." )
//        << BUILD_LOG( kPacket_.m_vecChannelInfo.size() );
//}

_IMPL_ON_FUNC( EVENT_NPGG_AUTH_REQ, KSerBuffer )
{
#ifdef NO_GAMEGUARD
    return;
#else
    // 121121. nodefeat. 기존에는 구조체로 받아서 구조체 사이즈만큼 read하면 됐음
    BYTE packet[4096];
    if( kPacket_.Read( packet, kPacket_.GetLength() ) )
    {
        // 세번째 인자는 일반적으로 사용하지 않아도 무관하다고 메뉴얼에 나와 있음
        g_CNPGameLib.Auth3( packet, kPacket_.GetLength(), 0 );
    }
#endif
}

_IMPL_ON_FUNC( EVENT_XTRAP_AUTH_REQ, KSerBuffer )
{
#if defined(USE_XTRAP) && !defined( _DEBUG)
    char caPacketBuffer[320] ={0,};
	char caPacketBufferOut[320] = {0,};

    LIF( kPacket_.Read( (void*)caPacketBuffer, sizeof(caPacketBuffer) ) );
	kPacket_.Clear();

    SOCKET sock = GetSockObj()->GetSocket();

	DWORD dwReturn = XTrap_CS_Step2_M( (LPCVOID)caPacketBuffer, (LPVOID)caPacketBufferOut, 
                                        XTRAP_PROTECT_PE | XTRAP_PROTECT_TEXT |XTRAP_PROTECT_EXCEPT_VIRUS, sock);

	kPacket_.Write( (void*)caPacketBufferOut, sizeof(caPacketBufferOut) );
	LIF( SendPacket( KUserEvent::EVENT_XTRAP_AUTH_ACK, kPacket_) );
#endif
}
