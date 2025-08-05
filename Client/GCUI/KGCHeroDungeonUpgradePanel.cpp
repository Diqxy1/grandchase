#include "stdafx.h"
#include "KGCHeroDungeonUpgradePanel.h"
#include "MyD3D.h"
#include "Procedure.h"

IMPLEMENT_CLASSNAME(KGCHeroDungeonUpgradePanel);
IMPLEMENT_WND_FACTORY(KGCHeroDungeonUpgradePanel);
IMPLEMENT_WND_FACTORY_NAME(KGCHeroDungeonUpgradePanel, "gc_herodungeonupgradepanel");


KGCHeroDungeonUpgradePanel::KGCHeroDungeonUpgradePanel(void):
    m_pkStaticHeroCoin(NULL),
    m_pkBtnPrevPage(NULL),
    m_pkBtnNextPage(NULL),
    m_pkBack(NULL),
    m_pkStaticPage(NULL),
    m_pkHeroCoinImg(NULL),
    m_eCurrentShopUpgradeCategoryType(ETAB_HERO_ALL)
{
	for (int i = 0; i < NUM_HERO_UPGRADE_BOX; ++i)
	{
		m_apkItemBox[i] = NULL;

		char szBuf[MAX_PATH];
		sprintf_s(szBuf, MAX_PATH - 1, "itemslot%02d", i);
		LINK_CONTROL(szBuf, m_apkItemBox[i]);
	}

	for (int i = 0; i < ETAB_HERO_SIZE; ++i)
	{
		m_pkTabWnd[i] = NULL;
		m_pkTabWndOn[i] = NULL;

        char buffTab[MAX_PATH];
        sprintf_s(buffTab, MAX_PATH - 1, "btn_tab%d", i);
        LINK_CONTROL(buffTab, m_pkTabWnd[i]);

        char buffTabOn[MAX_PATH];
        sprintf_s(buffTabOn, MAX_PATH - 1, "btn_tab_on%d", i);
        LINK_CONTROL(buffTabOn, m_pkTabWndOn[i]);
	}

	LINK_CONTROL("static_coin_num", m_pkStaticHeroCoin);
	LINK_CONTROL("btn_prev_page", m_pkBtnPrevPage);
	LINK_CONTROL("btn_next_page", m_pkBtnNextPage);
	LINK_CONTROL("back", m_pkBack);
	LINK_CONTROL("static_page", m_pkStaticPage);
    LINK_CONTROL("coin_img", m_pkHeroCoinImg);

	m_eCurrentCharacter = GC_CHAR_INVALID;
	m_iCurrentPage = -1;
	m_iTotalPage = -1;
}

KGCHeroDungeonUpgradePanel::~KGCHeroDungeonUpgradePanel() {

}

void KGCHeroDungeonUpgradePanel::OnCreate(void) {
	for (int i = 0; i < ETAB_HERO_SIZE; ++i) {
		m_pkTabWnd[i]->InitState(true, true, this);
		m_pkTabWndOn[i]->InitState(false, true, this);
	}

	for (int i = 0; i < NUM_HERO_UPGRADE_BOX; ++i)
	{
		m_apkItemBox[i]->InitState(true, true, this);
		m_apkItemBox[i]->SetBoxNumber(i);
	}

	m_pkStaticHeroCoin->SetFontColor(0xffffffff);
	m_pkStaticHeroCoin->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
	m_pkStaticHeroCoin->SetAlign(DT_RIGHT);
	m_pkStaticHeroCoin->InitState(true, false, this);
    
    m_pkStaticPage->SetFontColor(0xffffffff);
	m_pkStaticPage->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
	m_pkStaticPage->SetAlign(DT_CENTER);
	m_pkStaticPage->InitState(true, true, this);

	m_pkBtnPrevPage->InitState(true, true, this);
	m_pkBtnNextPage->InitState(true, true, this);

	m_pkBack->InitState(true, true, this);

    m_pkHeroCoinImg->InitState(true, true, this);
    m_pkHeroCoinImg->SetItemTexture(g_pGCDeviceManager2->CreateShopItemTexture(ITEM_ID_HERO_COIN));
}

void KGCHeroDungeonUpgradePanel::OnCreateComplete(void)
{
    SCharInfo charInfo = g_kGlobalValue.m_kUserInfo.GetCurrentChar();
    m_eCurrentCharacter = static_cast<EGCCharType>(charInfo.iCharType);
    SetItemUpgradeInfo();
    ChangeTab(ETAB_HERO_ALL);
    UpdateFrame(ETAB_HERO_ALL, m_iCurrentPage);
}

