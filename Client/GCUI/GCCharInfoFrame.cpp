#ifndef NEW_CHAR_SELECT_SCENE
#include "stdafx.h"
#include "GCCharInfoFrame.h"
#include "MyD3D.h"
#include "KGCRoomManager.h"
#include "GCIconImageManager.h"

#include "GCUI/GCCharIcon.h"

#include "GCUI/GCAccountNickNameFrame.h"
#include "KGCGiftBoxManager.h"
#include <GCUtil.h>

IMPLEMENT_CLASSNAME(KGCCharInfoFrame);
IMPLEMENT_WND_FACTORY(KGCCharInfoFrame);
IMPLEMENT_WND_FACTORY_NAME(KGCCharInfoFrame, "gc_char_info_frame");

KGCCharInfoFrame::KGCCharInfoFrame(void)
    : m_pkGuildMarkTex(NULL)
    , m_pTexTitleItem(NULL)
{
    m_iExtraCharNum = 0;
    m_pWaitMotion = NULL;
    m_pActionMotion = NULL;
    m_pMeshKOGRoom = NULL;
    m_pShopMotion = NULL;

    for (int i = 0; i < GC_CHAR_NUM; i++)
    {
        m_pkCharIcon[i] = NULL;
        char temp[MAX_PATH];
        sprintf(temp, "char%d", i);
        LINK_CONTROL(temp, m_pkCharIcon[i]);
    }

    m_pkExpGaugeBar = NULL;
    LINK_CONTROL("exp_gaugebar", m_pkExpGaugeBar);
    m_pkExpGaugeRect = NULL;
    LINK_CONTROL("exp_gauge_rect", m_pkExpGaugeRect);
    m_pkExpGauge = NULL;
    LINK_CONTROL("exp_gauge", m_pkExpGauge);
    m_pkStaticExpValue = NULL;
    LINK_CONTROL("exp_value", m_pkStaticExpValue);

    m_pkGuildMarkRect = NULL;
    LINK_CONTROL("guildmark_rect", m_pkGuildMarkRect);

    m_pkCharIconRect = NULL;
    LINK_CONTROL("char_icon_rect", m_pkCharIconRect);

    m_pkStaticRecord = NULL;
    LINK_CONTROL("record", m_pkStaticRecord);
    m_pkStaticAutoRecord = NULL;
    LINK_CONTROL("autorecord", m_pkStaticAutoRecord);

    m_pkStaticCharName = NULL;
    LINK_CONTROL("char_name", m_pkStaticCharName);
    m_pkStaticUserName = NULL;
    LINK_CONTROL("user_name", m_pkStaticUserName);
    m_pkStaticJobName = NULL;
    LINK_CONTROL("job_name", m_pkStaticJobName);

    m_pkStaticCharLV = NULL;
    LINK_CONTROL("char_lv", m_pkStaticCharLV);


    m_pkStaticGuildExplain = NULL;
    LINK_CONTROL("guild_explain", m_pkStaticGuildExplain);

    m_pkNoGuildMark = NULL;
    LINK_CONTROL("no_guild", m_pkNoGuildMark);

    m_pkBtn_Confirm = NULL;
    LINK_CONTROL("btn_confirm", m_pkBtn_Confirm);

    m_pkCharWnd = NULL;
    LINK_CONTROL("character_window", m_pkCharWnd);

    m_pkCharFocus = NULL;
    LINK_CONTROL("char_select_focus", m_pkCharFocus);
    m_pkFocusingIcon = NULL;

    m_bEmptyNoHaveChar = false;

    m_pkBtnMoveChannel = NULL;
    LINK_CONTROL("btn_move_channel", m_pkBtnMoveChannel);

    m_pkNoChar1 = NULL;
    LINK_CONTROL("nochar1", m_pkNoChar1);

    m_pkNoChar2 = NULL;
    LINK_CONTROL("nochar2", m_pkNoChar2);

    m_pkInfoTitle = NULL;
    LINK_CONTROL("info_title", m_pkInfoTitle);

    m_pkInfoTitleMatching = NULL;
    LINK_CONTROL("info_title_matching", m_pkInfoTitleMatching);

    m_pkNickNameFrame = NULL;
    LINK_CONTROL("nickname_frame", m_pkNickNameFrame);

    m_pkBackGroundWhite = NULL;
    LINK_CONTROL("background_white", m_pkBackGroundWhite);

    m_pkNameBack = NULL;
    LINK_CONTROL("name_back", m_pkNameBack);
    m_pkVdepot = NULL;
    LINK_CONTROL("btn_vdepot", m_pkVdepot);

    m_pkSloatAddIcon = NULL;
    LINK_CONTROL("add_slot_icon", m_pkSloatAddIcon);
}

KGCCharInfoFrame::~KGCCharInfoFrame(void)
{
    m_vecEmptySlot.clear();
}

