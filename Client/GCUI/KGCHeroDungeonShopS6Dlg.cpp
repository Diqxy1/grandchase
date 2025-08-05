#include "StdAfx.h"
#include "KGCHeroDungeonShopS6Dlg.h"
#include "KGCHeroDungeonShopPanel.h"
#include "KGCHeroDungeonShopSidePanel.h"
#include "KGCHeroDungeonUpgradePanel.h"
#include "KGCHeroDungeonUpgradeSidePanel.h"
#include "KGCHeroDungeonShopS6.h"

IMPLEMENT_CLASSNAME(KGCHeroDungeonShopS6Dlg);
IMPLEMENT_WND_FACTORY(KGCHeroDungeonShopS6Dlg);
IMPLEMENT_WND_FACTORY_NAME(KGCHeroDungeonShopS6Dlg, "gc_herodungeonshops6dlg");

KGCHeroDungeonShopS6Dlg::KGCHeroDungeonShopS6Dlg(void) :
	m_pkHeroShopPanel(NULL),
	m_pkHeroShopSidePanel(NULL),
	m_pkHeroUpgradePanel(NULL),
	m_pkHeroUpgradeSidePanel(NULL),
	m_pkExit(NULL),
	m_pkTitleNormal(NULL),
	m_pkTitleVIP(NULL)
{
	for (int i = 0; i < ETAB_HERO_DUNGEON_SIZE; ++i)
	{
		m_pkTabWnd[i] = NULL;
		m_pkTabWndOn[i] = NULL;
	}

	LINK_CONTROL("btn_tab0", m_pkTabWnd[ETAB_HERO_DUNGEON_SHOP]);
	LINK_CONTROL("btn_tab_on0", m_pkTabWndOn[ETAB_HERO_DUNGEON_SHOP]);

	LINK_CONTROL("btn_tab1", m_pkTabWnd[ETAB_HERO_ITEM_UPGRADE]);
	LINK_CONTROL("btn_tab_on1", m_pkTabWndOn[ETAB_HERO_ITEM_UPGRADE]);

	LINK_CONTROL("hero_shop_panel", m_pkHeroShopPanel);
	LINK_CONTROL("hero_shop_side_panel", m_pkHeroShopSidePanel);
	LINK_CONTROL("hero_upgrade_panel", m_pkHeroUpgradePanel);
	LINK_CONTROL("hero_upgrade_side_panel", m_pkHeroUpgradeSidePanel);
	LINK_CONTROL("btn_close", m_pkExit);
	LINK_CONTROL("title", m_pkTitleNormal);
	LINK_CONTROL("title_vip", m_pkTitleVIP);
}

KGCHeroDungeonShopS6Dlg::~KGCHeroDungeonShopS6Dlg(void)
{
}

void KGCHeroDungeonShopS6Dlg::OnCreate(void)
{
	g_HeroDungeonShopS6.ClearEquippedItems();
	for (int i = 0; i < ETAB_HERO_DUNGEON_SIZE; ++i) {
		m_pkTabWnd[i]->InitState(true, true, this);
		m_pkTabWndOn[i]->InitState(false, true, this);
	}

	m_pkHeroShopPanel->InitState(false, true);
	m_pkHeroShopSidePanel->InitState(false, true);
	m_pkHeroUpgradePanel->InitState(false, true);
	m_pkHeroUpgradeSidePanel->InitState(false, true);
	m_pkExit->InitState(true, true, this);
	m_pkExit->SetHotKey(DIK_ESCAPE);

	m_pkTitleNormal->InitState(false);
	m_pkTitleVIP->InitState(false);

	ChangeTab(ETAB_HERO_DUNGEON_SHOP);
}

void KGCHeroDungeonShopS6Dlg::OnCreateComplete(void)
{
	if (g_kGlobalValue.isCurrentShopVIP) {
		for (int i = 0; i < ETAB_HERO_DUNGEON_SIZE; ++i) {
			m_pkTabWnd[i]->ToggleRender(false);
			m_pkTabWndOn[i]->ToggleRender(false);
		}
		m_pkTitleNormal->ToggleRender(false);
		m_pkTitleVIP->ToggleRender(true);
	}
	else {
		m_pkTitleNormal->ToggleRender(true);
		m_pkTitleVIP->ToggleRender(false);
	}

	m_pkHeroShopSidePanel->InitCharacterInfo();
	m_pkHeroShopSidePanel->SetPreviewWindow();
}

