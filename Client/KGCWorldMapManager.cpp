#include "stdafx.h"
#include "KGCWorldMapManager.h"
#include "MyD3D.h"
//

#include "GCUI/KGCWorldMapPanel.h"
#include "GCUI/KGCWorldMapContainer.h"
#include "GCUI/KGCWorldMapIcon.h"
#include "GCUI/KGCWorldMapPath.h"

#include "KGCRoomManager.h"
#include "GCStateMachine.h"
#include "KGCRoomListManager.h"
#include "KGCRoomSceneS6.h"
#include "KGCPrivateDungeonManager.h"
#include "KGCInfinityDungeonManager.h"
#include "QuestGameManager.h"

//
//#define REG_GAME_MODE( map, dungeon, gamemode ) \
//    m_mapGameMode[map][dungeon].push_back( gamemode ); \

#define LINK_NODE( first, second ) \
    first->m_vecNext.push_back( second ) \

ImplementSingleton( KGCWorldMapManager )

KGCWorldMapManager::KGCWorldMapManager()
{
    m_eCurrentWorldMap      = WORLDMAP_UPPERMAP;

    m_iCurrentSelectDungeon = -1;
    m_iNewQuest = -1;
	m_dwLastRoomUpdateTime = 0;

	m_iLastRoomUpdateMap = -1;
    m_iCharBaseLife = 1;

    m_mapDungeonTipList.clear();
    m_vecSkillListGuideDungeonList.clear();
}

KGCWorldMapManager::~KGCWorldMapManager()
{
}

void KGCWorldMapManager::Init()
{
    SiKGCPrivateDungeonManager()->Init();
    SiKGCInfinityDungeonManager()->Init();

    if ( !LoadDungeonInfo() )
    {
        MessageBox( NULL, L"File Load Error", L"DungeonInfo.lua", MB_OK );
    }

    if ( !SiKGCInfinityDungeonManager()->LoadScript() )
    {
        MessageBox( NULL, L"File Load Error", L"InfinityDungeonInfo.lua", MB_OK );
    }

    InitWorldMap();

    return;
}

void KGCWorldMapManager::InitWorldMap()
{
	m_mapGameMode.clear();
	m_mapICONGameMode.clear();
	// 각 월드맵의 던전을 보기 쉽게 가짐.
	for ( MAP_DUNGEON_INFO_ITER mit = m_mapDungeonInfo.begin() ; mit != m_mapDungeonInfo.end() ; ++mit ) {
		for ( std::map<EGCWorldMap,std::pair<int, int>>::iterator mitWorld = mit->second.m_mapWorldMapInfo.begin() ; mitWorld != mit->second.m_mapWorldMapInfo.end() ; ++mitWorld ) {
			m_mapGameMode[mitWorld->first].push_back(mit->second.m_emGameMode);	
			m_mapICONGameMode[mitWorld->first][mitWorld->second.first] = mit->second.m_emGameMode;
		}
	}


}


