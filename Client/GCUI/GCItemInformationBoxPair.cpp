
#include "stdafx.h"
#include "GCItemInformationBox.h"
#include "KGCItemInformationBreakupBox.h"
#include "GCItemInformationBoxPair.h"
#include "GCCoordiItemInformationBox.h"

//
#include "../MyD3D.h"

#include "EnchantStrong.h"
#include "KSingleton.h"

#include "KGCEmoticon.h"
#include "GCForgeManager.h"

IMPLEMENT_CLASSNAME(KGCItemInformationBoxPair);
IMPLEMENT_WND_FACTORY(KGCItemInformationBoxPair);
IMPLEMENT_WND_FACTORY_NAME(KGCItemInformationBoxPair, "gc_item_information_box_pair");

KGCItemInformationBoxPair::KGCItemInformationBoxPair(void)
    :m_pkPointedItemInformationBox(NULL)
    , m_pkEquippedItemInformationBox(NULL)
    , m_pkBreakupItemInformationBox(NULL)
    , m_pkCoordiItemInformationBox(NULL)
    , m_bEnableEquippedItemInformationBox(false)
    , m_iPointedItemAbility(0)
    , m_iEquippedItemAbility(0)
    , m_iAlphaValue(0)
    , m_dwCheckUID(0)
    , m_dwStartGapCount(0)
    , m_dwEndGapCount(0)
    , m_vOriginPos(D3DXVECTOR2(0.0f, 0.0f))
    , m_bShowExtraInfo(false)
    , m_bHeroItemCheck(false)
    , m_bCoordiItem(false)
{
    LINK_CONTROL("pointed_item_information_box", m_pkPointedItemInformationBox);
    LINK_CONTROL("equipped_item_information_box", m_pkEquippedItemInformationBox);
    LINK_CONTROL("breakup_item_information_box", m_pkBreakupItemInformationBox);
    LINK_CONTROL("coordi_item_infomation_box", m_pkCoordiItemInformationBox);
}

KGCItemInformationBoxPair::~KGCItemInformationBoxPair(void)
{
}

void KGCItemInformationBoxPair::OnCreate(void)
{
    SetAlignType(WNDUIAT_POSZEROBASE);
#if !defined( ITEM_BREAKUP_INFO_BOX )
    if (m_pkBreakupItemInformationBox != NULL)
        m_pkBreakupItemInformationBox->InitState(false);

#endif
}

void KGCItemInformationBoxPair::OncreateComplete(void)
{

}

void KGCItemInformationBoxPair::FrameMoveInEnabledState(void)
{
}

void KGCItemInformationBoxPair::ActionPerformed(const KActionEvent& event)
{

}

void KGCItemInformationBoxPair::PostDrawEquip()
{
    SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);

    m_pkEquippedItemInformationBox->ToggleRender(true);

    DWORD dwMinusPosY;
    if (m_pkPointedItemInformationBox->GetHeight() >= m_pkEquippedItemInformationBox->GetHeight())
        dwMinusPosY = m_pkPointedItemInformationBox->GetHeight() / 2;
    else
        dwMinusPosY = m_pkEquippedItemInformationBox->GetHeight() / 2;

    D3DXVECTOR2 vNewPos((400.0f * g_pGCDeviceManager2->GetWindowScaleX()) - static_cast<float>(GetWidth() / 2), (300.0f * g_pGCDeviceManager2->GetWindowScaleY()) - static_cast<float>(dwMinusPosY));

    m_pkPointedItemInformationBox->SetAlpha(255);
    m_pkPointedItemInformationBox->ToggleRender(true);

    SetWindowPosDirect(vNewPos);
    SetDisplayState(false);
}

void KGCItemInformationBoxPair::PostDrawBreakUP()
{
    // ·»´õ¼³Á¤    
    m_pkBreakupItemInformationBox->ToggleRender(true);

    // À©µµ¿ì Ãâ·Â À§Ä¡ Àâ±â
    float fMinusPosY;
    if (m_pkPointedItemInformationBox->GetHeight() >= m_pkBreakupItemInformationBox->GetHeight())
        fMinusPosY = m_pkPointedItemInformationBox->GetHeight() / 2.0f;
    else
        fMinusPosY = m_pkBreakupItemInformationBox->GetHeight() / 2.0f;

    D3DXVECTOR2 vNewPos(((400.0f * g_pGCDeviceManager2->GetWindowScaleX()) - GetWidth()), ((300.0f * g_pGCDeviceManager2->GetWindowScaleY()) - fMinusPosY));
    m_pkPointedItemInformationBox->SetAlpha(255);
    m_pkPointedItemInformationBox->ToggleRender(true);

    SetWindowPosDirect(vNewPos);
    m_pkBreakupItemInformationBox->SetUIPosition();
    m_pkPointedItemInformationBox->SetUIPosition();

}


