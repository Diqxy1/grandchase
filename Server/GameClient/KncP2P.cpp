#include "stdafx.h"
#include "../P2P.h"
#include "../knc/ClientErr.h"
#include "./RakNetSrc/BitStream.h"
#include "KncPeerManager.h"
#include "KncPeer.h"
#include "./RakNetSrc/rakpeer.h"
#include "KncThreadRecv.h"
#include <Thread/Thread.h>
#include <Thread/Locker.h>
#include <Serializer/Serializer.h>
#include <Serializer/SerBuffer.h>
#include <Iphlpapi.h>
#include <iostream>
#include <sstream>
#include "dbg/dbg.hpp"

using namespace dbg;

#include "KncP2P.h"

KncP2P::KncP2P()
{
    m_pMsgHandler       = NULL;
    m_pPeerManager      = NULL;
    m_pThreadRecv       = NULL;
    m_pRakPeer          = NULL;

    m_uiMaxPeer         = DEFAULT_MAX_PEER;
    m_usLocalPort       = DEFAULT_PORT;
    m_fpStream          = NULL;
    m_bInit             = false;
#ifdef _DEBUG
    CreateConsoleWnd();
#endif
}
KncP2P::~KncP2P()
{
    KncP2PLog();
    Close();
    if ( m_fpStream )
        fclose( m_fpStream );
}

void KncP2P::Close()
{
    _JIF( m_pRakPeer, SET_ERROR( KNC_KNCP2P_CLOSE );return; );
    m_pRakPeer->Disconnect( 1000L );
    m_bInit = false;

    SAFE_DELETE( m_pThreadRecv );
    SAFE_DELETE( m_pPeerManager );
    SAFE_DELETE( m_pRakPeer );
    SAFE_DELETE( m_pMsgHandler );
}

void KncP2P::ResetConnectQ()
{
    _JIF( m_pThreadRecv, return; );    
    m_pThreadRecv->ResetConnectQ();
}


void KncP2P::KncP2PLog()
{
    //if ( m_pPeerManager == NULL || m_pRakPeer == NULL )
    //    return;
    //int iCount = m_pPeerManager->GetUserCount();
    //KncPeer* pPeer = NULL;
    //for ( int i = 0; i < iCount; i++ )
    //{       
    //    pPeer = m_pPeerManager->GetPeerFromIndex( i );
    //    if ( pPeer == NULL )
    //        continue;
    //    RakNetStatisticsStruct* statics = m_pRakPeer->GetStatistics( pPeer->m_RakPeerID );
    //    START_LOG( cout, "KncP2PLog Start" ) << 
    //        BUILD_LOG( pPeer->m_strPeerName ) <<
    //        BUILD_LOG( statics->packetsSent ) << 
    //        BUILD_LOG( statics->packetsReceived ) <<
    //        BUILD_LOG( statics->messageResends ) <<
    //        BUILD_LOG( statics->orderedMessagesOutOfOrder ) <<
    //        BUILD_LOG( statics->duplicateMessagesReceived ) <<
    //        BUILD_LOG( statics->invalidMessagesReceived ) <<
    //        BUILD_LOG( timeGetTime() - statics->connectionStartTime );
    //}
}