bool KGCWorldMapManager::LoadDungeonInfo()
{
    m_mapDungeonInfo.clear();

    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
        return false;

    if( GCFUNC::LoadLuaScript( luaMgr, "etc.stg" ) == false )
        return false;

    if( GCFUNC::LoadLuaScript( luaMgr , "DungeonInfo.lua" ) == false )
        return false;

    m_mapWorldMapParticleInfo.clear();
    LUA_BEGIN_TABLE_RAII( "WorldMapParticleInfo" )
    {
        int iTableLoop=1;
        std::vector<SWorldMapParticle> vecParticle;
        while( 1 )
        {
            LUA_BEGIN_TABLE_RAII( iTableLoop++ )
            {
                int iWorldMap;
                LUA_GET_VALUE_DEF( "WORLDMAP", iWorldMap, 0 );

                vecParticle.clear();
                LUA_BEGIN_TABLE_RAII( "PARTICLE" )
                {
                    int iParticleLoop=1;
                    while( 1 )
                    {
                        LUA_BEGIN_TABLE_RAII( iParticleLoop++ )
                        {
                            float fX, fY;
                            std::string strParticle;
                            LUA_GET_VALUE_DEF( 1, strParticle, "" );
                            LUA_GET_VALUE_DEF( 2, fX, 0.0f );
                            LUA_GET_VALUE_DEF( 3, fY, 0.0f );

                            vecParticle.push_back( SWorldMapParticle(strParticle.c_str(), fX, fY) );
                        }
                        else
                            break;
                    }
                }

                m_mapWorldMapParticleInfo.insert( std::make_pair(iWorldMap, vecParticle) );
            }
            else
                break;
        }
    }

    //  캐릭터 전용 던전 관련 로드
    if ( !SiKGCPrivateDungeonManager()->LoadScript( luaMgr ) )
        return false;

	
    LUA_BEGIN_TABLE("WorldMapDungeonInfo",return false);
	{
		for(int i = 1; ; i++)
		{
			LUA_BEGIN_TABLE(i,break)
			{
				int iCulMode = GC_GM_INVALID;
				int iPreMode = GC_GM_INVALID;
                int iLevel = -1;

				LUA_GET_VALUE( 1, iCulMode, return false );
				LUA_GET_VALUE( 2, iPreMode, return false );
                LUA_GET_VALUE( 3, iLevel, return false );

				EGCGameMode eCulMode = static_cast<EGCGameMode>(iCulMode);
				EGCGameMode ePreMode = static_cast<EGCGameMode>(iPreMode);

                m_mapWorldMapDungeonInfo.insert( std::make_pair( eCulMode, std::make_pair( ePreMode, iLevel ) ) );
			}
			LUA_END_TABLE(return false)
		}
	}
	LUA_END_TABLE(return false)


    LUA_GET_VALUE("DUNGEON_BASE_CHARLIFE",m_iCharBaseLife,1);

    LUA_BEGIN_TABLE("DungeonInfo",return false);
    {
        for(int i = 1; ; i++)
        {
            SDungeonInfo sDungeonInfo;
            LUA_BEGIN_TABLE(i,break)
            {
                int nGameMode = GC_GM_INVALID;
                int nDungeonType = DUNGEON_TYPE_NORMAL;

                LUA_GET_VALUE( "DUNGEON_ID",nGameMode,return false );
                sDungeonInfo.m_emGameMode = static_cast<EGCGameMode>(nGameMode);                
                luaMgr.GetValue( "LOADING_IMG", sDungeonInfo.m_strLoadingImg );
                luaMgr.GetValue( "BREAK_IMG", sDungeonInfo.m_strLoadingBreakImg );
                LUA_GET_VALUE( "LIMIT_LEVEL", sDungeonInfo.m_nLimitLevel,return false );
                LUA_GET_VALUE( "HELL_MODE", sDungeonInfo.m_bHellMode, return false );
                LUA_GET_VALUE( "BREAK_MODE", sDungeonInfo.m_bBreakMode, sDungeonInfo.m_bBreakMode = false );
                LUA_GET_VALUE( "DUNGEON_TYPE",nDungeonType,return false);
                sDungeonInfo.m_emDungeonType = static_cast<EM_DUNGEON_TYPE>(nDungeonType);
                LUA_GET_VALUE( "DENGEON_NAME", sDungeonInfo.m_nDungeonName,return false );
                LUA_GET_VALUE( "WORLDMAP_NAME", sDungeonInfo.m_nWorldMapName,return false );
                luaMgr.GetValue( "SHOW_PLAYTIME", sDungeonInfo.m_bShowPlayTime );
                luaMgr.GetValue( "SHOW_RANK", sDungeonInfo.m_bShowRank );
                luaMgr.GetValue( "SHOW_TODAY_TIP", sDungeonInfo.m_bShowTodayTip );
                luaMgr.GetValue( "COMBO_EFFECT", sDungeonInfo.m_bComboEffect );
                LUA_GET_VALUE_DEF( "MAX_PLAYER_NUM", sDungeonInfo.m_nMaxPlayerNum, OLD_MAX_DUNGEON_PLAYER_NUM );
                LUA_GET_VALUE_DEF( "CHARACTER_LIFE", sDungeonInfo.m_iCharLife, -1 );
                luaMgr.GetValue( "USE_GAME_ITEM", sDungeonInfo.m_bUseGameItem );
                luaMgr.GetValue( "USE_PET_SKILL", sDungeonInfo.m_bUsePetSkill );
                luaMgr.GetValue( "NO_MPRECOVERY_MODE", sDungeonInfo.m_bNoRecoveryMode );     
                LUA_BEGIN_TABLE_RAII( "NOT_USESHORTCUTITEM" )
                {
                    int iLoop = 1, iTemp;
                    sDungeonInfo.m_vecNotUseShortCutItem.clear();
                    while( SUCCEEDED( luaMgr.GetValue( iLoop++, iTemp ) ) )
                    {
                        sDungeonInfo.m_vecNotUseShortCutItem.push_back( iTemp );
                    }
                }
                LUA_BEGIN_TABLE_RAII( "NOT_REBIRTH" )
                {
                    int iLoop = 1, iTemp;
                    sDungeonInfo.m_vecNotUseRebirth.clear();
                    while( SUCCEEDED( luaMgr.GetValue( iLoop++, iTemp ) ) )
                    {
                        sDungeonInfo.m_vecNotUseRebirth.push_back( iTemp );
                    }
                }
                LUA_BEGIN_TABLE_RAII("CAMERA_ZOOM")
                {
                    LUA_GET_VALUE_DEF("UI_POS_X", sDungeonInfo.m_sCameraZoom.m_iPosUI_X, 0);
                    LUA_GET_VALUE_DEF("UI_POS_Y", sDungeonInfo.m_sCameraZoom.m_iPosUI_Y, 0);
                    LUA_GET_VALUE_DEF("FirstZoom", sDungeonInfo.m_sCameraZoom.m_fFristZoom, 0.0f);
                    LUA_GET_VALUE_DEF("NotJumpZoom", sDungeonInfo.m_sCameraZoom.m_bNotJumpZoom, false);
                    LUA_GET_VALUE_DEF("NotDashZoom", sDungeonInfo.m_sCameraZoom.m_bNotDashZoom, false);
					LUA_GET_VALUE_DEF("ShowDecs", sDungeonInfo.m_sCameraZoom.m_bShowDesc, true);
                    LUA_GET_VALUE_DEF("NotSpecialZoom", sDungeonInfo.m_sCameraZoom.m_bNotSpecialZoom, false);
                    LUA_GET_VALUE_DEF("NotBossDieEffect", sDungeonInfo.m_sCameraZoom.m_bNotBossDieEffect, false);
                    LUA_BEGIN_TABLE_RAII("ZoomStep")
                    {
                        int iLoop=1;
                        float fTemp;
                        while(1)
                        {
                            LUA_GET_VALUE_RAII( iLoop++, fTemp )
                                sDungeonInfo.m_sCameraZoom.m_vecZoomStep.push_back( fTemp );
                            else
                                break;
                        }
                    }
                }
                LUA_BEGIN_TABLE_RAII("REALMINIMAP_INFO")
                {
                    sDungeonInfo.m_sMiniMapInfo.Init();

                    LUA_GET_VALUE_DEF( "UI_POS_X", sDungeonInfo.m_sMiniMapInfo.m_iPosUI_X, 0);
                    LUA_GET_VALUE_DEF( "UI_POS_Y", sDungeonInfo.m_sMiniMapInfo.m_iPosUI_Y, 0);
                    LUA_GET_VALUE_DEF( "UI_WIDTH", sDungeonInfo.m_sMiniMapInfo.m_iWidthUI, 0);
                    LUA_GET_VALUE_DEF( "UI_HEIGHT", sDungeonInfo.m_sMiniMapInfo.m_iHeightUI, 0);
                    LUA_GET_VALUE_DEF( "RENEW_FRAME", sDungeonInfo.m_sMiniMapInfo.m_iRenewFrame, 1);
                    sDungeonInfo.m_sMiniMapInfo.m_bIsShow = true;
                    LUA_GET_VALUE_DEF( "FOOTHOLD_SCALE", sDungeonInfo.m_sMiniMapInfo.m_fScale_FootHold, 1.0f);
                    LUA_GET_VALUE_DEF( "ENEMY_SCALE", sDungeonInfo.m_sMiniMapInfo.m_fScale_Enemy, 1.0f);
                    LUA_GET_VALUE_DEF( "ALLY_SCALE", sDungeonInfo.m_sMiniMapInfo.m_fScale_Ally, 1.0f);
                    LUA_GET_VALUE_DEF( "MY_SCALE", sDungeonInfo.m_sMiniMapInfo.m_fScale_My, 1.0f);
                    LUA_BEGIN_TABLE_RAII("ENEMY_TEXTURE")
                    {
                        std::string strFileName;
                        D3DXVECTOR4 vUV;
                        luaMgr.GetValue( 1, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_Enemy.m_strFileName );
                        luaMgr.GetValue( 2, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_Enemy.m_vUV.x );
                        luaMgr.GetValue( 3, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_Enemy.m_vUV.y );
                        luaMgr.GetValue( 4, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_Enemy.m_vUV.z );
                        luaMgr.GetValue( 5, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_Enemy.m_vUV.w );
                    }
                    LUA_BEGIN_TABLE_RAII("ALLY_TEXTURE")
                    {
                        std::string strFileName;
                        D3DXVECTOR4 vUV;
                        luaMgr.GetValue( 1, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_Ally.m_strFileName );
                        luaMgr.GetValue( 2, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_Ally.m_vUV.x );
                        luaMgr.GetValue( 3, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_Ally.m_vUV.y );
                        luaMgr.GetValue( 4, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_Ally.m_vUV.z );
                        luaMgr.GetValue( 5, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_Ally.m_vUV.w );
                    }
                    LUA_BEGIN_TABLE_RAII("MY_TEXTURE")
                    {
                        std::string strFileName;
                        D3DXVECTOR4 vUV;
                        luaMgr.GetValue( 1, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_My.m_strFileName );
                        luaMgr.GetValue( 2, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_My.m_vUV.x );
                        luaMgr.GetValue( 3, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_My.m_vUV.y );
                        luaMgr.GetValue( 4, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_My.m_vUV.z );
                        luaMgr.GetValue( 5, sDungeonInfo.m_sMiniMapInfo.m_kTexInfo_My.m_vUV.w );
                    }
                    LUA_BEGIN_TABLE_RAII("FOOTHOLD_COLOR")
                    {
                        int iA, iR, iG, iB;
                        LUA_GET_VALUE_DEF( 1, iA, 0);
                        LUA_GET_VALUE_DEF( 2, iR, 0);
                        LUA_GET_VALUE_DEF( 3, iG, 0);
                        LUA_GET_VALUE_DEF( 4, iB, 0);
                        sDungeonInfo.m_sMiniMapInfo.m_dxColor_FootHold = D3DCOLOR_ARGB(iA, iR, iG, iB);
                    }
                    LUA_BEGIN_TABLE_RAII("ENEMY_COLOR")
                    {
                        int iA, iR, iG, iB;
                        LUA_GET_VALUE_DEF( 1, iA, 0);
                        LUA_GET_VALUE_DEF( 2, iR, 0);
                        LUA_GET_VALUE_DEF( 3, iG, 0);
                        LUA_GET_VALUE_DEF( 4, iB, 0);
                        sDungeonInfo.m_sMiniMapInfo.m_dxColor_Enemy = D3DCOLOR_ARGB(iA, iR, iG, iB);
                    }
                    LUA_BEGIN_TABLE_RAII("ALLY_COLOR")
                    {
                        int iA, iR, iG, iB;
                        LUA_GET_VALUE_DEF( 1, iA, 0);
                        LUA_GET_VALUE_DEF( 2, iR, 0);
                        LUA_GET_VALUE_DEF( 3, iG, 0);
                        LUA_GET_VALUE_DEF( 4, iB, 0);
                        sDungeonInfo.m_sMiniMapInfo.m_dxColor_Ally = D3DCOLOR_ARGB(iA, iR, iG, iB);
                    }
                    LUA_BEGIN_TABLE_RAII("MY_COLOR")
                    {
                        int iA, iR, iG, iB;
                        LUA_GET_VALUE_DEF( 1, iA, 0);
                        LUA_GET_VALUE_DEF( 2, iR, 0);
                        LUA_GET_VALUE_DEF( 3, iG, 0);
                        LUA_GET_VALUE_DEF( 4, iB, 0);
                        sDungeonInfo.m_sMiniMapInfo.m_dxColor_My = D3DCOLOR_ARGB(iA, iR, iG, iB);
                    }
                    LUA_BEGIN_TABLE_RAII("ETC_OBJECT")
                    {
                        int iLoop=1;
                        while( SUCCEEDED( luaMgr.BeginTable( iLoop++ ) ) )
                        {
                            int iETC_ID, iOBG_ID, iA, iR, iG, iB;
                            LUA_GET_VALUE_DEF( 1, iETC_ID, 0);
                            LUA_GET_VALUE_DEF( 2, iOBG_ID, 0);
                            LUA_GET_VALUE_DEF( 3, iA, 0);
                            LUA_GET_VALUE_DEF( 4, iR, 0);
                            LUA_GET_VALUE_DEF( 5, iG, 0);
                            LUA_GET_VALUE_DEF( 6, iB, 0);
                            sDungeonInfo.m_sMiniMapInfo.m_mapETCColor.insert( std::make_pair(std::make_pair(iETC_ID, iOBG_ID), D3DCOLOR_ARGB(iA, iR, iG, iB)) );
                            luaMgr.EndTable();
                        }
                    }
                }
                LUA_BEGIN_TABLE_RAII("LIMITED_TIME_INFO")
                {
                    SAlarmInfo sAlarmInfo;
                    LUA_BEGIN_TABLE_RAII("ALARM")
                    {
                        int iLoop=1;
                        while(1)
                        {
                            LUA_BEGIN_TABLE_RAII( iLoop++ )
                            {
                                LUA_GET_VALUE_DEF( 1, sAlarmInfo.iMitMin, 0);
                                LUA_GET_VALUE_DEF( 2, sAlarmInfo.iSecMin, 0);
                                LUA_GET_VALUE_DEF( 3, sAlarmInfo.iMitMax, 0);
                                LUA_GET_VALUE_DEF( 4, sAlarmInfo.iSecMax, 0);
                                LUA_GET_VALUE_DEF( 5, sAlarmInfo.strSound, "");
                            }
                            else
                                break;
                            sDungeonInfo.m_sLimitedTimeInfo.m_vecAlarmInfo.push_back( sAlarmInfo );
                        }
                    }
                }
                LUA_BEGIN_TABLE_RAII("ABILITY_REVISION")
                {
                    LUA_GET_VALUE_DEF("AP_STD_LEVEL", sDungeonInfo.m_sAbilityRevision.iStdLevel_AP, 0);
                    LUA_GET_VALUE_DEF("DP_STD_LEVEL", sDungeonInfo.m_sAbilityRevision.iStdLevel_DP, 0);
                    LUA_GET_VALUE_DEF("HP_STD_LEVEL", sDungeonInfo.m_sAbilityRevision.iStdLevel_HP, 0);
                    LUA_GET_VALUE_DEF("SpecialAP", sDungeonInfo.m_sAbilityRevision.fSpecialAP, 0.0f);
                    LUA_GET_VALUE_DEF("DrainHP", sDungeonInfo.m_sAbilityRevision.fDrainHP, 0.0f);
                    LUA_GET_VALUE_DEF("DrainMP", sDungeonInfo.m_sAbilityRevision.fDrainMP, 0.0f);
                    LUA_GET_VALUE_DEF("RecoveryHP", sDungeonInfo.m_sAbilityRevision.fRecoveryHP, 0.0f);
                    LUA_GET_VALUE_DEF("RecoveryMP", sDungeonInfo.m_sAbilityRevision.fRecoveryMP, 0.0f);
                    LUA_GET_VALUE_DEF("CriticalRate", sDungeonInfo.m_sAbilityRevision.fCriticalRate, 0.0f);
                    LUA_GET_VALUE_DEF("CounterDEF", sDungeonInfo.m_sAbilityRevision.fCounterDEF, 0.0f);
                    LUA_GET_VALUE_DEF("DarkATK", sDungeonInfo.m_sAbilityRevision.fDarkATK, 0.0f);
                    LUA_GET_VALUE_DEF("CriticalATK", sDungeonInfo.m_sAbilityRevision.fCriticalATK, 0.0f);
                    LUA_GET_VALUE_DEF("PetMPRecovery", sDungeonInfo.m_sAbilityRevision.fPetMPRecovery, 0.0f);
                    LUA_GET_VALUE_DEF("PortionUP", sDungeonInfo.m_sAbilityRevision.fPortionUP, 0.0f);
                    LUA_GET_VALUE_DEF("NoItemAbility", sDungeonInfo.m_sAbilityRevision.bNoItemAbility, false);
                    sDungeonInfo.m_sAbilityRevision.bLoadTable = true;
                }
                sDungeonInfo.m_mapMacroCommunity.clear();
                LUA_BEGIN_TABLE_RAII("MACRO_COMMUNITY")
                {
                    int iLoop=1;
                    char szTemp[1024];
                    SMacroCommunity sMacroCommunity;
                    while(1)
                    {
                        sprintf(szTemp, "Slot%d", iLoop++);
                        LUA_BEGIN_TABLE_RAII( szTemp )
                        {
                            LUA_GET_VALUE_DEF( 1, sMacroCommunity.m_strTexture, "");
                            LUA_GET_VALUE_DEF( 2, sMacroCommunity.m_iStringID, -1);
                            LUA_GET_VALUE_DEF( 3, sMacroCommunity.m_iStringColorR, 255);
                            LUA_GET_VALUE_DEF( 4, sMacroCommunity.m_iStringColorG, 255);
                            LUA_GET_VALUE_DEF( 5, sMacroCommunity.m_iStringColorB, 255);
                            LUA_GET_VALUE_DEF( 6, sMacroCommunity.m_iReEntryCool, 0);
                            LUA_GET_VALUE_DEF( 7, sMacroCommunity.m_iSoundID, -1);
                        }
                        else
                            break;

                        sDungeonInfo.m_mapMacroCommunity.insert( std::make_pair( (iLoop-1), sMacroCommunity ) );
                    }
                }
                sDungeonInfo.m_mapMapParticle.clear();
                LUA_BEGIN_TABLE_RAII( "DUN_PARTICLE" )
                {
                    int iTableLoop=1;
                    std::vector<SWorldMapParticle> vecParticle;
                    while( 1 )
                    {
                        LUA_BEGIN_TABLE_RAII( iTableLoop++ )
                        {
                            int iWorldMap;
                            LUA_GET_VALUE_DEF( "WORLDMAP", iWorldMap, 0 );

                            vecParticle.clear();
                            LUA_BEGIN_TABLE_RAII( "PARTICLE" )
                            {
                                int iParticleLoop=1;
                                while( 1 )
                                {
                                    LUA_BEGIN_TABLE_RAII( iParticleLoop++ )
                                    {
                                        float fX, fY;
                                        std::string strParticle;
                                        LUA_GET_VALUE_DEF( 1, strParticle, "" );
                                        LUA_GET_VALUE_DEF( 2, fX, 0.0f );
                                        LUA_GET_VALUE_DEF( 3, fY, 0.0f );

                                        vecParticle.push_back( SWorldMapParticle(strParticle.c_str(), fX, fY) );
                                    }
                                    else
                                        break;
                                }
                            }

                            sDungeonInfo.m_mapMapParticle.insert( std::make_pair(iWorldMap, vecParticle) );
                        }
                        else
                            break;
                    }
                }
                LUA_BEGIN_TABLE("WORLDMAP_INFO",return false)
                {
                    for(int j = 1; ; j++)
                    {
                        LUA_BEGIN_TABLE(j,break)
                        {
                            int nWorldMap = WORLDMAP_BERMESIA_SERDIN;
                            int nIcon = 0;
							int nPath = -1;

                            LUA_GET_VALUE( 1, nWorldMap, return false );
							LUA_GET_VALUE( 2, nIcon, return false );
							LUA_GET_VALUE_DEF( 3, nPath, -1 );

                            EGCWorldMap emWorldMap = static_cast<EGCWorldMap>(nWorldMap);

							sDungeonInfo.m_mapWorldMapInfo.insert( std::make_pair( emWorldMap, std::pair<int,int>(nIcon,nPath)) );                            
                        }
                        LUA_END_TABLE(return false)
                    }
                }
                LUA_END_TABLE(return false)
            }
            m_mapDungeonInfo.insert(std::make_pair(sDungeonInfo.m_emGameMode, sDungeonInfo));
            LUA_END_TABLE(return false)
        }
    }
    LUA_END_TABLE(return false)
    LUA_BEGIN_TABLE_RAII("DungeonTipList")
    {
        std::vector<int> vecStrIDList;
        vecStrIDList.clear();

        LUA_BEGIN_TABLE_RAII("Default")
        {
            for( int i = 1; ; ++i ) {
                int iValue = 0;
                if( luaMgr.GetValue( i, iValue ) == E_FAIL ) {
                    break;
                }
                vecStrIDList.push_back(iValue);
            }
            m_mapDungeonTipList.insert( std::make_pair( 0, vecStrIDList ) );
            vecStrIDList.clear();
        }
        for(int i = 1; ; i++) 
        {
            LUA_BEGIN_TABLE_RAII(i)
            {
                int nDungeon;
                LUA_GET_VALUE( 1, nDungeon, continue );

                for( int j = 2; ; ++j ) {
                    int iValue = 0;
                    if( luaMgr.GetValue( j, iValue ) == E_FAIL ) {
                        break;;
                    }
                    vecStrIDList.push_back(iValue);
                }
                m_mapDungeonTipList.insert( std::make_pair( nDungeon, vecStrIDList ) );
                vecStrIDList.clear();
            }
            else
            {
                break;
            }
        }
    }
    LUA_BEGIN_TABLE_RAII("SkillListGuideOpenDungeon")
    {
        for( int i = 1; ; ++i ) {
            int iValue = 0;
            if( luaMgr.GetValue( i, iValue ) == E_FAIL ) {
                break;
            }
            m_vecSkillListGuideDungeonList.push_back(iValue);
        }
    }

    return true;
}

