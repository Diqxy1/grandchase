#include "StdAfx.h"

#include "GCCheckBox.h"
#include "../uifx/D3DView.h"
#include "GCCoupleWnd.h"
#include "GCCoupleCharSelect.h"
#include "GCCoupleSystem.h"
#include "GCItemManager.h"
#include "GCCoupleMark.h"
#include "GCSquareSayBalloon.h"
#include "GCChatManager.h"
#include ".\BuddySystem\KFriendShip.h"
#include "GCRenderManager.h"
#include "GCObjectAndMotion.h"
#include "GCCharacterSelectBox.h"

IMPLEMENT_CLASSNAME(KGCCoupleWnd);
IMPLEMENT_WND_FACTORY(KGCCoupleWnd);
IMPLEMENT_WND_FACTORY_NAME(KGCCoupleWnd, "gc_couple_window");

KGCCoupleWnd::KGCCoupleWnd(void)
	: m_bMyCouple(true)
	, m_iSystemMsgAlpha(0xff)
	, m_iSystemMsgViewFrm(0)
{
	m_pkRingSlot = NULL;
	m_pkMotionText = NULL;
	m_pkCoupleView = NULL;
	LINK_CONTROL("ring_slot", m_pkRingSlot);
	LINK_CONTROL("static", m_pkMotionText);
	LINK_CONTROL("char_view", m_pkCoupleView);

	char strTemp[MAX_PATH];
	for (int i = 0; i < GCCW_CHAR_NUM; ++i)
	{
		m_pkNickName[i] = NULL;
		m_pkCharSelect[i] = NULL;

		sprintf(strTemp, "nickname%d", i);
		LINK_CONTROL(strTemp, m_pkNickName[i]);
	}

	for (int i = 0; i < GCCW_SLOT_NUM; ++i)
	{
		m_pkSlot[i] = NULL;
		sprintf(strTemp, "couplelook_slot%d", i);
		LINK_CONTROL(strTemp, m_pkSlot[i]);
	}

	for (int i = 0; i < GCCW_MOTION_NUM; ++i)
	{
		m_pkMotion[i] = NULL;
		sprintf(strTemp, "motion%d", i);
		LINK_CONTROL(strTemp, m_pkMotion[i]);
	}

	m_pkChatBg = NULL;
	m_pkChatUngerBg = NULL;
	m_pkChatEnter = NULL;

	m_pkChatListOnOff = NULL;
	m_pkChatListBg = NULL;
	m_pkChatList = NULL;

	m_pkChatEdit = NULL;
	m_pkSayBalloon = NULL;
	m_pkCoupleChatStatic = NULL;
	m_pkSystemMessage = NULL;
	m_pkSystemMsgBg = NULL;
	m_pkChatListClose = NULL;

	LINK_CONTROL("chat_bg", m_pkChatBg);
	LINK_CONTROL("chat_under_bg", m_pkChatUngerBg);
	LINK_CONTROL("chat_enter", m_pkChatEnter);
	LINK_CONTROL("edit_chat", m_pkChatEdit);
	LINK_CONTROL("chat_list_enable_checkbox", m_pkChatListOnOff);
	LINK_CONTROL("chat_list_bg", m_pkChatListBg);
	LINK_CONTROL("chat_list", m_pkChatList);
	LINK_CONTROL("sayballoon", m_pkSayBalloon);
	LINK_CONTROL("static_couplechatview", m_pkCoupleChatStatic);
	LINK_CONTROL("static_systemmessage", m_pkSystemMessage);
	LINK_CONTROL("systemmessage_bg", m_pkSystemMsgBg);
	LINK_CONTROL("chat_list_close", m_pkChatListClose);

	m_pkMark[GCCW_LEFT_CHAR] = NULL;
	m_pkMark[GCCW_RIGHT_CHAR] = NULL;

	LINK_CONTROL("couplemark_left", m_pkMark[GCCW_LEFT_CHAR]);
	LINK_CONTROL("couplemark_right", m_pkMark[GCCW_RIGHT_CHAR]);
}

KGCCoupleWnd::~KGCCoupleWnd(void)
{
}

void KGCCoupleWnd::OnCreate(void)
{
	m_pkMotionText->SetText(g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM5));

	m_pkSystemMessage->SetAlign(DT_CENTER);
#if defined( _DEBUG )
	KLuaManager luaMgr;
	GCFUNC::LoadLuaScript(luaMgr, "Dev.lua");
