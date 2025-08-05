#include "stdafx.h"
#include ".\GCNateOnMultiMenu.h"
//
//

//
//
#include "GCNateOnGroupMultiMenu.h"
//

//
#include "../BuddySystem/KFriendShip.h"
#include "MyD3D.h"
#include "Procedure.h"
//
#include "GCTalkList.h"
#include "GCNateOnChat.h"
#include "GCCoupleSystem.h"

IMPLEMENT_CLASSNAME(GCNateOnMultiMenu);
IMPLEMENT_WND_FACTORY(GCNateOnMultiMenu);
IMPLEMENT_WND_FACTORY_NAME(GCNateOnMultiMenu, "gc_nateon_default_popup");

GCNateOnMultiMenu::GCNateOnMultiMenu(void)
	: m_pkBG(NULL)
	, m_pkGroupMultiMenu(NULL)
	, m_iCanFollowFriend(1)
{
	LINK_CONTROL("back", m_pkBG);

	for (int i = 0; i < MAX_POPUP_NUM; ++i)
	{
		m_pkMenu[i] = NULL;
	}

	char strTemp[MAX_PATH];
	for (int i = 0; i < MAX_POPUP_NUM; ++i)
	{
		sprintf(strTemp, "static%d", i + 1);
		LINK_CONTROL(strTemp, m_pkMenu[i]);
	}
	LINK_CONTROL("group_popup", m_pkGroupMultiMenu);

	m_mapExecuteFunc.clear();
	SetFunc();
}

GCNateOnMultiMenu::~GCNateOnMultiMenu(void)
{
	m_mapExecuteFunc.clear();
}

void GCNateOnMultiMenu::OnCreate()
{
	m_pkBG->InitState(false);
	for (int i = 0; i < MAX_POPUP_NUM; ++i)
	{
		m_pkMenu[i]->InitState(false, true, this);
		m_pkMenu[i]->SetAlign(DT_VCENTER);
	}
	m_pkGroupMultiMenu->InitState(false);

	g_pkUIMgr->ConnectGlobalEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK, this, &GCNateOnMultiMenu::OnClose);
}

void GCNateOnMultiMenu::OnClose(const KActionEvent& event)
{
	for (int i = 0; i < MAX_POPUP_NUM; i++)
	{
		if (m_pkMenu[i] == NULL)continue;
		if (m_pkMenu[i] == event.m_pWnd)
			return;
	}

	if (event.m_pWnd != this && m_pkMenu[3])
		ToggleRender(false);
}

void GCNateOnMultiMenu::ActionPerformed(const KActionEvent& event)
{
	for (int i = 0; i < MAX_POPUP_NUM; ++i)
	{
		GCWND_WND_MAP(m_pkMenu[i], KD3DWnd::D3DWE_CURSOR_ENTER, OnCursorEnter);
		GCWND_WND_MAP(m_pkMenu[i], KD3DWnd::D3DWE_CURSOR_LEAVE, OnCursorLeave);
		GCWND_WND_MAP(m_pkMenu[i], KD3DWnd::D3DWE_BUTTON_CLICK, OnClick);
	}
}

void GCNateOnMultiMenu::OnCreateComplete()
{
}

