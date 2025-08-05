#include "stdafx.h"
#include "GCAgitSlot.h"

#include "GCAgitUserListCombo.h"

IMPLEMENT_CLASSNAME( KGCAgitSlot );
IMPLEMENT_WND_FACTORY( KGCAgitSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitSlot, "gc_agit_slot" );

KGCAgitSlot::KGCAgitSlot( void )
{
    m_pkDefaultFrame = NULL;
    m_pkActiveFrame = NULL;
    LINK_CONTROL( "mainframe", m_pkDefaultFrame );
    LINK_CONTROL( "activeFrame", m_pkActiveFrame );

    m_pkStaticAgitOwner = NULL;
    m_pkStaticAgitName = NULL;
    LINK_CONTROL( "staticAgitOwner", m_pkStaticAgitOwner );
    LINK_CONTROL( "staticAgitName",  m_pkStaticAgitName );

    m_pkUserListCombo = NULL;
    LINK_CONTROL( "userListCombo", m_pkUserListCombo );

    for( int i=0 ; i<NUM_ICON ; ++i )
        m_apkIcon[i] = NULL;

    LINK_CONTROL( "iconHide",   m_apkIcon[ ICON_CLOSE ] );
    LINK_CONTROL( "iconAll",    m_apkIcon[ ICON_OPEN ] );
    LINK_CONTROL( "iconFriend", m_apkIcon[ ICON_FRIEND ] );
    LINK_CONTROL( "iconCurSel", m_apkIcon[ ICON_CURSEL ] );

	m_bIsValid = false;
}

KGCAgitSlot::~KGCAgitSlot( void )
{
}

void KGCAgitSlot::OnCreate( void )
{
    m_pkDefaultFrame->InitState( true, false, NULL );
    m_pkActiveFrame->InitState( false, false, NULL );

    m_pkStaticAgitOwner->InitState( true, false, NULL );
    m_pkStaticAgitName->InitState( true, false, NULL );
    m_pkStaticAgitOwner->SetAlign( DT_LEFT );
    m_pkStaticAgitName->SetAlign( DT_LEFT );

    m_pkUserListCombo->InitState( true, true, this );

    for( int i=0 ; i<NUM_ICON ; ++i )
        m_apkIcon[i]->InitState( true, false, NULL );

    m_apkIcon[ ICON_CURSEL ]->ToggleRender( false );
}

void KGCAgitSlot::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkUserListCombo,   KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickCombo );
}

void KGCAgitSlot::FrameMoveInEnabledState( void )
{
    
}

void KGCAgitSlot::SetCurSel( bool bSet_ )
{
    m_apkIcon[ ICON_CURSEL ]->ToggleRender( bSet_ );
}

void KGCAgitSlot::SetAgitSlotInfo( KAgitPageInfo& info_ )
{
    m_kAgitInfo = info_;

    SetOpenType( static_cast<int>( m_kAgitInfo.m_ucOpenType ) );
    
   m_pkStaticAgitOwner->SetText( info_.m_strOwnerNick );
    
	m_pkStaticAgitName->SetText( m_kAgitInfo.m_strName );
    m_pkUserListCombo->SetUserList( m_kAgitInfo.m_ucMaxUser, m_kAgitInfo.m_mapUser );

	m_bIsValid = true;
}

void KGCAgitSlot::SetOpenType( int iType_ )
{
    if( iType_ < ICON_CLOSE || iType_ >= ICON_CURSEL )
        iType_ = ICON_CLOSE;

    for( int i=ICON_CLOSE ; i<ICON_CURSEL ; ++i )
    {
        if( i == iType_ )
            m_apkIcon[i]->ToggleRender( true );
        else
            m_apkIcon[i]->ToggleRender( false );
    }
}

void KGCAgitSlot::Clear( void )
{
    m_pkStaticAgitOwner->SetText( L"" );
    m_pkStaticAgitName->SetText( L"" );

    m_pkUserListCombo->Clear();

    KAgitPageInfo temp;
    m_kAgitInfo = temp;

	m_bIsValid = false;

	SetActive( false );
	SetCurSel( false );
	SetOpenType( ICON_CLOSE );
}

void KGCAgitSlot::OnClickCombo( void )
{
    m_pkUserListCombo->TogglePopup();
}

void KGCAgitSlot::SetActive( bool bActive_ )
{
    m_pkDefaultFrame->ToggleRender( !bActive_ );
    m_pkActiveFrame->ToggleRender( bActive_ );

    if( false == bActive_ )
        m_pkUserListCombo->SetPopupBoxRender( false );
 
    m_pkUserListCombo->SetActive( bActive_ );
}

void KGCAgitSlot::VisibleUserComboList(  bool bIsVisible )
{
    m_pkUserListCombo->ToggleRender( bIsVisible );
}