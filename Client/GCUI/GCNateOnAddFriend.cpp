#include "stdafx.h"
#include ".\GCNateOnAddFriend.h"
//
//

//
//

IMPLEMENT_CLASSNAME(GCNateOnAddFriend);
IMPLEMENT_WND_FACTORY(GCNateOnAddFriend);
IMPLEMENT_WND_FACTORY_NAME(GCNateOnAddFriend, "gc_nateon_add_friend_dlg");

GCNateOnAddFriend::GCNateOnAddFriend(void)
	: m_pkTitle(NULL)
	, m_pkNickNameStatic(NULL)
	, m_pkNickNameGuide(NULL)
	, m_pkNickNameEdit(NULL)
	, m_pkMessageStatic(NULL)
	, m_pkMessageGuide(NULL)
	, m_pkMessageEdit(NULL)
	, m_pkOK(NULL)
	, m_pkCancel(NULL)
	, m_eUse(GCNateOnAddFriend::INIT)
{
	LINK_CONTROL("title_static", m_pkTitle);
	LINK_CONTROL("nickname_static", m_pkNickNameStatic);
	LINK_CONTROL("nickname_guide_static", m_pkNickNameGuide);
	LINK_CONTROL("nickname_edit", m_pkNickNameEdit);
	LINK_CONTROL("message_static", m_pkMessageStatic);
	LINK_CONTROL("message_guide_static", m_pkMessageGuide);
	LINK_CONTROL("message_edit", m_pkMessageEdit);
	LINK_CONTROL("ok_button", m_pkOK);
	LINK_CONTROL("cancel_button", m_pkCancel);
}

GCNateOnAddFriend::~GCNateOnAddFriend(void)
{
}

void GCNateOnAddFriend::OnCreate()
{
	m_pkOK->InitState(true, true, this);
	m_pkOK->SetHotKey(DIK_RETURN);
	m_pkOK->SetHotKey(DIK_NUMPADENTER);
	m_pkCancel->InitState(true, true, this);
	m_pkCancel->SetHotKey(DIK_ESCAPE);

	m_pkTitle->InitState(true);
	m_pkTitle->SetAlign(DT_CENTER);

	m_pkNickNameStatic->InitState(true);
	m_pkNickNameGuide->InitState(true);
	m_pkNickNameGuide->SetAlign(DT_RIGHT);
	m_pkNickNameEdit->InitState(true, true, this);

	m_pkMessageStatic->InitState(true);
	m_pkMessageGuide->InitState(true);
	m_pkMessageGuide->SetAlign(DT_RIGHT);
	m_pkMessageEdit->InitState(true, true, this);

	Transformation();

	m_wstrNickName = L"";
	m_wstrMessage = L"";
}

void GCNateOnAddFriend::Transformation()
{
	m_pkTitle->SetText(L"");
	m_pkNickNameStatic->SetText(L"");
	m_pkNickNameGuide->SetText(L"");
	m_pkNickNameEdit->SetText(L"");
	m_pkMessageStatic->SetText(L"");
	m_pkMessageEdit->SetText(L"");
	m_pkMessageGuide->SetText(L"");

	m_pkNickNameEdit->SetLimitText(GC_NICKNAME_SIZE);
	m_pkMessageEdit->SetLimitText(25);
	m_pkMessageEdit->SetMultiLine(true);

	switch (m_eUse)
	{
	case GCNateOnAddFriend::ADD:
		m_pkTitle->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU7));
		m_pkNickNameStatic->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_ADD_BUDDY_ADDMSG));
		m_pkNickNameStatic->SetFontColor(0xCCCCCCCC);
		m_pkNickNameStatic->SetAlign(DT_CENTER);
		m_pkNickNameGuide->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NATEON_ADD_GROUP_DESC3, "i", GC_NICKNAME_SIZE));
		m_pkMessageGuide->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NATEON_ADD_GROUP_DESC3, "i", 25));
		m_pkMessageStatic->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_ADD_BUDDY_REQMSG));
		m_pkMessageStatic->SetFontColor(0xCCCCCCCC);
		m_pkMessageStatic->SetAlign(DT_CENTER);
		m_pkMessageEdit->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_ADD_BUDDY_DEFMSG).c_str());

		m_pkNickNameEdit->SetText(m_wstrNickName.c_str());

		if (m_wstrMessage.length() <= 0)
			m_pkMessageEdit->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_ADD_BUDDY_DEFMSG).c_str());
		else
			m_pkMessageEdit->SetText(m_wstrMessage.c_str());

		break;
	case GCNateOnAddFriend::REQUEST:
		m_pkTitle->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_MULTIMENU7));
		m_pkNickNameGuide->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NATEON_ADD_GROUP_DESC3, "i", GC_NICKNAME_SIZE));
		m_pkMessageGuide->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NATEON_ADD_GROUP_DESC3, "i", 25));

		m_pkNickNameStatic->SetText(m_wstrNickName.c_str());
		m_pkNickNameStatic->SetFontColor(0xCCCCCCCC);
		m_pkNickNameStatic->SetAlign(DT_CENTER);
		m_pkMessageStatic->SetText(m_wstrMessage.c_str());
		m_pkMessageStatic->SetFontColor(0xCCCCCCCC);
		m_pkMessageStatic->SetAlign(DT_CENTER);

		m_pkNickNameEdit->ToggleRender(false);
		m_pkMessageEdit->ToggleRender(false);
		break;
	}
}