bool KncP2P::InitKncP2P( const DWORD dwUID_ , const KNCMSGHANDLER pFuncMsgHandler_, unsigned int uiMaxPeer_,  
                         DWORD dwServerIP_, u_short usServerPort_, unsigned short usLocalPort_)
{
    m_pMsgHandler     = new KncMsgHandler_CS( pFuncMsgHandler_ );
    m_pRakPeer        = new RakPeer();
    m_pPeerManager    = new KncPeerManager();
    m_pThreadRecv     = new KncThreadRecv(m_pRakPeer ,m_pPeerManager, m_pMsgHandler );
    
    m_uiMaxPeer       = uiMaxPeer_;
    m_usLocalPort     = usLocalPort_;

    _JIF( GetValidPort(), m_bInit = false; SET_ERROR( KNC_KNCP2P_GETVALIDPORT );return false );
    _JIF( m_pRakPeer->Initialize( uiMaxPeer_, m_usLocalPort, 10 ), 
           m_bInit = false; SET_ERROR( KNC_KNCP2P_INIT_RAKNET );return false );

    m_pRakPeer->SetMaximumIncomingConnections( uiMaxPeer_ );
    m_pRakPeer->SetMTUSize( KNC_DEFAULT_MTU ); // DSL에서 최고 크기.
    m_pRakPeer->SetIncomingPassword( (char*)&dwUID_, sizeof(DWORD) );    
    m_pRakPeer->AssignPlayerIDToRemoteSystemList( m_pRakPeer->GetInternalID() , NULL, false );
    m_pRakPeer->SetRemoteStaticData( m_pRakPeer->GetInternalID(), (char*)&dwUID_, sizeof(DWORD) );
    m_pPeerManager->CreatePeer( dwUID_, m_pRakPeer->GetInternalID(), true );

    //====================================================================================
    // 2006.07.06 : Asirion
    // 내 정보를 Setting한다, 현재 IP는 최고 3개 까지 보았다.
    KConnectInfo kConnectInfo;
    kConnectInfo.m_dwUID = dwUID_;
    kConnectInfo.m_vecIP = m_pRakPeer->GetLocalIP();
    kConnectInfo.m_vecPort.push_back( m_pRakPeer->GetInternalID().port );
    m_pPeerManager->SetConnectInfo( kConnectInfo );
    //====================================================================================
    // 2006.07.04 : Asirion
    // GameServer에 있는 UDP Echo Module정보 설정
    m_pThreadRecv->Init( dwServerIP_, usServerPort_ );
    m_bInit = true;
    return true;
}

bool KncP2P::GetValidPort()
{
#ifndef KNC_UDP_TABLE_SIZE
#define KNC_UDP_TABLE_SIZE  2048
#endif
#define KNC_FIND_PORT_TIMEOUT   5000
    struct KNC_UDP_IPTABLE
    {
        DWORD dwNumEntries;
        MIB_UDPROW table[KNC_UDP_TABLE_SIZE];
    };
    KNC_UDP_IPTABLE udpTable;
    DWORD dwSize = KNC_UDP_TABLE_SIZE;
    DWORD ret = GetUdpTable( (PMIB_UDPTABLE)&udpTable, &dwSize, FALSE );
    if ( ret  != NO_ERROR )
        return false;
    DWORD dwPastTime = timeGetTime();
    DWORD dwCurTime = timeGetTime();
    DWORD dwAccumulateTime = 0;
    for ( int i = 0; i < (int)udpTable.dwNumEntries; i++ )
    {
        dwCurTime = timeGetTime();
        dwAccumulateTime = dwAccumulateTime + ( dwCurTime - dwPastTime );
        dwPastTime = dwCurTime;
        if ( KNC_FIND_PORT_TIMEOUT <= dwAccumulateTime)
            return false;
        if( m_usLocalPort == ::ntohs((u_short)udpTable.table[i].dwLocalPort) )
        {
            m_usLocalPort++;
            i = -1;
        }
    }
    return true;
}

bool KncP2P::ConnectP2P()
{
    _JIF( m_bInit, SET_ERROR( KNC_KNCP2P_INVALID_PTR );return false );
    _JIF( m_pRakPeer, SET_ERROR( KNC_KNCP2P_INVALID_PTR );return false );
    _JIF( m_pPeerManager, SET_ERROR( KNC_KNCP2P_INVALID_PTR );return false );
    _JIF( m_pPeerManager->GetMyPeer(), SET_ERROR( KNC_KNCP2P_INVALID_PTR );return false );

    m_pThreadRecv->Connect();
    return true;
}


