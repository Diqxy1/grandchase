#include "stdafx.h"
#include "../P2P.h"
#include "../knc/ClientErr.h"

#include ".\kncthreadrecv.h"
#include "dbg/dbg.hpp"
#include "../KNC/SOCKET/UdpToServer.h"
#include "../KSingleton.h"
#include "../GCClientErrorManager.h"

#ifdef CLASS_TYPE
#undef CLASS_TYPE
#endif

#define CLASS_TYPE KncThreadRecv


KncThreadRecv::KncThreadRecv( RakPeer* pRakPeer, KncPeerManager* pPeerManager,
                              KncMsgHandler_CS* pMsgHandler_) 
: KncMsgProc( pRakPeer, pPeerManager, pMsgHandler_ )
{
    m_dwPastTime                        = 0;
    m_dwCurTime                         = 0;
    m_dwElapsedTime                     = 0;

    m_usServerPort                      = 0;
    m_dwServerIP                        = 0;
    m_bServerDown                       = false;

    m_bRecvEchoPort                     = false;
    m_bInit                             = false;
    m_mapTimerFunc. clear();
    m_vecRemoveTimerID.clear();

	m_bRegisterToRelayServer = false;
	m_dwConnectionUID = 0;

	m_bRelayOnly = false;
}

KncThreadRecv::~KncThreadRecv(void)
{
    m_mapTimerFunc. clear();
    m_vecRemoveTimerID.clear();
}

void KncThreadRecv::FrameMove()
{
    if( m_pRakPeer == NULL || m_pPeerManager == NULL )
        return;
    CalcElapsedTime();
    OnTimerFrameMove();
	OnPartTimerFrameMove();

    UINT iPacketCount = m_pRakPeer->GetIncomingPacketQueueSize();
    for ( int i = 0; i < (int)iPacketCount; i++ )
    {
        Packet* pPacket = m_pRakPeer->Receive();
        if( pPacket == NULL ) 
            break;
        if ( pPacket->data[0] < PID_START )
        {
            _JIF( RakMsgProc( pPacket ), SET_ERROR( KNC_THREADRECV_INVALID_RAKNET_PACKET );return; );
        }
        else
        {
            _JIF( KncP2PMsgProc( pPacket ), SET_ERROR( KNC_THREADRECV_INVALID_P2P_PACKET );return; );
        }
        m_pRakPeer->DeallocatePacket(pPacket);
    }
}

void KncThreadRecv::CalcElapsedTime()
{
    m_dwCurTime = timeGetTime();
    m_dwElapsedTime = m_dwCurTime - m_dwPastTime;
    m_dwPastTime = m_dwCurTime;
}

void KncThreadRecv::OnTimerFrameMove()
{

    KncTimerMsgMap::iterator iter;
    for ( iter = m_mapTimerFunc.begin(); iter != m_mapTimerFunc.end(); iter++ )
    {
        (*iter).second.second.first += m_dwElapsedTime;
        if ( (*iter).second.first <= (*iter).second.second.first )
        {
            (*iter).second.second.first = 0;
            (this->*((*iter).second.second.second))(0, 0);
        }
    }
    for ( int i = 0; i < (int)m_vecRemoveTimerID.size(); i++ )
    {
        if( m_mapTimerFunc.find( m_vecRemoveTimerID[i]) != m_mapTimerFunc.end() )
            m_mapTimerFunc.erase( m_vecRemoveTimerID[i] );
    }
    m_vecRemoveTimerID.clear();
}

void KncThreadRecv::Init( DWORD dwServerIP_, u_short usServerPort_ )
{
    m_bRecvEchoPort     = false;
    m_bInit             = true;
	m_bServerDown       = false;
    m_usServerPort      = usServerPort_;
    m_dwServerIP        = dwServerIP_;
    m_dwPastTime        = timeGetTime();
    m_dwCurTime         = 0;
    #ifndef  _ENUM
    #define _ENUM( ID ) KSingleton<EnumString>::GetInstance()->SetEnumName( ID, L#ID );
    #include "KncP2PStateDef.h"
    #endif    
    SendServerEcho();

    KNC_TIMER_MSGMAP( T_SERVER_ECHO,    TIME_SERVER_ECHO,   &KncThreadRecv::ON_T_SERVER_ECHO );
    KNC_TIMER_MSGMAP( T_KEEP_UP_SERVER_ECHO_PORT, TIME_KEEP_UP_SERVER_ECHO_PORT,&KncThreadRecv::ON_T_KEEP_UP_SERVER_ECHO_PORT );

}

