#include "stdafx.h"
#include "GCRoomUserSlotS6.h"
#include "KGCFaceLift.h"
#include "GCStateMachine.h"
#include "KGCRoomManager.h"
#include "GCCoupleMark.h"
#include "KGCCountryGroup.h"
#include "KGCPingAntenna.h"
#include "GCObjectAndMotion.h"
#include "GCRenderManager.h"

IMPLEMENT_CLASSNAME(KGCRoomUserSlotS6);
IMPLEMENT_WND_FACTORY(KGCRoomUserSlotS6);
IMPLEMENT_WND_FACTORY_NAME(KGCRoomUserSlotS6, "gc_roomuserslot_s6");

const float fCharScale = 0.4f;
const float fANOTHER_PLATER_SCALE = 0.35f;

std::vector< int > m_vecEffectID;

KGCRoomUserSlotS6::KGCRoomUserSlotS6(void)
    : m_pkLevelMark(NULL)
    , m_pkHostMark(NULL)
    , m_pkOpenMark(NULL)
    , m_pkCloseMark(NULL)
    , m_pkOpenMark2(NULL)
    , m_pkCloseMark2(NULL)
    , m_pkCloseMarkPanel(NULL)
    , m_pkCharImg(NULL)
    , m_pkCharName(NULL)
    , m_pkPingAntenna(NULL)
    , m_pkCountryGroup(NULL)
    , m_pkKickBtn(NULL)
    , m_iIndex(0)
    , m_pkPet(NULL)
    , m_iCharType(-1)
    , m_bInitChar(true)
    , m_pkSlotBack(NULL)
    , m_pkMySlotBack(NULL)
    , m_pkIdleState(NULL)
    , m_pkHurryUpBtn(NULL)
    , m_pkBackVitality(NULL)
    , m_pkGuageVitality(NULL)
    , m_pkStaticVitality(NULL)
    , m_pkGuageVitalityRect(NULL)
    , m_pkFlag(NULL)
    , m_pkCharSelBtn(NULL)
#ifdef POTION_ROOM_SHOW
    , m_pkPotionSlot(NULL)
#endif
    , m_pkRank(NULL)
    , m_pkCharBanMark(NULL)
    , m_pkRankBanMark(NULL)
    , m_pkDisableImg(NULL)
    , m_pkPetCard(NULL)
    , m_pkChangeLeader(NULL)
    , m_bBaseEquip( false )
    , m_bAnimation( true )
    , m_pkExtraObject(NULL)
{
    char str[MAX_PATH] = { 0, };

    for (int i = 0; i < EM_USER_STATE_SIZE; ++i)
    {
        m_apkUserState[i] = NULL;

        sprintf(str, "user_state%d", i);
        LINK_CONTROL(str, m_apkUserState[i]);
    }

    for (int i = 0; i < EM_LEVEL_NUM; ++i)
    {
        m_apkUserLevel[i] = NULL;

        sprintf(str, "user_level_num%d", i);
        LINK_CONTROL(str, m_apkUserLevel[i]);
    }
    LINK_CONTROL("user_level", m_pkLevelMark);
    LINK_CONTROL("host_mark", m_pkHostMark);
    LINK_CONTROL("open_mark", m_pkOpenMark);
    LINK_CONTROL("close_mark", m_pkCloseMark);
    LINK_CONTROL("open_mark2", m_pkOpenMark2);
    LINK_CONTROL("close_mark2", m_pkCloseMark2);
    LINK_CONTROL("close_mark_panel", m_pkCloseMarkPanel);
    LINK_CONTROL("char_img", m_pkCharImg);
    LINK_CONTROL("user_name", m_pkCharName);
    LINK_CONTROL("ping_antenna", m_pkPingAntenna);
    LINK_CONTROL("country_group", m_pkCountryGroup);
    LINK_CONTROL("btn_kick", m_pkKickBtn);

    LINK_CONTROL("slotback", m_pkSlotBack);
    LINK_CONTROL("myback", m_pkMySlotBack);


    LINK_CONTROL("idle_state", m_pkIdleState);
    LINK_CONTROL("hurryupbtn", m_pkHurryUpBtn);

    LINK_CONTROL("back_vitality", m_pkBackVitality);
    LINK_CONTROL("guage_vitality", m_pkGuageVitality);
    LINK_CONTROL("guage_vitality_rect", m_pkGuageVitalityRect);
    LINK_CONTROL("static_vitality", m_pkStaticVitality);

    LINK_CONTROL("flag", m_pkFlag);

    LINK_CONTROL("char_sel_btn", m_pkCharSelBtn);

#ifdef POTION_ROOM_SHOW
    LINK_CONTROL("potion_room_slot", m_pkPotionSlot);
#endif

    LINK_CONTROL("rank", m_pkRank);
    LINK_CONTROL("disable_img", m_pkDisableImg);
    LINK_CONTROL("char_ban_mark", m_pkCharBanMark);
    LINK_CONTROL("rank_ban_mark", m_pkRankBanMark);

    LINK_CONTROL("pet_card_image", m_pkPetCard);

    LINK_CONTROL("btn_change_ownership", m_pkChangeLeader);

#ifdef ENCHANT_EFFECT
    m_bRenderEnchatEffect = false;
    m_vecEnchantParticle.clear();
#endif

}

KGCRoomUserSlotS6::~KGCRoomUserSlotS6(void)
{
    if (m_pkPetCard)
        m_pkPetCard->ResetItem();
    if (m_pkPet)
        m_pkPet->EndPet();
    if (m_pkExtraObject)
        SAFE_DELETE(m_pkExtraObject);
    SAFE_DELETE(m_pkPet);
}

void KGCRoomUserSlotS6::ActionPerformed(const KActionEvent& event)
{
    GCWND_MSG_MAP(m_pkOpenMark, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickOpen);
    GCWND_MSG_MAP(m_pkCloseMark, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose);
    GCWND_MSG_MAP(m_pkKickBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickKick);
    GCWND_MSG_MAP(m_pkHurryUpBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnHurryUpBtn);
    GCWND_MSG_MAP(m_pkChangeLeader, KD3DWnd::D3DWE_BUTTON_CLICK, OnChangeLeader);
}