void KGCCharInfoFrame::ActionPerformed(const KActionEvent& event)
{
    GCWND_MSG_MAP(m_pkSloatAddIcon, KD3DWnd::D3DWE_BUTTON_CLICK, OnAddSlot);
    GCWND_MSG_MAP(m_pkBtnMoveChannel, KD3DWnd::D3DWE_BUTTON_CLICK, OnMoveChannel);

    if (event.m_dwCode == D3DWE_BUTTON_CLICK)
    {
        if (m_pkBtn_Confirm == event.m_pWnd)
        {
            SendChangeChange();
        }
        if (m_pkBtn_Make_char == event.m_pWnd)
            g_MyD3D->m_pStateMachine->GoCharGain();

        if (m_pkVdepot == event.m_pWnd)
        {
            CreateVDepot();
        }
    }

    if (event.m_dwCode == D3DWE_BUTTON_CLICK)
    {
        for (int i = 0; i < (int)m_vecEmptySlot.size(); ++i)
        {
            if (m_vecEmptySlot[i] == event.m_pWnd)
            {
                g_MyD3D->m_pStateMachine->GoCharGain();
            }
        }
    }

    for (int i = 0; i < GC_CHAR_NUM; i++)
    {
        if (m_pkCharIcon[i] == event.m_pWnd)
        {
            if (m_pkCharIcon[i]->IsLockChar())
                continue;
            if (event.m_dwCode == D3DWE_BUTTON_CLICK)
            {
                m_iExtraCharNum = i;
                OnClickChangeCharacter(event);
            }
            if (event.m_dwCode == D3DWE_DOUBLE_CLICK)
            {
                m_iExtraCharNum = i;
                OnDblClickChangeCharacter(event);
            }
        }
    }
}

bool KGCCharInfoFrame::OnDblClickChangeCharacter(const KActionEvent& event)
{
    OnClickChangeCharacter(event);
    OnClickChannelBtn();
    return true;
}

void KGCCharInfoFrame::OnClickChannelBtn()
{
    //here has a code for return guide message box, I should see if we'll put this trash in our main
    if (g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM)
    {
        this->GoRoom();
    }
    else
    {
        this->GoChannel();
    }

    this->m_pkBackGroundWhite->ToggleRender(false);
}


bool KGCCharInfoFrame::OnClickChangeCharacter(const KActionEvent& event)
{
    KCharInfoVector::iterator vIter = g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.begin();
    for (int k = 0; vIter != g_MyD3D->m_TempPlayer.m_kUserInfo.vecCharInfo.end(); vIter++)
    {
        std::wstring currentChar = g_kGlobalValue.GetCharName(vIter->iCharType);
        std::wstring futureCharName = g_kGlobalValue.GetCharName(m_iExtraCharNum);

        GCUTIL_STR::ToLower(currentChar);
        GCUTIL_STR::ToLower(futureCharName);

        if (currentChar == futureCharName)
        {
            g_MyD3D->m_TempPlayer.m_kUserInfo.cCharIndex = static_cast<char>(k);
            g_kGlobalValue.m_kUserInfo.cCharIndex = static_cast<char>(k);
            m_pkFocusingIcon = m_pkCharIcon[g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType];
            break;
        }
        k++;
    }

    SetSelectedCharInfo();
    ChangeCharacter();

    return true;
}

void KGCCharInfoFrame::ChangeCharacter()
{
    m_pkCharWnd->UpdateMeshObject();
    m_pkCharWnd->SetChar(g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType);
}

void KGCCharInfoFrame::OnCreate(void)
{

    for (int i = 0; i < GC_CHAR_NUM; i++)
    {
        if (m_pkCharIcon[i])
            m_pkCharIcon[i]->InitState(false, true, this);
    }
    m_pkBtn_Confirm->InitState(true, true, this);
    m_pkBtnMoveChannel->InitState(true, true, this);

    m_pkCharIconRect->InitState(true);
    m_pkExpGaugeRect->InitState(false);
    m_pkExpGauge->InitState(true);
    m_pkExpGaugeBar->InitState(true);
    m_pkStaticExpValue->InitState(true);
    m_pkStaticExpValue->SetFontSize(10);
    m_pkStaticExpValue->SetFontOutline(true);

    m_pkGuildMarkRect->InitState(false);
    m_pkNoGuildMark->InitState(false);

    m_pkStaticRecord->InitState(true);
    m_pkStaticRecord->SetFontSize(13);
    m_pkStaticRecord->SetAlign(DT_LEFT);
    m_pkStaticAutoRecord->InitState(false);
    m_pkStaticAutoRecord->SetFontSize(13);
    m_pkStaticAutoRecord->SetAlign(DT_LEFT);

    m_pkStaticCharName->InitState(true);
    m_pkStaticCharName->SetFontSize(13);
    m_pkStaticCharName->SetAlign(DT_LEFT);

    m_pkStaticJobName->InitState(true);
    m_pkStaticJobName->SetFontSize(13);
    m_pkStaticJobName->SetAlign(DT_LEFT);

    m_pkStaticUserName->InitState(true);
    m_pkStaticUserName->SetFontSize(16);
    m_pkStaticUserName->SetAlign(DT_CENTER);
    m_pkStaticUserName->SetShadow(true);
    m_pkStaticUserName->SetShadowColor(0xFF000000);
    m_pkStaticCharLV->InitState(true);
    m_pkStaticCharLV->SetFontSize(14);
    m_pkStaticCharLV->SetAlign(DT_LEFT);
    m_pkStaticGuildExplain->InitState(true);
    m_pkStaticGuildExplain->SetFontSize(12);
    m_pkStaticGuildExplain->SetAlign(DT_LEFT);

    m_pkCharWnd->InitState(true, true);

    m_pkCharWnd->SetUseRoomObject(false);

    m_pkCharFocus->InitState(false);

    m_bEmptyNoHaveChar = GCFUNC::GetCharListWithoutHave().empty();

    g_pkUIMgr->DisconnectGlobalEventProcedure(GetUID(), KGCUIScene::D3DWE_BUT_ADD_SLOT_ITEM);
    g_pkUIMgr->ConnectGlobalEventProcedure(KGCUIScene::D3DWE_BUT_ADD_SLOT_ITEM, this, &KGCCharInfoFrame::RecvBuyItem);

    m_pkNoChar1->InitState(false);
    m_pkNoChar2->InitState(false);
    m_pkInfoTitle->InitState(true);
    m_pkInfoTitleMatching->InitState(false);
    m_pkNameBack->InitState(true);

    m_pkNickNameFrame->InitState(false, true, this);
    m_pkBackGroundWhite->InitState(false);
    m_pkBackGroundWhite->ForceSetWindowColor(D3DCOLOR_ARGB(0xb0, 0x00, 0x00, 0x00));

    m_pkVdepot->InitState(false, true, this);
    m_pkVdepot->SetToolTip(g_pkStrLoader->GetString(STR_ID_VIRTUAL_DEPOT_MSG3), 0.0f, 0.0f, WNDUIAT_EXPANTION, ED3DWND_TOOLTIP_ALIGN_RIGHT);

    m_pkSloatAddIcon->InitState(false, true, this);
}

