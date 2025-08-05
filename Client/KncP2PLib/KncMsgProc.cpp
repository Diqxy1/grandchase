#include "stdafx.h"
#include "../P2P.h"
#include "../knc/ClientErr.h"
#include <Serializer/Serializer.h>
#include <Serializer/SerBuffer.h>
#include ".\kncmsgproc.h"
#include "dbg/dbg.hpp"
#include "../KSingleton.h"
#include "../KGCPerformanceCounter.h"

#ifdef CLASS_TYPE
    #undef CLASS_TYPE
#endif 
#define CLASS_TYPE KncMsgProc

KncMsgProc::KncMsgProc( RakPeer* pRakPeer, KncPeerManager* pPeerManager, KncMsgHandler_CS* pMsgHandler_ )
{
    m_pPeerManager  = pPeerManager;
    m_pRakPeer      = pRakPeer;
    m_pMsgHandler   = pMsgHandler_;   
    m_eConnectState = E_CONNECT_WAIT;
    m_bRegisterToRelayServer = false;
    m_vecConnectQ.clear();
    m_setConnectUID.clear();
	m_dwConnectionUID = 0;

	m_bRelayOnly = false;
}

KncMsgProc::~KncMsgProc(void)
{
    m_pPeerManager  = NULL;
    m_pRakPeer      = NULL;
    m_pMsgHandler   = NULL;
    m_eConnectState = E_CONNECT_WAIT;
    m_bRegisterToRelayServer = false;
    m_vecConnectQ.clear();
    m_setConnectUID.clear();
}
void KncMsgProc::SetState( ECONNECT_STATE eState_ )
{
    //START_LOG( clog2, ENUM_STR_GETNAME( eState_ ) );
    m_eConnectState = eState_;
}
bool KncMsgProc::RakMsgProc( Packet* pPacket )
{
    switch(pPacket->data[0])
    {
        CASE_RAK_PACKET( ID_CONNECTION_REQUEST_ACCEPTED );
        CASE_RAK_PACKET( ID_NEW_INCOMING_CONNECTION );
        CASE_RAK_PACKET( ID_ADVERTISE_SYSTEM );
        CASE_RAK_PACKET( ID_DISCONNECTION_NOTIFICATION );
        _CASE_RAK_PACKET( ID_CONNECTION_LOST );
        CASE_RAK_PACKET( ID_REMOTE_PORT_REFUSED );
        CASE_RAK_PACKET( ID_PONG );
        CASE_RAK_PACKET( ID_NO_FREE_INCOMING_CONNECTIONS );
        CASE_RAK_PACKET( ID_RECEIVED_STATIC_DATA );
        CASE_RAK_PACKET( ID_REPORT_PUBLIC_IP );
    }
    return false;
}

bool KncMsgProc::KncP2PMsgProc( Packet* pPacket )
{
    //====================================================================================
    // 2006.07.20 : Asirion
    // Comment : 
    #pragma message( "m_eConnectState가 E_CONNECT_FINISH가 되지 않은 상태에서 Packet을 받는 경우 서버로 Report" )

    unsigned char* pData = pPacket->data + sizeof( char );
    unsigned char cPID = 0;
    memcpy( &cPID, pData, sizeof( unsigned char ) );
    switch( (int)cPID )
    {
        _CASE_KNC_PACKET( PID_GAME_MSG );
        _CASE_KNC_PACKET( PID_HEART_BIT_REQ );
        _CASE_KNC_PACKET( PID_HEART_BIT_ACK );
		_CASE_KNC_PACKET( PID_CHECK_P2P_CONDITION_REQ );
		_CASE_KNC_PACKET( PID_CHECK_P2P_CONDITION_ACK );
    }
    return false;
}