void KGCWorldMapManager::UpdateWorldMap()
{
	EnableObject();

    // 경험치, 활력 정보 업데이트
    g_pkUIScene->m_pkWorldMapContainer->SetVitalityInfo();
}

int KGCWorldMapManager::GetWorldMapIconNum( EGCWorldMap eWorldmap , int iMode)
{
#ifdef DIMENSIONAL_BREAK
    if (eWorldmap == WORLDMAP_NEWMAP) {
        return iMode;
    }
#endif

    if (eWorldmap == WORLDMAP_UPPERMAP) {
        return iMode;
    }
	
	MAP_DUNGEON_INFO::iterator mit = m_mapDungeonInfo.find(static_cast<EGCGameMode>(iMode));

	if ( mit == m_mapDungeonInfo.end() ) {
		return -1;
	}

	std::map<EGCWorldMap,pair<int, int>>::iterator mitInfo = mit->second.m_mapWorldMapInfo.find(eWorldmap);

	if ( mitInfo == mit->second.m_mapWorldMapInfo.end() ) {
		return -1;
	}

	return mitInfo->second.first;
}

int KGCWorldMapManager::GetWorldMapPathNum( EGCWorldMap eWorldmap , int iMode)
{
#ifdef DIMENSIONAL_BREAK
    if (eWorldmap == WORLDMAP_NEWMAP) {
        return iMode;
    }
#endif

    if (eWorldmap == WORLDMAP_UPPERMAP) {
        return iMode;
    }

	MAP_DUNGEON_INFO::iterator mit = m_mapDungeonInfo.find(static_cast<EGCGameMode>(iMode));

	if ( mit == m_mapDungeonInfo.end() ) {
		return -1;
	}
    
//	m_vecTreeRoot.push_back( apkWorldMapIcon[BERMESIA_EVENT_REVOLT] );

	std::map<EGCWorldMap,pair<int, int>>::iterator mitInfo = mit->second.m_mapWorldMapInfo.find(eWorldmap);

	if ( mitInfo == mit->second.m_mapWorldMapInfo.end() ) {
		return -1;
	}

	return mitInfo->second.second;
}


void KGCWorldMapManager::EnableObject()
{
    if (GetCurrentWorldMap() == WORLDMAP_UPPERMAP) {
        EnableWorldObject();
    }
    else {
#ifdef DIMENSIONAL_BREAK
        if (GetCurrentWorldMap() == WORLDMAP_NEWMAP) {
            EnableWorldObject();
        }
        else {
            EnableDungeonObject();
        }
#else
        EnableDungeonObject();
#endif
    }
}

bool KGCWorldMapManager::IsEnableWorldObject( EGCGameMode eCurGameMode ) 
{
    if ( IsPlayableDungeon( eCurGameMode, g_kGlobalValue.m_kUserInfo.GetCurrentCharType() ) )
		return true;

	return false;
}



bool KGCWorldMapManager::IsPlayableDungeon( EGCGameMode eCurGameMode, int iCharID ) 
{
	if ( g_kGlobalValue.IsUnlock() )
		return true;


    //  캐릭터 전용 던전일 경우
    if ( IsPrivateDungeon( eCurGameMode ) )
    {
        // 누구의 전용던전인가?         
        int iPrivateChar = SiKGCPrivateDungeonManager()->GetPrivateDungeonChar( eCurGameMode );
        if ( iPrivateChar == GC_CHAR_INVALID )
            return false;

        // 다른 캐릭터로는 플레이 할 수 없다.
        if ( iPrivateChar != iCharID ) 
            return false;

        return _IsPlayableDungeon( eCurGameMode, g_kGlobalValue.m_kUserInfo.mapStagePlayable );
    }
    else
    {
        // 캐릭터 전용 던전이 있는 캐릭터 일경우 졸업 여부 체크한다.
        if ( SiKGCPrivateDungeonManager()->IsHavePrivateDungeon( iCharID ) )
        {
            if ( SiKGCPrivateDungeonManager()->IsClearPrivateDungeon( iCharID ) )
            {
                if ( g_kGlobalValue.IsDungeonFreePass() )
                    return true;

                if ( IsHeroDungeonMode(eCurGameMode) )
                    return true;

                if ( IsEventDungeon( eCurGameMode ) )
                    return true;

                return _IsPlayableDungeon( eCurGameMode, g_kGlobalValue.m_kUserInfo.mapStagePlayable );
            }
            else
            {
                return false;
            }
        }
        else
        {
            if ( IsHeroDungeonMode(eCurGameMode) )
                return true;

            if ( IsEventDungeon( eCurGameMode ) )
                return true;

            if ( g_kGlobalValue.IsDungeonFreePass() )
                return true;

            return _IsPlayableDungeon( eCurGameMode, g_kGlobalValue.m_kUserInfo.mapStagePlayable );
        }
    }


	return false;
}

bool KGCWorldMapManager::_IsPlayableDungeon( EGCGameMode eCurGameMode, std::map<int,KDungeonUserInfo>& mapStagePlayable_ )
{
    std::map< EGCGameMode, std::pair<EGCGameMode,int> >::iterator mit = m_mapWorldMapDungeonInfo.find(eCurGameMode);
    if ( mit == m_mapWorldMapDungeonInfo.end() )
        return false;

    if ( mit->second.first == GC_GM_INVALID )
        return true;

    //  해당 게임모드의 난이도 플레이 기록이 있는지 체크
    if ( IsPlayedDungeon( mit->second.first, mit->second.second, mapStagePlayable_ ) )
        return true;

    return false;
}

void KGCWorldMapManager::EnableWorldObject()
{
    // 해당 대륙의 특정 던전을 플레이 가능하다면 해당 대륙이 열린다.
    // 단, 영웅던전 이벤트 던전은 제외한다. 
	KGCWorldMapPanel* pkWorldMapPanel = g_pkUIScene->m_pkWorldMapContainer->m_apkWorldMapPanel[ GetCurrentWorldMap() ];

    std::map< int, std::vector< EGCGameMode > >::iterator mit = m_mapGameMode.begin();
    for( ; mit != m_mapGameMode.end(); mit++ )
    {
        EGCUpperWorldMapIcon eIcon = GetWorldMapIconFromEnum( mit->first );
        
        if ( eIcon == NUM_UPPERWORLDMAP ) continue;
        if ( pkWorldMapPanel->m_apkWorldMapIcon[eIcon] == NULL ) continue;        
        
        std::vector<EGCGameMode>& vecGameMode = mit->second;
        std::vector<EGCGameMode>::iterator vit = vecGameMode.begin();

        bool bRender = false;

        for ( ; vit != vecGameMode.end(); vit++ )
        {
            if ( IsHeroDungeonMode( *vit ) )
                continue;

            if ( IsEventDungeon( *vit ) )
                continue;

            bRender = IsEnableWorldObject( *vit );
            bRender = bRender && SiKGCRoomManager()->IsAvailableGameMode(GC_GMC_DUNGEON,*vit);

            if ( bRender )
                break;
        }

        pkWorldMapPanel->m_apkWorldMapIcon[eIcon]->ToggleRender(bRender);            
    }
}
bool KGCWorldMapManager::CheckQuestInContinent(EGCWorldMap eWorldMap)
{
    KGCWorldMapPanel* pkWorldMapPanel = g_pkUIScene->m_pkWorldMapContainer->m_apkWorldMapPanel[ eWorldMap ];
    
    std::vector<EGCGameMode> &vecGameMod = m_mapGameMode[ eWorldMap ];
    
    for ( std::vector<EGCGameMode>::iterator vit = vecGameMod.begin() ; vit != vecGameMod.end() ; ++vit ) 
    {
        if( !IsInfinityDungeon( *vit ) && g_kGlobalValue.m_kUserMission.IsMissionDungeon( *vit ) )
            return true;
    }

    return false;
}
void KGCWorldMapManager::EnableDungeonObject()
{
	KGCWorldMapPanel* pkWorldMapPanel = g_pkUIScene->m_pkWorldMapContainer->m_apkWorldMapPanel[ GetCurrentWorldMap() ];

    PLAYER* pPlayer = NULL;
    pPlayer = &g_MyD3D->m_TempPlayer;
    int iCurrentCharLevel = pPlayer->GetCurrentCharLevel();

	std::vector<EGCGameMode> &vecGameMod = m_mapGameMode[m_eCurrentWorldMap];
	for ( std::vector<EGCGameMode>::iterator vit = vecGameMod.begin() ; vit != vecGameMod.end() ; ++vit ) {
		
		int iICON = GetWorldMapIconNum(GetCurrentWorldMap(), *vit);
		int iPath = GetWorldMapPathNum(GetCurrentWorldMap(), *vit);
		bool bRender = IsPlayableDungeon( *vit, g_kGlobalValue.m_kUserInfo.GetCurrentCharType() );
        bool bOpenedDungeon = SiKGCRoomManager()->IsAvailableGameMode(GC_GMC_DUNGEON,*vit);
		bRender = bRender && bOpenedDungeon;
        
#ifdef DIMENSIONAL_BREAK
        if (m_eCurrentWorldMap != WORLDMAP_UPPERMAP || m_eCurrentWorldMap != WORLDMAP_NEWMAP)
#else
        if (m_eCurrentWorldMap != WORLDMAP_UPPERMAP)
#endif
        {
            pkWorldMapPanel->m_apkWorldMapIcon[iICON]->AllOffEpicMissionIcon();
            QuestFileInfo kQuestFileInfo = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode( *vit, 0 );
            if ( !IsInfinityDungeon( *vit ) && g_kGlobalValue.m_kUserMission.IsMissionDungeon( *vit ) )
            {
                pkWorldMapPanel->m_apkWorldMapIcon[iICON]->SetMissionIcon( true );
            }
            else
            {
                pkWorldMapPanel->m_apkWorldMapIcon[iICON]->SetMissionIcon( false );

                if( IsInfinityDungeon( *vit ) ) { 
                    pkWorldMapPanel->m_apkWorldMapIcon[iICON]->AllOffEpicMissionIcon();
                }
                else if ( bRender )
                {
                    int iIconType = SiKGCMissionManager()->GetEpicMissionIconType( *vit, iCurrentCharLevel );
                    if ( iIconType != KGCWorldMapIcon::EICON_MAX )
                        pkWorldMapPanel->m_apkWorldMapIcon[iICON]->SetEpicMissionIcon(iIconType);
                }
            }
        }


		if ( iICON != -1 && pkWorldMapPanel->m_apkWorldMapIcon[iICON] != NULL ) {
            //bOpenedDungeon은 InitClientContents.lua에서 열어 놓았을 경우 true로 들어온다.
            //해당변수로 분기 통해 해외 국가의 던전 자동 오픈 기능이 동작한다.
            if( bOpenedDungeon ) {
                pkWorldMapPanel->m_apkWorldMapIcon[iICON]->ToggleRender( true );
                pkWorldMapPanel->m_apkWorldMapIcon[iICON]->RenderDungeonIcon(bRender);
            }
            else
                pkWorldMapPanel->m_apkWorldMapIcon[iICON]->ToggleRender(false);
		}

		if ( iPath != -1 && pkWorldMapPanel->m_apkWorldMapPath[iPath] != NULL ) {
                pkWorldMapPanel->m_apkWorldMapPath[iPath]->RenderPath( bRender );
		}
	}
}

