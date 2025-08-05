#include "stdafx.h"
#include ".\GCNateOn.h"
//
//
#include "KGCUIHelper.h"
//
//

#include "GCBuddyChild.h"
#include "GCNateOnChat.h"
//
#include "../BuddySystem/KFriendShip.h"
#include "GCNateOnMultiMenu.h"
#include "../BuddySystem/KFriendShip.h"
//
#include "../KGCEmoticon.h"
#include "../gcui/GCTalkList.h"
//

#include "GCCoupleSystem.h"
#include "GCMultiMenu.h"
#include "BuddySystem/KFriendShip.h"

IMPLEMENT_CLASSNAME(GCNateOn);
IMPLEMENT_WND_FACTORY(GCNateOn);
IMPLEMENT_WND_FACTORY_NAME(GCNateOn, "gc_nateon_dlg");

GCNateOn* g_pNateOn = NULL;

GCNateOn::GCNateOn(void)
	: m_pkNickName(NULL)
	, m_pkClose(NULL)
	, m_pkSendMailBtn(NULL)
	, m_pkInboxBtn(NULL)
	, m_pkDraftsBtn(NULL)
	, m_pkOnline(NULL)
	, m_pkAddBuddyBtn(NULL)
	, m_pkBlockBuddyBtn(NULL)
	, m_pkAddGroupBtn(NULL)
	, m_pkBuddyListBtn(NULL)
	, m_pkGuildListBtn(NULL)
	, m_pkBlockListBtn(NULL)
	, m_pkModeBuddyList(NULL)
	, m_pkModeGuildList(NULL)
	, m_pkModeBlockList(NULL)
	, m_eMode(INIT_MODE)
	, m_pkTree(NULL)
	, m_pkMultiMenu(NULL)
	, m_pkChangeEmoticon(NULL)
	, m_pkEmoticon(NULL)
	, m_pkEmoticonBG(NULL)
	, m_pkTalkList(NULL)
	, m_pkSpecialFriend(NULL)
	, m_pkCouplePopup(NULL)
	, m_pkRecommend(NULL)
	, m_pkGuildOfflineCheckBox(NULL)
	, m_pkGuildOfflineCheckSign(NULL)
	, m_pkGuildBlockCheckBox(NULL)
	, m_pkGuildBlockCheckSign(NULL)
	, m_pkGuildName(NULL)
	, m_pkGuildOnline(NULL)
	, m_pkGuildOfflineCheck(NULL)
	, m_pkGuildBlockCheck(NULL)
	, m_pkTexGuildMark(NULL)
	, m_pkWaitImage(NULL)
{
	assert(g_pNateOn == NULL);
	if (g_pNateOn == NULL)
	{
		g_pNateOn = this;
	}

	for (int i = 0; i < GC_GRADE_NUM; ++i)
		m_ppkGrade[i] = NULL;

	LINK_CONTROL("static_myname", m_pkNickName);
	LINK_CONTROL("player_grade_holy", m_ppkGrade[GC_GRADE_HOLY]);
	LINK_CONTROL("player_grade_royal", m_ppkGrade[GC_GRADE_ROYAL]);
	LINK_CONTROL("player_grade_mercenary", m_ppkGrade[GC_GRADE_MERCENARY]);
	LINK_CONTROL("player_grade_vagabond", m_ppkGrade[GC_GRADE_VAGABOND]);
	LINK_CONTROL("close_btn", m_pkClose);
	LINK_CONTROL("send_mail_btn", m_pkSendMailBtn);
	LINK_CONTROL("inbox_btn", m_pkInboxBtn);
	LINK_CONTROL("drafts_btn", m_pkDraftsBtn);
	LINK_CONTROL("static_online", m_pkOnline);
	LINK_CONTROL("add_buddy_btn", m_pkAddBuddyBtn);
	LINK_CONTROL("block_buddy_btn", m_pkBlockBuddyBtn);
	LINK_CONTROL("add_group_btn", m_pkAddGroupBtn);
	LINK_CONTROL("buddy_list_btn", m_pkBuddyListBtn);
	LINK_CONTROL("guild_list_btn", m_pkGuildListBtn);
	LINK_CONTROL("block_list_btn", m_pkBlockListBtn);
	LINK_CONTROL("buddy_list_on_btn", m_pkModeBuddyList);
	LINK_CONTROL("guild_list_on_btn", m_pkModeGuildList);
	LINK_CONTROL("block_list_on_btn", m_pkModeBlockList);
	LINK_CONTROL("buddy_tree", m_pkTree);
	LINK_CONTROL("default_popup", m_pkMultiMenu);
	LINK_CONTROL("emoticon_btn", m_pkChangeEmoticon);
	LINK_CONTROL("emoticon", m_pkEmoticon);
	LINK_CONTROL("bg_emoticon", m_pkEmoticonBG);
	LINK_CONTROL("talk_window", m_pkChat);
	LINK_CONTROL("talk_list", m_pkTalkList);
	LINK_CONTROL("special_friend", m_pkSpecialFriend);
	LINK_CONTROL("specialfriend_popup", m_pkCouplePopup);

	LINK_CONTROL("recommend_btn", m_pkRecommend);

	//길드 메신저용 추가 ui
	LINK_CONTROL("guild_offline_check_box", m_pkGuildOfflineCheckBox);
	LINK_CONTROL("guild_offline_check_sign", m_pkGuildOfflineCheckSign);
	LINK_CONTROL("guild_block_check_box", m_pkGuildBlockCheckBox);
	LINK_CONTROL("guild_block_check_sign", m_pkGuildBlockCheckSign);

	LINK_CONTROL("static_guild_name", m_pkGuildName);
	LINK_CONTROL("static_guild_online", m_pkGuildOnline);
	LINK_CONTROL("static_guild_offline_check", m_pkGuildOfflineCheck);
	LINK_CONTROL("static_guild_block_check", m_pkGuildBlockCheck);
	LINK_CONTROL("tex_guild_mark ", m_pkTexGuildMark);
	LINK_CONTROL("tex_wait_image", m_pkWaitImage);

	m_bNew = false;

	m_bViewRecommendBtn = true;

	m_mapTopIndex.clear();
}