void GCNateOnMultiMenu::SetMode(GCNateOn::E_NATEON_MODE eMode, const FRIEND_UI_DATA& kData)
{
	switch (eMode)
	{
	case GCNateOn::CHAT_MODE:
	{
		m_pkMenu[0]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU7));
		m_pkMenu[0]->ToggleRender(true);
		m_pkMenu[1]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU5));
		m_pkMenu[1]->ToggleRender(true);

		for (int i = 2; i < MAX_POPUP_NUM; ++i)
		{
			m_pkMenu[i]->ToggleRender(false);
		}
		m_pkGroupMultiMenu->ToggleRender(false);
		AdjustMenuSize();
		break;
	}
	case GCNateOn::FRIEND_MODE:
	{
		if (true == kData.m_bGroup)
		{
			// default그룹은 위에서 걸러줌.
			m_pkMenu[0]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU1));
			m_pkMenu[0]->ToggleRender(true);
			m_pkMenu[1]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU2));
			m_pkMenu[1]->ToggleRender(true);
			for (int i = 2; i < MAX_POPUP_NUM; ++i)
			{
				m_pkMenu[i]->ToggleRender(false);
			}
			m_pkGroupMultiMenu->ToggleRender(false);
			AdjustMenuSize();
		}
		else
		{
			m_pkMenu[0]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU3));
			m_pkMenu[0]->ToggleRender(true);
			m_pkMenu[1]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU5));
			m_pkMenu[1]->ToggleRender(true);
			m_pkMenu[2]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU6));
			m_pkMenu[2]->ToggleRender(true);
			if (kData.m_bOnline && eMode != GCNateOn::BLOCK_MODE)
			{
				m_pkMenu[3]->SetText(g_pkStrLoader->GetString(STR_ID_FOLLOW_FRIEND_7));
				m_pkMenu[3]->ToggleRender(false);
				m_pkMenu[4]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU9));
				m_pkMenu[4]->ToggleRender(true);
				m_pkMenu[5]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU4));
				m_pkMenu[5]->ToggleRender(false);
			}
			else
			{
				m_pkMenu[3]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU9));
				m_pkMenu[3]->ToggleRender(true);
				m_pkMenu[4]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU4));
				m_pkMenu[4]->ToggleRender(false);
				m_pkMenu[5]->ToggleRender(false);
			}
			m_pkGroupMultiMenu->ToggleRender(false);
			m_pkGroupMultiMenu->SetGroups(kData.m_dwUserUID);
			AdjustMenuSize();
		}
		break;
	}
	case GCNateOn::GUILD_MODE:
	{
		m_pkMenu[0]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU3));
		m_pkMenu[0]->ToggleRender(true);
		m_pkMenu[1]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU7));
		m_pkMenu[1]->ToggleRender(true);
		if (kData.m_bOnline && eMode != GCNateOn::BLOCK_MODE)
		{
			m_pkMenu[2]->SetText(g_pkStrLoader->GetString(STR_ID_FOLLOW_FRIEND_7));
			m_pkMenu[2]->ToggleRender(false);
		}
		else
		{
			m_pkMenu[2]->ToggleRender(false);
		}
		m_pkMenu[3]->ToggleRender(false);

		m_pkMenu[4]->ToggleRender(false);
		m_pkMenu[5]->ToggleRender(false);
		m_pkGroupMultiMenu->ToggleRender(false);
		AdjustMenuSize();
		break;
	}
	case GCNateOn::BLOCK_MODE:
	{
		m_pkMenu[0]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU8));
		m_pkMenu[0]->ToggleRender(true);
		m_pkMenu[1]->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU6));
		m_pkMenu[1]->ToggleRender(true);
		for (int i = 2; i < MAX_POPUP_NUM; ++i)
		{
			m_pkMenu[i]->ToggleRender(false);
		}
		m_pkGroupMultiMenu->ToggleRender(false);
		AdjustMenuSize();
		break;
	}
	default:
	{
		assert(!"invalid mode");
		break;
	}
	}
	m_eMode = eMode;
	m_kUIData = kData;
	m_iCanFollowFriend = 1;
	if (kData.m_bOnline && eMode != GCNateOn::BLOCK_MODE) {
		m_iCanFollowFriend = 0;
	}
}