void KncP2P::Send( void* pPakcet_, int iPacketSize_, DWORD dwUID_, SendReliability optReliability_ )
{
    _JIF( m_pPeerManager, SET_ERROR( KNC_KNCP2P_INVALID_PTR ); return; );
    _JIF( m_pPeerManager->GetMyPeer(), SET_ERROR( KNC_KNCP2P_INVALID_PTR ); return );

    static KPID_GAME_MSG s_kGameMsg;
	s_kGameMsg.dwUID        = m_pPeerManager->GetMyPeer()->GetUID(); 
	s_kGameMsg.iDataSize    = iPacketSize_;

    MAKE_BITSTREAM_FROM_PACKET( PID_GAME_MSG, &s_kGameMsg, sizeof( KPID_GAME_MSG ), kBitStream );
    kBitStream.Write( (char*)pPakcet_, iPacketSize_ );
    m_pThreadRecv->Send( kBitStream, dwUID_, optReliability_ );
}
void KncP2P::DisConnectPeer( DWORD dwUID_ )
{
    _JIF( m_pThreadRecv, SET_ERROR( KNC_KNCP2P_INVALID_PTR );return; );
    m_pThreadRecv->CloseConnection( dwUID_ );
}

std::vector<DWORD> KncP2P::GetLocalIP()
{
    _JIF( m_pRakPeer, SET_ERROR( KNC_KNCP2P_INVALID_PTR ); return std::vector<DWORD>() );
    KConnectInfo kConnectInfo;
    m_pPeerManager->GetConnectInfo( &kConnectInfo );
    return kConnectInfo.m_vecIP;
}

std::vector<u_short> KncP2P::GetLocalPort()
{
    _JIF( m_pPeerManager, SET_ERROR( KNC_KNCP2P_INVALID_PTR ); return std::vector< u_short >() );
    KConnectInfo kConnectInfo;
    m_pPeerManager->GetConnectInfo( &kConnectInfo );
    return kConnectInfo.m_vecPort;
}

int KncP2P::GetUserCount( KncUserCount::EUserCount eUserCount_ )
{
    _JIF( m_pPeerManager, SET_ERROR( KNC_KNCP2P_INVALID_PTR); return 0 );
    return m_pPeerManager->GetUserCount( eUserCount_ );
}
DWORD KncP2P::GetMyUID()
{
    _JIF( m_pPeerManager, SET_ERROR( KNC_KNCP2P_INVALID_PTR ); return 0 );
    _JIF( m_pPeerManager->GetMyPeer(), SET_ERROR( KNC_KNCP2P_INVALID_PTR ); return 0 );
    return m_pPeerManager->GetMyPeer()->GetUID();
}

void KncP2P::FrameMove()
{
    _JIF( m_bInit, SET_ERROR( KNC_KNCP2P_INVALID_PTR );return; );
    _JIF( m_pRakPeer, SET_ERROR( KNC_KNCP2P_INVALID_PTR );return; );
    _JIF( m_pPeerManager, SET_ERROR( KNC_KNCP2P_INVALID_PTR );return; );
    _JIF( m_pPeerManager->GetMyPeer(), SET_ERROR( KNC_KNCP2P_INVALID_PTR );return; );

    m_pThreadRecv->FrameMove();
}

UINT KncP2P::GetPacketQSize()
{ 
    _JIF( m_pRakPeer, SET_ERROR( KNC_KNCP2P_INVALID_PTR ); return 0 );
    return m_pRakPeer->GetIncomingPacketQueueSize();
}
void KncP2P::SetEchoPort( DWORD dwIP_, u_short usPort_ )
{
    _JIF( m_pThreadRecv, SET_ERROR( KNC_KNCP2P_INVALID_PTR );return; );
    m_pThreadRecv->SetEchoPort( dwIP_, usPort_ );
}

