#include "stdafx.h"

#include "KGCModeTutorial.h"

#include "Monster.h"
#include "KMci.h"
#include "DamageManager.h"
#include "Spark.h"
#include "GCCameraOrtho.h"
#include "Controls.h"
#include "KDInput.h"
#include "Headup Display.h"
#include "GCUI/GCGameOverlayUI.h"
#include "GCUI/GCTutorialDlg.h"

#include "Stage.h"

#include "Controls.h"

#include "gcui/GCSkillCheckBtn.h"
#include "KGCEmbarkManager.h"
#include "KGCSubjectManager.h"


KGCModeTutorial::KGCModeTutorial(void)
{
    m_uiCharType = 0;
    m_uiFrameCount = 0;
    iToggle_Key_S = -1;
    m_uiPractice_Motion = -1;
    m_uiPrevCount = 0;
    m_uiGapNextFrame = 0;
    m_iMaxMapNum = 0;

    if (!LoadTutorialModeCharPosition())
    {
        MessageBox(NULL, L"TutorialModeSet.stg Load Fail", L"", MB_OK);
    }
}

KGCModeTutorial::~KGCModeTutorial(void)
{
}
HRESULT KGCModeTutorial::InitAtLoading()
{
    m_emGameStage = GC_GS_FOREST_OF_ELF;
    ////////////////////////////////////////////////////////////////////////////

    // 튜토리얼 모드 초기화
    g_MyD3D->Pause(TRUE);

    // [8/5/2007 breadceo] 사운드 로딩이 InitDataForTutorial 에서 일어나기 때문에 순서 바뀌면 안됨.
    g_MyD3D->m_pDamageManager->MakeSoundLoadList();
    g_MyD3D->InitDataForTutorial();

    g_MyD3D->Story = 0;
    g_KMci.Play(GC_BGM_LETS_GO, true);

    m_uiCharType = 0;
    m_uiFrameCount = 0;

    //if( m_uiCharType == KCHAR_TYPE_RYAN_1 )
        //m_uiCharType = KCHAR_TYPE_RYAN_4;

    //게임 텍스쳐 로딩
    g_MyD3D->MySparks->Load();
    g_MyD3D->m_pDamageManager->Load();

    g_MyD3D->InsertTestValue();

    g_pkUIScene->m_pkGameOverlayUI->Create();
    //g_pkUIScene->m_pkGameOverlayUI->OffColorBtn(); //연습모드는 칼라 색상 안나옴-0- 혼자 연습하는데 색상넣어서 채팅할일이 머가 있음
    g_pkUIScene->m_pkGameOverlayUI->Initialize();
    g_pkUIScene->m_pkGameOverlayUI->ToggleRender(false);

    g_MyD3D->Pause(FALSE);

    return S_OK;
}

HRESULT KGCModeTutorial::InitAtGame()
{
    g_pkTutorialDlg->InitSkillList();
    InitPracticeCharMotion();

#ifndef _DEBUG
    g_pkTutorialDlg->Enable();
#endif
    return S_OK;
}