#endif

	m_kRingImage.SetScale(0.24f, 0.24f);
	m_kRingImage.SetPosition((m_pkRingSlot->GetFixedWindowPos().x + 3.0f) / (400.0f * GC_SCREEN_DIV_WIDTH), -(m_pkRingSlot->GetFixedWindowPos().y + 45.0f) / (300.0f * GC_SCREEN_DIV_WIDTH) * 0.75f + 1.1f);
	m_kRingImage.CameraMatrixOn();

	m_fOutlineWidthOrigin = g_RenderManager->GetOutlineWidth();
	g_RenderManager->SetOutlineWidth(OUT_LINE_WIDTH_IN_CHANNEL);

	InitCoupleView();
	Update();

	SiKGCCoupleSystem()->SendCharacterChange(g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType);
}

void KGCCoupleWnd::OnDestroy(void)
{
	m_pkMark[GCCW_LEFT_CHAR]->DeleteParticle();
	m_pkMark[GCCW_RIGHT_CHAR]->DeleteParticle();

	g_RenderManager->SetOutlineWidth(m_fOutlineWidthOrigin);
}

void KGCCoupleWnd::FrameMoveInEnabledState(void)
{
	if (m_pkChatUngerBg->IsRenderOn())
	{
		::SetFocus(m_pkChatEdit->GetHandle());
		m_pkSayBalloon->TimeCheck();

		if (SiKGCCoupleSystem()->IsCoupleChatExist())
		{
			AddChat(false, !SiKGCCoupleSystem()->GetPosition(true), SiKGCCoupleSystem()->GetCoupleChat());
		}

		FrameMove_Marks();
	}

#if defined( _DEBUG )
	KLuaManager luaMgr;
	GCFUNC::LoadLuaScript(luaMgr, "Dev.lua");

	float px, py, pz;
	LUA_GET_VALUE_DEF("Px", px, 1.0f);
	LUA_GET_VALUE_DEF("Py", py, 1.0f);
	LUA_GET_VALUE_DEF("Pz", pz, 1.0f);
	float sx, sy, sz;
	LUA_GET_VALUE_DEF("sx", sx, 1.0f);
	LUA_GET_VALUE_DEF("sy", sy, 1.0f);
	LUA_GET_VALUE_DEF("sz", sz, 1.0f);
	float rx, ry, rz;
	LUA_GET_VALUE_DEF("rx", rx, 1.0f);
	LUA_GET_VALUE_DEF("ry", ry, 1.0f);
	LUA_GET_VALUE_DEF("rz", rz, 1.0f);

	m_pkCoupleView->SetObjectPosition("Object", D3DXVECTOR3(px, py, pz));
	m_pkCoupleView->SetObjectScale("Object", D3DXVECTOR3(sx, sy, sz));
	m_pkCoupleView->SetObjectRotation("Object", D3DXVECTOR3(rx, ry, rz));
#endif

	if (m_pkSystemMessage->IsRenderOn())
	{
		m_pkSystemMessage->ForceSetWindowColor(D3DCOLOR_ARGB(m_iSystemMsgAlpha, 0xff, 0xff, 0xff));
		m_pkSystemMsgBg->ForceSetWindowColor(D3DCOLOR_ARGB(m_iSystemMsgAlpha, 0xff, 0xff, 0xff));

		if (m_iSystemMsgViewFrm > 0)
			--m_iSystemMsgViewFrm;
		else
			m_iSystemMsgAlpha -= 3;

		if (m_iSystemMsgAlpha < 3)
		{
			m_pkSystemMessage->ToggleRender(false);
			m_pkSystemMsgBg->ToggleRender(false);
		}
	}
}

