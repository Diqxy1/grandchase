#include "stdafx.h"
//

#include "QuestGameManager.h"
#include "GCIronDoor.h"
#include "TGPosition.h"

#include "GCUI/GCGameOverlayUI.h"
#include "KMci.h"
#include "DamageManager.h"

#include "Message.h"



#include "Monster.h"

#include "Stage.h"


#include "PetAI.h"
#include "GCStateGame.h"


#include "GCEventManager.h"
#include "buff/KGCBuffManager.h"
//

//
//


#include "EveryTriggerManager.h"
#include "MapSystem/KGCLayerManager.h"
#include "./GCUI/GCQuestTileMap.h"
//
//


#include "TGClearOfStage.h"
#include "TGKillMaster.h"
#include "TGLoading.h"
#include "TGLose.h"
#include "TGMonsterGenerater.h"
#include "TGNextStage.h"

#include "TGReward.h"
#include "TGSoundOnOff.h"
#include "TGSuperPlayer.h"
#include "TGTimer.h"
#include "TGDialogueState.h"
#include "TGTriggerOnOff.h"
#include "TGWin.h"
#include "TGGateObjectGenerater.h"
#include "TGPlayerPosition.h"
#include "TGObjectGenerater.h"
#include "TGOpenDGate.h"
#include "TGTitleText.h"
#include "TGTriggerReady.h"
#include "TGDialogue.h"
#include "TGPositionParticle.h"
#include "TGUnitMovement.h"
#include "TGDeleteMonster.h"
#include "TGFadeInOut.h"
#include "TGHp.h"
#include "TGMp.h"
#include "TGPlayerMotion.h"
#include "TGUnitParticle.h"
#include "TGPlayerMovingToArea.h"
#include "TGChangeObjState.h"
#include "TGObjDestroy.h"
#include "TGWater.h"

#include "TGObjHpChange.h"
#include "TGCheckWall.h"

#include "GCDropItemManager.h"
#include "KGCRoomManager.h"

#include "Drama/KGCDrama.h"
#include "TriggerManager.h"

#include "GCPortal.h"
#include "KGCGameBoard.h"
#include "procedure.h"
#include "KGCWorldMapManager.h"
#include "GCEclipseEventManager.h"
#include "KGCSyncObject.h"
#include "KGCRoomListManager.h"
#include "KGCChannelManager.h"
#include "TGEmbark.h"
#include "TGWall.h"
#include "TGSubject.h"
#include "TGInfinityMonsterGen.h"
#include "TGKillAllMonster.h"
#include "KGCInfinityDungeonManager.h"

#include "GCPetMagicStone.h"

KQuestGameManager* g_pkQuestManager = NULL;

void KQuestGameManager::RegisterLuabind()
{

}

KQuestGameManager::KQuestGameManager()
{
	m_bSkipping = false;
	g_pkQuestManager    = this;

	m_vecArena.x = 0.0f; //zstaiji - vector 초기화
	m_vecArena.y = 0.0f;
    
    m_bQuestStarted              = false;
    m_bWin                       = false;
    m_bEndGame                   = false;
    m_bClearStage                = false;
    m_bLoadStageByPlayers        = false;
    m_bLoading                   = false;
    m_bMovement                  = true;
	m_bShowStartMsg				 = false;
    m_bPerfectMode               = false;

	//던전해킹방지작업
	m_bIsHackedDungeon			 = false;
	m_bIsMonsterHPHack			 = false;
	m_bIsAttackedMon	         = false;
	m_bMonLoadComplete			 = false;
	m_bBossStage				 = false;
	m_iMonsterNum				 = 0;
	m_iCurMonsterNum			 = 0;
	m_vecCurAtk.clear();
	m_vecIncAtkRatio.clear();

	m_dwPlayFrame				 = 0;

    m_iCurTileX                  = 0;
    m_iCurTileY                  = 0;
    m_iCurDialoge                = 0;
    m_iBeforeDirection           = 0;
    m_iCurStage                  = -1;
    m_iBossStageNum              = -1;
    m_dwEndGameDelay             = END_GAME_DELAY_TIME;

    m_iComboBreakCount           = 0;

    m_iMaxStage                  = 0;
    m_iAllLevelMaxStage          = 0;

    if( FAILED(LoadQuestSignScript("QuestSign.stg")) )
    {
        assert(!"QuestSign.stg 파일을 찾을 수 없습니다.");
    }

    if( FAILED(LoadDialogueTable()) )
    {
        assert(!"Dialogue 정보 로드에 실패하였습니다.");
    }

	m_pkDrama = NULL;
    m_pkDrama = new KGCDrama();

    m_iPlayersAveLv = 0;
    m_ucPlayerNum = 0;
    m_iHeroDungeonLevel = 0;
    m_setGpUID.clear();
    m_bRankPortalCheck = false;
    m_bComboBreak = false;
    m_iState = -1;

    m_vecDungeonPersonalRecord.clear();

    for ( int i =0; i < MAX_PLAYER_NUM; ++i ) 
    {
        m_PlayerRankPoint[i] = 0;
        m_PlayerDamage[i] = 0.f;

        std::map< int, KDungeonPersonalRecord > mapDungeonPersonalRecord;
        m_vecDungeonPersonalRecord.push_back( mapDungeonPersonalRecord );
    }
    m_iQuestStartUserCnt= 0;
    m_bFirstStartUserRecord = true;

    
}

KQuestGameManager::~KQuestGameManager( void )
{
    m_Quest.DeleteItem();

    SAFE_DELETE( m_pkDrama );
}

HRESULT KQuestGameManager::LoadQuestScript_New(std::string strFileName)
{
    // read script file
    KLuaManager luaMgr;
    if( GCFUNC::LoadLuaScript( luaMgr, strFileName ) == false )
    {
        assert(0);
        return false; // file not exist
    }

    m_Quest.DeleteItem();
    LoadQuestFromLua(luaMgr, &m_Quest);

    m_iCurStage         = 0;
    m_iCurTileX         = 0;
    m_iCurTileY         = 0;

    return S_OK;
}

HRESULT KQuestGameManager::LoadDungeon_ExpGp(int stage,int difficult)
{	
	 KLuaManager luaMgr;
	if( GCFUNC::LoadLuaScript( luaMgr, "Dungeon_ExpGp.lua" ) == false )
	{
		assert(0);
		return false; // file not exist
	}
	LUA_BEGIN_TABLE("Dungeon_Exp_Gp", return E_FAIL)
	{
		for( int i = 1; ; ++i )
		{
			LUA_BEGIN_TABLE(i, break)
			{
				int DungeonID = 0;				
				LUA_GET_VALUE("DungeonID", DungeonID, break);
				if(DungeonID==stage)
				{
					LUA_BEGIN_TABLE("Difficult", break)
					{
						for( int j = 1; ; ++j )
						{	
							LUA_BEGIN_TABLE(j, break)
							{
								int Difficult = 0;
								LUA_GET_VALUE("Difficult", Difficult, break);
								if(Difficult==difficult)
								{
									LUA_GET_VALUE("MaxStage", m_iMaxStage, break);
									break;
								}
							}
							LUA_END_TABLE(break);

						}
					}
					LUA_END_TABLE(break)
				}
			}
			LUA_END_TABLE(break);
		}
	}
	LUA_END_TABLE(return E_FAIL)

	return S_OK;
}


HRESULT KQuestGameManager::LoadQuestFromLua(KLuaManager &luaMgr, Quest *quest)
{
    LUA_BEGIN_TABLE("quest", return E_FAIL)
    {
        int xSize = 0;
        int ySize = 0;
        if( SUCCEEDED(luaMgr.BeginTable( "questtilemap" )) )
        {
            LUA_GET_VALUE("X_Size", quest->iTileX, return E_FAIL);
            LUA_GET_VALUE("Y_Size", quest->iTileY, return E_FAIL);

            for(int i=1;;++i)
            {
                char szRow[MAX_PATH];
                sprintf(szRow, "row%d", i-1);
                LUA_BEGIN_TABLE(szRow, break)
                for(int j=1;;++j)
                {
                    char szCol[MAX_PATH];
                    sprintf(szCol, "col%d", j-1);
                    LUA_BEGIN_TABLE(szCol, break)
                    {
                        TileMap tileMap;
                        tileMap.iCol = j-1;
                        tileMap.iRow = i-1;
                        LUA_GET_VALUE("Name", tileMap.strName, break);
                        LUA_GET_VALUE("StageNum", tileMap.iStage, break);
                        LUA_GET_VALUE("D_Path", tileMap.iD_Path, break);
                        LUA_GET_VALUE("Flag", tileMap.iFlag, break);
                        quest->SetTileMap(tileMap);
                    }
                    LUA_END_TABLE(break)
                }
                LUA_END_TABLE(break)
            }
            LUA_END_TABLE(return E_FAIL)
        }

        m_iAllLevelMaxStage = 0;

        for(int i=0;;++i)
        {
            Stage stage;
            std::string strStage;
            char szStage[MAX_PATH];
            sprintf(szStage, "stage%d", i);
            strStage = szStage;
            CTriggerManager *pTriggerManager = new CTriggerManager;
            
            if( SUCCEEDED(LoadStageFromLua(luaMgr, strStage, &stage, &pTriggerManager)) )
            {
                quest->vtStage.push_back(stage);
                g_kEveryTriggerManager.AddTriggerMgr(pTriggerManager);
                m_iAllLevelMaxStage++;
            }
            else
            {
                SAFE_DELETE(pTriggerManager);
                break;
            }
        }
    }
    LUA_END_TABLE(return E_FAIL)

    return E_FAIL;
}

HRESULT KQuestGameManager::LoadStageFromLua(KLuaManager &luaMgr, std::string strStage, Stage *stage, CTriggerManager **pTriggerManager)
{
    LUA_BEGIN_TABLE(strStage.c_str(), return E_FAIL)
    {
        LUA_GET_VALUE("StageMap", stage->strStageMap, return E_FAIL);

        for(int i=1;;i++)
        {
			char szCategory[MAX_PATH] = {0,};
            sprintf(szCategory, "Category%d", i-1);
            LUA_BEGIN_TABLE(szCategory, break)
            {
                LoadFromFile_Trigger(luaMgr, pTriggerManager);
            }
            LUA_END_TABLE(break)
        }
    }
    LUA_END_TABLE(return E_FAIL)

    return S_OK;
}

HRESULT KQuestGameManager::LoadFromFile_Trigger(KLuaManager &luaMgr, CTriggerManager **pTriggerManager)
{
    for(int i=1;;i++)
    {
        char szTrigger[MAX_PATH];
        sprintf(szTrigger, "Trigger%d", i-1);
        LUA_BEGIN_TABLE(szTrigger, break)
        {
            CTrigger *pBaseTrigger = new CTrigger;
            LUA_GET_VALUE("TriggerName", pBaseTrigger->m_strTGName, break);
            LUA_GET_VALUE("Active", pBaseTrigger->m_bStartWithActive, break);
            LUA_GET_VALUE("Use", pBaseTrigger->m_bUse, break);
            LUA_BEGIN_TABLE("Event", break)
            {
                LoadFromFile_EAction(luaMgr, pBaseTrigger->m_vtEvent);
            }
            LUA_END_TABLE(break)

            LUA_BEGIN_TABLE("Condition", break)
            {
                LoadFromFile_Condition(luaMgr, pBaseTrigger->m_vtCondition);
            }
            LUA_END_TABLE(break)

            LUA_BEGIN_TABLE("Action", break)
            {
                LoadFromFile_EAction(luaMgr, pBaseTrigger->m_vtAction);
            }
            LUA_END_TABLE(break)
            if( !pBaseTrigger->m_bUse ) SAFE_DELETE(pBaseTrigger);
            if( pBaseTrigger ) (*pTriggerManager)->AddTrigger(pBaseTrigger);
        }
        LUA_END_TABLE(break)
    }

    return S_OK;
}

