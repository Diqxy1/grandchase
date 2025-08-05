#include "MRProxy.h"
#include "NetError.h"
#include "MSSimLayer.h"
#include "MSEvent.h"
#include "MSPacket.h"
#include "MSUser.h"
//FILE_NAME_FOR_LOG

NiImplementRTTI( KMRProxy, KActorProxy );


#define EVENT_TYPE KMREvent
#define CLASS_TYPE KMRProxy


KMRProxy::KMRProxy(void)
: m_iID( 0 )
{
    SetSendQueueSize( 1024 * 512 );
}

KMRProxy::~KMRProxy(void)
{
}

ImplToStringW( KMRProxy )
{
    return START_TOSTRING_PARENTW( KActorProxy )
        << TOSTRINGW( KncUtil::toWideString(GetIPStr()) )
        << TOSTRINGW( GetPort() )
        << TOSTRINGWb( IsConnected() );
}

// Call to ProcessEventQueue (Client)
void KMRProxy::ProcessEvent( const KEventPtr& spEvent_ )
{
     KSerializer     ks;
     KSerBuffer      kbuff;

     ks.BeginReading( const_cast<KSerBuffer*>( &spEvent_->m_kbuff ) );

    switch( spEvent_->m_usEventID )
    {
   _CASE( EMR_VERIFY_ACCOUNT_ACK, int );
    CASE( EMR_CLIENT_CONNECT_REPORT_ACK );
    CASE( EMR_LOGINNED_FRIEND_INFO_REQ );
    CASE( EMR_REMOTE_QUEUEING_ACK );
   _CASE( EMR_CLIENT_DISCONNECT_REPORT_ACK, KPacketNameOK );
    CASE( EMR_KICK_CLIENT_NOT );
    CASE( EMR_REMOTE_QUEUEING_PACK_ACK );

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetEventIDString() ) << END_LOG;
    }
}

_IMPL_ON_FUNC( EMR_VERIFY_ACCOUNT_ACK, int )
{
    LOG_CONDITION( kPacket_ == 0, clog, cerr )
        << BUILD_LOG( kPacket_ ) << END_LOG;

    m_nAckOK = kPacket_;

    SetEvent( m_spEventAck.get() );
}

IMPL_ON_FUNC( EMR_CLIENT_CONNECT_REPORT_ACK )
{
    LOG_CONDITION( kPacket_.m_nOK == NetError::NET_OK, clog, cerr )
        << BUILD_LOG( kPacket_.m_dwLogonUID )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( NetError::GetErrStr( kPacket_.m_nOK ) ) << END_LOG;
}

_IMPL_ON_FUNC( EMR_CLIENT_DISCONNECT_REPORT_ACK, KPacketNameOK )
{
    LOG_CONDITION( kPacket_.m_nOK == NetError::NET_OK, clog, cerr )
        << BUILD_LOG( kPacket_.m_strName )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( NetError::GetErrStr( kPacket_.m_nOK ) ) << END_LOG;
}

void KMRProxy::OnDestroy()
{
    KActorProxy::OnDestroy();

    SiKMRConnector()->DestroyMRProxy( m_iID );
}

IMPL_ON_FUNC( EMR_LOGINNED_FRIEND_INFO_REQ )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_dwLogonUID )
        << BUILD_LOG( kPacket_.m_mapFriendGroup.size() ) << END_LOG;

    SiKMSSimLayer()->GetLoginnedFriendGroupInfo_S2( kPacket_ );
}

IMPL_ON_FUNC( EMR_REMOTE_QUEUEING_ACK )
{
    KActorPtr spActor( SiKMSSimLayer()->m_kActorManager.GetByUID( kPacket_.m_dwRecverUID ) );

    if( spActor )
    {
        if( kPacket_.m_bDirectSend )
            LIF( spActor->SendPacket( kPacket_.m_kData, L"Remote Data" ) );
        else
            spActor->QueueingEvent( kPacket_.m_kData );
    }
    else
    {
        if( kPacket_.m_bCheckRecv ) // 메세지 전송이 실패했음을 알려야 하는 경우.
        {
            KSerBuffer kBuffer;
            kBuffer.Clear();

            KSerializer ks;
            ks.BeginWriting( &kBuffer );
            ks.Put( kPacket_.m_dwRecverUID );
            ks.EndWriting();

            RemoteQueueing( kPacket_.m_dwRecverUID,
                kPacket_.m_dwSenderUID,
                KMSEvent::EMS_S2_FRIEND_NOT_EXIST_NOT,
                kBuffer,
                false, false );
        }
    }

    LOG_CONDITION( spActor, clog, cwarn )
        << BUILD_LOG( kPacket_.m_dwSenderUID )
        << BUILD_LOG( kPacket_.m_dwRecverUID )
        << BUILD_LOG( kPacket_.m_bCheckRecv )
        << BUILD_LOG( kPacket_.m_bDirectSend )
        << BUILD_LOG( kPacket_.m_kData.GetLength() ) << END_LOG;
}