GCNateOn::~GCNateOn(void)
{
}

void GCNateOn::OnCreate()
{
	m_pkNickName->InitState(true);
	SetNickName();
	SetGrade();
	m_pkClose->SetHotKey(DIK_ESCAPE);
	m_pkClose->InitState(true, true, this);
	SetOnlineCount();
	SetTooltips();

	m_pkSendMailBtn->InitState(false, true, this);
	m_pkInboxBtn->InitState(false, true, this);
	m_pkDraftsBtn->InitState(false, true, this);
	m_pkAddBuddyBtn->InitState(true, true, this);
	m_pkBlockBuddyBtn->InitState(true, true, this);
	m_pkAddGroupBtn->InitState(true, true, this);
	m_pkBuddyListBtn->InitState(true, true, this);
#if !defined( GUILD_SYSTEM )
	m_pkGuildListBtn->InitState(false);
	m_pkGuildOfflineCheckBox->InitState(false);
	m_pkGuildOfflineCheckSign->InitState(false);
	m_pkGuildBlockCheckBox->InitState(false);
	m_pkGuildBlockCheckSign->InitState(false);
	m_pkGuildName->InitState(false);
	m_pkGuildOnline->InitState(false);
	m_pkGuildOfflineCheck->InitState(false);
	m_pkGuildBlockCheck->InitState(false);
#else
	m_pkGuildListBtn->InitState(true, true, this);
	if (g_kGlobalValue.m_kGuildInfo.m_dwUID == 0 || g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_JOINER)
		m_pkGuildListBtn->Lock(true);
	else
		m_pkGuildListBtn->Lock(false);
	m_pkGuildOfflineCheckBox->InitState(false, true, this);
	m_pkGuildOfflineCheckSign->InitState(false, true, this);
	m_pkGuildOfflineCheckSign->ToggleRender(m_pkTree->GetGuildOfflineMember());
	m_pkGuildBlockCheckBox->InitState(false, true, this);
	m_pkGuildBlockCheckSign->InitState(false, true, this);
	m_pkGuildBlockCheckSign->ToggleRender(m_pkTree->GetGuildBlockMember());
	m_pkGuildName->InitState(false, true, this);
	m_pkGuildName->SetAlign(DT_LEFT);
	m_pkGuildName->SetText(g_kGlobalValue.m_kGuildInfo.m_strName);
	m_pkGuildOnline->InitState(false, true, this);
	m_pkGuildOnline->SetAlign(DT_LEFT);
	m_pkGuildOfflineCheck->InitState(false, true, this);
	m_pkGuildOfflineCheck->SetAlign(DT_LEFT);
	m_pkGuildOfflineCheck->SetText(g_pkStrLoader->GetString(STR_ID_GUILD_MESSENGER_OFFLINE_SIGN).c_str());
	m_pkGuildBlockCheck->InitState(false, true, this);
	m_pkGuildBlockCheck->SetAlign(DT_LEFT);
	m_pkGuildBlockCheck->SetText(g_pkStrLoader->GetString(STR_ID_GUILD_MESSENGER_BLOCK_SIGN).c_str());
#endif
	m_pkBlockListBtn->InitState(true, true, this);
	m_pkModeBuddyList->InitState(true, true, this);
	m_pkModeGuildList->InitState(true, true, this);
	m_pkModeBlockList->InitState(true, true, this);
	m_pkChangeEmoticon->InitState(true, true, this);
	m_pkEmoticon->InitState(true, true, this);
	m_pkEmoticonBG->InitState(true, true, this);
	m_pkChat->InitState(false, true, this);
	m_pkTalkList->InitState(false, true, this);
	m_pkSpecialFriend->InitState(false, true, this);
	m_pkCouplePopup->InitState(false, true, this);
	m_pkCouplePopup->ClearMenu();
	m_pkCouplePopup->AddMenu(1, g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU3));
	m_pkCouplePopup->AddMenu(2, g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM2));
	m_pkCouplePopup->AddMenu(3, g_pkStrLoader->GetString(STR_ID_FOLLOW_FRIEND_7));
	//m_pkCouplePopup->AddMenu( 3, g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU4), 0xff888888, false );
	m_pkWaitImage->InitState(false, false, this);
	m_pkMultiMenu->InitState(false);

	m_pkTexGuildMark->InitState(false);

	if (GCNateOn::INIT_MODE == m_eMode)
	{
		SetNateonMode(GCNateOn::FRIEND_MODE);

		m_mapTopIndex[GCNateOn::FRIEND_MODE] = 0;
		m_mapTopIndex[GCNateOn::BLOCK_MODE] = 0;
		m_mapTopIndex[GCNateOn::GUILD_MODE] = 0;
	}
	else
	{
		SetNateonMode(m_eMode);
	}

	if (KP2P::GetInstance()->m_kMsgServer)
	{
		KP2P::GetInstance()->m_kMsgServer->Send_LoadSentPaperboxReq_S2();
		KP2P::GetInstance()->m_kMsgServer->Send_LoadRecvPaperboxReq_S2();
	}

