#include "stdafx.h"
#include "KGCNicknameColorBox.h"
#include "GCItemManager.h"
#include "../MyD3D.h"
#include "GCTextColorManager.h"
#include "KGCRoomListManager.h"

IMPLEMENT_CLASSNAME(KGCNicknameColorBox);
IMPLEMENT_WND_FACTORY(KGCNicknameColorBox);
IMPLEMENT_WND_FACTORY_NAME(KGCNicknameColorBox, "gc_nickcolorchangedlg");

KGCNicknameColorBox::KGCNicknameColorBox(void):
	m_pkBtnConfirm(NULL),
	m_pkClose(NULL),
	m_pkColorCombo(NULL),
	m_pkDefaultMessage(NULL) {
	LINK_CONTROL("btn_confirm", m_pkBtnConfirm);
	LINK_CONTROL("btn_close", m_pkClose);
	LINK_CONTROL("combo_nick", m_pkColorCombo);
	LINK_CONTROL("static_preview_nickname", m_pkDefaultMessage);
}

KGCNicknameColorBox::~KGCNicknameColorBox(void) {
}

void KGCNicknameColorBox::OnCreate(void) {
	m_pkBtnConfirm->InitState(true, true, this);
	m_pkClose->InitState(true, true, this);
	m_pkClose->SetHotKey(DIK_ESCAPE);
	m_pkColorCombo->InitState(true, true, this);
	m_pkColorCombo->SetAlign(DT_CENTER);
	m_pkDefaultMessage->InitState(false, true, this);
	m_pkDefaultMessage->SetAlign(DT_CENTER);
	currentColor = L"FFFFFF";
	m_pkBtnConfirm->Lock(true);

	SetNicknameColor();
	m_pkDefaultMessage->SetText(L"Selecione uma cor");
}

void KGCNicknameColorBox::SetNicknameColor() {
	m_pkColorCombo->DeleteAllItem();

	KLuaManager luaMgr;
	if (GCFUNC::LoadLuaScript(luaMgr, "nickcolortable.lua"))
	{
		luaMgr.BeginTable("NickNameColor");
		{
			for (int i = 1; ; ++i) {
				std::string str;
				if (!SUCCEEDED(luaMgr.GetValue(i, str))) break;
				std::wstring str2 = GCUTIL_STR::GCStrCharToWide(str.c_str());
				m_pkColorCombo->AddString(L"#c" + str2 + g_kGlobalValue.m_kUserInfo.strNickName + L"#cX", DT_CENTER);
				m_vecstrNickColorList.push_back(str2);
			}
		}
		luaMgr.EndTable();
	}
}

void KGCNicknameColorBox::OnClose() {
	currentColor = L"FFFFFF";
	g_KDSound.Play("31");
	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
}

void KGCNicknameColorBox::OnConfirm() {
	if (currentColor == g_kGlobalValue.m_kUserInfo.strNickColor)
	{
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_SAME_NICKCOLOR_TEXT), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
	}
	else
	{
		g_pkUIScene->ChangeNickColor( g_kGlobalValue.NickColorChangeItemID, g_kGlobalValue.NickColorChangeItemUID );
		OnClose();
	}
}

void KGCNicknameColorBox::ActionPerformed(const KActionEvent& event) {
	if (event.m_dwCode == D3DWE_COMBO_SELCHANGE)
	{
		currentColor = m_vecstrNickColorList[m_pkColorCombo->GetCurSel()];
		m_pkDefaultMessage->SetText(L"");

		if (currentColor == L"FFFFFF" || currentColor == g_kGlobalValue.m_kUserInfo.strNickColor)
		{
			m_pkBtnConfirm->Lock(true);
		}
		else
		{
			m_pkBtnConfirm->Lock(false);
		}
	}

	GCWND_MSG_MAP(m_pkBtnConfirm, KD3DWnd::D3DWE_BUTTON_CLICK, OnConfirm);
	GCWND_MSG_MAP(m_pkClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose);
}