void KGCWorldMapManager::SelectIcon( int iIconIndex )
{
    switch( m_eCurrentWorldMap )
    {

#ifdef DIMENSIONAL_BREAK
    case WORLDMAP_NEWMAP:
    {
        int iWorldMapIndex = WORLDMAP_NEWMAP;
        iWorldMapIndex = GetWorldMapEnumFromWorldMapIcon(iIconIndex);

        SetCurrentSelectDungeon(-1);
        SetCurrentWorldMap(iWorldMapIndex);
        g_pkUIScene->m_pkWorldMapContainer->UpdateWorldMap();
        break;
    }
#endif

    case WORLDMAP_UPPERMAP:
        {
            int iWorldMapIndex = WORLDMAP_BERMESIA_SERDIN;
            iWorldMapIndex = GetWorldMapEnumFromWorldMapIcon( iIconIndex );

            SetCurrentSelectDungeon( -1 );
            SetCurrentWorldMap( iWorldMapIndex );
            g_pkUIScene->m_pkWorldMapContainer->UpdateWorldMap();
            break;
        }
    case WORLDMAP_BERMESIA_SERDIN:
    default:
        {
            int iPrvIconIndex = GetCurrentSelectDungeon();            
            // 같은걸 선택했다면?
            m_iCurrentSelectDungeon = iIconIndex;
            if(g_MyD3D->m_pStateMachine->GetState() == GS_ROOM )
            {
                EGCGameMode emCurrentGameMode = SiKGCRoomManager()->GetPrevRoomInfo().eGameMode;
                EGCGameMode emSelectGameMode = GetGameMode();
                EM_DUNGEON_TYPE emRoomType = GetDungeonType( SiKGCRoomManager()->GetPrevRoomInfo().eGameMode );
                EM_DUNGEON_TYPE emSelectType = GetDungeonType( GetGameMode() );

                bool bOK = true;

                switch( emRoomType ) 
                {
                case DUNGEON_TYPE_HERO:
                case DUNGEON_TYPE_RELAY_HERO:
                case DUNGEON_TYPE_RAID_HERO:
                case DUNGEON_TYPE_RAID_INFINITY:
                    {
                        if ( emRoomType != emSelectType ) 
                        {
                            //영웅던전 방에서 일반던전을 선택할 수 없습니다.
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_FROM_HERO_DUNGEON_TO_DUNGEON ), 
                                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                            m_iCurrentSelectDungeon = iPrvIconIndex;
                            bOK = false;
                        }

                    }
                    break;
                case DUNGEON_TYPE_INFINITY:
                    {
                        //이벤트 던전 방에서는 다른 던전을 선택할 수 없습니다.
                        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_FROM_DUNGEON_TO_DIFFDUNGEON ), 
                            L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                        m_iCurrentSelectDungeon = iPrvIconIndex;
                        bOK = false;
                    }
                    break;
                case DUNGEON_TYPE_EVENT:
                    {
                            //이벤트 던전 방에서는 다른 던전을 선택할 수 없습니다.
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_FROM_EVENT_DUNGEON_TO_DIFFDUNGEON ), 
                                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                            m_iCurrentSelectDungeon = iPrvIconIndex;
                            bOK = false;
                    }
                    break;
                case DUNGEON_TYPE_NORMAL:
                    {
                        if ( emSelectType == DUNGEON_TYPE_HERO || emSelectType == DUNGEON_TYPE_RELAY_HERO || 
                             emSelectType == DUNGEON_TYPE_RAID_HERO ) 
                        {
                            //영웅던전 방에서 일반던전을 선택할 수 없습니다.
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_FROM_DUNGEON_TO_HERO_DUNGEON ), 
                                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                            m_iCurrentSelectDungeon = iPrvIconIndex;
                            bOK = false;
                        }
                        
                        if( emSelectType == DUNGEON_TYPE_EVENT )
                        {
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_FROM_DUNGEON_TO_EVENT_DUNGEON ), 
                                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                            m_iCurrentSelectDungeon = iPrvIconIndex;
                            bOK = false;
                        }

                        if( emSelectType == DUNGEON_TYPE_INFINITY || emSelectType == DUNGEON_TYPE_RAID_INFINITY )
                        {
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_FROM_DUNGEON_TO_DIFFDUNGEON ), 
                                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                            m_iCurrentSelectDungeon = iPrvIconIndex;
                            bOK = false;
                        }

                        //  일반 방에서 캐릭터 전용던전으로 이동이 불가능하다.
                        if ( !IsPrivateDungeon( emCurrentGameMode ) && IsPrivateDungeon( emSelectGameMode ) )
                        {
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_FROM_NORMAL_TO_PRIVATE_DUNGEON ), 
                                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                            m_iCurrentSelectDungeon = iPrvIconIndex;
                            bOK = false; 
                        }

                        //  캐릭터 전용던전에서 일반던전으로 이동이 불가능하다.
                        if ( IsPrivateDungeon( emCurrentGameMode ) && !IsPrivateDungeon( emSelectGameMode ) )
                        {
                            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK,g_pkStrLoader->GetString( STR_ID_FROM_PRIVATE_TO_NORMAL_DUNGEON ), 
                                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
                            m_iCurrentSelectDungeon = iPrvIconIndex;
                            bOK = false; 
                        }

                    }
                    break;
                default:
                    break;
                }

                if ( !bOK ) 
                    break;
            }      
            
            //임시 기능 셋팅 - 영웅던전이나 이벤트 던전이라면 
            if( IsHeroDungeonMode( EGCGameMode(iIconIndex ) ) )
            {
                SetCurrentSelectHeroDungeon( iIconIndex );   
                if( g_MyD3D->m_pStateMachine->GetState() != GS_ROOM )
                {
                    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_ROOM_LIST_DLG_S6);
                }
                return;
            }

            if ( IsEventDungeon( EGCGameMode(iIconIndex) ) )
            {
                SetCurrentSelectHeroDungeon( iIconIndex );
                if ( g_MyD3D->m_pStateMachine->GetState() != GS_ROOM )
                    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ROOM_LIST_DLG_S6 );
                return;
            }


            //비활성화된 던전이라면 던전 선택 셋팅을 하지 않는다.
            KGCWorldMapPanel* pkWorldMapPanel = g_pkUIScene->m_pkWorldMapContainer->m_apkWorldMapPanel[ GetCurrentWorldMap() ];
            if( !pkWorldMapPanel->m_apkWorldMapIcon[iIconIndex]->GetCurrentIconActived() )
                break;

			SetCurrentSelectDungeon( iIconIndex );       
            if (g_MyD3D->m_pStateMachine->GetState() != GS_ROOM)
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_ROOM_LIST_DLG_S6);
            }
            else {
				SiKGCRoomManager()->InitGameMode();

				g_pkUIScene->RoomSceneUpdateData();

				if( !( SiKGCRoomManager()->GetPrevRoomInfo() == SiKGCRoomManager()->GetRoomInfo() ) )
					KP2P::GetInstance()->Send_ChangeRoomInfoReq( &SiKGCRoomManager()->GetRoomInfo() );


                g_pkUIScene->m_pkRoomSceneS6->ShowWorldMap( false );

                // 맵 이펙트 없에자
				g_pkUIScene->m_pkWorldMapContainer->ClearMapEffect();
			}

            break;

        }
    }

}

EGCGameMode KGCWorldMapManager::GetGameMode()
{
    //영웅던전이면 대륙에 종속된 던전 정보에서 찾지 않고 바로 현재 게임 모드를 리턴한다.
    if( GetDungeonType( (EGCGameMode)m_iCurrentSelectDungeon ) == DUNGEON_TYPE_HERO       || 
        GetDungeonType( (EGCGameMode)m_iCurrentSelectDungeon ) == DUNGEON_TYPE_RAID_HERO  || 
        GetDungeonType( (EGCGameMode)m_iCurrentSelectDungeon ) == DUNGEON_TYPE_RELAY_HERO ||
        GetDungeonType( (EGCGameMode)m_iCurrentSelectDungeon ) == DUNGEON_TYPE_EVENT
        )
    {
        return (EGCGameMode)m_iCurrentSelectDungeon;
    }

	std::map<int,std::map<int,EGCGameMode>>::iterator mit = m_mapICONGameMode.find(GetCurrentWorldMap());

	if ( mit == m_mapICONGameMode.end() ) {
		return GC_GM_INVALID;
	}

	std::map<int,EGCGameMode>::iterator mitGame = mit->second.find(m_iCurrentSelectDungeon);

	if( mitGame == mit->second.end() ) {
		return GC_GM_INVALID;
	}

	return mitGame->second;
}

EGCGameMode KGCWorldMapManager::GetGameMode( int iIconIndex )
{
    //영웅던전이나 이벤트 던전이면 대륙에 종속된 던전 정보에서 찾지 않고 바로 현재 게임 모드를 리턴한다.
    if( GetDungeonType( (EGCGameMode)m_iCurrentSelectDungeon ) == DUNGEON_TYPE_HERO       || 
        GetDungeonType( (EGCGameMode)m_iCurrentSelectDungeon ) == DUNGEON_TYPE_RAID_HERO  || 
        GetDungeonType( (EGCGameMode)m_iCurrentSelectDungeon ) == DUNGEON_TYPE_RELAY_HERO ||
        GetDungeonType( (EGCGameMode)m_iCurrentSelectDungeon ) == DUNGEON_TYPE_EVENT
       )
    {
        return (EGCGameMode)iIconIndex;
    }

	std::map<int,std::map<int,EGCGameMode>>::iterator mit = m_mapICONGameMode.find(GetCurrentWorldMap());

	if ( mit == m_mapICONGameMode.end() ) {
		return GC_GM_INVALID;
	}

	std::map<int,EGCGameMode>::iterator mitGame = mit->second.find(iIconIndex);

	if( mitGame == mit->second.end() ) {
		return GC_GM_INVALID;
	}

	return mitGame->second;
}

int KGCWorldMapManager::GetDungeonIndex( EGCGameMode iGameMode )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( iGameMode );
    if ( pos == m_mapDungeonInfo.end() ) 
        return 0;

    int iIndex = 0;

    iIndex = std::distance( m_mapDungeonInfo.begin(), pos );
    return iIndex;
}

std::wstring KGCWorldMapManager::GetCurrentDungeonName()
{
    return GetCurrentDungeonName( GetGameMode() );
}

