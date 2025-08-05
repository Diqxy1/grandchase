#include "StdAfx.h"
//
#include "ClientErr.h"

#include ".\everytriggermanager.h"
#include "MyD3D.h"
#include "QuestGameManager.h"
#include "gcui/GCGameOverlayUI.h"
#include "GCStateGame.h"

#include "GCStateMachine.h"
//

#include "TriggerManager.h"
#include "KGCRoomManager.h"

#include "OpenProcess.h"

#include "gcui/GCAddtionGuide.h"
#include "GCClientErrorManager.h"

//#define DEFAULT_DUNGEON_LOADING_TIME (3000)
// 응? 그냥 준건가? 로딩?
#define DEFAULT_DUNGEON_LOADING_TIME (1000)
//#define DEFAULT_DUNGEON_LOADING_TIME (100)
#define NOT_MATCH_TRIGGER_END_TIME   (GC_FPS_LIMIT*10)

CEveryTriggerManager g_kEveryTriggerManager;

CEveryTriggerManager::CEveryTriggerManager(void)
{
    m_iStartDelay                = 0;
    m_iNextStageDelay            = 0;
    m_bNotMatchTrigger           = false;
}

CEveryTriggerManager::~CEveryTriggerManager(void)
{
    ClearEveryTrigger();
}

void CEveryTriggerManager::Init()
{
    TileMap blank;
    m_ActiveTileMap = blank;
}

CTrigger *CEveryTriggerManager::FindTrigger(std::string &strTGName) const
{
    for(int i=0;i<(int)m_vtTriggerManager.size();++i)
    {
        CTrigger *pTrigger = m_vtTriggerManager[i]->FindTrigger(strTGName);
        if( pTrigger ) return pTrigger;
    }

    return NULL;
}

CTrigger *CEveryTriggerManager::FindActiveTrigger(std::string &strTGName) const
{
    for(int i=0;i<(int)m_vtActiveTrigger.size();++i)
    {
        if( m_vtActiveTrigger[i]->m_strTGName == strTGName )
            return m_vtActiveTrigger[i];
    }

    return NULL;
}

void CEveryTriggerManager::ClearEveryTrigger(void)
{
    m_vtActiveTrigger.clear();

    for(int i=0;i<(int)m_vtTriggerManager.size();++i)
        SAFE_DELETE(m_vtTriggerManager[i]);
    m_vtTriggerManager.clear();
    for(int i=0;i<(int)m_vtConditionToStart.size();++i)
        SAFE_DELETE(m_vtConditionToStart[i]);
    m_vtConditionToStart.clear();    
}
bool CEveryTriggerManager::MovetoNextStage(void)
{
	bool bRet = false;
	//************************************************************************
	// 다음 스테이지로 넘어가도 되는지 확인
	//************************************************************************
	if( (m_NextTileMap.iCol != m_ActiveTileMap.iCol || m_NextTileMap.iRow != m_ActiveTileMap.iRow) )
	{
		const int iRemainDelayTime = GetRemainNextDelay();
		if( iRemainDelayTime <= DEFAULT_DUNGEON_LOADING_TIME )      // 시간이 3초 이하로 남았나? - 로딩시간을 0으로 입력해도 최소 3초.
		//if( iRemainDelayTime <= 1000 )      // 시간이 3초 이하로 남았나? - 로딩시간을 0으로 입력해도 최소 3초.
		{
			if( iRemainDelayTime > 0 )          // DEFAULT_DUNGEON_LOADING_TIME > 시간이 > 0  이면 로딩화면을 띄운다.
			{
				if( m_bLoadingScreen && !g_pkGameOverlayUI->IsShowQuestTitle() )
				{
#if defined( USE_STAGE_LOAD_COMPLETE_SIGN )
                    g_pkGameOverlayUI->InitStageLoadCompleteInfo();

                    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
                    {
                        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == true )
                        {
                            g_pkGameOverlayUI->SetLoadCompleteUserUID(g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID);
#if defined(DLL_INJECTION_CHECK)
                            KDLLChecker::GetInstance()->CheckDll();
#endif

                        }
                    }
#endif
					if (g_pkGameOverlayUI->m_pkAddtionGuide && g_pkGameOverlayUI->m_pkAddtionGuide->IsRenderOn())
					{
						g_pkGameOverlayUI->m_pkAddtionGuide->OnHide();
						g_pkGameOverlayUI->m_pkAddtionGuide->ToggleRender(false);
					}
					g_pkGameOverlayUI->ShowQuestTitle( SiKGCRoomManager()->GetGameMode(), g_pkQuestManager->m_iCurStage + 1, true );
				}
			}
			else
			{
#if defined( USE_ONGAME_INVENTORY )
                if( KD3DWnd::D3DWS_ENABLED == g_pkUIScene->GetStateUI( KGCUIScene::GC_MBOX_GAME_INVENTORY ) ) { 
                    g_pkUIScene->CloseGameInventory();
                }
#endif
				SetActiveStage(m_NextTileMap);
                g_pkQuestManager->RankPointCalcState(false);
				g_pkGameOverlayUI->HideAllQuestTitle();
				bRet = true;
			}
		}
	}
	return bRet;
}

