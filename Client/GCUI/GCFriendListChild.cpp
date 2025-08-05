#include "stdafx.h"
#include "GCFriendListChild.h"

IMPLEMENT_CLASSNAME( KGCFriendListChild );
IMPLEMENT_WND_FACTORY( KGCFriendListChild );
IMPLEMENT_WND_FACTORY_NAME( KGCFriendListChild, "gc_friend_list_child" );

KGCFriendListChild::KGCFriendListChild( void )
{
	m_pkFolderIcon = NULL;
	m_pkFolderBar = NULL;
	m_pkSelectBar = NULL;
	m_pkStaticName = NULL;
	LINK_CONTROL( "folder_icon",	m_pkFolderIcon );
	LINK_CONTROL( "bar_folder",		m_pkFolderBar );
	LINK_CONTROL( "bar_select",		m_pkSelectBar );
	LINK_CONTROL( "static_name",	m_pkStaticName );

	m_bGroupTitle = false;
	m_bGroupOpen = true;
	m_bSelected = false;
	m_iGroupId = -1;
	m_bOnline = false;
}

KGCFriendListChild::~KGCFriendListChild( void )
{
}

void KGCFriendListChild::OnCreate( void )
{
	m_pkFolderIcon->InitState( true );

	m_pkFolderBar->InitState( true );

	m_pkSelectBar->InitState( true );

	m_pkStaticName->InitState( true );
	m_pkStaticName->SetAlign( DT_LEFT );
}

void KGCFriendListChild::SetData( KFriendListData& kFriendData )
{
	m_bGroupTitle = kFriendData.m_bGroupTitle;

	if( m_bGroupTitle )
	{
		m_pkFolderIcon->ToggleRender( true );
		m_pkFolderBar->ToggleRender( true );
		m_pkSelectBar->ToggleRender( false );
		m_pkStaticName->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 255) );
		m_pkStaticName->SetText( kFriendData.m_strNickname );
		m_bGroupOpen = kFriendData.m_bGroupOpen;
		m_iGroupId = kFriendData.m_iGroupID;
		m_pkFolderIcon->SetWndMode( m_bGroupOpen ? D3DWM_HOVER : D3DWM_DEFAULT );
	}
	else
	{
		m_pkFolderIcon->ToggleRender( false );
		m_pkFolderBar->ToggleRender( false );
		SetSelection( kFriendData.m_bSelected );
		m_bOnline = kFriendData.m_bOnline; 

		OnCursor( false );
		m_pkStaticName->SetText( kFriendData.m_strNickname );
	}
}

void KGCFriendListChild::ActionPerformed( const KActionEvent& event )
{
}

void KGCFriendListChild::ToggleFolder( void )
{
	m_bGroupOpen = !m_bGroupOpen;
	m_pkFolderIcon->SetWndMode( m_bGroupOpen ? D3DWM_HOVER : D3DWM_DEFAULT );
}

void KGCFriendListChild::SetSelection( bool bSelected )
{
	m_bSelected = bSelected;
	m_pkSelectBar->ToggleRender( m_bSelected );
}

void KGCFriendListChild::OnCursor( bool bOnCursor )
{
	if( m_bGroupTitle )
		return;

	if( bOnCursor )
	{
		if( m_bOnline )
			m_pkStaticName->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 0) );
		else
			m_pkStaticName->SetFontColor( D3DCOLOR_ARGB(255, 192, 192, 0) );
	}
	else
	{
		if( m_bOnline )
			m_pkStaticName->SetFontColor( D3DCOLOR_ARGB(255, 255, 255, 255) );
		else
			m_pkStaticName->SetFontColor( D3DCOLOR_ARGB(255, 128, 128, 128) );
	}
}