void KGCCoupleWnd::FrameMove_Marks()
{
	int iMyPosition = SiKGCCoupleSystem()->GetPosition(true) ? GCCW_RIGHT_CHAR : GCCW_LEFT_CHAR;
	int iPartnerPosition = !SiKGCCoupleSystem()->GetPosition(true) ? GCCW_RIGHT_CHAR : GCCW_LEFT_CHAR;

	if (m_pkMark[iMyPosition]->GetCurrentMark() == KGCCoupleMark::GCCM_MARK_NUM)
		m_pkMark[iMyPosition]->ForceSetHeart(KGCCoupleMark::GCCM_MARK_YELLOW);

	if (m_pkMark[iPartnerPosition]->GetCurrentMark() == KGCCoupleMark::GCCM_MARK_NUM &&
		SiKGCCoupleSystem()->GetPartnerState() == KGCCoupleSystem::GCCS_COUPLE_WND)
	{
		m_pkMark[iPartnerPosition]->ForceSetHeart(KGCCoupleMark::GCCM_MARK_YELLOW);
		m_pkMark[iMyPosition]->ForceShowParticle(KGCCoupleMark::GCCM_MINI_SIZE);
		m_pkMark[iPartnerPosition]->ForceShowParticle(KGCCoupleMark::GCCM_MINI_SIZE);
		m_pkMark[iPartnerPosition]->ClearToolTip();
	}
	else if (m_pkMark[iPartnerPosition]->GetCurrentMark() == KGCCoupleMark::GCCM_MARK_YELLOW &&
		SiKGCCoupleSystem()->GetPartnerState() != KGCCoupleSystem::GCCS_COUPLE_WND)
	{
		m_pkMark[iPartnerPosition]->ForceSetHeart(KGCCoupleMark::GCCM_MARK_NUM);
	}

	while (m_pkMark[iPartnerPosition]->GetCurrentMark() == KGCCoupleMark::GCCM_MARK_NUM)
	{
		FRIEND_UI_DATA kData;
		if (!SiKFriendShip()->FindUserInfo(SiKGCCoupleSystem()->GetUID_of_my_honey(), kData))
		{
			m_pkMark[iPartnerPosition]->SetToolTip(g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM36));
			break;
		}

		if (kData.m_bOnline)
		{
			KBuddy buddy;
			if (SiKFriendShip()->GetFriendBuddyByUID(kData.m_dwUserUID, buddy))
			{
				std::wstring strLocation = buddy.GetServerName();
				strLocation += L"/";
				strLocation += buddy.GetLocation();
				m_pkMark[iPartnerPosition]->SetToolTip(strLocation);
			}
			else
				m_pkMark[iPartnerPosition]->SetToolTip(g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM37));
		}
		else
			m_pkMark[iPartnerPosition]->SetToolTip(g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM36));

		break;
	}
}

void KGCCoupleWnd::ActionPerformed(const KActionEvent& event)
{
	for (int i = 0; i < GCCW_MOTION_NUM; ++i)
	{
		if (event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK)
			break;

		if (event.m_pWnd != m_pkMotion[i])
			continue;

		OnClickMotionBtn(i);
		break;
	}

	for (int i = 0; i < GCCW_SLOT_NUM; ++i)
	{
		if (event.m_dwCode != KD3DWnd::D3DWE_BUTTON_CLICK)
			break;

		if (event.m_pWnd != m_pkSlot[i])
			continue;

		OnClickSlot(i);
		break;
	}

	for (int i = 0; i < GCCW_CHAR_NUM; ++i)
	{
		if (event.m_dwCode != KGCCoupleCharSelect::GCCCS_CHAR_BTN_CLICK)
			break;

		SiKGCCoupleSystem()->SendCharacterChange(static_cast<DWORD>(event.m_dwlParam));
	}

	GCWND_MSG_MAP(m_pkChatListOnOff, KD3DWnd::D3DWE_BUTTON_CLICK, ToggleChatList);
	GCWND_MSG_MAP(m_pkChatListClose, KD3DWnd::D3DWE_BUTTON_CLICK, ToggleChatList);
	GCWND_CODE_MAP(m_pkChatEdit, OnEdit);
}