#if defined WELCOMEBACK_FRIEND
	if (g_kGlobalValue.m_bIsRecommendEvent == false || m_bViewRecommendBtn == false || Result_RecomEnable == 1 || m_eMode == GCNateOn::GUILD_MODE)
		m_pkRecommend->ToggleRender(false);
	else
	{
		m_pkRecommend->InitState(true, true, this);
		m_pkRecommend->SetAlertButtonType(KGCAlertWnd::RECOMMEND);
	}
#else
	m_pkRecommend->ToggleRender(false);
#endif
}

void GCNateOn::SetGrade()
{
	{
		g_pkUIHelper->SetCharacterGrade(m_ppkGrade, g_kGlobalValue.m_kUserInfo.GetUserGrade());
	}
}

void GCNateOn::ActionPerformed(const KActionEvent& event)
{
	if (m_pkWaitImage == event.m_pWnd) return;

	if (m_pkMultiMenu->IsRenderOn() == true)
	{
		if (event.m_pWnd != m_pkMultiMenu && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK)
		{
			m_pkMultiMenu->ToggleRender(false);
		}
	}

	GCWND_MSG_MAP(m_pkClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose);
	GCWND_MSG_MAP(m_pkBuddyListBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnBuddyList);
	GCWND_MSG_MAP(m_pkGuildListBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnGuildList);
	GCWND_MSG_MAP(m_pkBlockListBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnBlockList);
	GCWND_MSG_MAP(m_pkAddGroupBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnAddGroup);
	GCWND_MSG_MAP(m_pkBlockBuddyBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnBlockBuddy);
	GCWND_MSG_MAP(m_pkChangeEmoticon, KD3DWnd::D3DWE_BUTTON_CLICK, OnChangeEmoticon);
	GCWND_MSG_MAP(m_pkEmoticon, KD3DWnd::D3DWE_BUTTON_CLICK, OnChangeEmoticon);
	GCWND_MSG_MAP(m_pkAddBuddyBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnAddBuddy);
	GCWND_MSG_MAP(m_pkAddBuddyBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnAddBuddy);
	GCWND_MSG_MAP(m_pkTalkList, NATEON_TALK_BOX_SELECT, OnSelectTalkBox);
	GCWND_MSG_MAP(m_pkSpecialFriend, KD3DWnd::D3DWE_RBUTTON_CLICK, OnRClickCouple);
	GCWND_MSG_MAP(m_pkRecommend, KD3DWnd::D3DWE_BUTTON_CLICK, OnRecommend);
	OnClickCouplePopup(event);

	if (m_pkChat == event.m_pWnd)
	{
		if (NATEON_TALK_USER_SELECT == event.m_dwCode)
		{
			NATE_TALK_BOX* pTalkBox = GetNateOnChatWindow()->GetTalkBox(GetNateOnChatWindow()->GetCurTalkKey());
			if (pTalkBox)
			{
				FRIEND_UI_DATA kData;
				kData.m_bUser = true;
				kData.m_strDisplay = GetNateOnChatWindow()->GetUserListBoxNickName(static_cast<const int>(event.m_dwlParam));
				kData.m_dwUserUID = pTalkBox->GetTalker(kData.m_strDisplay);
				kData.m_nGroupID = SiKFriendShip()->GetFriendBuddyGroupID(kData.m_dwUserUID);

				// 내가 아닌경우만 멀티메뉴를 띄운다. (친구추가/차단)
				if (kData.m_dwUserUID != g_kGlobalValue.m_kUserInfo.dwUID)
				{
					OnTreeRButtonClick(kData);
				}
			}
		}
	}

	if (KD3DWnd::D3DWE_BUTTON_CLICK == event.m_dwCode)
	{
		if (m_pkSendMailBtn == event.m_pWnd)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NATEON_MESSAGE,
				L"", L"", KGCUIScene::GC_MBOX_USE_NATEON_MSG_SEND, 0, 0, true, true);
		}

		if (m_pkInboxBtn == event.m_pWnd)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NATEON_MESSAGE,
				L"", L"", KGCUIScene::GC_MBOX_USE_NATEON_MSG_RECIVE, 0, 0, true, true);
		}

		if (m_pkDraftsBtn == event.m_pWnd)
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NATEON_MESSAGE,
				L"", L"", KGCUIScene::GC_MBOX_USE_NATEON_MSG_SENDED, 0, 0, true, true);
		}
	}
}

