#include "stdafx.h"
#include ".\kncpeer.h"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
KncPeer::KncPeer(void)
{
    InitPeer();
}
KncPeer::KncPeer( DWORD dwUID )
{
    InitPeer();
    SetUID( dwUID );
}
KncPeer::KncPeer( DWORD dwUID, PlayerID rakPeerID_ )
{
    InitPeer();
    SetRakPeerID( rakPeerID_ );
    SetUID( dwUID );
}
KncPeer::~KncPeer(void)
{
}
void KncPeer::InitPeer()
{
    m_bLocal        = false;
    m_bRelay        = false;
    m_kRakPeerID.binaryAddress = 0;
    m_kRakPeerID.port = 0;
    m_dwUID = 0xFFFFFFFF;
    m_strDesc       = L"";
    ClearPingTime();
}
bool KncPeer::operator==(  const KncPeer* pPeer )
{
    return m_dwUID == pPeer->m_dwUID;
}
bool KncPeer::operator!=( const KncPeer* pPeer )
{
    return !(this == pPeer);
}
void KncPeer::SetUID( DWORD dwUID_ )
{
    m_dwUID = dwUID_;
}
void KncPeer::SetRakPeerID( PlayerID rakPeerID_ )
{
    m_kRakPeerID = rakPeerID_;
}
void KncPeer::SetRakPeerID( DWORD dwIP_, unsigned short usPort_ )
{
    m_kRakPeerID.binaryAddress = dwIP_;
    m_kRakPeerID.port = usPort_;
}

void KncPeer::SetPing( const DWORD& dwPing_ )
{
	m_dwAvgPing = m_dwAvgPing * m_nPingCount + dwPing_;
    m_nPingCount++;

	m_dwAvgPing = (DWORD)((float)m_dwAvgPing / (float)m_nPingCount );

    SetPartPing( dwPing_ );
}

DWORD KncPeer::GetPing()
{
	return m_dwAvgPing;
}

void KncPeer::SetPartPing( const DWORD& dwPing_ )
{
    m_dwAvgPartPing = m_dwAvgPartPing * m_nPartPingCount + dwPing_;
    m_nPartPingCount++;

    m_dwAvgPartPing = (DWORD)((float)m_dwAvgPartPing / (float)m_nPartPingCount );
}

DWORD KncPeer::GetPartPing()
{
    return m_dwAvgPartPing;
}


void KncPeer::ClearPingTime()
{
    m_dwLastPingSendTime = 0;
    m_dwLastPingRecvTime = 0;
    m_dwAvgPing = INIT_MAX_PING;
    m_dwAvgPartPing = INIT_MAX_PING;
    m_iPingLossCount = 0;

    m_nPingCount = 0;
    m_nPartPingCount = 0;
}

void KncPeer::ClearPartPingTime()
{
    m_dwAvgPartPing = INIT_MAX_PING;
    m_nPartPingCount = 0;
}


void KncPeer::SetSendPingTime( const DWORD dwTime )
{
	m_dwLastPingSendTime = dwTime;
	m_dwLastPingRecvTime = 0;
}

void KncPeer::SetRecvPingTime( const DWORD dwTime )
{
	m_dwLastPingRecvTime = dwTime;
}

const WCHAR* KncPeer::ToString()
{
	in_addr in;
	in.s_addr = m_kRakPeerID.binaryAddress;
	WCHAR strIP[32] = {0,};
	MultiByteToWideChar( 437, 0, inet_ntoa( in ), -1, strIP, 32 );

	std::wostringstream strStream;
	strStream << L"\nUSER ID    : " << m_dwUID;
	strStream << L"\nLOCAL      : " << ( m_bLocal == true ? L"TRUE" : L"FALSE" );
	strStream << L"\nUSER RELAY : " << ( m_bRelay == true ? L"TRUE" : L"FALSE" );
	strStream << L"\nIP         : " << strIP;
	strStream << L"\nPORT       : " << m_kRakPeerID.port;
	m_strDesc = strStream.str();
	return m_strDesc.c_str();
}