void KncThreadRecv::RemoveTimer( DWORD dwID_ )
{
    m_vecRemoveTimerID.push_back( dwID_ );
}
void KncThreadRecv::SetupRelayServer()
{
    KNC_TIMER_MSGMAP( T_REGISTER_TO_RELAYSERVER, TIME_REGISTER_TO_RELAYSERVER,
                      &KncThreadRecv::ON_T_REGISTER_TO_RELAYSERVER );
    KNC_TIMER_MSGMAP( T_KEEP_UP_RELAYSERVER_CONNECTION, TIME_KEEP_UP_RELAYSERVER_CONNECTION,
                      &KncThreadRecv::ON_T_KEEP_UP_RELAYSERVER_CONNECTION );

}
void KncThreadRecv::Connect()
{
    for ( int i = 0; i < (int)m_vecConnectQ.size(); i++ )
    {
        m_setUID.insert( m_vecConnectQ[i].m_dwUID );
    }

	if( m_bRelayOnly && m_bRegisterToRelayServer )
	{
		ON_T_FINISH_CONNECTION( 0, 0 );
		return;
	}


    KNC_TIMER_MSGMAP( T_CONNECT_P2P,    TIME_CONNECT_P2P,   &KncThreadRecv::ON_T_CONNECT_P2P );
    KNC_TIMER_MSGMAP( T_FINISH_CONNECTION,    TIME_FINISH_CONNECTION,   &KncThreadRecv::ON_T_FINISH_CONNECTION );
    SetState( E_CONNECT_P2P );
}

void KncThreadRecv::Connect( DWORD dwUID_ )
{
	m_setUID.insert( dwUID_ );
	if( m_bRelayOnly && m_bRegisterToRelayServer )
	{
		ON_T_FINISH_PART_CONNECTION( dwUID_, 0 );

	}
	KNC_PART_TIMER_MSGMAP( dwUID_, T_CONNECT_PART_P2P,    TIME_CONNECT_P2P,   &KncThreadRecv::ON_T_CONNECT_PART_P2P );
	KNC_PART_TIMER_MSGMAP( dwUID_, T_FINISH_PART_CONNECTION,    TIME_FINISH_CONNECTION,   &KncThreadRecv::ON_T_FINISH_PART_CONNECTION );
}

IMPL_TIMER_HANDLER( T_KEEP_UP_SERVER_ECHO_PORT )
{
    SendServerEcho();
}

IMPL_TIMER_HANDLER( T_HEARTBIT )
{
	if( m_pPeerManager->GetMyPeer() == NULL )
		return;

    {
        KLocker kLocker( m_pPeerManager->m_mapPeer.csKncMap );
        std::map< DWORD, KncPeer* >::iterator itm;
        KncPeer* pPeer = NULL;
        for ( itm = m_pPeerManager->m_mapPeer.map.begin(); itm != m_pPeerManager->m_mapPeer.map.end(); itm++ )
        {
            pPeer = itm->second;
            
            if( pPeer->GetLastRecvPingTime() == 0 )
            {
                pPeer->PingPacketLost();
            }

            pPeer->SetRecvPingTime( 0 );
        }
    }

    KPID_HEART_BIT_REQ kPacket( m_pPeerManager->GetMyPeer()->GetUID(), timeGetTime() );
	m_pPeerManager->GetMyPeer()->SetSendPingTime( kPacket.dwCreateTime );
    MAKE_BITSTREAM_FROM_PACKET( PID_HEART_BIT_REQ, &kPacket, sizeof(KPID_HEART_BIT_REQ), kBitStream );
    Send( kBitStream, KNC_BROADCAST_NOT_ME, _RELIABLE );
}

IMPL_TIMER_HANDLER( T_HEARTBIT_CHECK )
{
    {
        KLocker kLocker( m_pPeerManager->m_mapPeer.csKncMap );
        std::map< DWORD, KncPeer* >::iterator itm;
        KncPeer* pPeer = NULL;
        for ( itm = m_pPeerManager->m_mapPeer.map.begin(); itm != m_pPeerManager->m_mapPeer.map.end(); itm++ )
        {
            pPeer = itm->second;

            if ( !pPeer || pPeer->IsLocal() )
                continue;


            bool bRelay = pPeer->IsRelay();

            // 100초에 한번씩 패킷 로스 카운트 와 평균 Ping 체크 한다.
            if ( pPeer->GetPingLossCount() > KncPeerManager::LIMIT_PING_LOSS_COUNT )
            {
                if ( bRelay )
                    SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_UR_PING_LOSS_COUNT );        
                else
                    SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_UDP_PING_LOSS_COUNT );
            }

            if ( pPeer->GetPartPing() > KncPeerManager::LIMIT_PING_TIME )
            {
                if ( bRelay )
                    SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_UR_PING_TIME_OVER );
                else
                    SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_NETWORK, KNetWorkErr::CE_UDP_PING_TIME_OVER );
            }

            pPeer->ClearPartPingTime();
            pPeer->ResetLossPingCount();
        }
    }    
}

