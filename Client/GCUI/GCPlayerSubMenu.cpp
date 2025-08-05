#include "stdafx.h"
//
#include "GCPlayerSubMenu.h"
//
//

//

IMPLEMENT_CLASSNAME(KGCPlayerSubMenu);
IMPLEMENT_WND_FACTORY(KGCPlayerSubMenu);
IMPLEMENT_WND_FACTORY_NAME(KGCPlayerSubMenu, "gc_player_sub_menu");

KGCPlayerSubMenu* g_pkSubMenu = NULL;

KGCPlayerSubMenu::KGCPlayerSubMenu(void)
{
	g_pkSubMenu = this;

	m_pkAddFriend = NULL;
	LINK_CONTROL("add_friend_static", m_pkAddFriend);
}

KGCPlayerSubMenu::~KGCPlayerSubMenu(void)
{
}

void KGCPlayerSubMenu::OnCreate(void)
{
	InitState(false, true, this);
	m_pkAddFriend->InitState(true, true, this);
	m_pkAddFriend->SetText(g_pkStrLoader->GetString(STR_ID_FRIEND_ADD_TOOLTIP));
}

void KGCPlayerSubMenu::ActionPerformed(const KActionEvent& event)
{
	if (event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER)
		event.m_pWnd->SetFontColor(0xffffff00);
	else if (event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE)
		event.m_pWnd->SetFontColor(0xffffffff);
	else if (event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
	{
		if (event.m_pWnd == m_pkAddFriend)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NATEON_ADD_BUDDY, m_strFriendNickName
				, g_pkStrLoader->GetString(STR_ID_NATEON_ADD_BUDDY_DEFMSG)
				, KGCUIScene::GC_MBOX_USE_NATEON_ADD_BUDDY, 0, 0, false, true);

			ToggleRender(false);
		}
	}
}