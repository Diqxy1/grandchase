#include "stdafx.h"
#include "GCAutoMatchManager.h"


ImplementSingleton( GCAutoMatchManager )


GCAutoMatchManager::GCAutoMatchManager()
{
    Reset();
}

GCAutoMatchManager::~GCAutoMatchManager()
{
}

void GCAutoMatchManager::Reset()
{
    for ( int i = 0; i < EM_AUTO_MATCH_MAX_MODE; ++i )
    {
        for ( int j = 0; j < EM_AM_STAT_SIZE; ++j ) 
        {
            m_sBallaceStat[i][j].Reset();
        }
    }

    for ( int i = 0; i < PMB_SIZE; ++i ) 
    {
        m_fPMBValue[i] = 1.0f;
    }
}

void GCAutoMatchManager::LoadScript()
{
    if ( !LoadScript( "AutoMatchBallance.lua" ) )
    {
        MessageBox( NULL, L"AutoMatchBallance.lua Load Error", L"", MB_OK );
    }
}

bool GCAutoMatchManager::LoadScript( std::string strFileName )
{
    KLuaManager luaMgr;

    Reset();

    if( GCFUNC::LoadLuaScript( luaMgr , strFileName.c_str() ) == false )
        return false;


    LUA_GET_VALUE("PMB_DIRECT_HP_DAMAGE",m_fPMBValue[PMB_DIRECT_HP_DAMAGE],return false);
    LUA_GET_VALUE("PMB_EQUIPMENT_AP",m_fPMBValue[PMB_EQUIPMENT_AP],return false);
    LUA_GET_VALUE("PMB_EQUIPMENT_DP",m_fPMBValue[PMB_EQUIPMENT_DP],return false);
    LUA_GET_VALUE("PMB_EQUIPMENT_HP",m_fPMBValue[PMB_EQUIPMENT_HP],return false);
    LUA_GET_VALUE("PMB_BASE_AP",m_fPMBValue[PMB_BASE_AP],return false);
    LUA_GET_VALUE("PMB_BASE_DP",m_fPMBValue[PMB_BASE_DP],return false);
    LUA_GET_VALUE("PMB_BASE_HP",m_fPMBValue[PMB_BASE_HP],return false);
    LUA_GET_VALUE("PMB_SPECIAL_AP",m_fPMBValue[PMB_SPECIAL_AP],return false);
    LUA_GET_VALUE("PMB_SPECIAL_DP",m_fPMBValue[PMB_SPECIAL_DP],return false);
    LUA_GET_VALUE("PMB_HP_RECORVERY",m_fPMBValue[PMB_HP_RECORVERY],return false);
    LUA_GET_VALUE("PMB_MP_RECORVERY",m_fPMBValue[PMB_MP_RECORVERY],return false);
    LUA_GET_VALUE("PMB_CRITICAL_AP",m_fPMBValue[PMB_CRITICAL_AP],return false);
    LUA_GET_VALUE("PMB_CRITICAL_PROBABILITY",m_fPMBValue[PMB_CRITICAL_PROBABILITY],return false);
    LUA_GET_VALUE("PMB_COUNTER_MP",m_fPMBValue[PMB_COUNTER_MP],return false);
    LUA_GET_VALUE("PMB_COUNTER_DP",m_fPMBValue[PMB_COUNTER_DP],return false);
    LUA_GET_VALUE("PMB_BUFF_AP",m_fPMBValue[PMB_BUFF_AP],return false);
    LUA_GET_VALUE("PMB_BUFF_DP",m_fPMBValue[PMB_BUFF_DP],return false);
    LUA_GET_VALUE("PMB_BUFF_HP",m_fPMBValue[PMB_BUFF_HP],return false);
    LUA_GET_VALUE("PMB_HELL_SPEAR",m_fPMBValue[PMB_HELL_SPEAR],return false);


    LUA_BEGIN_TABLE("AutoMacthBallaceStat",return false);
    {
        for(int nMode = 1; nMode <= EM_AUTO_MATCH_MAX_MODE; nMode++ )
        {
            LUA_BEGIN_TABLE(nMode,break)
            {

                for(int i = 1; ; i++)
                {
                    SAutoMatchStat sAutoMatchStat;
                    LUA_BEGIN_TABLE(i,break)
                    {
                        int nStatType = EM_AM_STAT_SIZE;

                        LUA_GET_VALUE("StatType",nStatType,return false);
                        LUA_GET_VALUE("MaxStat", sAutoMatchStat.m_fMaxStat,return false);
                        LUA_GET_VALUE("BasicStat", sAutoMatchStat.m_fBasicStat,return false);
                        LUA_GET_VALUE("RangeStat",sAutoMatchStat.m_fRangeStat,return false);

                        if ( nStatType < 0 || nStatType >= EM_AM_STAT_SIZE )
                            return false;

                        sAutoMatchStat.SetValid( true );

                        m_sBallaceStat[nMode-1][nStatType] = sAutoMatchStat;
                    }
                    LUA_END_TABLE(return false)
                }
            }
            LUA_END_TABLE(return false)
        }
    }
    LUA_END_TABLE(return false)

    return true;
}

