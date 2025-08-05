#include "stdafx.h"
#include "GCMagicBoxItemWnd.h"

IMPLEMENT_CLASSNAME( KGCMagicBoxItemWnd );
IMPLEMENT_WND_FACTORY( KGCMagicBoxItemWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCMagicBoxItemWnd, "gc_magic_box_item_wnd" );

KGCMagicBoxItemWnd::KGCMagicBoxItemWnd( void )
: m_pkItemImg(NULL)   
, m_pkMouseOverImg(NULL)
, m_pkSizingBg(NULL)   
, m_pkBtnClose(NULL)
, m_pkStaticCount(NULL)
, m_ItemID(-1)
{
    LINK_CONTROL("item_img", m_pkItemImg );
    LINK_CONTROL("mouse_over_img", m_pkMouseOverImg );
    LINK_CONTROL("sizing_bg", m_pkSizingBg );
    LINK_CONTROL("icon", m_pkBtnClose );
    LINK_CONTROL("static_item_count", m_pkStaticCount );
}

KGCMagicBoxItemWnd::~KGCMagicBoxItemWnd( void )
{
}

void KGCMagicBoxItemWnd::ActionPerformed( const KActionEvent& event )
{

}

void KGCMagicBoxItemWnd::OnCreate( void )
{
    if ( m_pkItemImg ) {
        m_pkItemImg->InitState( true );
        m_pkItemImg->ShowItem( true );
    }
    if ( m_pkMouseOverImg ) {
        m_pkMouseOverImg->InitState( false );
    }

    if ( m_pkSizingBg ) {
    m_pkSizingBg->InitState( true );
    }
    if ( m_pkBtnClose) {
        m_pkBtnClose->InitState( true );
    }

    if ( m_pkStaticCount ) {
        m_pkStaticCount->InitState( true );
        m_pkStaticCount->SetStringType( "very_small_num", true );
    }
}

void KGCMagicBoxItemWnd::FrameMoveInEnabledState( void )
{

    if ( m_pkMouseOverImg ) {
        const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

        if ( m_pkMouseOverImg->CheckPosInWindowBound( vMousePos ) ) {
            m_pkMouseOverImg->ToggleRender( true );
        }
        else {
            m_pkMouseOverImg->ToggleRender( false );      
        }
    }
}

void KGCMagicBoxItemWnd::SetBlank()
{
    SetImage();
    RenderIcon();
    SetCount();
}


GCITEMID KGCMagicBoxItemWnd::GetItemID( )
{
    return m_ItemID;
}

void KGCMagicBoxItemWnd::SetImage( DWORD dwGoodsID_ )
{
    if ( m_pkItemImg == NULL )
        return;
    if ( dwGoodsID_ != 0 )
	{
        m_pkItemImg->ShowItem( true );
	    RenderIcon(true);
	}
    else
        m_pkItemImg->ShowItem( false );

    m_ItemID = dwGoodsID_;
    m_pkItemImg->SetItemInfo(m_ItemID);

}

void KGCMagicBoxItemWnd::RenderIcon( bool _bRender )
{
    if ( m_pkBtnClose ) {
        m_pkBtnClose->ToggleRender( _bRender );
    }
}


void KGCMagicBoxItemWnd::SetCount( int nCount_ )
{
    if ( nCount_ == -1 ) {
        m_pkStaticCount->ToggleRender( false );
    }
    else {
        m_pkStaticCount->ToggleRender( true );
        m_pkStaticCount->SetNumber( nCount_ );
    }
}