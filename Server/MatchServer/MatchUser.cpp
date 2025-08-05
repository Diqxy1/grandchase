#include "MatchUser.h"
#include <dbg/dbg.hpp>
#include "defaultFSM.h"
#include "MatchServer.h"
#include "MatchSimLayer.h"
#include "MatchDBLayer.h"
#include "MatchManager.h"
#include "NetError.h"
#include "SlangFilter.h"
#include "support_MatchUser.h"
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

#define EVENT_TYPE KMatchEvent
#define CLASS_TYPE KMatchUser

NiImplementRTTI( KMatchUser, KActor );
ImplementException( KMatchUser );

KMatchUserPtr KMatchUser::Create()
{
    return SiKMatchSimLayer()->m_kActorManager.CreateActor<KMatchUser>();
}

void KMatchUser::AddManager( KMatchUserPtr spMatchUser_ )
{
    JIF( spMatchUser_ );
    JIF( SiKMatchSimLayer()->m_kActorManager.Add( spMatchUser_ ) );
}

KMatchUser::KMatchUser(void)
{
}

KMatchUser::~KMatchUser(void)
{
}

ImplToStringW( KMatchUser )
{
    return START_TOSTRING_PARENTW( KActor );
}

void KMatchUser::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

    switch( spEvent_->m_usEventID )
    {
     _CASE( EVENT_MATCH_VERIFY_ACCOUNT_REQ, KSimpleServerInfo );
      CASE( EVENT_MATCH_ADD_USER_REQ );
     _CASE( EVENT_MATCH_DEL_USER_REQ, KPartyData );

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString() ) << END_LOG;
    }

    dwElapTime = ::GetTickCount() - dwElapTime;
    if( dwElapTime > 3000 )
    {
        START_LOG_WITH_NAME( cwarn )
            << BUILD_LOG( spEvent_->GetEventIDString() )
            << BUILD_LOG( spEvent_->m_usEventID )
            << BUILD_LOG( dwElapTime ) << END_LOG;
    }
}

void KMatchUser::Tick()
{
    KActor::Tick();

    switch( GetStateID() ) { // 서버 상태체크 추가.
    case KDefaultFSM::STATE_EXIT:
        SiKMatchSimLayer()->m_kActorManager.ReserveDelete( m_strName );
        break;
    }
}

void KMatchUser::OnDestroy()
{
    KSession::OnDestroy();

    DWORD dwUID = GetUID();
    KIntEventPtr spEvent( new KIntEvent );
    SiKMatchManager()->QueueingEventAll( KMatchEvent::EVENT_MATCH_DISCONNECT_GAMESERVER_NOT, dwUID, dwUID );

    switch( GetStateID() ) {
    case KDefaultFSM::STATE_CONNECTED:
    case KDefaultFSM::STATE_LOGINED:
        StateTransition( KDefaultFSM::INPUT_EXIT_GAME );
        break;
    }
}

_IMPL_ON_FUNC( EVENT_MATCH_VERIFY_ACCOUNT_REQ, KSimpleServerInfo )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_CONNECTED ));

    KMatchUserPtr spMatchUser;
    KEVENT_MATCH_VERIFY_ACCOUNT_ACK kPacket;
    kPacket.m_nOK = -99;
    kPacket.m_kServerInfo = kPacket_;

    SET_ERROR( ERR_UNKNOWN );

    // 보고된 아이피가 소켓에서 얻은 것과 다름.
    if ( KncUtil::toWideString( GetIPStr() ) != kPacket_.m_strIP ) {
        SET_ERR_GOTO( ERR_VERIFY_08, END_PROC ); // 불량 유저가 접속 시도
    }

    // 서버/클라이언트간 프로토콜이 다름
    if ( kPacket_.m_nProtocolVer != SiKMatchServer()->m_nProtocolVer ) {
        SET_ERR_GOTO( ERR_VERIFY_07, END_PROC );
    }

    // 이중접속 시도
    spMatchUser = SiKMatchSimLayer()->m_kActorManager.GetByName<KMatchUser>( kPacket_.m_strName );
    if ( spMatchUser ) {
        SET_ERR_GOTO( ERR_VERIFY_04, END_PROC ); 
    }

    SetUID( kPacket_.m_dwUID );
    m_kServerInfo = kPacket.m_kServerInfo;

    // rename, UID 등록
    LIF( SiKMatchSimLayer()->m_kActorManager.Rename( GetName(), m_kServerInfo.m_strName ) );
    LIF( SiKMatchSimLayer()->m_kActorManager.RegByUID( *this ) );

    StateTransition( KDefaultFSM::INPUT_VERIFICATION_OK );
    SET_ERROR( NET_OK );

END_PROC:
    kPacket.m_nOK = NetError::GetLastNetError();

    _LOG_SUCCESS( NetError::GetLastNetError() == NetError::NET_OK, L"Ret : " << NetError::GetErrStr( kPacket.m_nOK ) )
        << BUILD_LOG( GetIPStr() )
        << BUILD_LOG( SiKMatchServer()->m_nProtocolVer )
        << BUILD_LOG( kPacket_.m_dwUID )
        << BUILD_LOG( kPacket_.m_strIP )
        << BUILD_LOG( kPacket_.m_usPort )
        << BUILD_LOG( kPacket_.m_strName )
        << BUILD_LOG( kPacket_.m_nProtocolVer ) << END_LOG;

    SEND_PACKET( EVENT_MATCH_VERIFY_ACCOUNT_ACK );
}

IMPL_ON_FUNC( EVENT_MATCH_ADD_USER_REQ )
{
    kPacket_.m_nStartScope = SiKMatchManager()->GetStartSearchScope();
    // 매칭 그룹이 생성되어 있지 않으면 매칭 그룹 객체 생성.
    SiKMatchManager()->AddMatchInfo( kPacket_.m_kPartyData );
    // 해당 매칭 그룹으로 Queuing..,
    SiKMatchManager()->QueueingEvent( KMatchEvent::EVENT_MATCH_ADD_USER_REQ, kPacket_.m_kPartyData.m_nModeInfo, GetUID(), kPacket_ );
}

_IMPL_ON_FUNC( EVENT_MATCH_DEL_USER_REQ, KPartyData )
{
    SiKMatchManager()->QueueingEvent( KMatchEvent::EVENT_MATCH_DEL_USER_REQ, kPacket_.m_nModeInfo, GetUID(), kPacket_ );
}