void KncMsgProc::Send( RakNet::BitStream& kBitStream_, DWORD dwUID_, SendReliability optReliability_ )
{
    std::vector<DWORD> vecRelayUID;
    std::vector< PlayerID > vecPlayerID;
    switch( dwUID_ ) 
    {
        case KNC_BROADCAST:
        {
            KLocker kLocker( m_pPeerManager->m_mapPeer.csKncMap );
            std::map< DWORD, KncPeer* >::iterator itm;
            KncPeer* pPeer = NULL;
            for ( itm = m_pPeerManager->m_mapPeer.map.begin(); itm != m_pPeerManager->m_mapPeer.map.end(); itm++ )
            {
                pPeer = itm->second;
                if ( pPeer->IsRelay() == false )
                    vecPlayerID.push_back( pPeer->GetRakPeerID() );
                else
                    vecRelayUID.push_back( pPeer->GetUID() );
            }
            break;
        }
        case KNC_BROADCAST_NOT_ME:
        {
            KLocker kLocker( m_pPeerManager->m_mapPeer.csKncMap );
            std::map< DWORD, KncPeer* >::iterator itm;
            KncPeer* pPeer = NULL;
            KncPeer* pMyPeer = m_pPeerManager->GetMyPeer();
            for ( itm = m_pPeerManager->m_mapPeer.map.begin(); itm != m_pPeerManager->m_mapPeer.map.end(); itm++ )
            {
                pPeer = itm->second;
                if ( pMyPeer == pPeer )
                    continue;
                if ( pPeer->IsRelay() == false )
                    vecPlayerID.push_back( pPeer->GetRakPeerID() );
                else
                    vecRelayUID.push_back( pPeer->GetUID() );
            }
            break;
        }
        case KNC_RELAY_BROADCAST_NOT_ME:
        {
            // 릴레이 서버로 패킷을 던집니다
            vecPlayerID.clear();

            KLocker kLocker( m_pPeerManager->m_mapPeer.csKncMap );
            std::map< DWORD, KncPeer* >::iterator itm;
            KncPeer* pPeer = NULL;
            KncPeer* pMyPeer = m_pPeerManager->GetMyPeer();
            for ( itm = m_pPeerManager->m_mapPeer.map.begin(); itm != m_pPeerManager->m_mapPeer.map.end(); itm++ )
            {
                pPeer = itm->second;
                if ( pMyPeer == pPeer )
                    continue;

                vecRelayUID.push_back( pPeer->GetUID() );
            }
            break;
        }
		case KNC_RELAY_TO_ME:
		{
			KncPeer* pMyPeer = m_pPeerManager->GetMyPeer();
			vecRelayUID.push_back( pMyPeer->GetUID() );
			break;
		}
        default:
        {
            KncPeer* pPeer = m_pPeerManager->GetPeer( dwUID_ );
            JIF( pPeer );
            if ( pPeer->IsRelay() == false )
                vecPlayerID.push_back( pPeer->GetRakPeerID() );
            else
                vecRelayUID.push_back( pPeer->GetUID() );
            break;
        }
    }
    for ( int i = 0; i < (int)vecPlayerID.size(); i++ )
	{
		KGCPC_VALUE( "P2P packet", kBitStream_.GetNumberOfBytesUsed() );
        m_pRakPeer->Send( &kBitStream_, HIGH_PRIORITY, static_cast<PacketReliability>(optReliability_), 
                                        KNC_GAME_PACKET_ORDER_CHANNEL, vecPlayerID[i], false );
    }
	if ( vecRelayUID.size() != 0 ) {
		KGCPC_VALUE( "UDP packet", kBitStream_.GetNumberOfBytesUsed() );
        m_pRakPeer->SendToRelayServer( &kBitStream_, HIGH_PRIORITY, static_cast<PacketReliability>(optReliability_), 
            KNC_GAME_PACKET_ORDER_CHANNEL, vecRelayUID );
    }
}

bool KncMsgProc::AddConnectQ( KConnectInfo& kInfo_ )
{

    std::vector<KConnectInfo>::iterator iter = std::find( m_vecConnectQ.begin(), m_vecConnectQ.end(), kInfo_.m_dwUID );
    _JIF( iter == m_vecConnectQ.end(), START_LOG( clog, kInfo_.m_dwUID );SET_ERROR( KNC_MSG_PROC_ADDCONNECTQ );return false );
    m_vecConnectQ.push_back( kInfo_ );
    START_LOG( clog2, kInfo_.m_dwUID );
    return true;
}

bool KncMsgProc::RemoveConnectQ( DWORD dwUID_ )
{
    std::vector<KConnectInfo>::iterator iter = std::find( m_vecConnectQ.begin(), m_vecConnectQ.end(), dwUID_ );
    _JIF( iter != m_vecConnectQ.end(),START_LOG( cerr, dwUID_ );SET_ERROR( KNC_MSG_PROC_REMOVECONNECTQ );return false );
    m_vecConnectQ.erase( iter );
    return true;
}

bool KncMsgProc::RemoveConnectUID( DWORD dwUID_ )
{
	std::set<DWORD>::iterator iter = m_setUID.find( dwUID_ );
	_JIF( iter != m_setUID.end(),START_LOG( cerr, dwUID_ );SET_ERROR( KNC_MSG_PROC_REMOVECONNECTUID );return false );
	m_setUID.erase( iter );
	return true;
}

bool KncMsgProc::RemoveConnectReqUID( DWORD dwUID_ )
{
	std::set<DWORD>::iterator iter = m_setConnectUID.find( dwUID_ );
	_JIF( iter != m_setConnectUID.end(),START_LOG( cerr, dwUID_ );SET_ERROR( KNC_MSG_PROC_REMOVECONNECTREQUID );return false );
	m_setConnectUID.erase( iter );
	return true;
}

