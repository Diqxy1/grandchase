#include "stdafx.h"
#include "GCRoomButton.h"

IMPLEMENT_CLASSNAME( KGCRoomButton );
IMPLEMENT_WND_FACTORY( KGCRoomButton );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomButton, "gc_room_button" );

KGCRoomButton::KGCRoomButton( void )
{
    m_dwWidthSave   = 0;
    m_dwHeightSave  = 0;
}

KGCRoomButton::~KGCRoomButton( void )
{
    // empty
}

void KGCRoomButton::OnCreate( void )
{
    m_dwWidthSave   = GetWidth();
    m_dwHeightSave  = GetHeight();
}

void KGCRoomButton::FrameMoveInEnabledState( void )
{
    switch ( GetWndMode() )
    {
        case KD3DWnd::D3DWM_HOVER:
        {
            SetCurrentWindowLocalPos( GetFixedWindowLocalPos() - D3DXVECTOR2( 1.0f, 1.0f ) );
            SetWidthHeight( m_dwWidthSave + 2, m_dwHeightSave + 2 );
            break;
        }
        default:
        {
            SetWidthHeight( m_dwWidthSave, m_dwHeightSave );
            break;
        }
    }
}