IMPL_ON_FUNC( EMR_REMOTE_QUEUEING_PACK_ACK )
{
    KSerBuffer  kData_ = kPacket_.m_kData;
    std::vector<DWORD>::iterator vit;
    //UID 리스트를 돌면서 유저에게 이벤트를 전송한다..sendtomyfriend로 보낼때만 성립..

    for( vit = kPacket_.m_vecRecverUIDList.begin() ; vit != kPacket_.m_vecRecverUIDList.end() ; ++vit )
    {
        KActorPtr spActor( SiKMSSimLayer()->m_kActorManager.GetByUID( *vit ) );

        kPacket_.m_kData = kData_;

        if( spActor )
            spActor->QueueingEvent( kPacket_.m_kData );
        
        LOG_CONDITION( spActor, clog, cwarn )
            << BUILD_LOG( kPacket_.m_dwSenderUID )
            << BUILD_LOG( *vit )
            << BUILD_LOG( kPacket_.m_bCheckRecv )
            << BUILD_LOG( kPacket_.m_bDirectSend )
            << BUILD_LOG( kPacket_.m_kData.GetLength() ) << END_LOG;
    }    
}

IMPL_ON_FUNC( EMR_KICK_CLIENT_NOT )
{
    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_dwUserUID )
        << BUILD_LOG( kPacket_.m_nOK )
        << BUILD_LOG( NetError::GetErrStr( kPacket_.m_nOK ) ) << END_LOG;

    KMSUserPtr spUser( SiKMSSimLayer()->m_kActorManager.GetByUID<KMSUser>( kPacket_.m_dwUserUID ) );

    if( spUser )
    {
        spUser->SendPacket( KMSEvent::EMS_KICK_NOT, kPacket_.m_nOK );
        spUser->ReserveDestroy();
    }
    else
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( kPacket_.m_dwUserUID )
            << BUILD_LOG( kPacket_.m_nOK )
            << BUILD_LOG( NetError::GetErrStr( kPacket_.m_nOK ) )
            << BUILD_LOG( SiKMSSimLayer()->m_kActorManager.GetCount() ) << END_LOG;
    }

}
void KMRProxy::OnSocketError()
{
    KSession::OnSocketError();

    START_LOG( cerr, L"Call Socket Err, Queue Size : " << GetQueueSize() ) << END_LOG;
}

void KMRProxy::RemoveIOCPKey()
{
    DeleteCompKey();
}

bool KMRProxy::RemoteQueueing( DWORD dwSenderUID_,
                              DWORD dwRecverUID_,
                              unsigned short usEventID_,
                              KSerBuffer& kBuf_,
                              bool bCheckRecv_,
                              bool bDirectSend_ )
{
    KEvent kEvent;
    kEvent.m_usEventID = usEventID_;
    kEvent.m_kbuff = kBuf_;

    KEMR_REMOTE_QUEUEING_REQ kReq;
    kReq.m_dwSenderUID  = dwSenderUID_;
    kReq.m_dwRecverUID  = dwRecverUID_;
    kReq.m_bCheckRecv   = bCheckRecv_;
    kReq.m_bDirectSend  = bDirectSend_;

    KSerializer ks;
    ks.BeginWriting( &kReq.m_kData );
    ks.Put( kEvent );
    ks.EndWriting();

    return SendPacket( KMREvent::EMR_REMOTE_QUEUEING_REQ, kReq );
}

bool KMRProxy::RemoteQueueingPack( DWORD dwSenderUID_,
                                  std::vector<DWORD> vecRecverUIDList_,
                                  unsigned short usEventID_,
                                  KSerBuffer& kBuf_,
                                  bool bCheckRecv_,
                                  bool bDirectSend_ )
{
    KEvent kEvent;
    kEvent.m_usEventID = usEventID_;
    kEvent.m_kbuff = kBuf_;

    std::vector<DWORD>::iterator vit;
    KEMR_REMOTE_QUEUEING_PACK_REQ kReq;
    kReq.m_dwSenderUID  = dwSenderUID_;

    for( vit = vecRecverUIDList_.begin() ; vit != vecRecverUIDList_.end() ; ++vit )
        kReq.m_vecRecverUIDList.push_back( *vit );

    kReq.m_bCheckRecv   = bCheckRecv_;
    kReq.m_bDirectSend  = bDirectSend_;

    KSerializer ks;
    ks.BeginWriting( &kReq.m_kData );
    ks.Put( kEvent );
    ks.EndWriting();

    return SendPacket( KMREvent::EMR_REMOTE_QUEUEING_PACK_REQ, kReq, false, true );
}