void KGCRoomUserSlotS6::OnCreate(void)
{
    for (int i = 0; i < EM_USER_STATE_SIZE; ++i)
    {
        m_apkUserState[i]->InitState(true, true, this);
    }

    for (int i = 0; i < EM_LEVEL_NUM; ++i)
    {
        m_apkUserLevel[i]->ToggleRender(true);
    }

    m_pkLevelMark->InitState(true, true, this);
    m_pkHostMark->InitState(true, true, this);
    m_pkOpenMark->InitState(true, true, this);
    m_pkCloseMark->InitState(true, true, this);
    m_pkOpenMark2->InitState(true, true, this);
    m_pkCloseMark2->InitState(true, true, this);
    m_pkCloseMarkPanel->InitState(true, true, this);
    m_pkCharImg->InitState(true, true, this);
    m_pkCharName->InitState(true, true, this);
    m_pkKickBtn->InitState(true, true, this);
    m_pkKickBtn->SetToolTip(g_pkStrLoader->GetString(STR_ID_BAN_PLAYER));

    m_pkSlotBack->InitState(false);
    m_pkMySlotBack->InitState(false);

    m_pkIdleState->InitState(false, true, this);
    m_pkHurryUpBtn->InitState(false, true, this);

    m_pkPingAntenna->InitState(false);

    m_pkFlag->InitState(false, true, this);
    if (m_pkCountryGroup)
        m_pkCountryGroup->InitState(false);

    if (m_pkPet == NULL)
        m_pkPet = new KPet();

    if (m_pkBackVitality)
        m_pkBackVitality->InitState(false);

    if (m_pkGuageVitality)
        m_pkGuageVitality->InitState(false);

    if (m_pkStaticVitality) {
        m_pkStaticVitality->InitState(false);
        m_pkStaticVitality->SetFontSize(6);
        m_pkStaticVitality->SetFontOutline(true);
        m_pkStaticVitality->SetAlign(DT_CENTER);
    }

    if (m_pkGuageVitalityRect)
        m_pkGuageVitalityRect->InitState(false);

    if (m_pkCharSelBtn) {
        m_pkCharSelBtn->InitState(false, true, this);
        m_pkCharSelBtn->ConnectEventProcedure(D3DWE_BUTTON_CLICK, this, &KGCRoomUserSlotS6::OnCharSelBtn);
    }

#ifdef POTION_ROOM_SHOW
    if (m_pkPotionSlot)
        m_pkPotionSlot->InitState(true, true, this);
#endif

    if (m_pkRank)
        m_pkRank->InitState(false);

    if (m_pkCharBanMark)
        m_pkCharBanMark->InitState(false);

    if (m_pkRankBanMark)
        m_pkRankBanMark->InitState(false);

    if (m_pkDisableImg)
        m_pkDisableImg->InitState(false);

    if (m_pkPetCard)
        m_pkPetCard->InitState(true, true, this);

    m_pkChangeLeader->InitState(true, true, this);

    m_bAnimation = g_MyD3D->m_KGCOption.GetRoomAnimationEnable();
    m_bBaseEquip = g_MyD3D->m_KGCOption.GetBasicEquipEnable();

    m_pkExtraObject = new KGCObjectAndMotion;
}

void KGCRoomUserSlotS6::Destroy()
{
    if (m_pkPetCard)
        m_pkPetCard->ResetItem();
    if (m_pkPet)
        m_pkPet->EndPet();
#ifdef POTION_ROOM_SHOW
    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
        m_pkPotionSlot->SlotRelease();
#endif

#ifdef ENCHANT_EFFECT
    ClearEnchantEffect();
#endif
    if (m_pkExtraObject)
        SAFE_DELETE(m_pkExtraObject);
}

void KGCRoomUserSlotS6::SetExtraRoomObject( bool bAddMotion )
{
    if (bAddMotion)
    {
        if (m_pkExtraObject)
        {
            SAFE_DELETE(m_pkExtraObject);
            m_pkExtraObject = new KGCObjectAndMotion;
        }
    }
    if ((!g_MyD3D->m_KGCOption.GetRoomAnimationEnable() && g_MyD3D->GetMyPlayer()->m_iPlayerIndex != m_iIndex) || g_MyD3D->GetMyPlayer()->m_iPlayerIndex == m_iIndex)
    {
        std::string tempDef[3] = { "" };
        if (g_kGlobalValue.IsSexCharacter(m_iCharType) == 2)
        {
            tempDef[0] = m_pkRoomAnimation.m_pkExtraRoomInfo.m_sExtraModelF;
            tempDef[1] = m_pkRoomAnimation.m_pkExtraRoomInfo.m_sExtraTextureF;
            tempDef[2] = m_pkRoomAnimation.m_pkExtraRoomInfo.m_sExtraAnimationF;
        }
        else
        {
            tempDef[0] = m_pkRoomAnimation.m_pkExtraRoomInfo.m_sExtraModelM;
            tempDef[1] = m_pkRoomAnimation.m_pkExtraRoomInfo.m_sExtraTextureM;
            tempDef[2] = m_pkRoomAnimation.m_pkExtraRoomInfo.m_sExtraAnimationM;
        }
        if ( m_pkExtraObject->GetVecMotionSize() > 0 && ( tempDef[0] == "" && tempDef[1] == "" ) )
            SetExtraRoomObject( true );
        if (tempDef[0] != "" && tempDef[1] != "")
        {
            RoomAnimationExtra sLiveModelData = m_pkRoomAnimation.m_pkExtraRoomInfo;
            D3DXVECTOR2 vPos = GetFixedWindowPos();
            vPos.x = (vPos.x / (400.0f * GC_SCREEN_DIV_WIDTH)) - 1.0f;
            vPos.y = ((-vPos.y / (300.0f * GC_SCREEN_DIV_WIDTH)) * 0.75f) + 0.75f;
            float fPlayerGapX = 70.0f;
            if (!g_pkUIScene->m_pkRoomSceneS6->IsMaxPlayerNumSix())
                fPlayerGapX = 115.0f;
            if( m_pkExtraObject->GetVecMotionSize() == 0 )
                m_pkExtraObject->AddMesh(tempDef[0], tempDef[1]);
            if ( tempDef[2] != "" && m_pkExtraObject->GetVecMotionSize() == 0 )
                m_pkExtraObject->AddMotion(tempDef[2], !sLiveModelData.m_bIsLoop);
            m_pkExtraObject->SetOffset(D3DXVECTOR3(sLiveModelData.m_fExtraX + (vPos.x + (fPlayerGapX / (400.0f))), sLiveModelData.m_fExtraY + (vPos.y - (170.0f / (400.0f))), sLiveModelData.m_fExtraZ));
            m_pkExtraObject->SetScale(D3DXVECTOR3(sLiveModelData.m_fExtraScale, sLiveModelData.m_fExtraScale, sLiveModelData.m_fExtraScale));
            std::stringstream tempName;
            tempName << "Player_" << m_iIndex;
            m_pkExtraObject->m_strName = tempName.str();
            m_pkExtraObject->SetRender(true);
        }
    }
}

