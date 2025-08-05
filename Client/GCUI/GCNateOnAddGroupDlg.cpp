#include "stdafx.h"
#include ".\GCNateOnAddGroupDlg.h"
//
//

//
//
//

IMPLEMENT_CLASSNAME(GCNateOnAddGroupDlg);
IMPLEMENT_WND_FACTORY(GCNateOnAddGroupDlg);
IMPLEMENT_WND_FACTORY_NAME(GCNateOnAddGroupDlg, "gc_nateon_add_group_dlg");

GCNateOnAddGroupDlg::GCNateOnAddGroupDlg(void)
	: m_pkOK(NULL)
	, m_pkCancel(NULL)
	, m_pkTitle(NULL)
	, m_pkDesc(NULL)
	, m_pkDesc2(NULL)
	, m_pkEditGroup(NULL)
	, m_eWarn(GCNateOnAddGroupDlg::NO_WARN)
	, m_strPrevInput(L"")
	, m_strNoWarn(L"")
{
	LINK_CONTROL("ok_button", m_pkOK);
	LINK_CONTROL("cancel_button", m_pkCancel);
	LINK_CONTROL("title_static", m_pkTitle);
	LINK_CONTROL("nickname_static", m_pkDesc);
	LINK_CONTROL("message_static", m_pkDesc2);
	LINK_CONTROL("nickname_edit", m_pkEditGroup);
}

GCNateOnAddGroupDlg::~GCNateOnAddGroupDlg(void)
{
}

void GCNateOnAddGroupDlg::OnCreate()
{
	m_pkOK->InitState(true, true, this);
	m_pkCancel->InitState(true, true, this);

	m_pkOK->SetHotKey(DIK_RETURN);
	m_pkOK->SetHotKey(DIK_NUMPADENTER);
	m_pkCancel->SetHotKey(DIK_ESCAPE);

	m_pkTitle->InitState(true);
	m_pkTitle->SetAlign(DT_CENTER);
	m_pkDesc->InitState(false);
	m_pkDesc->SetFontColor(0xCCCCCCCC);
	m_pkDesc->SetAlign(DT_CENTER);
	m_pkDesc2->SetAlign(DT_RIGHT);

	m_pkEditGroup->InitState(true, true, this);
	m_pkEditGroup->SetText(L"");

	Transformation();
	SetWarning(NO_WARN);
}

void GCNateOnAddGroupDlg::ActionPerformed(const KActionEvent& event)
{
	GCWND_MSG_MAP(m_pkOK, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK);
	GCWND_MSG_MAP(m_pkCancel, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose);

	if (event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN)
	{
		OnOK();
	}
	else if (event.m_dwCode == KD3DWnd::D3DWE_EDIT_ESCAPE)
	{
		OnClose();
	}
}

void GCNateOnAddGroupDlg::OnClose()
{
	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));

	m_strPrevInput = L"";
	m_strOriGroupName = L"";
}

void GCNateOnAddGroupDlg::OnCreateComplete()
{
	m_pkEditGroup->SetFocus();
	m_pkEditGroup->SetText(L"");

	if (m_eUse == GCNateOnAddGroupDlg::RENAME)
	{
		m_pkEditGroup->SetText(m_strOriGroupName.c_str());
	}
	else
	{
		m_strOriGroupName = L"";
	}
	m_strPrevInput = L"";
}

static wchar_t s_wcProhibition[] = { L'\t', L'\'', L'\"', L',', L'　', L'!' };

void GCNateOnAddGroupDlg::FrameMoveInEnabledState()
{
	switch (m_eUse)
	{
	case GCNateOnAddGroupDlg::ADD:
		return AddModeFrameMove();
	case GCNateOnAddGroupDlg::RENAME:
		return RenameModeFrameMove();
	case GCNateOnAddGroupDlg::BLOCK_BUDDY:
		return BlockModeFrameMove();
	default:
		assert(!"invalid mode");
		break;
	}
}

void GCNateOnAddGroupDlg::OnOK()
{
	if (0 == wcslen(m_pkEditGroup->GetText()) || GCNateOnAddGroupDlg::NO_WARN != m_eWarn)
	{
		OnClose();
	}
	else
	{
		SpeakToActionListener(KActionEvent(this, KD3DWnd::D3DWE_EDIT_RETURN));
	}
}

void GCNateOnAddGroupDlg::OnDestroyComplete()
{
	m_pkEditGroup->Activate(false);
	SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_DESTROYED));
}

std::wstring GCNateOnAddGroupDlg::GetInputStr()
{
	return m_pkEditGroup->GetText();
}

void GCNateOnAddGroupDlg::SetWarning(GCNateOnAddGroupDlg::WARN_TYPE eWarnType)
{
	switch (eWarnType)
	{
	case GCNateOnAddGroupDlg::NO_WARN:
		m_pkDesc2->SetText(m_strNoWarn);
		m_pkDesc2->SetFontColor(0xFFFFFFFF);
		break;
	case GCNateOnAddGroupDlg::EQUAL:
		m_pkDesc2->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_GROUP_NAME_DIFFERENT));
		m_pkDesc2->SetFontColor(0xFFFF0000);
		break;
	case GCNateOnAddGroupDlg::ILLEGAR:
		m_pkDesc2->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_ADD_GROUP_WARN));
		m_pkDesc2->SetFontColor(0xFFFF0000);
		break;
	default:
		assert(!"invalid warntype");
		break;
	}
	m_eWarn = eWarnType;
}