void KGCCharInfoFrame::FrameMoveInEnabledState(void)
{
    /*if (g_MyD3D->m_pStateMachine->GetState() == GS_ROOM) {
        m_pkBtnMoveChannel->ToggleRender(false);
    }
    else {
        m_pkBtnMoveChannel->ToggleRender(true);
    }*/

    if (m_pkFocusingIcon)
    {
        D3DXVECTOR2 vFocusingPos = m_pkFocusingIcon->GetFixedWindowLocalPos();
        vFocusingPos.x -= (m_pkCharFocus->GetWidth() - m_pkFocusingIcon->GetWidth()) / 2;
        vFocusingPos.y -= (m_pkCharFocus->GetHeight() - m_pkFocusingIcon->GetHeight()) / 2;

        m_pkCharFocus->SetFixedWindowLocalPos(vFocusingPos);
        m_pkCharFocus->ToggleRender(true);
    }
    else
    {
        m_pkCharFocus->ToggleRender(false);
    }


    if (SiKGCVirtualDepotManager()->GetRecvPacket())
    {
        if (!g_kGlobalValue.m_kUserInfo.vecCharInfo.empty())
        {
            if (SiKGCVirtualDepotManager()->GetFirstLogOn())
            {
                CreateVDepot();
                SiKGCVirtualDepotManager()->SetFirstLogOn(false);
            }

            if (SiKGCVirtualDepotManager()->IsEmptyItemList())
            {
                m_pkVdepot->ToggleRender(false);
                m_pkVdepot->SetWndMode(D3DWM_LOCK);
            }
            else
            {
                m_pkVdepot->ToggleRender(true);
                m_pkVdepot->SetWndMode(D3DWM_DEFAULT);
            }
            SiKGCVirtualDepotManager()->SetRecvPacket(false);
        }
        else
            SiKGCVirtualDepotManager()->SetFirstLogOn(false);

    }
}

void KGCCharInfoFrame::SettingCharIconPos()
{
    for (int i = 0; i < GC_CHAR_NUM; i++)
    {
        m_pkCharIcon[i]->ToggleRender(false);
    }
    // 	DWORD dwIconStandardWidth = m_pkCharIcon[0]->GetWidth();
    // 	DWORD dwIconStandardHeight = m_pkCharIcon[0]->GetHeight();
    // 
    // 	DWORD dwIconDrawRecWidth = m_pkCharIconRect->GetWidth();

    KCharInfoVector& vecChar = g_kGlobalValue.m_kUserInfo.vecCharInfo;

    //	int nColumn =(dwIconDrawRecWidth+ICON_GAP) / (dwIconStandardWidth+ICON_GAP) -1;
        //int nRow = vecChar.size()/nColumn + 1;

    D3DXVECTOR2 vStartPos = m_pkCharIconRect->GetFixedWindowLocalPos();


    int iIndex = g_kGlobalValue.m_kUserInfo.vecCharInfo.size();

    KCharInfoVector::iterator vIter = vecChar.begin();
    for (; vIter != vecChar.end(); ++vIter)
    {
        int iCharIndex = static_cast<int>(vIter->iCharType);
        if (m_pkCharIcon[iCharIndex])
        {
            std::map< int, KSealedCharInfo >::iterator mapSealedIter = g_kGlobalValue.m_mapSealedCharInfo.find(iCharIndex);
            if (mapSealedIter != g_kGlobalValue.m_mapSealedCharInfo.end())
            {
                m_pkCharIcon[iCharIndex]->SetLockIcon(mapSealedIter->second.m_bIsSealed);
                if (mapSealedIter->second.m_bIsSealed)
                {
                    m_pkCharIcon[iCharIndex]->SetToolTip(g_pkStrLoader->GetString(STR_ID_CHAR_SEAL_MSG_1), 0.0f, 0.0f, KD3DWnd::WNDUIAT_EXPANTION, KD3DWnd::ED3DWND_TOOLTIP_ALIGN_LEFT_TEXT_ALIGN_LEFT);
                    m_pkCharIcon[iCharIndex]->Lock(true);
                    m_pkCharIcon[iCharIndex]->SetSelfInputCheck(false);

                }
                else {
                    m_pkCharIcon[iCharIndex]->SetToolTip(L"");
                    m_pkCharIcon[iCharIndex]->Lock(false);
                    m_pkCharIcon[iCharIndex]->SetSelfInputCheck(true);
                }
            }

            m_pkCharIcon[iCharIndex]->ToggleRender(true);
        }
    }

    UpdateCharSlot();
    UpdateEmptySlot();
    UdateSlotAddIcon();
}