void KGCRoomUserSlotS6::UpdatePlayerData(int nPlayerIndex, bool bAddMotion)
{
    if (false == g_MyD3D->IsDungeonPlayerIndex(nPlayerIndex))
        return;

    for (int i = 0; i < EM_LEVEL_NUM; ++i)
    {
        m_apkUserLevel[i]->ToggleRender(true);
    }

    m_pkLevelMark->ToggleRender(true);
    m_pkHostMark->ToggleRender(g_MyD3D->MyPlayer[nPlayerIndex]->m_kUserInfo.bHost);
    m_pkChangeLeader->ToggleRender((g_MyD3D->MyPlayer[g_MyD3D->Get_MyPlayer()]->m_kUserInfo.bHost && g_MyD3D->Get_MyPlayer() != nPlayerIndex));
    m_pkOpenMark->ToggleRender(false);
    m_pkCloseMark->ToggleRender(false);
    m_pkOpenMark2->ToggleRender(false);
    m_pkCloseMark2->ToggleRender(false);
    m_pkCloseMarkPanel->ToggleRender(false);
    m_pkCharImg->ToggleRender(true);

    if (m_pkBackVitality) m_pkBackVitality->ToggleRender(false);
    if (m_pkGuageVitality) m_pkGuageVitality->ToggleRender(false);
    if (m_pkStaticVitality) m_pkStaticVitality->ToggleRender(false);

    // »õ·Î¿î À¯ÀúÀÌ¸é °­È­ ÀÌÆåÆ® Áö¿ö ÁÝ½Ã´Ù. 
    if (m_iIndex != nPlayerIndex) 
    {
#ifdef ENCHANT_EFFECT
        ClearEnchantEffect();
#endif
    }

    m_iIndex = nPlayerIndex;
    if (g_MyD3D->MyPlayer[m_iIndex]->m_kUserInfo.cCharIndex == -1)
    {
        return;
    }

    PLAYER* pPlayer = g_MyD3D->MyPlayer[nPlayerIndex];

    // Ä³¸¯ÅÍ°¡ ¹Ù²ãµµ °­È­ ÀÌÆåÆ® ÃÊ±âÈ­ ÇØ ÁØ´Ù. 
    if (m_iCharType != pPlayer->GetCurrentChar().iCharType) 
    {
#ifdef ENCHANT_EFFECT
        ClearEnchantEffect();
#endif
    }

    m_iCharType = pPlayer->GetCurrentChar().iCharType;

    UpdateEtcUI(pPlayer);
    UpdateCharData(pPlayer);
    UpdatIdleState(pPlayer);
    UpdateState(pPlayer->m_kUserInfo.nUserState);
    UpdateCharPet(pPlayer);

    if (pPlayer->m_pObject)
        pPlayer->m_pObject->SetShellMatrixScale(fANOTHER_PLATER_SCALE);

    pPlayer->SetSelectMotion(true);

#ifdef ENCHANT_EFFECT
    UpdateEnchantEffect(pPlayer);
#endif

    m_vecEffectID.clear();
    for (int i = 0; i < (int)pPlayer->GetCurrentChar().vecItemInfo.size(); ++i)
    {
        m_vecEffectID.push_back(pPlayer->GetCurrentChar().vecItemInfo[i].iItemID / 10);
    }
    ItemEffectParam kEffectParam(pPlayer, m_vecEffectID);
    SetItemEffect(kEffectParam);
    SetItemEffect2(kEffectParam);
    UpdateMyPlayer();

    if (m_pkCharSelBtn)
    {
        m_pkCharSelBtn->ToggleRender(g_MyD3D->Get_MyPlayer() == m_iIndex);
    }

#ifdef POTION_ROOM_SHOW
    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
        m_pkPotionSlot->SetSlotStatus(g_MyD3D->Get_MyPlayer() == m_iIndex);
#endif

    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
    {
        if (m_pkRank)
            m_pkRank->ToggleRender(false);

        if (m_pkCharBanMark)
            m_pkCharBanMark->ToggleRender(false);

        if (m_pkRankBanMark)
            m_pkRankBanMark->ToggleRender(false);
    }
    else
    {
        m_pkRank->ToggleRender(true);

        UpdateMark(pPlayer);

        m_pkCharBanMark->ToggleRender(SiKGCRoomManager()->GetRoomInfo().m_saAllowedChars[pPlayer->GetCurrentChar().iCharType] == false);

        int iGradeNumber = (pPlayer->GetCurrentChar().kELOUserData.m_ucGrade - 1);
        if (iGradeNumber < 0)
            m_pkRankBanMark->ToggleRender(false);
        else
            m_pkRankBanMark->ToggleRender(SiKGCRoomManager()->GetRoomInfo().m_saAllowedRankings[iGradeNumber] == false);
    }

    m_bInitChar = true;
    pPlayer->GetCurFormTemplate().vecExtraMesh.clear();

#ifdef POTION_ROOM_SHOW
    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
        m_pkPotionSlot->SetSlotItem();
#endif
}

void KGCRoomUserSlotS6::UpdateEtcUI(PLAYER* pPlayer)
{
#if defined(USE_PING_STATE_UI)      
    m_pkPingAntenna->ToggleRender(true);
#endif

    m_pkCountryGroup->ToggleRender(true);
    m_pkCountryGroup->SetCountryGroup(pPlayer->m_kUserInfo.strNation, false);

    bool bEnableKick = false;
    if (g_kGlobalValue.m_kUserInfo.bHost && g_MyD3D->Get_MyPlayer() != m_iIndex && pPlayer->m_kUserInfo.eGCUserLevel != USERLEVEL_ADMIN)
    {
        bEnableKick = true;
    }

    m_pkKickBtn->ToggleRender(bEnableKick);
}

void KGCRoomUserSlotS6::UpdateMark(PLAYER* pPlayer)
{
    SetGradeTex((pPlayer->m_kUserInfo.GetCurrentChar().kELOUserData.m_ucGrade - 1), m_pkRank);
}

void KGCRoomUserSlotS6::UpdateMyPlayer()
{
    //  ³»²¨¸é
    DWORD dwColor = 0xffffffff;
    bool bMyPlayer = true;
    if (g_MyD3D->Get_MyPlayer() == m_iIndex)
    {
        dwColor = 0xff88dd00;
        bMyPlayer = true;
    }
    else
    {
        dwColor = 0xff50dbfa;
        bMyPlayer = false;
    }

    m_pkCharImg->UpdateLineSel(bMyPlayer);
    m_apkUserLevel[0]->SetWindowColor(dwColor);
    m_apkUserLevel[1]->SetWindowColor(dwColor);
    m_pkLevelMark->SetWindowColor(dwColor);

    bool bRaidHero = SiKGCWorldMapManager()->IsRaidHeroDungeon(SiKGCRoomManager()->GetGameMode());

    m_pkSlotBack->ToggleRender(!bMyPlayer);
    m_pkMySlotBack->ToggleRender(bMyPlayer);

    if (bMyPlayer)
        SetVitalityInfo();
}

void KGCRoomUserSlotS6::UpdateCharData(PLAYER* pPlayer)
{
    m_pkCharName->SetText(L"#c" + g_MyD3D->MyPlayer[m_iIndex]->GetStrUserColor() + g_MyD3D->MyPlayer[m_iIndex]->GetStrUserName() + L"#cx");
    m_pkCharName->MakeAutoScrollString();
    m_pkCharName->SetShadow(true);
    m_pkCharName->SetShadowColor(0xFF000000);
    m_pkCharImg->SetChar(m_iCharType);

    int nLevel = Exp_2_Level(pPlayer->GetCurrentChar().biExp);

    if ((nLevel / 10) > 0)
    {
        SetNumberTex((nLevel / 10), m_apkUserLevel[0]);
    }
    else
    {
        m_apkUserLevel[0]->ToggleRender(false);
    }

    SetNumberTex((nLevel % 10), m_apkUserLevel[1]);
}