bool KncMsgProc::ResetConnectInfo( const DWORD dwUID_, const PlayerID& kRakPeerID_ )
{   
    std::vector<KConnectInfo>::iterator iter = std::find( m_vecConnectQ.begin(), m_vecConnectQ.end(), dwUID_ );
    _JIF( iter != m_vecConnectQ.end(),START_LOG( cerr, dwUID_ );SET_ERROR( KNC_MSG_PROC_RESETCONNECTINFO );return false );
    (*iter).m_vecIP.clear();
    (*iter).m_vecPort.clear();
    (*iter).m_vecIP.push_back( (DWORD)kRakPeerID_.binaryAddress );
    (*iter).m_vecPort.push_back( kRakPeerID_.port );
    return true;
}

//====================================================================================
// 2006.07.06 : Asirion
// m_pPeerManager에서만 지워주고 나머지는 RakNet에서 알아서 처리하도록 수정한다.
// 만약 접속 단계의 유저가 빠져 나간것이라면 ConnectQ에서만 삭제 하여 준다.
bool KncMsgProc::CloseConnection( DWORD dwUID_  )
{   
    if ( std::find( m_vecConnectQ.begin(), m_vecConnectQ.end(), dwUID_ ) != m_vecConnectQ.end() )
    {
        RemoveConnectQ( dwUID_ );
    }
    _JIF( m_pPeerManager->IsIn( dwUID_ ) , SET_ERROR( KNC_MSG_PROC_CLOSECONNECTION );return false );
    PlayerID kRakPeerID = m_pPeerManager->GetPeer( dwUID_ )->GetRakPeerID();
    m_pPeerManager->RemovePeer( dwUID_ );
    m_pRakPeer->CloseConnection( kRakPeerID , false, 0L, dwUID_ );
    return true;
}

void KncMsgProc::SetConnectionUID( DWORD dwConnectionUID_ )
{
	m_dwConnectionUID = dwConnectionUID_;
}

void KncMsgProc::ResetConnectQ()
{
	m_vecConnectQ.clear();
	m_setConnectUID.clear();
	m_setUID.clear();
}

IMPLEMENT_SYSMSG_HANDLER( ID_PONG )
{
    START_LOG( clog2, pPacket->playerId.ToString() );
    return true;
}

IMPLEMENT_SYSMSG_HANDLER( ID_DISCONNECTION_NOTIFICATION )
{
    return true;
}
//====================================================================================
// 2006.07.05 : Asirion
// 현재 Timeout 시간은 10초로 Setting 되어 있다.
IMPLEMENT_SYSMSG_HANDLER( ID_CONNECTION_LOST )
{
    return true;
}
//====================================================================================
// 2006.07.05 : Asirion
//          recvfrom 의 Return 값이 WSAECONNRESET라는 말이다
//     WSAECONNRESET일 경우 Remote Machine에서 Socket을 종료 했다는 말이다.
IMPLEMENT_SYSMSG_HANDLER( ID_REMOTE_PORT_REFUSED )
{
    return true;
}
//====================================================================================
// 2006.07.06 : Asirion
// 접속을 할 자리가 없다.
IMPLEMENT_SYSMSG_HANDLER( ID_NO_FREE_INCOMING_CONNECTIONS )
{
    return true;
}

IMPLEMENT_SYSMSG_HANDLER( ID_NEW_INCOMING_CONNECTION )
{
    return true;
}

IMPLEMENT_SYSMSG_HANDLER( ID_CONNECTION_REQUEST_ACCEPTED )
{
    return true;
}
//====================================================================================
// 2006.07.06 : Asirion
// P2P로 접속을 성공 했다라는 말이된다.
// 이미 RelayServer를 통하여 접속이 된 경우라면 P2P를 사용 하도록 바꾸어 준다.
IMPLEMENT_SYSMSG_HANDLER( ID_RECEIVED_STATIC_DATA )
{
#if defined(USE_ONLY_TCP_RELAY)
    return true;
#endif

    DWORD dwUID = 0;
    memcpy( &dwUID, pPacket->data + sizeof(char), sizeof(DWORD) );
    
	if( m_bRelayOnly && m_bRegisterToRelayServer )
	{

	}
	else
	{
		if ( m_pPeerManager->IsIn( dwUID ) == true )
		{
			KncPeer* pPeer = m_pPeerManager->GetPeer( dwUID );
			_JIF( pPeer->IsRelay() == true, SET_ERROR( KNC_MSG_PROC_RECV_STATICDATA );return false );
			pPeer->SetRelay( false );
			pPeer->SetRakPeerID( pPacket->playerId );
			return true;
		}

		m_pPeerManager->CreatePeer( dwUID, pPacket->playerId );
		RemoveConnectQ( dwUID );
	}    
    return true;
}

