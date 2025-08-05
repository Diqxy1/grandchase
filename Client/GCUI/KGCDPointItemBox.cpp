#include "stdafx.h"
#include "KGCDPointItemBox.h"

IMPLEMENT_CLASSNAME( KGCDPointItemBox );
IMPLEMENT_WND_FACTORY( KGCDPointItemBox );
IMPLEMENT_WND_FACTORY_NAME( KGCDPointItemBox, "gc_dpoint_item_box" );

KGCDPointItemBox::KGCDPointItemBox(void)
: m_pkSelecter( NULL )
, m_pkImg(NULL)
, m_pkPrice(NULL)
, m_pkMarkCheck(NULL)
{
    LINK_CONTROL( "select", m_pkSelecter );
    LINK_CONTROL( "backdpoint", m_pkBackPrice );
    LINK_CONTROL( "img_shop", m_pkImg );
    LINK_CONTROL( "static_period", m_pkPrice );
    LINK_CONTROL( "mark_check", m_pkMarkCheck );
}

KGCDPointItemBox::~KGCDPointItemBox(void){}

void KGCDPointItemBox::OnCreate( void )
{
    if( m_pkSelecter )
        m_pkSelecter->InitState( false );

    if( m_pkBackPrice )
    {
        m_pkBackPrice->InitState( true );
        m_pkBackPrice->SetAllWindowAlpha( 100 );
    }

    if( m_pkImg )
    {
        m_pkImg->InitState( true );
        m_pkImg->ShowItem( true );
    }

    if( m_pkPrice )
    {
        m_pkPrice->SetFontColor( D3DCOLOR_ARGB(255, 213, 210, 174) );
        m_pkPrice->SetFontOutline( true, D3DCOLOR_ARGB(255, 17, 28, 44) );
        m_pkPrice->SetAlign( DT_LEFT );
        m_pkPrice->InitState( true );
    }

    if( m_pkMarkCheck )
        m_pkMarkCheck->InitState( false );
}

void KGCDPointItemBox::ActionPerformed( const KActionEvent& event )
{

}

void KGCDPointItemBox::FrameMoveInEnabledState( void )
{

}

void KGCDPointItemBox::SetSelectBox( bool bSelect )
{
    m_pkMarkCheck->ToggleRender( bSelect );
}

void KGCDPointItemBox::SetRollOverBox( bool bSelect )
{
    m_pkSelecter->ToggleRender( bSelect );
}

// 아이템 텍스쳐 랜더링 처리