void KGCItemInformationBoxPair::PostDrawPointItem()
{
    SetWindowPosDirect(m_vOriginPos);

    // ¾ÆÀÌÅÛ¹Ú½º Ãâ·Â
    if (NULL != m_pkPointedItemInformationBox->GetItem())
    {
        // ¾ËÆÄ ¾Ö´Ï¸ÞÀÌ¼Ç È¿°ú ¾øÀ½
        if (m_bHeroItemCheck || !g_MyD3D->m_KGCOption.GetAlphaAnimation())
        {
            m_pkPointedItemInformationBox->SetAlpha(255);

            m_pkPointedItemInformationBox->ToggleRender(true);
        }
        // ¾ËÆÄ ¾Ö´Ï¸ÞÀÌ¼Ç È¿°ú ÀÖÀ½
        else
        {
            if (m_dwCheckUID != m_pkPointedItemInformationBox->GetItemBoxUID())
            {
                m_iAlphaValue = 0;
                m_dwStartGapCount = timeGetTime();

                m_pkPointedItemInformationBox->ToggleRender(false);
                m_dwCheckUID = m_pkPointedItemInformationBox->GetItemBoxUID();
            }

            m_dwEndGapCount = timeGetTime();
            if ((m_dwEndGapCount - m_dwStartGapCount) > (DWORD)SHOW_BOX_GAP)
            {
                m_iAlphaValue += (m_iAlphaValue < 255) ? SHOW_BOX_ALPHA : 0;
                if (m_iAlphaValue > 255) m_iAlphaValue = 255;
                m_pkPointedItemInformationBox->SetAlpha(m_iAlphaValue);
                m_pkPointedItemInformationBox->ToggleRender(true);
            }
        }
    }

    AdjustIntoScreen();
    SetDisplayState(true);
}

void KGCItemInformationBoxPair::PostDrawCoordiItem()
{
    SetWindowPosDirect(m_vOriginPos);

    // ¾ÆÀÌÅÛ¹Ú½º Ãâ·Â
    if (NULL != m_pkCoordiItemInformationBox->GetItem())
    {
        // ¾ËÆÄ ¾Ö´Ï¸ÞÀÌ¼Ç È¿°ú ¾øÀ½
        m_pkCoordiItemInformationBox->SetAlpha(255);
        m_pkCoordiItemInformationBox->ToggleRender(true);

    }

    AdjustIntoScreen();
    SetDisplayState(true, true);

}