IMPLEMENT_SYSMSG_HANDLER( ID_ADVERTISE_SYSTEM )
{    
    //====================================================================================
    // 2006.07.04 : Asirion
    // 접속 개선 작업.
    // UID가 작은 User가 Connect함수를 부르는 것으로 약속.
    // UID가 큰 쪽에서 ID_ADVERTISE_SYSTEM를 받았을 때에는 내가 Echo를 보냄으로써
    // 상대가 Connect할 수 있도록 유도한다.
    START_LOG( clog2, pPacket->playerId.ToString() );
    GET_UID_FROM_PACKET( dwUID, kBitStream );
	if ( m_setUID.count( dwUID ) == 0 )
		return true;
    if ( m_pPeerManager->GetMyPeer()->GetUID() < dwUID )
    {
        if( m_setConnectUID.count( dwUID ) > 0 )
            return true;

		m_setConnectUID.insert( dwUID );
        m_pRakPeer->Connect( (char*)m_pRakPeer->PlayerIDToDottedIP( pPacket->playerId ), 
                             pPacket->playerId.port, (char*)&dwUID, sizeof(DWORD) );
    }
    else
    {
        ResetConnectInfo( dwUID, pPacket->playerId );
        m_pRakPeer->AdvertiseSystem( (char*)m_pRakPeer->PlayerIDToDottedIP( pPacket->playerId ), 
                                     pPacket->playerId.port, 
                                     m_pPeerManager->GetMyPeer()->GetUID() );
    }
    return true;
}

IMPLEMENT_SYSMSG_HANDLER( ID_REPORT_PUBLIC_IP )
{
    m_bRegisterToRelayServer = true;
    return true;
}

IMPLEMENT_SYSMSG_HANDLER( PID_GAME_MSG )
{
    KPID_GAME_MSG* gameMsg = (KPID_GAME_MSG*)( pPacket->data + sizeof(char)*(RAK_MSG_ID_LENGTH + KNC_MSG_ID_LENGTH) );
    //====================================================================================
    // 2006.10.09 : Asirion
    // UDP Packet의 특성상 이전에 보낸 패킷이 한참 뒤에 도착하는 경우가 있다.
    // 이 경우를 막아줄 코드가 필요하다.
    if ( m_pPeerManager->IsIn( gameMsg->dwUID ) == false )
        return true;
    char* pRecvData = (char*)( pPacket->data + sizeof(char)*(RAK_MSG_ID_LENGTH + KNC_MSG_ID_LENGTH) + sizeof(KPID_GAME_MSG) );
    ADD_GAME_MSG_RECEIVE( gameMsg->dwUID, gameMsg->iDataSize, (PBYTE)pRecvData );
    return true;
}

IMPLEMENT_SYSMSG_HANDLER( PID_HEART_BIT_REQ )
{
    START_LOG( clog2, pPacket->playerId.ToString() );
    GET_BITSTREAM_FROM_PACKET( PID_HEART_BIT_REQ, kPacket, kBitStream ); 

    KPID_HEART_BIT_ACK kPacketAck( m_pPeerManager->GetMyPeer()->GetUID(), kPacket.dwCreateTime );
    MAKE_BITSTREAM_FROM_PACKET( PID_HEART_BIT_ACK, &kPacketAck, sizeof(KPID_HEART_BIT_ACK), kBitStreamAck );
    Send( kBitStreamAck, kPacket.dwUID, _RELIABLE );
    return true;
}
IMPLEMENT_SYSMSG_HANDLER( PID_HEART_BIT_ACK )
{
    START_LOG( clog2, pPacket->playerId.ToString() );
    GET_BITSTREAM_FROM_PACKET( PID_HEART_BIT_ACK, kPacket, kBitStream );
    KncPeer* pPeer = m_pPeerManager->GetPeer( kPacket.dwUID );
    if ( pPeer != NULL )
    {
        DWORD dwRecvTime = timeGetTime();
		DWORD dwPing = dwRecvTime - kPacket.dwCreateTime;        
        
        pPeer->SetRecvPingTime( dwRecvTime );
		
        pPeer->SetPing( dwPing );
		START_LOG( clog, "UserPing" )
			<< BUILD_LOG( kPacket.dwUID )
			<< BUILD_LOG( dwPing )
			<< END_LOG;
    }
    return true;
}


IMPLEMENT_SYSMSG_HANDLER( PID_CHECK_P2P_CONDITION_REQ )
{
	return false;
}

IMPLEMENT_SYSMSG_HANDLER( PID_CHECK_P2P_CONDITION_ACK )
{
	return false;
}