void GCNateOn::OnRClickCouple()
{
	POINT pt = g_pkInput->GetMousePos();
	pt.x = (LONG)(pt.x / m_fWindowScaleX);
	pt.y = (LONG)(pt.y / m_fWindowScaleY);

	D3DXVECTOR2 vMousePos((float)(pt.x - this->GetFixedWindowLocalPos().x - 4.0f), (float)(pt.y - this->GetFixedWindowLocalPos().y - 4.0f));

	m_pkCouplePopup->SetWindowPosDirect(vMousePos);
	m_pkCouplePopup->ToggleRender(true);
}

void GCNateOn::OnClickCouplePopup(const KActionEvent& event)
{
	if (event.m_pWnd != m_pkCouplePopup && event.m_dwCode != KGCMultiMenu::K3DWE_MENU_CLICKED)
		return;

	while (event.m_dwlParam == 1)
	{
		START_LOG(clog, L"StartChat()");

		KBuddy kFriend;
		FRIEND_UI_DATA kFriendUIData;
		if (SiKFriendShip()->GetFriendBuddyByUID(SiKGCCoupleSystem()->GetUID_of_my_honey(), kFriend))
		{
			kFriend.GetUIData(kFriendUIData);
		}
		else
			break;

		std::vector<DWORD> vChatUser;
		vChatUser.push_back(SiKGCCoupleSystem()->GetUID_of_my_honey());
		vChatUser.push_back(g_kGlobalValue.m_kUserInfo.dwUID);

		// 같은 유저들의 모임이 있다면, 그 대화 창을 열어준다.
		std::pair<DWORD, DWORD> Key = GetNateOnChatWindow()->GetAllTalkerEqual(vChatUser);
		if (0 == Key.first && 0 == Key.second)
		{
			std::set<DWORD> setChatUIDs;
			setChatUIDs.insert(kFriendUIData.m_dwUserUID);
			if (KP2P::GetInstance()->m_kMsgServer != NULL)
				KP2P::GetInstance()->m_kMsgServer->Send_StartChatReq_S2(setChatUIDs);
		}
		else
		{
			SetNateonMode(GCNateOn::CHAT_MODE);
			GetNateOnChatWindow()->SetCurTalkKey(Key);
			GetTalkListWindow()->SetCurTalkKey(Key);
			GetNateOnChatWindow()->RefreshTalkBox(Key);
			GetTalkListWindow()->SetFirstIndexByKey(Key);
		}
		break;
	}

	switch (event.m_dwlParam)
	{
		// 	case 2:                                     // 쪽지 버튼 없애면서
		// 		START_LOG( clog, L"SendMail()" );
		// 		break;
	case 2:
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_COUPLE_GOODBYE, L"", L"", KGCUIScene::GC_MBOX_USE_COUPLE_GOODBYE_REQ);
		OnClose();
		break;
	case 3:

		if (!SiKGCPrivateDungeonManager()->IsClearPrivateDungeon(g_kGlobalValue.m_kUserInfo.GetCurrentCharType()))
		{
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK,
				g_pkStrLoader->GetString(STR_ID_NO_CLEAR_PRIVATE_DUNGEON),
				L"",
				KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
			return;
		}

		g_pkUIScene->m_pkNateOn->OnViewWaitImage();

		KP2P::GetInstance()->Send_UserServerRoomReq(SiKGCCoupleSystem()->GetUID_of_my_honey());
		break;
	}
}