void KGCItemInformationBoxPair::PostDraw()
{
    m_pkPointedItemInformationBox->ToggleRender(false);
    m_pkEquippedItemInformationBox->ToggleRender(false);
    if (m_pkCoordiItemInformationBox)
        m_pkCoordiItemInformationBox->ToggleRender(false);


#if defined( ITEM_BREAKUP_INFO_BOX )
    m_pkBreakupItemInformationBox->ToggleRender(false);
#endif

    short sKeyDown = GetAsyncKeyState(VK_SHIFT);

    // Á¤º¸¹Ú½º Ãâ·Â ¾È³» String
    if (SiGCForgeManager()->IsEnableBreakUp())
        m_pkPointedItemInformationBox->SetKeyText(STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC4);
    else
        m_pkPointedItemInformationBox->SetKeyText(STR_ID_ITEM_INFORMATION_BOX_EXTRA_DESC3);

    // ShiftÅ°°¡ ´­·¯Á³°í ÀåºñÅÇÀÌ¸é
    if (sKeyDown && m_bEnableEquippedItemInformationBox && !m_bCoordiItem)
    {
        PostDrawEquip();
    }

#if defined( ITEM_BREAKUP_INFO_BOX )
    // ShiftÅ°°¡ ´­·¯Á³°í, ¾ÆÀÌÅÛÇØÃ¼ÀÌ°í, ÇØÃ¼°¡´ÉÇÑ ¾ÆÀÌÅÛÀÌ¸é...
    else if (sKeyDown && SiGCForgeManager()->IsEnableBreakUp() && !m_bCoordiItem)
    {
        // ÇØÃ¼ °¡´É ¾ÆÀÌÅÛÆÇ´Ü
        if (false == g_pItemMgr->IsBreakUpItem(m_pkPointedItemInformationBox->GetItem()->dwGoodsID))
        {
            PostDrawPointItem();
        }
        // ¾ÆÀÌÅÛÇØÃ¼ Á¤º¸¼ÂÆÃ
        else if (!m_pkBreakupItemInformationBox->SetInfoString(m_pkPointedItemInformationBox->GetItem(), m_pkPointedItemInformationBox->GetItemUID()))
        {
            PostDrawPointItem();
        }
        else
        {
            PostDrawBreakUP();
        }
    }

#endif
    else if (!m_bCoordiItem)
    {
        PostDrawPointItem();
    }
    else
    {
        PostDrawCoordiItem();
    }

    if (g_pkInput->IsUp(DIK_F))
    {
        m_bShowExtraInfo = m_bShowExtraInfo ? false : true;
        m_pkPointedItemInformationBox->SetShowExtraInfo(m_bShowExtraInfo);
        m_pkEquippedItemInformationBox->SetShowExtraInfo(m_bShowExtraInfo);
    }
}

bool KGCItemInformationBoxPair::IsCoordiItem(GCItem* pItem_, KItem* pInven_)
{
    if (!pItem_)
        return false;

    if (pItem_->eItemKind == GIK_COORDI_ITEM)
        return true;

    if (pInven_ && pInven_->CheckItemType(KItem::TYPE_LOOK))
        return true;

    return false;
}

void KGCItemInformationBoxPair::SetPointedItemInformation(GCItem* pItem_, char cPetPromotion_, KItem* pInven_, SUserItem* pUserItem_, KPetInfo* pPetInfo_, char cItemGrade_ /*= -1*/, int iLevel_ /*= -1*/)
{
    if (GC_GLOBAL_DEFINE::bReformCoordi)
    {
        m_bCoordiItem = IsCoordiItem(pItem_, pInven_);
    }

    if (!m_bCoordiItem)
    {
        m_pkPointedItemInformationBox->SetItemInformation(pItem_, cPetPromotion_, pInven_, pUserItem_, pPetInfo_, cItemGrade_, iLevel_);
        m_iPointedItemAbility = MakeCompareStandardValue(pItem_, g_MyD3D->m_TempPlayer.GetCurrentChar().iLevel, pInven_, pUserItem_, pPetInfo_);
    }
    else
    {
        m_pkCoordiItemInformationBox->SetItemInformation(pItem_, cPetPromotion_, pInven_, pUserItem_, pPetInfo_, cItemGrade_, iLevel_);
    }

    SetDisplayState(true);
    SetWindowZOrder(D3DWZ_TOPMOST);
    m_bEnableEquippedItemInformationBox = false;

}

void KGCItemInformationBoxPair::SetOriginPosition(D3DXVECTOR2 vPos_)
{
    m_vOriginPos = vPos_;
    SetWindowPosDirect(m_vOriginPos);
}

void KGCItemInformationBoxPair::SetDisplayState(bool bAlone_, bool bCoordi)
{
    if (bCoordi)
    {
        m_pkCoordiItemInformationBox->SetBoxState();
    }
    else if (bAlone_)
    {
        m_pkPointedItemInformationBox->SetBoxState(KGCItemInformationBox::EIIBS_ALONE);
        m_pkPointedItemInformationBox->SetCompareIcon(KGCItemInformationBox::EICI_MAX);
    }
    else
    {
        if (m_iPointedItemAbility > m_iEquippedItemAbility)
        {
            m_pkPointedItemInformationBox->SetCompareIcon(KGCItemInformationBox::EICI_UP);
            m_pkEquippedItemInformationBox->SetCompareIcon(KGCItemInformationBox::EICI_DOWN);
        }
        else if (m_iPointedItemAbility == m_iEquippedItemAbility)
        {
            m_pkPointedItemInformationBox->SetCompareIcon(KGCItemInformationBox::EICI_EQUAL);
            m_pkEquippedItemInformationBox->SetCompareIcon(KGCItemInformationBox::EICI_EQUAL);
        }
        else
        {
            m_pkPointedItemInformationBox->SetCompareIcon(KGCItemInformationBox::EICI_DOWN);
            m_pkEquippedItemInformationBox->SetCompareIcon(KGCItemInformationBox::EICI_UP);
        }

        m_pkPointedItemInformationBox->SetBoxState(KGCItemInformationBox::EIIBS_POINTED);
        m_pkEquippedItemInformationBox->SetBoxState(KGCItemInformationBox::EIIBS_EQUIPPED);
    }
}