void KGCHeroDungeonUpgradePanel::OnDestroy(void)
{
    m_eCurrentCharacter = GC_CHAR_INVALID;
    m_iCurrentPage = -1;
    m_iTotalPage = -1;
}

void KGCHeroDungeonUpgradePanel::ActionPerformed(const KActionEvent& event)
{
    for (int i = 0; i < ETAB_HERO_SIZE; i++) {
        GCWND_MSG_MAP(m_pkTabWnd[i],
            KD3DWnd::D3DWE_BUTTON_CLICK,
            [&]()
            {
                m_eCurrentShopUpgradeCategoryType = static_cast<EShopUpgradeCategoryType>(i);
                ChangeTab(m_eCurrentShopUpgradeCategoryType);
                UpdateFrame(m_eCurrentShopUpgradeCategoryType, m_iCurrentPage);
            });

    }
    GCWND_MSG_MAP(m_pkBtnPrevPage, KD3DWnd::D3DWE_BUTTON_CLICK, OnPrevPage);
    GCWND_MSG_MAP(m_pkBtnNextPage, KD3DWnd::D3DWE_BUTTON_CLICK, OnNextPage);
}

void KGCHeroDungeonUpgradePanel::OnDestroyComplete(void)
{
}

void KGCHeroDungeonUpgradePanel::FrameMoveInEnabledState(void)
{
    int nHeroCoinCount = (g_pItemMgr->GetInventoryItemDuration(ITEM_ID_HERO_COIN));
    m_pkStaticHeroCoin->SetText(nHeroCoinCount);
}

void KGCHeroDungeonUpgradePanel::PostChildDraw(void)
{
}

void KGCHeroDungeonUpgradePanel::OnPrevPage(void)
{
    --m_iCurrentPage;
    if (m_iCurrentPage < 1) {
        m_iCurrentPage = 1;
    }
    else {
        g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel->InitUIStateSide();
    }

    UpdateFrame(m_eCurrentShopUpgradeCategoryType, m_iCurrentPage);
}

void KGCHeroDungeonUpgradePanel::OnNextPage(void)
{
    ++m_iCurrentPage;
    if (m_iCurrentPage > m_iTotalPage) {
        m_iCurrentPage = m_iTotalPage;
    }
    else {
        g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel->InitUIStateSide();
    }

    UpdateFrame(m_eCurrentShopUpgradeCategoryType, m_iCurrentPage);
}

void KGCHeroDungeonUpgradePanel::ResetPage(int iItemCount_)
{
    m_iTotalPage = iItemCount_ / NUM_HERO_UPGRADE_BOX;

    if (0 < m_iTotalPage && 0 != iItemCount_ % NUM_HERO_UPGRADE_BOX) {
        ++m_iTotalPage;
    }
    else if (0 == m_iTotalPage && 0 != iItemCount_ % NUM_HERO_UPGRADE_BOX) {
        m_iTotalPage = 1;
    }

    m_iCurrentPage = 1;
}

void KGCHeroDungeonUpgradePanel::SetMyHeroCoin(void)
{
    m_pkStaticHeroCoin->SetText(g_pItemMgr->GetInventoryItemDuration(ITEM_ID_HERO_COIN));
}

void KGCHeroDungeonUpgradePanel::SetItemUpgradeInfo(void)
{
    if (!m_mapUpgrageInfo.empty()) {
        return;
    }

    SendItemUpgradeInfoReq();

    if (g_kGlobalValue.m_kHeroDungeonInfo.m_mapUpgradeInfo.empty()) {
        return;
    }

    m_mapUpgrageInfo.clear();

    KEVENT_HERO_ITEM_UPGRADE_INFO_ACK mapUpgrade = g_kGlobalValue.m_kHeroDungeonInfo.m_mapUpgradeInfo;
    KEVENT_HERO_ITEM_UPGRADE_INFO_ACK::iterator mapIterator = mapUpgrade.begin();

    for (; mapIterator != mapUpgrade.end(); ++mapIterator)
    {

        KHeroItemUpgradeInfo heroItemUpgradeInfo = mapIterator->second;

        PushItemMap(ETAB_HERO_ALL, heroItemUpgradeInfo);
        switch (heroItemUpgradeInfo.m_nGrade)
        {   
        case KItem::GRADE_EPIC:
            PushItemMap(ETAB_HERO_EPIC, heroItemUpgradeInfo);
            break;
        case KItem::GRADE_LEGEND:
            PushItemMap(ETAB_HERO_RELIC, heroItemUpgradeInfo);
            break;
        }
    }

}

