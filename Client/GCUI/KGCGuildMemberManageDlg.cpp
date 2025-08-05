#include "StdAfx.h"
#include "KGCGuildManageDlg.h"
#include "KGCGuildMemberList.h"

IMPLEMENT_CLASSNAME( KGCGuildMemberManageDlg );
IMPLEMENT_WND_FACTORY( KGCGuildMemberManageDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildMemberManageDlg, "gc_guild_member_manage_dlg" );

KGCGuildMemberManageDlg::KGCGuildMemberManageDlg(void)
:m_pkGuildMemberList(NULL)
,m_pkBtnOk(NULL)
,m_pkBtnCancel(NULL)
{
	LINK_CONTROL("guild_member_list",			m_pkGuildMemberList );

    LINK_CONTROL("button_ok",					m_pkBtnOk );
    LINK_CONTROL("button_cancel",				m_pkBtnCancel );
}

KGCGuildMemberManageDlg::~KGCGuildMemberManageDlg(void)
{
}


void KGCGuildMemberManageDlg::OnCreate( void )
{
    m_pkGuildMemberList->InitState(true, true, this);
	m_pkGuildMemberList->SetMaxMemberNum(MAX_MEMBER_LIST);
    
	m_pkBtnOk->InitState(true, true, this);
	m_pkBtnCancel->InitState(true, true, this);
}

void KGCGuildMemberManageDlg::OnCreateComplete( void )
{
}

void KGCGuildMemberManageDlg::ActionPerformed( const KActionEvent& event )
{
	if( event.m_pWnd == m_pkBtnOk && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		if( !g_kGlobalValue.m_mapNGuildChangeMemberGrade.empty() )
		{
			std::map<char, std::set<DWORD>>::iterator mit = g_kGlobalValue.m_mapNGuildChangeMemberGrade.find((int)KNGuildUserInfo::GL_MASTER);
			if( mit != g_kGlobalValue.m_mapNGuildChangeMemberGrade.end() )
			{
				if( !mit->second.empty() )
				{
					g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, 
						g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MASTER_COMMITION) + g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MEMBER_COMMITION), 
						g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MASTER_COMMITION_MSG2), KGCUIScene::GC_MBOX_GUILD_MASTER_COMMTION, 
						KGCGuildPublicMbox::MBR_CONFIRM_CANCEL, 0, false, true );
				}				
			}
			else
			{
				KP2P::GetInstance()->Send_ChangeGuildMemberLevelReq(g_kGlobalValue.m_mapNGuildChangeMemberGrade);
			}
		}
		g_KDSound.Play( "31" );
	}

	if( event.m_pWnd == m_pkBtnCancel && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		SetMemberInfo();
		g_KDSound.Play( "31" );
	}
}

void KGCGuildMemberManageDlg::OnDestroyComplete( void )
{
	g_kGlobalValue.m_mapNGuildChangeMemberGrade.clear();
}

void KGCGuildMemberManageDlg::FrameMoveInEnabledState( void )
{
}

void KGCGuildMemberManageDlg::PostChildDraw( void )
{
}

void KGCGuildMemberManageDlg::SetMemberInfo()
{
	g_kGlobalValue.m_mapNGuildChangeMemberGrade.clear();
	m_pkGuildMemberList->SetCursorBoxOffset(D3DXVECTOR2(0.f,0.f));
	m_pkGuildMemberList->SetMaxMemberNum(MAX_MEMBER_LIST);
	m_pkGuildMemberList->SetGuildUserInfo();
	m_pkGuildMemberList->SetUsePopupMenu(true);
}