void GCNateOnAddGroupDlg::SetOrigGroupName(const std::wstring& strGroupName)
{
	if (m_eUse != GCNateOnAddGroupDlg::RENAME)
		return;

	m_strOriGroupName = strGroupName;
}

void GCNateOnAddGroupDlg::Transformation()
{
	m_pkTitle->SetText(L"");
	m_pkDesc->SetText(L"");
	m_pkDesc2->SetText(L"");
	m_strNoWarn = L"";

	switch (m_eUse)
	{
	case GCNateOnAddGroupDlg::ADD:
		m_pkTitle->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_ADD_GROUP));
		m_pkDesc->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_ADD_GROUP_DESC));
		m_strNoWarn = g_pkStrLoader->GetString(STR_ID_NATEON_ADD_GROUP_DESC2);
		m_pkEditGroup->SetLimitText(12);
		break;
	case GCNateOnAddGroupDlg::RENAME:
		m_pkTitle->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_RENAME_GROUP));
		m_strNoWarn = g_pkStrLoader->GetString(STR_ID_NATEON_ADD_GROUP_DESC2);
		m_pkEditGroup->SetLimitText(12);
		break;
	case GCNateOnAddGroupDlg::BLOCK_BUDDY:
		m_pkTitle->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_BLOCK_BUDDY));
		m_pkDesc->SetText(g_pkStrLoader->GetString(STR_ID_NATEON_ADD_GROUP_DESC4));
		m_strNoWarn = g_pkStrLoader->GetReplacedString(STR_ID_NATEON_ADD_GROUP_DESC3, "i", GC_NICKNAME_SIZE);
		m_pkEditGroup->SetLimitText(GC_NICKNAME_SIZE);
		break;
	}
}

void GCNateOnAddGroupDlg::AddModeFrameMove()
{
	std::wstring strInput = m_pkEditGroup->GetText();

	if (m_strPrevInput == strInput)
	{
		if (m_strPrevInput.empty())
		{
			SetWarning(GCNateOnAddGroupDlg::NO_WARN);
			m_pkDesc->ToggleRender(true);
		}
		return;
	}

	m_strPrevInput = strInput;
	if (strInput.empty())
	{
		SetWarning(GCNateOnAddGroupDlg::NO_WARN);
		m_pkDesc->ToggleRender(true);
	}
	else
	{
		for (int i = 0; i < sizeof(s_wcProhibition) / sizeof(s_wcProhibition[0]); ++i)
		{
			if (strInput.find(s_wcProhibition[i]) != std::wstring::npos)
			{
				SetWarning(GCNateOnAddGroupDlg::ILLEGAR);
				return;
			}
		}
		SetWarning(GCNateOnAddGroupDlg::NO_WARN);
		m_pkDesc->ToggleRender(false);
	}
}

void GCNateOnAddGroupDlg::RenameModeFrameMove()
{
	std::wstring strInput = m_pkEditGroup->GetText();

	m_pkDesc->ToggleRender(false);
	if (m_strPrevInput == strInput)
	{
		if (m_strPrevInput.empty())
		{
			SetWarning(GCNateOnAddGroupDlg::NO_WARN);
		}
		return;
	}

	m_strPrevInput = strInput;
	if (strInput.empty())
	{
		SetWarning(GCNateOnAddGroupDlg::NO_WARN);
	}
	else
	{
		for (int i = 0; i < sizeof(s_wcProhibition) / sizeof(s_wcProhibition[0]); ++i)
		{
			if (strInput.find(s_wcProhibition[i]) != std::wstring::npos)
			{
				SetWarning(GCNateOnAddGroupDlg::ILLEGAR);
				return;
			}
		}

		if (strInput == m_strOriGroupName)
		{
			SetWarning(GCNateOnAddGroupDlg::EQUAL);
			return;
		}

		SetWarning(GCNateOnAddGroupDlg::NO_WARN);
	}
}

void GCNateOnAddGroupDlg::BlockModeFrameMove()
{
	std::wstring strInput = m_pkEditGroup->GetText();
	SetWarning(GCNateOnAddGroupDlg::NO_WARN);
	if (m_strPrevInput == strInput)
	{
		if (m_strPrevInput.empty())
		{
			m_pkDesc->ToggleRender(true);
		}
		return;
	}

	m_strPrevInput = strInput;
	if (strInput.empty())
	{
		m_pkDesc->ToggleRender(true);
	}
	else
	{
		m_pkDesc->ToggleRender(false);
	}
}

bool GCNateOnAddGroupDlg::CheckCondition(IN const KDialogInfo& kInfo_)
{
	if (kInfo_.m_nMsgBoxUse == KGCUIScene::GC_MBOX_USE_NATEON_ADD_GROUP)
	{
		this->SetUse(GCNateOnAddGroupDlg::ADD, (int)kInfo_.m_lParam);
	}
	else if (kInfo_.m_nMsgBoxUse == KGCUIScene::GC_MBOX_USE_NATEON_RENAME_GROUP)
	{
		this->SetUse(GCNateOnAddGroupDlg::RENAME, (int)kInfo_.m_lParam);
		std::wstring strOrign;
		SiKFriendShip()->GetGroupName((int)kInfo_.m_lParam, strOrign);
		this->SetOrigGroupName(strOrign);
	}
	else if (kInfo_.m_nMsgBoxUse == KGCUIScene::GC_MBOX_USE_NATEON_BLOCK_BUDDY)
	{
		this->SetUse(GCNateOnAddGroupDlg::BLOCK_BUDDY, (int)kInfo_.m_lParam);
	}
	return true;
}