int KGCItemInformationBoxPair::MakeCompareStandardValue(GCItem* pItem_, int iCharacterLevel_, KItem* pInventory_, SUserItem* pUserItem_, KPetInfo* pPetInfo_)
{
    EItemAbilityData stItemAbilityData = g_pItemMgr->GetItemAbilityData(pItem_, iCharacterLevel_, pInventory_, pUserItem_, pPetInfo_);
    return static_cast<int>(stItemAbilityData.fAtk + stItemAbilityData.fDef + stItemAbilityData.fHP);
}

DWORD KGCItemInformationBoxPair::GetWidth() const
{
    if (m_pkEquippedItemInformationBox->IsRenderOn())
        return m_pkPointedItemInformationBox->GetWidth() + m_pkEquippedItemInformationBox->GetWidth();
    else if (m_bCoordiItem && m_pkCoordiItemInformationBox->IsRenderOn())
        return m_pkCoordiItemInformationBox->GetWidth();

    return m_pkPointedItemInformationBox->GetWidth();
}

DWORD KGCItemInformationBoxPair::GetHeight() const
{
    if (m_pkEquippedItemInformationBox->IsRenderOn())
    {
        if (m_pkPointedItemInformationBox->GetHeight() > m_pkEquippedItemInformationBox->GetHeight())
            return m_pkPointedItemInformationBox->GetHeight();
        return m_pkEquippedItemInformationBox->GetHeight();
    }
    else if (m_bCoordiItem && m_pkCoordiItemInformationBox->IsRenderOn())
        return m_pkCoordiItemInformationBox->GetHeight();

    return m_pkPointedItemInformationBox->GetHeight();
}

DWORD KGCItemInformationBoxPair::GetEquippedItemInformationWidth()
{
    return m_pkEquippedItemInformationBox->GetWidth();
}

DWORD KGCItemInformationBoxPair::GetEquippedItemInformationHeight()
{
    return m_pkEquippedItemInformationBox->GetHeight();
}

void KGCItemInformationBoxPair::PostChildDraw()
{
    ToggleRender(false);
}

void KGCItemInformationBoxPair::SetEquippedItemInformation()
{
    GCITEMID PointedItemID = m_pkPointedItemInformationBox->GetItemID();
    GCITEMUID PointedItemUID = m_pkPointedItemInformationBox->GetItemUID();
    GCItem* pPointedItem = g_pItemMgr->GetItemData(PointedItemID);

    SUserItem* pUserItem = NULL;
    GCItem* pEquipItem = NULL;
    KPetInfo* pPetInfo = NULL;

    // new> prevent crash
    if (pEquipItem == NULL)
        return;

    if (pPointedItem && false == g_pItemMgr->IsEquippedSameItem(&g_MyD3D->m_TempPlayer, pPointedItem, PointedItemUID) &&
        g_pItemMgr->GetEquipItemOfSamePosition(&g_MyD3D->m_TempPlayer, pPointedItem, &pEquipItem, &pUserItem, &pPetInfo) &&
        false == g_pItemMgr->IsBaseItem(pEquipItem->dwGoodsID) &&
        g_pItemMgr->GetItemData(PointedItemID)->eItemKind != GIK_MONSTER_CARD)
    {
        if (pUserItem)
        {
            m_pkEquippedItemInformationBox->SetItemInformation(pEquipItem, -1, g_pItemMgr->GetInventoryItem(pUserItem->dwItemUID), pUserItem, pPetInfo);
            m_iEquippedItemAbility = MakeCompareStandardValue(pEquipItem, g_MyD3D->m_TempPlayer.GetCurrentChar().iLevel, g_pItemMgr->GetInventoryItem(pUserItem->dwItemUID), pUserItem, pPetInfo);
        }
        else
        {
            m_pkEquippedItemInformationBox->SetItemInformation(pEquipItem, -1, NULL, pUserItem, pPetInfo);
            m_iEquippedItemAbility = MakeCompareStandardValue(pEquipItem, g_MyD3D->m_TempPlayer.GetCurrentChar().iLevel, NULL, pUserItem, pPetInfo);
        }

        SetDisplayState(false);
        m_bEnableEquippedItemInformationBox = true;
    }
}