HRESULT KQuestGameManager::LoadFromFile_EAction(KLuaManager &luaMgr, vector<CTriggerCondition*> &pTrigger)
{
    for(int i=1;;i++)
    {
        char szFunction[MAX_PATH];
        sprintf(szFunction, "Function%d", i-1);
        LUA_BEGIN_TABLE(szFunction, break)
        {                
            std::string strName, strDesc;
            int iType, iTG_ID;
            bool bRepeat;
            LUA_GET_VALUE("FuctionName", strName, break);
            LUA_GET_VALUE("FuctionType", iType, break);
            LUA_GET_VALUE("FuctionDesc", strDesc, break);
            luaMgr.GetValue( "Repeat", bRepeat );
            if( FAILED(luaMgr.GetValue( "TG_ID", iTG_ID )) )
            {
                iTG_ID = g_iTG_ID;
            }
            g_iTG_ID++;
            SetMaxQuestID( iTG_ID );
            CTriggerCondition *pBaseCondition = NULL;
            switch( (TG_EACTION)iType )
            {
                case TG_EACTION_WIN:
                    {
                        pBaseCondition = new CTGWin;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_LOSE:
                    {
                        pBaseCondition = new CTGLose;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_MON_GENNER:
                    {
                        pBaseCondition = new CTGMonsterGenerater;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_SOUND_ONOFF:
                    {
                        pBaseCondition = new CTGSoundOnOff;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_LOADING:
                    {
                        pBaseCondition = new CTGLoading;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_CLEAROFSTAGE:
                    {
                        pBaseCondition = new CTGClearOfStage;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_TRIGGERONOFF:
                    {
                        pBaseCondition = new CTGTriggerOnOff;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_REWARD:
                    {
                        pBaseCondition = new CTGReward;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_NEXTSTAGE:
                    {
                        pBaseCondition = new CTGNextStage;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_SUPERPLAYER:
                    {
                        pBaseCondition = new CTGSuperPlayer;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_PLAYERPOSITION:
                    {
                        pBaseCondition = new CTGPlayerPosition;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_OBJECTGENERATER:
                    {
                        pBaseCondition = new CTGObjectGenerater;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_OPEN_DGATE:
                    {
                        pBaseCondition = new CTGOpenDGate;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_TITLETEXT:
                    {
                        pBaseCondition = new CTGTitleText;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_TRIGGERREADY:
                    {
                        pBaseCondition = new CTGTriggerReady;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_DIALOGUE:
                    {
                        pBaseCondition = new CTGDialogue;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_POSITIONPARTICLE:
                    {
                        pBaseCondition = new CTGPositionParticle;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_UNITMOVEMENT:
                    {
                        pBaseCondition = new CTGUnitMovement;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_DELETEMONSTER:
                    {
                        pBaseCondition = new CTGDeleteMonster;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_FADE_INOUT:
                    {
                        pBaseCondition = new CTGFadeInOut;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_HP:
                    {
                        pBaseCondition = new CTGHp;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_MP:
                    {
                        pBaseCondition = new CTGMp;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_PLAYERMOTION:
                    {
                        pBaseCondition = new CTGPlayerMotion;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_UNIT_PARTICLE:
                    {
                        pBaseCondition = new CTGUnitParticle;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_PLAYERMOVINGTOAREA:
                    {
                        pBaseCondition = new CTGPlayerMovingToArea;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_CHANGE_OBJ_STATE:
                    {
                        pBaseCondition = new CTGChangeObjState;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_GATE_OBJECT_GEN:
                    {
                        pBaseCondition = new CTGGateObjectGenerater;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_WATER:
                    {
                        pBaseCondition = new CTGWater;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_ADD_DAMAGE:
                    {
                        pBaseCondition = new CTGAddDamage;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_WEATHER:
                    {
                        pBaseCondition = new CTGWeather;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_EMBARK:
                    {
                        pBaseCondition = new CTGEmbark;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_WALL:
                    {
                        pBaseCondition = new CTGWall;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_SUBJECT:
                    {
                        pBaseCondition = new CTGSubject;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
                    break;
                case TG_EACTION_INFINITY_MON_GEN:
                    { 
                        pBaseCondition = new CTGInfinityMonsterGen;
                        pBaseCondition->LoadItemFromLua(luaMgr);
                    }
            }
            if( pBaseCondition )
            {
                pBaseCondition->m_bRepeat   = bRepeat;
                pBaseCondition->m_iTG_ID    = iTG_ID;
                pTrigger.push_back(pBaseCondition);
            }
        }
        LUA_END_TABLE(return E_FAIL)
    }

    return S_OK;
}

HRESULT KQuestGameManager::LoadFromFile_Condition(KLuaManager &luaMgr, vector<CTriggerCondition*> &pTrigger)
{
    for(int i=1;;i++)
    {
        char szFunction[MAX_PATH];
        sprintf(szFunction, "Function%d", i-1);
        LUA_BEGIN_TABLE(szFunction, break)
        {                
            std::string strName, strDesc;
            int iType, iTG_ID;
            bool bRepeat;
            LUA_GET_VALUE("FuctionName", strName, break);
            LUA_GET_VALUE("FuctionType", iType, break);
            LUA_GET_VALUE("FuctionDesc", strDesc, break);
            luaMgr.GetValue( "Repeat", bRepeat );
            if( FAILED(luaMgr.GetValue( "TG_ID", iTG_ID )) )
            {
                iTG_ID = g_iTG_ID;
            }
            g_iTG_ID++;
            SetMaxQuestID( iTG_ID );
            CTriggerCondition *pBaseCondition = NULL;
            switch( (TG_CONDITION)iType )
            {
            case TG_CONDITION_KILLMASTER:
                {
                    pBaseCondition = new CTGKillMaster;
                    pBaseCondition->LoadItemFromLua(luaMgr);
                }
                break;
            case TG_CONDITION_TIMER:
                {
                    pBaseCondition = new CTGTimer;
                    pBaseCondition->LoadItemFromLua(luaMgr);
                }
                break;
            case TG_CONDITION_POSITION:
                {
                    pBaseCondition = new CTGPosition;
                    pBaseCondition->LoadItemFromLua(luaMgr);
                }
                break;
            case TG_CONDITION_OBJ_DESTROY:
                {
                    pBaseCondition = new CTGObjDestroy;
                    pBaseCondition->LoadItemFromLua(luaMgr);
                }
                break;
            case TG_CONDITION_DIALOGUE_STATE:
                {
                    pBaseCondition = new CTGDialogueState;
                    pBaseCondition->LoadItemFromLua(luaMgr);
                }
                break;
			case TG_CONDITION_HP_CHANGE:
				{
					pBaseCondition = new CTGObjHpChange;
					pBaseCondition->LoadItemFromLua(luaMgr);
				}
				break;
            case TG_CONDITION_WALL:
                {
                    pBaseCondition = new CTGCheckWall;
                    pBaseCondition->LoadItemFromLua(luaMgr);
                }
                break;
            case TG_CONDITION_KILLALLMONSTER:
                {
                    pBaseCondition = new CTGKillAllMonster;
                    pBaseCondition->LoadItemFromLua(luaMgr);
                }
                break;
            }

            if( pBaseCondition )
            {
                pBaseCondition->m_bRepeat = bRepeat;
                pBaseCondition->m_iTG_ID    = iTG_ID;
                pTrigger.push_back(pBaseCondition);
            }
        }
        LUA_END_TABLE(return E_FAIL)
    }

    return S_OK;
}

std::string KQuestGameManager::CreateQuestFileName( QuestFileInfo &questFileInfo )
{
    std::string strQuestFileName = questFileInfo.strTGFileName;
    switch( SiKGCRoomManager()->GetGameMode() )
    {
    case GC_GM_QUEST49:
    case GC_GM_QUEST53:
        {
            int iHostPlayer;
            for(iHostPlayer = 0; iHostPlayer < MAX_PLAYER_NUM; ++iHostPlayer)
            {
                if( g_MyD3D->MyPlayer[iHostPlayer]->m_kUserInfo.bHost )
                    break;
            }
            
            if( iHostPlayer < MAX_PLAYER_NUM )
            {
                if( 0 == (g_MyD3D->MyPlayer[iHostPlayer]->m_kUserInfo.dwPremium & KPremiumInfo::TYPE_GOLDSTAGE) )
                    strQuestFileName = questFileInfo.strExTGFileName;
            }
        }
        break;
    }
    return strQuestFileName;
}

void KQuestGameManager::EnterQuest(void)
{
	g_dwLoadingTimeCheck = timeGetTime();
    m_mapStageHistory.clear();
    g_kEveryTriggerManager.SetNotMatchTrigger( false );
    m_bWin                     = false;
    m_bEndGame                 = false;
    m_bLoadStageByPlayers      = false;
    m_bClearStage              = false;
    m_bGetEndGameInfo          = false;
    m_bSendGetEndGameInfo      = false;
    m_bRouletteList            = false;
    m_bSendRouletteList        = false;
    m_iCurStage                = 0;
    m_iCurTileX                = 0;
    m_iCurTileY                = 0;
    m_iBeforeDirection         = 0;
	m_bShowStartMsg			   = false;
    m_bPerfectMode             = false;
	m_dwPlayFrame			   = 0;
	

    g_kEveryTriggerManager.Init();
    g_kEveryTriggerManager.ClearEveryTrigger();
    UINT iPacketPacker;
    g_kTGPacketPacker.GetData(iPacketPacker);       // TCP/IP 릴레이서버용 패킷패커의 버퍼를 비운다.
    InitTG_ID();
    g_sTG_MaxQuestID = 0;

    QuestFileInfo questFileInfo = GetQuestFileInfo(SiKGCRoomManager()->GetRoomInfo().ucStage, SiKGCRoomManager()->GetDungeonLevel());
    std::string strQuestFileName = CreateQuestFileName( questFileInfo );
    LoadQuestScript_New( strQuestFileName );

	// Total Stage Setting
	LoadDungeon_ExpGp(SiKGCRoomManager()->GetGameMode(),SiKGCRoomManager()->GetDungeonLevel());

    int iCount = 0;
    vector<TileMap> vtStartStage;
    for(int i=0;i<(int)m_Quest.iTileY;++i)
    {
        for(int j=0;j<(int)m_Quest.iTileX;++j)
        {
            TileMap *pTileMap = m_Quest.GetTileMap(i, j);
            if( 1 == pTileMap->iFlag )      // 시작 부분을 찾는다.
            {
                if( IsExistenceStage(pTileMap->iStage) )
                {
                    vtStartStage.push_back(*pTileMap);
                }
            }

            if( -1 != pTileMap->iStage )
            {
                // 보스 스테이지
                if ( pTileMap->iFlag == 2 )
                    m_iBossStageNum = pTileMap->iStage;

                // 스테이지 기록을 모두 초기화 한다.
                m_mapStageHistory[ std::make_pair(pTileMap->iRow, pTileMap->iCol) ] = false;

                // 서버에서 주는 이벤트 몬스터!! 여기서 자신에 맞는 스테이지를 셋팅한다!
                std::vector<KEventMonster>::iterator vit = g_kGlobalValue.m_vecEventMonster.begin();
                while ( vit != g_kGlobalValue.m_vecEventMonster.end() )
                {
                    // 이미 스테이지가 정해졌다!
                    if ( (*vit).m_nStageNum != -1 ) { ++vit; continue; }

                    switch ( (*vit).m_nSpecial )
                    {
                    case KEventMonster::OPT_GATE:
                        {
                            if ( iCount == (*vit).m_nStage )
                            {
                                // 플랫포머, 히든, 보스 스테이지에서는 안나온다! 여기면 살짝 다른 맵으로 넘기쟈!
                                if ( pTileMap->iFlag == 4 || pTileMap->iFlag == 3 || pTileMap->iFlag == 2 )
                                {
                                    (*vit).m_nStage = iCount + 1;
                                    break;
                                }
                                else
                                    (*vit).m_nStageNum = pTileMap->iStage;
                            }
                        }
                        break;
                    case KEventMonster::OPT_NONE:
                    case KEventMonster::OPT_LUPIN:
                        {
                            // 플랫포머면 안나와!!! ㅋㅋㅋ 이 스테이지에서 나오쟈!
                            if ( pTileMap->iFlag != 4 && pTileMap->iFlag != 2 && iCount == (*vit).m_nStage )
                            {
                                (*vit).m_nStageNum = pTileMap->iStage;
                            }

                            // 보스면 보스 스테이지에서 나와야함! 
                            if ( (*vit).m_bIsBoss && pTileMap->iFlag == 2 )
                            {
                                (*vit).m_nStageNum = pTileMap->iStage;
                            }
                        }
                        break;
                    default:
                        break;
                    }

                    ++vit;
                }

                iCount++;
            }
        }
    }

    if( vtStartStage.size() )
    {
        int iRandom = 0;
        if( vtStartStage.size() > 0 ) 
            iRandom = ::rand()%(int)vtStartStage.size();

        g_kEveryTriggerManager.SetNextStage(vtStartStage[iRandom], true, 0);
    }
    else
    {
        ASSERT(false && "퀘스트 시작지점이 없습니다.");
        m_iCurTileX = 0, m_iCurTileY = 0;
        TileMap *titlemap = m_Quest.GetTileMap(m_iCurTileY, m_iCurTileX);
        g_kEveryTriggerManager.SetNextStage(*titlemap, true, 0);
    }

    SQuestPoint sQuestPointBlank;
    for(int i=0;i<MAX_PLAYER_NUM;++i)
    {
        m_PlayerRankPoint[i] = 0;
        m_PlayerDamage[i] = 0.f;
        m_PlayerQuestPoint[i].Init();
		m_alQuestPoint[i].clear();
		for(int j=0;j<g_kEveryTriggerManager.GetTotalStageCnt();++j)
			m_alQuestPoint[i].insert( std::map<int,SQuestPoint>::value_type( j , sQuestPointBlank ) );

        m_vecDungeonPersonalRecord[i].clear();
    }

    m_mapRouletteList.clear();
    m_vecEndGameInfo.clear();
    m_vecRouletteWaitList.clear();
    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if ( g_MyD3D->Get_MyPlayer() != i && g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == true )
        {
            m_vecRouletteWaitList.push_back( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );
            m_vecEndGameInfo.push_back( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );
        }
    }

   CalPlayerInfo();

    //g_pkGameOverlayUI->InitMinimap();

    g_kGateStage->Init();               // 문 상태 보내는 구조체를 초기화
    m_dwEndGameDelay             = END_GAME_DELAY_TIME;
    m_dwRouletteWaitDelay        = 0;
    m_dwEndGameInfoDelayTime     = 0;

	g_MyD3D->Story = 0;
	
	std::vector<float> vecTemp;
	vecTemp.clear();
	m_vecCurAtk.clear();
	m_vecIncAtkRatio.clear();
	for( int i = 0; i < MAX_PLAYER_NUM; i++ )
	{
		m_vecCurAtk.push_back(vecTemp);
		m_vecIncAtkRatio.push_back(vecTemp);
	}

    // 싱크오브젝트 초기화
    SiKGCSyncObjectManager()->InitCalss();
}

Stage *KQuestGameManager::GetCurStage(void)
{
    if( IsExistenceStage(m_iCurStage) )
        return &m_Quest.vtStage[m_iCurStage];

    return NULL;
}

void KQuestGameManager::SetCurStage(int iStage)
{
    if( IsExistenceStage(iStage) )
    {
        m_iCurStage = iStage;
    }
}

void KQuestGameManager::OnEndGame( void )
{
    m_bEndGame                = false;
    m_bLoadStageByPlayers     = false;
    m_bClearStage             = false;
    m_bLoading                = false;
    SendNetworkData(GC_PID_TG_DEFAULT, 0, 0);     // 퀘스트가 끝나기 전에 남은 데이터를 전송한다.

    SetEnableMovement(true);

    m_setGpUID.clear();

    ClearEndDialogList();
    SiKGCFantasticMap()->ClearWater();
    g_pkGameOverlayUI->HideAllQuestTitle();
    g_pkGameOverlayUI->EndQuestResult();
    g_KMci.Stop();
    InitAllPlayerComboState();    

    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        if( FORM_NORMAL != g_MyD3D->MyPlayer[i]->MetamorphosisForm && true == g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
        {
            g_MyD3D->MyPlayer[i]->ProcessMetamorphosisNormalForm();
        }
    }

	m_pkDrama->End();
}

QuestSign KQuestGameManager::GetQuestSign(int iStage)
{
    if( iStage >= 0 && iStage < (int)m_vtQuestSign.size() )
        return m_vtQuestSign[iStage];

    QuestSign blank;
    return blank;
}

QuestFileInfo KQuestGameManager::GetQuestFileInfo(int iStage, int iLevel)
{
    if( iLevel < 0 ) iLevel = 0;

    if( iStage >= 0 && iStage < (int)m_vtQuestSign.size() )
        return m_vtQuestSign[iStage].GetQuestDifficultInfo(iLevel);
    
    QuestFileInfo blank;
    return blank;
}

HRESULT KQuestGameManager::LoadQuestSignScript( std::string strFileName )
{
    m_vtQuestSign.clear();
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "Enum.stg" ) == false )
        return E_FAIL;

    if( GCFUNC::LoadLuaScript( luaMgr, "etc.stg" ) == false )
        return E_FAIL;

    if( GCFUNC::LoadLuaScript( luaMgr, strFileName.c_str() ) == false )
        return E_FAIL;

    LUA_BEGIN_TABLE( "quest", return E_FAIL )
    {
        for(int i=1;;i++)
        {
            QuestSign questsign;
            char szQuestSign[128];
            sprintf(szQuestSign, "QuestSign%d", i-1);
            LUA_BEGIN_TABLE( szQuestSign, break )
            {
                int iDefNameStrID               = 0;
                int iDefDescStrID               = 0;
                int iDefLowLevel                = 0;
                int iDefHighLevel               = 0;
                std::string strdefTGFileName    = "";
                std::string strdefExTGFileName  = "";
                bool bReviseCharLevel           = false;
                bool bIsCorrupted               = false;
                int iCorruptionIndex            = 0;

                LUA_GET_VALUE( "ID",                questsign.iQuestID, break );
                LUA_GET_VALUE( "Name",              iDefNameStrID, break );
                LUA_GET_VALUE( "Desc",              iDefDescStrID, break );
                LUA_GET_VALUE_DEF( "LOW_LEVEL",     iDefLowLevel, 0 );
                LUA_GET_VALUE_DEF( "HIGH_LEVEL",    iDefHighLevel, MAX_LEVEL );
                LUA_GET_VALUE( "TGFileName",        strdefTGFileName, break );
                LUA_GET_VALUE_DEF( "ExTGFileName",  strdefExTGFileName, "" );
                LUA_GET_VALUE_DEF( "ReviseCharLevel",bReviseCharLevel, false );
                LUA_GET_VALUE_DEF( "IsCorrupted",    bIsCorrupted, false );
                LUA_GET_VALUE_DEF( "CorruptionIndex", iCorruptionIndex, 0 );
                
                if( iDefLowLevel > iDefHighLevel ) swap(iDefLowLevel, iDefHighLevel);

                LUA_BEGIN_TABLE( "LevelInfo", return E_FAIL )
                {
                    for(int j=1;;j++)
                    {
                    QuestFileInfo questfileinfo;
                    questfileinfo.iNameStringID     = iDefNameStrID;
                    questfileinfo.iDescStringID     = iDefDescStrID;
                    questfileinfo.iLowLevel         = iDefLowLevel;
                    questfileinfo.iHighLevel        = iDefHighLevel;
                    questfileinfo.strTGFileName     = strdefTGFileName;
                    questfileinfo.strExTGFileName   = strdefExTGFileName;
                    questfileinfo.bReviseCharLevel  = bReviseCharLevel;
                    questfileinfo.bIsCorrupted = bIsCorrupted;
                    questfileinfo.iCorruptionIndex = iCorruptionIndex;
                    //questsign.SetQuestDifficultInfo( questfileinfo, j );

                        LUA_BEGIN_TABLE( j,break )
                    {
                            int nLevel = 0;
                            LUA_GET_VALUE_DEF( "Level",         nLevel, 0 );
                        LUA_GET_VALUE_DEF( "Name",          questfileinfo.iNameStringID, iDefNameStrID );
                        LUA_GET_VALUE_DEF( "Desc",          questfileinfo.iDescStringID, iDefDescStrID );
                        LUA_GET_VALUE_DEF( "LOW_LEVEL",     questfileinfo.iLowLevel, iDefLowLevel);
                        LUA_GET_VALUE_DEF( "HIGH_LEVEL",    questfileinfo.iHighLevel, iDefHighLevel );
                        LUA_GET_VALUE_DEF( "RANK_DECREASE_NUM_OF_HITTED_BY_MONSTER", questfileinfo.fHittedByMonNum, 0);
                        LUA_GET_VALUE_DEF( "RANK_DECREASE_NUM",    questfileinfo.fDungeonDecreaseNum, 0 );
                        LUA_GET_VALUE_DEF( "RANK_TOTAL_SCORE",     questfileinfo.fRankTotalScore, 0);
                        LUA_GET_VALUE_DEF( "TGFileName",    questfileinfo.strTGFileName, strdefTGFileName );
                        LUA_GET_VALUE_DEF( "ExTGFileName",  questfileinfo.strExTGFileName, strdefExTGFileName );
                        LUA_GET_VALUE_DEF( "ReviseCharLevel",questfileinfo.bReviseCharLevel, bReviseCharLevel );
                        LUA_GET_VALUE_DEF( "IsCorrupted", questfileinfo.bIsCorrupted, bIsCorrupted );
                        LUA_GET_VALUE_DEF( "CorruptionIndex", questfileinfo.iCorruptionIndex, iCorruptionIndex );

						questfileinfo.bLoading = true;
                        if( questfileinfo.iLowLevel > questfileinfo.iHighLevel ) swap(questfileinfo.iLowLevel, questfileinfo.iHighLevel);
                            questsign.SetQuestDifficultInfo( questfileinfo, nLevel );
                    }
                    LUA_END_TABLE( return E_FAIL )
                }
            }
            LUA_END_TABLE( return E_FAIL )
            }
            LUA_END_TABLE( return E_FAIL )
            m_vtQuestSign.push_back(questsign);
        }
    }
    LUA_END_TABLE( return E_FAIL )

    return S_OK;
}

std::wstring KQuestGameManager::GetCurQuestName(void)
{
    if( (int)m_vtQuestSign.size() > SiKGCRoomManager()->GetRoomInfo().ucStage )
        return g_pkStrLoader->GetString(m_vtQuestSign[SiKGCRoomManager()->GetRoomInfo().ucStage].GetQuestDifficultInfo( SiKGCRoomManager()->GetDungeonLevel() ).iNameStringID);

    return L"";
}

std::wstring KQuestGameManager::GetCurQuestDesc(void)
{
    if( (int)m_vtQuestSign.size() > SiKGCRoomManager()->GetRoomInfo().ucStage )
        return g_pkStrLoader->GetString(m_vtQuestSign[SiKGCRoomManager()->GetRoomInfo().ucStage].GetQuestDifficultInfo( SiKGCRoomManager()->GetDungeonLevel() ).iDescStringID);

    return L"";
}

std::string KQuestGameManager::GetCurQuestTrigger(void)
{
    if( (int)m_vtQuestSign.size() > SiKGCRoomManager()->GetRoomInfo().ucStage )
    {
        std::string strQuestFileName = CreateQuestFileName( m_vtQuestSign[SiKGCRoomManager()->GetRoomInfo().ucStage].GetQuestDifficultInfo( SiKGCRoomManager()->GetDungeonLevel() ) );
        return strQuestFileName;
    }
     return "";
}

bool KQuestGameManager::GetReviseCharLevel(void)
{
    if( (int)m_vtQuestSign.size() > SiKGCRoomManager()->GetRoomInfo().ucStage )
    {
        return m_vtQuestSign[ SiKGCRoomManager()->GetRoomInfo().ucStage ].GetQuestDifficultInfo( SiKGCRoomManager()->GetDungeonLevel() ).bReviseCharLevel;
    }
    return false;
}

bool KQuestGameManager::GetIsCorrupted(void)
{
    if ((int)m_vtQuestSign.size() > SiKGCRoomManager()->GetRoomInfo().ucStage)
    {
        return m_vtQuestSign[SiKGCRoomManager()->GetRoomInfo().ucStage].GetQuestDifficultInfo(SiKGCRoomManager()->GetDungeonLevel()).bIsCorrupted;
    }
    return false;
}

int KQuestGameManager::GetCorruptedIndex(void)
{
    if ((int)m_vtQuestSign.size() > SiKGCRoomManager()->GetRoomInfo().ucStage)
    {
        return m_vtQuestSign[SiKGCRoomManager()->GetRoomInfo().ucStage].GetQuestDifficultInfo(SiKGCRoomManager()->GetDungeonLevel()).iCorruptionIndex;
    }
    return 0;
}

int KQuestGameManager::GetQuestIndexFromGameMode( EGCGameMode eGameMode )
{
    for( int i = 0; i < (int)m_vtQuestSign.size(); ++i )
    {
        if( (EGCGameMode)m_vtQuestSign[i].iQuestID == eGameMode )
            return i;
    }
    return 0;
}

QuestSign KQuestGameManager::GetQuestSignFromGameMode( EGCGameMode eGameMode )
{
    for( int i = 0; i < (int)m_vtQuestSign.size(); ++i )
    {
        if( (EGCGameMode)m_vtQuestSign[i].iQuestID == eGameMode )
            return m_vtQuestSign[i];
    }

    QuestSign blank;
    return blank;
}

QuestFileInfo KQuestGameManager::GetQuestFileInfoFromGameMode( EGCGameMode eGameMode, int iLevel )
{
    if( iLevel < 0 ) iLevel = 0;

    for( int i = 0; i < (int)m_vtQuestSign.size(); ++i )
    {
        if( (EGCGameMode)m_vtQuestSign[i].iQuestID == eGameMode && m_vtQuestSign[i].GetQuestDifficultAllow( iLevel ) )
			return m_vtQuestSign[i].GetQuestDifficultInfo(iLevel);
    }

    QuestFileInfo blank;
    return blank;
}

int KQuestGameManager::GetQuestCorruptionIndex( EGCGameMode eGameMode )
{
    int iCorruptionIndex = 0;
    for ( int i = 0; i < (int)m_vtQuestSign.size(); ++i )
    {
        if ( (EGCGameMode)m_vtQuestSign[i].iQuestID == eGameMode )
        {
            iCorruptionIndex = m_vtQuestSign[i].GetQuestDifficultInfo( SiKGCRoomManager()->GetDungeonLevel() ).iCorruptionIndex;
            break;
        }
    }
    return iCorruptionIndex;
}

bool KQuestGameManager::IsQuestLevelFromInputLevel(int iLowLevel, int iHighLevel, EGCGameMode eGameMode, int iDungeonLevel, int iMonLevel )
{
    int iLimitLevel;

    // 몬스터 레벨을 모를 때
    if( -1 == iMonLevel )
    {
        // 게임모드를 모를 때( 월드맵에서 처리됨 )
        if( EGCGameMode(-1) == eGameMode )
        {
            std::vector< EGCGameMode > vecDungeonInfo = SiKGCWorldMapManager()->GetCurWorldMapInfo();
            std::vector< EGCGameMode >::iterator vecIter = vecDungeonInfo.begin();
            for(; vecIter != vecDungeonInfo.end(); ++vecIter)
            {
                //  일반 모드
                iLimitLevel = SiKGCWorldMapManager()->GetLimiteOpenLevel( *vecIter );
                for(int iLoopY=0; iLoopY<iLimitLevel; ++iLoopY)
                {
                    QuestFileInfo kQuestFileInfo = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode( *vecIter, iLoopY );

                    // 입력받은 레벨이 던전레벨 범위안에 포함되는지 확인
                    // 입력받은 레벨이 던전레벨 범위안에 포함되는지 확인
                    if( ( kQuestFileInfo.iLowLevel <= iLowLevel && iHighLevel <= kQuestFileInfo.iHighLevel ) ||   // 완전포함           [   [==]   ]
                        ( kQuestFileInfo.iLowLevel >= iLowLevel && iHighLevel >= kQuestFileInfo.iLowLevel  ) ||   // High이 Low에 걸침 [=[=]        ]
                        ( kQuestFileInfo.iHighLevel >= iLowLevel && iHighLevel >= kQuestFileInfo.iHighLevel) )    // Low가 High에 걸침   [        [=]=]
                        return true;
                }

                //  헬모드 체크
                if (SiKGCRoomListManager()->IsHellMode())
                {
                    QuestFileInfo kQuestFileInfo = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode(*vecIter, GC_DUNGEON_LEVEL_HELL);

                    // 입력받은 레벨이 던전레벨 범위안에 포함되는지 확인                        
                    if ((kQuestFileInfo.iLowLevel <= iLowLevel && iHighLevel <= kQuestFileInfo.iHighLevel) ||   // 완전포함           [   [==]   ]
                        (kQuestFileInfo.iLowLevel >= iLowLevel && iHighLevel >= kQuestFileInfo.iLowLevel) ||   // High이 Low에 걸침 [=[=]        ]
                        (kQuestFileInfo.iHighLevel >= iLowLevel && iHighLevel >= kQuestFileInfo.iHighLevel))    // Low가 High에 걸침   [        [=]=]
                        return true;
                }

                if (SiKGCRoomListManager()->IsBreakMode())
                {
                    QuestFileInfo kQuestFileInfo = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode(*vecIter, GC_DUNGEON_LEVEL_BREAK);

                    // 입력받은 레벨이 던전레벨 범위안에 포함되는지 확인                        
                    if ((kQuestFileInfo.iLowLevel <= iLowLevel && iHighLevel <= kQuestFileInfo.iHighLevel) ||   // 완전포함           [   [==]   ]
                        (kQuestFileInfo.iLowLevel >= iLowLevel && iHighLevel >= kQuestFileInfo.iLowLevel) ||   // High이 Low에 걸침 [=[=]        ]
                        (kQuestFileInfo.iHighLevel >= iLowLevel && iHighLevel >= kQuestFileInfo.iHighLevel))    // Low가 High에 걸침   [        [=]=]
                        return true;
                }

            }
        }
        // 던전 난이도를 모를때( 월드맵에서 처리됨 )
        else if( -1 == iDungeonLevel )
        {
            //  일반 모드 체크
            iLimitLevel = SiKGCWorldMapManager()->GetLimiteOpenLevel( eGameMode );
            for(int iLoopY=0; iLoopY<iLimitLevel; ++iLoopY)
            {
                QuestFileInfo kQuestFileInfo = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode( eGameMode, iLoopY );

                // 입력받은 레벨이 던전레벨 범위안에 포함되는지 확인
                if( ( kQuestFileInfo.iLowLevel <= iLowLevel && iHighLevel <= kQuestFileInfo.iHighLevel ) ||   // 완전포함           [   [==]   ]
                    ( kQuestFileInfo.iLowLevel >= iLowLevel && iHighLevel >= kQuestFileInfo.iLowLevel  ) ||   // High이 Low에 걸침 [=[=]        ]
                    ( kQuestFileInfo.iHighLevel >= iLowLevel && iHighLevel >= kQuestFileInfo.iHighLevel) )    // Low가 High에 걸침   [        [=]=]
                    return true;
            }
            //  헬모드 체크
            if ( SiKGCRoomListManager()->IsHellMode() ) 
            {
                QuestFileInfo kQuestFileInfo = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode( eGameMode, GC_DUNGEON_LEVEL_HELL );

                // 입력받은 레벨이 던전레벨 범위안에 포함되는지 확인
                if( ( kQuestFileInfo.iLowLevel <= iLowLevel && iHighLevel <= kQuestFileInfo.iHighLevel ) ||   // 완전포함           [   [==]   ]
                    ( kQuestFileInfo.iLowLevel >= iLowLevel && iHighLevel >= kQuestFileInfo.iLowLevel  ) ||   // High이 Low에 걸침 [=[=]        ]
                    ( kQuestFileInfo.iHighLevel >= iLowLevel && iHighLevel >= kQuestFileInfo.iHighLevel) )    // Low가 High에 걸침   [        [=]=]
                    return true;
            }

            if (SiKGCRoomListManager()->IsBreakMode())
            {
                QuestFileInfo kQuestFileInfo = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode(eGameMode, GC_DUNGEON_LEVEL_BREAK);

                // 입력받은 레벨이 던전레벨 범위안에 포함되는지 확인
                if ((kQuestFileInfo.iLowLevel <= iLowLevel && iHighLevel <= kQuestFileInfo.iHighLevel) ||   // 완전포함           [   [==]   ]
                    (kQuestFileInfo.iLowLevel >= iLowLevel && iHighLevel >= kQuestFileInfo.iLowLevel) ||   // High이 Low에 걸침 [=[=]        ]
                    (kQuestFileInfo.iHighLevel >= iLowLevel && iHighLevel >= kQuestFileInfo.iHighLevel))    // Low가 High에 걸침   [        [=]=]
                    return true;
            }

        }
        else
        {
            QuestFileInfo kQuestFileInfo = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode( eGameMode, iDungeonLevel );

            // 입력받은 레벨이 던전레벨 범위안에 포함되는지 확인
            if( ( kQuestFileInfo.iLowLevel <= iLowLevel && iHighLevel <= kQuestFileInfo.iHighLevel ) ||   // 완전포함           [   [==]   ]
                ( kQuestFileInfo.iLowLevel >= iLowLevel && iHighLevel >= kQuestFileInfo.iLowLevel  ) ||   // High이 Low에 걸침 [=[=]        ]
                ( kQuestFileInfo.iHighLevel >= iLowLevel && iHighLevel >= kQuestFileInfo.iHighLevel) )    // Low가 High에 걸침   [        [=]=]
                return true;
        }
    }
    else
    {
        if( iLowLevel <= iMonLevel && iMonLevel <= iHighLevel )
            return true;
    }

    return false;
}

bool KQuestGameManager::ReloadQuestScript(void)
{
    if( FAILED(LoadQuestSignScript("QuestSign.stg")) ) return false;

    return true;
}

bool KQuestGameManager::IsExistenceStage(int iStage)
{
    if( (int)m_Quest.vtStage.size() > iStage && iStage >= 0 )
        return true;

    return false;
}

void KQuestGameManager::CheckMinimapArrowColor(int iOldRow, int iOldCol, int iOldStage, int iNewRow, int iNewCol, int iNewStage)
{
    //// ARROW_RIGHT = 1, ARROW_BOTTOM = 2, ARROW_LEFT = 3, ARROW_TOP = 4
    //// 이전 위치에서 현재 위치를 빼서 이동 경로를 알아냅니다.
    //// 1칸 차이 나는 것들만 이동한것으로 체크합니다.
    TileMap *pTileMap = NULL;

    // 왼쪽
    pTileMap = m_Quest.GetTileMap(iNewRow, iNewCol-1);
    if( pTileMap )
    {
        if( m_mapStageHistory[ std::make_pair(pTileMap->iRow, pTileMap->iCol) ] )
        {
            g_pkGameOverlayUI->SetMiniMapArrowColor(iNewRow, iNewCol, KGCQuestTile::ARROW_LEFT, 1);
            g_pkGameOverlayUI->SetMiniMapArrowColor(pTileMap->iRow, pTileMap->iCol, KGCQuestTile::ARROW_RIGHT, 1);
        }
    }

    // 오른쪽
    pTileMap = m_Quest.GetTileMap(iNewRow, iNewCol+1);
    if( pTileMap )
    {
        if( m_mapStageHistory[ std::make_pair(pTileMap->iRow, pTileMap->iCol) ] )
        {
            g_pkGameOverlayUI->SetMiniMapArrowColor(iNewRow, iNewCol, KGCQuestTile::ARROW_RIGHT, 1);
            g_pkGameOverlayUI->SetMiniMapArrowColor(pTileMap->iRow, pTileMap->iCol, KGCQuestTile::ARROW_LEFT, 1);
        }
    }

    // 위
    pTileMap = m_Quest.GetTileMap(iNewRow-1, iNewCol);
    if( pTileMap )
    {
        if( m_mapStageHistory[ std::make_pair(pTileMap->iRow, pTileMap->iCol) ] )
        {
            g_pkGameOverlayUI->SetMiniMapArrowColor(iNewRow, iNewCol, KGCQuestTile::ARROW_TOP, 1);
            g_pkGameOverlayUI->SetMiniMapArrowColor(pTileMap->iRow, pTileMap->iCol, KGCQuestTile::ARROW_BOTTOM, 1);
        }
    }

    // 아래
    pTileMap = m_Quest.GetTileMap(iNewRow+1, iNewCol);
    if( pTileMap )
    {
        if( m_mapStageHistory[ std::make_pair(pTileMap->iRow, pTileMap->iCol) ] )
        {
            g_pkGameOverlayUI->SetMiniMapArrowColor(iNewRow, iNewCol, KGCQuestTile::ARROW_BOTTOM, 1);
            g_pkGameOverlayUI->SetMiniMapArrowColor(pTileMap->iRow, pTileMap->iCol, KGCQuestTile::ARROW_TOP, 1);
        }
    }
}

void KQuestGameManager::LoadStage(TileMap tilemap)
{
    // 스테이지 로딩이 30초안에 끝나지 않는 경우를 체크하겠다.
    m_kStageLoadingStopWatch.SetStopWatch(30000);
    g_MyD3D->Pause( TRUE );

    // 이전 스테이지에서 먹은 gp들을 서버에 알린다.
    if ( g_MyD3D->GetMyPlayer()->m_kUserInfo.bHost )
    {
        if ( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) )
        {
            SiKP2P()->GetInstance()->Send_InfinityDungeonRewardGPNot( m_setGpUID );
        }
        else
        {
            SiKP2P()->GetInstance()->Send_DungeonRewardGPNot( m_setGpUID );
        }
        m_setGpUID.clear();
    }

    // 미니맵 상의 지나온 경로 화살표 색상을 변경해준다.
    CheckMinimapArrowColor(m_iCurTileY, m_iCurTileX, m_iCurStage, tilemap.iRow, tilemap.iCol, tilemap.iStage);

    //************************************************************************
    // 기존 스테이지를 초기화 한다.
    //************************************************************************
    SiKGCFantasticMap()->ClearWater();
    g_pkGameOverlayUI->ClearTitleText();                    // 표시할 TitleText들을 모두 지운다.
    SetShowDialogue( false );                               // 다이얼로그 박스를 표시하지 않는다.
    SiKGCDropItemManager()->ClearNowDropedItem();           // 드랍 되었었던 아이템을 모두 없앤다.
    SiKGCDropItemManager()->ClearNowDropedGP();           // 드랍 되었었던 아이템을 모두 없앤다.
    g_pStateGame->ClearStageResource( true );
    for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
        {
            // 제일 처음에 방장에게 트리거 패킷을 못 받았을때, 위치 세팅이 0, 0으로 되어있어서..
            // 영웅던전 첫 스테이지에서 떨어져 죽는다...
            if ( tilemap.iFlag == 1 )
            {
                if( GC_GMC_DOTA != SiKGCRoomManager()->GetGameModeCategory() )
                SetPlayerPosition( i, 0.0f, 1.0f );
            }

            g_MyD3D->MyPlayer[i]->ProcessMetamorphosisNormalForm( false );
            g_MyD3D->MyPlayer[i]->InitForceNum();
			g_MyD3D->MyPlayer[i]->InitMonoVolt();
        }
    }

    // 첫 스테이지가 아니면 이 지랄ㅋ
    if( GetNumOfClearStage() )
    {
        AllPlayerToReady();
    }
    else
    {
		//if ( !SiKGCWorldMapManager()->IsSkipStartMotion( SiKGCRoomManager()->GetGameMode() ) )
		{
			// 다른 플레이어가 승리포즈를 취하고 있을 경우를 대비하여 대기 모션으로 바꿔준다.
			g_MyD3D->GetMyPlayer()->SetPlayerState( g_MyD3D->GetMyPlayer()->GetCurFormTemplate().START );
			g_MyD3D->GetMyPlayer()->UpdateData();
		}
    }

    // 기존 스테이지의 포인트를 저장하고, 다음 스테이지의 포인트를 불러온다.
    if( !g_MyD3D->IsLocalPlayerObserver() )
    {
        SaveToQuestPointList(m_iCurStage, g_MyD3D->Get_MyPlayer(), m_PlayerQuestPoint[g_MyD3D->Get_MyPlayer()]);
        SendNetworkData(GC_PID_QUEST_STAGE_RESULT_SCORE, 0);
        LoadFromQuestPointList(tilemap.iStage, g_MyD3D->Get_MyPlayer(), m_PlayerQuestPoint[g_MyD3D->Get_MyPlayer()]);
    }    

    //************************************************************************
    // 이번 스테이지를 로드한다.
    //************************************************************************
    m_iCurTileX = tilemap.iCol;
    m_iCurTileY = tilemap.iRow;
    m_iCurStage = tilemap.iStage;

    // 스크립트 로드
    SiKGCFantasticMap()->UnLoad();
    SiKGCLayerManager()->UnLoad();

    std::string strMapPath;// = "./Stage/";
    EGCGameMode eCurrentGameMode = SiKGCRoomManager()->GetGameMode();
    if( SiKGCWorldMapManager()->IsInfinityDungeon( eCurrentGameMode ) ) {
        int iStageIndex = SiKGCInfinityDungeonManager()->GetStartStageIndex();
        if( -1 == iStageIndex ) { 
            strMapPath += m_Quest.vtStage[tilemap.iStage].strStageMap;
        }
        else { 
            strMapPath += SiKGCInfinityDungeonManager()->GetStageName( eCurrentGameMode, iStageIndex );
        }
    }
    else { 
        strMapPath += m_Quest.vtStage[tilemap.iStage].strStageMap;
    }

    SiKGCLayerManager()->RegisterLuabind();
    //SiKGCLayerManager()->LoadScript( strMapPath.c_str() );
    SiKGCLayerManager()->LoadScript( strMapPath );
	
    PreGeneraterStageMonster( tilemap.iStage );

    // 로딩을 기다릴 사람에 대한 정보를 설정해둠
    m_vecWaitToLoad.clear();
    m_vecEndGameInfo.clear();

    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == true )
        {
            m_vecWaitToLoad.push_back( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );

            if( i != g_MyD3D->Get_MyPlayer() )
                m_vecEndGameInfo.push_back( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID );

			//스테이지가 로드될 때 마다 현재 공격력을 저장한다.
#ifdef LEVEL_DESIGN_STAT_CALC
            m_vecCurAtk[i].push_back(g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_ATK]);
#else
            m_vecCurAtk[i].push_back(g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_ATK] * 100.0f);
#endif
        }
		else
		{
			m_vecCurAtk[i].push_back(0.0f);
		}
    }
    std::vector<SUserInfo> &observer = SiKGCRoomManager()->GetRoomInfo().vecObserverUserInfo;

    for(std::vector<SUserInfo>::iterator vit = observer.begin();vit != observer.end();vit++)
    {
        if(SiKGCRoomManager()->IsObserverLive(vit->dwUID) )
            m_vecWaitToLoad.push_back(vit->dwUID);
    }


    m_bLoadStageByPlayers   = false;
    m_bClearStage           = false;
    m_bGetEndGameInfo       = false;
    SetEnableMovement(false);                           // 몬스터의 움직임을 정지 시킨다.

    SetPlayerPositionByGate();                          // 플레이어의 위치를 지나온 문 옆으로 옮긴다.    
    SetSummonMonsterPosition();                         // 소환 몬스터의 위치를 주인 플레이어에게로 옮긴다.
    g_pStateGame->OnEnterStage();    
    g_kCamera.SetTargetMonster(-1);                         //혹시나 몬스터를 가리키고 있었다면 취소한다    
    g_kCamera.SetTargetPlayer( g_MyD3D->Get_MyPlayer() );   // Drama에서 다른 플레이어를 가리키고 있다면 나를 가리키도록 수정함.    
    g_pkPetAI->AllPetEnable();                              //펫이 움직이게 한다.
    g_kCamera.InitCamera();

    g_kGateStage->Init();               // 문 상태 보내는 구조체를 초기화
    m_dwEndGameDelay             = END_GAME_DELAY_TIME;

    m_mapStageHistory[ std::make_pair(m_iCurTileY, m_iCurTileX) ] = true;

    m_bQuestStarted = false;

    m_vtWaterPoleTime.clear();	
    m_vtCurWaterPole.clear();

    ClearEndDialogList();

    // 탑승물을 탑승한 상태로 이동한 캐릭터를 처리해주자!!

    DWORD dwCountEmbarkUID=0;
    for(int iLoop=0; iLoop<MAX_PLAYER_NUM; ++iLoop)
    {
        PLAYER *pPlayer = g_MyD3D->MyPlayer[iLoop];
        int iEmbarkTypeOfInPortal = pPlayer->GetEmbarkTypeOfInPortal();
        if( INT_MAX > iEmbarkTypeOfInPortal )
        {
            KGC_PID_BROAD_EMBARK_SUMMON_ACK kPacket;
            kPacket.dwPlayerUID = pPlayer->GetPlayerUID();
            kPacket.dwEmbarkUID = pPlayer->GetEmbarkUIDOfInPortal();
            kPacket.iEmbarkType = iEmbarkTypeOfInPortal;
            kPacket.fPosX = pPlayer->vPos.x;
            kPacket.fPosY = pPlayer->vPos.y;
            kPacket.bIsRight = pPlayer->GetIsRight();
            kPacket.bEmbakation = true;
            SiKGCEmbarkManager()->SummonEmbark( &kPacket );
            pPlayer->SetEmbarkTypeOfInPortal( INT_MAX );
            pPlayer->SetEmbarkUIDOfInPortal( INT_MAX );

            if( dwCountEmbarkUID < kPacket.dwEmbarkUID )
                dwCountEmbarkUID = kPacket.dwEmbarkUID;
        }
    }
    SiKGCEmbarkManager()->SetCountUID( dwCountEmbarkUID );

    //m_bDramaStage = m_pkDrama.IsDramaStage( m_vtQuestSign[SiKGCRoomManager()->GetRoomInfo().ucStage].iQuestID, m_iCurStage );  

    // 로딩이 끝났다!
    g_MyD3D->Pause( FALSE );
    
#if defined(USE_STAGE_LOAD_COMPLETE_SIGN)
    g_pkGameOverlayUI->CheckStageLoadComplete(g_kGlobalValue.m_kUserInfo.dwUID);  
#endif

    // 버닝모드를 초기화 해주자!
    if( g_MyD3D->GetMyPlayer()->m_bIsBurnningMode )
        g_MyD3D->GetMyPlayer()->ChangeBurnningTemplate();

    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if( pPlayer && true == SiGCEclipseEventManager()->IsValidDungeon( SiKGCRoomManager()->GetGameMode() ) ) {
        pPlayer->RenderEclipseBack();
    }

    if (tilemap.iStage > 0)
    {
        if (g_MyD3D->m_akPet[g_MyD3D->Get_MyPlayer()]->m_pObject != nullptr)
        {
            auto fPetGlyphRetainMP = SiGCPetMagicStone()->GetGlyphEffectAmmount(PET_MAGIC_STONE_EFFECT_TYPE::PMSET_RETAIN_PET_MP);
            if (fPetGlyphRetainMP > 0.f)
            {
                g_MyD3D->m_akPet[g_MyD3D->Get_MyPlayer()]->SetMp(fPetGlyphRetainMP);
            }
        }
    }

    g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->Item_Remain_Time[GC_GAME_ITEM_SUPER] = 0;

    // 스테이지가 넘어갈때 이전 다이얼로그 자료 없애쟈!
    m_vecDialogue.clear();
}

void KQuestGameManager::PreGeneraterStageMonster( int iStage )
{
    std::vector<CTriggerCondition*> vtMonGenTrigger;
    g_kEveryTriggerManager.GetTriggerConditionByType( iStage, CTriggerCondition::TG_MON_GEN, vtMonGenTrigger );

	//던전해킹방지
	m_iMonsterNum = (int)vtMonGenTrigger.size();
	m_iCurMonsterNum = m_iMonsterNum;
	m_bMonLoadComplete = false;
	m_bBossStage = false;
	g_MyD3D->m_pkQuestGameMgr->SetAttackedMonster(false);

	std::map<int,bool> mapReserved;

    for(int i=0;i<(int)vtMonGenTrigger.size();++i)
    {
        CTGMonsterGenerater* pMonGen = (CTGMonsterGenerater*)vtMonGenTrigger[i];

		m_bMonLoadComplete = true;

        // 이미 Complete(사용)된 패킷이고, 반복 사용 체크가 없다면 생성하지 않는다.
		if( pMonGen->IsComplete() && false == pMonGen->m_bRepeat ) {
			mapReserved.insert(std::pair<int,bool>(pMonGen->m_iMonIndex,false));
            continue;
		}

        g_MyD3D->CreateMeshAnimationForPrevLoad( pMonGen->m_iMonType );
		mapReserved.insert(std::pair<int,bool>(pMonGen->m_iMonIndex,true));

		if( pMonGen->m_iBoss == MONSTER::MON_BOSS )
			m_bBossStage = true;
    }


    // 던전 이벤트 몬스터를 로딩한다.
    std::vector<KEventMonster>::iterator vit = g_kGlobalValue.m_vecEventMonster.begin();
    while ( vit != g_kGlobalValue.m_vecEventMonster.end() )
    {
        if ( (*vit).m_nStageNum == iStage )
        {
            int iMonIndex = -1;

			for ( std::map<int,bool>::iterator mit = mapReserved.begin() ; mit != mapReserved.end() ; ++mit ) {
				if ( false == mit->second )
				{
					iMonIndex = mit->first;
					mit->second = true;
					break;
				}
			}

            if ( iMonIndex == -1 )
            {
                iMonIndex = g_kMonsterManager.GetNextCreateMonstersNum( ECREATE_MONSTER_TYPE_EVENT );
                g_kMonsterManager.CreateMonster( iMonIndex );
            }

            MONSTER* pMon = g_kMonsterManager.GetMonster(iMonIndex);

			if(pMon == NULL) continue;

            D3DXVECTOR2 pos = g_MyD3D->MyStg->Get_Random_XY();

            if ( pos.x - pMon->m_fWidth / 2.0f < 0 )
                pos.x = pMon->m_fWidth / 2.0f;
            else if ( pos.x + pMon->m_fWidth / 2.0f > SiKGCFantasticMap()->GetWorldWidth() )
                pos.x = SiKGCFantasticMap()->GetWorldWidth() - pMon->m_fWidth / 2.0f;

            DWORD dwChampionProperty = 0x00000000;
            for( int d = 0; d < (int)g_kGlobalValue.m_vecChampionInfo.size(); ++d )
            {
                if( g_kGlobalValue.m_vecChampionInfo[d].m_nTriggerID == (*vit).m_nTriggerID )
                {
                    dwChampionProperty = g_kGlobalValue.m_vecChampionInfo[d].m_dwProperty;
                    break;
                }
            }

            // 보스 오브젝트 
            if ( (*vit).m_nSpecial == KEventMonster::OPT_GATE )
            {
                // 보스 오브젝트는 보스 스테이지의 보스와 레벨 스텟을 맞춘다 ㅠㅠ 아...
                std::vector<CTriggerCondition*> vtMonGenTrigger;
                g_kEveryTriggerManager.GetTriggerConditionByType( m_iBossStageNum, CTriggerCondition::TG_MON_GEN, vtMonGenTrigger );
                for ( int i = 0; i < (int)vtMonGenTrigger.size(); ++i )
                {
                    CTGMonsterGenerater* pMonGen = (CTGMonsterGenerater*)vtMonGenTrigger[i];
                    if ( pMonGen->m_iBoss == 1 )
                    {
                        pMon->Wait_Room( MON_BOSS_OBJECT, pos.x, pos.y, true, true, 99, 0x00000000, false );
                        pMon->SetTriggerID(-1);

                        // 몬스터 템플릿의 정보를 가져옴
                        SMonsterTemplate kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ pMonGen->m_iMonType ];
                        g_kGlobalValue.SetMonsterStat( pMonGen->m_iMonType, pMonGen->m_iMonLevel, kMonsterTemplate, true );

                        pMon->m_kChangeMon.SetChangeMonInfo( (*vit).m_nMonsterID, pMonGen->m_iMonLevel, kMonsterTemplate.fAttackPoint, kMonsterTemplate.fHPPoint, (*vit).m_nSpecial, (*vit).m_nTriggerID, dwChampionProperty);

                        // 미리로딩
                        g_MyD3D->CreateMeshAnimationForPrevLoad( (*vit).m_nMonsterID );
                        break;
                    }
                }
            }
            else
            {
                pMon->Wait_Room( (*vit).m_nMonsterID, pos.x, pos.y, true, true, (*vit).m_nMonLv, dwChampionProperty, (*vit).m_bIsBoss );
                pMon->SetTriggerID((*vit).m_nTriggerID);
                pMon->SetTargetingDelayTime( GAME_START_TARGETING_DELAY_TIME );     // 5초간 몬스터들이 타게팅을 하지 않는다.
            }

            pMon->m_bLive = true;
            pMon->ToggleRender( true );

            vit = g_kGlobalValue.m_vecEventMonster.erase(vit);
        }
        else
            ++vit;
    }

	if (m_bBossStage)
	{
		if (SiKGCEventManager()->IsActiveEvent(EVENT_ID_BOSS_FURY))
		{
			g_pMagicEffect->SetMagicEffect(g_MyD3D->Get_MyPlayer() , EGC_EFFECT_BOSS_FURY_BUFF, 999999.0f);
		}
		else
		{
			g_pMagicEffect->ClearMagicEffect(g_MyD3D->Get_MyPlayer() , EGC_EFFECT_BOSS_FURY_BUFF);
		}
	}

}

void KQuestGameManager::MoveGate(int iDirection, int iDelayTime /*= 0*/)
{
    // 이동처리를 한다.        
    TileMap *pTilemap = NULL;
    int iCurTileX = m_iCurTileX;
    int iCurTileY = m_iCurTileY;
    m_iBeforeDirection = iDirection;
    switch( iDirection )
    {
    case DIRECTION_LEFT:         // left
        if( m_iCurTileX-1 >= 0 )
        {
            iCurTileX = m_iCurTileX-1;
            iCurTileY = m_iCurTileY;
            pTilemap = m_Quest.GetTileMap(iCurTileY, iCurTileX);
        }
        break;
    case DIRECTION_UP:         // top
        if( m_iCurTileY-1 >= 0 )
        {
            iCurTileX = m_iCurTileX;
            iCurTileY = m_iCurTileY-1;
            pTilemap = m_Quest.GetTileMap(iCurTileY, iCurTileX);
        }
        break;
    case DIRECTION_RIGHT:         // right
        if( m_iCurTileX+1 < m_Quest.iTileX )
        {
            iCurTileX = m_iCurTileX+1;
            iCurTileY = m_iCurTileY;
            pTilemap = m_Quest.GetTileMap(iCurTileY, iCurTileX);
        }
        break;
    case DIRECTION_DOWN:         // down
        if( m_iCurTileY+1 < m_Quest.iTileY )
        {
            iCurTileX = m_iCurTileX;
            iCurTileY = m_iCurTileY+1;
            pTilemap = m_Quest.GetTileMap(iCurTileY, iCurTileX);
        }
        break;
    }

    if( pTilemap )
    {
        if( IsExistenceStage(pTilemap->iStage) )
        {
#if defined( DUNGEONPLAYSTATE )
            g_kGlobalValue.AddDungeonPlayState( KGCGlobal::DUNPLAY_STATE_CLEARTIME );
#endif
            g_kEveryTriggerManager.SetNextStage(*pTilemap, true, iDelayTime);
        }
    }
}

void KQuestGameManager::SetArenaPos(float x,float y)
{
	m_vecArena.x=x;
	m_vecArena.y=y;
}

D3DXVECTOR2 &  KQuestGameManager::GetArenaPos()
{
	/*D3DXVECTOR2 vec(0,0);
	// 게이트 오브젝트만을 추려내어 벡터에 담는다.
	std::vector<CTriggerCondition*> vtTGCondition;
	g_kEveryTriggerManager.GetTriggerConditionByType(m_iCurStage, CTriggerCondition::TG_GATE_OBJECT_GENERATER, vtTGCondition);

	// 이동해온 방향과 비교하여서 캐릭터를 이동시킨다.
	for(int i=0;i<(int)vtTGCondition.size();++i)
	{
		CTGGateObjectGenerater *pGateObjGen = static_cast<CTGGateObjectGenerater*>(vtTGCondition[i]);

		bool bNextStage = false;
		switch( m_iBeforeDirection )
		{
		case DIRECTION_LEFT:         // left
			if( DIRECTION_RIGHT == pGateObjGen->m_iDirection )
			{
				bNextStage = true;
			}
			break;
		case DIRECTION_UP:         // top
			if( DIRECTION_DOWN == pGateObjGen->m_iDirection )
			{
				bNextStage = true;
			}
			break;
		case DIRECTION_RIGHT:         // right
			if( DIRECTION_LEFT == pGateObjGen->m_iDirection )
			{
				bNextStage = true;
			}
			break;
		case DIRECTION_DOWN:         // down
			if( DIRECTION_UP == pGateObjGen->m_iDirection )
			{
				bNextStage = true;
			}
			break;
		}

		if( bNextStage )
		{
			m_vecArena.x=pGateObjGen->m_fEntranceX*0.0025f;
			m_vecArena.y=pGateObjGen->m_fEntranceY*0.0025f;
			return m_vecArena;
		}
	}*/

	return m_vecArena;
}

void KQuestGameManager::SetPlayerPositionByGate(void)
{
    // 게이트 오브젝트만을 추려내어 벡터에 담는다.
    std::vector<CTriggerCondition*> vtTGCondition;
    g_kEveryTriggerManager.GetTriggerConditionByType(m_iCurStage, CTriggerCondition::TG_GATE_OBJECT_GENERATER, vtTGCondition);

    // 이동해온 방향과 비교하여서 캐릭터를 이동시킨다.
    for(int i=0;i<(int)vtTGCondition.size();++i)
    {
        CTGGateObjectGenerater *pGateObjGen = static_cast<CTGGateObjectGenerater*>(vtTGCondition[i]);

        bool bNextStage = false;
        switch( m_iBeforeDirection )
        {
            case DIRECTION_LEFT:         // left
                if( DIRECTION_RIGHT == pGateObjGen->m_iDirection )
                {
                    bNextStage = true;
                }
                break;
            case DIRECTION_UP:         // top
                if( DIRECTION_DOWN == pGateObjGen->m_iDirection )
                {
                    bNextStage = true;
                }
                break;
            case DIRECTION_RIGHT:         // right
                if( DIRECTION_LEFT == pGateObjGen->m_iDirection )
                {
                    bNextStage = true;
                }
                break;
            case DIRECTION_DOWN:         // down
                if( DIRECTION_UP == pGateObjGen->m_iDirection )
                {
                    bNextStage = true;
                }
                break;
        }

        if( bNextStage )
        {
            for(int i=0;i<MAX_PLAYER_NUM;++i)
            {
                SetPlayerPosition(i, pGateObjGen->m_fEntranceX*0.0025f, pGateObjGen->m_fEntranceY*0.0025f);                
            }
            g_MyD3D->GetMyPlayer()->All_Latency_Equal();

            return;
        }
    }
}

void KQuestGameManager::SetSummonMonsterPosition()
{
    // 소환된 몬스터의 위치를 부모위치와 같게 해준다.
	for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) == false )
			continue;

		DWORD dwUID = mit->second->GetSummonPlayerUID();
		PLAYER* pPlayer = g_MyD3D->GetPlayerByUID( dwUID );
		if( pPlayer )
		{
			D3DXVECTOR3 vPos = pPlayer->GetPosition();
			mit->second->SetX( vPos.x );
			mit->second->SetY( vPos.y );
		}

	}

}

void KQuestGameManager::SetPlayerPosition(int iPlayerIndex, float iX, float iY)
{
    if( iPlayerIndex < 0 || iPlayerIndex >= MAX_PLAYER_NUM ) return;

    // 속도가 조금이라도 있어야지 해당 좌표로 캐릭터가 이동하더라.. 그래서 일부러 스피드를 준것임.
    g_MyD3D->MyPlayer[iPlayerIndex]->x_Speed = 0.0001f;
    g_MyD3D->MyPlayer[iPlayerIndex]->y_Speed = 0.0001f;
    g_MyD3D->MyPlayer[iPlayerIndex]->vPos.x = iX;
    g_MyD3D->MyPlayer[iPlayerIndex]->vPos.y = iY;

	if( iPlayerIndex==g_MyD3D->GetMyPlayer()->m_iPlayerIndex)
	{
		g_MyD3D->m_pkQuestGameMgr->SetArenaPos(iX,iY);
	}

    for(int i=0;i<LATENCY+SHOWLATENCY;++i)
    {
        g_MyD3D->MyPlayer[iPlayerIndex]->L_x[i]        = g_MyD3D->MyPlayer[iPlayerIndex]->vPos.x;
        g_MyD3D->MyPlayer[iPlayerIndex]->L_y[i]        = g_MyD3D->MyPlayer[iPlayerIndex]->vPos.y;
    }
}

void KQuestGameManager::SetStageLoadComplete( DWORD dwUID_ )
{
    std::vector<DWORD>::iterator vit = std::find( m_vecWaitToLoad.begin(), m_vecWaitToLoad.end(), dwUID_ );
    if ( vit != m_vecWaitToLoad.end() )
    {
        m_vecWaitToLoad.erase( vit );
    }
}

void KQuestGameManager::AllStageLoadComplete()
{
    m_vecWaitToLoad.clear();
}

bool KQuestGameManager::IsPlayerStageLoadComplete( DWORD dwUID_ )
{
    std::vector<DWORD>::iterator vit = std::find( m_vecWaitToLoad.begin(), m_vecWaitToLoad.end(), dwUID_ );

    return ( vit == m_vecWaitToLoad.end() );
}

bool KQuestGameManager::IsStageLoadComplete(void)
{
    if( m_bLoadStageByPlayers ) return true;

    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == true )
        {
            if( !IsPlayerStageLoadComplete( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID ) )
                return false;
        }
    }
    std::vector<SUserInfo> &observer = SiKGCRoomManager()->GetRoomInfo().vecObserverUserInfo;

    for(std::vector<SUserInfo>::iterator vit = observer.begin();vit != observer.end();vit++)
    {
        if(SiKGCRoomManager()->IsObserverLive(vit->dwUID) )
        {
            if( !IsPlayerStageLoadComplete( vit->dwUID ) )
                return false;
        }
    }

    if( !m_bLoadStageByPlayers )
    {
        m_vecWaitToLoad.clear();
        //AllPlayerToReady();
        m_bLoadStageByPlayers = true;
		//g_pkGameOverlayUI->FadeInOrOut(true);

        // Jeaho.Test UDP Connect Test
        // 릴레이 서버에서 제명되었을 수도 있기 때문에 릴레이 서버로 패킷을 던진다.
        SendNetworkData( GC_PID_UDP_RELAYSERVER_HEART_BEAT, 0 );

        SendP2PPacket( GC_PID_RELAY_PING );
    }

    return true;
}

void KQuestGameManager::AllPlayerToReady(void)
{
    // 다른 플레이어가 승리포즈를 취하고 있을 경우를 대비하여 대기 모션으로 바꿔준다.
    for(int i=0;i<MAX_PLAYER_NUM;i++)
    {
        PlayerToReady(i);
    }
    for(int i=0;i<LATENCY;i++)
    {
        g_kLocalPlayerLatency->kRenderData[i].usMotion = g_MyD3D->GetMyPlayer()->GetWaitMotion();
        g_kLocalPlayerLatency->kRenderData[i].cFrame = i;
    }
    g_MyD3D->GetMyPlayer()->UpdateData();
}

void KQuestGameManager::PlayerToReady(int iPlayer)
{
    // 다른 플레이어가 승리포즈를 취하고 있을 경우를 대비하여 대기 모션으로 바꿔준다.
    if( g_MyD3D->MyPlayer[iPlayer]->m_kUserInfo.bLive
        && g_MyD3D->MyPlayer[iPlayer]->m_cLife > 0
        )
    {
        for(int j=0;j<LATENCY+SHOWLATENCY;j++)
        {
            g_MyD3D->MyPlayer[iPlayer]->L_Now_Motion[j] = g_MyD3D->MyPlayer[iPlayer]->GetWaitMotion();
            g_MyD3D->MyPlayer[iPlayer]->L_Frame[j] = j;
        }
        g_MyD3D->MyPlayer[iPlayer]->Silk               = 0;    // 캐릭터 필살기 그림을 안나오게 한다.
        g_MyD3D->MyPlayer[iPlayer]->cFrame             = 0;
        g_MyD3D->MyPlayer[iPlayer]->Old_Motion         = g_MyD3D->MyPlayer[iPlayer]->GetWaitMotion();
        g_MyD3D->MyPlayer[iPlayer]->Reserve_Motion     = g_MyD3D->MyPlayer[iPlayer]->GetWaitMotion();
        g_MyD3D->MyPlayer[iPlayer]->Next_Motion        = g_MyD3D->MyPlayer[iPlayer]->GetWaitMotion();
        g_MyD3D->MyPlayer[iPlayer]->Before_Motion      = g_MyD3D->MyPlayer[iPlayer]->GetWaitMotion();
        g_MyD3D->MyPlayer[iPlayer]->Direct_Motion_Input(g_MyD3D->MyPlayer[iPlayer]->GetWaitMotion());
        g_MyD3D->MyPlayer[iPlayer]->SetPlayerState(g_MyD3D->MyPlayer[iPlayer]->GetWaitMotion());
        g_MyD3D->MyPlayer[iPlayer]->ProcessMetamorphosisNormalForm( false );
    }
    for(int i=0;i<LATENCY+SHOWLATENCY;i++)
    {
        g_kLocalPlayerLatency->kRenderData[i].usMotion = g_MyD3D->GetMyPlayer()->GetWaitMotion();
        g_kLocalPlayerLatency->kRenderData[i].cFrame = i;
    }
    g_MyD3D->m_akPet[iPlayer]->InitBeforeGameStart();
    g_MyD3D->GetMyPlayer()->UpdateData();
}

HRESULT KQuestGameManager::LoadDialogueTable(void)
{
    m_vecDialogue.clear();
    m_mapDialogueList.clear();

    HRESULT hr;
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "QuestDialogue.stg" ) == false )
        return E_FAIL;

    // NOTE : 대화 테이블들은 dialogue*안에 기록된다.

    if ( SUCCEEDED( hr = luaMgr.BeginTable( "dialogue" ) ) )
    {
        // read dialogue table
        for ( int j = 1; ; ++j )
        {
            if ( SUCCEEDED( hr = luaMgr.BeginTable( j ) ) )
            {
                // NOTE : 대화 테이블은 다음과 같이 구성되어 있다.
                // { (좌우위치), (대화내용) }

                SDialogue dlg;
                int iStrSpeaker, iStrSpeech;

                std::string strName;
                std::vector<SDialogue> vecDialogue;
                luaMgr.GetValue("name", strName);

                for (int k = 1;; ++k)
                {
                    if (SUCCEEDED(hr = luaMgr.BeginTable(k)))
                    {
                        if (FAILED(hr = luaMgr.GetValue(1, dlg.bRight)))
                        {
                            break;
                        }

                        if (FAILED(hr = luaMgr.GetValue(2, iStrSpeaker)))
                        {
                            break;
                        }

                        if (FAILED(hr = luaMgr.GetValue(3, iStrSpeech)))
                        {
                            break;
                        }

                        if (FAILED(hr = luaMgr.GetValue(4, dlg.strExtra)))
                        {
                            dlg.strExtra = "";
                        }

                        if (FAILED(hr = luaMgr.GetValue(5, dlg.dwDelayTime)))
                        {
                            dlg.dwDelayTime = 0;
                        }

                        if (FAILED(hr = luaMgr.GetValue(6, dlg.dwTime)))
                        {
                            dlg.dwTime = -1;
                        }

                        if (dlg.dwDelayTime < DIALOGUE_MIN_DEALY_TIME)
                        {
                            dlg.dwDelayTime = DIALOGUE_DEALY_TIME;
                            dlg.dwTime = DIALOGUE_MIN_DEALY_TIME;
                        }

                        if (dlg.dwTime == -1)
                        {
                            dlg.dwTime = dlg.dwDelayTime;
                        }

                        if (-1 != iStrSpeaker)
                        {
                            dlg.strSpeaker = g_pkStrLoader->GetString(iStrSpeaker);
                        }

                        dlg.iSpeakerID = iStrSpeaker;
                        dlg.strSpeech = g_pkStrLoader->GetString(iStrSpeech);
                        dlg.iSpeechID = iStrSpeech;

                        vecDialogue.push_back(dlg);

                        if (FAILED(hr = luaMgr.EndTable()))
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                AddDialogueList(strName, vecDialogue);

                if ( FAILED( hr = luaMgr.EndTable() ) ) 
                {
                    assert(0);
                    return E_FAIL;
                }
            }
            else 
                break; // no more table
        }

        if ( FAILED( hr = luaMgr.EndTable() ) ) return false; // invalid file format
    }

    return S_OK; // success
}

bool KQuestGameManager::SetDialogueList(std::string strDialogue)
{
    if( m_mapDialogueList.find(strDialogue) != m_mapDialogueList.end() )
    {
        m_vecDialogue = m_mapDialogueList[strDialogue];
        m_iCurDialoge = 0;

        m_bDramaStage = false;

		std::vector<SDialogue>::iterator vit;
        for ( vit = m_vecDialogue.begin() ; vit != m_vecDialogue.end(); vit++ )
        {
            if ( vit->strExtra.size() != 0 )
            {
                m_bDramaStage = true;
                break;
            }
        }
    }
    else
    {
        return false;
    }

    return true;
}

void KQuestGameManager::AddDialogueList( std::string strName, std::vector<SDialogue> &sDialogue )
{
    m_mapDialogueList[strName] = sDialogue;
}

void KQuestGameManager::NextDialogue(void)
{
    if( !g_pkGameOverlayUI->IsShowDialogue() ) return;

    if( (int)m_vecDialogue.size() > m_iCurDialoge+1 )
    {
        m_iCurDialoge++;
        SetDialogue(m_iCurDialoge);
    }
    else
    {
        m_iCurDialoge = 0;
        SetShowDialogue(false);
    }
}

void KQuestGameManager::PrevDialogue(void)
{
    if( !g_pkGameOverlayUI->IsShowDialogue() ) return;

    if( (int)m_vecDialogue.size() > 0 && (m_iCurDialoge-1 >= 0)  )
    {
        m_iCurDialoge--;
        SetDialogue(m_iCurDialoge);
    }
    else
    {
        if( (int)m_vecDialogue.size() > 0 )
            m_iCurDialoge = (int)m_vecDialogue.size()-1;
        else
            m_iCurDialoge = 0;

        SetShowDialogue(false);
    }
}

SDialogue* KQuestGameManager::GetDialogue( int iIdx )
{
    if( iIdx < 0 || (int)m_vecDialogue.size() <= iIdx )
        return NULL;

    return &m_vecDialogue[iIdx];
}

int KQuestGameManager::GetCurDialogeIndex(void) const
{
    return m_iCurDialoge;
}

void KQuestGameManager::SetShowDialogue(bool bShow)
{
    g_pkGameOverlayUI->EnableDialogueBox(bShow);
    if( g_pkGameOverlayUI->IsShowDialogue() )
    {
        m_dwDialogueDelay       = timeGetTime();
        if ( g_kGlobalValue.m_kUserInfo.bHost )
        {
            // 방장일 때만 하쟈! 방원들은 하면 첫 드라마 안나온다.
            SetDialogue(m_iCurDialoge);
        }
        
        SetEnableMovement(false);
		g_pkGameOverlayUI->RenderOffHPBar( true );
    }
    else
    {
        SetEnableMovement(true);
		g_pkGameOverlayUI->UpdateBossList();
    }
}

void KQuestGameManager::SetDialogue(int iIdx)
{
    if( !g_pkGameOverlayUI->IsShowDialogue() ) return;

    if( (int)m_vecDialogue.size() > iIdx && iIdx >= 0 )
    {
        m_iCurDialoge = iIdx;
        g_pkGameOverlayUI->SetDialogue( m_vecDialogue[m_iCurDialoge].bRight
                                        , m_vecDialogue[m_iCurDialoge].iSpeakerID
                                        , m_vecDialogue[m_iCurDialoge].iSpeechID );

        
        g_pkGameOverlayUI->HideBossHPBar();

        if( strncmp( m_vecDialogue[m_iCurDialoge].strExtra.c_str(), "CameraMonster", 13 ) == 0 )
        {
            //몬스터를 가리키는 카메라로 바꿔준다
            int iMonster;
            sscanf( m_vecDialogue[m_iCurDialoge].strExtra.c_str(), "CameraMonster%d",&iMonster);
            g_kCamera.SetTargetMonster(iMonster);
        }
        else if( m_vecDialogue[m_iCurDialoge].strExtra == "CameraInit" )
        {
            g_kCamera.SetTargetMonster(-1);
        }
		else if( m_vecDialogue[m_iCurDialoge].strExtra == "InitCamera" )
		{
			g_kCamera.InitCamera();
		}
        else if( strncmp( m_vecDialogue[m_iCurDialoge].strExtra.c_str(), "QuestDrama", 10 ) == 0 )
        {
            int iQuest, iDramaIndex;
            sscanf( m_vecDialogue[m_iCurDialoge].strExtra.c_str(), "QuestDrama%d_%d",&iQuest, &iDramaIndex );
            m_pkDrama->Start( iQuest , iDramaIndex );
        }
    }
    else
    {
        SetShowDialogue(false);		
    }
}

void KQuestGameManager::SetEnableMovement(bool bEnable)
{
	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		mit->second->SetMovement(bEnable);
	}
    g_MyD3D->SetEnableKeyInput(bEnable);

    m_bMovement = bEnable;
}

bool KQuestGameManager::IsEnableMovement(void)
{
    return m_bMovement;
}

int KQuestGameManager::GetNumOfClearStage()
{
    int iCntOfClearStage = 0;
    std::map< std::pair<int, int>, bool >::iterator itSH = m_mapStageHistory.begin();
    for(;itSH != m_mapStageHistory.end();++itSH)
    {
        if( itSH->second )
            ++iCntOfClearStage;
    }

    return iCntOfClearStage;
}

int KQuestGameManager::GetPowerPoint(void)
{
    int iTotalPowerRank = 0;
    for(int i=0;i<g_kEveryTriggerManager.GetTotalStageCnt();++i)
    {
        int iTotalBackAttack          = 0;
        int iTotalAttack              = 0;
        int iTotalS1Attack            = 0;
        int iTotalS2Attack            = 0;
        int iTotalS3Attack            = 0;
        int iTotalCriticalAttack      = 0;
        int iTotalDoubleAttack        = 0;
		for(int j=0;j<MAX_PLAYER_NUM;++j)
		{
			if( m_alQuestPoint[j].find( i ) != m_alQuestPoint[j].end() )
			{
				iTotalBackAttack        += m_alQuestPoint[j][i].BackAttack;
				iTotalAttack            += m_alQuestPoint[j][i].Attack;
				iTotalS1Attack          += m_alQuestPoint[j][i].iSpecial1;
				iTotalS2Attack          += m_alQuestPoint[j][i].iSpecial2;
				iTotalS3Attack          += m_alQuestPoint[j][i].iSpecial3;
				iTotalCriticalAttack    += m_alQuestPoint[j][i].CriticalAttack;
				iTotalDoubleAttack      += m_alQuestPoint[j][i].DoubleAttack;
			}
		}

        int iTemp = ( (iTotalS1Attack*3) + (iTotalS2Attack*4) + (iTotalS3Attack*6) + iTotalCriticalAttack*3 + iTotalDoubleAttack*3 );
        iTemp += iTotalBackAttack;

        //if( iTotalAttack > 0 )
        //    iTotalPowerRank += (int)(iTemp / (float)iTotalAttack * 100);
        iTotalPowerRank += iTemp;
    }

    if( g_kEveryTriggerManager.GetTotalStageCnt() > 0 )
        return ((int)(iTotalPowerRank/(float)g_kEveryTriggerManager.GetTotalStageCnt()) + DEFAULT_POWER_POINT);
 
    return (0 + DEFAULT_POWER_POINT);
}

int KQuestGameManager::GetControlPoint(void)
{
    int iTotalControlPoint = 0;
    for(int i=0;i<g_kEveryTriggerManager.GetTotalStageCnt();++i)
    {
        //int iTotalAttack              = 0;
        int iTotalBackAttack          = 0;
        int iTotalAerialAttack        = 0;
        //int iTotalCounter             = 0;
        //int iTotalComboPoint          = 0;
		for(int j=0;j<MAX_PLAYER_NUM;++j)
		{
			if( m_alQuestPoint[j].find( i ) != m_alQuestPoint[j].end() )
			{
				//iTotalAttack           += m_vtQuestPoint[j][i].Attack;
				iTotalBackAttack       += m_alQuestPoint[j][i].BackAttack;
				iTotalAerialAttack     += m_alQuestPoint[j][i].AerialAttack;
				//iTotalCounter          += m_vtQuestPoint[j][i].CounterAttack;
			}
		}

        iTotalControlPoint += iTotalBackAttack;
        iTotalControlPoint += iTotalAerialAttack;
    }

    if( g_kEveryTriggerManager.GetTotalStageCnt() > 0 )
        return ((int)(iTotalControlPoint/(float)g_kEveryTriggerManager.GetTotalStageCnt()) + DEFAULT_CONTROL_POINT);

    return (0 + DEFAULT_CONTROL_POINT);
}

int KQuestGameManager::GetAttackedPoint(void)
{ 
    int iTotalAttackedRank = 0;
    for(int i=0;i<g_kEveryTriggerManager.GetTotalStageCnt();++i)
    {
        int iTotalAttacked            = 0;
        for(int j=0;j<MAX_PLAYER_NUM;++j)
        {
			if( m_alQuestPoint[j].find( i ) != m_alQuestPoint[j].end() )
			{
				iTotalAttacked += m_alQuestPoint[j][i].Attacked;
			}
		}
        iTotalAttackedRank += iTotalAttacked;
    }

    int iPlayerCnt = 0;
    for(int i=0;i<MAX_PLAYER_NUM;++i)
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
            ++iPlayerCnt;
    }

    if( g_kEveryTriggerManager.GetTotalStageCnt()+iPlayerCnt > 0 )
        return (int)(iTotalAttackedRank/(float)(g_kEveryTriggerManager.GetTotalStageCnt()+iPlayerCnt));

    return 0;
}

int KQuestGameManager::GetTotalScorePoint(void)
{
 #if !defined( __PATH__ )
     if ( m_bPerfectMode == true )
         return 90;
     else
 #endif
         return GetPowerPoint() + GetControlPoint() - GetAttackedPoint();
}

int KQuestGameManager::GetTotalScoreStar(void)
{
    int iTemp = GetTotalScorePoint();

    int iNumOfStar = 1;
    if( iTemp >= RANK_STAR5 )
        iNumOfStar = 5;
    else if( iTemp >= RANK_STAR4 )
        iNumOfStar = 4;
    else if( iTemp >= RANK_STAR3 )
        iNumOfStar = 3;
    else if( iTemp >= RANK_STAR2 )
        iNumOfStar = 2;
    else if( iTemp >= RANK_STAR1 )
        iNumOfStar = 1;

    // 퀘스트 클리어 경우 최소 별1개, 클리어 실패시 무조건 별 0개
    if( g_pkQuestManager->m_bWin )
    {
        if( iNumOfStar <= 0 )
            iNumOfStar = 1;
    }
    else
    {
        iNumOfStar = 0;
    }

    return iNumOfStar;
}

void KQuestGameManager::CalcQuestPointByDamageType(int iPlayer, int iType)
{
	if( false == g_MyD3D->IsPlayerIndex( iPlayer )  )
		return;

    switch(iType)
    {
    case DT_DEFENCE:
        break;
    case DT_CRITICAL:
        m_PlayerQuestPoint[iPlayer].CriticalAttack++;
        break;
    case DT_DOUBLE:
        m_PlayerQuestPoint[iPlayer].DoubleAttack++;
        break;
    case DT_AERIAL:
        m_PlayerQuestPoint[iPlayer].AerialAttack++;
        break;
    case DT_BACKATTACK:
        m_PlayerQuestPoint[iPlayer].BackAttack++;
        g_MyD3D->MyPlayer[iPlayer]->AddControlInfoBackAttack();
        break;
    case DT_COUNTER:
        m_PlayerQuestPoint[iPlayer].CounterAttack++;
        break;
    }
}

void KQuestGameManager::LoadFromQuestPointList(int iStage, int iPlayer, SQuestPoint &sQuestPoint)
{
	if( iStage < 0 || g_kEveryTriggerManager.GetTotalStageCnt() <= iStage ) return;

	if( g_MyD3D->IsPlayerIndex( iPlayer ) && ( m_alQuestPoint[iPlayer].find( iStage ) != m_alQuestPoint[iPlayer].end()) )
		sQuestPoint = m_alQuestPoint[iPlayer][iStage];
}

void KQuestGameManager::SaveToQuestPointList(int iStage, int iPlayer, SQuestPoint &sQuestPoint)
{
	if( iStage < 0 || g_kEveryTriggerManager.GetTotalStageCnt() <= iStage ) return;

	if( g_MyD3D->IsPlayerIndex( iPlayer ) && ( m_alQuestPoint[iPlayer].find( iStage ) != m_alQuestPoint[iPlayer].end() ) )
		m_alQuestPoint[iPlayer][iStage] = sQuestPoint;
}

void KQuestGameManager::AddRouletteList(DWORD iUIDItem, DWORD iUIDPlayer)
{
    m_mapRouletteList[iUIDItem].push_back(iUIDPlayer);
}

void KQuestGameManager::CheckCompleteRouletteWaitList(void)
{
    // 룰렛 체크하는데 나간 사람이 있을지도 모르므로 없는 UID는 제거해버린다.
    std::vector<DWORD>::iterator itWait = m_vecRouletteWaitList.begin();
    for(;itWait != m_vecRouletteWaitList.end();)
    {
        bool bFindEqual = false;
        for(int i=0;i<MAX_PLAYER_NUM;++i)
        {
            if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
            {
                if( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == (*itWait) )
                {
                    bFindEqual = true;
                    break;
                }
            }
        }
        if( bFindEqual )
            ++itWait;
        else
            itWait = m_vecRouletteWaitList.erase(itWait);
    }

    if( 0 == m_vecRouletteWaitList.size() )
        m_bRouletteList = true;
}

void KQuestGameManager::CheckCompleteEndGameInfo(void)
{
    // 게임종료정보(랭크) 나간 사람이 있을지도 모르므로 없는 UID는 제거해버린다.
    std::vector<DWORD>::iterator itWait = m_vecEndGameInfo.begin();
    for(;itWait != m_vecEndGameInfo.end();)
    {
        bool bFindEqual = false;
        for(int i=0;i<MAX_PLAYER_NUM;++i)
        {
            if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
            {
                if( g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID == (*itWait) )
                {
                    bFindEqual = true;
                    break;
                }
            }
        }
        if( bFindEqual )
            ++itWait;
        else
            itWait = g_MyD3D->m_pkQuestGameMgr->m_vecEndGameInfo.erase(itWait);            
    }

    if( 0 == m_vecEndGameInfo.size() )
        m_bGetEndGameInfo = true;
}

void KQuestGameManager::GateMoveGateCheck(void)
{
    // Jaeho 2007.12.12
    // GC_IRON_DOOR 전용 체크 함수다... 사이즈가 다른 문이 추가되면 수정해줘야함...
    // 후에 더 좋은 방법을 택하고... 지금은 시간이 너무 없다 -ㅁ-;;
    // 미안... 꼭 고쳐줄게 ㅡ.ㅜ..

    if( g_kEveryTriggerManager.GetRemainNextDelay() ) return;

    bool bGoNextStage = false;
    for(int i=0;i<MAX_OBJECT;++i)
    {
        if( g_MyD3D->m_pObject[i] )
        {
            if( GC_PORTAL == g_MyD3D->m_pObject[i]->GetObjType() )
            {
                KGCDoor *pDoor = static_cast<KGCDoor*>(g_MyD3D->m_pObject[i]);
                if( KGCDoor::IDS_OPEN == pDoor->GetDoorState() )
                {
                    if( pDoor->GetGateDelay() >= 0 )
                    {
                        MoveGate(pDoor->GetDirection(), pDoor->GetGateDelay());
                        pDoor->SetGateDelay(-1);
                        bGoNextStage = true;
                        break;
                    }
                }
            }
        }
    }

    if( bGoNextStage )
    {
        for(int i=0;i<MAX_OBJECT;++i)
        {
            if( g_MyD3D->m_pObject[i] )
                g_MyD3D->m_pObject[i]->EnableLive(false);
        }
    }
}

bool KQuestGameManager::IsCompleteEndGameDelayTime(void)
{
    if( m_dwEndGameDelay > 0 )
    {
        --m_dwEndGameDelay;
        return false;
    }

    return true;
}

bool KQuestGameManager::IsStartedStage(void)
{
    if( g_pkGameOverlayUI->IsShowQuestTitle() )
        return false;

    return true;
}

void KQuestGameManager::FrameMove_Dialogue(void)
{
	
	if(g_pkGameOverlayUI->IsShowDialogue())
	{
		// 다이얼로그 대화 넘기는 부분
		if( g_kGlobalValue.m_kUserInfo.bHost)
		{
			SDialogue *pDialogue = GetDialogue( GetCurDialogeIndex() );
			if( pDialogue )
			{
				KGC_PID_DIALOGUE_PROGRESS packet;
                packet.bShow = true;

				if(!m_bSkipping)
				{

#ifdef USE_JOYSTICK
                    int iKey = g_MyD3D->GetJoystickKey();
#endif
					// X, Z, 혹은 다이얼로그 대기시간을 초과했으면 다음장으로 넘김.
                    DWORD dwDlgDelayTime = pDialogue->dwDelayTime;

					if(g_pkInput->IsUp(DIK_X) || g_pkInput->IsUp(DIK_Z) || ((timeGetTime() - m_dwDialogueDelay) > dwDlgDelayTime)
#ifdef USE_JOYSTICK
                        || iKey == EJK_KEY_4 || iKey == EJK_KEY_START
#endif
                        )
					{
						// 스크립트에 설정한 다이얼로그 딜레이 시간이 초과 되었나 확인. //드라마 플레이 중에는 못넘긴다.
                        // 모든 플레이어가 스테이지 로드가 끝나있어야 키 스킵이나 다음장 넘김 가능
                        auto timeDelay = (timeGetTime() - m_dwDialogueDelay);
                        if(timeDelay > pDialogue->dwTime && IsStageLoadComplete() == true)
						{
							
							packet.m_sTG_ID     = m_iDialogueTG_ID;
							
							if( g_pkInput->IsUp(DIK_X)
#ifdef USE_JOYSTICK
                                || iKey == EJK_KEY_START
#endif
                                )
							{
								if(!m_bDramaStage)
								{
									SetShowDialogue( false );
									packet.m_sIndex     = -1;
                                    packet.bShow        = false;
								}
								else
								{
									
									packet.m_sIndex     = GetCurDialogeIndex();				
									packet.bSkipping = true;
                                    packet.bShow = true;
									g_kTGPacketPacker.Pack(&packet, sizeof(KGC_PID_DIALOGUE_PROGRESS));
									SendNetworkData(GC_PID_TG_DEFAULT, 0, 0);
									SkipDialogue();	
									return;
								}
							}
							else  if(m_pkDrama->IsEnd())
							{
								packet.m_sIndex     = GetCurDialogeIndex()+1;
								NextDialogue();
							}
                            else
                            {
                                packet.m_sIndex     = GetCurDialogeIndex();
                            }

							g_kTGPacketPacker.Pack(&packet, sizeof(KGC_PID_DIALOGUE_PROGRESS));
							m_dwDialogueDelay = timeGetTime();
						}
					}
				}
				
			}		
		}
		
		if(m_bSkipping)
		{
			if(m_pkDrama->IsEnd())
				NextDialogue();
		}			
		
	}

    m_pkDrama->FrameMove();
}

void KQuestGameManager::FrameMove_Msg(void)
{
    if( m_pkDrama->IsEnd() && !g_pkGameOverlayUI->IsShowDialogue() )
	{
		if( false == m_bShowStartMsg )
		{
#if !defined( NATION_PHILIPPINE ) && !defined(CLOSED_BETA) && !defined( NATION_LATINAMERICA )
			// 헬모드 일때만 띄워주는 메시지
            if( (GC_DUNGEON_LEVEL_HELL == SiKGCRoomManager()->GetDungeonLevel() || GC_DUNGEON_LEVEL_BREAK == SiKGCRoomManager()->GetDungeonLevel())
                 && (m_dwPlayFrame > 200) )
			{
				g_pkUIScene->m_pkGameOverlayUI->SetTitleText( g_pkStrLoader->GetString( STR_ID_HELL_MODE_TITLE_ITEM_MSG ) );
				m_bShowStartMsg = true;
			}
#endif

            if( g_kGlobalValue.IsEscortEventActive() && (m_dwPlayFrame > 200) )
            {
                std::vector<int> &strEscort = g_kGlobalValue.GetEscortString();

                for( UINT i = 0; i < strEscort.size(); ++i ){
                    g_pkUIScene->m_pkGameOverlayUI->SetTitleText( g_pkStrLoader->GetString( strEscort[i] ) );
                }

                m_bShowStartMsg = true;
            }
		}
	}

	m_dwPlayFrame++;
}

void KQuestGameManager::StartWaterPole(CTGAddDamage tgWaterPole)
{
    m_vtCurWaterPole.push_back( tgWaterPole );
    m_vtWaterPoleTime.push_back( timeGetTime() + tgWaterPole.m_iContinueance );	
    g_KDSound.Play( "101" );
}

void KQuestGameManager::FrameMove_WaterPole(void)
{	

    if( !o5 )
    {
        for( int i = (int)m_vtWaterPoleTime.size() - 1; i >= 0; --i )
        {
            if( m_vtWaterPoleTime[i] > timeGetTime() )
            {				
				CDamageInstance *pDamage =NULL;                				
				
				if(DT_WATER_POLE == m_vtCurWaterPole[i].m_iDamageID)
				{										
					pDamage = g_MyD3D->m_pDamageManager->AddWithLocate(DT_WATER_POLE, MAX_PLAYER_NUM,m_vtCurWaterPole[i].m_iEntranceX*0.0025f ,m_vtCurWaterPole[i].m_iEntranceY*0.0025f, false, 0);
				}
                else if(DT_FIRE_POLE==m_vtCurWaterPole[i].m_iDamageID)
				{
					pDamage = g_MyD3D->m_pDamageManager->AddWithLocate(DT_FIRE_POLE, MAX_PLAYER_NUM,m_vtCurWaterPole[i].m_iEntranceX*0.0025f ,m_vtCurWaterPole[i].m_iEntranceY*0.0025f, false, 0);
				}						

                if( pDamage )
                {
                    pDamage->m_Life                                     = m_vtCurWaterPole[i].m_iLife;					
                    pDamage->m_pOriginalDamage->m_Start_Life            = m_vtCurWaterPole[i].m_iLife;
                    pDamage->m_pOriginalDamage->m_iCrashStart           = m_vtCurWaterPole[i].m_iLife;

                    for(int j=0;j<(int)pDamage->m_pOriginalDamage->m_vecChildParticle.size();j++)
                    {
                        pDamage->m_pOriginalDamage->m_vecChildParticle[j].m_iStartLife = pDamage->m_Life;						
                    }
                }
            }
            else
            {
                m_vtWaterPoleTime.erase( m_vtWaterPoleTime.begin() + i );
                m_vtCurWaterPole.erase( m_vtCurWaterPole.begin() + i );				
            }
        }
    }
}

std::set<int> KQuestGameManager::GetClearStageSet()
{
	std::set<int> setClearList;
	std::map< std::pair<int, int>, bool >::iterator itSH = m_mapStageHistory.begin();
	for(;itSH != m_mapStageHistory.end();++itSH)
	{
		if( itSH->second )
        {
			TileMap* pTileMap = m_Quest.GetTileMap(itSH->first.first, itSH->first.second);

            if(pTileMap)
                setClearList.insert( pTileMap->iStage );
        }
	}

	return setClearList;	
}

int KQuestGameManager::GetTotalStageNum()
{
	//return (int)m_Quest.vtStage.size();
	return m_iMaxStage;
}

bool KQuestGameManager::IsPlatformer()
{
    TileMap* pTileMap = m_Quest.GetTileMap(m_iCurTileY,m_iCurTileX);

	if( GC_GMC_DUNGEON == SiKGCRoomManager()->GetGameModeCategory() && pTileMap)
		return (pTileMap->iFlag==4);

	return false;
}

bool KQuestGameManager::IsEndDialogList( std::string strName )
{
    for(int i=0;i<(int)m_vecEndDialogList.size();++i)
    {
        if( m_vecEndDialogList[i] == strName )
            return true;
    }

    return false;
}

void KQuestGameManager::AddEndDialogList( std::string strName )
{
    m_vecEndDialogList.push_back( strName );
}

void KQuestGameManager::ClearEndDialogList(void)
{
    m_vecEndDialogList.clear();
}

void KQuestGameManager::IncreaseAttackPoint(int iPlayer)
{ 
	if( g_MyD3D->IsPlayerIndex( iPlayer ) )
		m_PlayerQuestPoint[iPlayer].Attack++; 
}

void KQuestGameManager::IncreaseAttackedPoint(int iPlayer)
{ 
	if( g_MyD3D->IsPlayerIndex( iPlayer ) )
		m_PlayerQuestPoint[iPlayer].Attacked++; 
}

void KQuestGameManager::IncreaseSpecial1Point(int iPlayer)
{ 
	if( g_MyD3D->IsPlayerIndex( iPlayer ) )
		m_PlayerQuestPoint[iPlayer].iSpecial1++; 
}

void KQuestGameManager::IncreaseSpecial2Point(int iPlayer)
{ 
	if( g_MyD3D->IsPlayerIndex( iPlayer ) )
		m_PlayerQuestPoint[iPlayer].iSpecial2++; 
}

void KQuestGameManager::IncreaseSpecial3Point(int iPlayer)
{ 
	if( g_MyD3D->IsPlayerIndex( iPlayer ) )
		m_PlayerQuestPoint[iPlayer].iSpecial3++; 
}

void KQuestGameManager::SkipDialogue()
{
	m_bSkipping=true;
	KGCGameModeInterface* pQuest = g_pStateGame->GetCurGameModePtr();
	
	
	
	while(g_pkGameOverlayUI->IsShowDialogue())
	{	
		pQuest->FrameMove(GC_ELAPSED_TIME);		
	}

	m_bSkipping=false;

    // 보스 사망 이펙트 따위한방에 없애 -_-;;

	for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
			continue;
		MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

		if(pMonster == NULL) continue;

		if ( pMonster->m_iBossMonster == MONSTER::MON_BOSS )
		{
			pMonster->EndBlur();
		}
	}
}

void KQuestGameManager::CheckHackAction()
{	
	//몬스터의 hp변화나 죽음체크등은 호스트만 하기 때문에 호스트만 체크하겠다.
	if(!g_kGlobalValue.m_kUserInfo.bHost)
		return;

	if( m_bMonLoadComplete == false || m_iMonsterNum < 3)
		return;

	int iCurMonsterNum = m_iMonsterNum;
	//로딩중에 이 안으로 들어올 일은 없지만 이상현상을 발견해서 혹시나 있을 오작동을 예방하기 위해서 bool체크를 함
	bool bCheck = false; 

	for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
			continue;
		MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

		if(pMonster == NULL) continue;

		if( pMonster->m_fHP == 0.0f && pMonster->m_fOldHP > 0.0f )
		{
			iCurMonsterNum--; //현재 살아 있는 몬
			bCheck = true; 
		}
	}

	// 몬스터 전부가 아무 타격도 입지 않고 다 같이 죽어버리는 경우만 체크한다.
	if( iCurMonsterNum == 0 && GetAttackedMonster() == false && bCheck)
	{
		//해킹이 감지되었다고 본다.
		SetHackedDungeon(true);
		m_bMonLoadComplete = false;
	}
}

void KQuestGameManager::InitHackingCheckData()
{
	m_bMonLoadComplete	= false;
	m_bBossStage = false;
	m_iMonsterNum		= 0;
	m_iCurMonsterNum	= 0;
	SetHackedDungeon(false);
	SetMonsterHPHack(false);
	SetAttackedMonster(false);
}

void KQuestGameManager::SetCurIncAtkRatio()
{
	for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
	{
		if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == true )
		{
			//스테이지의 마지막 몬스터가 죽을 때의 공격력 증가비율을 저장한다.
			m_vecIncAtkRatio[i].push_back( g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_ATTACK_PERCENT] );
		}
		else
		{
			m_vecIncAtkRatio[i].push_back(0.0f);
		}
	}
}

void KQuestGameManager::CountComboBreak()
{
    m_iComboBreakCount++;
}

bool KQuestGameManager::IsAllDieMonsterToStageClearRank()
{
    bool bAllDie = true;

	for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
			continue;
		MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

		if(pMonster == NULL) continue;

        const SMonsterTemplate &kMonsterTemplate = g_kMonsterManager.m_vecMonsterTable[ pMonster->m_iMonsterType ];
        if( kMonsterTemplate.bNoCheckStageRank )
            continue;

		if( pMonster->IsDie() == false )
		{
			bAllDie = false;
			break;
		}
	}

    return bAllDie;
}

bool KQuestGameManager::CheckMonsterExistInScreen()
{
	m_bExistMonsterInScreen = false;

	for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
			continue;
		MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

		if(pMonster == NULL) continue;
		
		if( pMonster->IsInScreen() )
		{
			m_bExistMonsterInScreen = true;			
			return true;
		}
	}

	return false;
}

void KQuestGameManager::InitAllPlayerComboState()
{
    for( int i = 0; i < MAX_PLAYER_NUM; i++ )
        g_MyD3D->MyPlayer[i]->SetComboState(false);
}

bool KQuestGameManager::CheckPortalInScreen()
{
    m_bPortalInScreen = false;

    if( KGCPortal::IsPortalInScreen() )
        m_bPortalInScreen = true;

    return m_bPortalInScreen;
}

void KQuestGameManager::ClearDungeonKillMon( void )
{
    std::vector<CTriggerCondition*> vtMonGenTrigger;
    std::vector<CTriggerCondition*> vtWinTrigger;

    for(int i=0;i<m_Quest.iTileY;++i)
    {
        for(int j=0;j<m_Quest.iTileX;++j)
        {
            TileMap* pTileMap = m_Quest.GetTileMap(i,j);

            if( (int)m_Quest.vtStage.size() <= pTileMap->iStage || pTileMap->iStage < 0 ) continue;
            if ( NULL != pTileMap )
            {
                g_kEveryTriggerManager.GetTriggerConditionByType( pTileMap->iStage, CTriggerCondition::TG_MON_GEN, vtMonGenTrigger );
                m_mapStageHistory[ std::make_pair(pTileMap->iRow, pTileMap->iCol) ] = true;
            }
        }
    }

    for(int i=0;i<(int)vtMonGenTrigger.size();++i)
    {
        CTGMonsterGenerater* pMonGen = (CTGMonsterGenerater*)vtMonGenTrigger[i];
        pMonGen->SetComplete(true);
        g_kGlobalValue.m_kUserMission.VirtualKillMonster( pMonGen->m_iMonType );
        g_pGameBoard->AddMonKillNum( pMonGen->m_iMonType, 1, pMonGen->m_iTG_ID );
    }
}

bool KQuestGameManager::IsFallingDie()
{
    // 영웅던전...엘리아꺼는.. 무조건 플랫폼 속성주쟈.. 떨어진다
    if ( SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST46 )
        return true;
    
    return false;
}

bool KQuestGameManager::IsExistMonsterTagetRefresh( KGC_PID_MONSTER_TARGET_REFRESH* kMonsterTagetRefresh_ )
{
    std::vector<int>::iterator vit = m_mapMonsterRefreshTaget.begin();

    if( vit != m_mapMonsterRefreshTaget.end() )
    {
        kMonsterTagetRefresh_->iMonsterIndex = (*vit);
        m_mapMonsterRefreshTaget.erase(vit);
        return true;
    }

    return false;
}

int KQuestGameManager::GetPlayerAveLv( void )
{
    if ( m_ucPlayerNum > 0 ) 
    {
        float aveLv = static_cast<float>(m_iPlayersAveLv / m_ucPlayerNum);
        return static_cast<int>(round( aveLv ));
    }
    else
        return 0;
}

void KQuestGameManager::CalPlayerInfo( void )
{
    // 게임 시작할 때 있었던 아이들의 평균 공,방,생을 구해놓쟈 ㅎ 
    m_iPlayersAveLv = 0;
    m_ucPlayerNum = 0;
    m_iHeroDungeonLevel = 0;

    std::vector<int> vecTmp;

    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == true )
        {
            m_iPlayersAveLv += g_MyD3D->MyPlayer[i]->GetCurrentChar().iLevel;
            vecTmp.push_back( g_MyD3D->MyPlayer[i]->GetCurrentChar().iLevel );
            ++m_ucPlayerNum;
        }
    }

    {
        //////////////////////////////////////////////////////////////////////////
        // 영웅던전 난이도 설정..이랍니다.
        /* 수식에 적용되는 캐릭터 레벨 = 던전 플레이 결정된 캐릭터(게임 Start)

        난이도 산정값	Ai 수준
        40 ~ 45	        보통
        46 ~ 55	        어려움
        56 ~ 75	        매우어려움


        최고레벨 = 파티원들 중 가장 높은 레벨
        최저레벨 = 파티원들 중 가장 낮은 레벨
        파티원 A, B = 나머지 떨거지 2명 --;

        {(최고레벨x3 - 최저레벨) + 파티원 A Lv + 파티원B Lv }/ 파티원 수*/

        int tmp = 0;
        if ( m_ucPlayerNum == 1 )
        {
            tmp = vecTmp[0];
        }
        else
        {
            std::sort( vecTmp.begin(), vecTmp.end(), std::greater<int>() );
            tmp = (*vecTmp.begin()) * 3;
            vecTmp.erase( vecTmp.begin() );

            tmp -= (*vecTmp.rbegin());
            vecTmp.erase( vecTmp.end() - 1 );

            std::vector<int>::iterator vit = vecTmp.begin();
            while ( vit != vecTmp.end() )
            {
                tmp += (*vit);
                ++vit;
            }

            tmp = static_cast<int>(tmp / m_ucPlayerNum);
        }

        if ( tmp <= 45 )
        {
            m_iHeroDungeonLevel = 0;
        }
        else if ( tmp > 45 && tmp <= 55 )
        {
            m_iHeroDungeonLevel = 1;
        }
        else
        {
            m_iHeroDungeonLevel = 2;
        }

    }
    //////////////////////////////////////////////////////////////////////////
}

bool KQuestGameManager::IsNextSceneKeyUp()
{
    if ( g_pkInput->IsUp( DIK_Z ) )
        return true;
    else
        return false;
}

void KQuestGameManager::SetTotalDamagePoint_S6( float fPoint_, float fMaxHP_, bool bComboState , int iIndex )
{
    if ( m_bRankPortalCheck 
        || g_pkQuestManager->IsPlatformer() 
        || g_pkQuestManager->IsAllDieMonsterToStageClearRank() 
        || ( g_MyD3D->Get_MyPlayer() != iIndex && iIndex != -1) 
        || NotDecreaseCheck()
        )
        return; 

    float fRankPoint = fPoint_;
    float fDungeonMaxScore = g_pkQuestManager->GetDungeonRankInfo( SiKGCRoomManager()->GetGameMode() , SiKGCRoomManager()->GetDungeonLevel(), 0 );

    if ( fRankPoint > fMaxHP_ )
        fRankPoint = fMaxHP_;

    //최소 최대값 보정
    if ( m_sQuestRankInfoS6.fRankPoint > fDungeonMaxScore ) {
        m_sQuestRankInfoS6.fRankPoint = fDungeonMaxScore;
    }
    else if ( m_sQuestRankInfoS6.fRankPoint < 0 )
        m_sQuestRankInfoS6.fRankPoint = 0;

    m_sQuestRankInfoS6.fRankPoint += fRankPoint;

    if ( bComboState )
        m_sQuestRankInfoS6.m_ComboDamage += static_cast<int>(fRankPoint);
    else if( m_bComboBreak )
    {
        if ( m_sQuestRankInfoS6.m_ComboDamage > m_sQuestRankInfoS6.m_MaxComboDamage) {
            m_sQuestRankInfoS6.m_MaxComboDamage = m_sQuestRankInfoS6.m_ComboDamage;
        }
        m_sQuestRankInfoS6.m_ComboDamage = 0;
        m_bComboBreak = false;
    }

    g_pkGameOverlayUI->SetPlayerRankInfo( m_sQuestRankInfoS6.fRankPoint );
}

void KQuestGameManager::RankPointCalcState( bool bGoing )
{
    m_bRankPortalCheck = bGoing;
}

void KQuestGameManager::CalcMaxCombo( int iCombo )
{
    if ( iCombo > m_sQuestRankInfoS6.m_MaxCombo )
         m_sQuestRankInfoS6.m_MaxCombo = iCombo;
}

void KQuestGameManager::CalcDamageFromMonster( float fDamage , float fMaxHP_)
{
    float fPoint = fDamage;
    if ( fPoint > fMaxHP_ )
        fPoint = fMaxHP_;

    m_sQuestRankInfoS6.m_TotlaDamageFromMonster += static_cast<int>(fPoint);
}

void KQuestGameManager::GetDungeonDetailInfo(OUT int& iMaxCombo, OUT int& iMaxComboDamage, OUT int& iContributionPercent, 
                                             OUT int& iTotalDamageFromMon , OUT int& iTotalBackAerial , OUT int& iMaxKillCombo )
{
    float fDungeonMaxScore = g_pkQuestManager->GetDungeonRankInfo( SiKGCRoomManager()->GetGameMode() , SiKGCRoomManager()->GetDungeonLevel(), 0 );

    iMaxCombo = m_sQuestRankInfoS6.m_MaxCombo;
    if ( m_sQuestRankInfoS6.m_MaxComboDamage <=0  )
        iMaxComboDamage = m_sQuestRankInfoS6.m_ComboDamage;
    else
        iMaxComboDamage = m_sQuestRankInfoS6.m_MaxComboDamage;

    //던전 데미지 기여도
    float fPercent = 0.f; 
    float fSum = 0.f;

    for (int i =0; i < MAX_PLAYER_NUM ; i ++)
    {
        fSum += m_PlayerDamage[i];
    }

    int iMyIndex = g_MyD3D->Get_MyPlayer();
    
    if( iMyIndex >= 0 && iMyIndex < MAX_PLAYER_NUM && fSum != 0)
        fPercent = ( m_PlayerDamage[iMyIndex] / fSum ) * 100.f;
    else if ( fSum == 0 )
        fPercent = -0.f;

    if ( fPercent > 100.f)
        fPercent  = 100.f;
    else if (fPercent < 0 )
        fPercent  = 0.f;
    //던전 데미지 기여도 % 구하기 끝
    
    iContributionPercent = static_cast<int>(fPercent);

    iTotalDamageFromMon = m_sQuestRankInfoS6.m_TotlaDamageFromMonster;
    iTotalBackAerial = m_sQuestRankInfoS6.m_TotalBackPlusAerialAttack;
    
    if ( m_sQuestRankInfoS6.m_MaxKillCombo <= 0 )
        iMaxKillCombo = m_sQuestRankInfoS6.m_KillCombo;
    else
        iMaxKillCombo = m_sQuestRankInfoS6.m_MaxKillCombo;
}

void KQuestGameManager::ClearDungeonDetailInfo()
{
    m_sQuestRankInfoS6.Init();
    
    float fDungeonMaxScore = g_pkQuestManager->GetDungeonRankInfo( SiKGCRoomManager()->GetGameMode() , SiKGCRoomManager()->GetDungeonLevel(), 0 );
    m_sQuestRankInfoS6.fRankPoint =  fDungeonMaxScore * 0.2132855f;
    g_pkGameOverlayUI->SetPlayerRankInfo( m_sQuestRankInfoS6.fRankPoint );

    m_vecPlayerRanking.clear();
    int iRankIndex = GetRankIndex( m_sQuestRankInfoS6.fRankPoint );
    for ( int i =0; i < MAX_PLAYER_NUM ; i++)  {
        m_PlayerRankPoint[i] = iRankIndex;
        m_PlayerDamage[i] = 0.f;
    }
    m_iQuestStartUserCnt = 0;
    m_bFirstStartUserRecord = true;
}

void KQuestGameManager::CalcBackArielCount(int iPlayer, int iType)
{
    PLAYER* pPlayer = g_MyD3D->GetMyPlayer();
    if ( pPlayer->m_iPlayerIndex != iPlayer)
        return;

    if( false == g_MyD3D->IsPlayerIndex( iPlayer )  )
        return;

    if ( iType == DT_AERIAL || iType == DT_BACKATTACK )
    {
        m_sQuestRankInfoS6.m_TotalBackPlusAerialAttack++;
    }
}

void KQuestGameManager::ComboBreakCheck( bool bComboBreak )
{
    m_bComboBreak = bComboBreak;
}

void KQuestGameManager::SetAttackTypeState( int iState)
{
    m_iState = iState;
}

void KQuestGameManager::GetComboDamage(OUT int& iComboDamage )
{
    iComboDamage = m_sQuestRankInfoS6.m_ComboDamage;
}

void KQuestGameManager::SetComboKillPoint( bool bClear)
{
    if ( bClear )
    {
        if ( m_sQuestRankInfoS6.m_KillCombo > m_sQuestRankInfoS6.m_MaxKillCombo)
            m_sQuestRankInfoS6.m_MaxKillCombo = m_sQuestRankInfoS6.m_KillCombo;

        if( !SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) )
        {
            m_sQuestRankInfoS6.m_KillCombo = 0;
        }
    }
    else
    {
        m_sQuestRankInfoS6.m_KillCombo++;
    }
}


void KQuestGameManager::GetComboKillPoint(OUT int& iKillPoint )
{
    iKillPoint = m_sQuestRankInfoS6.m_KillCombo;
}

void KQuestGameManager::SetComboBossKillPoint()
{
    m_sQuestRankInfoS6.m_BossKillCombo++;
}

void KQuestGameManager::GetComboBossKillPoint( OUT int& iBossKillPoint )
{
    iBossKillPoint = m_sQuestRankInfoS6.m_BossKillCombo;
}

int KQuestGameManager::GetRankNum()
{
    int iIndex = GetRankIndex( m_sQuestRankInfoS6.fRankPoint );

    if (iIndex >= RANK_GRADE_NUM)
        iIndex = RANK_GRADE_NUM - 1;

    return iIndex;
}

void KQuestGameManager::CalcPlayerRank()
{
    LTP_PLAYER_QUEST_RANK kPacket;
    kPacket.iPlayerIndex =  g_MyD3D->Get_MyPlayer();
    kPacket.iRankGrade = GetRankNum();

    SetPlayerRank( &kPacket);
    g_LatencyTransfer.PushPacket( &kPacket, 0 );
}


void KQuestGameManager::SetPlayerRank( LTP_PLAYER_QUEST_RANK* kPacket)
{
    if (kPacket->iPlayerIndex < MAX_PLAYER_NUM) 
    {
        m_PlayerRankPoint[kPacket->iPlayerIndex] = kPacket->iRankGrade;

        g_pkGameOverlayUI->UpdatePlayerRankInfo();
    }
}

bool KQuestGameManager::GetPlayerRank( IN int iIndex , OUT std::wstring& wstrNickName, OUT int& iRankGrade )
{
    if( !g_MyD3D->IsPlayerIndex( iIndex ) )
        return false;

    wstrNickName = g_MyD3D->MyPlayer[iIndex]->m_kUserInfo.strNickName;
    iRankGrade = m_PlayerRankPoint[iIndex];

    return true;
}

int KQuestGameManager::GetPlayerRank( IN int iIndex )
{
    if( !g_MyD3D->IsPlayerIndex( iIndex ) )
        return 0;

    return m_PlayerRankPoint[iIndex];;
}

void KQuestGameManager::GetPlyaerIdxForRanking( IN int iRanking, OUT int& iPlayerIdx )
{
    if ( m_vecPlayerRanking.empty() ) {
        iPlayerIdx = -1;
        return;
    }

    if ( iRanking >= 0 && iRanking < MAX_PLAYER_NUM )
        iPlayerIdx =  m_vecPlayerRanking[iRanking].second;
    else
        iPlayerIdx = -1;
}

float KQuestGameManager::GetDungeonRankInfo( EGCGameMode eGameMode , int iLevel, int iDataType)
{
    QuestFileInfo kQuestFileInfo = g_MyD3D->m_pkQuestGameMgr->GetQuestFileInfoFromGameMode( eGameMode, iLevel );

    if ( iDataType == TOTAL_RANK )
        return kQuestFileInfo.fRankTotalScore;
    else if ( iDataType == RANK_DECREASE_NUM )
        return kQuestFileInfo.fDungeonDecreaseNum;
    else if ( iDataType == RANK_HITTED_BY_MON )
        return kQuestFileInfo.fHittedByMonNum;

    return -1;
}

int KQuestGameManager::GetRankIndex( float fRankPoint)
{
    int iIndex = 0;
    float fDungeonMaxScore = g_pkQuestManager->GetDungeonRankInfo( SiKGCRoomManager()->GetGameMode() , SiKGCRoomManager()->GetDungeonLevel(), 0 );
    float iPercent = (fRankPoint / fDungeonMaxScore ) * 100.f;

    if ( iPercent < 4.191521f )
        return iIndex = 0;
    else if ( iPercent > 4.191521f && iPercent < 9.640497f )
        return iIndex = 1;
    else if ( iPercent > 9.640497f && iPercent < 16.72417f )
        return iIndex = 2;
    else if ( iPercent > 16.72417f && iPercent < 25.93294f )
        return iIndex = 3;
    else if ( iPercent > 25.93294f && iPercent < 37.90434f )
        return iIndex = 4;
    else if ( iPercent > 37.90434f && iPercent < 53.46716f )
        return iIndex = 5;
    else if ( iPercent > 53.46716f && iPercent < 73.69883f )
        return iIndex = 6;
    else if ( iPercent > 73.69883f )
        return iIndex = 7;

    return 0;
}

void KQuestGameManager::GetRankScroeTermPercent(IN int iIndex, IN float fRankPoint , OUT float& fPercent )
{
    float fDungeonMaxScore = g_pkQuestManager->GetDungeonRankInfo( SiKGCRoomManager()->GetGameMode() , SiKGCRoomManager()->GetDungeonLevel(), 0 );
    float fBelowValue = 0;
    float fMaxValue = 0;
    
    switch(iIndex)
    {
    case 0:
        {
            fBelowValue = 0;
            fMaxValue = fDungeonMaxScore * 0.04191521f;
        }
        break;
    case 1:
        {
            fBelowValue = fDungeonMaxScore * 0.04191521f;
            fMaxValue = fDungeonMaxScore * 0.09640497f;
        }
        break;
    case 2:
        {
            fBelowValue = fDungeonMaxScore * 0.09640497f;
            fMaxValue = fDungeonMaxScore * 0.1672417f;
        }
        break;
    case 3:
        {
            fBelowValue = fDungeonMaxScore * 0.1672417f;
            fMaxValue = fDungeonMaxScore * 0.2593294f;
        }
        break;
    case 4:
        {
            fBelowValue = fDungeonMaxScore * 0.2593294f;
            fMaxValue = fDungeonMaxScore * 0.3790434f;
        }
        break;
    case 5:
        {
            fBelowValue = fDungeonMaxScore * 0.3790434f;
            fMaxValue = fDungeonMaxScore * 0.5346716f;
        }
        break;
    case 6:
        {
            fBelowValue = fDungeonMaxScore * 0.5346716f;
            fMaxValue = fDungeonMaxScore * 0.7369883f;
        }
        break;
    case 7:
        {
            fBelowValue = fDungeonMaxScore * 0.7369883f;
            fMaxValue = fDungeonMaxScore * 1.f;
        }
        break;
    }

    float fRankTermPoint = fMaxValue - fBelowValue;
    float fMyTermPoint = fRankPoint - fBelowValue;

    fPercent =  fMyTermPoint /fRankTermPoint;
}

bool KQuestGameManager::LoadRankCorrectionScript()
{
    m_vecRankCorrectionValue_Attack.clear();
    m_vecRankCorrectionValue_Decrease.clear();
    m_vecRankGradeRatioValue.clear();
    m_vecRankCorrectionValue_AttackInInfinite.clear();
    m_vecRankCorrectionValue_DecreaseInInfinite.clear();

    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr, "RankCorrectionValue.stg" ) == false )
        return false;

    LUA_BEGIN_TABLE("RankCorrectionValue",return false)
    {
        LUA_BEGIN_TABLE("ATTACK_VALUE",return false)
        {                    
            LUA_BEGIN_TABLE_RAII("VALUE")
            {
                int iLoop = 1; float fTemp;
                while( SUCCEEDED( luaMgr.GetValue( iLoop++, fTemp ) ) )
                {
                    m_vecRankCorrectionValue_Attack.push_back( fTemp );
                }
            }
        }
        LUA_END_TABLE(return false)

        LUA_BEGIN_TABLE("DECREASE_VALUE",return false)
        {                    
            LUA_BEGIN_TABLE_RAII("VALUE")
            {
                int iLoop = 1; float fTemp;
                while( SUCCEEDED( luaMgr.GetValue( iLoop++, fTemp ) ) )
                {
                    m_vecRankCorrectionValue_Decrease.push_back( fTemp );
                }   
            }
        }
        LUA_END_TABLE(return false)

        LUA_BEGIN_TABLE("RANK_GRADE_RATIO",return false)
        {                    
            LUA_BEGIN_TABLE_RAII("VALUE")
            {
                int iLoop = 1; float fTemp;
                while( SUCCEEDED( luaMgr.GetValue( iLoop++, fTemp ) ) )
                {
                    m_vecRankGradeRatioValue.push_back( fTemp );
                }
            }
        }
        LUA_END_TABLE(return false)

        LUA_BEGIN_TABLE("ATTACK_VALUE_IN_INFINITE_DUN",return false)
        {                    
            LUA_BEGIN_TABLE_RAII("VALUE")
            {
                int iLoop = 1; float fTemp;
                while( SUCCEEDED( luaMgr.GetValue( iLoop++, fTemp ) ) )
                {
                    m_vecRankCorrectionValue_AttackInInfinite.push_back( fTemp );
                }
            }
        }
        LUA_END_TABLE(return false)

            
        LUA_BEGIN_TABLE("DECREASE_VALUE_IN_INFINITE_DUN",return false)
        {                    
            LUA_BEGIN_TABLE_RAII("VALUE")
            {
                int iLoop = 1; float fTemp;
                while( SUCCEEDED( luaMgr.GetValue( iLoop++, fTemp ) ) )
                {
                    m_vecRankCorrectionValue_DecreaseInInfinite.push_back( fTemp );
                }
            }
        }
        LUA_END_TABLE(return false)

    }
    LUA_END_TABLE(return false)

    return true;
}

void KQuestGameManager::GetRankCorrectionValue(IN int iPlayerCnt , IN int iType, OUT float& fCorrectionValue )
{
    if ( iPlayerCnt < 1 )
        return;

    int iTemp = iPlayerCnt -1;
    if ( iTemp < 0 )
        iTemp = 0;

    if (iType == 0)
    {
        if ( iTemp > static_cast<int> (m_vecRankCorrectionValue_Attack.size() ) )
            fCorrectionValue = 0.f;
        else
            fCorrectionValue = m_vecRankCorrectionValue_Attack[iTemp];
    }
    else if ( iType == 1)
    {
        if ( iTemp > static_cast<int> (m_vecRankCorrectionValue_Decrease.size()) )
            fCorrectionValue = 0.f;
        else
            fCorrectionValue = m_vecRankCorrectionValue_Decrease[iTemp];
    }
    else if (iType ==2 )
    {
        if ( iTemp > static_cast<int> (m_vecRankCorrectionValue_AttackInInfinite.size()) )
            fCorrectionValue = 0.f;
        else
            fCorrectionValue = m_vecRankCorrectionValue_AttackInInfinite[iTemp];
    }
    else if (iType ==3 )
    {
        if ( iTemp > static_cast<int> (m_vecRankCorrectionValue_DecreaseInInfinite.size()) )
            fCorrectionValue = 0.f;
        else
            fCorrectionValue = m_vecRankCorrectionValue_DecreaseInInfinite[iTemp];
    }
}

int KQuestGameManager::GetPartyAvgRank()
{
    int nSum = 0;
    int nCount = 0;
    for ( int i = 0; i < MAX_PLAYER_NUM; ++i ) 
    {
        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
        {
            nSum += m_PlayerRankPoint[i];
            nCount++;
        }
    }

    if ( nCount == 0 ) 
        return 0;

    // 올림 처리 한다.
    int nAvg = ( nSum + ( nCount -1 )) / nCount;

    return nAvg;
}


int KQuestGameManager::GetPartyHighestRank()
{
    int iRank = 0;
    for( int i=0 ; i< MAX_PLAYER_NUM ; ++i ) { 
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && 
            m_PlayerRankPoint[i] > iRank ) { 
                iRank = m_PlayerRankPoint[i];
        }
    }

    return iRank;
}

bool KQuestGameManager::NotDecreaseCheck()
{
    //랭크 자연 감소 되지 말야아 할 몬스터 등장 체크
    for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
        if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
            continue;

        MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);
        
        if(pMonster == NULL) continue;

        if ( pMonster->m_iMonsterType == MON_DUEL )
            return true;
    }

    // 사냥터의 경우 몬스터 없으면 자연감소 없도록  
    if( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) &&
        g_kMonsterManager.GetLiveInfinityMonsterTotalNum() == 0 ) { 
            return true;
    }

    return false;
}

void KQuestGameManager::CalcTotalDamageForMonster(float fDamage)
{
    m_sQuestRankInfoS6.fTotalDamage -= fDamage;
}

void KQuestGameManager::SetStartUserCount ( int iCnt)
{
    if ( m_bFirstStartUserRecord ) {
        m_iQuestStartUserCnt = iCnt;
        m_bFirstStartUserRecord = false;
    }
}

void KQuestGameManager::SetPlayerDamage( IN int iIndex, IN float fDamage )
{
    if( !g_MyD3D->IsPlayerIndex( iIndex ) )
        return;

    m_PlayerDamage[iIndex] += fDamage;
}

void KQuestGameManager::GetRankGradeRatioValue(OUT float& fRankRatio )
{
    int iIndex = GetRankIndex( m_sQuestRankInfoS6.fRankPoint );

    if (iIndex >= RANK_GRADE_NUM)
        iIndex = RANK_GRADE_NUM - 1;

    if (iIndex >= static_cast<int> ( m_vecRankGradeRatioValue.size() ) || m_vecRankGradeRatioValue.empty() )
        return;

    fRankRatio = m_vecRankGradeRatioValue[iIndex];
}

void KQuestGameManager::SetInfinityDungeonPartyRank( const int iRank_ )
{
    if( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) ) { 
        m_sQuestRankInfoS6.m_InfinityDungeonPartyRank = iRank_;
    }
}

