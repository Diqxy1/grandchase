#include "stdafx.h"
#include ".\gcnateoninvitebuddy.h"
#include "GCInviteBuddyTree.h"
//

#include "GCTalkList.h"
//
//

IMPLEMENT_CLASSNAME(GCNateOnInviteBuddy);
IMPLEMENT_WND_FACTORY(GCNateOnInviteBuddy);
IMPLEMENT_WND_FACTORY_NAME(GCNateOnInviteBuddy, "gc_nateon_invite");

GCNateOnInviteBuddy::GCNateOnInviteBuddy(void)
{
	m_pkInvite = NULL;
	m_pkClose = NULL;
	m_pkBuddyTree = NULL;

	LINK_CONTROL("btn_invite", m_pkInvite);
	LINK_CONTROL("btn_close", m_pkClose);
	LINK_CONTROL("invite_buddy_tree", m_pkBuddyTree);
}

GCNateOnInviteBuddy::~GCNateOnInviteBuddy(void)
{
}

void GCNateOnInviteBuddy::OnCreate(void)
{
	m_pkInvite->SetHotKey(DIK_RETURN);
	m_pkClose->SetHotKey(DIK_ESCAPE);

	m_pkInvite->InitState(true, true, this);
	m_pkClose->InitState(true, true, this);
	m_pkBuddyTree->InitState(true, true, this);
}

void GCNateOnInviteBuddy::ActionPerformed(const KActionEvent& event)
{
	GCWND_MSG_MAP(m_pkInvite, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK);
	GCWND_MSG_MAP(m_pkClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnCancel);
}

void GCNateOnInviteBuddy::OnOK()
{
	JIF(KP2P::GetInstance()->m_kMsgServer != NULL);

	std::set<DWORD> setInviteList;
	m_pkBuddyTree->GetInviteList(setInviteList);

	if (setInviteList.size() > 0)
	{
		KP2P::GetInstance()->m_kMsgServer->Send_InviteChatMemberReq_S2(g_pkUIScene->m_pkNateOn->GetTalkListWindow()->GetCurTalkKey(), setInviteList);
	}

	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
}

void GCNateOnInviteBuddy::OnCancel()
{
	SpeakToActionListener(KActionEvent(this, EWNDMESSAGE_CLOSE));
}