IMPL_TIMER_HANDLER( T_SERVER_ECHO )
{
    if ( m_bRecvEchoPort == false )
    {
        SendServerEcho();
    }
}

IMPL_TIMER_HANDLER( T_CONNECT_P2P )
{    
    for( int i = 0; i < (int)m_vecConnectQ.size(); i++ )
    {
        SendConnectReq( m_vecConnectQ[i] );
    }
}

IMPL_TIMER_HANDLER( T_CONNECT_PART_P2P )
{
	std::vector<KConnectInfo>::iterator vit = std::find( m_vecConnectQ.begin(), m_vecConnectQ.end(), wParam_ );

	if ( vit == m_vecConnectQ.end() )
	{
		if( !m_pPeerManager->IsIn( wParam_ ) ) 
			SiGCClientErrorManager()->ErrorCollect( KEventErr::ERR_CLIENTRPT, KClientErr::CE_NOT_IN_VECCONNECTQ );
		return;
	}

	SendConnectReq( *vit );	
}

IMPL_TIMER_HANDLER( T_FINISH_CONNECTION )
{    
    KNC_TIMER_MSGMAP( T_HEARTBIT,       TIME_HEARTBIT,     &KncThreadRecv::ON_T_HEARTBIT );
    KNC_TIMER_MSGMAP( T_HEARTBIT_CHECK, TIME_HEARTBIT_CHECK,    &KncThreadRecv::ON_T_HEARTBIT_CHECK );
    
    
    RemoveTimer( T_FINISH_CONNECTION );
    RemoveTimer( T_CONNECT_P2P );
    SetState( E_CONNECT_FINISH );
    std::vector< DWORD > vecUID;
    for ( int i = 0; i < (int)m_vecConnectQ.size(); i++ )
    {
        if ( m_pPeerManager->IsIn( m_vecConnectQ[i].m_dwUID ) == false )
        {
            vecUID.push_back( m_vecConnectQ[i].m_dwUID );
            KncPeer* pPeer = m_pPeerManager->CreatePeer( m_vecConnectQ[i].m_dwUID, m_pRakPeer->GetRelayServerID(), false, true );
            JIF( pPeer );
        }
    }
    ADD_GAME_MSG_CONNECT_COMPLETE( vecUID );
    m_vecConnectQ.clear();
}

IMPL_TIMER_HANDLER( T_FINISH_PART_CONNECTION )
{
	// Timer 들 삭제
	RemovePartTimer( wParam_, T_FINISH_PART_CONNECTION );
	RemovePartTimer( wParam_, T_CONNECT_PART_P2P );

	// PeerManager에 없는 아이면 ㅡ 릴레이로 돌려줘야 할지 체크 한다.
	if ( m_pPeerManager->IsIn( wParam_ ) == false )
	{		
		KncPeer* pPeer = m_pPeerManager->CreatePeer( wParam_, m_pRakPeer->GetRelayServerID(), false, true );
		JIF( pPeer );       
	}

	KNC_TIMER_MSGMAP( T_HEARTBIT, TIME_HEARTBIT, &KncThreadRecv::ON_T_HEARTBIT );
    KNC_TIMER_MSGMAP( T_HEARTBIT_CHECK, TIME_HEARTBIT_CHECK,    &KncThreadRecv::ON_T_HEARTBIT_CHECK );
	
    // vecUID를 GameLayer로 전달 해 주어서 해당 유저와의P2P 연결이 종료 되었음을 알린다.
	std::vector< DWORD > vecUID;	
	vecUID.push_back( wParam_ );
	//ADD_GAME_MSG_CONNECT_COMPLETE( vecUID );

	RemoveConnectQ( wParam_ );

	KNC_PART_TIMER_MSGMAP( wParam_, T_CONDITION_CHECK, TIME_CONDITION_CHECK, &KncThreadRecv::ON_T_CONDITION_CHECK );
	KNC_PART_TIMER_MSGMAP( wParam_, T_FINISH_CONDITION_CHECK, TIME_FINISH_CONDITION_CHECK, &KncThreadRecv::ON_T_FINISH_CONDITION_CHECK );
}