int KQuestGameManager::GetInfinityDungeonPartyRank( void )
{
    if( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) ) { 
        return m_sQuestRankInfoS6.m_InfinityDungeonPartyRank;
    }

    return 0;
}

void KQuestGameManager::SetInfinityDungeonMonsterLevel( const int iLevel_ )
{
    if( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) ) { 
        m_sQuestRankInfoS6.m_InfinityDungeonMonsterLevel = iLevel_;
    }
}

int KQuestGameManager::GetInfinityDungeonMonsterLevel( void )
{
    if( SiKGCWorldMapManager()->IsInfinityDungeon( SiKGCRoomManager()->GetGameMode() ) ) { 
        return m_sQuestRankInfoS6.m_InfinityDungeonMonsterLevel;
    }

    return 0;
}

void KQuestGameManager::DoStageClearProcess()
{
    DWORD sec = g_pGameBoard->GetPlayTimeInFrame() / GC_FPS_LIMIT;
    DWORD mesec = (DWORD)(( ( g_pGameBoard->GetPlayTimeInFrame() % GC_FPS_LIMIT ) / GC_FPS_LIMIT ) * 1000 );
    
    KDungeonPersonalRecord sDungeonRecord;
    sDungeonRecord.m_nStageNum = m_iCurStage;
    sDungeonRecord.m_dwStageClearTime = (sec * 1000) + mesec;

    if ( SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST81 )
    {
        for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
        {
            if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && !g_MyD3D->MyPlayer[i]->IsDie() )
            {
                sDungeonRecord.m_nControlRank = GetPlayerRank( i );
                
                m_vecDungeonPersonalRecord[i].insert ( std::make_pair( m_iCurStage, sDungeonRecord ) );
            }
        }
    }
}