#include "stdafx.h"
#include "GCGachaSelectSlot.h"

IMPLEMENT_CLASSNAME( KGCGachaSelectSlot );
IMPLEMENT_WND_FACTORY( KGCGachaSelectSlot );
IMPLEMENT_WND_FACTORY_NAME( KGCGachaSelectSlot, "gc_gacha_select_slot" );

KGCGachaSelectSlot::KGCGachaSelectSlot( void )
: m_pkActiveBack( NULL )
, m_pkCardBack( NULL )
, m_pkCardFrame( NULL )
, m_pkActiveMark( NULL )
, m_pkStaticCardName( NULL )
, m_pkItemImg( NULL )
, m_PetID( 0 )
, m_iPetNameID( -1 )
, m_iGachaVer( -1 )
{
    LINK_CONTROL( "activeBack",     m_pkActiveBack );
    LINK_CONTROL( "cardBack",       m_pkCardBack );
    LINK_CONTROL( "cardFrame",      m_pkCardFrame );
    LINK_CONTROL( "activeMark",     m_pkActiveMark );
    LINK_CONTROL( "staticCardName", m_pkStaticCardName );
    LINK_CONTROL( "staticCardType", m_pkStaticType );
    LINK_CONTROL( "item_img",       m_pkItemImg );
}

KGCGachaSelectSlot::~KGCGachaSelectSlot( void )
{
}

void KGCGachaSelectSlot::OnCreate( void )
{
    m_pkActiveBack->InitState( false, false, this );
    m_pkCardBack->InitState( true, false, this );
    m_pkCardFrame->InitState( false, false, this );
    m_pkActiveMark->InitState( false, false, this );
    
    m_pkStaticCardName->InitState( true, false, this );
    m_pkStaticCardName->SetFontColor( FONT_COLOR_DEFAULT );
    m_pkStaticCardName->SetFontSize( SiKGCMultipleLanguages()->GetDefualtFontSize() );
    m_pkStaticCardName->SetAlign( DT_LEFT );

    m_pkStaticType->InitState(true, false, this);
    m_pkStaticType->SetFontColor(FONT_COLOR_DEFAULT);
    m_pkStaticType->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticType->SetAlign(DT_LEFT);

    m_pkItemImg->InitState( true, false, this );

    this->InitState( true, true, this );
}

void KGCGachaSelectSlot::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_ENTER ) { 
        SetMouseIn();
    }

    if( event.m_dwCode == KD3DWnd::D3DWE_CURSOR_LEAVE ) { 
        SetMouseOut();
    }

    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) { 
        if( m_PetID != 0 ) { 
            static_cast<KGCSuperGachaSelectWnd*>( this->GetParent() )->SetSelectSlot( m_PetID );
        }
    }
}

void KGCGachaSelectSlot::ClearSlot( void )
{
    m_PetID = 0;
    m_iPetNameID = -1;
    m_iGachaVer = -1;

    m_pkActiveBack->ToggleRender( false );
    m_pkCardFrame->ToggleRender( false );
    m_pkActiveMark->ToggleRender( false );

    m_pkStaticCardName->SetFontColor( FONT_COLOR_DEFAULT );
    m_pkStaticCardName->SetText( L"-" );

    m_pkStaticType->SetFontColor(FONT_COLOR_DEFAULT);
    m_pkStaticType->SetText(L"-");

    m_pkItemImg->ShowItem( false );
    m_pkItemImg->SetItemInfo( KItem::INVALID_ITEM );
}

void KGCGachaSelectSlot::SetGachaSelectSlotInfo( const GCITEMID petID_, const int iPetNameID_, const int iGachaVer_, const int iGachaType_ )
{
    ClearSlot();

    if( petID_ <= 0 || iPetNameID_ < 0 || iGachaVer_ < 0 ) { 
        return;
    }

    if( g_pItemMgr->GetItemData( petID_ ) == NULL ) { 
        return;
    }

    m_PetID = petID_;
    m_iPetNameID = iPetNameID_;
    m_iGachaVer = iGachaVer_;

    m_pkStaticCardName->SetText( g_pkStrLoader->GetString( m_iPetNameID ) );

    switch ( iGachaType_ ) {
        case 0:
            m_pkStaticType->SetText(g_pkStrLoader->GetString(STR_ID_EQUIP_NEW_SR_SLOT));
            break;
        case 1:
        case 2:
            m_pkStaticType->SetText(g_pkStrLoader->GetString(STR_ID_COORDI_CLOSET));
            break;
    }

    m_pkItemImg->SetItemInfo( m_PetID );
    m_pkItemImg->ShowItem( true );
}

void KGCGachaSelectSlot::SetMouseIn( void )
{
    if( m_PetID == 0 ) { 
        return;
    }

    m_pkActiveBack->ToggleRender( true );
    m_pkStaticCardName->SetFontColor( FONT_COLOR_ACTIVE );
}

void KGCGachaSelectSlot::SetMouseOut( void )
{
    if( m_PetID == 0 || m_pkActiveMark->IsRenderOn() ) { 
        return;
    }

    m_pkActiveBack->ToggleRender( false );
    m_pkStaticCardName->SetFontColor( FONT_COLOR_DEFAULT );
}

void KGCGachaSelectSlot::SetClick( const bool bSet_ )
{
    if( m_PetID == 0 ) { 
        return;
    }

    m_pkActiveBack->ToggleRender( bSet_ );
    m_pkActiveMark->ToggleRender( bSet_ );
    m_pkCardFrame->ToggleRender( bSet_ );
    m_pkStaticCardName->SetFontColor( bSet_ ? FONT_COLOR_ACTIVE : FONT_COLOR_DEFAULT );
}