void KncP2P::SetupRelayServer( DWORD dwIP_, USHORT usPort_ )
{
    _JIF( m_bInit, SET_ERROR( KNC_KNCP2P_INVALID_PTR );return; );
    _JIF( m_pRakPeer, SET_ERROR( KNC_KNCP2P_INVALID_PTR );return; );
    _JIF( m_pPeerManager, SET_ERROR( KNC_KNCP2P_INVALID_PTR );return; );
    _JIF( m_pPeerManager->GetMyPeer(), SET_ERROR( KNC_KNCP2P_INVALID_PTR );return; );

    //====================================================================================
    // 2006.07.04 : Asirion
    // RelayServer정보 Setting
    m_pRakPeer->SetupRelayServer( m_pRakPeer->IPToPlayerID( dwIP_, usPort_ ), 
                                  m_pPeerManager->GetMyPeer()->GetUID() );
    m_pThreadRecv->SetupRelayServer();
    START_LOG( clog2, m_pRakPeer->GetRelayServerID().ToString() );
}

bool KncP2P::IsIn( DWORD dwUID_ )
{
    _JIF( m_bInit, SET_ERROR( KNC_KNCP2P_INVALID_PTR ); return false );
    _JIF( m_pPeerManager, SET_ERROR( KNC_KNCP2P_INVALID_PTR ); return false );
    return m_pPeerManager->IsIn( dwUID_ );
}

bool KncP2P::AddConnectQ( KConnectInfo& kInfo_ )
{
    //====================================================================================
    // 2006.07.19 : Asirion
    // Comment : Start하고 난 후에 이 작업을 하게 되면 오류다.
    //#pragma TODO( "서버로 Error Reporting하는 모듈 필요." );
	#pragma message("서버로 Error Reporting하는 모듈 필요.")
	_JIF( m_pThreadRecv, SET_ERROR( KNC_KNCP2P_INVALID_PTR ); return false );
    return m_pThreadRecv->AddConnectQ( kInfo_ );
}

std::vector<DWORD> KncP2P::GetConnectQUID()
{
    //====================================================================================
    // 2006.07.19 : Asirion
    // Comment : Start하고 난 후에 이 작업을 하게 되면 오류다.
	//#pragma TODO( "서버로 Error Reporting하는 모듈 필요." );
	#pragma message("서버로 Error Reporting하는 모듈 필요.")
    _JIF( m_pThreadRecv, SET_ERROR( KNC_KNCP2P_INVALID_PTR ); return std::vector<DWORD> () );
    return m_pThreadRecv->GetConnectQUID();
}

std::vector<DWORD> KncP2P::GetPeerUID()
{
	_JIF( m_bInit, SET_ERROR( KNC_KNCP2P_INVALID_PTR ); return std::vector<DWORD> () );
    _JIF( m_pPeerManager, SET_ERROR( KNC_KNCP2P_INVALID_PTR ); return std::vector<DWORD> () );
    return m_pPeerManager->GetPeerUID();
}

DWORD KncP2P::GetAvgPingTime()
{
    _JIF( m_pPeerManager, SET_ERROR( KNC_THREADRECV_INVALID_PTR );return 0; );
    return m_pPeerManager->GetAvgPingTime();
}

DWORD KncP2P::GetAvgPingTime( DWORD dwUID_ )
{
	_JIF( m_pPeerManager, SET_ERROR( KNC_THREADRECV_INVALID_PTR );return 0; );
	return m_pPeerManager->GetAvgPingTime(dwUID_);
}
UINT KncP2P::GetErrorInfo()
{
#ifdef _ENUM
#undef _ENUM
#define             _ENUM( ID, strError_ )                  ID,
#else
#define             _ENUM( ID, strError_ )                  ID,
#endif
    enum GC_CONNECTION_ERROR
    {
#include "../Knc/FailRate_def.h"
    };
    return P2P_NO_ERROR;
}

void KncP2P::SetServerDown( bool bDown_ )
{
    _JIF( m_pThreadRecv, SET_ERROR( KNC_KNCP2P_INVALID_PTR );return; );
    	m_pThreadRecv->SetServerDown( true );
}

void KncP2P::CreateConsoleWnd()
{
    AllocConsole();
    m_fpStream = freopen("CONOUT$", "a", stdout); // redirect printf to console
}