#ifdef ENCHANT_EFFECT // Kawan>
void KGCRoomUserSlotS6::UpdateEnchantEffect(PLAYER* pPlayer)
{
    // °­È­ ÀÌÆåÆ® 
    if (pPlayer != NULL && m_apkUserState[EM_USER_STATE_CHANGE]->IsRenderOn() == false)
    {
        std::pair<bool, int> prEnchantWeaponInfo = g_pItemMgr->GetOtherUserEquipedEnchantWeaponInfo(&(pPlayer->GetCurrentChar()));
        if (prEnchantWeaponInfo.first && prEnchantWeaponInfo.second >= 17)
        {
            g_ParticleManager->DeleteSequence(m_vecEnchantParticle);
            D3DXVECTOR2 vPos = this->GetFixedWindowPos();

            float fPosX = 0.0f;
            float fPosY = 0.0f;

            float x = -300.0f;
            float y = -342.0f;

            if (!g_pkUIScene->m_pkRoomSceneS6->IsMaxPlayerNumSix())
            {
                x = -255.0f;
            }

            x *= GC_SCREEN_DIV_WIDTH;
            y *= GC_SCREEN_DIV_WIDTH;

            fPosX = vPos.x + x;
            fPosY = -vPos.y + y;

            EGCEnchantColorGroup eColor = pPlayer->GetEnchnatColorGroup(pPlayer->GetCurrentChar().iCharType, 17);
            g_kGlobalValue.AddEnchantEffect(eColor, ENCHANT_EFFECT_LEVEL_17, pPlayer, fPosX / (400.0f * GC_SCREEN_DIV_WIDTH), fPosY / (300.0f * GC_SCREEN_DIV_WIDTH) * 0.75f + 1.1f, m_iIndex, &m_vecEnchantParticle, ENCHANT_EFFECT_ROOM_OTHER);
            m_bRenderEnchatEffect = true;
        }
        else
        {
            ClearEnchantEffect();
        }
    }
}
#endif

void KGCRoomUserSlotS6::UpdateCharPet(PLAYER* pPlayer)
{
    if (pPlayer->m_kUserInfo.bLive && pPlayer->m_kUserInfo.cCharIndex != -1)
    {
        KPetInfo kPet = pPlayer->m_kUserInfo.GetCurrentChar().kPetInfo;
        int iPetID = g_kGlobalValue.GetPetIDFromItemID(kPet.m_dwID);
        int iPetEvolutionLevel = (int)kPet.m_cPromotion;
        int iPetLevel = kPet.m_iLevel;
        DWORD dwPetCostumeID = kPet.m_kCostume.m_dwID;

        m_pkPetCard->ResetItem();

        if (g_MyD3D->m_KGCOption.GetPetViewHide())
        {
            m_pkPet->EndPet();
            SetDrawItemTexture(kPet);
        }
        else
        {
            if (kPet.m_dwUID != 0 && iPetID != -1)
            {
                if (iPetID != m_pkPet->m_iPetID ||
                    iPetEvolutionLevel != m_pkPet->m_iEvolutionLevel ||
                    iPetLevel != m_pkPet->m_iLevel ||
                    dwPetCostumeID != m_pkPet->GetCostumeID())
                {
                    m_pkPet->StartPet(m_iIndex, iPetID, iPetLevel, iPetEvolutionLevel, EGCPETLOCATION::ROOM, kPet.m_strName, false, kPet.m_kCostume.m_dwID, true);
                    m_pkPet->SetPetMotion(EGCPETMOTION::ROOM);
                }
            }
            else
            {
                m_pkPet->EndPet();
            }
        }

        //모든 캐릭터의 펫 스킬을 로드하도록 수정했습니다.Luma 가 제작했습니다.
        if (g_MyD3D->Get_MyPlayer() == m_iIndex) {
            g_MyD3D->GetMyPlayer()->AutoEquipPetItem();
        }
    }
}

void KGCRoomUserSlotS6::UpdateSlot(bool bSlot)
{
    bool bOpen = (!m_bInitChar && bSlot);
    bool bClose = (!m_bInitChar && !bSlot);

    int nMaxPlayerNum = SiKGCWorldMapManager()->GetDungeonMaxPlayerNum(SiKGCRoomManager()->GetGameMode());

    bool bPlayerNum = true;
    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON && m_iIndex >= nMaxPlayerNum)
    {
        bPlayerNum = false;
    }

    m_pkOpenMark->ToggleRender(bOpen && bPlayerNum);
    m_pkCloseMark->ToggleRender(bClose && bPlayerNum);
    m_pkOpenMark2->ToggleRender(bOpen && bPlayerNum);
    m_pkCloseMark2->ToggleRender(bClose && bPlayerNum);
    m_pkCloseMarkPanel->ToggleRender(bClose);

    if (bSlot == false && SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON && bClose == false) {
        if (IsTeamMode())
            m_pkFlag->ToggleRender(true);
        else
            m_pkFlag->ToggleRender(false);
    }
    else {
        m_pkFlag->ToggleRender(false);
    }
}

void KGCRoomUserSlotS6::ClearPlayerData()
{
    m_pkRoomAnimation = RoomAnimationInfo();

    for (int i = 0; i < EM_USER_STATE_SIZE; ++i)
    {
        m_apkUserState[i]->ToggleRender(false);
    }

    for (int i = 0; i < EM_LEVEL_NUM; ++i)
    {
        m_apkUserLevel[i]->ToggleRender(false);
    }

    m_pkLevelMark->ToggleRender(false);
    m_pkHostMark->ToggleRender(false);
    m_pkOpenMark->ToggleRender(false);
    m_pkCloseMark->ToggleRender(false);
    m_pkOpenMark2->ToggleRender(false);
    m_pkCloseMark2->ToggleRender(false);
    m_pkCloseMarkPanel->ToggleRender(false);
    m_pkCharImg->ToggleRender(false);
    m_pkPingAntenna->ToggleRender(false);
    m_pkCountryGroup->ToggleRender(false);
    m_pkKickBtn->ToggleRender(false);

    m_pkIdleState->ToggleRender(false);
    m_pkHurryUpBtn->ToggleRender(false);

    if (m_pkCountryGroup)
        m_pkCountryGroup->ToggleRender(false);

    m_pkCharName->SetText(L"");

    m_bInitChar = false;

    m_pkSlotBack->ToggleRender(false);
    m_pkMySlotBack->ToggleRender(false);
    m_pkRank->ToggleRender(false);

    if (m_pkCharSelBtn)
        m_pkCharSelBtn->ToggleRender(false);

#ifdef POTION_ROOM_SHOW
    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
        m_pkPotionSlot->SetSlotStatus(false);
#endif

    if (m_pkCharBanMark)
        m_pkCharBanMark->ToggleRender(false);

    if (m_pkRankBanMark)
        m_pkRankBanMark->ToggleRender(false);

    if (m_pkBackVitality) m_pkBackVitality->ToggleRender(false);
    if (m_pkGuageVitality) m_pkGuageVitality->ToggleRender(false);
    if (m_pkStaticVitality) m_pkStaticVitality->ToggleRender(false);

#ifdef ENCHANT_EFFECT
    ClearEnchantEffect();
#endif

    m_pkPetCard->ResetItem();
    m_pkChangeLeader->ToggleRender(false);
}

void KGCRoomUserSlotS6::SetNumberTex(int index, KD3DWnd* pWnd)
{
    if (pWnd == NULL) return;

    int x_index = index % 10;

    float tu = static_cast<float>(x_index) * 17.0f + 281.f;
    float tv = 73.f;
    float tu1 = static_cast<float>(x_index) * 17.0f + 295.f;
    float tv1 = 87.f;
    D3DXVECTOR4 vec(tu, tv, tu1, tv1);
    ChangeUVtoTexel(vec);

    pWnd->SetWndTexCoords(D3DWM_DEFAULT, vec);
}