void GCNateOnMultiMenu::AdjustMenuSize()
{
	int iWidth = 0;
	int iHeight = 0;
	for (int i = 0; i < MAX_POPUP_NUM; ++i)
	{
		if (true == m_pkMenu[i]->IsRenderOn())
		{
			iWidth = max(iWidth, g_pkFontMgr->GetWidth(m_pkMenu[i]->GetOriginalText().c_str()));
			iHeight++;
		}
	}

	for (int i = 0; i < MAX_POPUP_NUM; ++i)
	{
		if (true == m_pkMenu[i]->IsRenderOn())
		{
			m_pkMenu[i]->SetWidth(iWidth);
		}
	}

	m_pkBG->SetWidthWithoutEdge(iWidth + 5);
	m_pkBG->SetHeightWithoutEdge(iHeight * 20 - 5);
	m_pkBG->ToggleRender(true);
}

void GCNateOnMultiMenu::OnCursorEnter(KD3DWnd* pkWnd)
{
	KD3DStatic* pkMenu = static_cast<KD3DStatic*>(pkWnd);
	for (int i = 0; i < MAX_POPUP_NUM; ++i)
	{
		if (m_pkMenu[i] == pkMenu)
		{
			m_pkMenu[i]->SetFontColor(0xffffff00);
			if (m_eMode == GCNateOn::FRIEND_MODE
				&& m_pkMenu[i]->GetText() == g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU9)
				&& (m_pkGroupMultiMenu->GetGroupCount() > 0)
				)
			{
				m_pkGroupMultiMenu->ToggleRender(true);
			}
		}
		else
		{
			if (m_pkGroupMultiMenu->IsRenderOn() &&
				m_eMode == GCNateOn::FRIEND_MODE &&
				m_pkMenu[i]->GetText() == g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU9))
			{
				m_pkGroupMultiMenu->ToggleRender(false);
			}

			m_pkMenu[i]->SetFontColor(0xffffffff);
		}
	}
}

void GCNateOnMultiMenu::OnCursorLeave(KD3DWnd* pkWnd)
{
	KD3DStatic* pkMenu = static_cast<KD3DStatic*>(pkWnd);
	for (int i = 0; i < MAX_POPUP_NUM; ++i)
	{
		if (m_pkMenu[i] == pkMenu)
		{
			m_pkMenu[i]->SetFontColor(0xffffffff);
		}
	}
}

void GCNateOnMultiMenu::FrameMoveInEnabledState()
{
	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos((float)pt.x, (float)pt.y);
	if (GetParent()->CheckPosInWindowBoundWithChild(vMousePos, true) == false)
	{
		m_pkGroupMultiMenu->ToggleRender(false);
	}
}

void GCNateOnMultiMenu::AdjustChildPosition()
{
	D3DXVECTOR2 vMenuPos((float)m_pkGroupMultiMenu->GetWidth(), 0.0f);
	vMenuPos.x *= -1.0f;
	vMenuPos.y = (float)GetHeight() - (float)m_pkGroupMultiMenu->GetHeight() - 5.0f;
	m_pkGroupMultiMenu->SetWindowPosDirect(vMenuPos);
}

void GCNateOnMultiMenu::OnClick(KD3DWnd* pkWnd)
{
	KD3DStatic* pkMenu = static_cast<KD3DStatic*>(pkWnd);

	if (pkMenu->GetOriginalText() == g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU9))
	{
		return;
	}

	std::map< std::wstring, EXECEFUNC >::iterator mit = m_mapExecuteFunc.find(pkMenu->GetOriginalText());
	assert(mit != m_mapExecuteFunc.end());

	EXECEFUNC pfn = mit->second;
	(this->*pfn)();

	m_pkGroupMultiMenu->ToggleRender(false);
	ToggleRender(false);
}

