#include "StdAfx.h"
#include "gcui/KGCPartyWindow.h"

IMPLEMENT_CLASSNAME( KGCPartyWindow );
IMPLEMENT_WND_FACTORY( KGCPartyWindow );
IMPLEMENT_WND_FACTORY_NAME( KGCPartyWindow, "gc_party" );

KGCPartyWindow::KGCPartyWindow(void)
: m_pkDungeonBtn ( NULL )
, m_pkMatchBtn ( NULL )
, m_pkPartyUserList( NULL )
{
    LINK_CONTROL( "dungeon_btn", m_pkDungeonBtn );
    LINK_CONTROL( "match_btn", m_pkMatchBtn );
    LINK_CONTROL( "party_user_list", m_pkPartyUserList );
}

KGCPartyWindow::~KGCPartyWindow(void)
{

}

void KGCPartyWindow::OnCreate()
{
    m_pkDungeonBtn->InitState( true , true, this );
    m_pkMatchBtn ->InitState( true , true, this );
    m_pkPartyUserList->InitState( true, true, this);
}

void KGCPartyWindow::OnCreateComplete( void )
{
    if ( m_pkDungeonBtn ) {
        m_pkDungeonBtn->InitState( true, true, this);
        m_pkDungeonBtn->DisconnectAllEventProcedure();
        m_pkDungeonBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPartyWindow::OnClickDungeon );
    }

    if ( m_pkMatchBtn ) {
        m_pkMatchBtn->InitState( true, true, this);
        m_pkMatchBtn->DisconnectAllEventProcedure();
        m_pkMatchBtn->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCPartyWindow::OnClickMatch );
    }

}

void KGCPartyWindow::ActionPerformed( const KActionEvent& event )
{

}

void KGCPartyWindow::OnClickDungeon( void )
{
	if ( g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP ) ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_AUTO_MATCH_ERR1) , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true , false);
		return;
	}

    //원하는 Messagebox를 부른다.
    if ( SiKGCPartyManager()->IsDungeonParty() )
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PARTY_DUNGEON , L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL , 0, 0, true, true, true );
    else
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_PARTY_DUNGEON_ERROR) , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true , false);
    
    g_KDSound.Play( "31" );
}

void KGCPartyWindow::OnClickMatch( void )
{
	if ( g_kGlobalValue.ServerInfo.CheckServerType( ST_DUNGEON ) ) {
		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_AUTO_MATCH_ERR0) , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true , false);
		return;
	}

	//원하는 Messagebox를 부른다.

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_PARTY_MATCH , L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL , 0, 0, true, true, false );
    g_KDSound.Play( "31" );
}

void KGCPartyWindow::FrameMoveInEnabledState( void )
{

}

void KGCPartyWindow::PostChildDraw( void )
{

}

void KGCPartyWindow::OnPartyDlg()
{
    ToggleRender(true);
    UpdatePartyDlg();

}

void KGCPartyWindow::OffPartyDlg()
{
    ToggleRender(false);
}

void KGCPartyWindow::UpdatePartyDlg()
{
    m_pkPartyUserList->ToggleRender(true);
    m_pkPartyUserList->SetPartyMember();
    m_pkDungeonBtn->Lock(!SiKGCPartyManager()->IsLocalPartyMasterUser());
    m_pkMatchBtn->Lock(!SiKGCPartyManager()->IsLocalPartyMasterUser());
}
