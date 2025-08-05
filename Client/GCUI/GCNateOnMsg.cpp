#include "stdafx.h"
#include ".\gcnateonmsg.h"
//
//
//
//
//
//
//

IMPLEMENT_CLASSNAME(GCNateOnMsg);
IMPLEMENT_WND_FACTORY(GCNateOnMsg);
IMPLEMENT_WND_FACTORY_NAME(GCNateOnMsg, "gc_nateon_msg");

GCNateOnMsg::GCNateOnMsg(void)
{
	m_pkTitleSend = NULL;
	m_pkTitleSended = NULL;
	m_pkTitleRecive = NULL;

	m_pkSend = NULL;
	m_pkDelete = NULL;
	m_pkReply = NULL;
	m_pkClose = NULL;

	m_pkReciver = NULL;
	m_pkMsgContent = NULL;
	m_pkOhterReciver = NULL;
	m_pkScrollBar = NULL;

	LINK_CONTROL("send_mode", m_pkTitleSend);
	LINK_CONTROL("sendremain_mode", m_pkTitleSended);
	LINK_CONTROL("recive_mode", m_pkTitleRecive);
	LINK_CONTROL("btn_send", m_pkSend);
	LINK_CONTROL("btn_delete", m_pkDelete);
	LINK_CONTROL("btn_reply", m_pkReply);
	LINK_CONTROL("btn_close", m_pkClose);
	LINK_CONTROL("reciver", m_pkReciver);
	LINK_CONTROL("msg_content", m_pkMsgContent);
	LINK_CONTROL("other_reciver", m_pkOhterReciver);
	LINK_CONTROL("scroll_msg", m_pkScrollBar);
}

GCNateOnMsg::~GCNateOnMsg(void)
{
}

void GCNateOnMsg::ActionPerformed(const KActionEvent& event)
{
	GCWND_MSG_MAP(m_pkSend, KD3DWnd::D3DWE_BUTTON_CLICK, OnSend);
	GCWND_MSG_MAP(m_pkDelete, KD3DWnd::D3DWE_BUTTON_CLICK, OnDelete);
	GCWND_MSG_MAP(m_pkReply, KD3DWnd::D3DWE_BUTTON_CLICK, OnReply);
	GCWND_MSG_MAP(m_pkClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose);
}

void GCNateOnMsg::OnCreate(void)
{
	m_pkTitleSend->InitState(true, true, this);
	m_pkTitleSended->InitState(false, true, this);
	m_pkTitleRecive->InitState(false, true, this);

	m_pkSend->InitState(true, true, this);
	m_pkDelete->InitState(false, true, this);
	m_pkReply->InitState(false, true, this);
	m_pkClose->InitState(true, true, this);

	m_pkReciver->InitState(true, true, this);
	m_pkMsgContent->InitState(true, true, this);
	m_pkOhterReciver->InitState(true, true, this);

	m_pkMsgContent->SetMultiLine(true);
	//m_pkScrollBar->SetScrollPageSize( m_pkMsgContent->GetTextDisplayLimit() );
	m_pkScrollBar->SetScrollRangeMax(0);
	m_pkScrollBar->SetScrollPos(0);

	SetMsgMode(NMM_SEND);
}

void GCNateOnMsg::OnSend()
{
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);

	std::set<DWORD> setRecvUIDs_;

	std::wstring wstrContent = m_pkMsgContent->GetText();
	KP2P::GetInstance()->m_kMsgServer->Send_SendPaperReq_S2(g_kGlobalValue.m_kUserInfo.strNickName, setRecvUIDs_, wstrContent, 0);
	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
}

void GCNateOnMsg::OnDelete()
{
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);

	std::set<DWORD> setDelUIDs_;
	setDelUIDs_.insert(m_CurMsg.first);

	KP2P::GetInstance()->m_kMsgServer->Send_DelRecvPaperReq_S2(setDelUIDs_);
	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
}

void GCNateOnMsg::OnReply()
{
	g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_NATEON_MESSAGE,
		L"", L"", KGCUIScene::GC_MBOX_USE_NATEON_MSG_SEND, 0, 0, true, false);

	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
}

void GCNateOnMsg::OnClose()
{
	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
}

void GCNateOnMsg::SetMsgMode(NATEON_MSG_MODE eMode)
{
	m_pkTitleSend->ToggleRender(false);
	m_pkTitleSended->ToggleRender(false);
	m_pkTitleRecive->ToggleRender(false);

	m_pkSend->ToggleRender(false);
	m_pkDelete->ToggleRender(false);
	m_pkReply->ToggleRender(false);

	switch (eMode)
	{
	case NMM_SEND:
		m_pkTitleSend->ToggleRender(true);
		m_pkSend->ToggleRender(true);
		break;
	case NMM_SENDED:
		m_pkTitleSended->ToggleRender(true);
		m_pkDelete->ToggleRender(true);
		break;
	case NMM_RECIVE:
		m_pkTitleRecive->ToggleRender(true);
		m_pkReply->ToggleRender(true);
		break;
	}
}

void GCNateOnMsg::SetCurMsg(DWORD dwUID)
{
	if (m_mapSendedMsg.count(dwUID) > 0)
	{
		m_CurMsg = std::make_pair(dwUID, m_mapSendedMsg[dwUID]);
	}
	else
	{
		m_CurMsg.first = 0;
	}

	m_pkMsgContent->SetText(m_CurMsg.second.m_strMessage.c_str());
}

void GCNateOnMsg::AddSendedMsg(DWORD dwUID, KPaper paper)
{
}

void GCNateOnMsg::AddReciveMsg(DWORD dwUID, KPaper paper)
{
}

bool GCNateOnMsg::InitDialog(IN const KDialogInfo& kInfo_)
{
	switch (kInfo_.m_nMsgBoxUse)
	{
	case KGCUIScene::GC_MBOX_USE_NATEON_MSG_SEND:
	{
		this->SetMsgMode(GCNateOnMsg::NMM_SEND);
	}
	break;
	case KGCUIScene::GC_MBOX_USE_NATEON_MSG_SENDED:
	{
		this->SetMsgMode(GCNateOnMsg::NMM_SENDED);
	}
	break;
	case KGCUIScene::GC_MBOX_USE_NATEON_MSG_RECIVE:
	{
		this->SetMsgMode(GCNateOnMsg::NMM_RECIVE);
	}
	break;
	}
	return true;
}