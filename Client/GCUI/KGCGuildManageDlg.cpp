#include "StdAfx.h"
#include "KGCGuildManageDlg.h"
#include "KGCGuildMemberList.h"

IMPLEMENT_CLASSNAME( KGCGuildManageDlg );
IMPLEMENT_WND_FACTORY( KGCGuildManageDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildManageDlg, "gc_guild_manage_dlg" );

KGCGuildManageDlg::KGCGuildManageDlg(void)
:m_pkBtnMemberManage(NULL)
,m_pkBtnSettingManage(NULL)
,m_pkBtnJoinManage(NULL)
,m_pkGuildMemberManageDlg(NULL)
,m_pkActiveMemberManage(NULL)
,m_pkActiveSettingManage(NULL)
,m_pkActiveJoinManage(NULL)
{
	LINK_CONTROL("guild_member_manage_dlg",		m_pkGuildMemberManageDlg );
	LINK_CONTROL("guild_join_manage_dlg",		m_pkGuildJoinManageDlg );
	LINK_CONTROL("guild_setting_change_dlg",	m_pkGuildSettingManageDlg );

    LINK_CONTROL("button_member_management",	m_pkBtnMemberManage );
    LINK_CONTROL("button_setting_management",	m_pkBtnSettingManage );
    LINK_CONTROL("button_join_management",		m_pkBtnJoinManage );

	LINK_CONTROL("active_member_management",	m_pkActiveMemberManage );
	LINK_CONTROL("active_setting_management",	m_pkActiveSettingManage );
	LINK_CONTROL("active_join_management",		m_pkActiveJoinManage );
}

KGCGuildManageDlg::~KGCGuildManageDlg(void)
{
}


void KGCGuildManageDlg::OnCreate( void )
{
    m_pkGuildMemberManageDlg->InitState(true, true, this);
	m_pkGuildJoinManageDlg->InitState(false, true, this);
	m_pkGuildSettingManageDlg->InitState(false, true, this);
    
	m_pkBtnMemberManage->InitState(true, true, this);
	m_pkBtnSettingManage->InitState(true, true, this);
	m_pkBtnJoinManage->InitState(true, true, this);

	m_pkActiveMemberManage->InitState(false);
	m_pkActiveSettingManage->InitState(false);
	m_pkActiveJoinManage->InitState(false);

	if( g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_GENERAL )
	{
		m_pkBtnSettingManage->Lock(true);
	}
}

void KGCGuildManageDlg::OnCreateComplete( void )
{
}

void KGCGuildManageDlg::ActionPerformed( const KActionEvent& event )
{
	if( event.m_pWnd == m_pkBtnMemberManage && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		OnSelectDlg(BTN_MEMBER_MANAGE);
		g_KDSound.Play( "31" );
	}

	if( event.m_pWnd == m_pkBtnSettingManage && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		OnSelectDlg(BTN_SETTING_MANAGE);
		g_KDSound.Play( "31" );
	}

	if( event.m_pWnd == m_pkBtnJoinManage && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		OnSelectDlg(BTN_JOIN_MANAGE);
		g_KDSound.Play( "31" );
	}
}

void KGCGuildManageDlg::OnDestroyComplete( void )
{
}

void KGCGuildManageDlg::FrameMoveInEnabledState( void )
{
}

void KGCGuildManageDlg::PostChildDraw( void )
{
}

void KGCGuildManageDlg::OnSelectDlg(EGUILD_MANAGE_BTN eGuildManageBtn_ )
{
	bool bMemberManage = false, bJoinManage = false, bSettingManage = false;

	switch(eGuildManageBtn_)
	{
	case BTN_JOIN_MANAGE:
		bJoinManage = true;
		m_pkGuildJoinManageDlg->SetJoinWaitingUserInfo(g_kGlobalValue.m_mapNGuildUserInfo);
		m_pkGuildJoinManageDlg->SetCursorBoxOffset(D3DXVECTOR2(0.f, 0.f));
		break;
	case BTN_SETTING_MANAGE:
		bSettingManage = true;
		m_pkGuildSettingManageDlg->InitGuildSettingInfo();
		break;
	case BTN_MEMBER_MANAGE:
	default:
		bMemberManage = true;
		m_pkGuildMemberManageDlg->SetMemberInfo();
		break;
	}

	// 길마만 길드원관리 및 길드설정변경 탭을 사용할 수 있다
	if( g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_MASTER )
	{
		m_pkBtnMemberManage->Lock(false);
		m_pkBtnSettingManage->Lock(false);
	}
	else
	{
		m_pkBtnMemberManage->Lock(true);
		m_pkBtnSettingManage->Lock(true);
	}

	m_pkGuildMemberManageDlg->ToggleRender(bMemberManage);
	m_pkActiveMemberManage->ToggleRender(bMemberManage);

	m_pkGuildSettingManageDlg->ToggleRender(bSettingManage);
	m_pkActiveSettingManage->ToggleRender(bSettingManage);

	m_pkGuildJoinManageDlg->ToggleRender(bJoinManage);
	m_pkActiveJoinManage->ToggleRender(bJoinManage);	
}

void KGCGuildManageDlg::UpdateAllManageDlg()
{
	m_pkGuildJoinManageDlg->SetJoinWaitingUserInfo(g_kGlobalValue.m_mapNGuildUserInfo);
	m_pkGuildSettingManageDlg->InitGuildSettingInfo();
	m_pkGuildMemberManageDlg->SetMemberInfo();
}