void KGCRoomUserSlotS6::SetGradeTex(int index, KD3DWnd* pWnd)
{
    if (pWnd == NULL) return;

    if (index > (GC_BG_GRADE_MAX - 1) || index < 0) {
        m_pkRank->ToggleRender(false);
        return;
    }

    int x_index = index / GC_BG_GRADE_MAX;
    int y_index = index % GC_BG_GRADE_MAX;

    float tu = static_cast<float>(x_index) * 20.f + 345.f;
    float tv = static_cast<float>(y_index) * 21.f + 2.f;
    float tu1 = static_cast<float>(x_index) * 20.f + 365.f;
    float tv1 = static_cast<float>(y_index) * 21.f + 21.f;
    D3DXVECTOR4 vec(tu, tv, tu1, tv1);
    ChangeUVtoTexel(vec);

    pWnd->SetWndTexCoords(D3DWM_DEFAULT, vec);
}

void KGCRoomUserSlotS6::ChangeUVtoTexel(D3DXVECTOR4& vUV)
{
    float fMagicNumber = 0.5f;
    if (vUV.x > vUV.z)
    {
        fMagicNumber *= -1.0f;
    }
    vUV.x = (vUV.x + fMagicNumber) / 512.0f;
    vUV.z = (vUV.z - fMagicNumber) / 512.0f;

    fMagicNumber = 0.5f;
    if (vUV.y > vUV.w)
    {
        fMagicNumber *= -1.0f;
    }
    vUV.y = (vUV.y + fMagicNumber) / 512.0f;
    vUV.w = (vUV.w - fMagicNumber) / 512.0f;

    TEXEL_BOUND(vUV.x);
    TEXEL_BOUND(vUV.y);
    TEXEL_BOUND(vUV.z);
    TEXEL_BOUND(vUV.w);
}

void KGCRoomUserSlotS6::UpdatIdleState(PLAYER* pPlayer)
{
    if (g_MyD3D->MyPlayer[m_iIndex]->m_bIdle == true)
    {
        m_pkIdleState->ToggleRender(true);
        m_pkHurryUpBtn->ToggleRender(false);
    }
    else
    {
        m_pkIdleState->ToggleRender(false);
        if (pPlayer->m_kPressureState.m_nState == KPressureState::PS_GAUGE)
        {
            m_pkHurryUpBtn->ToggleRender(true);
            m_pkHurryUpBtn->SetProgressTime(pPlayer->m_kPressureState.m_dwElapsedTime);
        }
        else if (pPlayer->m_kPressureState.m_nState == KPressureState::PS_PRESS)
        {
            m_pkHurryUpBtn->ToggleRender(true);
        }
        else
        {
            m_pkHurryUpBtn->ToggleRender(false);
        }
    }
}

void KGCRoomUserSlotS6::UpdateState(int nState)
{
    for (int i = 0; i < EM_USER_STATE_SIZE; ++i)
    {
        if(m_apkUserState[EM_USER_STATE_SKILL]->IsRenderOn())
            SetExtraRoomObject(true);
        m_apkUserState[i]->ToggleRender(false);
    }

    switch (nState)
    {
    case GC_RUS_READY:
    {
        m_apkUserState[EM_USER_STATE_READY]->ToggleRender(true);
    }
    break;
    case GC_RUS_MYINFO:
    {
        m_apkUserState[EM_USER_STATE_EQUIP]->ToggleRender(true);
    }
    break;
    case GC_RUS_MISSION:
    {
        m_apkUserState[EM_USER_STATE_MISSION]->ToggleRender(true);
    }
    break;
    case GC_RUS_SKILL:
    {
        SetExtraRoomObject(true);
        m_apkUserState[EM_USER_STATE_SKILL]->ToggleRender(true);
    }
    break;
    case GC_RUS_CHANGECHARACTER:
    {
        m_apkUserState[EM_USER_STATE_CHANGE]->ToggleRender(true);
    }
    break;
    default:
        break;
    }

}

void KGCRoomUserSlotS6::FrameMoveInEnabledState(void)
{
    if ((m_pkPet->m_pObject && g_MyD3D->m_KGCOption.GetPetViewHide()) || (!m_pkPet->m_pObject && !g_MyD3D->m_KGCOption.GetPetViewHide()))
        UpdateCharPet(g_MyD3D->MyPlayer[m_iIndex]);

    if ((m_bBaseEquip != g_MyD3D->m_KGCOption.GetBasicEquipEnable()) || (m_bAnimation != g_MyD3D->m_KGCOption.GetRoomAnimationEnable()))
    {
        m_bAnimation = g_MyD3D->m_KGCOption.GetRoomAnimationEnable();
        m_bBaseEquip = g_MyD3D->m_KGCOption.GetBasicEquipEnable();
        if(!g_MyD3D->MyPlayer[m_iIndex]->IsLocalPlayer())
            g_MyD3D->MyPlayer[m_iIndex]->UpdatePlayerResource();
    }

    if (m_pkPotionSlot) {
        if (SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_DUNGEON && true == m_pkPotionSlot->IsRenderOn())
            m_pkPotionSlot->ToggleRender(false);
        if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON && false == m_pkPotionSlot->IsRenderOn())
            m_pkPotionSlot->ToggleRender(true);
    }

    if (g_MyD3D->MyPlayer[m_iIndex]->m_kUserInfo.cCharIndex < 0)
        return;

    g_MyD3D->MyPlayer[m_iIndex]->Body_Angle = 0.0f;        //¸ñ µ¹¾Æ°£°Í ¸·±âÀ§ÇÑÄÚµå
    g_MyD3D->MyPlayer[m_iIndex]->cFrame++;

    if(m_pkExtraObject)
        m_pkExtraObject->FrameMoveExtra();

    if (m_pkPet->m_pObject)
    {
        GCDeviceMotionFRM* pCurrentMotion = pCurrentMotion = m_pkPet->GetPetMotionFrm(m_pkPet->usMotion);
        if (m_pkPet->cFrame < pCurrentMotion->GetNum_Frame() - 1)
            ++m_pkPet->cFrame;
    }

    if (!(g_MyD3D->MyPlayer[m_iIndex]->m_kPressureState.m_nState == KPressureState::PS_GAUGE || g_MyD3D->MyPlayer[m_iIndex]->m_kPressureState.m_nState == KPressureState::PS_PRESS))
        m_pkHurryUpBtn->ToggleRender(false);

    // ¿òÁ÷ÀÌ´Â ¾ÆÀÌÅÛÀ» À§ÇÑ ÇÁ·¹ÀÓ¹«ºê
    if (g_MyD3D->MyPlayer[m_iIndex]->m_pObject)
    {
        g_MyD3D->MyPlayer[m_iIndex]->SetRender(true);
        g_MyD3D->MyPlayer[m_iIndex]->EgoItemFrameMove(g_MyD3D->MyPlayer[m_iIndex]->GetFrameIndex());
        g_MyD3D->MyPlayer[m_iIndex]->m_pObject->ChildFrameMove();
    }
}

