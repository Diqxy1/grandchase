#include "stdafx.h"
#include "KGCLoginErrorDlg.h"
//

//

//

IMPLEMENT_CLASSNAME( KGCLoginErrorDlg );
IMPLEMENT_WND_FACTORY( KGCLoginErrorDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCLoginErrorDlg, "login_error_dlg" );

KGCLoginErrorDlg::KGCLoginErrorDlg(void)
{
	LINK_CONTROL("ok_bt",	m_pButtonOK		);
	LINK_CONTROL("message",	m_pStaticMsg	);
} 

KGCLoginErrorDlg::~KGCLoginErrorDlg(void)
{
}

void KGCLoginErrorDlg::ActionPerformed( const KActionEvent& event )
{
	
	switch ( event.m_dwCode )
	{
		case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play( "30" ); return;
		case KD3DWnd::D3DWE_BUTTON_DOWN:  g_KDSound.Play( "31" ); return;
	}

	GCWND_MSG_MAP(m_pButtonOK, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK )	
	//if( event.m_pWnd == m_pEditID )
	//{
	//	if( event.m_dwCode == KD3DWnd::D3DWE_EDIT_RETURN || event.m_dwCode == KD3DWnd::D3DWE_EDIT_TAB)
	//		m_pEditPasswd->SetFocus();
	//	return;
	//}	
}
void KGCLoginErrorDlg::OnCreate( void )
{	
	m_pButtonOK->InitState( true, true, this );
	m_pStaticMsg->InitState( true, false );	
	m_pStaticMsg->SetMultiLine( true );
}

void KGCLoginErrorDlg::OnOK()
{
	SpeakToActionListener( KActionEvent(this, KD3DWnd::D3DWE_DIALOG_END ) );
}

void KGCLoginErrorDlg::SetMsg( const WCHAR* wcsMsg )
{
	m_pStaticMsg->SetText( wcsMsg );
}