void KGCCharInfoFrame::SetSelectedChar()
{
    for (int i = 0; i < GC_CHAR_NUM; i++)
    {
        if (m_pkCharIcon[i]->IsRenderOn())
            m_pkCharIcon[i]->SetWndMode(D3DWM_DEFAULT);
    }

    SetSelectedCharInfo();
    m_pkCharWnd->UpdateMeshObject();
    m_pkCharWnd->SetChar(g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType);
}

void KGCCharInfoFrame::SetSelectedCharInfo()
{
    PLAYER* pPlayer = NULL;
    pPlayer = &g_MyD3D->m_TempPlayer;

    std::wostringstream strRecord;

    // 자유 대전
    int iWin = pPlayer->m_kUserInfo.GetCurrentChar().iWin;
    int iLose = pPlayer->m_kUserInfo.GetCurrentChar().iLose;

    if (iWin + iLose > 0)
    {
        int iWinRate = 0;
        iWinRate = (iWin * 100) / (iWin + iLose);

        strRecord << iWin << g_pkStrLoader->GetString(STR_ID_WIN).c_str()
            << " " << iLose << g_pkStrLoader->GetString(STR_ID_LOSE).c_str()
            << " " << g_pkStrLoader->GetString(STR_ID_WINRATE).c_str() << iWinRate << "%";

        m_pkStaticRecord->SetText(strRecord.str());
    }
    else
    {
        m_pkStaticRecord->SetText(g_pkStrLoader->GetString(STR_NO_MATCH_COUNT));
    }

    //  대전 매칭
    int iWinAuto = pPlayer->m_kUserInfo.GetCurrentChar().kELOUserData.m_nELOWin;
    int iLoseAuto = pPlayer->m_kUserInfo.GetCurrentChar().kELOUserData.m_nELOLose;
    int iTotal = pPlayer->m_kUserInfo.GetCurrentChar().kELOUserData.m_nMatchTotalCount;
    if (iTotal > 0)
    {
        std::wostringstream strAutoRecord;
        int iWinRaitAuto = (iWinAuto * 100) / iTotal;

        strAutoRecord << iWinAuto << g_pkStrLoader->GetString(STR_ID_WIN).c_str()
            << " " << iLoseAuto << g_pkStrLoader->GetString(STR_ID_LOSE).c_str()
            << " " << g_pkStrLoader->GetString(STR_ID_WINRATE).c_str() << iWinRaitAuto << "%";

        m_pkStaticAutoRecord->SetText(strAutoRecord.str());
    }
    else
    {
        m_pkStaticAutoRecord->SetText(g_pkStrLoader->GetString(STR_NO_MATCH_COUNT));
    }

    float fPercent = GetCurrentExpPercent(pPlayer->GetCurrentChar().biExp);
    DWORD dwPercentWidth = static_cast<DWORD>(m_pkExpGaugeRect->GetWidth() * fPercent / 100);
    D3DXVECTOR2 vGaugePos = m_pkExpGaugeRect->GetFixedWindowLocalPos();

    m_pkExpGauge->SetWidth(dwPercentWidth);
    m_pkExpGauge->SetFixedWindowLocalPos(vGaugePos);

    WCHAR strTemp[10];
    swprintf(strTemp, 10, L"%3.1f%%", fPercent);
    m_pkStaticExpValue->SetText(strTemp);


    m_pkStaticUserName->SetText( L"#c" + pPlayer->GetStrUserColor() + pPlayer->GetStrUserName() + L"#cx" );

    std::wstring strCharName = g_kGlobalValue.GetCharName(pPlayer->GetCurrentChar().iCharType);
    std::wstring strJobName = g_kGlobalValue.GetJopName(pPlayer->GetCurrentChar().iCharType, pPlayer->GetCurrentChar().iPromotionLevel);

    m_pkStaticCharName->SetText(strCharName);
    m_pkStaticJobName->SetText(strJobName);
    m_pkStaticCharLV->SetText(pPlayer->GetCurrentChar().iLevel);


    if (g_kGlobalValue.m_kGuildInfo.m_dwUID == 0 ||
        g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_JOINER ||
        g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_DRUMOUT)
    {
        m_pkStaticGuildExplain->SetText(g_pkStrLoader->GetString(STR_ID_NOT_JOIN_GUILD));
        m_pkNoGuildMark->ToggleRender(true);
    }
    else
    {
        m_pkStaticGuildExplain->SetText(g_kGlobalValue.m_kGuildInfo.m_strName);

        m_pkGuildMarkTex = SiKGCIconImageManager()->GetTexture(g_kGlobalValue.m_kGuildInfo.m_strFileName);

        if (m_pkGuildMarkTex == NULL)
        {
            SiKGCIconImageManager()->LoadIconImageFromHTTP(g_kGlobalValue.m_kGuildInfo.m_strFileName);
            m_pkGuildMarkTex = SiKGCIconImageManager()->GetTexture(g_kGlobalValue.m_kGuildInfo.m_strFileName);
        }
    }
}