void GCNateOn::OnClose()
{
	if (m_pkWaitImage != NULL)
		m_pkWaitImage->ToggleRender(false);
	//m_pkTalkList->DeleteAllTalkList();

#if defined( USE_AGIT_SYSTEM )
	g_pkAgitOverlayUI->ToggleRenderBrownieBtn(true);
#endif

	m_pkChat->FocusClose();

	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
}

void GCNateOn::SetNickName()
{
	std::wstring strNickName = L"#c";
	strNickName += g_kGlobalValue.m_kUserInfo.strNickColor;
	strNickName += g_kGlobalValue.m_kUserInfo.strNickName;
	strNickName += L"#cx";
	strNickName += L" - ";
	strNickName += g_pkStrLoader->GetString(STR_ID_ONLINE);
	m_pkNickName->SetText(strNickName);
	m_pkNickName->SetAlign(DT_VCENTER);
}

void GCNateOn::SetTooltips()
{
	m_pkSendMailBtn->SetToolTip(g_pkStrLoader->GetString(STR_ID_SEND_MAIL_BTN));
	m_pkInboxBtn->SetToolTip(g_pkStrLoader->GetString(STR_ID_INBOX_BTN));
	m_pkDraftsBtn->SetToolTip(g_pkStrLoader->GetString(STR_ID_DRAFTS_BTN));
	m_pkAddBuddyBtn->SetToolTip(g_pkStrLoader->GetString(STR_ID_NATEON_ADD_BUDDY));
	m_pkBlockBuddyBtn->SetToolTip(g_pkStrLoader->GetString(STR_ID_NATEON_BLOCK_BUDDY));
	m_pkAddGroupBtn->SetToolTip(g_pkStrLoader->GetString(STR_ID_NATEON_ADD_GROUP));
	m_pkChangeEmoticon->SetToolTip(g_pkStrLoader->GetString(STR_ID_NATEON_CHANGE_EMOTICON));
	m_pkRecommend->SetToolTip(g_pkStrLoader->GetString(STR_ID_RECOMMEND_LIST_TOLL_TIP));
}

void GCNateOn::SetOnlineCount()
{
	m_pkOnline->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NATEON_ONLINE_BUDDY_COUNT, "i"
		, SiKFriendShip()->GetDataUserCount(GCNateOn::FRIEND_MODE, true)));

#if defined( GUILD_SYSTEM )
	m_pkGuildOnline->SetText(g_pkStrLoader->GetReplacedString(STR_ID_GUILD_MESSENGER_ONLINE_MSG, "iii", SiKFriendShip()->GetOnlineGuildMember(),
		SiKFriendShip()->GetOnlineGuildMember(), g_kGlobalValue.GetGuildMemberTotalCount()));
#endif
	m_pkOnline->SetAlign(DT_VCENTER);
}