HRESULT KGCModeTutorial::CustomFrameMove(float fElapsedTime)
{
    PROFILE_SET("KGCModeTutorial::CustomFrameMove");

    g_MyD3D->Story++;
    ++m_uiFrameCount;

    if (m_uiFrameCount == 200)
    {
        g_pkUIScene->m_pkGameOverlayUI->SetTitleText(g_pkStrLoader->GetString(STR_ID_TUTORIAL_END_MSG));
        //if ( g_kGlobalValue.m_bNewUser_Tutorial == true )
        {
            g_pkUIScene->m_pkGameOverlayUI->SetTitleText(g_pkStrLoader->GetString(STR_ID_TUTORIAL_END_MSG2));
            g_pkUIScene->m_pkGameOverlayUI->SetTitleText(g_pkStrLoader->GetString(STR_ID_TUTORIAL_END_MSG3));
        }
    }

    if (g_MyD3D->MyCtrl->k_Use_PetSkill)
    {
        g_pkTutorialDlg->NextSkillListPage();
    }

    //g_MyD3D->Story++;

    //if ( g_MyD3D->Story == 30 && m_uiCharType == KCHAR_TYPE_RYAN_2 )
    //{
    //  g_pkChatManager->AddChatMsg( g_pkStrLoader->GetString(STR_ID_RYAN_PATCH_DELAY_COMMENT), KGCChatManager::CHAT_TYPE_INVITE );
    //  DXUtil_Timer( TIMER_GETELAPSEDTIME );
    //}

    // 몬스터가 죽지않게할것
    // 나기기처리 확인할것
//#ifndef _DEBUG
    if (diks[DIK_TAB] && !Old_diks[DIK_TAB])
    {
        if (g_pkTutorialDlg->IsEnable() == true)
            g_pkTutorialDlg->Disable();
        else
            g_pkTutorialDlg->Enable();
    }
    //#endif
    g_pkTutorialDlg->FrameMove();

    if (g_MyD3D->GetMyPlayer()->IsMPRecoveryFix())
    {
        float fFixMPValue = g_MyD3D->GetMyPlayer()->GetMPRecoveryFixValue();
        g_MyD3D->ChargeLocalPlayerMana(fFixMPValue);
    }
    else
    {
        if (g_MyD3D->MyHead->Level < g_kGlobalValue.GetMaxMP())
        {
            float fMPBuffRatio = g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_BUFF_MP_RECORVERY_RATE];
            g_MyD3D->ChargeLocalPlayerMana(g_MyD3D->m_fIncMP * 2, fMPBuffRatio);
        }
    }

    KeyProc();
    CheckDieOfChar();
    IncreaseHP();

    //////////////////////////////////////////////////////////////////////////
    /// Practice player motion
    PracticeCharFrameMove();
    //////////////////////////////////////////////////////////////////////////

    SiKGCEmbarkManager()->FrameMove();

    //// 플레이어에 대한 프레임 업데이트 및 충돌처리용 영역을 계산한다.
    g_MyD3D->MyPlayer[0]->Calculate_Body_Rect();
    g_MyD3D->MyPlayer[1]->Calculate_Body_Rect();
    g_MyD3D->MyPlayer[0]->Frame_Move();
    g_MyD3D->MyPlayer[1]->Frame_Move();

    for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
        if (mit->second == NULL) continue;

        mit->second->FrameMove();
    }

    //// 플레이어의 충돌을 체크한다.
    g_MyD3D->MyPlayer[0]->Crash_Check();
    g_MyD3D->MyPlayer[1]->Crash_Check();

    for (int i = 0; i < MAX_OBJECT; i++)
    {
        if (g_MyD3D->m_pObject[i])
        {
            g_MyD3D->m_pObject[i]->FrameMove();
        }
    }

    SiKGCSubjectManager()->FrameMove();

    return S_OK;
}

void KGCModeTutorial::GetCurMousePosInMap(float& fx, float& fy)
{
    POINT ptMPos = g_pkInput->GetMousePos();
    fx = (float)ptMPos.x;
    fy = (float)ptMPos.y;
    g_MyD3D->MyCtrl->Picking(fx, fy);
}

void KGCModeTutorial::SetCharType(int uiCharType)
{
    m_uiCharType = uiCharType;
}

void KGCModeTutorial::CheckTutCharDirection(void)
{
    if (g_MyD3D->MyPlayer[0]->vPos.x < g_MyD3D->MyPlayer[1]->vPos.x)
    {
        g_MyD3D->MyPlayer[1]->bIsRight = false;
        g_MyD3D->MyPlayer[1]->x_Speed = -0.001f;
    }
    else
    {
        g_MyD3D->MyPlayer[1]->bIsRight = true;
        g_MyD3D->MyPlayer[1]->x_Speed = 0.001f;
    }
}

void KGCModeTutorial::CheckDieOfChar(void)
{
    //////////////////////////////////////////////////////////////////////////
    // 튜토리얼 캐릭터의 체크.

    // 지하로 떨어졌 죽었다면 지정된 위치로 옮긴다.
    if (g_MyD3D->MyPlayer[1]->vPos.y < 0.0f)
    {
        g_MyD3D->MyPlayer[1]->vPos.x = GetCurStageTutorialCharPos(m_emGameStage).x;
        g_MyD3D->MyPlayer[1]->vPos.y = GetCurStageTutorialCharPos(m_emGameStage).y;

        g_MyD3D->MyPlayer[1]->SetPlayerState(g_MyD3D->MyPlayer[1]->GetWaitMotion());

        CheckTutCharDirection();
    }
    //////////////////////////////////////////////////////////////////////////



    //////////////////////////////////////////////////////////////////////////
    // 내 캐릭터의 체크

    // 죽었다면 다시 살린다.
    if (g_MyD3D->MyPlayer[0]->m_cLife <= 0)
    {
        g_MyD3D->MyPlayer[0]->SetHPFull();
        g_MyD3D->MyPlayer[0]->vPos.x = GetCurStageMyCharPos(m_emGameStage).x;
        g_MyD3D->MyPlayer[0]->vPos.y = GetCurStageMyCharPos(m_emGameStage).y;
        g_MyD3D->MyPlayer[0]->SetPlayerState(g_MyD3D->MyPlayer[0]->GetWaitMotion());
        g_MyD3D->MyPlayer[0]->x_Speed = 0.0f;
        g_MyD3D->MyPlayer[0]->y_Speed = 0.0f;
        g_MyD3D->MyPlayer[0]->m_cLife = 1;
        g_MyD3D->MyPlayer[0]->Slow_Count = 0;
        g_MyD3D->MyPlayer[0]->Item_Remain_Time[GC_GAME_ITEM_SUPER] = 100;
        g_kCamera.SetTargetPlayer(0);
    }
    //////////////////////////////////////////////////////////////////////////
}