void KGCCharInfoFrame::OnInitialize()
{
    m_pkBtn_Confirm->Lock(g_kGlobalValue.m_kUserInfo.vecCharInfo.empty());

    UpdateUIVisible();
    SettingCharIconPos();
    SetChildOrderTop(m_pkBackGroundWhite);
    SetChildOrderTop(m_pkNickNameFrame);

    if (!g_kGlobalValue.m_kUserInfo.vecCharInfo.empty())
    {
        SetSelectedChar();
    }
    if (g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType != -1)
    {
        m_pkFocusingIcon = m_pkCharIcon[g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType];
    }
    else
    {
        m_pkFocusingIcon = NULL;
    }

    m_bEmptyNoHaveChar = GCFUNC::GetCharListWithoutHave().empty();

    if (g_kGlobalValue.m_kUserInfo.strNickName == L"")
    {
        m_pkNickNameFrame->ToggleRender(true);
        m_pkBackGroundWhite->ToggleRender(true);
        LockEmptySlot(true);
    }
    else
    {
        m_pkNickNameFrame->ToggleRender(false);
        m_pkBackGroundWhite->ToggleRender(false);
        LockEmptySlot(false);
    }

    if (SiKGCVirtualDepotManager()->IsEmptyItemList())
    {
        m_pkVdepot->ToggleRender(false);
        m_pkVdepot->SetWndMode(D3DWM_LOCK);
    }
}


void KGCCharInfoFrame::OnDestroyComplete(void)
{
    m_pkCharWnd->UpdateMeshObject();
}

void KGCCharInfoFrame::OnTutorial()
{

    SiGCSKT()->SetOwnerPlayer(g_MyD3D->MyPlayer[0]);
    SiGCSKT()->SetCharacterSelectable(false);
    SiGCSKT()->SetUnlockAll(true);

    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_TUTORIAL_OPTION_NEW);
}


void KGCCharInfoFrame::PostChildDraw()
{
    if (g_kGlobalValue.m_kGuildInfo.m_dwUID == 0)
        return;

    if (m_pkGuildMarkTex == NULL)
        return;

    D3DXVECTOR2 vPos = GetRelocatedWindowPos();
    vPos.x += m_pkGuildMarkRect->GetFixedWindowLocalPos().x * m_fWindowScaleX;
    vPos.y += m_pkGuildMarkRect->GetFixedWindowLocalPos().y * m_fWindowScaleY;

    g_pGCDeviceManager2->DrawInScreen(m_pkGuildMarkTex,
        vPos.x, vPos.y,
        vPos.x + m_pkGuildMarkRect->GetWidth() * m_fWindowScaleX, vPos.y + m_pkGuildMarkRect->GetHeight() * m_fWindowScaleY,
        0.0f, 0.0f, 1.0f, 1.0f, false, 10.0f, 0xffffffff);

}

void KGCCharInfoFrame::SendChangeChange() {
    //here has a code for return guide message box, I should see if we'll put this trash in our main
    if (g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM)
    {
        this->GoRoom();
    }
    else
    {
        this->GoChannel();
    }

    this->m_pkBackGroundWhite->ToggleRender(false);
}

void KGCCharInfoFrame::GoChannel()
{
    if (g_kGlobalValue.m_bSendServerInfoNot == false)
    {
        KP2P::GetInstance()->Send_CharSelectJoinReq();
        KP2P::GetInstance()->Send_ChoiceBoxListReq();
        KP2P::GetInstance()->Send_ExpPotionListReq();
#if defined( USE_AGIT_SYSTEM )
        SiKGCAgitGameManager()->SetAgitShop();
        KP2P::GetInstance()->Send_FairyTreeLvTableReq();
#endif

        KP2P::GetInstance()->Send_MyDonationInfoReq();
        KP2P::GetInstance()->Send_RecommendFullInfoReq();

        KP2P::GetInstance()->Send_PetCostumeListReq();
        SiKGCInviteManager()->SetDenyInviteMsg(g_MyD3D->m_KGCOption.IsDenyInviteMsg(), true);

        g_kGlobalValue.m_bSendServerInfoNot = true;
    }

    int iCharType = g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;

    if (iCharType != g_kGlobalValue.GetSavedCharType())
    {
        SiKGCGiftBoxManager()->Reset();
        KP2P::GetInstance()->Send_ChangeCharReq(g_kGlobalValue.GetSavedCharType(), iCharType);

        Result_ChangeChar = INT_MAX;
        g_MyD3D->WaitForServerAck(Result_ChangeChar, INT_MAX, 30000, TIME_OUT_VALUE);

        if (Result_ChangeChar != 0)
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_GET_INVEN_ERROR),
                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            g_MyD3D->OnServerSceneBtn();
            return;
        }
    }

#if defined(NATION_CHINA)
    KP2P::GetInstance()->Send_CYOU_UserLogin_Req(g_kGlobalValue.GetSavedCharType(), iCharType);
#endif

    SiKGCPrivateDungeonManager()->MoveStateCondition(EM_CREATE_ROOM_TYPE);

#ifdef AUTO_CITY
    g_MyD3D->m_pStateMachine->GoWorldMap();

    if (g_MyD3D->m_pStateMachine->IsJoinedChannel())
    {
        SiGCSquare()->SetJoinedChannel(true);
        SiGCSquare()->SetChannelID(SiKGCChannelManager()->GetCurrentChannel());
        SiGCSquare()->SetJoinedGuild(false);
    }

    SiGCSquare()->SetPrevState(g_MyD3D->m_pStateMachine->GetState());

    std::vector< KSquareInfo > vecSquareInfo = SiGCSquare()->GetSquareInfo();
    if (vecSquareInfo.size() > 0) {
        KSquareInfo     m_kSquareInfo = vecSquareInfo[0];
        SiGCSquare()->SetSquareID(m_kSquareInfo.m_dwID);
        g_MyD3D->m_pStateMachine->GoSquareLoading();
    }