void GCNateOn::SetNateonMode(E_NATEON_MODE eMode)
{
	if (eMode <= GCNateOn::INIT_MODE || eMode >= NUM_MODE)
	{
		assert(L"invalid mode");
		return;
	}

	if (eMode == GCNateOn::GUILD_MODE && (g_kGlobalValue.m_kGuildInfo.m_dwUID == 0 || g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_JOINER))
		eMode = GCNateOn::FRIEND_MODE;

	m_pkBuddyListBtn->ToggleRender(eMode != GCNateOn::FRIEND_MODE);
#if !defined( GUILD_SYSTEM )
	m_pkGuildListBtn->ToggleRender(false);
#else
	//m_pkGuildListBtn->ToggleRender( eMode == GCNateOn::GUILD_MODE );
	m_pkAddBuddyBtn->ToggleRender(eMode != GCNateOn::GUILD_MODE);
	m_pkAddGroupBtn->ToggleRender(eMode != GCNateOn::GUILD_MODE);
	m_pkBlockBuddyBtn->ToggleRender(eMode != GCNateOn::GUILD_MODE);
	m_pkOnline->ToggleRender(eMode != GCNateOn::GUILD_MODE);

	if (eMode == GCNateOn::GUILD_MODE)
	{
		m_pkGuildOfflineCheckBox->ToggleRender(true);
		m_pkGuildOfflineCheckSign->ToggleRender(m_pkTree->GetGuildOfflineMember());
		m_pkGuildBlockCheckBox->ToggleRender(true);
		m_pkGuildBlockCheckSign->ToggleRender(m_pkTree->GetGuildBlockMember());
		m_pkGuildName->ToggleRender(true);
		m_pkGuildOnline->ToggleRender(true);
		m_pkGuildOfflineCheck->ToggleRender(true);
		m_pkGuildBlockCheck->ToggleRender(true);
		m_pkRecommend->ToggleRender(false);
		SiKGCIconImageManager()->LoadIconImageFromHTTP(g_kGlobalValue.m_kGuildInfo.m_strFileName);
		m_pkTexGuildMark->SetTextureName(g_kGlobalValue.m_kGuildInfo.m_strFileName);
		m_pkTexGuildMark->ToggleRender(true);
	}
	else
	{
		//길드 메신저 추가 ui들
		m_pkGuildOfflineCheckBox->ToggleRender(false);
		m_pkGuildOfflineCheckSign->ToggleRender(false);
		m_pkGuildBlockCheckBox->ToggleRender(false);
		m_pkGuildBlockCheckSign->ToggleRender(false);
		m_pkGuildName->ToggleRender(false);
		m_pkGuildOnline->ToggleRender(false);
		m_pkGuildOfflineCheck->ToggleRender(false);
		m_pkGuildBlockCheck->ToggleRender(false);
#ifdef WELCOMEBACK_FRIEND
		m_pkRecommend->ToggleRender(g_kGlobalValue.m_bIsRecommendEvent && Result_RecomEnable != 1);
#endif
		m_pkTexGuildMark->ToggleRender(false);
	}
#endif

	m_pkBlockListBtn->ToggleRender(eMode != GCNateOn::BLOCK_MODE);
	m_pkModeBuddyList->ToggleRender(eMode == GCNateOn::FRIEND_MODE);
	m_pkModeGuildList->ToggleRender(eMode == GCNateOn::GUILD_MODE);
	m_pkModeBlockList->ToggleRender(eMode == GCNateOn::BLOCK_MODE);

	switch (eMode)
	{
	case GCNateOn::FRIEND_MODE:
	case GCNateOn::GUILD_MODE:
	case GCNateOn::BLOCK_MODE:
	{
		GetTalkListWindow()->InitSelectState();
		GetTalkListWindow()->SetCurTalkKey(std::make_pair(0, 0));
		GetNateOnChatWindow()->SetCurTalkKey(std::make_pair(0, 0));
		m_pkChat->ToggleRender(false);
		// milennium9 test code
		if (false == SiKGCCoupleSystem()->Am_I_Solo() && eMode == GCNateOn::FRIEND_MODE)
		{
			m_pkTree->SetChildSize(MAX_CHILD_NUM - 1, false);

			KBuddy kFriend;
			FRIEND_UI_DATA kFriendUIData;
			if (SiKFriendShip()->GetFriendBuddyByUID(SiKGCCoupleSystem()->GetUID_of_my_honey(), kFriend))
			{
				kFriend.GetUIData(kFriendUIData);
				m_pkSpecialFriend->ToggleRender(true);
				m_pkSpecialFriend->SetData(kFriendUIData, false);
				m_pkCouplePopup->ClearMenu();
				m_pkCouplePopup->AddMenu(1, g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU3));
				m_pkCouplePopup->AddMenu(2, g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM2));
				if (kFriendUIData.m_bOnline)
					m_pkCouplePopup->AddMenu(3, g_pkStrLoader->GetString(STR_ID_FOLLOW_FRIEND_7));
			}
		}
		else
		{
			m_pkSpecialFriend->ToggleRender(false);
			m_pkTree->SetChildSize(MAX_CHILD_NUM);
		}
		SetTreeMode(eMode);
	}
	break;
	case GCNateOn::CHAT_MODE:
		m_pkTree->ToggleRender(false);
		m_pkChat->ToggleRender(true);
		m_pkSpecialFriend->ToggleRender(false);
		break;
	default:
	{
	}
	break;
	}
	m_eMode = eMode;
}

