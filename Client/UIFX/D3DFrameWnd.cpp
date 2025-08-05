#include "stdafx.h"
#include "D3DFrameWnd.h"
//

IMPLEMENT_CLASSNAME( KD3DFrameWnd );

void KD3DFrameWnd::FrameMoveInEnabledState( void )
{
    D3DXVECTOR2 vMousePos( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

    if ( CheckPosInWindowBound( vMousePos ) )
    {
        if ( g_pkInput->BtnDown( KInput::MBLEFT ) )
        {
            m_vMousePosSave = vMousePos - GetCurrentWindowPos();
            m_bDrag = true;
            // TODO : activate window
        }
        else if ( g_pkInput->BtnUp( KInput::MBLEFT ) )
        {
            m_bDrag = false;
        }
    }

    if ( IsActive() && m_bDrag && g_pkInput->IsMouseMoving() &&
         g_pkInput->BtnPressed( KInput::MBLEFT ) )
    {
        SetWindowPos( vMousePos - m_vMousePosSave );
    }
}

void KD3DFrameWnd::OnCreateComplete( void )
{
    m_vInitPos = GetFixedWindowPos();
}

void KD3DFrameWnd::OnDestroyComplete( void )
{
    SetWindowPos( m_vInitPos );
}