#else
    g_MyD3D->GoChannel();
#endif
}

void KGCCharInfoFrame::GoRoom()
{
    int iCharType = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType;
    if (SiKGCPrivateDungeonManager()->IsHavePrivateDungeon(iCharType))
    {
        if (g_MyD3D->m_TempPlayer.GetCurrentCharLevel() < 18)
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NO_CLEAR_PRIVATE_DUNGEON));
            return;
        }
    }

    if (iCharType != g_kGlobalValue.GetSavedCharType())
    {
        SiKGCGiftBoxManager()->Reset();
        KP2P::GetInstance()->Send_InRoomChangeCharReq(g_kGlobalValue.GetSavedCharType(), iCharType);

        Result_ChangeChar = INT_MAX;
        g_MyD3D->WaitForServerAck(Result_ChangeChar, INT_MAX, 30000, TIME_OUT_VALUE);

        if (Result_ChangeChar != 0)
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_GET_INVEN_ERROR),
                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            g_MyD3D->OnServerSceneBtn();
            return;
        }
    }

    if (g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM)
    {
        KP2P::GetInstance()->Send_InRoomChangeIndoorUserInfoReq(&g_kGlobalValue.m_kUserInfo);
        Result_ReceiveInRoomIndoorChangeReq = INT_MAX;
        g_MyD3D->WaitForServerAck(Result_ReceiveInRoomIndoorChangeReq, INT_MAX, 5000, TIME_OUT_VALUE);

        if (Result_ReceiveInRoomIndoorChangeReq != 0)
        {
            g_MyD3D->m_pStateMachine->GoMatch();
            return;
        }

        g_MyD3D->m_pStateMachine->GoRoom();
        g_pkUIHelper->SetUILockTime(LOCK_ROOM, 0);
        KP2P::GetInstance()->Send_Into_Myinfo(g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strLogin, GC_RUS_WAIT);
    }
}

void KGCCharInfoFrame::UpdateMeshObject(void)
{
    m_Player.SetMyPlayer(-1, true);
    m_Player.ReleasePlayerResource();

    m_Player.AutoSetExtra();

    int iBeforeExtraCharNum = m_iExtraCharNum;
    m_iExtraCharNum = m_Player.Extra_Char_Num;

    //  캐릭터가 바꼈을 경우
    if (m_iExtraCharNum != iBeforeExtraCharNum)
    {
        ReloadMotion();
    }

    if (m_pWaitMotion != NULL)
        m_pShopMotion = m_pWaitMotion;


    m_Player.m_iPlayerIndex = 0;
    m_Player.m_kUserInfo.dwUID = g_kGlobalValue.m_kUserInfo.dwUID;

    m_Player.UpdatePlayerResource(true);

    switch (m_iExtraCharNum % GC_CHAR_NUM)
    {
    case GC_CHAR_ZERO:
        m_Player.ToggleExtraMesh("GRANDARK_2ND", false);
        m_Player.ToggleExtraMesh("GRANDARK_2ND_1", false);
        m_Player.ToggleExtraMesh("GRANDARK_2ND_2", false);
        m_Player.ToggleExtraMesh("DOG_GRANDARK", false);
        break;
    case GC_CHAR_LUPUS:
        m_Player.ToggleExtraMesh("WEAPON_0", true);
        m_Player.ToggleExtraMesh("WEAPON_1", true);
        break;
    default:
        m_Player.ToggleExtraMesh("WEAPON", m_Player.GetCurFormTemplate().WeaponRenderBasically);
        m_Player.ToggleExtraMesh("MERGE_WEAPON", m_Player.GetCurFormTemplate().LeftWeaponRenderBasically);
        break;
    }

    if (m_Player.m_pObject)
        m_Player.m_pObject->SetShellMatrixScale(0.25f);

    if (!m_pMeshKOGRoom)
    {
        m_pMeshKOGRoom = new GCObject();
#ifdef _DEBUG
        m_pMeshKOGRoom->m_strName = "룸";
#endif
    }

    int iTitleITemID = g_pItemMgr->GetEquipTitleItem(&m_Player.GetCurrentChar());
    SetTitleItemID(iTitleITemID);

}

void KGCCharInfoFrame::SetTitleItemID(int iTitleItemID)
{
    SAFE_RELEASE(m_pTexTitleItem);

    if (-1 == iTitleItemID)
        return;

    GCItem* pItem = g_pItemMgr->GetItemData(iTitleItemID);
    if (NULL == pItem)
        return;

    if (GIK_TITLE_ITEM == pItem->eItemKind
        || g_pItemMgr->IsRallyTitleItemID(pItem->dwGoodsID))
    {
        m_pTexTitleItem = g_pItemMgr->CreateItemTexture(iTitleItemID, 0);
        if (0 == m_pTexTitleItem->GetDeviceTexture())
            SAFE_RELEASE(m_pTexTitleItem);
    }
}

void KGCCharInfoFrame::ReloadMotion()
{
    GCFUNC::CreateCharViewMotion(m_iExtraCharNum, &m_pWaitMotion, &m_pActionMotion);
}


