#include "stdafx.h"
#include ".\GCNateOnChat.h"

IMPLEMENT_CLASSNAME(GCNateOnChat);
IMPLEMENT_WND_FACTORY(GCNateOnChat);
IMPLEMENT_WND_FACTORY_NAME(GCNateOnChat, "gc_talk_window");

GCNateOnChat::GCNateOnChat(void)
{
	m_pkScroll = NULL;
	m_pkUserScroll = NULL;
	m_pkEmoticonSelect = NULL;
	m_pkInvite = NULL;
	m_pkTalkBox = NULL;
	m_pkUserList = NULL;
	m_pkTalkEditor = NULL;
	m_bEnableEditor = false;

	LINK_CONTROL("scroll", m_pkScroll);
	LINK_CONTROL("user_list_scroll", m_pkUserScroll);
	LINK_CONTROL("emoticon_btn", m_pkEmoticonSelect);
	LINK_CONTROL("invite", m_pkInvite);
	LINK_CONTROL("talk_box", m_pkTalkBox);
	LINK_CONTROL("user_list", m_pkUserList);
	LINK_CONTROL("talk_editor", m_pkTalkEditor);
}

GCNateOnChat::~GCNateOnChat(void)
{
}

void GCNateOnChat::OnCreate()
{
	m_pkScroll->InitState(true, true, this);
	m_pkUserScroll->InitState(true, true, this);
#if defined( NATION_BRAZIL )
	m_pkEmoticonSelect->InitState(false);
#else
	m_pkEmoticonSelect->InitState(true, true, this);
#endif
	m_pkInvite->InitState(true, true, this);
	m_pkTalkBox->InitState(true, true, this);
	m_pkTalkBox->SetLineSpace(1.6f);
	m_pkUserList->InitState(true, true, this);
	m_pkUserList->SetLineSpace(1.6f);
	m_pkTalkEditor->InitState(true, true, this);
}

void GCNateOnChat::OnCreateComplete(void)
{
	m_pkScroll->SetScrollPageSize(m_pkTalkBox->GetDisplayLimitLine());
	m_pkScroll->SetScrollRangeMax(m_pkTalkBox->GetCount());
	m_pkScroll->SetScrollPos(m_pkTalkBox->GetTopIndex());

	m_pkUserScroll->SetScrollPageSize(m_pkUserList->GetDisplayLimitLine());
	m_pkUserScroll->SetScrollRangeMax(m_pkUserList->GetCount());
	m_pkUserScroll->SetScrollPos(m_pkUserList->GetTopIndex());
}

void GCNateOnChat::FrameMoveInEnabledState()
{
	if (m_bEnableEditor)
	{
		if (m_pkTalkEditor != NULL)
		{
			if (::GetFocus() != m_pkTalkEditor->GetHandle())
				m_pkTalkEditor->SetFocus();
		}
	}
}