void KGCRoomUserSlotS6::PostDraw(void)
{
    D3DXMATRIX  matScale, matRot, matTrans, matWorld;
    g_pGCDeviceManager2->Flush();

    // ³² Ä³¸¯ÅÍ¸¦ ±×¸°´Ù
    int iIndex = m_iIndex;

    if (false == g_MyD3D->IsDungeonPlayerIndex(iIndex))
        return;

    if (g_MyD3D->MyPlayer[iIndex] == NULL)
        return;

    if (m_vecEnchantParticle.empty() == false && m_bRenderEnchatEffect) {
        g_ParticleManager->Render(GC_LAYER_CUSTOM_UI + m_iIndex);
    }

    if (g_MyD3D->MyPlayer[iIndex]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[iIndex]->m_kUserInfo.cCharIndex != -1)
    {
        D3DXVECTOR2 vPos = GetFixedWindowPos();
        vPos.x = (vPos.x / (400.0f * GC_SCREEN_DIV_WIDTH)) - 1.0f;
        vPos.y = ((-vPos.y / (300.0f * GC_SCREEN_DIV_WIDTH)) * 0.75f) + 0.75f;

        float fPetGapX = 110.0f;
        float fPlayerGapX = 70.0f;

        if (!g_pkUIScene->m_pkRoomSceneS6->IsMaxPlayerNumSix())
        {
            fPlayerGapX = 115.0f;
            fPetGapX = 155.0f;
        }

        // ÆêÀ» ¸ÕÀú ±×¸®ÀÚ
        if (m_pkPet && m_pkPet->m_pObject)
        {
            GCDeviceMotionFRM* pCurrentMotion = NULL;

            if (0 <= m_pkPet->usMotion && m_pkPet->IsAvailableMotion(m_pkPet->usMotion))
                pCurrentMotion = m_pkPet->GetPetMotionFrm(m_pkPet->usMotion);

            if (pCurrentMotion)
            {
                float fPetPosY = pCurrentMotion->GetFrameData(m_pkPet->cFrame).Pos_y * fCharScale;
                D3DXMatrixScaling(&matScale, m_pkPet->m_fScale, m_pkPet->m_fScale, m_pkPet->m_fScale);
                D3DXMatrixTranslation(&matTrans, vPos.x + (fPetGapX / 400.0f),
                    vPos.y - (165.0f / 400.0f) + fPetPosY, 20.0f);
                matWorld = matScale * matTrans;
                m_pkPet->m_pObject->SetWorldMat(&matWorld);
                m_pkPet->m_pObject->SetFrame(&pCurrentMotion->GetFrameData(m_pkPet->cFrame));
                m_pkPet->m_pObject->SetShellMatrixScale(m_pkPet->m_fScale);
                g_RenderManager->Render(false, m_pkPet->m_pObject);
            }
        }

        m_pkRoomAnimation = g_pRoomAnimationMgr->GetRoomAnimation( iIndex );
        GCDeviceMotionFRM* pMotion = g_pGraphicsHelper->GetPlayerSelectMotionRoom( (CHAR_IDENTY)static_cast<char>( g_MyD3D->MyPlayer[iIndex]->Extra_Char_Num ), m_pkRoomAnimation.m_iMotion );
        SetExtraRoomObject();

        if (NULL == pMotion)
            return;

        if (g_MyD3D->MyPlayer[iIndex]->cFrame >= static_cast<int>(pMotion->GetNum_Frame())) {
            g_MyD3D->MyPlayer[iIndex]->cFrame = pMotion->GetNum_Frame() - 1;
        }

        g_MyD3D->MyPlayer[iIndex]->vPos.x = pMotion->GetFrameData(g_MyD3D->MyPlayer[iIndex]->cFrame).Plus_x;
        g_MyD3D->MyPlayer[iIndex]->y_Ex = pMotion->GetFrameData(g_MyD3D->MyPlayer[iIndex]->cFrame).Pos_y;

        float fReCharSacle = fCharScale;
        // º£ÀÌ°¡½º´Â 5% ÁÙ¾î¾ß µÈ´Ù. ÅÛÇÃ¸´¿¡ ¹Ú±ä Çß´Âµ¥ ¿©±â¼± ÇÃ·¹ÀÌ¾î ÅÛÇÃ¸´À» ÀÐÁö ¾Ê±â ¶§¹®¿¡....
        if (g_MyD3D->MyPlayer[iIndex]->GetCurrentChar().iCharType == GC_CHAR_BEIGAS) {
            fReCharSacle = fCharScale * 0.92f;
        }
        D3DXMatrixScaling(&matScale, fReCharSacle, fReCharSacle, fReCharSacle);
        D3DXMatrixTranslation(&matTrans, vPos.x + (fPlayerGapX / (400.0f)),
            g_MyD3D->MyPlayer[iIndex]->y_Ex * fCharScale + vPos.y - (170.0f / (400.0f)), 30.0f);
        matWorld = matScale /** matRot*/ * matTrans;

        if (g_MyD3D->MyPlayer[iIndex]->m_pObject)
        {
            g_MyD3D->MyPlayer[iIndex]->m_pObject->SetWorldMat(&matWorld);
            g_MyD3D->MyPlayer[iIndex]->m_pObject->SetFrame(&pMotion->GetFrameData(g_MyD3D->MyPlayer[iIndex]->cFrame));

            g_MyD3D->MyPlayer[iIndex]->SetRenderFaceLift(g_MyD3D->MyPlayer[iIndex]->GetDefaultFaceLiftType());
            SiKGCFaceLift()->SetFaceLiftByMotionName(g_MyD3D->MyPlayer[iIndex], pMotion->GetDeviceID(), static_cast<int>(g_MyD3D->MyPlayer[iIndex]->cFrame));

            g_RenderManager->Render(false, g_MyD3D->MyPlayer[iIndex]->m_pObject);
        }

        if (m_pkExtraObject)
        {
            g_RenderManager->Render(false, m_pkExtraObject);
        }
    }
}

void KGCRoomUserSlotS6::PostChildDraw(void)
{
#ifdef POTION_ROOM_SHOW
    if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON)
        m_pkPotionSlot->SetSlotImage(m_iIndex);
#endif
}

void KGCRoomUserSlotS6::OnClickOpen()
{
    if (EnableOpenSlotClieck())
        ChangeOpenSlot(true);
}

void KGCRoomUserSlotS6::OnClickClose()
{
    if (EnableOpenSlotClieck())
        ChangeOpenSlot(false);
}

void KGCRoomUserSlotS6::OnClickKick()
{
    if (!g_kGlobalValue.m_kUserInfo.bHost)
        return;

    if (SiKGCRoomManager()->GetRoomInfo().iRestBanCount > 0) // °­ÅðÇÒ ¼ö ÀÖ´Â È½¼ö ÀÜ¿©·®ÀÌ ÀÖ¾î¾ßÇÔ
    {
        g_KDSound.Play("74");

        SiKGCRoomManager()->Kick_Player(m_iIndex);

        // [8/9/2007 breadceo] GC CLUB ¾ÆÀÌÅÛÀ» °¡Áö°í ÀÖÀ¸¸é ¹«ÇÑ °­Åð
        if (false == (g_pItemMgr->GetPremiumType(g_MyD3D->Get_MyPlayer()) & KPremiumInfo::TYPE_GCCLUB))
        {
            if (SiKGCRoomManager()->GetRoomInfo().iRestBanCount > 0)
            {
                std::wostringstream strmRemain;
                strmRemain << g_pkStrLoader->GetReplacedString(STR_ID_KICK_REMAIN_CNT_SOME, "i", (int)SiKGCRoomManager()->GetRoomInfo().iRestBanCount - 1);

                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                    g_pkStrLoader->GetString(STR_ID_KICK_REMAIN_CNT_IS),
                    strmRemain.str());
            }
            else
            {
                g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
                    g_pkStrLoader->GetString(STR_ID_KICK_REMAIN_CNT_IS),
                    g_pkStrLoader->GetString(STR_ID_KICK_REMAIN_CNT_NONE));
            }
        }
    }
    else
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NO_MORE_KICK1), L"");
    }
}