void KGCCharInfoFrame::CreateEmptySlot()
{
    for (int i = 0; i < (int)m_vecEmptySlot.size(); ++i)
    {
        m_vecEmptySlot[i]->ToggleRender(false);
    }

    int nSlotCount = g_kGlobalValue.m_kUserInfo.m_nSlotCount;

    if ((int)m_vecEmptySlot.size() < nSlotCount)
    {
        int nCount = nSlotCount - m_vecEmptySlot.size();

        for (int i = 0; i < nCount; ++i)
        {
            KD3DWnd* pWnd = NULL;

            pWnd = g_pkUIMgr->CreateTemplateUIByFindName(this, "ui_scene_char_select.stg", "window", "char_select_scene\\char_info_frame\\charempty");
            if (pWnd == NULL)
                return;

            pWnd->InitState(false, true, this);

            m_vecEmptySlot.push_back(pWnd);

        }
    }
}

void KGCCharInfoFrame::UpdateCharSlot()
{
    DWORD dwIconStandardWidth = m_pkCharIcon[0]->GetWidth();
    DWORD dwIconStandardHeight = m_pkCharIcon[0]->GetHeight();
    DWORD dwIconDrawRecWidth = m_pkCharIconRect->GetWidth();
    D3DXVECTOR2 vStartPos = m_pkCharIconRect->GetFixedWindowLocalPos();

    int nHaveChar = g_kGlobalValue.m_kUserInfo.vecCharInfo.size();

    int nColumn = (dwIconDrawRecWidth + ICON_GAP) / (dwIconStandardWidth + ICON_GAP);
    int cntColumn = 0;
    int cntRow = 0;

    for (int i = 0; i < nHaveChar; ++i)
    {
        int nCharType = g_kGlobalValue.m_kUserInfo.vecCharInfo[i].iCharType;
        cntColumn = i % nColumn;
        cntRow = i / nColumn;

        float fPosX = vStartPos.x + (cntColumn * (dwIconStandardWidth + ICON_GAP));
        float fPosY = vStartPos.y + (cntRow * (dwIconStandardHeight + ICON_GAP));

        m_pkCharIcon[nCharType]->SetWindowPosDirect(D3DXVECTOR2(fPosX, fPosY));
    }
}

void KGCCharInfoFrame::UpdateEmptySlot()
{
    DWORD dwIconStandardWidth = m_pkCharIcon[0]->GetWidth();
    DWORD dwIconStandardHeight = m_pkCharIcon[0]->GetHeight();
    DWORD dwIconDrawRecWidth = m_pkCharIconRect->GetWidth();
    D3DXVECTOR2 vStartPos = m_pkCharIconRect->GetFixedWindowLocalPos();

    int nColumn    = (dwIconDrawRecWidth + ICON_GAP) / (dwIconStandardWidth + ICON_GAP);
    int nHaveChar  = g_kGlobalValue.m_kUserInfo.vecCharInfo.size();
    int cntColumn  = nHaveChar % nColumn;
    int cntRow     = nHaveChar / nColumn;

    CreateEmptySlot();
    int nSlotCount = g_kGlobalValue.m_kUserInfo.m_nSlotCount;

    for (int i = 0; i < nSlotCount; ++i)
    {
        cntColumn = (i + nHaveChar) % nColumn;
        cntRow = (i + nHaveChar) / nColumn;

        float fPosX = vStartPos.x + (cntColumn * (dwIconStandardWidth + ICON_GAP));
        float fPosY = vStartPos.y + (cntRow * (dwIconStandardHeight + ICON_GAP));

        m_vecEmptySlot[i]->SetWindowPosDirect(D3DXVECTOR2(fPosX, fPosY));
        m_vecEmptySlot[i]->ToggleRender(true);
    }

    if (nSlotCount > 0)
    {
        m_pkSloatAddIcon->InitState(false, false, NULL);
    }
}

void KGCCharInfoFrame::OnAddSlot()
{
    if (g_kGlobalValue.m_kUserInfo.vecCharInfo.empty())
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ADD_SLOT_CHECK_ERROR1), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
        return;
    }

    int nOpenChar = g_kGlobalValue.GetCharOpenType().size();
    int nHaveChar = g_kGlobalValue.m_kUserInfo.vecCharInfo.size();
    int nSlotCount = g_kGlobalValue.m_kUserInfo.m_nSlotCount;

    if (nOpenChar <= nHaveChar + nSlotCount)
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ADD_SLOT_CHECK_ERROR2), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
        return;
    }

    GCITEMID itemID = KGCItemManager::ITEM_ADD_SLOT_ITEM;

    //  인벤에 있는지 뒤진다.
    if (g_pItemMgr->GetInventoryItemDuration(itemID) > 0)
    {
        KP2P::GetInstance()->Send_AddSlotReq(itemID * 10);
    }
    //  일단 사고...
    else
    {
        BuyAddSlotItem(itemID);
    }
}

void KGCCharInfoFrame::OnMoveChannel()
{
    if (g_MyD3D->m_pStateMachine->GetState() == GS_CHAR_SELECT_FROM_ROOM)
    {
        g_MyD3D->m_pStateMachine->GoRoom();
        g_pkUIHelper->SetUILockTime(LOCK_ROOM, BUTTON_LOCK_FRAME);
        KP2P::GetInstance()->Send_Into_Myinfo(g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strLogin, GC_RUS_WAIT);
    }
    else
    {
        g_MyD3D->OnServerSceneBtn();
    }
}