void KGCModeTutorial::KeyProc(void)
{
    // MsgBox가 떠있으면 마우스 클릭 이벤트와 다이얼로그 활성화를 발생시키지 않는다.
    if (!g_pkUIScene->IsMsgBoxModal())
    {
        // 튜토리얼 옵션창 띄우기
        if (g_pkInput->IsDown(DIK_1) && !g_pkGameOverlayUI->HasEditFocus() && g_kGlobalValue.m_bNewUser_Tutorial)
        {
            g_kGlobalValue.m_bTutorialReselect = true;
            InitPracticeCharMotion();
            g_MyD3D->MyStg->ClearParticle();    // 연습모드 맵 파티클 지워줍니다. 

            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_TUTORIAL_OPTION_NEW);
        }

        if (g_pkInput->IsDown(DIK_S))
        {
            SetPracticeCharMotion();
        }

        // 튜토리얼 캐릭터 위치 변경
        if (g_pkInput->BtnPressed(KInput::MBLEFT))
        {
            // 캐릭터 위치 변경
            GetCurMousePosInMap(g_MyD3D->MyPlayer[1]->vPos.x, g_MyD3D->MyPlayer[1]->vPos.y);

            //화면 좌우로 못나가게 함
            if (g_MyD3D->MyPlayer[1]->vPos.x > 1.5f * (float)g_MyD3D->MyStg->Num_Width - 0.1f)
            {
                g_MyD3D->MyPlayer[1]->vPos.x = 1.5f * (float)g_MyD3D->MyStg->Num_Width - 0.1f;
                g_MyD3D->MyPlayer[1]->x_Speed = 0;
            }
            else if (g_MyD3D->MyPlayer[1]->vPos.x < 0.05f)
            {
                g_MyD3D->MyPlayer[1]->vPos.x = 0.05f;
                g_MyD3D->MyPlayer[1]->x_Speed = 0;
            }
            //////////////////////////////////////////////////////////////////////////
            // 캐릭터 출현위치 재지정
            SetCurStageTutorialCharPos(m_emGameStage, D3DXVECTOR2(g_MyD3D->MyPlayer[1]->vPos.x, g_MyD3D->MyPlayer[1]->vPos.y));

            // 캐릭터 방향지정
            CheckTutCharDirection();
        }
    }
}

bool KGCModeTutorial::IsComboIng(void)
{
    if ((timeGetTime() - g_MyD3D->MyHead->Combo_Time) < 1000 * (0.8 /*초*/))
        return true;

    return false;
}

void KGCModeTutorial::IncreaseHP(void)
{
    // 튜토리얼 캐릭터의 HP가 최고치 보다 낮다면?
    if (g_MyD3D->MyPlayer[1]->GetHP() < g_MyD3D->MyPlayer[1]->m_fAbility[ABILITY_HP])
    {
        // HP 상승은 콤보를 때리고 있지 않을 경우.
        if (!IsComboIng())
        {
#ifdef LEVEL_DESIGN_STAT_CALC
            if (g_MyD3D->MyPlayer[1]->GetHP() + 15.f > g_MyD3D->MyPlayer[1]->m_fAbility[ABILITY_HP])
                g_MyD3D->MyPlayer[1]->SetHPFull();
            else
                g_MyD3D->MyPlayer[1]->SetHP(g_MyD3D->MyPlayer[1]->GetHP() + 15.f);
#else
            if (g_MyD3D->MyPlayer[1]->GetHP() + 0.015f > g_MyD3D->MyPlayer[1]->m_fAbility[ABILITY_HP])
                g_MyD3D->MyPlayer[1]->SetHPFull();
            else
                g_MyD3D->MyPlayer[1]->SetHP(g_MyD3D->MyPlayer[1]->GetHP() + 0.015f);
#endif		

        }

        // 생명이 1 이하 이면 생명을 1 올려준다.
        if (g_MyD3D->MyPlayer[1]->m_cLife < 1)
            g_MyD3D->MyPlayer[1]->m_cLife = 1;
    }
    else if (g_MyD3D->MyPlayer[1]->GetHP() > g_MyD3D->MyPlayer[1]->m_fAbility[ABILITY_HP])
    {
        // HP가 최고치를 뛰어넘었다면 정리해준다.
        g_MyD3D->MyPlayer[1]->SetHPFull();
    }
}