void GCNateOn::SetTreeMode(E_NATEON_MODE eMode)
{
	m_mapTopIndex[m_eMode] = m_pkTree->GetTopIndex();

	m_pkTree->InitState(true, true);
	m_pkTree->SetMode(m_mapTopIndex[eMode], eMode);
	m_pkTree->UpdateData();
}

void GCNateOn::OnCreateComplete()
{
	UpdateTreeData();
	SetNew(false);
	GetTalkListWindow()->RefreshTalkList();
}

void GCNateOn::OnBuddyList()
{
	SetNateonMode(GCNateOn::FRIEND_MODE);
}

void GCNateOn::OnGuildList()
{
	SetNateonMode(GCNateOn::GUILD_MODE);
}

void GCNateOn::OnBlockList()
{
	SetNateonMode(GCNateOn::BLOCK_MODE);
}

void GCNateOn::OnAddGroup()
{
	if (SiKFriendShip()->GetFriendGroupCount() > MAX_GROUP_NUM)
	{
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NATEON_ADD_GROUP_ERROR2), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
		return;
	}

	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NATEON_INPUT_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NATEON_ADD_GROUP, -1, 0, false, true);
}

void GCNateOn::OnDestroyComplete()
{
	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_DESTROYED));
}

void GCNateOn::UpdateTreeData()
{
	if (false == m_pkTree->IsCreate())
		return;

	m_pkTree->UpdateData();
}

void GCNateOn::OnTreeRButtonClick(const FRIEND_UI_DATA& kData)
{
	if (kData.m_bUser == false && kData.m_bGroup == false)
	{
		return;
	}

	switch (m_eMode)
	{
	case GCNateOn::CHAT_MODE:
	{
		//return;
		break;
	}
	case GCNateOn::FRIEND_MODE:
	{
		if (kData.m_bGroup == true && kData.m_nGroupID == SiKFriendShip()->DEFGROUPID)
		{
			return;
		}
		break;
	}
	case GCNateOn::BLOCK_MODE:
	case GCNateOn::GUILD_MODE:
	{
		if (kData.m_bGroup == true)
		{
			return;
		}
		break;
	}
	default:
	{
		assert(!"invalid mode");
		return;
	}
	}

	m_pkMultiMenu->SetMode(m_eMode, kData);

	POINT pt = g_pkInput->GetMousePos();
	pt.x = (LONG)(pt.x / m_fWindowScaleX);
	pt.y = (LONG)(pt.y / m_fWindowScaleY);

	D3DXVECTOR2 vMousePos((float)pt.x, (float)pt.y);
	D3DXVECTOR2 vNateOnPos = GetCurrentWindowPos();
	vMousePos -= vNateOnPos;
	if (vMousePos.x + m_pkMultiMenu->GetWidth() > GC_SCREEN_WIDTH)
	{
		vMousePos.x = float(GC_SCREEN_WIDTH - m_pkMultiMenu->GetWidth());
	}
	vMousePos.x -= 10.0f;
	vMousePos.y -= 10.0f;
	m_pkMultiMenu->SetWindowPosDirect(vMousePos);
	m_pkMultiMenu->AdjustChildPosition();
	m_pkMultiMenu->ToggleRender(true);
}

void GCNateOn::FrameMoveInEnabledState()
{
	POINT pt = g_pkInput->GetMousePos();

	D3DXVECTOR2 vMousePos((float)pt.x, (float)pt.y);

	if (m_pkCouplePopup->CheckPosInWindowBoundWithChild(vMousePos, true) == false)
	{
		m_pkCouplePopup->ToggleRender(false);
	}

	CheckTalkList();

	//길드메신저
	if (m_pkGuildOfflineCheckBox->CheckPosInWindowBound(vMousePos) && g_pkInput->BtnUp(KInput::MBLEFT) && m_eMode == GCNateOn::GUILD_MODE)
	{
		if (m_pkTree->GetGuildOfflineMember())
		{
			SetGuildOfflineCheckBox(false);
		}
		else
		{
			SetGuildOfflineCheckBox(true);
		}
	}

	if (m_pkGuildBlockCheckBox->CheckPosInWindowBound(vMousePos) && g_pkInput->BtnUp(KInput::MBLEFT) && m_eMode == GCNateOn::GUILD_MODE)
	{
		if (m_pkTree->GetGuildBlockMember())
		{
			SetGuildBlockCheckBox(false);
		}
		else
		{
			SetGuildBlockCheckBox(true);
		}
	}
}