std::wstring KGCWorldMapManager::GetCurrentDungeonName( EGCGameMode eMode )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eMode );
    if ( pos == m_mapDungeonInfo.end() )
        return L"NULL";

     return g_pkStrLoader->GetString( pos->second.m_nDungeonName );
}

int KGCWorldMapManager::GetDungeonStringNumber(EGCGameMode iDungeonNum)
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( iDungeonNum );
    if ( pos == m_mapDungeonInfo.end() )
        return 0;

    return pos->second.m_nDungeonName;
}

bool KGCWorldMapManager::CheckContinentEnable( EGCWorldMap eWorldMap )
{
    // 상위 월드맵이면 월드맵에서 갈 수 있는 지역이 두군데 이상이어야 한다.
    if ( eWorldMap == WORLDMAP_UPPERMAP ) 
    {
        int nCount = 0;

        for ( int i = 0; i < NUM_WORLDMAP; ++i ) 
        {
            if ( i == WORLDMAP_UPPERMAP ) 
                continue;
            
            if ( _CheckContinentEnable( static_cast<EGCWorldMap>(i) ) )
                nCount++;
        }

        // 갈수 있는 지역이 2군데 이상이면
        if ( nCount >= 2) 
            return true;
        else
            return false;
    }
    // 일반 대륙이면 해당 대륙에 진입할 수 있는 던전이 있어야 한다.
    else
    {
        return _CheckContinentEnable( eWorldMap );
    }
}

bool KGCWorldMapManager::_CheckContinentEnable( EGCWorldMap eWorldMap )
{
    if ( eWorldMap == WORLDMAP_UPPERMAP )
        return false;

    std::map< int, std::vector< EGCGameMode > >::iterator mit = m_mapGameMode.find( eWorldMap );
    if ( mit == m_mapGameMode.end() )
        return false;

    std::vector<EGCGameMode>& vecGameMode = mit->second;
    std::vector<EGCGameMode>::iterator vit = vecGameMode.begin();

    bool bOpen = false;

    for ( ; vit != vecGameMode.end(); vit++ )
    {
        if ( IsHeroDungeonMode( *vit ) )
            continue;

        if ( IsEventDungeon( *vit ) )
            continue;

        bOpen = IsEnableWorldObject( *vit );
        bOpen = bOpen && SiKGCRoomManager()->IsAvailableGameMode(GC_GMC_DUNGEON,*vit);

        if ( bOpen )
            break;
    }

    return bOpen;
}

EGCWorldMap KGCWorldMapManager::GetPlayableWorldMap()
{
    for (int i = 0; i < NUM_WORLDMAP; ++i)
    {
#ifdef DIMENSIONAL_BREAK
        if (i == WORLDMAP_UPPERMAP || i == WORLDMAP_NEWMAP)
#else
        if (i == WORLDMAP_UPPERMAP)
#endif
            continue;

        if (_CheckContinentEnable(static_cast<EGCWorldMap>(i)))
            return static_cast<EGCWorldMap>(i);
    }

    return WORLDMAP_BERMESIA_SERDIN;
}

int KGCWorldMapManager::GetLastDungeonIndex()
{
	std::map< int, std::vector< EGCGameMode > >::iterator mit = m_mapGameMode.find(GetCurrentWorldMap());

	if(mit == m_mapGameMode.end()) return GC_GM_INVALID;

	EGCGameMode eLastIndex = GC_GM_INVALID;

	for ( std::vector<EGCGameMode>::iterator vit = mit->second.begin(); vit != mit->second.end() ; ++vit ) {
		if ( eLastIndex == GC_GM_INVALID ) {
			eLastIndex = *vit;
			continue;
		}

		QuestFileInfo kQuestFileInfo1 = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode( *vit, 0 );
		QuestFileInfo kQuestFileInfo2 = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode( eLastIndex, 0 );

		if ( kQuestFileInfo1.iLowLevel > kQuestFileInfo2.iLowLevel ) {
			eLastIndex = *vit;
		}
	}

	return eLastIndex;
}

EGCWorldMap KGCWorldMapManager::GetWorldMapEnumFromWorldMapIcon( int iWorldMapIconID )
{
    if( iWorldMapIconID < 0 || iWorldMapIconID >= NUM_UPPERWORLDMAP )
        return WORLDMAP_BERMESIA_SERDIN;
    switch( iWorldMapIconID )
    {
    case UPPERWORLDMAP_BERMESIA_SERDIN:     return WORLDMAP_BERMESIA_SERDIN;
    case UPPERWORLDMAP_BERMESIA_CANAVAN:    return WORLDMAP_BERMESIA_CANAVAN;
    case UPPERWORLDMAP_ELIA:                return WORLDMAP_ELIA;
    case UPPERWORLDMAP_XENEA:               return WORLDMAP_XENEA;
    case UPPERWORLDMAP_SILVERLAND:	        return WORLDMAP_SILVERLAND;
    case UPPERWORLDMAP_ATUM:		        return WORLDMAP_ATUM;
    case UPPERWORLDMAP_ACHAEMEDIA:	        return WORLDMAP_ACHAEMEDIA;
    case UPPERWORLDMAP_FLOSLAND:            return WORLDMAP_FLOSLAND;
    case UPPERWORLDMAP_DEVILDOM:            return WORLDMAP_DEVILDOM;
#ifdef ELYOS
    case UPPERWORLDMAP_ELYOS:               return WORLDMAP_ELYOS;
#endif
#ifdef DIMENSIONAL_BREAK
    case UPPERWORLDMAP_NEWMAP:              return WORLDMAP_NEWMAP;
#endif
    }
    return WORLDMAP_BERMESIA_SERDIN;
}

KGCWorldMapManager::EGCUpperWorldMapIcon KGCWorldMapManager::GetWorldMapIconFromEnum( int iWorldMapEnum )
{
    if( iWorldMapEnum < 0 || iWorldMapEnum >= NUM_WORLDMAP )
        return NUM_UPPERWORLDMAP;
    
    switch( iWorldMapEnum )
    {
    case WORLDMAP_BERMESIA_SERDIN:      return UPPERWORLDMAP_BERMESIA_SERDIN;
    case WORLDMAP_BERMESIA_CANAVAN:     return UPPERWORLDMAP_BERMESIA_CANAVAN;
    case WORLDMAP_ELIA:                 return UPPERWORLDMAP_ELIA;
    case WORLDMAP_XENEA:                return UPPERWORLDMAP_XENEA;
    case WORLDMAP_SILVERLAND:	        return UPPERWORLDMAP_SILVERLAND;
    case WORLDMAP_ATUM:		            return UPPERWORLDMAP_ATUM;
    case WORLDMAP_ACHAEMEDIA:	        return UPPERWORLDMAP_ACHAEMEDIA;
    case WORLDMAP_FLOSLAND:             return UPPERWORLDMAP_FLOSLAND;
    case WORLDMAP_DEVILDOM:             return UPPERWORLDMAP_DEVILDOM;
#ifdef ELYOS
    case WORLDMAP_ELYOS:                return UPPERWORLDMAP_ELYOS;
#endif
#ifdef DIMENSIONAL_BREAK
    case WORLDMAP_NEWMAP:               return UPPERWORLDMAP_NEWMAP;
#endif
    }
    return NUM_UPPERWORLDMAP;
}

void KGCWorldMapManager::ShowDungeonMissionList( int iIconIndex )
{
#ifdef DIMENSIONAL_BREAK
    if (m_eCurrentWorldMap == WORLDMAP_UPPERMAP || m_eCurrentWorldMap == WORLDMAP_NEWMAP)
#else
    if (m_eCurrentWorldMap == WORLDMAP_UPPERMAP)
#endif
        return;

    if( iIconIndex == m_iMissionListedIndex || ( iIconIndex < 0 || iIconIndex >= MAX_WORLDMAP_ICON ))
        return;

    if( iIconIndex != m_iMissionListedIndex )
    {
        HideDungeonMissionList( m_iMissionListedIndex );
    }

    KGCWorldMapContainer* pkWorldMapContainer = g_pkUIScene->m_pkWorldMapContainer;
    KGCWorldMapPanel* pkWorldMapPanel = pkWorldMapContainer->m_apkWorldMapPanel[ m_eCurrentWorldMap ];

    pkWorldMapContainer->ShowDungeonMissionList( GetGameMode( iIconIndex ), pkWorldMapPanel->GetWorldMapIcon( iIconIndex)->GetFixedWindowLocalPos() + D3DXVECTOR2(100.0f, 0.f) );

    m_iMissionListedIndex = iIconIndex;
}

void KGCWorldMapManager::HideDungeonMissionList( int iIconIndex )
{
    KGCWorldMapContainer* pkWorldMapContainer = g_pkUIScene->m_pkWorldMapContainer;

    if( iIconIndex == m_iMissionListedIndex )
    {
        pkWorldMapContainer->HideDungeonMissionList();
        m_iMissionListedIndex = -1;
    }
}

int KGCWorldMapManager::GetDungeonIconIndex( EGCGameMode eGameMode_ )
{
	MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
	if ( pos == m_mapDungeonInfo.end() ) 
		return -1;

	std::map<EGCWorldMap,std::pair<int, int>>::iterator mit =pos->second.m_mapWorldMapInfo.find(GetCurrentWorldMap());

	if ( mit == pos->second.m_mapWorldMapInfo.end() ) {
		return -1;
	}

	return mit->second.first;
}

void KGCWorldMapManager::SetCurrentSelectGameMode( int iGameMode)
{
	SetCurrentSelectDungeon(GetDungeonIconIndex(static_cast<EGCGameMode>(iGameMode)));
}

void KGCWorldMapManager::SetCurrentSelectDungeon( int iSelect )
{
    m_iCurrentSelectDungeon = iSelect;

    int iGameMode = GetGameMode();
    if( iGameMode != GC_GM_INVALID )
        SiKGCRoomListManager()->SetRoomFilter( GetGameMode(), true );
}
void KGCWorldMapManager::SetCurrentSelectHeroDungeon( int iSelect )
{
    m_iCurrentSelectDungeon = iSelect;
    if( iSelect != GC_GM_INVALID )
        SiKGCRoomListManager()->SetRoomFilter( iSelect, true );
}

void KGCWorldMapManager::SetCurrentWorldMap( int iCurrentWorldMap )
{
    m_eCurrentWorldMap = (EGCWorldMap)iCurrentWorldMap;

    int iGameMode = GetGameMode();
    if( iGameMode != GC_GM_INVALID )
        SiKGCRoomListManager()->SetRoomFilter( GetGameMode(), true );
}

EM_DUNGEON_TYPE KGCWorldMapManager::GetDungeonType( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() ) 
        return DUNGEON_TYPE_INVALID;

    return pos->second.m_emDungeonType;
}

bool KGCWorldMapManager::IsRaidHeroDungeon( EGCGameMode eGameMode_ )
{
    EGCGameMode eGameMode;

    if( eGameMode_ == GC_GM_INVALID )
        eGameMode = GetGameMode();
    else
        eGameMode = eGameMode_;       

    if ( GetDungeonType( eGameMode ) == DUNGEON_TYPE_RAID_HERO )
        return true;
    else 
        return false;
}

bool KGCWorldMapManager::IsRelayHeroDungeon( EGCGameMode eGameMode_ )
{
    EGCGameMode eGameMode;

    if( eGameMode_ == GC_GM_INVALID )
        eGameMode = GetGameMode();
    else
        eGameMode = eGameMode_;       

    if ( GetDungeonType( eGameMode ) == DUNGEON_TYPE_RELAY_HERO )
        return true;
    else 
        return false;
}

bool KGCWorldMapManager::IsHeroDungeon( EGCGameMode eGameMode_ )
{
    EGCGameMode eGameMode;
    
    if( eGameMode_ == GC_GM_INVALID )
        eGameMode = GetGameMode();
    else
        eGameMode = eGameMode_;       


    if ( GetDungeonType( eGameMode ) == DUNGEON_TYPE_HERO )
        return true;
    else 
        return false;
}