void GCNateOnChat::ActionPerformed(const KActionEvent& event)
{
	if (event.m_pWnd == m_pkTalkEditor
		&& KP2P::GetInstance()->m_kMsgServer != NULL)
	{
		if (0 == ::wcslen(m_pkTalkEditor->GetText()))
			return;

		if (event.m_dwCode == D3DWE_EDIT_ESCAPE)
		{
			m_pkTalkEditor->SetText(L"");
			g_KDSound.Play("72");
			return;
		}
		else if (event.m_dwCode == D3DWE_EDIT_RETURN)
		{
			if (wcslen(m_pkTalkEditor->GetText()) == 0)
				return;

			std::wstring wstrChatMsg = m_pkTalkEditor->GetText();
			SiKD3DEmoticonString()->CheckForSend(wstrChatMsg);
			KP2P::GetInstance()->m_kMsgServer->Send_ChatReq_S2(m_prCurTalkKey, m_mapTalkBox[m_prCurTalkKey].GetTalkerUID(), wstrChatMsg);

			m_pkTalkEditor->SetText(L"");

			return;
		}
	}

	if (event.m_pWnd == m_pkScroll)
	{
		if (m_pkTalkBox->GetTopIndex() != m_pkScroll->GetScrollPos())
		{
			m_pkTalkBox->SetTopIndex(m_pkScroll->GetScrollPos());
		}
	}

	if (event.m_pWnd == m_pkUserScroll)
	{
		if (m_pkUserList->GetTopIndex() != m_pkUserScroll->GetScrollPos())
		{
			m_pkUserList->SetTopIndex(m_pkUserScroll->GetScrollPos());
		}
	}

	if (event.m_pWnd == m_pkUserList)
	{
		if (event.m_dwCode == D3DWE_RBTN_SELECT)
		{
			KActionEvent temp(this, GCNateOn::NATEON_TALK_USER_SELECT);
			temp.m_dwlParam = event.m_dwlParam;
			SpeakToActionListener(temp);
		}
	}

	if (m_bEnableEditor)
	{
		GCWND_MSG_MAP(m_pkEmoticonSelect, KD3DWnd::D3DWE_BUTTON_CLICK, OnInputEmoticon);
	}

	if (event.m_pWnd == m_pkInvite)
	{
		if (D3DWE_BUTTON_CLICK == event.m_dwCode)
			g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NATEON_TALK_INVITE, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
	}
}

void GCNateOnChat::RefreshTalkBox(std::pair<DWORD, DWORD> UID)
{
	m_pkTalkBox->Clear();
	m_pkScroll->SetScrollPos(0);
	m_pkScroll->SetScrollRangeMax(0);

	NATE_TALK_BOX* pTalkBox = GetTalkBox(UID);
	if (pTalkBox)
	{
		for (int j = 0; j < (int)pTalkBox->GetTalkContent().size(); ++j)
		{
			m_pkTalkBox->AddString(pTalkBox->GetTalkContent()[j].first, pTalkBox->GetTalkContent()[j].second);
		}
	}

	m_pkScroll->SetScrollPageSize(m_pkTalkBox->GetDisplayLimitLine());
	m_pkScroll->SetScrollRangeMax(m_pkTalkBox->GetCount());
	m_pkScroll->SetScrollPos(m_pkTalkBox->GetTopIndex());
}

void GCNateOnChat::RefreshTalkerList(std::pair<DWORD, DWORD> UID)
{
	m_pkUserList->Clear();
	m_pkUserScroll->SetScrollPos(0);
	m_pkUserScroll->SetScrollRangeMax(0);

	std::wstring talkerColor;

	NATE_TALK_BOX* pTalkBox = GetTalkBox(UID);
	if (pTalkBox)
	{
		std::map< DWORD, std::pair<std::wstring, std::wstring> >::iterator itTalker = pTalkBox->GetTalker().begin();
		for (; itTalker != pTalkBox->GetTalker().end(); ++itTalker)
		{
			if (itTalker->second.second == L"" || itTalker->second.second.size() < 6)
				talkerColor = L"FFFFFF";
			else
				talkerColor = itTalker->second.second;


			if (itTalker->first == g_kGlobalValue.m_kUserInfo.dwUID)
				m_pkUserList->AddString(itTalker->second.first, SiGCTextColorManager()->StringToColor(talkerColor), true, 0, false, itTalker->second.first);
			else
				m_pkUserList->AddString(itTalker->second.first, SiGCTextColorManager()->StringToColor(talkerColor), true, 0, false, itTalker->second.first);
		}
	}

	m_pkUserScroll->SetScrollPageSize(m_pkUserList->GetDisplayLimitLine());
	m_pkUserScroll->SetScrollRangeMax(m_pkUserList->GetCount());
	m_pkUserScroll->SetScrollPos(m_pkUserList->GetTopIndex());
}