void CEveryTriggerManager::FrameMove(void)
{
    //************************************************************************
    // 완료한 트리거는 액티브 트리거에서 제거해준다.
    //************************************************************************
    bool bEnter = false;
    vector<CTrigger*>::iterator itTG = m_vtActiveTrigger.begin();
    for(;m_vtActiveTrigger.end() != itTG;)
    {
        (*itTG)->FrameMove();
#if !defined(__PATH__)
        if( g_kGlobalValue.m_bPrintTG && (*itTG)->m_bActive )
        {
            if( g_kGlobalValue.m_strBeforeTG != (*itTG)->m_strTGName )
            {
                g_kGlobalValue.m_strBeforeTG = (*itTG)->m_strTGName;
                printf("TGName : %s\n", (*itTG)->m_strTGName.c_str() );
                bEnter = true;
            }
        }
#endif
        if( (*itTG)->IsComplete() )
            itTG = m_vtActiveTrigger.erase(itTG);
        else
            itTG++;
    }

    if( bEnter )
        printf("\n");
}

void CEveryTriggerManager::AddTriggerMgr(CTriggerManager *pTriggerMgr)
{
    if( pTriggerMgr ) m_vtTriggerManager.push_back(pTriggerMgr);
}

void CEveryTriggerManager::AddActiveTrigger(CTrigger *pTrigger)
{
    if( pTrigger ) m_vtActiveTrigger.push_back(pTrigger);
}

void CEveryTriggerManager::SetActiveStage(TileMap tilemap)
{
    if( (int)m_vtTriggerManager.size() <= tilemap.iStage || tilemap.iStage < 0 ) return;

    //************************************************************************
    // 스테이지를 변경하고 해당 스테이지에 사용되는 트리거를 
    // Active 트리거에 넣는다.
    //************************************************************************
    m_ActiveTileMap = tilemap;
    m_vtTriggerManager[m_ActiveTileMap.iStage]->SetActive(true);    
    m_vtTriggerManager[m_ActiveTileMap.iStage]->StartTrigger();
    m_vtActiveTrigger.clear();
    for(int i=0;i<(int)m_vtTriggerManager[m_ActiveTileMap.iStage]->m_vtTriggerList.size();++i)
    {
        m_vtActiveTrigger.push_back(m_vtTriggerManager[m_ActiveTileMap.iStage]->m_vtTriggerList[i]);
    }

#if defined(USE_LOADINGTIME_RECORD)
    FILE *m_pLoadingTimeFile = fopen("LoadingTime_Stage.txt", "at");
    DWORD dwStartTime = ::timeGetTime();
#endif

    g_pkQuestManager->LoadStage(m_ActiveTileMap);

#if defined(USE_LOADINGTIME_RECORD)
    fprintf(m_pLoadingTimeFile, "GameMode = %d\nStage = %d\nLoadingTime = %f\n\n", int(SiKGCRoomManager()->GetGameMode()), tilemap.iStage, (::timeGetTime()-dwStartTime)/1000.0f);
    fclose( m_pLoadingTimeFile );
#endif
}

void CEveryTriggerManager::SetNextStage(TileMap tilemap, bool bLoadingScreen /*= true*/, int iNextStageTime /* = 0 */ )
{
    if( (int)m_vtTriggerManager.size() <= tilemap.iStage || tilemap.iStage < 0 ) return;

    g_pkQuestManager->DoStageClearProcess();


    m_NextTileMap       = tilemap;
    m_iStartDelay       = timeGetTime();
    m_iNextStageDelay   = (iNextStageTime + DEFAULT_DUNGEON_LOADING_TIME);
    m_bLoadingScreen    = bLoadingScreen;

    g_pkQuestManager->m_bClearStage = true;
#if defined( DUNGEONPLAYSTATE )
    g_kGlobalValue.m_dwStageStartTime = timeGetTime();
#endif
    for(int i=0;i<MAX_PLAYER_NUM;++i)
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
        {
            if( g_MyD3D->MyPlayer[i]->CheckInPortal() == false ) {
                g_MyD3D->MyPlayer[i]->ProcessMetamorphosisNormalForm( false, MID_COMMON_INTO_TELEPORT );
            } else {
                g_MyD3D->MyPlayer[i]->ProcessMetamorphosisNormalForm( false );
            }
        }
    }
}