void KGCItemInformationBoxPair::SetPointedItemInfoByItemUID(KD3DWnd* pWnd_, GCITEMUID ItemUID_)
{
    GCItem* pItem = NULL;
    KItem* pInvenItem = NULL;

    if (false == g_pItemMgr->FindInventory(ItemUID_, &pInvenItem, &pItem) || NULL == pInvenItem || NULL == pItem)
        return;

    SetPointedItemInformation(pItem, -1, pInvenItem);
    SetOriginPosition(GetNewPosition(pWnd_));
}

void KGCItemInformationBoxPair::SetPointedItemInfoByItemID(KD3DWnd* pWnd_, GCITEMID ItemID_)
{
    GCItem* pItem = g_pItemMgr->GetItemData(ItemID_);
    KItem* pInvenItem = g_pItemMgr->m_kInventory.FindItemByItemID(ItemID_);

    if (NULL == pItem)
        return;

    SetPointedItemInformation(pItem, -1, pInvenItem);
    SetOriginPosition(GetNewPosition(pWnd_));
}

void KGCItemInformationBoxPair::SetPointedItemInfoByItemIDAndLevel(KD3DWnd* pWnd_, GCITEMID ItemID_, int iLevel_)
{
    GCItem* pItem = g_pItemMgr->GetItemData(ItemID_);
    KItem* pInvenItem = g_pItemMgr->m_kInventory.FindItemByItemID(ItemID_);

    if (NULL == pItem)
        return;

    SetPointedItemInformation(pItem, -1, pInvenItem, 0, 0, -1, iLevel_);
    SetOriginPosition(GetNewPosition(pWnd_));
}



D3DXVECTOR2 KGCItemInformationBoxPair::GetNewPosition(KD3DWnd* pWnd_)
{
    D3DXVECTOR2 vStanDardWndPos(pWnd_->GetCurrentWindowPos().x * pWnd_->GetWindowScaleX(), pWnd_->GetCurrentWindowPos().y * pWnd_->GetWindowScaleY());
    D3DXVECTOR2 vNewPos(vStanDardWndPos.x + static_cast<float>(pWnd_->GetWidth()) * pWnd_->GetWindowScaleX(), vStanDardWndPos.y);

    if (vNewPos.x >= (GC_SCREEN_WIDTH * g_pGCDeviceManager->GetWindowScaleX() * 0.5f))
    {
        vNewPos.x = vStanDardWndPos.x - (static_cast<float>(GetWidth()) * m_fWindowScaleX);
    }

    return vNewPos;
}

void KGCItemInformationBoxPair::BindToLua()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    lua_tinker::class_add<KGCItemInformationBoxPair>(L, "KGCItemInformationBoxPair");
    lua_tinker::class_def<KGCItemInformationBoxPair>(L, "SetPointedItemInfoByItemUID", &KGCItemInformationBoxPair::SetPointedItemInfoByItemUID);
    lua_tinker::class_def<KGCItemInformationBoxPair>(L, "SetPointedItemInfoByItemID", &KGCItemInformationBoxPair::SetPointedItemInfoByItemID);
    lua_tinker::class_def<KGCItemInformationBoxPair>(L, "SetPointedItemInfoByItemIDAndLevel", &KGCItemInformationBoxPair::SetPointedItemInfoByItemIDAndLevel);
    lua_tinker::class_inh<KGCItemInformationBoxPair, KD3DWnd>(L);
}


void KGCItemInformationBoxPair::DrawItemInfoBox(DWORD dwGoodsID, D3DXVECTOR2 vPos)
{
    if (dwGoodsID == -1) {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender(false);
        return;
    }

    GCItem* pItem = NULL;
    pItem = g_pItemMgr->GetItemData(dwGoodsID);
    g_pkUIScene->m_pkItemInformationBox->ToggleRender(true);
    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation(pItem);
    g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition(vPos);
    g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);
}
