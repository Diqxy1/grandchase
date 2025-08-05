#include "stdafx.h"
#include "../Procedure.h"
#include ".\GCNateOnGroupMultiMenu.h"
#include "../BuddySystem/KFriendShip.h"
#include "../BuddySystem/KFriendShip.h"

#include "MyD3D.h"

IMPLEMENT_CLASSNAME(GCNateOnGroupMultiMenu);
IMPLEMENT_WND_FACTORY(GCNateOnGroupMultiMenu);
IMPLEMENT_WND_FACTORY_NAME(GCNateOnGroupMultiMenu, "gc_nateon_group_popup");

GCNateOnGroupMultiMenu::GCNateOnGroupMultiMenu(void)
	: m_pkBG(NULL)
	, m_nFromGroupID(-1)
	, m_dwUserUID(0)
{
	LINK_CONTROL("back", m_pkBG);

	for (int i = 0; i < MAX_GROUP_POPUP_NUM; ++i)
	{
		m_pkMenu[i] = NULL;
	}

	char strTemp[MAX_PATH];
	for (int i = 0; i < MAX_GROUP_POPUP_NUM; ++i)
	{
		sprintf(strTemp, "static%d", i + 1);
		LINK_CONTROL(strTemp, m_pkMenu[i]);
	}
	m_vecGroupID.clear();
}

GCNateOnGroupMultiMenu::~GCNateOnGroupMultiMenu(void)
{
	m_vecGroupID.clear();
}

void GCNateOnGroupMultiMenu::OnCreate()
{
	m_pkBG->InitState(true);
	for (int i = 0; i < MAX_GROUP_POPUP_NUM; ++i)
	{
		m_pkMenu[i]->InitState(true, true, this);
		m_pkMenu[i]->SetAlign(DT_VCENTER);
	}
}

void GCNateOnGroupMultiMenu::ActionPerformed(const KActionEvent& event)
{
	for (int i = 0; i < MAX_GROUP_POPUP_NUM; ++i)
	{
		GCWND_WND_MAP(m_pkMenu[i], KD3DWnd::D3DWE_CURSOR_ENTER, OnCursorEnter);
		GCWND_WND_MAP(m_pkMenu[i], KD3DWnd::D3DWE_CURSOR_LEAVE, OnCursorLeave);
		GCWND_WND_MAP(m_pkMenu[i], KD3DWnd::D3DWE_BUTTON_CLICK, OnClick);
	}
}

void GCNateOnGroupMultiMenu::OnCreateComplete()
{
}

void GCNateOnGroupMultiMenu::SetGroups(DWORD dwUID)
{
	m_dwUserUID = dwUID;
	m_nFromGroupID = -1;

	for (int i = 0; i < MAX_GROUP_POPUP_NUM; ++i)
	{
		m_pkMenu[i]->ToggleRender(false);
	}

	std::vector< KBuddyGroup > vecRet;
	SiKFriendShip()->GetFriendGroupVector(vecRet);

	std::vector< KBuddyGroup >::iterator vit;
	for (vit = vecRet.begin(); vit != vecRet.end(); )
	{
		if (vit->FindBuddy(dwUID) == true)
		{
			m_nFromGroupID = vit->GetGroupID();
			vit = vecRet.erase(vit);
		}
		else
		{
			++vit;
		}
	}
	m_vecGroupID.clear();

	for (int i = 0; i < (int)vecRet.size(); ++i)
	{
		m_pkMenu[i]->SetText(vecRet[i].GetGroupName());
		m_pkMenu[i]->ToggleRender(true);
		m_vecGroupID.push_back(vecRet[i].GetGroupID());
	}

	AdjustMenuSize();
}

void GCNateOnGroupMultiMenu::AdjustMenuSize()
{
	int iWidth = 0;
	int iHeight = 0;
	for (int i = 0; i < MAX_GROUP_POPUP_NUM; ++i)
	{
		if (true == m_pkMenu[i]->IsRenderOn())
		{
			iWidth = max(iWidth, g_pkFontMgr->GetWidth(m_pkMenu[i]->GetText().c_str()));
			iHeight++;
		}
	}

	for (int i = 0; i < MAX_GROUP_POPUP_NUM; ++i)
	{
		if (true == m_pkMenu[i]->IsRenderOn())
		{
			m_pkMenu[i]->SetWidth(iWidth);
		}
	}

	m_pkBG->SetWidthWithoutEdge(iWidth + 5);
	m_pkBG->SetHeightWithoutEdge(iHeight * 20 - 5);
	m_pkBG->ToggleRender(true);

	SetWidth(m_pkBG->GetWidth());
	SetHeight(m_pkBG->GetHeight());
}

void GCNateOnGroupMultiMenu::OnCursorEnter(KD3DWnd* pkWnd)
{
	KD3DStatic* pkMenu = static_cast<KD3DStatic*>(pkWnd);
	for (int i = 0; i < MAX_GROUP_POPUP_NUM; ++i)
	{
		if (m_pkMenu[i] == pkMenu)
		{
			m_pkMenu[i]->SetFontColor(0xffffff00);
		}
		else
		{
			m_pkMenu[i]->SetFontColor(0xffffffff);
		}
	}
}

void GCNateOnGroupMultiMenu::OnCursorLeave(KD3DWnd* pkWnd)
{
	KD3DStatic* pkMenu = static_cast<KD3DStatic*>(pkWnd);
	for (int i = 0; i < MAX_GROUP_POPUP_NUM; ++i)
	{
		if (m_pkMenu[i] == pkMenu)
		{
			m_pkMenu[i]->SetFontColor(0xffffffff);
		}
	}
}

void GCNateOnGroupMultiMenu::OnClick(KD3DWnd* pkWnd)
{
	KD3DStatic* pkMenu = static_cast<KD3DStatic*>(pkWnd);
	for (int i = 0; i < MAX_GROUP_POPUP_NUM; ++i)
	{
		if (m_pkMenu[i] == pkMenu)
		{
			assert(i < (int)m_vecGroupID.size());
			SiKFriendShip()->MoveBuddyReq(m_nFromGroupID, m_vecGroupID[i], m_dwUserUID);
			OnMoving();
			break;
		}
	}
	ToggleRender(false);
}

void GCNateOnGroupMultiMenu::OnMoving()
{
	g_MyD3D->WaitForServerAck(Result_NateOn_Move_Group, INT_MAX, 30000, TIME_OUT_VALUE);

	switch (Result_NateOn_Move_Group)
	{
	case 0:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_MOVE_GROUP_SUCCESS), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		g_pkUIScene->m_pkNateOn->UpdateTreeData();
		break;
	}
	case 2: // 친구 요청중인 상태에서는 그룹 이동을 할 수 없습니다.
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_MOVE_NOT_FRIEND), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
	case 1: // 내 정보가 존재하지 않음
	case 11: // 해당그룹이 없음
	case 25: // 해당 그룹에 존재하지 않는 유저임
	default: // 알 수 없는 오류
	{
		std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KMSEvent::EMS_S2_MOVE_FRIEND_TO_GROUP_ACK, Result_NateOn_Move_Group);
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_MOVE_GROUP_ERROR),
			strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
	}
	}

	Result_NateOn_Move_Group = INT_MAX;
}