void KGCCoupleWnd::OnEdit(DWORD dwCode)
{
	UNREFERENCED_PARAMETER(dwCode);

#if defined( NATION_USA ) || defined( NATION_BRAZIL ) || defined( NATION_EU )
	std::wstring strText = m_pkChatEdit->GetText();
	if (SiKSlangFilter()->CheckUnicode(strText) == true)
	{
		//g_pkChatManager->AddSystemMsg( -1, g_pkStrLoader->GetString(STR_ID_NOT_AVAILABLE_LANGUAGE) , KGCChatManager::CHAT_TYPE_ALERT );
		m_pkChatEdit->SetText(L"");
		g_KDSound.Play("72");
		return;
	}
#endif

	if (0 == ::wcslen(m_pkChatEdit->GetText())) return;
	if (dwCode == D3DWE_EDIT_ESCAPE)
	{
		m_pkChatEdit->SetText(L"");
		g_KDSound.Play("72");
		return;
	}

	if (dwCode != D3DWE_EDIT_RETURN)
		return;

	std::wstringstream strmChatMsg;
	strmChatMsg << m_pkChatEdit->GetText();

#if defined( ENABLE_TEAM_CHAT )
	WCHAR Input_Text[MAX_CHAT_STR_BUF_SIZE] = { 0, };

	StringCchCopyW(Input_Text, MAX_CHAT_STR_BUF_SIZE, m_pkChatEdit->GetText());
	if (L'%' != Input_Text[0])
	{
#endif
		KP2P::GetInstance()->Send_CoupleChatReq(strmChatMsg.str(), 0xffffffff);
		AddChat(true, SiKGCCoupleSystem()->GetPosition(true), strmChatMsg.str());
#if defined( ENABLE_TEAM_CHAT )
	}
	else
	{
		AddSystemMessage(g_pkStrLoader->GetString(STR_ID_NOT_IN_TEAM));
	}
#endif
	m_pkChatEdit->SetText(L"");

	// 소리 시끄럽다고 꺼달래요!
	g_KDSound.Play("72");
	m_pkChatEdit->ToggleRender(true);//원인은 모르지만 가끔 리드로우 안되는 부분이 있어 글짜 쓸때 강제로 한번 더 드로우 해줌
}

void KGCCoupleWnd::ToggleChatList()
{
	m_pkChatListOnOff->ToggleCheck();
	m_pkChatListBg->ToggleRender(m_pkChatListOnOff->GetCheck());
	m_pkChatList->ToggleRender(m_pkChatListOnOff->GetCheck());
	m_pkCoupleChatStatic->SetText(m_pkChatListOnOff->GetCheck() ? g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM34) : g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM33));
	m_pkChatListClose->ToggleRender(m_pkChatListOnOff->GetCheck());
	m_pkSayBalloon->ToggleRender(!m_pkChatListOnOff->GetCheck());
}

void KGCCoupleWnd::AddMotionToCouple(int iMotionID, bool bOnce, bool bClear)
{
	if (bClear)
	{
		m_pkCoupleView->ClearMotionFromCharacter("Left");
		m_pkCoupleView->ClearMotionFromCharacter("Right");
	}

	m_pkCoupleView->AddMotionToCharacter(
		"Left",
		GetMotionID(iMotionID
			, SiKGCCoupleSystem()->GetCoupleCharacter(m_bMyCouple, SiKGCCoupleSystem()->GetPosition(m_bMyCouple))
			, SiKGCCoupleSystem()->GetCoupleCharacter(m_bMyCouple, !SiKGCCoupleSystem()->GetPosition(m_bMyCouple))
			, false)
		, bOnce
	);
	m_pkCoupleView->AddMotionToCharacter(
		"Right",
		GetMotionID(iMotionID
			, SiKGCCoupleSystem()->GetCoupleCharacter(m_bMyCouple, SiKGCCoupleSystem()->GetPosition(m_bMyCouple))
			, SiKGCCoupleSystem()->GetCoupleCharacter(m_bMyCouple, !SiKGCCoupleSystem()->GetPosition(m_bMyCouple))
			, true)
		, bOnce
	);
}

void KGCCoupleWnd::OnClickMotionBtn(int i)
{
	int iMotionID = i + 1;
	AddMotionToCouple(iMotionID, true, false);
}

void KGCCoupleWnd::SetDefaultMotion()
{
	AddMotionToCouple(0, false, true);
}

void KGCCoupleWnd::OnClickSlot(int i)
{
	SiKGCCoupleSystem()->SendEquipItemChange(m_mapSlotInfo[i]);
}

void KGCCoupleWnd::PostChildDraw(void)
{
	for (int i = 0; i < GCCW_SLOT_NUM; ++i)
	{
		m_akItemImage[i].Render();
	}
	m_kRingImage.Render();
}