void GCNateOnMultiMenu::SetFunc()
{
	m_mapExecuteFunc[g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU1)] = &GCNateOnMultiMenu::RenameGroup;
	m_mapExecuteFunc[g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU2)] = &GCNateOnMultiMenu::DeleteGroup;
	m_mapExecuteFunc[g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU3)] = &GCNateOnMultiMenu::StartChat;
	m_mapExecuteFunc[g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU4)] = &GCNateOnMultiMenu::SendMail;
	m_mapExecuteFunc[g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU5)] = &GCNateOnMultiMenu::Block;
	m_mapExecuteFunc[g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU6)] = &GCNateOnMultiMenu::DeleteBuddy;
	m_mapExecuteFunc[g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU7)] = &GCNateOnMultiMenu::RegisterBuddy;
	m_mapExecuteFunc[g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU8)] = &GCNateOnMultiMenu::UnBlock;
	m_mapExecuteFunc[g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU9)] = &GCNateOnMultiMenu::MoveBuddy;
	m_mapExecuteFunc[g_pkStrLoader->GetString(STR_ID_FOLLOW_FRIEND_7)] = &GCNateOnMultiMenu::FollowFriend;
}

void GCNateOnMultiMenu::RenameGroup()
{
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NATEON_INPUT_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NATEON_RENAME_GROUP, m_kUIData.m_nGroupID, 0, false, true);
}

void GCNateOnMultiMenu::DeleteGroup()
{
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString(STR_ID_NATEON_DELETE_GROUP_ASK1)
		, L"", KGCUIScene::GC_MBOX_USE_NATEON_DELETE_GROUP, m_kUIData.m_nGroupID, 0, false, true);
}

void GCNateOnMultiMenu::StartChat()
{
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);

	START_LOG(clog, L"StartChat()");

	std::vector<DWORD> vChatUser;
	vChatUser.push_back(m_kUIData.m_dwUserUID);
	vChatUser.push_back(g_kGlobalValue.m_kUserInfo.dwUID);

	// 같은 유저들의 모임이 있다면, 그 대화 창을 열어준다.
	std::pair<DWORD, DWORD> Key = g_pkUIScene->m_pkNateOn->GetNateOnChatWindow()->GetAllTalkerEqual(vChatUser);
	if (0 == Key.first && 0 == Key.second)
	{
		std::set<DWORD> setChatUIDs;
		setChatUIDs.insert(m_kUIData.m_dwUserUID);
		KP2P::GetInstance()->m_kMsgServer->Send_StartChatReq_S2(setChatUIDs);
	}
	else
	{
		g_pkUIScene->m_pkNateOn->SetNateonMode(GCNateOn::CHAT_MODE);
		g_pkUIScene->m_pkNateOn->GetNateOnChatWindow()->SetCurTalkKey(Key);
		g_pkUIScene->m_pkNateOn->GetTalkListWindow()->SetCurTalkKey(Key);
		g_pkUIScene->m_pkNateOn->GetNateOnChatWindow()->RefreshTalkBox(Key);
		g_pkUIScene->m_pkNateOn->GetTalkListWindow()->SetFirstIndexByKey(Key);
	}
}

void GCNateOnMultiMenu::SendMail()
{
	START_LOG(clog, L"SendMail()");
}

void GCNateOnMultiMenu::Block()
{
	// 커플인 경우 블록 하지 않는다.
	if (SiKGCCoupleSystem()->Are_you_My_Honey(m_kUIData.m_dwUserUID))
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM40)
			, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);

		return;
	}

	if (SiKFriendShip()->FindUIDInContactFriendList(m_kUIData.m_dwUserUID))
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_CAN_NOT_DELETE_RECOMMENDER)
			, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);

		return;
	}

	// 여기서 -1은 친구 그릅에서 친구를 찾을 수 없었다는 것이 므로...
	if (-1 == m_kUIData.m_nGroupID)
	{
		KBuddy FData;
		if (SiKFriendShip()->GetBlockBuddy(m_kUIData.m_dwUserUID, FData))
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_BLOCK_BUDDY_ALREADY)
				, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		}
		else
		{
			g_pkUIScene->BlockBuddy(m_kUIData.m_strDisplay);
		}
	}
	else
	{
		SiKFriendShip()->BlockMyFriendReq(m_kUIData.m_nGroupID, m_kUIData.m_dwUserUID);
		BlockFriendAck();
	}
}

