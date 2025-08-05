#include "GCEclipseEventManager.h"
#include "KGCWorldMapManager.h"

ImplementSingleton( GCEclipseEventManager )

GCEclipseEventManager::GCEclipseEventManager(void)
: m_tmEventBegin(0)
, m_tmEventEnd(0)
, m_dwEventPeriod(0)
, m_dwExeciseEventTime(0)
{
}

GCEclipseEventManager::~GCEclipseEventManager(void)
{
}

bool GCEclipseEventManager::IsEventTime()
{
    time_t tmCurrentTime = g_kGlobalValue.TimeToInt(g_kGlobalValue.m_tmServerTime);

    if(m_tmEventBegin > tmCurrentTime|| m_tmEventEnd <= tmCurrentTime)
        return false;
    return true;
}

int GCEclipseEventManager::GetLeftEventTime()
{
    time_t tmCurrentTime = g_kGlobalValue.TimeToInt(g_kGlobalValue.m_tmServerTime);

    if(m_tmEventBegin > tmCurrentTime|| m_tmEventEnd <= tmCurrentTime)
        return 0;

    time_t tmTime = tmCurrentTime - m_tmEventBegin;

    int iPTime = static_cast<int>( tmTime % m_dwEventPeriod );

    return iPTime;
}

int GCEclipseEventManager::GetLeftEventTimePercent()
{
    int iPTime = GetLeftEventTime();

    if(static_cast<DWORD>(iPTime) < m_dwExeciseEventTime) return 100;

    int iWaitTime = iPTime - m_dwExeciseEventTime;

    int iPercent = (iWaitTime * 100) / m_dwExeciseEventTime;

    return iPercent;
}

void GCEclipseEventManager::SetEventTime( time_t tmBeginTime,time_t tmEndTime, DWORD dwPeriod, DWORD dwExecise )
{
    m_tmEventBegin = tmBeginTime + g_kGlobalValue.m_tmServerSyncTime;
    m_tmEventEnd = tmEndTime + g_kGlobalValue.m_tmServerSyncTime;

    m_dwEventPeriod = dwPeriod;
    m_dwExeciseEventTime = dwExecise;

}

bool GCEclipseEventManager::IsActiveEvent()
{
    time_t tmCurrentTime = g_kGlobalValue.TimeToInt(g_kGlobalValue.m_tmServerTime);

    if(m_tmEventBegin > tmCurrentTime|| m_tmEventEnd <= tmCurrentTime)
        return false;

    time_t tmTime = tmCurrentTime - m_tmEventBegin;
    int iPTime = static_cast<int>( tmTime % m_dwEventPeriod );
    if( static_cast<DWORD>(iPTime) >= m_dwExeciseEventTime )
        return false;

    return true;
}

bool GCEclipseEventManager::IsVeiwMode()
{
    if(IsEventTime() == false) return false;

    GAME_STATE eGameState = g_MyD3D->m_pStateMachine->GetState();
    
    if( eGameState != GS_WORLDMAP &&
        eGameState != GS_MATCH &&
        eGameState != GS_GUILD &&
        eGameState != GS_SHOP_CASH &&
        eGameState != GS_SHOP_GP &&
#ifdef USE_COORDI_SHOP
        eGameState != GS_COORDI_SHOP &&
#endif
        eGameState != GS_MY_INFO &&
        eGameState != GS_ROOM &&
        eGameState != GS_MY_INFO_FROM_ROOM ){
        return false;
    }
    return true;
}

bool GCEclipseEventManager::IsValidDungeon( const EGCGameMode eGameMode_ )
{
    // 영던 제외 
    if( true == SiKGCWorldMapManager()->IsHeroDungeonMode( eGameMode_ ) ) { 
        return false;
    }
        
    // 시련의 숲 ,  탑 , 펫던전 제외 
    switch( eGameMode_ ) 
    {
    case GC_GM_QUEST0:
    case GC_GM_QUEST1:
    case GC_GM_DOTA:
    case GC_GM_QUEST50:
    case GC_GM_QUEST54:
        return false;
    default:
        return true;
    }

    return true;
}