void GCNateOn::OnDestroy()
{
	m_pkMultiMenu->ToggleRender(false);
}

bool GCNateOn::IsPopupOn() const
{
	return m_pkMultiMenu->IsRenderOn();
}

void GCNateOn::OnBlockBuddy()
{
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NATEON_INPUT_BOX, L"", L"", KGCUIScene::GC_MBOX_USE_NATEON_BLOCK_BUDDY, -1, 0, false, true);
}

void GCNateOn::PostChildDraw()
{
	if (m_pkWaitImage)
	{
		if (m_pkWaitImage->IsRenderOn())
			return;
	}
	DWORD dwEmoticon = SiKFriendShip()->GetEmoticon();
	m_pkEmoticon->ToggleRender(true);
	m_pkEmoticonBG->ToggleRender(true);

	if (0 == dwEmoticon)
		return;

	KGCEmoticon* pEmoticon = SiKGCEmoticonManager()->GetEmoticonData(dwEmoticon);
	JIF(pEmoticon != NULL);
	D3DXVECTOR2 vPos = m_pkEmoticon->GetRelocatedWindowPos();
	pEmoticon->RenderForChatbox(vPos.x, vPos.y, vPos.x + m_pkEmoticon->GetWidth() * m_fWindowScaleX, vPos.y + m_pkEmoticon->GetHeight() * m_fWindowScaleY);
	m_pkEmoticon->ToggleRender(false);
	m_pkEmoticonBG->ToggleRender(false);
}

void GCNateOn::OnChangeEmoticon()
{
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EMOTICON_SELECT_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NATEON_SELECT_EMOTICON, 0, 0, false, true);
}

void GCNateOn::OnAddBuddy()
{
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NATEON_ADD_BUDDY, L"", L"", KGCUIScene::GC_MBOX_USE_NATEON_ADD_BUDDY, 0, 0, false, true);
}

void GCNateOn::OnSelectTalkBox()
{
	m_pkChat->SetCurTalkKey(m_pkTalkList->GetCurTalkKey());
	m_pkChat->RefreshTalkBox(m_pkTalkList->GetCurTalkKey());
	m_pkChat->RefreshTalkerList(m_pkTalkList->GetCurTalkKey());

	m_pkChat->EnableEditor(false);
	NATE_TALK_BOX* pTalkBox = m_pkChat->GetTalkBox(m_pkTalkList->GetCurTalkKey());
	if (pTalkBox)
	{
		if (2 <= pTalkBox->GetTalkerCount())
			m_pkChat->EnableEditor(true);
	}

	SetNateonMode(GCNateOn::CHAT_MODE);
}

void GCNateOn::CheckTalkList(void)
{
	m_pkTalkList->ToggleRender((m_pkTalkList->GetTalkListCount() > 0));
}

void GCNateOn::DeleteAllTalkList()
{
	m_pkTalkList->DeleteAllTalkList();
	m_pkTalkList->ToggleRender(false);
}

void GCNateOn::DeleteTalkBox()
{
	m_pkChat->ClearChatWindow();
	m_pkChat->ToggleRender(false);
}

void GCNateOn::OnRecommend()
{
	g_pkUIScene->m_pkNateOn->SetNew(false);
	Result_recommendFullINfo = INT_MAX;
	KP2P::GetInstance()->Send_RecommendFullInfoReq();
	g_MyD3D->WaitForServerAck(Result_recommendFullINfo, INT_MAX, 3000, 2);

	if (!SiKFriendShip()->m_vecWaitRecommenders.empty())
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_RECOMMEND, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
	else {
#if defined( WELCOMEBACK_FRIEND_NEW )
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_WELCOMEBACK_DLG_NEW, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
#else
		g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_WELCOMEBACK_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
#endif
	}
}

void GCNateOn::SetGuildOfflineCheckBox(bool bCheck_)
{
	m_pkGuildOfflineCheckSign->ToggleRender(bCheck_);
	m_pkTree->SetGuildOfflineMember(bCheck_);
	m_pkTree->UpdateData();
}

void GCNateOn::SetGuildBlockCheckBox(bool bCheck_)
{
	m_pkGuildBlockCheckSign->ToggleRender(bCheck_);
	m_pkTree->SetGuildBlockMember(bCheck_);
	m_pkTree->UpdateData();
}

bool GCNateOn::InitDialog(IN const KDialogInfo& kInfo_)
{
	SiKFriendShip()->UpdateFriendShipData();
	this->UpdateTreeData();
	return true;
}