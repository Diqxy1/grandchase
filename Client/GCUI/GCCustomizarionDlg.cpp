#include "stdafx.h"
#include "GCCustomizarionDlg.h"
#include "GCFontSkinDlg.h"
#include "GCRoomAnimationDlg.h"
#include "GCCutInSkinDlg.h"

IMPLEMENT_CLASSNAME( KGCCustomizationDlg );
IMPLEMENT_WND_FACTORY( KGCCustomizationDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCCustomizationDlg, "gc_customizationdlg" );

KGCCustomizationDlg::KGCCustomizationDlg( void )
{
	LINK_CONTROL( "btn_close", m_pkClose );
	for ( int i = 0; i < 3; i++ )
	{
		char strTemp[ 50 ];
		sprintf( strTemp, "btn_tab%d", i );
		m_vecBtn[ i ] = NULL;
		LINK_CONTROL( strTemp, m_vecBtn[ i ] );
		sprintf( strTemp, "btn_tabon%d", i );
		m_vecBtnOn[ i ] = NULL;
		LINK_CONTROL( strTemp, m_vecBtnOn[ i ] );
	}
	LINK_CONTROL( "fontskin_dlg", m_pkFontSkinDlg );
	LINK_CONTROL( "animation_dlg", m_pkAnimationDlg );
	LINK_CONTROL( "cutin_dlg", m_pkCutinSkinDlg );
}

KGCCustomizationDlg::~KGCCustomizationDlg( void )
{
}

void KGCCustomizationDlg::ActionPerformed( const KActionEvent& event )
{
	if ( event.m_dwCode == D3DWE_BUTTON_CLICK )
	{
		if ( event.m_pWnd == m_vecBtn[ 0 ] )
		{
			g_KDSound.Play( "31" );
			if( m_vecBtnOn[ 0 ] )
				m_vecBtnOn[ 0 ]->ToggleRender( true );
			if( m_vecBtnOn[ 1 ] )
				m_vecBtnOn[ 1 ]->ToggleRender( false );
			if( m_vecBtnOn[ 2 ] )
				m_vecBtnOn[ 2 ]->ToggleRender( false );
			if( m_pkFontSkinDlg )
				m_pkFontSkinDlg->ToggleRender( true );
			if( m_pkAnimationDlg )
				m_pkAnimationDlg->ToggleRender( false );
			if( m_pkCutinSkinDlg )
				m_pkCutinSkinDlg->ToggleRender( false );
			UpdateSubWindow( 0 );
			return;
		}
		else if ( event.m_pWnd == m_vecBtn[ 1 ] )
		{
			g_KDSound.Play( "31" );
			if( m_vecBtnOn[ 0 ] )
				m_vecBtnOn[ 0 ]->ToggleRender( false );
			if( m_vecBtnOn[ 1 ] )
				m_vecBtnOn[ 1 ]->ToggleRender( true );
			if( m_vecBtnOn[ 2 ] )
				m_vecBtnOn[ 2 ]->ToggleRender( false );
			if( m_pkFontSkinDlg )
				m_pkFontSkinDlg->ToggleRender( false );
			if( m_pkAnimationDlg )
				m_pkAnimationDlg->ToggleRender( true );
			if( m_pkCutinSkinDlg )
				m_pkCutinSkinDlg->ToggleRender( false );
			UpdateSubWindow( 1 );
			return;
		}
		else if ( event.m_pWnd == m_vecBtn[ 2 ] )
		{
			g_KDSound.Play( "31" );
			if( m_vecBtnOn[ 0 ] )
				m_vecBtnOn[ 0 ]->ToggleRender( false );
			if( m_vecBtnOn[ 1 ] )
				m_vecBtnOn[ 1 ]->ToggleRender( false );
			if( m_vecBtnOn[ 2 ] )
				m_vecBtnOn[ 2 ]->ToggleRender( true );
			if( m_pkFontSkinDlg )
				m_pkFontSkinDlg->ToggleRender( false );
			if( m_pkAnimationDlg )
				m_pkAnimationDlg->ToggleRender( false );
			if( m_pkCutinSkinDlg )
				m_pkCutinSkinDlg->ToggleRender( true );
			UpdateSubWindow( 2 );
			return;
		}
	}
}

void KGCCustomizationDlg::OnCreate( void )
{
	if( m_pkClose )
	{	
		m_pkClose->InitState( true, true, this );
		m_pkClose->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCCustomizationDlg::OnClose );
		m_pkClose->SetHotKey( DIK_ESCAPE );
	}
	for ( int i = 0; i < 3; i++ )
	{
		if( m_vecBtn[ i ] )
			m_vecBtn[ i ]->InitState( true, true, this );
		if( m_vecBtnOn[ i ] )
			m_vecBtnOn[ i ]->InitState( false, true, this );
	}
	if( m_pkFontSkinDlg )
		m_pkFontSkinDlg->InitState( true, true, this );
	if( m_pkAnimationDlg )
		m_pkAnimationDlg->InitState( false, true, this );
	if( m_pkCutinSkinDlg )
		m_pkCutinSkinDlg->InitState( false, true, this );
}

void KGCCustomizationDlg::OnCreateComplete( void )
{
	if( m_vecBtnOn[ 0 ] )
		m_vecBtnOn[ 0 ]->ToggleRender( true );
}

void KGCCustomizationDlg::OnClose()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCCustomizationDlg::UpdateSubWindow( int iOption )
{
	if ( iOption == 0 )
		m_pkFontSkinDlg->UpdateFontSkinItemList();
	else if ( iOption == 1 )
		m_pkAnimationDlg->UpdateRoomAnimationItemList();
	else if ( iOption == 2 )
		m_pkCutinSkinDlg->UpdateCutInSkinItemList();
}

void KGCCustomizationDlg::UpdateAllSubWindows()
{
	m_pkFontSkinDlg->UpdateFontSkinItemList();
	m_pkAnimationDlg->UpdateRoomAnimationItemList();
	m_pkCutinSkinDlg->UpdateCutInSkinItemList();
}

bool KGCCustomizationDlg::IsSubWindowActive( int iOption )
{
	if( iOption == 0 )
		return m_pkFontSkinDlg->IsRenderOn();
	if( iOption == 1 )
		return m_pkAnimationDlg->IsRenderOn();
	if( iOption == 2 )
		return m_pkCutinSkinDlg->IsRenderOn();
	return false;
}