void GCNateOnMultiMenu::DeleteBuddy()
{
	if (false == SiKGCCoupleSystem()->Am_I_Solo())
	{
		if (m_kUIData.m_dwUserUID == SiKGCCoupleSystem()->GetUID_of_my_honey())
		{
			// 메시지 출력하고 돌려줘야할듯. 커플 해제하고 삭제하세효
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM41)
				, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
			return;
		}
	}

	if (SiKFriendShip()->FindUIDInContactFriendList(m_kUIData.m_dwUserUID))
	{
		// 메시지 출력하고 돌려줘야할듯. 커플 해제하고 삭제하세효
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_CAN_NOT_DELETE_RECOMMENDER)
			, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return;
	}

	SiKFriendShip()->DeleteBuddyReq(m_kUIData.m_nGroupID, m_kUIData.m_dwUserUID);
	DeleteBuddyAck();
}

void GCNateOnMultiMenu::RegisterBuddy()
{
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);
	//std::wstring strNickName;
	//strNickName = g_pkUIScene->m_pkNateOn->GetNateOnChatWindow()->GetUserNickName( m_kUIData.m_dwUserUID );
	KBuddy FData;
	if (SiKFriendShip()->GetBlockBuddy(m_kUIData.m_dwUserUID, FData))
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_BLOCK_FRIEND_TO_ADD)
			, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return;
	}

	if (m_kUIData.m_strDisplay.length())
	{
		KP2P::GetInstance()->m_kMsgServer->Send_AddFriendReq_S2(m_kUIData.m_strNickname
			, g_pkStrLoader->GetString(STR_ID_NATEON_ADD_BUDDY_DEFMSG));
	}
	else
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_ERROR_ADD_DEFAULT)
			, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
	}
}

void GCNateOnMultiMenu::UnBlock()
{
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);
	KBuddy FData;
	if (SiKFriendShip()->GetBlockBuddy(m_kUIData.m_dwUserUID, FData))
	{
		if (SiKGCCoupleSystem()->GetUID_of_my_honey() == m_kUIData.m_dwUserUID)
		{
			KP2P::GetInstance()->m_kMsgServer->Send_UnBlockMoveFriendReq_S2(m_kUIData.m_dwUserUID);
			//g_pkUIScene->m_pkNateOn->GetMultiMenu()->UnBlockFriendAck();

			SpeakToActionListener(KActionEvent(this, KD3DWnd::D3DWE_EDIT_RETURN));
		}
		else if (SiKFriendShip()->FindUIDInContactFriendList(m_kUIData.m_dwUserUID))
		{
			KP2P::GetInstance()->m_kMsgServer->Send_UnBlockMoveFriendReq_S2(m_kUIData.m_dwUserUID);
			SpeakToActionListener(KActionEvent(this, KD3DWnd::D3DWE_EDIT_RETURN));
		}
		else if (FData.IsFriend())
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NATEON_BLOCK_TO_ADD_BUDDY, FData.GetNickName()
				, L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_kUIData.m_dwUserUID, 0, false, true);
		}
		else
		{
			SiKFriendShip()->UnBlockMyFriendReq(m_kUIData.m_dwUserUID);
			//UnBlockFriendAck();
		}
	}
}

void GCNateOnMultiMenu::MoveBuddy()
{
	// 아무것도 안해여..
}