void KGCHeroDungeonShopS6Dlg::ChangeTab(EShopTabType eTab) {

	if (eTab < 0 || eTab >= ETAB_HERO_DUNGEON_SIZE) return;
	for (int i = 0; i < ETAB_HERO_DUNGEON_SIZE; ++i)
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

	if (eTab == ETAB_HERO_DUNGEON_SHOP) {
		if (g_kGlobalValue.isCurrentShopVIP) {
			for (int i = 0; i < ETAB_HERO_DUNGEON_SIZE; ++i) {
				m_pkTabWnd[i]->ToggleRender(false);
				m_pkTabWndOn[i]->ToggleRender(false);
			}
			m_pkTitleNormal->ToggleRender(false);
			m_pkTitleVIP->ToggleRender(true);
		}
		else {
			m_pkTitleNormal->ToggleRender(true);
			m_pkTitleVIP->ToggleRender(false);
		}
		m_pkHeroShopSidePanel->ToggleRender(true);
		m_pkHeroShopPanel->ToggleRender(true);
		m_pkHeroUpgradePanel->ToggleRender(false);
		m_pkHeroUpgradeSidePanel->ToggleRender(false);
		m_pkHeroShopPanel->UpdateClickShopPanel();
		g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel->InitUIStateSide();
		for (int i = 0; i < (int)g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel->m_vecInvenItemImgListSide.size(); ++i) {
			g_pkUIScene->m_pkHeroDungeonShopS6->m_pkHeroUpgradeSidePanel->m_vecInvenItemImgListSide[i]->SelectedItem(false);
		}
	}
	else if (eTab == ETAB_HERO_ITEM_UPGRADE) {
		m_pkHeroShopPanel->ToggleRender(false);
		m_pkHeroShopSidePanel->ToggleRender(false);
		m_pkHeroUpgradePanel->ToggleRender(true);
		m_pkHeroUpgradeSidePanel->ToggleRender(true);
		m_pkHeroUpgradePanel->OnClickHeroUpgradeTab();
	}
}

void KGCHeroDungeonShopS6Dlg::OnClose(void)
{
	g_KDSound.Play("31");
	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
}

void KGCHeroDungeonShopS6Dlg::EquipItem(GCItem* item)
{
	m_pkHeroShopSidePanel->EquipItem(item);
}

void KGCHeroDungeonShopS6Dlg::UnequipItem(GCItem* item)
{
	m_pkHeroShopSidePanel->UnequipItem(item);
}

void KGCHeroDungeonShopS6Dlg::OnDestroy(void)
{
}

void KGCHeroDungeonShopS6Dlg::ActionPerformed(const KActionEvent& event)
{
	for (int i = 0; i < EShopTabType::ETAB_HERO_DUNGEON_SIZE; i++) {
		GCWND_MSG_MAP(m_pkTabWnd[i], KD3DWnd::D3DWE_BUTTON_CLICK, [&]()
			{
				ChangeTab(static_cast<EShopTabType>(i));
			});
	}
	GCWND_MSG_MAP(m_pkExit, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose);
}

void KGCHeroDungeonShopS6Dlg::OnDestroyComplete(void)
{
}

void KGCHeroDungeonShopS6Dlg::FrameMoveInEnabledState(void)
{
}

void KGCHeroDungeonShopS6Dlg::PostChildDraw(void)
{
}

void KGCHeroDungeonShopS6Dlg::UpdateShopPanel(void)
{
	m_pkHeroShopPanel->UpdateShopPanel();
}

void KGCHeroDungeonShopS6Dlg::UpdateUpgradeItemsTab()
{
}

void KGCHeroDungeonShopS6Dlg::ReloadAnimationMotion(void)
{
	m_pkHeroShopSidePanel->ReloadAnimationMotion();
}