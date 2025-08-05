#include "stdafx.h"
#include "GCAgitEnterPass.h"
#include "GCStateGame.h"

IMPLEMENT_CLASSNAME( KGCAgitEnterPass );
IMPLEMENT_WND_FACTORY( KGCAgitEnterPass );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitEnterPass, "gc_agitenterpass" );

KGCAgitEnterPass::KGCAgitEnterPass( void )
: m_pkStaticDesc ( NULL ) 
, m_pkBtnOK( NULL ) 
, m_pkBtnCancel( NULL ) 
, m_pkEditPass( NULL ) 
, m_dwAgitID( UINT_MAX ) 
{
	LINK_CONTROL( "static_pass_desc", m_pkStaticDesc );
	LINK_CONTROL( "btn_ok",           m_pkBtnOK );
	LINK_CONTROL( "btn_cancel",       m_pkBtnCancel );
	LINK_CONTROL( "editPassword",     m_pkEditPass );
}

KGCAgitEnterPass::~KGCAgitEnterPass( void )
{
}

void KGCAgitEnterPass::OnCreate( void )
{
	m_pkBtnOK->InitState(true, true, this);
	m_pkBtnCancel->InitState(true, true, this);
	m_pkEditPass->InitState( true, false, this );
	m_pkEditPass->SetLimitText( MAX_AGIT_PASSWORD );
	m_pkEditPass->SetWndMode( D3DWM_LOCK );


	m_pkStaticDesc->InitState(true);
	m_pkStaticDesc->SetMultiLine(true);
	m_pkStaticDesc->SetAlign( DT_CENTER ); 
	m_pkStaticDesc->SetTextAutoMultiline(L"");
	m_pkStaticDesc->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_ENTER_PASS ) );

}

void KGCAgitEnterPass::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtnCancel,   D3DWE_BUTTON_CLICK, OnClose );
	GCWND_MSG_MAP( m_pkBtnOK,       D3DWE_BUTTON_CLICK, OnEnterAgit );
	GCWND_MSG_MAP( m_pkEditPass,	KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickEditPass );
}

void KGCAgitEnterPass::VisibleUI( DWORD dwAgitID )
{
	InitData();
	m_dwAgitID = dwAgitID;
	
	this->ToggleRender(true);	
}

void KGCAgitEnterPass::InitData() 
{
	m_dwAgitID = UINT_MAX;
	m_strPass.clear();
    if( m_pkEditPass )
	    m_pkEditPass->SetText( L"" );
}

void KGCAgitEnterPass::OnClose( void )
{
	this->ToggleRender(false);
}


void KGCAgitEnterPass::OnEnterAgit( void )
{
	if ( m_dwAgitID != UINT_MAX ) 
	{
		//	Enter Agit
		m_strPass = m_pkEditPass->GetText();

		g_pkAgitOverlayUI->CloseInviteVisitUI();
        if ( SiKGCPartyManager()->IsParty() )
            SiKGCPartyManager()->LeaveParty();
//		g_pStateGame->OnExitGame(); // 내아지트에서 나온 다음
		g_pStateGame->OnExitGame(); // 내아지트에서 나온 다음

		SiKGCAgitGameManager()->EnterAgit( m_dwAgitID, m_strPass ); // 이동~
	}

	this->ToggleRender(false);
}

void KGCAgitEnterPass::OnClickEditPass( void )
{
	m_pkEditPass->SetText( L"" );
	m_pkEditPass->SetFocus();
}
