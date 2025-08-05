#include "MRUser.h"
#include <dbg/dbg.hpp>
#include "defaultFSM.h"
#include "MRSimLayer.h"
#include "MRDBLayer.h"
#include "NetError.h"
#include "MSEvent.h"
#include "Log4.h"
#include <boost/format.hpp>
//FILE_NAME_FOR_LOG

KMRUserPtr KMRUser::Create()
{
    return SiKMRSimLayer()->m_kActorManager.CreateActor<KMRUser>();
}

void KMRUser::AddManager( KMRUserPtr spMRUser_ )
{
    JIF( spMRUser_ );
    JIF( SiKMRSimLayer()->m_kActorManager.Add( spMRUser_ ) );
}

NiImplementRTTI( KMRUser, KActor );
ImplementException( KMRUser );

#define EVENT_TYPE  KMREvent
#define CLASS_TYPE  KMRUser

#define QUEUEING_EVENT_TO_DB( id, data )    SiKMRDBHandler()->QueueingEvent( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID, data )
#define QUEUEING_ID_TO_DB( id )             SiKMRDBHandler()->QueueingID( EVENT_TYPE::##id, m_strName.c_str(), m_dwUID )

KMRUser::KMRUser(void) 
: 
    m_dwUserUID( 0 )
{
    SetSendQueueSize( 1024 * 512 );
}

KMRUser::~KMRUser(void)
{
}

ImplToStringW( KMRUser )
{
    return START_TOSTRING_PARENTW( KActor );
}

void KMRUser::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

    switch( spEvent_->m_usEventID )
    {
   _CASE( EMR_VERIFY_ACCOUNT_REQ, KPacketNameOK );
    CASE( EMR_CLIENT_CONNECT_REPORT_REQ );
    CASE( EMR_REMOTE_QUEUEING_REQ );
   _CASE( EMR_CLIENT_DISCONNECT_REPORT_REQ, DWORD );
    CASE( EMR_REMOTE_QUEUEING_PACK_REQ );

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

void KMRUser::Tick()
{
    KActor::Tick();

    switch( GetStateID() ) {
    case KDefaultFSM::STATE_EXIT:
        SiKMRSimLayer()->m_kActorManager.ReserveDelete( m_strName );    // 자기 자신을 스스로 삭제할 수 없음.
        break;
    }
}

void KMRUser::OnDestroy()
{
    KSession::OnDestroy();

    switch( GetStateID() ) {
    case KDefaultFSM::STATE_CONNECTED:
    case KDefaultFSM::STATE_LOGINED:
        StateTransition( KDefaultFSM::INPUT_EXIT_GAME );    // OnDestroy()가 불린 현재 KSession::Tick 처리 중이며,
                                                            // KMRUser::Tick 처리 단계에서 종료 대기명단에 등록된다.
        break;
    }
}

_IMPL_ON_FUNC( EMR_VERIFY_ACCOUNT_REQ, KPacketNameOK )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_CONNECTED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_strName ) << END_LOG;

    KMRUserPtr spUser( SiKMRSimLayer()->m_kActorManager.GetByName<KMRUser>( kPacket_.m_strName ) );

    if( spUser != NULL )
    {
        SET_ERROR( ERR_VERIFY_04 ); // 동일서버간 이중접속 시도

        spUser->ReserveDestroy();   // 이전 접속자를 종료하는 부분

    }
    else
    {
        m_dwUserUID = kPacket_.m_nOK;
        LIF( SiKMRSimLayer()->m_kActorManager.Rename( GetName(), kPacket_.m_strName ) );

        StateTransition( KDefaultFSM::INPUT_VERIFICATION_OK );

        SET_ERROR( NET_OK );
    }


    int kPacket;
    kPacket = NetError::GetLastNetError();

    LOG_CONDITION( kPacket == NetError::NET_OK, clog, cerr )
        << BUILD_LOG( NetError::GetLastNetErrMsg() )
        << BUILD_LOG( kPacket )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( kPacket_.m_strName ) << END_LOG;

    SEND_PACKET( EMR_VERIFY_ACCOUNT_ACK );
}

IMPL_ON_FUNC( EMR_CLIENT_CONNECT_REPORT_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    KMRSimLayer::KMSUserInfo kInfo;
    kInfo.m_dwUserUID       = kPacket_.m_dwLogonUID;
    kInfo.m_strMsgServer    = GetName();

    bool bRet = SiKMRSimLayer()->AddMSUserInfo( kInfo );

    KEMR_CLIENT_CONNECT_REPORT_ACK kPacket;
    kPacket.m_dwLogonUID    = kPacket_.m_dwLogonUID;
    kPacket.m_nOK           = bRet ? NetError::NET_OK : NetError::ERR_UNKNOWN;

    LOG_CONDITION( bRet, clog, cerr )
        << BUILD_LOG( SiKMRSimLayer()->GetMSUserSize() )
        << BUILD_LOG( GetName() )
        << BUILD_LOG( kPacket_.m_dwLogonUID )
        << BUILD_LOG( kPacket_.m_mapFriendGroup.size() ) << END_LOG;

    SEND_PACKET( EMR_CLIENT_CONNECT_REPORT_ACK );

    // 모든 ms에게 현재 로그인 중인 친구 정보를 문의한다.
    SiKMRSimLayer()->m_kActorManager.SendToAll( KMREvent::EMR_LOGINNED_FRIEND_INFO_REQ, kPacket_ );
}