void CEveryTriggerManager::AddCheckWIthConditionForStart(CTriggerCondition *pConditon)
{
    //************************************************************************
    // 실행 대기 컨디션 추가
    //************************************************************************
    if( pConditon ) m_vtConditionToStart.push_back(pConditon);
}

CTriggerManager* CEveryTriggerManager::GetCurrentStageTriggerManager() const
{
    if( !IsExistenceTriggerMgr(m_ActiveTileMap.iStage) )
        return NULL;

    return m_vtTriggerManager[m_ActiveTileMap.iStage];
}

CTriggerManager* CEveryTriggerManager::GetNextStageTriggerManager() const
{
    if( !IsExistenceTriggerMgr(m_NextTileMap.iStage) )
        return NULL;

    return m_vtTriggerManager[m_NextTileMap.iStage];
}

CTriggerCondition *CEveryTriggerManager::FindTriggerConditonByID(const int iTG_ID) const
{
    for(int i=0;i<(int)m_vtTriggerManager.size();++i)
    {
        CTriggerCondition *pTCondition = m_vtTriggerManager[i]->FindTriggerConditonByID(iTG_ID);
        if( pTCondition ) return pTCondition;
    }

    return NULL;
}

CTriggerCondition *CEveryTriggerManager::FindActiveTriggerConditonByID(const int iTG_ID) const
{
    for(int i=0;i<(int)m_vtActiveTrigger.size();++i)
    {
        CTriggerCondition *pTCondition = m_vtActiveTrigger[i]->FindTriggerConditonByID(iTG_ID);
        if( pTCondition ) return pTCondition;
    }

    return NULL;
}

void CEveryTriggerManager::GetTriggerConditionByType(int iStage, CTrigger::enTriggerCondition enTG_Type, std::vector<CTriggerCondition*> &vtTGCondition)
{
    if( !IsExistenceTriggerMgr(iStage) ) return;

    m_vtTriggerManager[iStage]->GetTriggerConditionByType(enTG_Type, vtTGCondition);
}

int CEveryTriggerManager::GetCntOfTriggerType(int iStage, CTrigger::enTriggerCondition enTG_Type) const
{
    if( !IsExistenceTriggerMgr(iStage) ) return 0;

    return m_vtTriggerManager[iStage]->GetCntOfTriggerType(enTG_Type);
}

int CEveryTriggerManager::GetRemainNextDelay(void) const
{
    const int iRemainTime = m_iNextStageDelay - (timeGetTime() - m_iStartDelay);

    if( iRemainTime > 0 )
        return iRemainTime;

    return 0;
}

bool CEveryTriggerManager::IsExistenceTriggerMgr(int iIndex) const
{
    if( m_vtTriggerManager.empty() || iIndex < 0 || iIndex >= (int)m_vtTriggerManager.size() )
        return false;

    return true;
}


void CEveryTriggerManager::SetNotMatchTrigger(bool bNotMatch, bool bNoSendReport )
{
    if ( bNotMatch && !bNoSendReport )
    {
        //SiGCClientErrorManager()->ErrorCollectData( KEventErr::ERR_CUSTOM, KClientErr::CE_NOT_EQUAL_QUEST_INFO, GCUTIL_STR::GCStrCharToWide( g_kGlobalValue.m_kLoginInfo.strID.c_str() ) );
    }
    if( !m_bNotMatchTrigger )        // 이미 종료 처리 중이라면 EndTime 을 업데이트 하지 않는다.
        m_dwNotMatchEndTime = NOT_MATCH_TRIGGER_END_TIME;
    m_bNotMatchTrigger  = bNotMatch;
}

bool CEveryTriggerManager::FrameMove_NotMatchTrigger(void)
{
	

    if( !m_bNotMatchTrigger ) return false;

    if( m_dwNotMatchEndTime > 0 )
    {
        --m_dwNotMatchEndTime;
		
    }
    else
    {
		KP2P::GetInstance()->Send_ClientFailedGameStartNot(KFailRate::DIFFERENT_LOADING_DATA);
        g_pStateGame->EndGame();
        g_MyD3D->m_pStateMachine->ExitRoom( true );

        return true;
    }

    return false;
}
