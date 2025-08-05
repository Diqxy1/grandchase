#include "StdAfx.h"
#include "KGCHeroDungeonDlg.h"

IMPLEMENT_CLASSNAME(KGCHeroDungeonDlg);
IMPLEMENT_WND_FACTORY(KGCHeroDungeonDlg);
IMPLEMENT_WND_FACTORY_NAME(KGCHeroDungeonDlg, "gc_hero_dungeon_dlg");

KGCHeroDungeonDlg::KGCHeroDungeonDlg(void)
    : m_pkDungeonInfo(NULL)
    , m_pkDungeonItemShop(NULL)
    , m_pkDungeonItemReward1(NULL)
    , m_pkItemUpgrade(NULL)
    , m_pkClose1(NULL)
    , m_pkClose2(NULL)
    , m_pkTitleVIP(NULL)
{
    for (int i = 0; i < ETAB_HERO_DUNGEON_SIZE; ++i)
    {
        m_pkTabWnd[i] = NULL;
        m_pkTabWndOn[i] = NULL;
    }

    LINK_CONTROL("hero_dungeon_info", m_pkDungeonInfo);
    LINK_CONTROL("hero_dungeon_shop", m_pkDungeonItemShop);
#if defined (HERO_DUNGEON_REWARD)
    LINK_CONTROL("hero_dungeon_reward1", m_pkDungeonItemReward1);
#endif

    LINK_CONTROL("tab_btn_shop", m_pkTabWnd[ETAB_HERO_DUNGEON_SHOP]);
    LINK_CONTROL("tab_btn_shop_on", m_pkTabWndOn[ETAB_HERO_DUNGEON_SHOP]);

    LINK_CONTROL("tab_btn_dungeon_info", m_pkTabWnd[ETAB_HERO_DUNGEON_INFO]);
    LINK_CONTROL("tab_btn_dungeon_info_on", m_pkTabWndOn[ETAB_HERO_DUNGEON_INFO]);

#if defined (HERO_DUNGEON_REWARD)
    LINK_CONTROL("tab_btn_dungeon1", m_pkTabWnd[ETAB_HERO_DUNGEON_REWARD1]);
    LINK_CONTROL("tab_btn_dungeon1_on", m_pkTabWndOn[ETAB_HERO_DUNGEON_REWARD1]);
#endif

#if defined( USE_HERO_ITEM_UPGRADE )
    LINK_CONTROL("hero_dungeon_upgrade", m_pkItemUpgrade);
    LINK_CONTROL("tab_btn_upgrade", m_pkTabWnd[ETAB_HERO_ITEM_UPGRADE]);
    LINK_CONTROL("tab_btn_upgrade_on", m_pkTabWndOn[ETAB_HERO_ITEM_UPGRADE]);
#endif

    LINK_CONTROL("btn_close1", m_pkClose1);
    LINK_CONTROL("btn_close2", m_pkClose2);
    LINK_CONTROL("title_vip", m_pkTitleVIP);

}

KGCHeroDungeonDlg::~KGCHeroDungeonDlg(void)
{
}

void KGCHeroDungeonDlg::OnCreate(void)
{
    m_pkTabWnd[ETAB_HERO_DUNGEON_SHOP]->InitState(false, true, this);
    m_pkTabWndOn[ETAB_HERO_DUNGEON_SHOP]->InitState(false, true, this);
    m_pkDungeonItemShop->InitState(false, true, this);

    m_pkTabWnd[ETAB_HERO_DUNGEON_INFO]->InitState(false, true, this);
    m_pkTabWndOn[ETAB_HERO_DUNGEON_INFO]->InitState(false, true, this);
    m_pkDungeonInfo->InitState(false, true, this);

    if (GC_GLOBAL_DEFINE::bRelayHeroDungeon)
    {
#if defined (HERO_DUNGEON_REWARD)
        m_pkTabWnd[ETAB_HERO_DUNGEON_REWARD1]->InitState(false, true, this);
        m_pkTabWndOn[ETAB_HERO_DUNGEON_REWARD1]->InitState(false, true, this);
        m_pkDungeonItemReward1->InitState(true, true, this);
        m_pkDungeonItemReward1->SetGameMode(GC_GM_QUEST52);
#endif
    }

#if defined( USE_HERO_ITEM_UPGRADE )
    m_pkTabWnd[ETAB_HERO_ITEM_UPGRADE]->InitState(false, true, this);
    m_pkTabWndOn[ETAB_HERO_ITEM_UPGRADE]->InitState(false, true, this);
    m_pkItemUpgrade->InitState(false, true, this);
#endif

    m_pkClose1->InitState(false, true, this);
    m_pkClose2->InitState(true, true, this);

    m_pkTitleVIP->InitState(false, true, this);
}