void KGCCoupleWnd::InitCoupleView()
{
	m_pkCoupleView->InitState(true, true);

	m_pkCoupleView->ClearObject();
	m_pkCoupleView->ClearParticle();

	KGCObjectAndMotion* obj = m_pkCoupleView->AddCharacter(
		SiKGCCoupleSystem()->GetPosition(m_bMyCouple) ? "Right" : "Left",
		SiKGCCoupleSystem()->GetEquipItem(SiKGCCoupleSystem()->GetPosition(m_bMyCouple), m_bMyCouple),
		SiKGCCoupleSystem()->GetCoupleCharacter(m_bMyCouple, false),
		0,
		false
	);

	obj->SetEnable_FirstFrameForPosition(true);

	obj = m_pkCoupleView->AddCharacter(
		!SiKGCCoupleSystem()->GetPosition(m_bMyCouple) ? "Right" : "Left",
		SiKGCCoupleSystem()->GetEquipItem(!SiKGCCoupleSystem()->GetPosition(m_bMyCouple), m_bMyCouple),
		SiKGCCoupleSystem()->GetCoupleCharacter(m_bMyCouple, true),
		0,
		false
	);
	SetDefaultMotion();

	obj->SetEnable_FirstFrameForPosition(true);

#if defined( _DEBUG )
	KLuaManager luaMgr;
	GCFUNC::LoadLuaScript(luaMgr, "Dev.lua");

	float px, py, pz;
	LUA_GET_VALUE_DEF("Px", px, 1.0f);
	LUA_GET_VALUE_DEF("Py", py, 1.0f);
	LUA_GET_VALUE_DEF("Pz", pz, 1.0f);
#endif
	//m_pkCoupleView->AddObject( "Object", "couple_Room_Tree.p3m", "couple_Room_Tree.dds" );
	//m_pkCoupleView->AddParticle( "haha", "Portal_Zone03", D3DXVECTOR3( px, py, pz ) );
	m_pkCoupleView->SetBackground("CoupleRoom001.p3m", "CoupleRoom001.dds");
}

void KGCCoupleWnd::Update()
{
	InitCoupleView();
	SetDefaultMotion();
	UpdateEquipSlot();
}

const char* KGCCoupleWnd::GetCharSex(int iCharType)
{
	switch (iCharType)
	{
	case GC_CHAR_ELESIS:
	case GC_CHAR_LIRE:
	case GC_CHAR_ARME:
	case GC_CHAR_AMY:
	case GC_CHAR_MARI:
	case GC_CHAR_LEY:
	case GC_CHAR_RIN:
	case GC_CHAR_LIME:
	case GC_CHAR_EDEL:
#ifdef ALICE 
	case GC_CHAR_ALICE:
#endif
#ifdef DECANEE 
	case GC_CHAR_DECANEE:
#endif
		return "f";
	case GC_CHAR_LAS:
	case GC_CHAR_RYAN:
	case GC_CHAR_RONAN:
	case GC_CHAR_JIN:
	case GC_CHAR_SIEG:
	case GC_CHAR_DIO:
	case GC_CHAR_ZERO:
	case GC_CHAR_LUPUS:
	case GC_CHAR_ASIN:
	case GC_CHAR_BEIGAS:
	case GC_CHAR_UNO:
#ifdef PERYTON // Kawan>
	case GC_CHAR_PERYTON:
#endif
		return "m";
	}
	return "f";
}

std::string KGCCoupleWnd::GetMotionID(int iMotionNum, int iCharType, int iPartnerCharType, bool bRightMyChar)
{
	std::stringstream stm;
	stm << "Couple" << iMotionNum << "_"
		<< GetCharSex(iCharType)
		<< GetCharSex(iPartnerCharType)
		<< (bRightMyChar ? "r" : "l")
		<< ".frm";
	return stm.str();
}