void GCNateOnChat::AddChatString(std::pair<DWORD, DWORD> UID, std::wstring& strChat, DWORD dwColor)
{
	std::wstring strFiltered = strChat;
	strFiltered = SiKSlangFilter()->Filter(strFiltered, KSlangFilter::SLANG_FILTER_CHAT);

	if (m_mapTalkBox.count(UID))
	{
		m_mapTalkBox[UID].InsertTalk(strFiltered, dwColor);

		if (GetCurTalkKey() == UID)
		{
			m_pkTalkBox->AddString(strFiltered, dwColor);
			m_pkScroll->SetScrollPageSize(m_pkTalkBox->GetDisplayLimitLine());
			m_pkScroll->SetScrollRangeMax(m_pkTalkBox->GetCount());
			m_pkScroll->SetScrollPos(m_pkTalkBox->GetTopIndex());
		}
	}
}

void GCNateOnChat::AddTalkBox(std::pair<DWORD, DWORD> UID, NATE_TALK_BOX& vtTalkBox)
{
	m_mapTalkBox.insert(std::make_pair(UID, vtTalkBox));
}

NATE_TALK_BOX* GCNateOnChat::GetTalkBox(std::pair<DWORD, DWORD> UID)
{
	if (m_mapTalkBox.count(UID))
		return &m_mapTalkBox[UID];

	return NULL;
}

void GCNateOnChat::DeleteTalkBox(std::pair<DWORD, DWORD> UID)
{
	if (m_mapTalkBox.count(UID))
	{
		m_mapTalkBox.erase(UID);

		if (UID == GetCurTalkKey())
		{
			if ((int)m_mapTalkBox.size() > 0)
			{
				SetCurTalkKey(m_mapTalkBox.begin()->first);
			}
			else
			{
				SetCurTalkKey(std::make_pair(0, 0));
			}
		}
	}
}

void GCNateOnChat::ClearTalkListBox(void)
{
	m_pkTalkBox->Clear();
}

void GCNateOnChat::ClearChatWindow()
{
	ClearTalkListBox();
	m_pkUserList->Clear();
	m_mapTalkBox.clear();
	m_bEnableEditor = false;
}

std::pair<DWORD, DWORD> GCNateOnChat::GetAllTalkerEqual(std::vector< DWORD >& vtTalker)
{
	std::map< std::pair<DWORD, DWORD>, NATE_TALK_BOX >::iterator itTB = m_mapTalkBox.begin();
	for (; itTB != m_mapTalkBox.end(); ++itTB)
	{
		if (itTB->second.IsAllTalkerEqual(vtTalker))
			return itTB->first;
	}

	return std::make_pair(0, 0);
}

void GCNateOnChat::EnableEditor(bool bEnable)
{
	if (D3DWS_ENABLED != GetWndState())
		return;

	if (m_pkTalkEditor)
	{
		m_pkTalkEditor->EnableSelfFocusCheck(bEnable);
		if (bEnable)
			m_pkTalkEditor->SetFocus();
		else
			::SetFocus(NULL);
	}

	m_bEnableEditor = bEnable;
}

void GCNateOnChat::OnInputEmoticon()
{
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_EMOTICON_SELECT_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NATEON_INPUT_EMOTICON, 0, 0, false, true);
}

std::wstring GCNateOnChat::GetUserListBoxNickName(const int iIndex)
{
	if (m_pkUserList)
		return m_pkUserList->GetReturnMsg(iIndex);

	return L"";
}

std::wstring GCNateOnChat::GetUserNickName(const DWORD dwUID)
{
	std::map< std::pair<DWORD, DWORD>, NATE_TALK_BOX >::iterator itTB = m_mapTalkBox.begin();
	for (; itTB != m_mapTalkBox.end(); ++itTB)
	{
		std::wstring wstrTemp = itTB->second.GetTalker(dwUID);
		if (wstrTemp.length())
			return wstrTemp;
	}

	return L"";
}

void GCNateOnChat::FocusClose()
{
	if (m_bEnableEditor)
	{
		if (m_pkTalkEditor != NULL)
		{
			m_pkTalkEditor->SetFocus(false);
		}
	}
}