#include "stdafx.h"
//

#include ".\KGCPetEvolutionBox.h"

//

//
//
//

#include "MyD3D.h"
#include "KGCPetCardSelect.h"
#include "KGCPetEvolutionWnd.h"



IMPLEMENT_CLASSNAME(KGCPetEvolutionBox);
IMPLEMENT_WND_FACTORY(KGCPetEvolutionBox);
IMPLEMENT_WND_FACTORY_NAME(KGCPetEvolutionBox, "gc_pet_evolution_box");


#ifdef DELTAZOOM
#undef DELTAZOOM
#endif
#define DELTAZOOM	0.01f

#ifdef MAXZOOM
#undef MAXZOOM
#endif
#define MAXZOOM	0.35f

#ifdef MINZOOM
#undef MINZOOM
#endif
#define MINZOOM 0.2f

KGCPetEvolutionBox::KGCPetEvolutionBox(void)
{
    m_pkTextEvolution = NULL;
    m_pkTextDevolution = NULL;
    m_pkTextHatch = NULL;
    m_pkTextEvolutionSamll = NULL;
    m_pkTextDevolutionSmall = NULL;
    m_pkTextHatchSmall = NULL;
    m_pkCurrentPetStatic = NULL;
    m_pkNextPetStatic = NULL;
    m_pkExit = NULL;
    m_pkEvolutionButton = NULL;
    m_pkDevolutionButton = NULL;
    m_pkCurrentPetPreview = NULL;
    m_pkNextPetPreview = NULL;
    m_pkPetCardSelect = NULL;
    m_pkCurrentPetWnd = NULL;
    m_pkNextPetWnd = NULL;
    m_pkCardLeft = NULL;
    m_pkCardRight = NULL;
    m_pkDisableEvolutionButton = NULL;
    m_pkDisableDevolutionButton = NULL;
    m_pkInvalidEvolutionMark = NULL;
    m_pkHatchButton = NULL;
    m_pkDisableHatchButton = NULL;
    m_pkCurrentPetLevelStatic = NULL;
    m_pkNextPetLevelStatic = NULL;


    LINK_CONTROL("text_evolution", m_pkTextEvolution);
    LINK_CONTROL("text_devolution", m_pkTextDevolution);
    LINK_CONTROL("text_hatch", m_pkTextHatch);
    LINK_CONTROL("text_evolution_small", m_pkTextEvolutionSamll);
    LINK_CONTROL("text_devolution_small", m_pkTextDevolutionSmall);
    LINK_CONTROL("text_hatch_small", m_pkTextHatchSmall);
    LINK_CONTROL("current_pet_static", m_pkCurrentPetStatic);
    LINK_CONTROL("next_pet_static", m_pkNextPetStatic);
    LINK_CONTROL("exit_button", m_pkExit);
    LINK_CONTROL("evolution_button", m_pkEvolutionButton);
    LINK_CONTROL("devolution_button", m_pkDevolutionButton);
    LINK_CONTROL("preview_current_pet", m_pkCurrentPetPreview);
    LINK_CONTROL("preview_next_pet", m_pkNextPetPreview);
    LINK_CONTROL("pet_card_select", m_pkPetCardSelect);
    LINK_CONTROL("current_pet_wnd", m_pkCurrentPetWnd);
    LINK_CONTROL("next_pet_wnd", m_pkNextPetWnd);
    LINK_CONTROL("card_left_button", m_pkCardLeft);
    LINK_CONTROL("card_right_button", m_pkCardRight);
    LINK_CONTROL("invalid_evolution_mark", m_pkInvalidEvolutionMark);
    LINK_CONTROL("disable_evolution_button", m_pkDisableEvolutionButton);
    LINK_CONTROL("disable_devolution_button", m_pkDisableDevolutionButton);
    LINK_CONTROL("hatch_button", m_pkHatchButton);
    LINK_CONTROL("disable_hatch_button", m_pkDisableHatchButton);
    LINK_CONTROL("current_pet_level_static", m_pkCurrentPetLevelStatic);
    LINK_CONTROL("next_pet_level_static", m_pkNextPetLevelStatic);

    m_iEvolutionType = 0;
    m_bClose = true;
    m_dwPetUID = 0;
}

KGCPetEvolutionBox::~KGCPetEvolutionBox(void)
{
}