bool KGCRoomUserSlotS6::EnableOpenSlotClieck()
{
#if defined(USE_ONLY_TCP_RELAY)
    if (g_kGlobalValue.m_dwJoinRoomLimitPing != -1)
    {
        if (static_cast<int>(g_kGlobalValue.m_dwJoinRoomLimitPing) < g_kGlobalValue.GetTRAvgPing())
        {
            g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_BAD_PING_USER_JOIN_LIMIT_MESSAGE2),
                L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
            return false;
        }
    }
#endif

    // ´ëÈ¸¼­¹ö°¡ ¾Æ´Ñ °æ¿ì ¹æÀå¸¸ ½½·ÔÀ» ¿­°í ´ÝÀ» ¼ö ÀÖ´Ù. //1:1 Ã¤³Îµµ ¾Æ´Ò°æ¿ì ¤Ñ
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_INDIGO)
        || false == g_kGlobalValue.m_kUserInfo.bHost)
        return false;

#if !defined( NATION_USA ) && !defined( NATION_EU ) && !defined( NATION_KOREA )
    if (g_kGlobalValue.ServerInfo.CheckServerType(ST_PVP_BALANCE))
    {
        // ¹æÀåÀÎ ³»°¡ ¾Æ´Ñ ¿ÉÀú¹ö°¡ ÀÖ´Ù¸é ¹æÀå ±â´ÉÀ» »ó½ÇÇÑ´Ù.
        if (SiKGCRoomManager()->GetRoomInfo().IsExistObserver() && !g_MyD3D->IsLocalPlayerObserver())
            return false;
    }
#endif

    return true;

}

void KGCRoomUserSlotS6::OnHurryUpBtn()
{
    if (g_MyD3D->MyPlayer[m_iIndex]->m_kPressureState.m_nState == KPressureState::PS_PRESS)
    {
        KP2P::GetInstance()->Send_PressMember(g_MyD3D->MyPlayer[m_iIndex]->m_kUserInfo.dwUID); //ÀçÃËÇÏ±â!
    }
}

void KGCRoomUserSlotS6::OnCharSelBtn()
{
    if (g_pkUIHelper->GetUILockTime(LOCK_ROOM) == 0)
    {
        g_pkUIHelper->SetUILockTime(LOCK_ROOM, BUTTON_LOCK_FRAME);
        g_pkUIScene->m_pkRoomSceneS6->SetEnableStart(false);

        if (g_kGlobalValue.m_kUserInfo.bHost || g_kGlobalValue.m_kUserInfo.nUserState != GC_RUS_READY)
        {
#ifdef ENCHANT_EFFECT
            ClearEnchantEffect();
#endif
            g_MyD3D->m_pStateMachine->GoCharSelectFromRoom();
            KP2P::GetInstance()->Send_Into_Myinfo(g_kGlobalValue.m_kUserInfo.dwUID, g_kGlobalValue.m_kUserInfo.strLogin, GC_RUS_CHANGECHARACTER);
        }
    }
}

