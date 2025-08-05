#include "stdafx.h"
#include "KGCLoginEventDlg.h"
//

//

//

IMPLEMENT_CLASSNAME( KGCLoginEventDlg );
IMPLEMENT_WND_FACTORY( KGCLoginEventDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCLoginEventDlg, "login_event_dlg" );

KGCLoginEventDlg::KGCLoginEventDlg(void)
{
	LINK_CONTROL("ok_bt",m_pButtonOk);
	LINK_CONTROL("cancel_bt",m_pButtonCancel);
	LINK_CONTROL("edit_code",m_pEditCode);	
}

KGCLoginEventDlg::~KGCLoginEventDlg(void)
{
}

void KGCLoginEventDlg::ActionPerformed( const KActionEvent& event )
{
	
	switch ( event.m_dwCode )
	{
		case KD3DWnd::D3DWE_CURSOR_ENTER: g_KDSound.Play( "30" ); return;
		case KD3DWnd::D3DWE_BUTTON_DOWN:  g_KDSound.Play( "31" ); return;
	}

	GCWND_MSG_MAP( m_pEditCode, KD3DWnd::D3DWE_EDIT_RETURN, OnOK )
	GCWND_MSG_MAP( m_pButtonOk, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK )
	GCWND_MSG_MAP( m_pButtonCancel, KD3DWnd::D3DWE_BUTTON_CLICK, OnCancel )
}
void KGCLoginEventDlg::OnCreate( void )
{	
	m_pButtonOk->InitState( true, true, this );
	m_pButtonCancel->InitState( true, true, this );
	m_pEditCode->InitState( true, false, this );
}

void KGCLoginEventDlg::EndDialog( int iEndCode )
{
	m_iEndCode = iEndCode;
	ToggleRender( false );
	SpeakToActionListener( KActionEvent(this, KD3DWnd::D3DWE_DIALOG_END) );
}

void KGCLoginEventDlg::DoModal()
{
	ToggleRender(true);
	m_pEditCode->SetFocus();
}


void KGCLoginEventDlg::OnOK()
{
	EndDialog(EC_OK);
}

void KGCLoginEventDlg::OnCancel()
{
	EndDialog(EC_CANCEL);
}
