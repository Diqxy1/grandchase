#include "stdafx.h"
#include "D3DTab.h"

IMPLEMENT_CLASSNAME( KD3DTab );

KD3DTab::KD3DTab( void )
{
    // empty
}

KD3DTab::~KD3DTab( void )
{
    // empty
}

void KD3DTab::OnActivate( void )
{
    if ( IsActive() )
    {
        for ( UINT i = 0; i < m_vecChild.size(); ++i )
        {
            m_vecChild[i]->ToggleRender( true );
        }
    }
    else
    {
        for ( UINT i = 0; i < m_vecChild.size(); ++i )
        {
            m_vecChild[i]->ToggleRender( false );
        }
    }
}