void KGCCoupleWnd::UpdateEquipSlot()
{
	KUserItemVector vecEquip = SiKGCCoupleSystem()->GetMyEquipItem();

	for (int i = 0; i < GCCW_SLOT_NUM; ++i)
	{
		m_akItemImage[i].RemoveAllTexture();
	}

	m_mapSlotInfo.clear();
	for (int i = 0; i < (int)vecEquip.size(); ++i)
	{
		if (g_pItemMgr->IsBaseItem(vecEquip[i].iItemID) == true)
			continue;

		GCItem* pItem = g_pItemMgr->GetItemData(vecEquip[i].iItemID);

		GCDeviceTexture* pTexture = g_pItemMgr->CreateItemTexture(vecEquip[i].iItemID, 0);

		if (pItem->dwSlotPosition & ESP_HELMET)
		{
			m_akItemImage[GCCW_SLOT_HELMET].AddTexture(pTexture);
			m_mapSlotInfo[GCCW_SLOT_HELMET] = vecEquip[i].iItemID;
		}
		else if (pItem->dwSlotPosition & ESP_JACKET)
		{
			m_akItemImage[GCCW_SLOT_SUIT].AddTexture(pTexture);
			m_mapSlotInfo[GCCW_SLOT_SUIT] = vecEquip[i].iItemID;
		}
		else if (pItem->dwSlotPosition & ESP_PANTS)
		{
			m_akItemImage[GCCW_SLOT_PANTS].AddTexture(pTexture);
			m_mapSlotInfo[GCCW_SLOT_PANTS] = vecEquip[i].iItemID;
		}
		else if (pItem->dwSlotPosition & ESP_SHOES)
		{
			m_akItemImage[GCCW_SLOT_SHOES].AddTexture(pTexture);
			m_mapSlotInfo[GCCW_SLOT_SHOES] = vecEquip[i].iItemID;
		}
		else if (pItem->dwSlotPosition & ESP_SLOT_ITEM)
		{
			m_akItemImage[GCCW_SLOT_ITEM].AddTexture(pTexture);
			m_mapSlotInfo[GCCW_SLOT_ITEM] = vecEquip[i].iItemID;
		}
		else if (!(pItem->dwSlotPosition & ESP_A_FINGER))
		{
			m_akItemImage[GCCW_SLOT_BG].AddTexture(pTexture);
			m_mapSlotInfo[GCCW_SLOT_BG] = vecEquip[i].iItemID;
		}

		AddItemParts(vecEquip[i].iItemID);
	}
}

void KGCCoupleWnd::AddItemParts(int iItemID)
{
	std::pair<bool, SCoupleItemInfo> pairCoupleItem = SiKGCCoupleSystem()->CoupleItemScript(iItemID);

	if (pairCoupleItem.first == false)
		return;

	SCoupleItemInfo& item = pairCoupleItem.second;

	for (int i = 0; i < (int)item.vecParticle.size(); ++i)
	{
		std::string strName = m_pkCoupleView->GetRandomName();
		SCoupleParticle& particle = item.vecParticle[i];
		m_pkCoupleView->AddParticle(strName, particle.strName, particle.vPos, (EGCRenderLayer)particle.iLayer);
	}

	for (int i = 0; i < (int)item.vecMesh.size(); ++i)
	{
		std::string strName = m_pkCoupleView->GetRandomName();
		SCoupleMesh& mesh = item.vecMesh[i];
		m_pkCoupleView->AddObject(strName, mesh.strMesh, mesh.strTexture);
		//m_pkCoupleView->SetObjectScale( strName, mesh.vScale );   //해상도 높였을때 커플창에 생기는 문제발생해서 해당코드 주석처리함
		m_pkCoupleView->SetObjectRotation(strName, mesh.vRotation);
		m_pkCoupleView->SetObjectPosition(strName, mesh.vTranslation);
	}

	if (item.sBackground.strMesh.empty() == false)
		m_pkCoupleView->SetBackground(item.sBackground.strMesh, item.sBackground.strTexture);
}

