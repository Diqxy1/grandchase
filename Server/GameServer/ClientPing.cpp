#include "ClientPing.h"
#include "dbg/dbg.hpp"
KClientPing::KClientPing(void)
:m_bFirst(true)
,m_nCurrentIndex(0)
,m_bOverOneCycle(false)
{
    InitPingInfo( INIT_VALUE );
    UpdateLastTick();
}

KClientPing::~KClientPing(void)
{
}

void KClientPing::InitPingInfo( IN const DWORD dwClinetPing_ )
{
    boost::array<DWORD,MAX_PING_COUNT>::iterator ait;
    for ( ait = m_aPingInfo.begin() ; ait != m_aPingInfo.end() ; ++ait ) {
        *ait = dwClinetPing_;
    }
}

void KClientPing::AddPing( IN const DWORD dwClientPing_ )
{
    UpdateLastTick();
    if ( m_bFirst ) {
        InitPingInfo( dwClientPing_ );
        m_bFirst = false;
    }
    else {
        m_aPingInfo[m_nCurrentIndex] = dwClientPing_;
    }

    ++m_nCurrentIndex;

    if ( !m_bOverOneCycle && m_nCurrentIndex >= MAX_PING_COUNT ) {
        m_bOverOneCycle = true;
    }

    m_nCurrentIndex = m_nCurrentIndex % MAX_PING_COUNT;


}

DWORD KClientPing::GetPing()
{
    DWORD dwResult = 0;

    boost::array<DWORD,MAX_PING_COUNT>::iterator ait;
    for ( ait = m_aPingInfo.begin() ; ait != m_aPingInfo.end() ; ++ait ) {
        dwResult += *ait;        
    }
    DWORD dwSize = MAX_PING_COUNT;

    if ( !m_bOverOneCycle ) {
        dwSize = (DWORD)(m_nCurrentIndex );
        dwSize = (dwSize <= 0 ? 1 : dwSize); // 0이 되는것을 방지.
    }

    return (dwResult / dwSize);
}

void KClientPing::GetAvgPingInfo( boost::array<DWORD,MAX_PING_COUNT>& avgPingInfo_ )
{
    avgPingInfo_ = m_aPingInfo;
}


DWORD KClientPing::GetPingGap()
{
    return ::GetTickCount() - m_dwLastRecvPingTick;
}

void KClientPing::UpdateLastTick()
{
    m_dwLastRecvPingTick = ::GetTickCount();
}

bool KClientPing::IsDelayPingTick( IN const DWORD dwUpdateTickGap_ )
{
    return ( GetPingGap() > dwUpdateTickGap_ );
}

bool KClientPing::IsLowSpeedPing( IN const DWORD dwLimitPingSpeed_ )
{
    return ( GetPing() > dwLimitPingSpeed_ );
}