void KGCCharInfoFrame::BuyAddSlotItem(GCITEMID itemID)
{
    if (itemID == 0) return;

    GCItem* pItem = g_pItemMgr->GetItemData(itemID);
    if (pItem == NULL) return;

    if (pItem->eMoneyType == EMT_CASH)
    {
#if defined ( USE_VIP_POINT )
        g_pItemMgr->m_bIsInPresentState = false;
        g_pItemMgr->BuyCashItemS6(itemID);
#endif
    }
    else
    {
        g_MyD3D->m_kItemMgr.BuyItem(itemID, pItem->iItemTypeValue);
    }
}

void KGCCharInfoFrame::RecvBuyItem()
{
    GCITEMID itemID = KGCItemManager::ITEM_ADD_SLOT_ITEM;

    if (g_pItemMgr->GetInventoryItemDuration(itemID) > 0)
    {
        KP2P::GetInstance()->Send_AddSlotReq(itemID * 10);
    }
}

void KGCCharInfoFrame::UpdateUIVisible()
{
    bool bNoChar = g_kGlobalValue.m_kUserInfo.vecCharInfo.empty();

    m_pkNoChar1->ToggleRender(bNoChar);
    m_pkNoChar2->ToggleRender(bNoChar);

    m_pkInfoTitle->ToggleRender(!bNoChar);
    if (GC_GLOBAL_DEFINE::bAutoMatch)
        m_pkInfoTitleMatching->ToggleRender(!bNoChar);
    else
        m_pkInfoTitleMatching->ToggleRender(false);
    m_pkNameBack->ToggleRender(!bNoChar);

    m_pkStaticUserName->ToggleRender(!bNoChar);
    m_pkStaticCharName->ToggleRender(!bNoChar);
    m_pkStaticJobName->ToggleRender(!bNoChar);
    m_pkStaticCharLV->ToggleRender(!bNoChar);
    m_pkStaticRecord->ToggleRender(!bNoChar);
    if (GC_GLOBAL_DEFINE::bAutoMatch)
        m_pkStaticAutoRecord->ToggleRender(!bNoChar);
    else
        m_pkStaticAutoRecord->ToggleRender(false);

    m_pkExpGaugeBar->ToggleRender(!bNoChar);
    m_pkExpGauge->ToggleRender(!bNoChar);
    m_pkStaticExpValue->ToggleRender(!bNoChar);

    m_pkStaticGuildExplain->ToggleRender(!bNoChar);
    m_pkNoGuildMark->ToggleRender(!bNoChar);
}

void KGCCharInfoFrame::LockEmptySlot(bool bRender_)
{
    if (true == m_vecEmptySlot.empty())
        return;

    std::vector<KD3DWnd*>::iterator vit = m_vecEmptySlot.begin();
    for (; vit != m_vecEmptySlot.end(); ++vit)
        (*vit)->Lock(bRender_);

}

void KGCCharInfoFrame::SetVitalityTooltip()
{
    KCharInfoVector& vecChar = g_kGlobalValue.m_kUserInfo.vecCharInfo;

    KCharInfoVector::iterator vIter = vecChar.begin();
    for (; vIter != vecChar.end(); ++vIter) {
        int iCharIndex = static_cast<int>(vIter->iCharType);
        m_pkCharIcon[iCharIndex]->SetToolTip(g_pkStrLoader->GetReplacedString(STR_VITALITY_STR_7, "i", g_kGlobalValue.GetVitality(iCharIndex)));
    }
}

void KGCCharInfoFrame::CreateVDepot()
{
    if (SiKGCVirtualDepotManager()->IsEmptyItemList())
        return;

    SiKGCVirtualDepotManager()->SetItemCategory();

    g_KDSound.Play("31");
    g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_VIRTUALDEPOT);
}

void KGCCharInfoFrame::UdateSlotAddIcon()
{
    DWORD dwIconStandardWidth = m_pkCharIcon[0]->GetWidth();
    DWORD dwIconStandardHeight = m_pkCharIcon[0]->GetHeight();
    DWORD dwIconDrawRecWidth = m_pkCharIconRect->GetWidth();
    D3DXVECTOR2 vStartPos = m_pkCharIconRect->GetFixedWindowLocalPos();

    int nOpenChar = g_kGlobalValue.GetCharOpenType().size();
    int nHaveChar = g_kGlobalValue.m_kUserInfo.vecCharInfo.size();
    int nSlotCount = g_kGlobalValue.m_kUserInfo.m_nSlotCount;

    int nColumn = (dwIconDrawRecWidth + ICON_GAP) / (dwIconStandardWidth + ICON_GAP);
    int cntColumn = 0;
    int cntRow = 0;

    cntColumn = (nHaveChar) % nColumn;
    cntRow = (nHaveChar) / nColumn;

    float fPosX = vStartPos.x + (cntColumn * (dwIconStandardWidth + ICON_GAP));
    float fPosY = vStartPos.y + (cntRow * (dwIconStandardHeight + ICON_GAP));

    m_pkSloatAddIcon->InitState(false, false, this);

    if (nSlotCount == 0 && nHaveChar != nOpenChar) {
        m_pkSloatAddIcon->SetWindowPosDirect(D3DXVECTOR2(fPosX, fPosY));
        m_pkSloatAddIcon->InitState(true, true, this);
    }
}
#endif