void KGCRoomUserSlotS6::ChangeOpenSlot(bool bOpenClick)
{
    if (g_pkUIHelper->GetUILockTime(LOCK_ROOM) == 0)
    {
        bool bOpen = false;
        int cIndex = -1;

        if (bOpenClick)
        {
            // ºó ½½·ÔÀÌ ÃÖ¼Ò 2°³ ÀÌ»óÀÌ¾î¾ß ÇÑ´Ù. ( ÃÖ¼Ò ¹æ¿¡ 2¸íÀº ÀÔÀå °¡´ÉÇÏ°Ô ÇØ¾ß ÇÑ´Ù. ) 
            if (2 >= (SiKGCRoomManager()->GetOpenSlotCount() + g_MyD3D->GetPlayerNum()))
                return;

            UpdateSlot(false);
            cIndex = m_iIndex;
            bOpen = false;
        }
        else
        {
            //  ´øÀü ¸ðµåÀÌ°í ÃÖ´ë ÀÎ¿ø ÃÊ°ú½Ã
            if (SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DUNGEON &&
                m_iIndex >= SiKGCWorldMapManager()->GetDungeonMaxPlayerNum(SiKGCRoomManager()->GetGameMode()))
            {
                return;
            }

            UpdateSlot(true);
            cIndex = m_iIndex;
            bOpen = true;
        }

        // ¹æ Á¤º¸ º¯°æÀ» ¿äÃ»ÇÑ´Ù.
        if (cIndex != -1)
        {
            std::vector<std::pair<char, bool> > vecChangeSlot;
            vecChangeSlot.push_back(std::make_pair(cIndex, !bOpenClick));

            if (SiKGCRoomManager()->GetGameModeType() == GC_GMT_TEAM_FIGHT)
            {
                // ¹æÀÎ¿øÀÌ Â¦¼ö ÀÏ¶§¸¸ µÎ°³ °°ÀÌ ´Ý°Ô ÇÏ°í½Í¾î¿ä.
                if (((SiKGCRoomManager()->GetOpenSlotCount() + g_MyD3D->GetPlayerNum()) % 2) == 0)
                {
                    SRoomInfo& roomInfo = SiKGCRoomManager()->GetRoomInfo();

                    if (cIndex < MAX_PLAYER_NUM / 2)
                    {
                        cIndex += MAX_PLAYER_NUM / 2;
                        if (roomInfo.bOpenSlot[cIndex] == bOpen || g_MyD3D->MyPlayer[cIndex]->m_kUserInfo.bLive)
                        {
                            for (int i = MAX_PLAYER_NUM - 1; i >= 0; i--)
                            {
                                if (roomInfo.bOpenSlot[i] != bOpen && !g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive)
                                {
                                    cIndex = i;
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        cIndex -= MAX_PLAYER_NUM / 2;
                        if (roomInfo.bOpenSlot[cIndex] == bOpen || g_MyD3D->MyPlayer[cIndex]->m_kUserInfo.bLive)
                        {
                            for (int i = 0; i < MAX_PLAYER_NUM; i++)
                            {
                                if (roomInfo.bOpenSlot[i] != bOpen && !g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive)
                                {
                                    cIndex = i;
                                    break;
                                }
                            }
                        }
                    }
                    vecChangeSlot.push_back(std::make_pair(cIndex, bOpen));
                }
            }
            KP2P::GetInstance()->Send_ChangeRoomInfoReq(vecChangeSlot);
        }

        g_pkUIHelper->SetUILockTime(LOCK_ROOM, BUTTON_LOCK_FRAME);
    }
}

void KGCRoomUserSlotS6::SetPingState(DWORD dwPing_)
{
    m_pkPingAntenna->SetPingState(dwPing_);
}

void KGCRoomUserSlotS6::Send_ChangeRoomUserInfoReq(int iChar)
{
    KP2P::GetInstance()->Send_ChangeRoomUserInfoReq(g_kGlobalValue.m_kUserInfo, KChangeRoomUserInfo::RUI_CHAR, g_MyD3D->Get_MyPlayer());
}

#ifdef ENCHANT_EFFECT
void KGCRoomUserSlotS6::ClearEnchantEffect()
{
    g_ParticleManager->DeleteSequence(m_vecEnchantParticle);
    m_bRenderEnchatEffect = false;
}
#endif

void KGCRoomUserSlotS6::SetVitalityInfo()
{
    if (g_kGlobalValue.UseVitalitySystem() == false)
        return;

    if (m_pkBackVitality == NULL || m_pkGuageVitality == NULL || m_pkStaticVitality == NULL || m_pkGuageVitalityRect == NULL) {
        return;
    }

    m_pkBackVitality->ToggleRender(true);
    m_pkGuageVitality->ToggleRender(true);
    m_pkStaticVitality->ToggleRender(true);

    SCharInfo* pCharInfo = &g_kGlobalValue.m_kUserInfo.GetCurrentChar();

    // È°·Â
    D3DXVECTOR2 vGaugePos = m_pkGuageVitalityRect->GetFixedWindowLocalPos();
    int nCurrVitality = g_kGlobalValue.GetVitality(pCharInfo->iCharType);
    int nMaxVitality = g_kGlobalValue.GetMaxVitality();

    // ÀÜ¿© È°·ÂÀÌ MaxVitality º¸´Ù ¸¹À» °æ¿ì °èÁ¤ Ã¹ Ä³¸¯ÅÍ »ý¼º¿ë ±âÁØÀ¸·Î Ç¥½ÃÇÑ´Ù
    if (nCurrVitality > nMaxVitality) {
        nMaxVitality = g_kGlobalValue.m_kVitalityInfo.m_nMaxVitalityForFirstCharacter;
    }

    float fPercent;
    if (nMaxVitality == 0) {
        fPercent = 0.0f;
    }
    else {
        fPercent = static_cast<float>(nCurrVitality) / static_cast<float>(nMaxVitality);
    }
    DWORD dwLength = static_cast<DWORD>(m_pkGuageVitalityRect->GetWidth() * fPercent);

    m_pkGuageVitality->SetWidth(dwLength);
    m_pkGuageVitality->SetFixedWindowLocalPos(vGaugePos);

    WCHAR strTemp[10];
    swprintf(strTemp, 10, L"%d/%d", nCurrVitality, nMaxVitality);
    m_pkStaticVitality->SetText(strTemp);
}

bool KGCRoomUserSlotS6::IsTeamMode()
{
    bool bbIsTeamMode = false;
    switch (SiKGCRoomManager()->GetGameMode())
    {
    case GC_GM_TEAM_MODE:
    case GC_GM_TAG_TEAM:
    case GC_GM_GUILD_BATTLE:
    case GC_GM_DEATH_TEAM:
    case GC_GM_DOTA:
    case GC_GM_CAPTAIN:
    case GC_GM_FATAL_DEATH_TEAM:
        bbIsTeamMode = true;
        break;
    }

    return bbIsTeamMode;
}

#ifdef SHOW_SBTAINROOM
void KGCRoomUserSlotS6::GetDrawItemID() //Ken 22/02/05
{
    int iItemListSize = 0;
    int iItemID = -1;

    m_fXTerm = 200.0f;
    m_fYTerm = -10.0f;
    m_fWidth = 50.0f;
    m_fHeight = 50.0f;

    GCMemoryFile memfile(g_pGCDeviceManager2->GetMassFileManager(), "PlayerBoxRenderItem.lua");

    if (!memfile.IsLoaded())
        return iItemID;

    KGCLuaManager LuaMgr;
    if (FAILED(LuaMgr.DoString((const char*)memfile.GetDataPointer(), MemFile.GetFileSize())))
        return iItemID;

    LuaMgr.GetValue("ItemListSize", iItemListSize);

    HRESULT hr = LuaMgr.BeginTable("RenderItem");

    for (int i = 1; i <= iItemListSize; ++i)
    {
        if (FAILED(hr = LuaMgr.BeginTable(i)))
            break;
        {
            int iTempItemID;
            if (FAILED(hr = LuaMgr.GetValue("ITEMID", iTempItemID)))
                assert(!"FAILED( hr = LuaMgr.GetValue( ""ITEMID"", iTempItemID ) )");

            bool bNeedEquip;
            if (FAILED(hr = LuaMgr.GetValue("NEEDEQUIP", bNeedEquip)))
                assert(!"FAILED( hr = LuaMgr.GetValue( ""NEEDEQUIP"", bNeedEquip ) )");

            float fTempX, fTempY, fTempWidth, fTempHeight;

            if (FAILED(hr = LuaMgr.GetValue("X_TERM", fTempX)))
                assert(!"FAILED( hr = LuaMgr.GetValue( ""X_TERM"", fTempX ) )");

            if (FAILED(hr = LuaMgr.GetValue("Y_TERM", fTempY)))
                assert(!"FAILED( hr = LuaMgr.GetValue( ""Y_TERM"", fTempY ) )");

            if (FAILED(hr = LuaMgr.GetValue("WIDTH", fTempWidth)))
                assert(!"FAILED( hr = LuaMgr.GetValue( ""WIDTH"", fTempWidth ) )");

            if (FAILED(hr = LuaMgr.GetValue("HEIGHT", fTempHeight)))
                assert(!"FAILED( hr = LuaMgr.GetValue( ""HEIGHT"", fTempHeight ) )");


            if (FAILED(hr = LuaMgr.GetValue("LINEARFILTER", m_bLinearFilter)))
                assert(!"FAILED( hr = LuaMgr.GetValue( ""LINEARFILTER"", m_bLinearFilter ) )");

            if (bNeedEquip == false)
            {
                if (g_pItemMgr->GetPremiumType(m_iIndex) > KPremiumInfo::TYPE_NORMAL)
                {
                    iItemID = iTempItemID;
                    m_fXTerm = fTempX;
                    m_fYTerm = fTempY;
                    m_fWidth = fTempWidth;
                    m_fHeight = fTempHeight;
                    break;
                }
            }
            else
            {
                if (g_pItemMgr->CheckEquipItemForItemID(&STemp, iTempItemID))
                {
                    iItemID = iTempItemID;
                    m_fXTerm = fTempX;
                    m_fYTerm = fTempY;
                    m_fWidth = fTempWidth;
                    m_fHeight = fTempHeight;
                    break;
                }
            }
        }
        if (FAILED(hr = LuaMgr.EndTable()))
            break;
    }

    LuaMgr.EndTable();

    return iItemID;
}
#endif

void KGCRoomUserSlotS6::SetDrawItemTexture( KPetInfo pPetInfo_ )
{
    float fSize = 4.0f;
    float fPos_x = 0.5f;
    D3DXVECTOR2 CharBtnPosition = m_pkCharSelBtn->GetFixedWindowLocalPos();
    CharBtnPosition.y -= 10;
    if ( pPetInfo_.m_dwID > 0 )
    {
        m_pkPetCard->SetItem( &pPetInfo_ );
    }
}

void KGCRoomUserSlotS6::OnChangeLeader()
{
    if (m_iIndex != g_MyD3D->Get_MyPlayer())
    {
        g_kGlobalValue.m_kUserInfo.bHost = false;
        KP2P::GetInstance()->Send_ChangeRoomHostReq(g_MyD3D->MyPlayer[m_iIndex]->GetPlayerUID());
    }
}