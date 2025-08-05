#include "StdAfx.h"
#include "D3DSizingBoxBtn.h"

IMPLEMENT_CLASSNAME( KD3DSizingBoxBtn );

KD3DSizingBoxBtn::KD3DSizingBoxBtn(void)
: m_eChildMode( KD3DWnd::D3DWM_DEFAULT )
, m_pkSwapDef( NULL )
{
	for( int i = 0 ; i < NUM_WINDOW_MODE ; ++i )
	{
		m_pkStateBox[i] = NULL;
	}
	LINK_CONTROL( "def", m_pkStateBox[D3DWM_DEFAULT] );
	LINK_CONTROL( "hov", m_pkStateBox[D3DWM_HOVER] );
	LINK_CONTROL( "sel", m_pkStateBox[D3DWM_SELECT] );
	LINK_CONTROL( "act", m_pkStateBox[D3DWM_ACTIVE] );
	LINK_CONTROL( "loc", m_pkStateBox[D3DWM_LOCK] );
    LINK_CONTROL( "swapdef", m_pkSwapDef );
}

KD3DSizingBoxBtn::~KD3DSizingBoxBtn(void)
{
}

void KD3DSizingBoxBtn::OnCreate( void )
{
	for( int i = 0 ; i < NUM_WINDOW_MODE ; ++i )
	{
		if( m_pkStateBox[i] == NULL )
			continue;
		m_pkStateBox[i]->InitState( i == 0 ); 
	}
    if( m_pkSwapDef )
        m_pkSwapDef->InitState( false );
}

void KD3DSizingBoxBtn::OnSetHeight( DWORD dwHeight )
{
    for( int i = 0 ; i < NUM_WINDOW_MODE ; ++i ) {
        if( m_pkStateBox[i] == NULL )
            continue;
        m_pkStateBox[i]->SetHeight( dwHeight );
    }
    if( m_pkSwapDef )
        m_pkSwapDef->SetHeight( dwHeight );
}

void KD3DSizingBoxBtn::OnSetWidth( DWORD dwWidth )
{
    for( int i = 0 ; i < NUM_WINDOW_MODE ; ++i ) {
        if( m_pkStateBox[i] == NULL )
            continue;
        m_pkStateBox[i]->SetWidth( dwWidth );
    }
    if( m_pkSwapDef )
        m_pkSwapDef->SetWidth( dwWidth );
}

bool KD3DSizingBoxBtn::OnWndModeChange()
{
    for( int i = 0 ; i < NUM_WINDOW_MODE ; ++i )
    {
        if( m_pkStateBox[i] == NULL )
            continue;
        m_pkStateBox[i]->ToggleRender( i == GetWndMode() || i == 0 );
    }

    return true;
}

void KD3DSizingBoxBtn::FrameMoveInEnabledState()
{
    if( m_eChildMode != GetWndMode() ) {
        OnWndModeChange();
        m_eChildMode = GetWndMode();
    }
}

void KD3DSizingBoxBtn::SwapDef( bool bChange )
{
    if( m_pkSwapDef )
        m_pkSwapDef->ToggleRender( bChange );
}