//====================================================================================
// 2006.07.26 : Asirion
// SET_ECHO_PORT 와 마찬가지로 이 부분도 구조상으로 깔끔하지 못한 부분이 있다.
// 시간의 압박으로 현재는 이렇게 처리 하고 후에 수정하도록 하겠다.
IMPL_TIMER_HANDLER( T_REGISTER_TO_RELAYSERVER )
{
    if ( m_bRegisterToRelayServer == false )
        m_pRakPeer->RegisterToRelayServer();
}

IMPL_TIMER_HANDLER( T_KEEP_UP_RELAYSERVER_CONNECTION )
{
    m_pRakPeer->RegisterToRelayServer();
}

void KncThreadRecv::SendServerEcho()
{
    JIF( m_bServerDown == false );
    KUdpToServer kUdpToServer;
    kUdpToServer.m_dwUserUID= m_pPeerManager->GetMyPeer()->GetUID();
    kUdpToServer.m_usPort   = m_pPeerManager->GetMyPeer()->GetPort();
    m_pRakPeer->SendEchoToServer( m_dwServerIP, m_usServerPort, (char*)&kUdpToServer, sizeof( KUdpToServer ) );
}

void KncThreadRecv::SetEchoPort( DWORD dwIP_, u_short usPort_ )
{
    JIF( m_bInit );
    _JIF( m_pPeerManager, SET_ERROR( KNC_THREADRECV_INVALID_PTR );return; );
    _JIF( m_pPeerManager->GetMyPeer(), SET_ERROR( KNC_THREADRECV_INVALID_PTR );return; );
    

    std::vector<DWORD>::iterator vitIP;
    vitIP = std::find( m_pPeerManager->m_kConnectInfo.m_vecIP.begin(), m_pPeerManager->m_kConnectInfo.m_vecIP.end(), dwIP_ );
    if( vitIP == m_pPeerManager->m_kConnectInfo.m_vecIP.end() )
    {
        m_pPeerManager->m_kConnectInfo.m_vecIP.push_back( dwIP_ );        
    }
    
    std::vector<u_short>::iterator vitPort;
    vitPort = std::find( m_pPeerManager->m_kConnectInfo.m_vecPort.begin(), m_pPeerManager->m_kConnectInfo.m_vecPort.end(), usPort_ );
    if( vitPort == m_pPeerManager->m_kConnectInfo.m_vecPort.end() )
    {
        m_pPeerManager->m_kConnectInfo.m_vecPort.push_back( usPort_ );
    }

    PlayerID kPlayerID = m_pRakPeer->IPToPlayerID( dwIP_, usPort_ );
    START_LOG( clog2, kPlayerID.ToString() );
    m_dwPastTime = timeGetTime();
    m_bRecvEchoPort  = true;
}

//====================================================================================
// 2006.07.06 : Asirion
// m_pPeerManager에서만 지워주고 나머지는 RakNet에서 알아서 처리하도록 수정한다.
// 만약 접속 단계의 유저가 빠져 나간것이라면 ConnectQ에서만 삭제 하여 준다.
bool KncThreadRecv::CloseConnection( DWORD dwUID_  )
{   
	RemoveConnectQ( dwUID_ );
	RemoveConnectUID( dwUID_ );
	RemoveConnectReqUID( dwUID_ );

	if( !m_pPeerManager->IsIn( dwUID_ ) )
	{
		return false;
	}

	PlayerID kRakPeerID = m_pPeerManager->GetPeer( dwUID_ )->GetRakPeerID();
	m_pPeerManager->RemovePeer( dwUID_ );
	m_pRakPeer->CloseConnection( kRakPeerID , false, 0L, dwUID_ );

	RemovePartTimer( dwUID_, T_FINISH_PART_CONNECTION );
	RemovePartTimer( dwUID_, T_CONNECT_PART_P2P );

	return true;
}


IMPLEMENT_SYSMSG_HANDLER( PID_CHECK_P2P_CONDITION_REQ )
{
	//START_LOG( clog2, pPacket->playerId.ToString() );
	GET_BITSTREAM_FROM_PACKET( PID_CHECK_P2P_CONDITION_REQ, kPacket, kBitStream ); 

	KPID_CHECK_P2P_CONDITION_ACK kPacketAck( m_pPeerManager->GetMyPeer()->GetUID(), kPacket.dwCreateTime );
	MAKE_BITSTREAM_FROM_PACKET( PID_CHECK_P2P_CONDITION_ACK, &kPacketAck, sizeof(KPID_CHECK_P2P_CONDITION_ACK), kBitStreamAck );
	Send( kBitStreamAck, kPacket.dwUID, _RELIABLE );
	return true;
}