bool KGCWorldMapManager::IsHeroDungeonMode( EGCGameMode eGameMode_ )
{
    EGCGameMode eGameMode;

    if( eGameMode_ == GC_GM_INVALID )
        eGameMode = GetGameMode();
    else
        eGameMode = eGameMode_;       

    EM_DUNGEON_TYPE emDungeonType = GetDungeonType( eGameMode );

    if ( emDungeonType == DUNGEON_TYPE_HERO )
        return true;
    else if ( emDungeonType == DUNGEON_TYPE_RELAY_HERO ) 
        return true;
    else if ( emDungeonType == DUNGEON_TYPE_RAID_HERO )
        return true;
    else 
        return false;
}
bool KGCWorldMapManager::IsExistEventDungeon( )
{
    MAP_DUNGEON_INFO_ITER itor = m_mapDungeonInfo.begin();

    for( ; itor != m_mapDungeonInfo.end(); ++itor )
    {
        if( (*itor).second.m_emDungeonType == DUNGEON_TYPE_EVENT )
            return true;
    }
    return false;
}

bool KGCWorldMapManager::IsEventDungeon( EGCGameMode eGameMode_ )
{
    EGCGameMode eGameMode;

    if( eGameMode_ == GC_GM_INVALID )
        eGameMode = GetGameMode();
    else
        eGameMode = eGameMode_;         
    
    
    if ( GetDungeonType( eGameMode ) == DUNGEON_TYPE_EVENT || eGameMode == GC_GM_QUEST71)
        return true;
    else 
        return false;
}

bool KGCWorldMapManager::IsInfinityDungeon( EGCGameMode eGameMode_ )
{
    EGCGameMode eGameMode;

    if( eGameMode_ == GC_GM_INVALID )
        eGameMode = GetGameMode();
    else
        eGameMode = eGameMode_;         


    if ( GetDungeonType( eGameMode ) == DUNGEON_TYPE_INFINITY || GetDungeonType( eGameMode ) == DUNGEON_TYPE_RAID_INFINITY )
        return true;
    else 
        return false;
}

bool KGCWorldMapManager::IsInfinityRaid( EGCGameMode eGameMode_ )
{
    EGCGameMode eGameMode;

    if (eGameMode_ == GC_GM_INVALID)
        eGameMode = GetGameMode();
    else
        eGameMode = eGameMode_;


    if (GetDungeonType(eGameMode) == DUNGEON_TYPE_RAID_INFINITY)
        return true;
    else
        return false;
}

bool KGCWorldMapManager::IsPrivateDungeon( EGCGameMode eGameMode_ )
{
    EGCGameMode eGameMode;

    if( eGameMode_ == GC_GM_INVALID )
        eGameMode = GetGameMode();
    else
        eGameMode = eGameMode_;


    return SiKGCPrivateDungeonManager()->IsPrivateDungeon( eGameMode );
}

std::wstring KGCWorldMapManager::GetContinentName( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return L"NULL";

    return g_pkStrLoader->GetString( pos->second.m_nWorldMapName );
}

SCameraZoom* KGCWorldMapManager::GetCameraZoomInfo( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return NULL;

    return &pos->second.m_sCameraZoom;
}

SAbilityRevision* KGCWorldMapManager::GetAbilityRevisionInfo( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return NULL;

    return &pos->second.m_sAbilityRevision;
}

std::vector<SWorldMapParticle> KGCWorldMapManager::GetDungeonParticle( EGCGameMode eGameMode_, EGCWorldMap iWorldMap_ )
{
    std::vector<SWorldMapParticle> vecInfo;
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return vecInfo;

    std::map<int, std::vector<SWorldMapParticle>>::iterator mapiter = pos->second.m_mapMapParticle.find( iWorldMap_ );
    if( mapiter == pos->second.m_mapMapParticle.end() )
        return vecInfo;

    return mapiter->second;
}

std::vector<SWorldMapParticle> KGCWorldMapManager::GetMapParticleInfo( EGCWorldMap iWorldMap_ )
{
    std::vector<SWorldMapParticle> vecInfo;
    std::map< int, std::vector<SWorldMapParticle> >::iterator mapIter = m_mapWorldMapParticleInfo.find( iWorldMap_ );
    if( mapIter != m_mapWorldMapParticleInfo.end() )
    {
        return mapIter->second;
    }
    return vecInfo;
}

SMiniMapInfo* KGCWorldMapManager::GetMiniMapInfo( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return NULL;

    return &pos->second.m_sMiniMapInfo;
}

SLimitedTimeInfo* KGCWorldMapManager::GetLimitedTimeInfo( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return NULL;

    return &pos->second.m_sLimitedTimeInfo;
}

std::map<int, SMacroCommunity> KGCWorldMapManager::GetMacroCommunity( EGCGameMode eGameMode_ )
{
    std::map<int, SMacroCommunity> mapInfo;
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return mapInfo;

    return pos->second.m_mapMacroCommunity;
}

bool KGCWorldMapManager::IsShowPlayTime( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return true;

    return pos->second.m_bShowPlayTime;
}

bool KGCWorldMapManager::IsShowRank( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return true;

    return pos->second.m_bShowRank;
}

bool KGCWorldMapManager::IsShowTodayTip( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return true;

    return pos->second.m_bShowTodayTip;
}

bool KGCWorldMapManager::IsShowComboEffect( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return true;

    return pos->second.m_bComboEffect;
}

bool KGCWorldMapManager::IsUseGameItem( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return true;

    return pos->second.m_bUseGameItem;
}

bool KGCWorldMapManager::IsNotUseShortCutItem( EGCGameMode eGameMode_, int iLevel )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return false;

    std::vector<int>::iterator vecIter = pos->second.m_vecNotUseShortCutItem.begin();
    for(; vecIter != pos->second.m_vecNotUseShortCutItem.end(); ++vecIter)
    {
        if( *vecIter == iLevel )
            return true;
    }

    return false;
}

bool KGCWorldMapManager::IsUsePetSkill( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return true;

    return pos->second.m_bUsePetSkill;
}

bool KGCWorldMapManager::IsNoMPRecoveryMode( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return false;

    return pos->second.m_bNoRecoveryMode;
}

bool KGCWorldMapManager::IsNotRebirth(EGCGameMode eGameMode_, int iLevel)
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find(eGameMode_);
    if (pos == m_mapDungeonInfo.end())
        return false;

    std::vector<int>::iterator vecIter = pos->second.m_vecNotUseRebirth.begin();
    for (; vecIter != pos->second.m_vecNotUseRebirth.end(); ++vecIter)
    {
        if (*vecIter == iLevel)
            return true;
    }

    return false;
}

bool KGCWorldMapManager::IsOpenHellMode( EGCGameMode eGameMode_, std::map<int,KDungeonUserInfo>& mapStagePlayable_ )
{
    
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() ) 
        return false;

    if ( !pos->second.m_bHellMode ) 
        return false;

    int nLimiteLevel = pos->second.m_nLimitLevel;


    std::map<int,KDungeonUserInfo>::iterator pos_map = mapStagePlayable_.find( static_cast<int>(eGameMode_) );
    if ( pos_map == mapStagePlayable_.end() ) 
        return false;

    KDungeonUserInfo& kDungeonUserInfo = pos_map->second;

    for ( int i = 0; i < nLimiteLevel; ++i ) 
    {
        //  모든 일반 난이도를 클리어야 해야지 헬모드가 열린다.
        if ( !kDungeonUserInfo.m_kModeDifficulty.Get(i) ) 
            return false;
    }

    return true;
}

//해당 던전의 헬모드를 플레이 할 수 있는가?? 이전 레벨을 완수 했는가??
bool KGCWorldMapManager::IsOpenHellMode( ) 
{
	return IsOpenHellMode( GetGameMode(), g_kGlobalValue.m_kUserInfo.mapStagePlayable );
}

bool KGCWorldMapManager::IsOpenBreakMode(EGCGameMode eGameMode_, std::map<int, KDungeonUserInfo>& mapStagePlayable_)
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find(eGameMode_);
    if (pos == m_mapDungeonInfo.end())
        return false;

    if (!pos->second.m_bBreakMode)
        return false;

    std::map<int, KDungeonUserInfo>::iterator pos_map = mapStagePlayable_.find(static_cast<int>(eGameMode_));
    if (pos_map == mapStagePlayable_.end())
        return false;

    KDungeonUserInfo& kDungeonUserInfo = pos_map->second;

    if (!kDungeonUserInfo.m_kModeDifficulty.Get(255))
        return false;

    return true;
}

//해당 던전의 헬모드를 플레이 할 수 있는가?? 이전 레벨을 완수 했는가??
bool KGCWorldMapManager::IsOpenBreakMode()
{
    return IsOpenBreakMode(GetGameMode(), g_kGlobalValue.m_kUserInfo.mapStagePlayable);
}


bool KGCWorldMapManager::IsPlayedDungeon( EGCGameMode eGameMode_, int nLevel, std::map<int,KDungeonUserInfo>& mapStagePlayable_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() ) 
        return false;

    int nLimiteLevel = pos->second.m_nLimitLevel;

    std::map<int,KDungeonUserInfo>::iterator pos_map = mapStagePlayable_.find( static_cast<int>(eGameMode_) );
    if ( pos_map == mapStagePlayable_.end() ) 
        return false;

    KDungeonUserInfo& kDungeonUserInfo = pos_map->second;


    if ( nLevel == -1 )
    {
        for ( int i = 0; i < nLimiteLevel; ++i ) 
        {
            //  하나의 난이도라도 클리어했으면 다음 던전이 열린다.
            if ( kDungeonUserInfo.m_kModeDifficulty.Get(i) ) 
                return true;
        }

        //  헬모드면 헬모드도 체크해준다.
        if ( pos->second.m_bHellMode ) 
        {
            if( kDungeonUserInfo.m_kModeDifficulty.Get(GC_DUNGEON_LEVEL_HELL) )
                return true;
        }

        if (pos->second.m_bBreakMode)
        {
            if (kDungeonUserInfo.m_kModeDifficulty.Get(GC_DUNGEON_LEVEL_BREAK))
                return true;
        }

        if ( !kDungeonUserInfo.m_kModeDifficulty.empty() )
            return true;
    }
    else
    {
        if ( kDungeonUserInfo.m_kModeDifficulty.Get( nLevel ) )
            return true;
    }   

    return false;
}

bool KGCWorldMapManager::IsHellMode( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() ) 
        return false;

    if ( !pos->second.m_bHellMode ) 
        return false;

    return true;
}

bool KGCWorldMapManager::IsHellMode( )
{
	return IsHellMode( GetGameMode());
}

bool KGCWorldMapManager::IsBreakMode(EGCGameMode eGameMode_)
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find(eGameMode_);
    if (pos == m_mapDungeonInfo.end())
        return false;

    if (!pos->second.m_bBreakMode)
        return false;

    return true;
}

bool KGCWorldMapManager::IsBreakMode()
{
    return IsBreakMode(GetGameMode());
}

bool KGCWorldMapManager::IsEnablePartyDungeon( EGCGameMode eGameMode_ )
{
    switch( eGameMode_ ) 
    {
    case GC_GM_QUEST35:
    case GC_GM_QUEST36:
    case GC_GM_QUEST37:
    case GC_GM_QUEST38:
    case GC_GM_QUEST63:
    case GC_GM_QUEST64:
        return false;
    default:
        break;
    }

    if ( IsHeroDungeonMode( eGameMode_ ) )
        return false;

    if ( IsEventDungeon( eGameMode_ ) )
        return false;

    if ( IsPrivateDungeon( eGameMode_ ) )
        return false;

    if ( IsInfinityDungeon( eGameMode_ ) )
        return false;

    return true;
}

int KGCWorldMapManager::GetLimiteOpenLevel( EGCGameMode eGameMode_ )
{
    if (IsRelayHeroDungeon(eGameMode_))
    {
        MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find(eGameMode_);
        if (pos == m_mapDungeonInfo.end())
            return 1;

        return pos->second.m_nLimitLevel;
    }
    else
    {
        MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find(eGameMode_);
        if (pos == m_mapDungeonInfo.end())
            return 1;

        return pos->second.m_nLimitLevel;
    }
}