void KGCPetEvolutionBox::OnCreate(void)
{
    m_pkTextEvolution->InitState(true);
    m_pkTextDevolution->InitState(false);
    m_pkTextHatch->InitState(false);

    m_pkTextEvolutionSamll->InitState(true);
    m_pkTextDevolutionSmall->InitState(false);
    m_pkTextHatchSmall->InitState(false);

    m_pkCurrentPetStatic->InitState(true);
    m_pkNextPetStatic->InitState(true);

    m_pkExit->InitState(true, true, this);
    m_pkEvolutionButton->InitState(true, true, this);
    m_pkDevolutionButton->InitState(false, true, this);
    m_pkHatchButton->InitState(false, true, this);
    m_pkDisableEvolutionButton->InitState(false, true);
    m_pkDisableDevolutionButton->InitState(false, true);
    m_pkDisableHatchButton->InitState(false, true);

    m_pkCurrentPetPreview->InitState(true, true, this);
    m_pkNextPetPreview->InitState(true, true, this);
    m_pkPetCardSelect->InitState(true, true, this);
    m_pkCurrentPetWnd->InitState(true, true, this);
    m_pkNextPetWnd->InitState(true, true, this);

    m_pkCardLeft->InitState(true, true, this);
    m_pkCardRight->InitState(true, true, this);

    m_pkInvalidEvolutionMark->InitState(false);

    m_pkCurrentPetLevelStatic->InitState(true);
    m_pkNextPetLevelStatic->InitState(true);

    m_pkCurrentPetStatic->SetShadow(true);
    m_pkNextPetStatic->SetShadow(true);
    m_pkCurrentPetStatic->SetAlign(DT_CENTER);
    m_pkNextPetStatic->SetAlign(DT_CENTER);

    m_pkCurrentPetLevelStatic->SetAlign(DT_CENTER);
    m_pkNextPetLevelStatic->SetAlign(DT_CENTER);
    m_pkCurrentPetLevelStatic->SetFontColor(0xFFFF0000);
    m_pkNextPetLevelStatic->SetFontColor(0xFFFF0000);
    m_pkCurrentPetLevelStatic->SetShadow(true);
    m_pkNextPetLevelStatic->SetShadow(true);

    m_dwPetUID = 0;
    m_bClose = true;
}