float GCAutoMatchManager::GetAutoMatchBallanacStat( int iMode, EAbilityType eAbilityType, float fValue )
{
    switch ( eAbilityType ) 
    {
    case ABILITY_ATK:
        {
            return GetBallanacStat( iMode, EM_AM_STAT_AP, fValue );
        }
        break;
    case ABILITY_DEF:
        {
            return GetBallanacStat( iMode, EM_AM_STAT_DP, fValue );
        }
        break;
    case ABILITY_HP:
        {
            return GetBallanacStat( iMode, EM_AM_STAT_HP, fValue );
        }
        break;
    case ABILITY_SPECIAL_ATK:
        {
            return GetBallanacStat( iMode, EM_AM_STAT_SPEC_AP, fValue );
        }
        break;
    case ABILITY_SPECIAL_DEF:
        {
            return GetBallanacStat( iMode, EM_AM_STAT_SPEC_DP, fValue );
        }
        break;
    case ABILITY_CRITICAL_ATTACK_RATE:
        {
            float fResult = GetBallanacStat( iMode, EM_AM_STAT_CRITICAL, fValue * 100.0f );
            return fResult / 100.0f;
        }
        break;
    case ABILITY_CRITICAL_DAMAGE_UP:
        {
            return GetBallanacStat( iMode, EM_AM_STAT_CRITICAL_DAMAGE, fValue );
        }
        break;
    default:
        {
            return fValue;
        }
        break;
    }

    return fValue;
}

float GCAutoMatchManager::GetBallanacStat( int iMode, EMAutoMatchBallanceStat eStat, float fValue )
{   
    if ( iMode < 0 || iMode >= EM_AUTO_MATCH_MAX_MODE ) 
        return fValue;

    if ( !m_sBallaceStat[iMode][eStat].IsValid() ) 
        return fValue;

    float fRate = fValue / m_sBallaceStat[iMode][eStat].m_fMaxStat;

    if ( fRate >= 1.0f ) 
        fRate = 1.0f;

    if ( fRate < 0.0f )
        fRate = 0.0f;

    float fOffset = fRate * ( m_sBallaceStat[iMode][eStat].m_fRangeStat * 2 );
    float fResult = ( m_sBallaceStat[iMode][eStat].m_fBasicStat - m_sBallaceStat[iMode][eStat].m_fRangeStat ) + fOffset;

    return fResult;
}


void GCAutoMatchManager::GetPvPMatchingBalance( int iType, float &tVal )
{
    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON || ( g_MyD3D->m_pStateMachine->GetState() != GS_GAME && g_MyD3D->m_pStateMachine->GetState() != GS_GAME_LOADING ) ) {
        tVal = 1.0f;
        return;
    }

    if ( iType < 0 || iType >= PMB_SIZE )
    {
        tVal = 1.0f;
        return;
    }

    tVal = m_fPMBValue[iType];
}

bool GCAutoMatchManager::IsAutoMatchBalance()
{
    if ( SiKGCChannelManager()->IsAutoMatchChannel() ) 
        return true;

    // 대전 매칭 평준화 다른 대전 모드에서도 사용 여부
    if ( GC_GLOBAL_DEFINE::bUseAutoMatchBalance )
    {
        if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH || 
             SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
             SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH ) 
        {
            return true;
        }
    }

    return false;
}

bool GCAutoMatchManager::IsNotPetSpecial()
{
    if ( SiKGCChannelManager()->IsAutoMatchChannel() && GC_GLOBAL_DEFINE::bAutoMatchUsePetSpecial ) 
        return true;

    return false;
}

bool GCAutoMatchManager::IsNot4Special()
{
    if ( SiKGCChannelManager()->IsAutoMatchChannel() && GC_GLOBAL_DEFINE::bAutoMatch4Special )
        return true;

    return false;
}

int GCAutoMatchManager::GetCurrentBalanceMode()
{
    if ( SiKGCChannelManager()->IsAutoMatchChannel() ) 
    {
         return SiKGCMatchManager()->GetCurrentMode();
    }

    if ( SiKGCRoomManager()->GetGameMode() == GC_GM_SURVIVAL_MODE || 
         SiKGCRoomManager()->GetGameMode() == GC_GM_DEATH_SURVIVAL ||
         SiKGCRoomManager()->GetGameMode() == GC_GM_FATAL_DEATH_SURVIVAL )
    {
        return 0;   // 1vs1;
    }

    if ( SiKGCRoomManager()->GetGameMode() == GC_GM_TEAM_MODE ||
         SiKGCRoomManager()->GetGameMode() == GC_GM_DEATH_TEAM ||
         SiKGCRoomManager()->GetGameMode() == GC_GM_FATAL_DEATH_TEAM )
    { 
        int nMode = ( g_kGlobalValue.m_nPlayerCount / 2 ) - 1;
        if ( nMode < 0 || nMode >= EM_AUTO_MATCH_MAX_MODE ) 
            nMode = 0;
        return nMode;
    }

    return 0;
}