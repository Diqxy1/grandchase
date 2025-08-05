#include "stdafx.h"
#include ".\gcsizingthreebox.h"

IMPLEMENT_CLASSNAME( KGCSizingThreeBox );
IMPLEMENT_WND_FACTORY( KGCSizingThreeBox );
IMPLEMENT_WND_FACTORY_NAME( KGCSizingThreeBox, "gc_sizing_three_box" );

KGCSizingThreeBox::KGCSizingThreeBox(void)
{
    m_pkLeft = NULL;
    m_pkMiddle = NULL;
    m_pkRight = NULL;

    LINK_CONTROL( "Left"    , m_pkLeft   );
    LINK_CONTROL( "Middle"  , m_pkMiddle );
    LINK_CONTROL( "Right"   , m_pkRight  );
}

KGCSizingThreeBox::~KGCSizingThreeBox(void)
{
}

void KGCSizingThreeBox::SetSize( DWORD dwWidth, DWORD dwHeight )
{
    m_pkLeft->SetHeight( dwHeight );
    m_pkMiddle->SetHeight( dwHeight );
    m_pkRight->SetHeight( dwHeight );

    m_pkMiddle->SetWidth( dwWidth );
    D3DXVECTOR2 pos = m_pkMiddle->GetFixedWindowLocalPos();
    pos.x += m_pkMiddle->GetWidth();
    m_pkRight->SetFixedWindowLocalPos( pos );
}

void KGCSizingThreeBox::OnSetHeight( DWORD dwHeight )
{
    m_pkLeft->SetHeight( dwHeight );
    m_pkMiddle->SetHeight( dwHeight );
    m_pkRight->SetHeight( dwHeight );
}

void KGCSizingThreeBox::OnSetWidth( DWORD dwWidth )
{
	int iTotalWidth = dwWidth;
	int iSideWidth = m_pkLeft->GetWidth() + m_pkRight->GetWidth();
	iTotalWidth -= iSideWidth;

	if( iTotalWidth <= 0 )
		dwWidth = iSideWidth;

	m_pkMiddle->SetWidthDirect( dwWidth - iSideWidth );
	m_pkRight->SetWindowPosDirect( m_pkMiddle->GetFixedWindowLocalPos() + D3DXVECTOR2( (float)m_pkMiddle->GetWidth(), 0.0f ) );
}