void KGCPetEvolutionBox::ActionPerformed(const KActionEvent& event)
{
    if (event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER)
        g_KDSound.Play("30");

    if (event.m_pWnd == m_pkPetCardSelect)
    {
        switch (event.m_dwCode)
        {
        case KGCPetCardSelect::CHANGE_PET_CARD_LEFT:
            ChangePet(false);
            break;

        case KGCPetCardSelect::CHANGE_PET_CARD_RIGHT:
            ChangePet(true);
            break;
        }
    }

    if (event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
    {
        if (event.m_pWnd == m_pkExit) // 닫기 버튼
        {
            g_KDSound.Play("31");
            SpeakToActionListener(KActionEvent(this, EXIT));
        }
        else if (event.m_pWnd == m_pkEvolutionButton || event.m_pWnd == m_pkDevolutionButton) // 진화 / 퇴화 버튼
        {
            g_KDSound.Play("31");
            SpeakToActionListener(KActionEvent(this, TRANSFORMATION));
        }
        else if (event.m_pWnd == m_pkHatchButton) // 부화 버튼
        {
            g_KDSound.Play("31");
            SpeakToActionListener(KActionEvent(this, HATCH));
        }
        else if (event.m_pWnd == m_pkCurrentPetPreview) // 현재펫 프리뷰
        {
            g_KDSound.Play("31");

            // 현재 펫과 진화(퇴화)할 펫에 대한 펫 프리뷰를 보여준다
            // 망할 키티펫.........
            if (m_dwPetGoodsID != 30798)
                SpeakToActionListener(KActionEvent(this, CURRENT_PET_PREVIEW));
            m_bClose = false;
        }
        else if (event.m_pWnd == m_pkNextPetPreview) // 다음상태의 펫 프리뷰
        {
            g_KDSound.Play("31");

            // 현재 펫과 진화(퇴화)할 펫에 대한 펫 프리뷰를 보여준다
            SpeakToActionListener(KActionEvent(this, NEXT_PET_PREVIEW));
            m_bClose = false;
        }
        else if (event.m_pWnd == m_pkCardLeft)
        {
            g_KDSound.Play("31");
            ChangePet(false);
        }
        else if (event.m_pWnd == m_pkCardRight)
        {
            g_KDSound.Play("31");
            ChangePet(true);
        }
    }
}

void KGCPetEvolutionBox::FrameMoveInEnabledState()
{
    if (g_pkInput->IsDown(DIK_ESCAPE) || g_pkInput->IsDown(DIK_RETURN)
#ifdef USE_JOYSTICK
        || g_MyD3D->GetJoystickKey() == EJK_KEY_SELECT
        || g_MyD3D->GetJoystickKey() == EJK_KEY_START
#endif
        )
    {
        if (m_bClose)
        {
            if (g_pkInput->IsDown(DIK_ESCAPE)
#ifdef USE_JOYSTICK
                || g_MyD3D->GetJoystickKey() == EJK_KEY_SELECT
#endif
                )
                SpeakToActionListener(KActionEvent(this, EXIT));
        }
        else
        {
            m_bClose = true;
        }

        g_pkInput->IgnoreDown(DIK_ESCAPE, 30);
    }
}

void KGCPetEvolutionBox::SetEvolutionPet(GCITEMUID dwPetUID, int iEvolutionType, DWORD dwUseItemID)
{
    m_iEvolutionType = iEvolutionType;
    m_dwUseItemID = dwUseItemID;
    m_iMinNeedPetLevel = GetMinNeedPetLevel(dwUseItemID); // 이 진화마법서를 사용 시 최소 진화 레벨

    // 버튼이나 다른 윈도우에 대한 세팅
    m_pkTextEvolution->ToggleRender(m_iEvolutionType == EGCPETEVOLUTIONTYPE::EVOLUTION);
    m_pkTextDevolution->ToggleRender(m_iEvolutionType == EGCPETEVOLUTIONTYPE::DEVOLUTION);
    m_pkTextHatch->ToggleRender(m_iEvolutionType == EGCPETEVOLUTIONTYPE::HATCH);

    m_pkTextEvolutionSamll->ToggleRender(m_iEvolutionType == EGCPETEVOLUTIONTYPE::EVOLUTION);
    m_pkTextDevolutionSmall->ToggleRender(m_iEvolutionType == EGCPETEVOLUTIONTYPE::DEVOLUTION);
    m_pkTextHatchSmall->ToggleRender(m_iEvolutionType == EGCPETEVOLUTIONTYPE::HATCH);

    m_pkEvolutionButton->ToggleRender(m_iEvolutionType == EGCPETEVOLUTIONTYPE::EVOLUTION);
    m_pkDevolutionButton->ToggleRender(m_iEvolutionType == EGCPETEVOLUTIONTYPE::DEVOLUTION);
    m_pkHatchButton->ToggleRender(m_iEvolutionType == EGCPETEVOLUTIONTYPE::HATCH);

    m_pkEvolutionButton->SetSelfInputCheck(true);
    m_pkDevolutionButton->SetSelfInputCheck(true);
    m_pkHatchButton->SetSelfInputCheck(true);
    m_pkDisableEvolutionButton->ToggleRender(false);
    m_pkDisableDevolutionButton->ToggleRender(false);
    m_pkDisableHatchButton->ToggleRender(false);

    m_pkInvalidEvolutionMark->ToggleRender(false);

    std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.find(dwPetUID);
    if (mit == g_MyD3D->m_mapPetInfo.end())
    {
        ASSERT(!"Pet정보가 없다");
        return;
    }

    KPetInfo kPetInfo = mit->second;

    int iPetID = g_kGlobalValue.GetPetIDFromItemID(kPetInfo.m_dwID);
    int iPetEvolutionLevel = kPetInfo.m_cPromotion;

    m_dwPetUID = kPetInfo.m_dwUID;
    m_dwPetGoodsID = kPetInfo.m_dwID;

    m_pkCurrentPetWnd->InitPetWnd(iPetID, iPetEvolutionLevel);

    if (m_iEvolutionType == EGCPETEVOLUTIONTYPE::HATCH)
        m_pkCurrentPetStatic->SetText(g_pkStrLoader->GetString(STR_ID_PET_EVOLUTION_STATE0));
    else
        m_pkCurrentPetStatic->SetText(g_pkStrLoader->GetString(STR_ID_PET_EVOLUTION_STATE1 + iPetEvolutionLevel));

    // 현재 펫 레벨..
    std::wostringstream strmPetLevel;
    strmPetLevel << g_pkStrLoader->GetString(STR_ID_LEVEL) << " " << kPetInfo.m_iLevel;
    m_pkCurrentPetLevelStatic->SetText(strmPetLevel.str());
    m_pkNextPetLevelStatic->SetText(L"");

    // 다음상태의 펫을 세팅한다..
    switch (m_iEvolutionType)
    {
    case EGCPETEVOLUTIONTYPE::EVOLUTION:
    {
        if (g_kGlobalValue.GetPetTemplate(iPetID, 0).iMaxEvolutionLevel >= iPetEvolutionLevel + 1 && g_kGlobalValue.GetPetTemplate(iPetID, 0).iMaxEvolutionLevel > 0)
        {
            m_pkNextPetWnd->InitPetWnd(iPetID, iPetEvolutionLevel + 1);
            m_pkNextPetStatic->SetText(g_pkStrLoader->GetString(STR_ID_PET_EVOLUTION_STATE1 + iPetEvolutionLevel + 1));
            m_pkNextPetPreview->ToggleRender(true);

            // 진화 시 펫 레벨..
            std::wostringstream strmNextPetLevel;
            DWORD dwNextPetExp = kPetInfo.m_mapInitExp[(char)(iPetEvolutionLevel + 1)];
            int iNextPetLevel = GetPetLevel(dwNextPetExp);
            strmNextPetLevel << g_pkStrLoader->GetString(STR_ID_LEVEL) << " " << iNextPetLevel;
            m_pkNextPetLevelStatic->SetText(strmNextPetLevel.str());

            // 진화를 할 수 있는 최소 레벨이 되지 않는다면 진화 버튼을 비활성화 시킨다
            if (kPetInfo.m_iLevel < m_iMinNeedPetLevel)
            {
                m_pkEvolutionButton->SetSelfInputCheck(false);
                m_pkEvolutionButton->ToggleRender(false);
                m_pkDisableEvolutionButton->ToggleRender(true);
                m_pkDisableEvolutionButton->SetToolTip(g_pkStrLoader->GetString(STR_ID_PET_EVOLUTION_MESSAGE8));
            }
        }
        else
        {
            m_pkNextPetWnd->SetInvalidPet();
            m_pkNextPetStatic->SetText(g_pkStrLoader->GetString(STR_ID_PET_EVOLUTION_STATE4));
            m_pkNextPetPreview->ToggleRender(false);

            m_pkEvolutionButton->SetSelfInputCheck(false);
            m_pkEvolutionButton->ToggleRender(false);
            m_pkDisableEvolutionButton->ToggleRender(true);
            m_pkInvalidEvolutionMark->ToggleRender(true);
            m_pkDisableEvolutionButton->SetToolTip(g_pkStrLoader->GetString(STR_ID_PET_EVOLUTION_STATE4));
        }
        break;
    }
    case EGCPETEVOLUTIONTYPE::DEVOLUTION:
    {
        if (iPetEvolutionLevel > 0 && g_kGlobalValue.GetPetTemplate(iPetID, 0).iMaxEvolutionLevel > 0)
        {
            m_pkNextPetWnd->InitPetWnd(iPetID, iPetEvolutionLevel - 1);
            m_pkNextPetStatic->SetText(g_pkStrLoader->GetString(STR_ID_PET_EVOLUTION_STATE1 + iPetEvolutionLevel - 1));
            m_pkNextPetPreview->ToggleRender(true);

            // 퇴화 시 펫 레벨..
            std::wostringstream strmNextPetLevel;
            DWORD dwNextPetExp = kPetInfo.m_mapInitExp[(char)(iPetEvolutionLevel - 1)];
            int iNextPetLevel = GetPetLevel(dwNextPetExp);
            strmNextPetLevel << g_pkStrLoader->GetString(STR_ID_LEVEL) << " " << iNextPetLevel;
            m_pkNextPetLevelStatic->SetText(strmNextPetLevel.str());
        }
        else
        {
            m_pkNextPetWnd->SetInvalidPet();
            m_pkNextPetStatic->SetText(g_pkStrLoader->GetString(STR_ID_PET_EVOLUTION_STATE5));
            m_pkNextPetPreview->ToggleRender(false);

            m_pkDevolutionButton->SetSelfInputCheck(false);
            m_pkDevolutionButton->ToggleRender(false);
            m_pkDisableDevolutionButton->ToggleRender(true);
            m_pkInvalidEvolutionMark->ToggleRender(true);
            m_pkDisableDevolutionButton->SetToolTip(g_pkStrLoader->GetString(STR_ID_PET_EVOLUTION_STATE5));
        }
        break;
    }
    case EGCPETEVOLUTIONTYPE::HATCH:
    {
        int iHatchingPetID = g_kGlobalValue.GetPetIDFromItemID(kPetInfo.m_nHatchingID / 10);

        m_pkNextPetWnd->InitPetWnd(iHatchingPetID, 0);
        m_pkNextPetStatic->SetText(g_pkStrLoader->GetString(STR_ID_PET_EVOLUTION_STATE1));
        m_pkHatchButton->Lock(false);
        m_pkNextPetPreview->ToggleRender(true);

        // 부화 시 펫 레벨..
        std::wostringstream strmNextPetLevel;
        strmNextPetLevel << g_pkStrLoader->GetString(STR_ID_LEVEL) << " " << 0;
        m_pkNextPetLevelStatic->SetText(strmNextPetLevel.str());

        // 진화를 할 수 있는 최소 레벨이 되지 않는다면 진화 버튼을 비활성화 시킨다
        if (kPetInfo.m_iLevel < m_iMinNeedPetLevel)
        {
            m_pkHatchButton->SetSelfInputCheck(false);
            m_pkHatchButton->ToggleRender(false);
            m_pkDisableHatchButton->ToggleRender(true);
            m_pkDisableHatchButton->SetToolTip(g_pkStrLoader->GetString(STR_ID_PET_EVOLUTION_MESSAGE13));
        }
        break;
    }
    }

    // 펫 정보를 진화/퇴화 냐 부화냐에 따라서..
    // 벡터에 따로 담는다..
    std::vector< KPetInfo > vecPetInfo;
    std::map<GCITEMUID, KPetInfo>::iterator mitTemp;
    for (mitTemp = g_MyD3D->m_mapPetInfo.begin(); mitTemp != g_MyD3D->m_mapPetInfo.end(); ++mitTemp)
    {
        if (m_iEvolutionType == EGCPETEVOLUTIONTYPE::HATCH)
        {
            // 부화 상자 타입이면.. 펫 정보중에 알들을 모두 벡터에 담는다..
            if (mitTemp->second.m_nHatchingID != -1)
                vecPetInfo.push_back(mitTemp->second);
        }
        else
        {
            // 진화나 퇴화 상자이면.. 일반 펫정보만 담는다..
            if (mitTemp->second.m_nHatchingID == -1)
                vecPetInfo.push_back(mitTemp->second);
        }
    }

    // 왼쪽과 오른쪽에 표시해 줄 펫카드 정보
    KPetInfo kLeftPetInfo;
    KPetInfo kRightPetInfo;
    // [2/11/2008] breadceo. PetInfo는 Init후에 사용하세요.
    kLeftPetInfo.Init();
    kRightPetInfo.Init();
    int iSize = (int)vecPetInfo.size();
    for (int i = 0; i < iSize; ++i)
    {
        // 벡터에서 현재 펫을 찾자..
        if (vecPetInfo[i].m_dwUID == kPetInfo.m_dwUID)
        {
            // 찾으면..
            kLeftPetInfo = vecPetInfo[(iSize + i - 1) % iSize];
            kRightPetInfo = vecPetInfo[(iSize + i + 1) % iSize];
        }
    }

    m_pkPetCardSelect->SetCard(kPetInfo.m_dwUID, kLeftPetInfo.m_dwUID, kRightPetInfo.m_dwUID);
}

void KGCPetEvolutionBox::Release()
{
    m_dwPetUID = 0;
    m_pkCurrentPetWnd->Release();
    m_pkNextPetWnd->Release();
    m_pkPetCardSelect->Release();
}

void KGCPetEvolutionBox::ChangePet(bool bRight)
{
    KPetInfo kPetInfo;
    std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.find(m_dwPetUID);

    // 펫을 찾지 못했다!?
    if (mit == g_MyD3D->m_mapPetInfo.end())
    {
        ASSERT(!"Pet정보가 없다");
        return;
    }

    // 펫 정보를 진화/퇴화 냐 부화냐에 따라서..
    // 벡터에 따로 담는다..
    std::vector< KPetInfo > vecPetInfo;
    std::map<GCITEMUID, KPetInfo>::iterator mitTemp;
    for (mitTemp = g_MyD3D->m_mapPetInfo.begin(); mitTemp != g_MyD3D->m_mapPetInfo.end(); ++mitTemp)
    {
        if (m_iEvolutionType == EGCPETEVOLUTIONTYPE::HATCH)
        {
            // 부화 상자 타입이면.. 펫 정보중에 알들을 모두 벡터에 담는다..
            if (mitTemp->second.m_nHatchingID != -1)
                vecPetInfo.push_back(mitTemp->second);
        }
        else
        {
            // 진화나 퇴화 상자이면.. 일반 펫정보만 담는다..
            if (mitTemp->second.m_nHatchingID == -1)
                vecPetInfo.push_back(mitTemp->second);
        }
    }

    int iSize = (int)vecPetInfo.size();
    for (int i = 0; i < iSize; ++i)
    {
        // 벡터에서 현재 펫을 찾자..
        if (vecPetInfo[i].m_dwUID == mit->second.m_dwUID)
        {
            if (bRight)
                kPetInfo = vecPetInfo[(iSize + i + 1) % iSize];
            else
                kPetInfo = vecPetInfo[(iSize + i - 1) % iSize];
        }
    }

    SetEvolutionPet(kPetInfo.m_dwUID, m_iEvolutionType, m_dwUseItemID);
}

void KGCPetEvolutionBox::OnDestroyComplete(void)
{
    SpeakToActionListener(KActionEvent(this, DESTROYED));
}

int KGCPetEvolutionBox::GetMinNeedPetLevel(DWORD dwUseItemID)
{
    //ITEM_PET_EVOLUTION_ITEM_CASH5000
    //ITEM_PET_EVOLUTION_ITEM_CASH3000
    //ITEM_PET_EVOLUTION_ITEM_POINT
    //ITEM_PET_EVOLUTION_ITEM_GP

    switch (dwUseItemID)
    {
    case KGCItemManager::ITEM_PET_EVOLUTION_ITEM_CASH5000:
        return 0;

    case KGCItemManager::ITEM_PET_EVOLUTION_ITEM_CASH3000:
        return 20;

    case KGCItemManager::ITEM_PET_EVOLUTION_ITEM_POINT:
        return 0;

    case KGCItemManager::ITEM_PET_EVOLUTION_ITEM_GP:
        return 40;

    case KGCItemManager::ITEM_PET_HATCH_ITEM_GP:
        return 7;

    case KGCItemManager::ITEM_PET_DEVOLUTION_ITEM_GP:
    case KGCItemManager::ITEM_PET_DEVOLUTION_ITEM_POINT:
        return 0;
    }

    return 40;
}

int KGCPetEvolutionBox::GetCurrentPetID()
{
    return m_pkCurrentPetWnd->GetPetID();
}
int KGCPetEvolutionBox::GetNextPetID()
{
    return m_pkNextPetWnd->GetPetID();
}
int KGCPetEvolutionBox::GetCurrentPetEvolutionLevel()
{
    return m_pkCurrentPetWnd->GetPetEvolutionLevel();
}
int KGCPetEvolutionBox::GetNextPetEvolutionLevel()
{
    return m_pkNextPetWnd->GetPetEvolutionLevel();
}

bool KGCPetEvolutionBox::CheckCondition(IN const KDialogInfo& kInfo_)
{
    std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.find(kInfo_.m_lParam2);
    if (mit == g_MyD3D->m_mapPetInfo.end())
    {
        ASSERT(!"Pet정보가 없다");
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_INVAILD_ITEM), g_pkStrLoader->GetString(STR_ID_INQUIRE_TO_CUSTOMER_SUPPORT));
        return false;
    }
    return true;
}

bool KGCPetEvolutionBox::InitDialog(IN const KDialogInfo& kInfo_)
{
    if (this->GetPetUID() == 0)
    {
        int iEvolutionType = 0;
        switch (kInfo_.m_nMsgBoxUse)
        {
        case KGCUIScene::GC_MBOX_USE_PET_EVOLUTION_BOX:
            iEvolutionType = 0;
            break;
        case KGCUIScene::GC_MBOX_USE_PET_DEVOLUTION_BOX:
            iEvolutionType = 1;
            break;
        case KGCUIScene::GC_MBOX_USE_PET_HATCH_BOX:
            iEvolutionType = 2;
            break;
        }
        this->SetEvolutionPet(kInfo_.m_lParam2, iEvolutionType, static_cast<GCITEMID>(kInfo_.m_lParam));
    }
    return true;
}