bool KGCModeTutorial::LoadTutorialModeCharPosition()
{
    m_mapTutorialInfo.clear();
    m_vecSeqTutorial.clear();
    m_iMaxMapNum = 0;

    KLuaManager luaMgr;

    if (GCFUNC::LoadLuaScript(luaMgr, "Enum.stg") == false)
        return false;

    if (GCFUNC::LoadLuaScript(luaMgr, "TutorialModeSet.lua") == false)
        return false;

    LUA_GET_VALUE("TUTORIAL_MAP_SIZE", m_iMaxMapNum, return false);

    LUA_BEGIN_TABLE("TUTORIAL_MAP_INFO", return false);
    {
        KTutorialMapInfo kTutorialMapInfo;

        for (int i = 1; ; i++)
        {
            LUA_BEGIN_TABLE(i, break)
            {
                kTutorialMapInfo.Init();
                int iGameStage = 0;

                LUA_GET_VALUE("GAME_STAGE", iGameStage, return false);

                kTutorialMapInfo.emGameStage = static_cast<EGCGameStage>(iGameStage);

                LUA_GET_VALUE("STRING_ID", kTutorialMapInfo.nStringIndex, return false);
                LUA_GET_VALUE("MAP_INDEX", kTutorialMapInfo.nMapIndex, return false);
                LUA_GET_VALUE("MY_CHAR_POS_X", kTutorialMapInfo.vMyPos.x, return false);
                LUA_GET_VALUE("MY_CHAR_POS_Y", kTutorialMapInfo.vMyPos.y, return false);
                LUA_GET_VALUE("COM_CHAR_POS_X", kTutorialMapInfo.vComPos.x, return false);
                LUA_GET_VALUE("COM_CHAR_POS_Y", kTutorialMapInfo.vComPos.y, return false);

                m_mapTutorialInfo.insert(std::make_pair(kTutorialMapInfo.emGameStage, kTutorialMapInfo));
                m_vecSeqTutorial.push_back(kTutorialMapInfo.emGameStage);
            }
            LUA_END_TABLE(return false)
        }
    }
    LUA_END_TABLE(return false)


        //  로드한 갯수와 맵 크기가 다르다.
        if (m_mapTutorialInfo.size() != m_iMaxMapNum)
            return false;

    return true;
}

void KGCModeTutorial::GetTutoriaInfo(std::map< EGCGameStage, KTutorialMapInfo >& mapTutorialInfo)
{
    mapTutorialInfo.clear();

    mapTutorialInfo = m_mapTutorialInfo;
}

void KGCModeTutorial::SetCurStageMyCharPos(EGCGameStage emGameStage, D3DXVECTOR2 vPos)
{
    std::map< EGCGameStage, KTutorialMapInfo >::iterator pos = m_mapTutorialInfo.find(emGameStage);

    if (pos == m_mapTutorialInfo.end())
        return;

    pos->second.vMyPos = vPos;
}

void KGCModeTutorial::SetCurStageTutorialCharPos(EGCGameStage emGameStage, D3DXVECTOR2 vPos)
{
    std::map< EGCGameStage, KTutorialMapInfo >::iterator pos = m_mapTutorialInfo.find(emGameStage);

    if (pos == m_mapTutorialInfo.end())
        return;

    pos->second.vComPos = vPos;
}

D3DXVECTOR2 KGCModeTutorial::GetCurStageMyCharPos(EGCGameStage emGameStage)
{
    std::map< EGCGameStage, KTutorialMapInfo >::iterator pos = m_mapTutorialInfo.find(emGameStage);

    if (pos == m_mapTutorialInfo.end())
        return D3DXVECTOR2(0.0f, 0.0f);

    return pos->second.vMyPos;
}