void GCNateOnMultiMenu::BlockFriendAck()
{
	g_MyD3D->WaitForServerAck(Result_NateOn_Block_Buddy, INT_MAX, 30000, TIME_OUT_VALUE);

	switch (Result_NateOn_Block_Buddy)
	{
	case 0:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_BLOCK_BUDDY_SUCCESS), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		g_pkUIScene->m_pkNateOn->UpdateTreeData();
		break;
	}
	case 2: // 상대방 정보가 존재하지 않음
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_DONT_FIND), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
	case 6: // 자기 자신을 추가하려 함
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_BLOCK_ME), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
	case 7: // 이미 요청한 상태임
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_BLOCK_ALREADY), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
	case 8: // 제한 인원수 초과
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_BLOCK_OVERFLOW), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
		//case 1: // 내 정보가 존재하지 않음
	 //   case 25: // 해당 그룹에 존재하지 않는 유저임
	 //   case 11: // 해당하는 그룹이 없음
	 //   case 16: // 해당 그룹에 이미 등록된 유저임
	 //   case 27: // 그룹내 해당유저를 찾을 수 없음
	default: // 알 수 없는 오류
	{
		std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KMSEvent::EMS_S2_BLOCK_MOVE_FRIEND_ACK, Result_NateOn_Block_Buddy);
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_BLOCK_BUDDY_ERROR),
			strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
	}
	}

	Result_NateOn_Block_Buddy = INT_MAX;
}

void GCNateOnMultiMenu::UnBlockFriendAck()
{
	g_MyD3D->WaitForServerAck(Result_NateOn_UnBlock_Buddy, INT_MAX, 30000, TIME_OUT_VALUE);

	switch (Result_NateOn_UnBlock_Buddy)
	{
	case 0:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_UNBLOCK_BUDDY_SUCCESS), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		g_pkUIScene->m_pkNateOn->UpdateTreeData();
		break;
	}
	case 2: // 해당 리스트에 없는 유저임
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_DONT_FIND), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
	case 6: // 자기 자신을 추가하려 함
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_UNBLOCK_ME), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
		//case 1: // 내 정보가 존재하지 않음
	 //   case 7: // 이미 요청한 상태,이미 친구임
	 //   case 11: // 해당하는 그룹이 없음
	 //   case 16: // 해당 그룹에 이미 등록된 유저임
	 //   case 27: // 그룹내 해당유저를 찾을 수 없음
	default: // 알 수 없는 오류
	{
		std::wstring strTemp = g_pkStrLoader->GetReplacedString(STR_UNKWON_ERROR_CODE, "ii", KMSEvent::EMS_S2_UNBLOCK_MOVE_FRIEND_ACK, Result_NateOn_UnBlock_Buddy);
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_UNBLOCK_BUDDY_ERROR),
			strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
	}
	}

	Result_NateOn_UnBlock_Buddy = INT_MAX;
}

void GCNateOnMultiMenu::DeleteBuddyAck()
{
	g_MyD3D->WaitForServerAck(Result_NateOn_Delete_Buddy, INT_MAX, 30000, TIME_OUT_VALUE);

	switch (Result_NateOn_Delete_Buddy)
	{
	case 0:
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_DELETE_BUDDY_SUCCESS), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		g_pkUIScene->m_pkNateOn->UpdateTreeData();
		break;
	}
	case 2: // 상대방 정보가 존재하지 않음
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_DONT_FIND), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
		//case 1: // 내 정보가 존재하지 않음
	 //   case 11: // 해당 그룹이 없음
	 //   case 27: // 해당 그룹에 존재하지 않는 유저임
	default: // 알 수 없는 오류
	{
		std::wstring strTemp = boost::str(boost::wformat(L"(%1% - %2%)") % KMSEvent::EMS_S2_DELETE_FRIEND_ACK%Result_NateOn_Delete_Buddy);
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_DELETE_BUDDY_ERROR),
			strTemp, KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		break;
	}
	}

	Result_NateOn_Delete_Buddy = INT_MAX;
}

void GCNateOnMultiMenu::FollowFriend()
{
	if (m_iCanFollowFriend != 0) {
		return;
	}

	if (!SiKGCPrivateDungeonManager()->IsClearPrivateDungeon(g_kGlobalValue.m_kUserInfo.GetCurrentCharType()))
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
			g_pkStrLoader->GetString(STR_ID_NO_CLEAR_PRIVATE_DUNGEON),
			L"",
			KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return;
	}

	g_pkUIScene->m_pkNateOn->OnViewWaitImage();

	KP2P::GetInstance()->Send_UserServerRoomReq(m_kUIData.m_dwUserUID);
}