void GCNateOnAddFriend::SetUse(E_USE eUse)
{
	m_eUse = eUse;
}

void GCNateOnAddFriend::OnDestroyComplete()
{
	m_pkNickNameEdit->Activate(false);
	m_pkMessageEdit->Activate(false);
	SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_DESTROYED));
}

void GCNateOnAddFriend::OnCancel()
{
	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
}

void GCNateOnAddFriend::ActionPerformed(const KActionEvent& event)
{
	GCWND_MSG_MAP(m_pkOK, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK);
	GCWND_MSG_MAP(m_pkCancel, KD3DWnd::D3DWE_BUTTON_CLICK, OnCancel);

	if (event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN)
	{
		OnOK();
	}
	else if (event.m_dwCode == KD3DWnd::D3DWE_EDIT_ESCAPE)
	{
		OnCancel();
	}
}

void GCNateOnAddFriend::FrameMoveInEnabledState()
{
	switch (m_eUse)
	{
	case GCNateOnAddFriend::ADD:
		return AddModeFrameMove();
	case GCNateOnAddFriend::REQUEST:
		return AddModeFrameMove();
	default:
		assert(!"invalid mode");
		break;
	}
}

void GCNateOnAddFriend::AddModeFrameMove()
{
	std::wstring strMsg = m_pkMessageEdit->GetText();
	if (strMsg.empty())
	{
		m_pkMessageStatic->ToggleRender(true);
	}
	else
	{
		m_pkMessageStatic->ToggleRender(false);
	}
	std::wstring strNick = m_pkNickNameEdit->GetText();
	if (strNick.empty())
	{
		m_pkNickNameStatic->ToggleRender(true);
	}
	else
	{
		m_pkNickNameStatic->ToggleRender(false);
	}
}

void GCNateOnAddFriend::OnOK()
{
	if (GCNateOnAddFriend::ADD == m_eUse) {
		if (0 == wcslen(m_pkNickNameEdit->GetText()))
		{
			OnCancel();
		}
		else
		{
			m_wstrNickName = m_pkNickNameEdit->GetText();
			m_wstrMessage = m_pkMessageEdit->GetText();

			SpeakToActionListener(KActionEvent(this, KD3DWnd::D3DWE_EDIT_RETURN));
		}
	}
	else if (GCNateOnAddFriend::REQUEST == m_eUse) {
		m_wstrNickName = m_pkNickNameStatic->GetText();
		m_wstrMessage = m_pkMessageStatic->GetText();

		SpeakToActionListener(KActionEvent(this, KD3DWnd::D3DWE_EDIT_RETURN));
	}
}

void GCNateOnAddFriend::OnCreateComplete()
{
	if (m_eUse == GCNateOnAddFriend::ADD)
	{
		m_pkNickNameEdit->SetFocus();
	}
}

bool GCNateOnAddFriend::CheckCondition(IN const KDialogInfo& kInfo_)
{
	switch (kInfo_.m_nMsgBoxUse)
	{
	case KGCUIScene::GC_MBOX_USE_NATEON_ADD_BUDDY:
	{
		this->SetUse(GCNateOnAddFriend::ADD);
	}
	break;
	case KGCUIScene::GC_MBOX_USE_NATEON_ADD_BUDDY_ALRAM:
	{
		this->SetUse(GCNateOnAddFriend::REQUEST);
	}
	break;
	}
	this->SetNickName(kInfo_.m_strText1);
	this->SetMessage(kInfo_.m_strText2);
	return true;
}