D3DXVECTOR2 KGCModeTutorial::GetCurStageTutorialCharPos(EGCGameStage emGameStage)
{
    std::map< EGCGameStage, KTutorialMapInfo >::iterator pos = m_mapTutorialInfo.find(emGameStage);

    if (pos == m_mapTutorialInfo.end())
        return D3DXVECTOR2(0.0f, 0.0f);

    return pos->second.vComPos;
}

EGCGameStage KGCModeTutorial::GetSeqMagStage(int nSeqIndex)
{
    if (nSeqIndex < 0 || nSeqIndex >= (int)m_vecSeqTutorial.size())
        nSeqIndex = 0;

    std::map< EGCGameStage, KTutorialMapInfo >::iterator pos = m_mapTutorialInfo.find(m_vecSeqTutorial[nSeqIndex]);
    if (pos == m_mapTutorialInfo.end())
        return GC_GS_FOREST_OF_ELF;

    return pos->second.emGameStage;
}

int KGCModeTutorial::GetSeqMapStringID(int nSeqIndex)
{
    if (nSeqIndex < 0 || nSeqIndex >= (int)m_vecSeqTutorial.size())
        nSeqIndex = 0;

    std::map< EGCGameStage, KTutorialMapInfo >::iterator pos = m_mapTutorialInfo.find(m_vecSeqTutorial[nSeqIndex]);
    if (pos == m_mapTutorialInfo.end())
        return 0;

    return pos->second.nStringIndex;
}

void KGCModeTutorial::PracticeCharFrameMove()
{
    if (m_uiPractice_Motion != -1)
    {
        if ((g_MyD3D->MyPlayer[1]->uiMotion == g_MyD3D->MyPlayer[1]->GetWaitMotion()) && (m_uiFrameCount - m_uiPrevCount > m_uiGapNextFrame))
        {
            g_MyD3D->MyPlayer[1]->SetPlayerState(m_uiPractice_Motion);
            g_MyD3D->MyPlayer[1]->cFrame = 0;
            m_uiPrevCount = m_uiFrameCount;
        }
    }
}

void KGCModeTutorial::SetPracticeCharMotion()
{
#ifdef DECANEE
    if (g_MyD3D->MyPlayer[0]->Extra_Char_Num == CID_RONAN3 || ( g_MyD3D->MyPlayer[0]->Extra_Char_Num % GC_CHAR_NUM ) == GC_CHAR_DECANEE )
#else
    if (g_MyD3D->MyPlayer[0]->Extra_Char_Num == CID_RONAN3)
#endif
    {
        iToggle_Key_S *= -1;
        if (iToggle_Key_S == 1)
        {
            m_uiPrevCount = m_uiFrameCount;

            switch (g_MyD3D->MyPlayer[1]->Extra_Char_Num)
            {
            case CID_ARME1:
                g_MyD3D->MyPlayer[1]->SetPlayerState(MID_ARME_DASHATK);
                break;
            case CID_ARME2:
                g_MyD3D->MyPlayer[1]->SetPlayerState(MID_ARME2_DASHATK);
                break;
            case CID_ARME3:
                g_MyD3D->MyPlayer[1]->SetPlayerState(MID_ARME3_SUMMON_UNDINE);
                break;
            case CID_ARME4:
                g_MyD3D->MyPlayer[1]->SetPlayerState(MID_ARME4_JUMP_ATK2);
                break;
            default:
                g_MyD3D->MyPlayer[1]->SetPlayerState(g_MyD3D->MyPlayer[1]->GetCurFormTemplate().ATK1);
                break;
            }

            m_uiPractice_Motion = g_MyD3D->MyPlayer[1]->uiMotion;
            m_uiGapNextFrame = (g_MyD3D->MyPlayer[1]->GetNowMotionFRM()->GetNum_Frame() - 1) + 30;
            g_MyD3D->MyPlayer[1]->cFrame = 0;
        }
        else
        {
            m_uiPractice_Motion = -1;
            g_MyD3D->MyPlayer[1]->SetPlayerState(g_MyD3D->MyPlayer[1]->GetWaitMotion());
            g_MyD3D->MyPlayer[1]->cFrame = 0;
        }
    }
}

void KGCModeTutorial::InitPracticeCharMotion()
{
    m_uiPractice_Motion = -1;
    iToggle_Key_S = -1;
    g_MyD3D->MyPlayer[1]->SetPlayerState(g_MyD3D->MyPlayer[1]->GetWaitMotion());
}
