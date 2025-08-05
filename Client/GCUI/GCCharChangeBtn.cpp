#include "stdafx.h"
#include "GCCharChangeBtn.h"

IMPLEMENT_CLASSNAME( KGCCharChangeBtn );
IMPLEMENT_WND_FACTORY( KGCCharChangeBtn );
IMPLEMENT_WND_FACTORY_NAME( KGCCharChangeBtn, "gc_char_change_btn" );

KGCCharChangeBtn::KGCCharChangeBtn( void )
{
    m_dwWidthSave   = 0;
    m_dwHeightSave  = 0;
}

KGCCharChangeBtn::~KGCCharChangeBtn( void )
{
    // empty
}

void KGCCharChangeBtn::OnCreate( void )
{
    m_dwWidthSave   = GetWidth();
    m_dwHeightSave  = GetHeight();
}

void KGCCharChangeBtn::FrameMoveInEnabledState( void )
{
    if ( KD3DWnd::D3DWM_SELECT == GetWndMode() )
    {
        SetCurrentWindowLocalPos( GetFixedWindowLocalPos() + D3DXVECTOR2( 1.0f, 1.0f ) );
        SetWidthHeight( m_dwWidthSave - 2, m_dwHeightSave - 1 );
    }
    else
    {
        SetWidthHeight( m_dwWidthSave, m_dwHeightSave );
    }
}