#include "stdafx.h"
#include "GCAllCollection.h"
#include "GCVipCollectionDlg.h"
#include "GCCharCollectionDlg.h"

IMPLEMENT_CLASSNAME( KGCAllCollection );
IMPLEMENT_WND_FACTORY( KGCAllCollection );
IMPLEMENT_WND_FACTORY_NAME( KGCAllCollection, "gc_all_collection_dlg" );

KGCAllCollection::KGCAllCollection( void )
{
	LINK_CONTROL( "btn_close", m_pkClose );
	for ( int i = 0; i < 2; i++ )
	{
		char strTemp[ 50 ];
		sprintf( strTemp, "btn_tab%d", i );
		m_vecBtn[ i ] = NULL;
		LINK_CONTROL( strTemp, m_vecBtn[ i ] );
		sprintf( strTemp, "btn_tabon%d", i );
		m_vecBtnOn[ i ] = NULL;
		LINK_CONTROL( strTemp, m_vecBtnOn[ i ] );
	}
	LINK_CONTROL( "char_collection_dlg", m_pkCharCollection );
	LINK_CONTROL( "vip_collection_dlg", m_pkVipCollection );
}

KGCAllCollection::~KGCAllCollection( void )
{
}

void KGCAllCollection::ActionPerformed( const KActionEvent& event )
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
			if( m_pkCharCollection )
				m_pkCharCollection->ToggleRender( true );
			if( m_pkVipCollection )
				m_pkVipCollection->ToggleRender( false );
			return;
		}
		if ( event.m_pWnd == m_vecBtn[ 1 ] )
		{
			g_KDSound.Play( "31" );
			if( m_vecBtnOn[ 1 ] )
				m_vecBtnOn[ 1 ]->ToggleRender( true );
			if( m_vecBtnOn[ 0 ] )
				m_vecBtnOn[ 0 ]->ToggleRender( false );
			if( m_pkCharCollection )
				m_pkCharCollection->ToggleRender( false );
			if( m_pkVipCollection )
				m_pkVipCollection->ToggleRender( true );
			return;
		}
	}
}

void KGCAllCollection::OnCreate( void )
{
	if( m_pkClose )
	{	
		m_pkClose->InitState( true, true, this );
		m_pkClose->ConnectEventProcedure( D3DWE_BUTTON_CLICK, this, &KGCAllCollection::OnClose );
		m_pkClose->SetHotKey( DIK_ESCAPE );
	}
	for ( int i = 0; i < 2; i++ )
	{
		if( m_vecBtn[ i ] )
			m_vecBtn[ i ]->InitState( true, true, this );
		if( m_vecBtnOn[ i ] )
			m_vecBtnOn[ i ]->InitState( false, true, this );
	}
	if( m_pkCharCollection )
		m_pkCharCollection->InitState( true, true, this );
	if( m_pkVipCollection )
		m_pkVipCollection->InitState( false, true, this );
}

void KGCAllCollection::OnCreateComplete( void )
{
	if( m_vecBtnOn[ 0 ] )
		m_vecBtnOn[ 0 ]->ToggleRender( true );
}

void KGCAllCollection::OnClose()
{
	g_KDSound.Play( "31" );
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}