void KGCCoupleWnd::SetMyCoupleWnd()
{
	m_bMyCouple = true;
	for (int i = 0; i < GCCW_MOTION_NUM; ++i)
	{
		m_pkMotion[i]->Lock(false);
		m_pkMotion[i]->ClearToolTip();
#if defined( __PATH__ )
		if (i == 0)
			m_pkMotion[i]->InitState(true, true, this);
		else if (i == 1 && SiKGCCoupleSystem()->GetMyCoupleDay() >= 10)
			m_pkMotion[i]->InitState(true, true, this);
		else if (i == 2 && SiKGCCoupleSystem()->GetMyCoupleDay() >= 20)
			m_pkMotion[i]->InitState(true, true, this);
		else
		{
			m_pkMotion[i]->InitState(true, true);
			m_pkMotion[i]->Lock(true);
			if (i == 1 && SiKGCCoupleSystem()->GetMyCoupleDay() < 10)
				m_pkMotion[i]->SetToolTip(g_pkStrLoader->GetReplacedString(STR_ID_COUPLE_SYSTEM38, "i", 10));
			else if (i == 2 && SiKGCCoupleSystem()->GetMyCoupleDay() < 20)
				m_pkMotion[i]->SetToolTip(g_pkStrLoader->GetReplacedString(STR_ID_COUPLE_SYSTEM38, "i", 20));
			else
				m_pkMotion[i]->SetToolTip(g_pkStrLoader->GetString(STR_ID_NOW_WE_MAKE_IT_TOT));
		}
#else
		m_pkMotion[i]->InitState(true, true, this);	//현재 모션이 대기모션 외 3가지 뿐이다.
		m_pkMotion[i]->Lock(i >= 3);
#endif
		//m_pkMotion[i]->Lock( true );
		m_pkMotion[i]->SetText(i + 1);
	}

	for (int i = 0; i < GCCW_CHAR_NUM; ++i)
	{
		m_pkNickName[i]->InitState(true, true, this);
		m_pkNickName[i]->SetAlign(DT_CENTER);
	}

	for (int i = 0; i < GCCW_SLOT_NUM; ++i)
	{
		m_pkSlot[i]->InitState(true, true, this);
		m_pkSlot[i]->Lock(false);

		m_akItemImage[i].SetScale(0.13f, 0.13f);
		m_akItemImage[i].SetPosition((m_pkSlot[i]->GetFixedWindowPos().x + 4.0f) / (400.0f * GC_SCREEN_DIV_WIDTH), -(m_pkSlot[i]->GetFixedWindowPos().y - 7.0f) / (300.0f * GC_SCREEN_DIV_WIDTH) * 0.75f + 1.1f);
		m_akItemImage[i].CameraMatrixOn();
	}

	m_kRingImage.RemoveAllTexture();
	m_kRingImage.AddTexture(g_pItemMgr->CreateItemTexture(SiKGCCoupleSystem()->GetRingItemID(true), 0));

	std::wstring strHoney = L"#c" + SiKGCCoupleSystem()->GetNickColor_of_my_honey() + SiKGCCoupleSystem()->GetNickName_of_my_honey() + L"#cx";
	std::wstring strMyOneHoney = L"#c" + g_kGlobalValue.m_kUserInfo.strNickColor + g_kGlobalValue.m_kUserInfo.strNickName + L"#cx";
	if (strHoney == L"")
	{
		strHoney = g_pkStrLoader->GetString(STR_ID_NO_NICKNAME);
	}

	m_pkNickName[GCCW_RIGHT_CHAR]->SetText(SiKGCCoupleSystem()->GetPosition(true) ? strMyOneHoney : strHoney);
	m_pkNickName[GCCW_LEFT_CHAR]->SetText(!SiKGCCoupleSystem()->GetPosition(true) ? strMyOneHoney : strHoney);

	ChatEnable(true);
}

void KGCCoupleWnd::SetOtherCoupleWnd()
{
	m_bMyCouple = false;
	for (int i = 0; i < GCCW_MOTION_NUM; ++i)
	{
#if defined( __PATH__ )
		if (i == 0)
			m_pkMotion[i]->InitState(true, true, this);
		else if (i == 1 && SiKGCCoupleSystem()->GetYourCoupleDay() >= 10)
			m_pkMotion[i]->InitState(true, true, this);
		else if (i == 2 && SiKGCCoupleSystem()->GetYourCoupleDay() >= 20)
			m_pkMotion[i]->InitState(true, true, this);
		else
		{
			m_pkMotion[i]->InitState(true, true);
			m_pkMotion[i]->Lock(true);
		}
#else
		m_pkMotion[i]->InitState(true, true, this);	//현재 모션이 대기모션 외 3가지 뿐이다.
		m_pkMotion[i]->Lock(i >= 3);
#endif
		//m_pkMotion[i]->Lock( true );
		m_pkMotion[i]->SetText(i + 1);
	}

	for (int i = 0; i < GCCW_CHAR_NUM; ++i)
	{
		m_pkNickName[i]->InitState(true, true, this);
		m_pkNickName[i]->SetAlign(DT_CENTER);
	}

	for (int i = 0; i < GCCW_SLOT_NUM; ++i)
	{
		m_pkSlot[i]->InitState(true);
		m_pkSlot[i]->Lock(true);

		m_akItemImage[i].SetScale(0.0f, 0.0f);
		m_akItemImage[i].SetPosition((m_pkSlot[i]->GetFixedWindowPos().x + 4.0f) / (400.0f * GC_SCREEN_DIV_WIDTH), -(m_pkSlot[i]->GetFixedWindowPos().y - 7.0f) / (300.0f * GC_SCREEN_DIV_WIDTH) * 0.75f + 1.1f);
		m_akItemImage[i].CameraMatrixOn();
	}

	m_kRingImage.RemoveAllTexture();
	m_kRingImage.AddTexture(g_pItemMgr->CreateItemTexture(SiKGCCoupleSystem()->GetRingItemID(false), 0));

	m_pkNickName[GCCW_RIGHT_CHAR]->SetText(SiKGCCoupleSystem()->GetNickName(false, SiKGCCoupleSystem()->GetTempPosition()));
	m_pkNickName[GCCW_LEFT_CHAR]->SetText(SiKGCCoupleSystem()->GetNickName(false, !SiKGCCoupleSystem()->GetTempPosition()));

	ChatEnable(false);
}