IMPLEMENT_SYSMSG_HANDLER( PID_CHECK_P2P_CONDITION_ACK )
{
	//START_LOG( clog2, pPacket->playerId.ToString() );
	GET_BITSTREAM_FROM_PACKET( PID_HEART_BIT_ACK, kPacket, kBitStream );
	KncPeer* pPeer = m_pPeerManager->GetPeer( kPacket.dwUID );
	if ( pPeer != NULL )
	{
		DWORD dwRecvTime = timeGetTime();
		pPeer->SetPing( dwRecvTime - kPacket.dwCreateTime );
	}
	return true;
}

void KncThreadRecv::OnPartTimerFrameMove()
{
	KncPartTimerMsgMap::iterator mit;
	for( mit = m_mapPartTimerFunc.begin(); mit != m_mapPartTimerFunc.end(); ++mit )
	{
		KncTimerMsgMap& tMsgMap = mit->second;

		KncTimerMsgMap::iterator iter2;
		for ( iter2 = tMsgMap.begin(); iter2 != tMsgMap.end(); iter2++ )
		{
			(*iter2).second.second.first += m_dwElapsedTime;
			if ( (*iter2).second.first <= (*iter2).second.second.first )
			{
				(*iter2).second.second.first = 0;
				(this->*((*iter2).second.second.second))(mit->first, 0);
			}
		}
	}

	std::map< DWORD, std::vector< UINT > >::iterator mit2;
	for( mit2 = m_mapRemovePartTimerID.begin(); mit2 != m_mapRemovePartTimerID.end(); ++mit2 )
	{
		if( m_mapPartTimerFunc.find( mit2->first ) != m_mapPartTimerFunc.end() )
		{
			KncTimerMsgMap& tMsgMap = m_mapPartTimerFunc[ mit2->first ];

			std::vector< UINT >& vecTimerID = mit2->second;
			for( int i = 0; i < (int)vecTimerID.size(); ++i )
			{
				if( tMsgMap.find( vecTimerID[i] ) != tMsgMap.end() )
					tMsgMap.erase( vecTimerID[i] );
			}
		}
	}

	m_mapRemovePartTimerID.clear();
}

void KncThreadRecv::RemovePartTimer( DWORD dwUID_, UINT ID_ )
{
	m_mapRemovePartTimerID[dwUID_].push_back( ID_ );
}

IMPL_TIMER_HANDLER( T_CONDITION_CHECK )
{
	if( m_pPeerManager->GetMyPeer() == NULL )
		return;

	// ping loss check
	KLocker kLocker( m_pPeerManager->m_mapPeer.csKncMap );
	KncPeer* pPeer = m_pPeerManager->GetPeer( wParam_ );
	if( pPeer == NULL )
	{
		// 나갔을 경우 제거
		RemovePartTimer( wParam_, T_CONDITION_CHECK );
		RemovePartTimer( wParam_, T_FINISH_CONDITION_CHECK );
		return;
	}

    KPID_HEART_BIT_REQ kPacket( m_pPeerManager->GetMyPeer()->GetUID(), timeGetTime() );

	MAKE_BITSTREAM_FROM_PACKET( PID_CHECK_P2P_CONDITION_REQ, &kPacket, sizeof(KPID_CHECK_P2P_CONDITION_REQ), kBitStream );
	Send( kBitStream, wParam_, _RELIABLE );
}

IMPL_TIMER_HANDLER( T_FINISH_CONDITION_CHECK )
{
	KLocker kLocker( m_pPeerManager->m_mapPeer.csKncMap );
	KncPeer* pPeer = m_pPeerManager->GetPeer( wParam_ );
	if( pPeer == NULL )
	{
		// 나갔을 경우 제거
		RemovePartTimer( wParam_, T_CONDITION_CHECK );
		RemovePartTimer( wParam_, T_FINISH_CONDITION_CHECK );
		return;
	}
}

void KncThreadRecv::SendConnectReq( KConnectInfo kConnectInfo_ )
{
	in_addr kAddInfo;
	for ( int j = 0; j < (int)kConnectInfo_.m_vecIP.size(); j++ )
	{
		kAddInfo.s_addr = kConnectInfo_.m_vecIP[j];
		for ( int k = 0; k < (int)kConnectInfo_.m_vecPort.size(); k++ )
		{
			m_pRakPeer->AdvertiseSystem( (char*)::inet_ntoa( kAddInfo ), kConnectInfo_.m_vecPort[k], 
				m_pPeerManager->GetMyPeer()->GetUID() );
		}
	}
}

