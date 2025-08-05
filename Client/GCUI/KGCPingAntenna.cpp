#include "StdAfx.h"
#include "KGCPingAntenna.h"


IMPLEMENT_CLASSNAME( KGCPingAntenna );
IMPLEMENT_WND_FACTORY( KGCPingAntenna );
IMPLEMENT_WND_FACTORY_NAME( KGCPingAntenna, "gc_ping_antenna" );

#define PING_VERY_BEST  1.0f
#define PING_BEST       0.92f
#define PING_GOOD       0.78f
#define PING_NORMAL     0.65f
#define PING_BAD        0.52f
#define PING_VERY_BAD   0.4f
#define PING_WORST      0.22f

KGCPingAntenna::KGCPingAntenna(void) : m_vAntennaCoords( 0.0f, 0.0f, 0.0f, 0.0f )
{
    m_pkAntenna			= NULL;
    m_pkWorstPing		= NULL;

    LINK_CONTROL( "antenna",			m_pkAntenna );
    LINK_CONTROL( "worst_ping",			m_pkWorstPing );

    m_dwOriginalWidth = 0;
}

KGCPingAntenna::~KGCPingAntenna(void)
{
}

void KGCPingAntenna::OnCreate( void )
{
    m_vAntennaCoords = m_pkAntenna->GetWndTexCoords();
    m_pkAntenna->SetSelfInputCheck(false);
    m_pkAntenna->ToggleRender(true);
    m_pkWorstPing->SetSelfInputCheck(false);
    m_pkWorstPing->ToggleRender(false);

    if( m_dwOriginalWidth != 0 )
        m_pkAntenna->SetWidth(m_dwOriginalWidth);
    m_dwOriginalWidth = m_pkAntenna->GetWidth();

    SetPingStateClass();
    SetPingState(m_sPingStateClass.dwNormal);
}

void KGCPingAntenna::SetPingState( DWORD dwPing_ )
{
    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
    //LUA_GET_VALUE_DEF( "ping", dwPing_, 1 );

    float fPingRate = 0.f;
    if( m_sPingStateClass.dwVeryBest >= dwPing_ )
        fPingRate = PING_VERY_BEST;
    else if( m_sPingStateClass.dwVeryBest < dwPing_ && dwPing_ <= m_sPingStateClass.dwBest )
        fPingRate = PING_BEST;
    else if( m_sPingStateClass.dwBest < dwPing_ && dwPing_ <= m_sPingStateClass.dwGood )
        fPingRate = PING_GOOD;
    else if( m_sPingStateClass.dwGood < dwPing_ && dwPing_ <= m_sPingStateClass.dwNormal )
        fPingRate = PING_NORMAL;
    else if( m_sPingStateClass.dwNormal < dwPing_ && dwPing_ <= m_sPingStateClass.dwBad )
        fPingRate = PING_BAD;
    else if( m_sPingStateClass.dwBad < dwPing_ && dwPing_ <= m_sPingStateClass.dwVeryBad )
        fPingRate = PING_VERY_BAD;
    else
    {
        fPingRate = PING_VERY_BAD; // 완전히 끊겨버린 표현이 좀 안 좋은 듯... 그래서 최하 상태를 1칸으로 표시 하겠음.
    }

    float fTU = m_vAntennaCoords.x + fPingRate * ( m_vAntennaCoords.z - m_vAntennaCoords.x );

    m_pkAntenna->SetWndTexCoords( D3DXVECTOR4( m_vAntennaCoords.x, m_vAntennaCoords.y, fTU, m_vAntennaCoords.w ) );   
    m_pkAntenna->SetWidthDirect( static_cast<int>( fPingRate * m_dwOriginalWidth ) );	

    if( fPingRate == PING_WORST )
        m_pkWorstPing->ToggleRender(true);
    else
        m_pkWorstPing->ToggleRender(false);
}

void KGCPingAntenna::FrameMoveInEnabledState( void )
{
}

void KGCPingAntenna::SetPingStateClass()
{
    KLuaManager luaMgr;
    GCFUNC::LoadLuaScript( luaMgr, "PingCheckGrade.lua" );

    LUA_GET_VALUE_DEF( "PingState_VeryBest",    m_sPingStateClass.dwVeryBest,    0 );    
    LUA_GET_VALUE_DEF( "PingState_Best",        m_sPingStateClass.dwBest,        0 );    
    LUA_GET_VALUE_DEF( "PingState_Good",        m_sPingStateClass.dwGood,        0 );    
    LUA_GET_VALUE_DEF( "PingState_Normal",      m_sPingStateClass.dwNormal,      0 );    
    LUA_GET_VALUE_DEF( "PingState_Bad",         m_sPingStateClass.dwBad,         0 );    
    LUA_GET_VALUE_DEF( "PingState_VeryBad",     m_sPingStateClass.dwVeryBad,     0 );    
}