void KGCCoupleWnd::ChatEnable(bool bEnable)
{
	m_pkChatBg->InitState(bEnable);
	m_pkChatUngerBg->InitState(bEnable);
	m_pkChatEnter->InitState(bEnable);

	m_pkChatListOnOff->InitState(bEnable, bEnable, bEnable ? this : NULL);
	m_pkChatListOnOff->SetCheck(false);
	m_pkCoupleChatStatic->SetText(g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM33));
	m_pkCoupleChatStatic->SetFontColor(D3DCOLOR_ARGB(255, 0, 0, 0));
	m_pkChatListBg->InitState(false);
	m_pkChatList->InitState(false, bEnable, bEnable ? this : NULL);
	m_pkChatListClose->InitState(false, bEnable, bEnable ? this : NULL);

	m_pkChatEdit->InitState(bEnable, bEnable, bEnable ? this : NULL);
	m_pkSayBalloon->InitState(bEnable, bEnable);

	AddSystemMessage(g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM35));

	m_pkMark[GCCW_LEFT_CHAR]->InitState(bEnable, true, NULL);
	m_pkMark[GCCW_RIGHT_CHAR]->InitState(bEnable, true, NULL);
}

void KGCCoupleWnd::AddSystemMessage(const std::wstring& strMsg)
{
	m_pkSystemMessage->ToggleRender(true);
	m_pkSystemMsgBg->ToggleRender(true);
	m_pkSystemMessage->SetText(strMsg);
	m_iSystemMsgAlpha = 0xff;
	m_iSystemMsgViewFrm = GC_FPS_LIMIT;
}

void KGCCoupleWnd::AddChat(bool bMyChat, bool bRight, std::wstring& strMsg)
{
	ASSERT(g_pkChatManager != NULL);
	g_pkChatManager->RemoveColorKey(strMsg);
	g_pkChatManager->RemoveSpaceChar(strMsg);

	std::wstringstream stmChatMsg;
	stmChatMsg << SiKGCCoupleSystem()->GetNickName(true, !bMyChat) << " : " << strMsg;
	m_pkChatList->AddString(stmChatMsg.str());
	if (!m_pkChatListOnOff->GetCheck())
	{
#if defined( _DEBUG )
		KLuaManager luaMgr;
		GCFUNC::LoadLuaScript(luaMgr, "Dev.lua");

		float px, py;
		LUA_GET_VALUE_DEF("say_x", px, 1.0f);
		LUA_GET_VALUE_DEF("say_y", py, 1.0f);
#endif
		DWORD dwUID = bMyChat ? g_kGlobalValue.m_kUserInfo.dwUID : SiKGCCoupleSystem()->GetUID_of_my_honey();
		m_pkSayBalloon->NewUserMsg(strMsg, dwUID, KChatData::MSG_COMMON, 0xffffffff);
		//m_pkSayBalloon->SetBalloonPos( D3DXVECTOR2(px, py), 0 );
		m_pkSayBalloon->SetBalloonPos(D3DXVECTOR2(bRight ? 260.f : 140.f, 180.f),
			bMyChat ? g_kGlobalValue.m_kUserInfo.dwUID : SiKGCCoupleSystem()->GetUID_of_my_honey(),
			bRight ? KGCSayBalloonSquare::GCSB_ARROW_POS_RIGHT : KGCSayBalloonSquare::GCSB_ARROW_POS_LEFT);
		m_pkSayBalloon->SetBalloonPos(D3DXVECTOR2(!bRight ? 260.f : 140.f, 180.f),
			!bMyChat ? g_kGlobalValue.m_kUserInfo.dwUID : SiKGCCoupleSystem()->GetUID_of_my_honey(),
			!bRight ? KGCSayBalloonSquare::GCSB_ARROW_POS_RIGHT : KGCSayBalloonSquare::GCSB_ARROW_POS_LEFT);
	}
}

void KGCCoupleWnd::OnDestroyComplete(void)
{
	m_pkSayBalloon->DeleteAllBalloon();
}