int KGCWorldMapManager::GetLimiteOpenLevel( )
{
	return GetLimiteOpenLevel(GetGameMode());
}


std::vector< EGCGameMode > KGCWorldMapManager::GetCurWorldMapInfo()
{
    return m_mapGameMode[m_eCurrentWorldMap];
}

int KGCWorldMapManager::GetRegionStringNumber(int iRegionNum)
{
    switch(iRegionNum)
    {
    case WORLDMAP_BERMESIA_SERDIN:
        return STR_TEAM_SERDIN;
    case WORLDMAP_BERMESIA_CANAVAN:
        return STR_TEAM_CANABAN;
    case WORLDMAP_ELIA:
        return STR_ID_CONTINENT_ELIA;
    case WORLDMAP_XENEA:
        return STR_ID_CONTINENT_XENIA;
    case WORLDMAP_SILVERLAND:
        return STR_ID_CONTINENT_SILVERLAND;
    case WORLDMAP_ATUM:
        return STR_ID_CONTINENT_ATUM;
    case WORLDMAP_ACHAEMEDIA:
        return STR_ID_CONTINENT_ACHAEMEDIA;
    case WORLDMAP_FLOSLAND:
        return STR_ID_CONTINENT_FLOSLAND;
    case WORLDMAP_DEVILDOM:
        return STR_ID_CONTINENT_DEVILDOM;
#ifdef ELYOS
    case WORLDMAP_ELYOS:
        return STR_ID_ELYOS;
#endif
#ifdef DIMENSIONAL_BREAK
    case WORLDMAP_NEWMAP:
        return STR_ID_DIMENSIONAL_MAP;
#endif
    default:
        break;
    }
    return 0;
}

std::map<int,int> KGCWorldMapManager::GetEnableRegionNumberAndStringNumber()
{

    std::map<int,int> temp;


    std::map<int,KDungeonUserInfo>::iterator mIter  =  g_kGlobalValue.m_kUserInfo.mapStagePlayable.begin();
    
    //  한번이라도 플레이한 던전이 있는 지역을 출력한다.
    //  이런 형태라면 오픈되고 플레이 안한 던전은 가지 못한다.
    //  파티에서 사용하는 기능이니 나중에 고치자.
    //  추가 - 전용던전은 파티플레이를 하면 안되므로 캐릭터 전용던전 체크 루틴 추가
    for(;mIter != g_kGlobalValue.m_kUserInfo.mapStagePlayable.end() ;mIter++)
    {
        int iRegion = 0;
        EGCGameMode emGameMode = static_cast<EGCGameMode>(mIter->first);
        
        if( GetWorldMapOfGameMode(emGameMode,iRegion) )
        {
            // 캐릭터 전용 던전이면,
            if ( SiKGCWorldMapManager()->IsPrivateDungeon( emGameMode ) )
                continue;

            if ( SiKGCWorldMapManager()->IsInfinityDungeon( emGameMode ) )
                continue;

            //  최초 던전이면...
            if ( mIter == g_kGlobalValue.m_kUserInfo.mapStagePlayable.begin() )
            {
                temp.insert(std::pair<int,int>(iRegion,GetRegionStringNumber(iRegion)));
                continue;
            }

            //  한번이라도 해당던전에 플레이 기록이 있다면
            if ( IsPlayedDungeon( emGameMode, -1, g_kGlobalValue.m_kUserInfo.mapStagePlayable ) )
            {
                temp.insert(std::pair<int,int>(iRegion,GetRegionStringNumber(iRegion)));
            }
        }
    }

    return temp;
}

std::map<int,int> KGCWorldMapManager::GetEnableDungeonNumberAndStringNumber(int iRegion_)
{

    std::map<int,int> temp;

    std::map<int,KDungeonUserInfo>::iterator mIter  =  g_kGlobalValue.m_kUserInfo.mapStagePlayable.begin();

    // 한번이라도 플레이한 던전만 갈수 있게 변경했다.
    // 기존 로직은 enum순서로 체크했는데 현재 오픈 할 던전 순서가 enum순으로 되어 있지 않아서 문제가 있다.
    // 그래서 일단 이렇게 수정했다.
    // 추후에 해당 던전을 깼을때 플레이 가능한 다음 던전을 가져올수 있는 구조 만들자.

    for(;mIter != g_kGlobalValue.m_kUserInfo.mapStagePlayable.end() ;mIter++)
    {
        int iRegion = 0;
        EGCGameMode emGameMode = static_cast<EGCGameMode>(mIter->first);

        if( GetWorldMapOfGameMode(emGameMode,iRegion) )
        {
            if(iRegion_ == iRegion)
            {
                //  최초 던전이면...
                if ( mIter == g_kGlobalValue.m_kUserInfo.mapStagePlayable.begin() )
                {
                    int nIndex = GetDungeonStringNumber( emGameMode );
                    temp.insert(std::pair<int,int>(mIter->first,nIndex));
                    continue;
                }

                //  한번이라도 해당던전에 플레이 기록이 있다면
                if ( IsPlayedDungeon( emGameMode, -1, g_kGlobalValue.m_kUserInfo.mapStagePlayable ) )
                {
                    int nIndex = GetDungeonStringNumber( emGameMode );
                    temp.insert(std::pair<int,int>(mIter->first,nIndex));
                }
            }
        }        
    }
    return temp;
}

int KGCWorldMapManager::GetPlayEnableTowerLevel(EGCGameMode eGameMode_, std::map<int, KDungeonUserInfo>& mapStagePlayable_)
{
    int nEnableLevel = 0;

    int nLimitLevel = GetLimiteOpenLevel(static_cast<EGCGameMode>(eGameMode_));

    std::map<int, KDungeonUserInfo>::iterator pos_map = mapStagePlayable_.find(static_cast<int>(eGameMode_));

    if (pos_map == mapStagePlayable_.end())
        return nEnableLevel;

    KDungeonUserInfo& KDungeonUserInfo = pos_map->second;

    nEnableLevel = KDungeonUserInfo.m_kClearData.m_shMaxDifficulty;

    if (nEnableLevel >= nLimitLevel)
    {
        nEnableLevel = nLimitLevel - 1;
    }

    return nEnableLevel;

}


//  플레이 가능한 던전 레벨( 챔피언 모드 제외 버전 )
//  클리어한 이전 단계 난이도까지만 플레이 가능하다.
int KGCWorldMapManager::GetPlayEnableDungeonLevel( EGCGameMode eGameMode_, std::map<int,KDungeonUserInfo>& mapStagePlayable_ )
{
    int nEnableLevel = 0;

    int nLimitLevel = GetLimiteOpenLevel( static_cast<EGCGameMode>(eGameMode_) );

    std::map<int,KDungeonUserInfo>::iterator pos_map = mapStagePlayable_.find( static_cast<int>(eGameMode_) );
    
    if ( pos_map == mapStagePlayable_.end() ) 
        return nEnableLevel;

    KDungeonUserInfo& KDungeonUserInfo = pos_map->second;

    for( int i = 0; i < nLimitLevel; ++i )
    {  
        if ( KDungeonUserInfo.m_kModeDifficulty.Get( i ) ) 
        {
            nEnableLevel = i + 1;
        }
        else
        {
            break;
        }
    }

    //  릴레이 영웅던전 일때
    if ( IsRelayHeroDungeon( eGameMode_ ) )
    {
        if ( nEnableLevel > KDungeonUserInfo.m_kClearData.m_shMaxDifficulty )
        {
            nEnableLevel = KDungeonUserInfo.m_kClearData.m_shMaxDifficulty;
        }
    }

    if ( nEnableLevel >= nLimitLevel ) 
    {
       nEnableLevel = nLimitLevel-1;
    }

    return nEnableLevel;

}

bool KGCWorldMapManager::IsPlayEnableDungeonLevel( EGCGameMode eGameMode_, int nLevel, 
                                                   std::map<int,KDungeonUserInfo>& mapStagePlayable_ )
{
    // 헬모드면~ 
    if ( nLevel == GC_DUNGEON_LEVEL_HELL )
    {
        return IsOpenHellMode( eGameMode_, mapStagePlayable_ );
    }
    else if (nLevel == GC_DUNGEON_LEVEL_BREAK)
    {
        return IsOpenBreakMode(eGameMode_, mapStagePlayable_);
    }
    else
    {
        std::map<int,KDungeonUserInfo>::iterator iter = mapStagePlayable_.find( eGameMode_ );
        if ( iter == mapStagePlayable_.end() ) 
            return false;

        KDungeonUserInfo& kDungeonUserInfo = iter->second;

        // 이전판을 클리어 하거나
        if( kDungeonUserInfo.m_kModeDifficulty.Get( nLevel - 1 ) )
            return true;

        // 현재 스테이지를 클리어 하거나
        return ( kDungeonUserInfo.m_kModeDifficulty.Get( nLevel ) );
    }
}

//  해당 게임모드에 마지막 난이도를 가져온다.
int KGCWorldMapManager::GetLastPlayDungeonLevel( EGCGameMode eGameMode_, std::map<int,KDungeonUserInfo>& mapStagePlayable_ )
{
    std::map<int,KDungeonUserInfo>::iterator pos_map = mapStagePlayable_.find( static_cast<int>(eGameMode_) );

    if ( pos_map == mapStagePlayable_.end() ) 
        return 0;

    KDungeonUserInfo& kDungeonUserInfo = pos_map->second;

    int nLastLevel = kDungeonUserInfo.m_kClearData.m_shLastDifficulty;

    int nEnableLevel = GetPlayEnableDungeonLevel( eGameMode_, mapStagePlayable_ );

    if ( nLastLevel > nEnableLevel ) 
        return nEnableLevel;

    return nLastLevel;
}

int KGCWorldMapManager::GetCorruptionRecomended( EGCGameMode eGameMode_ )
{
    int iCorruptionIndex = g_MyD3D->m_pkQuestGameMgr->GetQuestCorruptionIndex( eGameMode_ );
    if( IsHellMode() )
        iCorruptionIndex *= 2;
    if( iCorruptionIndex == 0 )
        return 0;
    return ( iCorruptionIndex / 0.3 );
}

// 선택된 던전의 마지막 난이도를 가져 온다. <플레이가능한 레벨, 던전의 마지막 레벨>
std::pair<int, int> KGCWorldMapManager::GetCurrentDungeonPlayLevelAndLastLevel( EGCGameMode eGameMode_ )
{
    if (eGameMode_ == GC_GM_INVALID)
        eGameMode_ = GetGameMode();

    int nLastLevel = GetLimiteOpenLevel(eGameMode_);

    int nPlayLevel = GetPlayEnableDungeonLevel(eGameMode_, g_kGlobalValue.m_kUserInfo.mapStagePlayable);

    return std::pair<int, int>(nPlayLevel, nLastLevel);
}

int KGCWorldMapManager::GetDungeonRewardNum( EGCGameMode eGameMode_, std::map<int,KDungeonUserInfo>& mapStagePlayable_ )
{
    std::map<int,KDungeonUserInfo>::iterator pos_map = mapStagePlayable_.find( static_cast<int>(eGameMode_) );

    if ( pos_map == mapStagePlayable_.end() ) 
        return 0;

    KDungeonUserInfo& kDungeonUserInfo = pos_map->second;

    return kDungeonUserInfo.m_kClearData.m_usLeftRewardCount;
}

void KGCWorldMapManager::SetDungeonRewardNum( EGCGameMode eGameMode_, USHORT _usLeftRewardCount )
{
    std::map<int,KDungeonUserInfo>::iterator pos_map = g_kGlobalValue.m_kUserInfo.mapStagePlayable.find( static_cast<int>(eGameMode_) );

    if ( pos_map == g_kGlobalValue.m_kUserInfo.mapStagePlayable.end() ) 
        return;

    KDungeonUserInfo& kDungeonUserInfo = pos_map->second;

    kDungeonUserInfo.m_kClearData.m_usLeftRewardCount = _usLeftRewardCount;

}