IMPL_ON_FUNC( EMR_REMOTE_QUEUEING_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_dwSenderUID )
        << BUILD_LOG( kPacket_.m_dwRecverUID )
        << BUILD_LOG( kPacket_.m_kData.GetLength() )
        << BUILD_LOG( kPacket_.m_bCheckRecv )
        << BUILD_LOG( kPacket_.m_bDirectSend ) << END_LOG;

    KMRSimLayer::KMSUserInfo kInfo;

    if( !SiKMRSimLayer()->GetMSUserInfo( kPacket_.m_dwRecverUID, kInfo ) )
    {
        if( kPacket_.m_bCheckRecv ) // 메세지 전송이 실패했음을 알려야 하는 경우
        {
            KEMR_REMOTE_QUEUEING_ACK kPacket;
            kPacket.m_bCheckRecv   = false;
            kPacket.m_bDirectSend  = false;
            kPacket.m_dwRecverUID  = kPacket_.m_dwSenderUID;
            kPacket.m_dwSenderUID  = kPacket_.m_dwRecverUID;
            KEvent kEvent;
            kEvent.m_usEventID  = KMSEvent::EMS_FRIEND_NOT_EXIST_NOT;
            kEvent.Serialize( kPacket_.m_dwRecverUID );
            KSerializer ks;
            ks.BeginWriting( &kPacket.m_kData );
            ks.Put( kEvent );
            ks.EndWriting();

            SEND_PACKET( EMR_REMOTE_QUEUEING_ACK );
        }

        return;
    }

    KMRUserPtr spMRUser( SiKMRSimLayer()->m_kActorManager.GetByName<KMRUser>( kInfo.m_strMsgServer ) );

    _JIF( spMRUser, return );

    LIF( spMRUser->SendPacket( KMREvent::EMR_REMOTE_QUEUEING_ACK, kPacket_ ) );
}

_IMPL_ON_FUNC( EMR_CLIENT_DISCONNECT_REPORT_REQ, DWORD )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));
    
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    SiKMRSimLayer()->DeleteMSUserInfo( kPacket_ );
}

IMPL_ON_FUNC( EMR_REMOTE_QUEUEING_PACK_REQ )
{
    VERIFY_STATE(( 1, KDefaultFSM::STATE_LOGINED ));

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_dwSenderUID )
        << BUILD_LOG( kPacket_.m_vecRecverUIDList.size() )
        << BUILD_LOG( kPacket_.m_kData.GetLength() )
        << BUILD_LOG( kPacket_.m_bCheckRecv )
        << BUILD_LOG( kPacket_.m_bDirectSend ) << END_LOG;

    KMRSimLayer::KMSUserInfo kInfo;
    std::vector<DWORD>::iterator vit;
    std::map< std::wstring, std::vector<DWORD> > mapSortRecverUID;
        
    for( vit = kPacket_.m_vecRecverUIDList.begin() ; vit != kPacket_.m_vecRecverUIDList.end() ; ++vit )
    {
        if( !SiKMRSimLayer()->GetMSUserInfo( *vit, kInfo ) )
        {
            if( kPacket_.m_bCheckRecv ) // 메세지 전송이 실패했음을 알려야 하는 경우
            {
                KEMR_REMOTE_QUEUEING_PACK_ACK kPacket;
                kPacket.m_bCheckRecv   = false;
                kPacket.m_bDirectSend  = false;
                kPacket.m_vecRecverUIDList.push_back( kPacket_.m_dwSenderUID );
                kPacket.m_dwSenderUID  = *vit;
                KEvent kEvent;
                kEvent.m_usEventID  = KMSEvent::EMS_S2_FRIEND_NOT_EXIST_NOT;
                kEvent.Serialize( *vit );
                KSerializer ks;
                ks.BeginWriting( &kPacket.m_kData );
                ks.Put( kEvent );
                ks.EndWriting();

                SEND_PACKET( EMR_REMOTE_QUEUEING_PACK_ACK );
            }
            continue;
        }
        else        //유저 정보를 찾앗다!!!
            mapSortRecverUID[kInfo.m_strMsgServer].push_back( *vit );   //메신저 서버별로 친구의 UID를 모아야함...
    }
    // iterator for MRUser
    std::map< std::wstring, std::vector<DWORD> >::iterator mit;
    for( mit =  mapSortRecverUID.begin() ; mit != mapSortRecverUID.end() ; ++mit )
    {
        KMRUserPtr spMRUser( SiKMRSimLayer()->m_kActorManager.GetByName<KMRUser>( mit->first ) );
        
        _JIF( spMRUser, return );

        kPacket_.m_vecRecverUIDList.clear();
        kPacket_.m_vecRecverUIDList = mapSortRecverUID[mit->first];

        START_LOG( clog, L"Remote_Queueing_Pack Data" )
            << BUILD_LOG( kPacket_.m_dwSenderUID )
            << BUILD_LOG( kPacket_.m_vecRecverUIDList.size() ) << END_LOG;

        LIF( spMRUser->SendPacket( KMREvent::EMR_REMOTE_QUEUEING_PACK_ACK, kPacket_ ) );
    }
}
