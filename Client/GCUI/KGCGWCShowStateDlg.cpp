//#include "stdafx.h"
//
//#include "KGCGWCShowStateDlg.h"
//
//IMPLEMENT_CLASSNAME( KGCGWCShowStateDlg );
//IMPLEMENT_WND_FACTORY( KGCGWCShowStateDlg );
//IMPLEMENT_WND_FACTORY_NAME( KGCGWCShowStateDlg, "gc_gwc_state_dlg" );
//
//KGCGWCShowStateDlg::KGCGWCShowStateDlg(void)
//{
//	m_pkBtnClose = NULL;
//	m_pkStaticDesc = NULL;
//
//	char str[256] = "";
//	for( int i = 0; i < MAXNUM-1; ++i)
//	{
//		m_pkStaticState[i] = NULL;
//		
//		sprintf( str, "static_nation%d", i );
//		LINK_CONTROL( str, m_pkStaticState[i] );
//	}
//
//	LINK_CONTROL("btn_close", m_pkBtnClose);
//	LINK_CONTROL("static_desc", m_pkStaticDesc);
//}
//
//KGCGWCShowStateDlg::~KGCGWCShowStateDlg(void)
//{
//
//}
//
//void KGCGWCShowStateDlg::OnCreate(void)
//{
//	m_pkBtnClose->InitState( true, true, this );
//	m_pkBtnClose->SetHotKey(DIK_ESCAPE);
//	m_pkStaticDesc->InitState( true );
//	for( int i = 0; i < MAXNUM-1; ++i)
//	{
//		m_pkStaticState[i]->SetAlign( DT_CENTER );
//		m_pkStaticState[i]->InitState( true);
//	}
//
//	m_pkStaticDesc->SetFontColor( 0xFF000000 );
//	m_pkStaticDesc->SetMultiLine( true );
//	m_pkStaticDesc->SetText( g_pkStrLoader->GetString( STR_ID_GWC_STATE_DESC ) );
//}
//
//void KGCGWCShowStateDlg::ActionPerformed( const KActionEvent & event)
//{
//	if( KD3DWnd::D3DWS_ENABLED != GetWndState() )
//		return;
//
//	GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnExit );
//}
//
//void KGCGWCShowStateDlg::OnExit()
//{
//	g_KDSound.Play( "31" );
//	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
//}
//
//void KGCGWCShowStateDlg::SetState( std::vector< KGWCSupportData> vecData )
//{
//	std::vector< KGWCSupportData >::iterator it;
//	for( it = vecData.begin() ; it != vecData.end(); ++it )
//	{
//		m_pkStaticState[it->m_nNationCode-1]->SetText( it->m_nAccCount );
//	}
//}