//  모든 난이도를 클리어한 던전의 몬스터레벨을 가져온다.
bool KGCWorldMapManager::GetClearMonsterLevel( int& iLevel )
{
    if ( iLevel < 0 ) 
        return false;

    EGCGameMode emGameMode = SiKGCRoomManager()->GetGameMode();     

    if ( !IsRelayHeroDungeon( emGameMode ) )
        return false;

    std::map<int,KDungeonUserInfo>::iterator pos_map = g_kGlobalValue.m_kUserInfo.mapStagePlayable.find( static_cast<int>(emGameMode) );

    if ( pos_map == g_kGlobalValue.m_kUserInfo.mapStagePlayable.end() ) 
        return false;

    KDungeonUserInfo& kDungeonUserInfo = pos_map->second;

    if ( !kDungeonUserInfo.m_kClearData.m_bClear )
        return true;

    iLevel = g_kGlobalValue.m_kHeroDungeonInfo.GetClearMonsterLevel( emGameMode );

    return true;
}

bool KGCWorldMapManager::IsEnableJoinRoomCheck( EGCGameMode eGameMode_, int nLevel, bool bClearLastDifficult )
{
    if ( !IsRelayHeroDungeon(eGameMode_) )
        return true;

    std::map<int,KDungeonUserInfo>::iterator pos_map = g_kGlobalValue.m_kUserInfo.mapStagePlayable.find( static_cast<int>(eGameMode_) );

    if ( pos_map == g_kGlobalValue.m_kUserInfo.mapStagePlayable.end() ) 
        return false;

    KDungeonUserInfo& kDungeonUserInfo = pos_map->second;

    if ( kDungeonUserInfo.m_kClearData.m_bClear != bClearLastDifficult )
        return false;

    if ( kDungeonUserInfo.m_kClearData.m_shMaxDifficulty < nLevel )
        return false;

    return true;
}

bool KGCWorldMapManager::GetWorldMapOfGameMode(EGCGameMode eGameMode_, OUT int& iGetWorldMap_)
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() ) 
        return false;

    if ( pos->second.m_mapWorldMapInfo.empty() ) 
        return false;

	std::map<EGCWorldMap,std::pair<int,int>>::iterator mapInfo = pos->second.m_mapWorldMapInfo.begin();
    
    iGetWorldMap_ = mapInfo->first;
    return true;
}

int KGCWorldMapManager::GetBasicLevelString(int nLevel)
{
    switch (nLevel)
    {
    case 0:
        return STR_ID_DUNGEON_LEVEL_1;
    case 1:
        return STR_ID_DUNGEON_LEVEL_2;
    case 2:
        return STR_ID_DUNGEON_LEVEL_3;
    case 255:
    {
        if (IsEventDungeon() || IsHeroDungeon())
        {
            return STR_ID_DUNGEON_LEVEL_HELL;
        }

        return STR_ID_DUNGEON_LEVEL_HELL;
    }
    case 256:
    {
        if (IsEventDungeon() || IsHeroDungeon())
        {
            return STR_ID_DUNGEON_LEVEL_BREAK;
        }

        return STR_ID_DUNGEON_LEVEL_BREAK;
    }
    break;
    default:
        return STR_ID_DUNGEON_LEVEL_1;
    }
}

int KGCWorldMapManager::GetDungeonLevelString(EGCGameMode eGameMode_, int nLevel)
{
    return GetBasicLevelString(nLevel);
}


bool KGCWorldMapManager::IsSkipStartMotion( EGCGameMode eGameMode_ )
{
	switch( eGameMode_ ) 
	{
	case GC_GM_QUEST53:
		return true;
	default:
		return false;
	}
}

bool KGCWorldMapManager::IsPlayEnableDungeon( EGCGameMode eGameMode_ , int iCharLevel )
{
	QuestFileInfo kQuestFileInfo = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode( eGameMode_, 0 );
	if ( SiKGCRoomManager()->IsAvailableGameMode(GC_GMC_DUNGEON, eGameMode_) == false )
		return false;

	return kQuestFileInfo.iLowLevel <= iCharLevel;

}

bool KGCWorldMapManager::IsHeroDungeonGradeCheck( EGCGameMode eGameMode_, int iChar )
{
    if ( eGameMode_ == GC_GM_INVALID ) 
        eGameMode_ = GetGameMode();

    if ( iChar == -1 )
        iChar = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;

    std::vector<KHeroDungeon>::iterator vit = std::find_if( g_kGlobalValue.m_kHeroDungeonInfo.m_vecDungeonInfo.begin(), 
        g_kGlobalValue.m_kHeroDungeonInfo.m_vecDungeonInfo.end(),
        boost::bind(&KHeroDungeon::m_nModeID,_1) == eGameMode_ );

    if ( vit == g_kGlobalValue.m_kHeroDungeonInfo.m_vecDungeonInfo.end() )
        return false;

    if ( vit->m_nMinUserGrade != -1 && GetChangeUserGradeInfo(iChar) < vit->m_nMinUserGrade )
        return false;

    return true;
}

int KGCWorldMapManager::GetHeroDungeonPlayType(EGCGameMode eGameMode_, int iChar)
{
    if (eGameMode_ == GC_GM_INVALID)
        eGameMode_ = GetGameMode();

    if (iChar == -1)
        iChar = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;

    std::vector<KHeroDungeon>::iterator vit = std::find_if(g_kGlobalValue.m_kHeroDungeonInfo.m_vecDungeonInfo.begin(),
        g_kGlobalValue.m_kHeroDungeonInfo.m_vecDungeonInfo.end(),
        boost::bind(&KHeroDungeon::m_nModeID, _1) == eGameMode_);

    if (vit == g_kGlobalValue.m_kHeroDungeonInfo.m_vecDungeonInfo.end())
        return GRPT_NOT_INFO;

    if (g_kGlobalValue.m_kUserInfo.GetCurrentChar().iLevel < static_cast<int>(vit->m_dwMinCharacterLv) ||
        GetChangeUserGradeInfo(iChar) < vit->m_nMinUserGrade) {
        return GRPT_CONDITIONS_BELOW;
    }

    return GRPT_ADMISSION;
}

int KGCWorldMapManager::GetDungeonMaxPlayerNum( EGCGameMode eGameMode_ )
{
    if (eGameMode_ == GC_GM_INVALID)
        eGameMode_ = GetGameMode();

    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find(eGameMode_);
    if (pos == m_mapDungeonInfo.end())
        return OLD_MAX_DUNGEON_PLAYER_NUM;

    return pos->second.m_nMaxPlayerNum;
}

int KGCWorldMapManager::GetDungeonCharBaseLife( EGCGameMode eGameMode_ )
{
    if (eGameMode_ == GC_GM_INVALID)
        eGameMode_ = GetGameMode();

    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find(eGameMode_);
    if (pos == m_mapDungeonInfo.end())
        return m_iCharBaseLife;

    if (-1 == pos->second.m_iCharLife)
        return m_iCharBaseLife;
    return pos->second.m_iCharLife;
}

int KGCWorldMapManager::GetChangeUserGradeInfo(int iCharType)
{
	PLAYER* pPlayer = NULL;
	pPlayer = &g_MyD3D->m_TempPlayer;

    // 기본능력치 얻기
    EAbilityData stAbilityData;
    pPlayer->GetCharAbility( stAbilityData );

    float fTotalAck = pPlayer->GetCharTotalAtk( stAbilityData );

	// 등급
    return (int)fTotalAck;
}


int KGCWorldMapManager::GetWorldMapBgm( EGCWorldMap eWorldMap )
{
    switch ( eWorldMap ) 
    {
    case WORLDMAP_BERMESIA_SERDIN:      return GC_BGM_CONTINENT_BERMESIAH;
    case WORLDMAP_BERMESIA_CANAVAN:     return GC_BGM_CONTINENT_BERMESIAH;
    case WORLDMAP_ELIA:                 return GC_BGM_CONTINENT_ELIA;
    case WORLDMAP_XENEA:                return GC_BGM_CONTINENT_XENIA;
    case WORLDMAP_UPPERMAP:             return GC_BGM_WORLDMAP;
    case WORLDMAP_SILVERLAND:           return GC_BGM_CONTINENT_SILVERLAND;
    case WORLDMAP_ATUM:                 return GC_BGM_CONTINENT_ATUM;
    case WORLDMAP_ACHAEMEDIA:           return GC_BGM_CONTINENT_ACHAEMEDIA;
    case WORLDMAP_FLOSLAND:             return GC_BGM_CONTINENT_FLOSLAND;
    case WORLDMAP_DEVILDOM:             return GC_BGM_CONTINENT_TRIVIA;
#ifdef ELYOS
    case WORLDMAP_ELYOS:                return GC_BGM_CONTINENT_ELYOS;
#endif
#ifdef DIMENSIONAL_BREAK
    case WORLDMAP_NEWMAP:               return GC_BGM_WORLDMAP_BREAK;
#endif
    default:                            return GC_BGM_WORLDMAP;
    }
    
    return GC_BGM_WORLDMAP;
}

std::string KGCWorldMapManager::GetDungeonLoadingImgName(const EGCGameMode eGamemode_)
{
    MAP_DUNGEON_INFO_ITER mit = m_mapDungeonInfo.find(eGamemode_);
    if (mit == m_mapDungeonInfo.end()) {
        return std::string();
    }

    return mit->second.m_strLoadingImg;
}

std::string KGCWorldMapManager::GetDungeonLoadingBreakImgName(const EGCGameMode eGamemode_)
{
    MAP_DUNGEON_INFO_ITER mit = m_mapDungeonInfo.find(eGamemode_);
    if (mit == m_mapDungeonInfo.end()) {
        return std::string();
    }

    return mit->second.m_strLoadingBreakImg;
}

int KGCWorldMapManager::GetDungeonLoadingTip( EGCGameMode iGameMode )
{
    int iRandom;
    if ( !m_mapDungeonTipList[iGameMode].empty() )
    {
        iRandom = rand()%m_mapDungeonTipList[iGameMode].size();
        return m_mapDungeonTipList[iGameMode][iRandom];
    }
    else
    {
        iRandom = rand()%m_mapDungeonTipList[0].size();
        return m_mapDungeonTipList[0][iRandom];
    }
}

bool KGCWorldMapManager::IsSkillListGuideDungeon( EGCGameMode eGameMode_ )
{
    std::vector<int>::iterator vit;
    for ( vit = m_vecSkillListGuideDungeonList.begin(); vit != m_vecSkillListGuideDungeonList.end(); ++vit )
    {
        if ( *vit == eGameMode_ )
            return true;
    }
    return false;
}

int KGCWorldMapManager::GetCurrentDungeonRank( bool bRoom , bool bBoosInfo) 
{
    EGCGameMode eGameMode;
    if (bRoom) {
        eGameMode = SiKGCRoomManager()->GetGameMode();
    }
    else {
        eGameMode = GetGameMode();
    }

    std::map<int,KDungeonUserInfo>::iterator mIter  =  g_kGlobalValue.m_kUserInfo.mapStagePlayable.find( eGameMode );

    if ( mIter != g_kGlobalValue.m_kUserInfo.mapStagePlayable.end() ) {
        std::map< int, KUsersDungeonRecordData >::iterator mRecordIter = mIter->second.m_kClearData.m_mapBestRecord.find( SiKGCRoomListManager()->GetCurrentLevel() );
        
        if ( mRecordIter != mIter->second.m_kClearData.m_mapBestRecord.end() ) {
            if ( bBoosInfo)
                return mRecordIter->second.m_nBestClearRank;
            else
                return mRecordIter->second.m_nPrevBestClearRank;
        }
    }

    return -1;
}

int KGCWorldMapManager::GetContinentNameIndex( EGCGameMode eGameMode_ )
{
    MAP_DUNGEON_INFO_ITER pos = m_mapDungeonInfo.find( eGameMode_ );
    if ( pos == m_mapDungeonInfo.end() )
        return 0;

    return pos->second.m_nWorldMapName;
}