void KGCHeroDungeonUpgradePanel::OnClickHeroUpgradeTab() {
    ChangeTab(ETAB_HERO_ALL);
    m_iCurrentPage = 1;
    m_eCurrentShopUpgradeCategoryType = ETAB_HERO_ALL;
    UpdateFrame(m_eCurrentShopUpgradeCategoryType, m_iCurrentPage);
}

void KGCHeroDungeonUpgradePanel::SendItemUpgradeInfoReq(void)
{
    KP2P::GetInstance()->Send_HeroItemUpgradeInfoReq();

    Result_HeroItemList = INT_MAX;
    g_MyD3D->WaitForServerAck(Result_HeroItemList, INT_MAX, 3000, INT_MAX);
}

void KGCHeroDungeonUpgradePanel::PushItemMap(EShopUpgradeCategoryType eTab, KHeroItemUpgradeInfo heroItemUpgrade)
{
    std::map<EShopUpgradeCategoryType, std::vector<KHeroItemUpgradeInfo>>::iterator heroUpgradeItemInfoIterator = m_mapUpgrageInfo.find(eTab);

    if (m_mapUpgrageInfo.end() == heroUpgradeItemInfoIterator)
    {
        std::vector<KHeroItemUpgradeInfo> vecHeroItemInfo;
        vecHeroItemInfo.push_back(heroItemUpgrade);
        m_mapUpgrageInfo.insert(std::make_pair(eTab, vecHeroItemInfo));
    }
    else
    {
        m_mapUpgrageInfo[eTab].push_back(heroItemUpgrade);
    }
}

void KGCHeroDungeonUpgradePanel::ChangeTab(EShopUpgradeCategoryType eTab)
{
    if (eTab < 0 || eTab >= ETAB_HERO_SIZE) return;
    for (int i = 0; i < ETAB_HERO_SIZE; ++i)
    {
        if (!m_pkTabWnd[i] && !m_pkTabWndOn[i])
            continue;

        if (i == eTab)
        {
            m_pkTabWnd[i]->ToggleRender(false);
            m_pkTabWndOn[i]->ToggleRender(true);
        }
        else
        {
            m_pkTabWnd[i]->ToggleRender(true);
            m_pkTabWndOn[i]->ToggleRender(false);
        }
    }
    m_iCurrentPage = 1;
    UpdateFrame(m_eCurrentShopUpgradeCategoryType, m_iCurrentPage);
}

void KGCHeroDungeonUpgradePanel::UpdateFrame(EShopUpgradeCategoryType eTab, int iCurrentPage)
{
    std::map<EShopUpgradeCategoryType, std::vector<KHeroItemUpgradeInfo>>::iterator itemIterator = m_mapUpgrageInfo.find(eTab);

    if (itemIterator == m_mapUpgrageInfo.end())
        return;

    int iStartIndex = iCurrentPage;
    --iStartIndex;
    iStartIndex *= NUM_HERO_UPGRADE_BOX;

    std::vector<std::pair<GCItem*, KHeroItemUpgradeInfo>> tempVector;

    if (itemIterator != m_mapUpgrageInfo.end())
    {
        for (int i = 0; i < static_cast<int>(itemIterator->second.size()); ++i)
        {
            KHeroItemUpgradeInfo heroItemUpgradeInfo = itemIterator->second[i];
            GCItem* itemData = g_pItemMgr->GetItemData(heroItemUpgradeInfo.m_ItemID / 10);
            if (itemData->dwCharType & (1 << m_eCurrentCharacter))
            {
                tempVector.push_back(std::make_pair(itemData, itemIterator->second[i]));
            }
        }
    }

    m_iTotalPage = ceil(tempVector.size() * 2 / 2 / (double)NUM_HERO_UPGRADE_BOX);
    if (m_iTotalPage <= 0) ++m_iTotalPage;

    std::wostringstream strm;
    strm << m_iCurrentPage << L" / " << m_iTotalPage;
    m_pkStaticPage->SetText(strm.str());


    for (int i = 0; i < NUM_HERO_UPGRADE_BOX; ++i)
    {
        if (iStartIndex < tempVector.size())
        {
            if ((int)tempVector.size() <= iStartIndex)
                break;

            std::pair<GCItem*, KHeroItemUpgradeInfo> tempInfo = tempVector[iStartIndex++];
            m_apkItemBox[i]->SetItemInfo(tempInfo.second);
            m_apkItemBox[i]->ToggleRender(true);
        }
        else
        {
            m_apkItemBox[i]->ToggleRender(false);
        }
    }
}

void KGCHeroDungeonUpgradePanel::ResetSlotStatus()
{
    for (int i = 0; i < NUM_HERO_UPGRADE_BOX; ++i)
    {
        if (m_apkItemBox[i])
        {
            m_apkItemBox[i]->IsSlotActive = false;
        }
    }
}