void KGCHeroDungeonDlg::OnCreateComplete(void)
{
    for (int i = 0; i < ETAB_HERO_DUNGEON_SIZE; ++i)
    {
        m_pkTabWnd[i]->ToggleRender(false);
        m_pkTabWndOn[i]->ToggleRender(false);
    }
    m_pkTitleVIP->ToggleRender(false);
}

void KGCHeroDungeonDlg::ActionPerformed(const KActionEvent& event)
{
    GCWND_MSG_MAP(m_pkTabWnd[ETAB_HERO_DUNGEON_INFO], KD3DWnd::D3DWE_BUTTON_CLICK, OnTabDungeonInfo);
    GCWND_MSG_MAP(m_pkTabWnd[ETAB_HERO_DUNGEON_SHOP], KD3DWnd::D3DWE_BUTTON_CLICK, OnTabItemShop);

#if defined (HERO_DUNGEON_REWARD)
    if (GC_GLOBAL_DEFINE::bRelayHeroDungeon)
    {
        GCWND_MSG_MAP(m_pkTabWnd[ETAB_HERO_DUNGEON_REWARD1], KD3DWnd::D3DWE_BUTTON_CLICK, OnTabDungeonReward1);
    }
#endif

#if defined( USE_HERO_ITEM_UPGRADE )
    GCWND_MSG_MAP(m_pkTabWnd[ETAB_HERO_ITEM_UPGRADE], KD3DWnd::D3DWE_BUTTON_CLICK, OnTabItemUpgrade);
#endif

    GCWND_MSG_MAP(m_pkClose2, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose);
}

void KGCHeroDungeonDlg::OnDestroyComplete(void)
{
}

bool KGCHeroDungeonDlg::IsESCAPEKey(void)
{
    if (m_pkDungeonInfo->IsRenderOnOpentimeBox())
        return false;

    if (m_pkDungeonItemShop->IsRenderItemPreview())
        return false;

    if (m_pkDungeonItemReward1 && m_pkDungeonItemReward1->IsRenderItemPreview())
        return false;

    return true;
}

void KGCHeroDungeonDlg::FrameMoveInEnabledState(void)
{

    if (g_pkInput->IsDown(DIK_ESCAPE)
#ifdef USE_JOYSTICK
        || g_MyD3D->GetJoystickKey() == EJK_KEY_SELECT
#endif
        )
    {
        if (IsESCAPEKey())
        {
            SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
            g_pkInput->IgnoreDown(DIK_ESCAPE, 30);
        }
    }
}

void KGCHeroDungeonDlg::PostChildDraw(void)
{
}

void KGCHeroDungeonDlg::OnChangeTab(int nTab)
{
    m_pkDungeonInfo->ToggleRender(false);
    m_pkDungeonItemShop->ToggleRender(false);

    if (m_pkItemUpgrade) {
        m_pkItemUpgrade->ToggleRender(false);
    }

    if (m_pkDungeonItemReward1)
    {
        m_pkDungeonItemReward1->ToggleRender(true);
        m_pkDungeonItemReward1->UpdateCharacterInfo();
    }
}

void KGCHeroDungeonDlg::OnTabDungeonInfo()
{
    OnChangeTab(ETAB_HERO_DUNGEON_INFO);
}

void KGCHeroDungeonDlg::OnTabItemShop()
{
    OnChangeTab(ETAB_HERO_DUNGEON_SHOP);
}

void KGCHeroDungeonDlg::OnTabDungeonReward1()
{
    OnChangeTab(ETAB_HERO_DUNGEON_REWARD1);
}

void KGCHeroDungeonDlg::OnClose()
{
    g_KDSound.Play("31");

    SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
}

bool KGCHeroDungeonDlg::IsRenderItemPreview()
{
    if (m_pkDungeonItemShop->IsRenderItemPreview())
        return true;

    if (m_pkDungeonItemReward1 && m_pkDungeonItemReward1->IsRenderItemPreview())
        return true;

    return false;
}

void KGCHeroDungeonDlg::ReleaseLock()
{
    if (m_pkDungeonItemShop->IsRenderOn())
        m_pkDungeonItemShop->ReleaseLock();

    if (m_pkDungeonItemReward1 && m_pkDungeonItemReward1->IsRenderOn())
        m_pkDungeonItemReward1->ReleaseLock();
}

void KGCHeroDungeonDlg::UpdateReward()
{
    if (m_pkDungeonItemReward1)
        m_pkDungeonItemReward1->UpdateRewardButton();
}

void KGCHeroDungeonDlg::OnTabItemUpgrade()
{
    OnChangeTab(ETAB_HERO_ITEM_UPGRADE);
}