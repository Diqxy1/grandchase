#include "stdafx.h"
//#include "D3DSizingBox.h"

IMPLEMENT_CLASSNAME( KD3DSizingBox );
IMPLEMENT_WND_FACTORY( KD3DSizingBox );
IMPLEMENT_WND_FACTORY_NAME( KD3DSizingBox, "gc_sizingbox" );

KD3DSizingBox::KD3DSizingBox(void)
{
	LINK_CONTROL("center",		m_pBoxComponent[SBC_CENTER] );
	LINK_CONTROL("left",		m_pBoxComponent[SBC_LEFT] );
	LINK_CONTROL("right",		m_pBoxComponent[SBC_RIGHT] );
	LINK_CONTROL("top",			m_pBoxComponent[SBC_TOP] );
	LINK_CONTROL("bottom",		m_pBoxComponent[SBC_BOTTOM] );
	LINK_CONTROL("left_top",		m_pBoxComponent[SBC_LEFT_TOP] );
	LINK_CONTROL("left_bottom",	m_pBoxComponent[SBC_LEFT_BOTTOM] );
	LINK_CONTROL("right_top",	m_pBoxComponent[SBC_RIGHT_TOP] );
	LINK_CONTROL("right_bottom",	m_pBoxComponent[SBC_RIGHT_BOTTOM] );

    SetChildLock( true );
}

KD3DSizingBox::~KD3DSizingBox(void)
{
}

void KD3DSizingBox::OnCreate( void )
{	
    DWORD dwWidth = 0;
    DWORD dwHeight = 0;
    dwWidth += m_pBoxComponent[SBC_LEFT]->GetWidth();
    dwWidth += m_pBoxComponent[SBC_RIGHT]->GetWidth();
    dwWidth += m_pBoxComponent[SBC_TOP]->GetWidth();
    dwHeight += m_pBoxComponent[SBC_LEFT]->GetHeight();
    dwHeight += m_pBoxComponent[SBC_TOP]->GetHeight();
    dwHeight += m_pBoxComponent[SBC_BOTTOM]->GetHeight();
    SetWidthDirect( dwWidth );
    SetHeightDirect( dwHeight );
}

void KD3DSizingBox::SetSize(DWORD iWidth, DWORD iHeight)
{
	iWidth += (iWidth % 2);		//짝수를 맞춰주자~!
	iHeight += (iHeight % 2);

	DWORD dwEdgeWidth = m_pBoxComponent[SBC_LEFT_TOP]->GetWidth() + m_pBoxComponent[SBC_RIGHT_TOP]->GetWidth();
	DWORD dwEdgeHeight = m_pBoxComponent[SBC_LEFT_TOP]->GetHeight() + m_pBoxComponent[SBC_LEFT_BOTTOM]->GetHeight();

    if( iWidth < dwEdgeWidth )
    {
        //이렇게 되면 DWORD에 음수가 들어가게 되고 MAX값을 가지게 된다.
        iWidth = dwEdgeWidth;
    }

    if( iHeight < dwEdgeHeight )
    {
        iHeight = dwEdgeHeight;
    }

	DWORD dwWidth = iWidth - dwEdgeWidth;
	DWORD dwHeight = iHeight - dwEdgeHeight;

	//dwEdgeWidth -= 1;
	//dwEdgeHeight -= 1;

	D3DXVECTOR2 wndpos;
	D3DXVECTOR2 pivot;

	// 'SBC_TOP'	
	wndpos = m_pBoxComponent[SBC_LEFT_TOP]->GetFixedWindowLocalPos();
	wndpos.x += m_pBoxComponent[SBC_LEFT_TOP]->GetWidth();
	m_pBoxComponent[SBC_TOP]->SetWindowPosDirect( wndpos );
	m_pBoxComponent[SBC_TOP]->SetWidthDirect( dwWidth );

	// 'SBC_RIGHT_TOP'
	wndpos.x += dwWidth;
	m_pBoxComponent[SBC_RIGHT_TOP]->SetWindowPosDirect( wndpos );

	// SBC_LEFT
	wndpos = m_pBoxComponent[SBC_LEFT_TOP]->GetFixedWindowLocalPos();
	wndpos.y += m_pBoxComponent[SBC_LEFT_TOP]->GetHeight();
	m_pBoxComponent[SBC_LEFT]->SetHeightDirect( dwHeight );
	m_pBoxComponent[SBC_LEFT]->SetWindowPosDirect( wndpos );

	// SBC_CENTER
	wndpos = m_pBoxComponent[SBC_LEFT]->GetFixedWindowLocalPos();
	wndpos.x += m_pBoxComponent[SBC_LEFT]->GetWidth();
	m_pBoxComponent[SBC_CENTER]->SetWindowPosDirect( wndpos );
	m_pBoxComponent[SBC_CENTER]->SetWidthDirect( dwWidth );
	m_pBoxComponent[SBC_CENTER]->SetHeightDirect( dwHeight );

	// SBC_RIGHT
	wndpos = m_pBoxComponent[SBC_LEFT]->GetFixedWindowLocalPos();
	wndpos.x += dwWidth + m_pBoxComponent[SBC_LEFT]->GetWidth();
	m_pBoxComponent[SBC_RIGHT]->SetWindowPosDirect( wndpos );
	m_pBoxComponent[SBC_RIGHT]->SetHeightDirect( dwHeight );

	// SBC_LEFT_BOTTOM
	wndpos = m_pBoxComponent[SBC_LEFT_TOP]->GetFixedWindowLocalPos();
	wndpos.y += dwHeight + m_pBoxComponent[SBC_LEFT_TOP]->GetHeight();
	m_pBoxComponent[SBC_LEFT_BOTTOM]->SetWindowPosDirect( wndpos );

	// SBC_BOTTOM,
	wndpos.x += m_pBoxComponent[SBC_LEFT_TOP]->GetWidth();
	m_pBoxComponent[SBC_BOTTOM]->SetWindowPosDirect( wndpos );
	m_pBoxComponent[SBC_BOTTOM]->SetWidthDirect( dwWidth );

	// SBC_RIGHT_BOTTOM,
	wndpos.x += dwWidth;
	m_pBoxComponent[SBC_RIGHT_BOTTOM]->SetWindowPosDirect( wndpos );

	SetWidthDirect(iWidth);
	SetHeightDirect(iHeight);
}

void KD3DSizingBox::OnSetWidth( DWORD dwWidth )
{
    //2007.11.14 코드 중복이라 생각되어 삭제
	SetSize( dwWidth, this->GetHeight() );
	SetWidthDirect( dwWidth );
}

void KD3DSizingBox::OnSetHeight( DWORD dwHeight )
{
    SetSize( this->GetWidth(), dwHeight );
	SetHeightDirect( dwHeight );
}

void KD3DSizingBox::SetSizeWithoutEdge( int iWidth, int iHeight )
{
	iWidth += m_pBoxComponent[SBC_LEFT]->GetWidth() + m_pBoxComponent[SBC_RIGHT]->GetWidth();
	iHeight += m_pBoxComponent[SBC_TOP]->GetHeight() + m_pBoxComponent[SBC_BOTTOM]->GetHeight();
	SetSize( iWidth , iHeight );
}

void KD3DSizingBox::ForceSetWindowColor( D3DCOLOR dwWndColor )
{
	KD3DWnd::ForceSetWindowColor( dwWndColor );
	for( int i = 0 ; i < SIZINGBOX_COMPONENT_NUM ; ++i )
	{
		m_pBoxComponent[i]->ForceSetWindowColor( dwWndColor );
	}
}

void KD3DSizingBox::SetWidthWithoutEdge( int iWidth )
{
    iWidth += m_pBoxComponent[SBC_LEFT]->GetWidth() + m_pBoxComponent[SBC_RIGHT]->GetWidth();
    SetWidth( iWidth );
}

void KD3DSizingBox::SetHeightWithoutEdge( int iHeight )
{
    iHeight += m_pBoxComponent[SBC_TOP]->GetHeight() + m_pBoxComponent[SBC_BOTTOM]->GetHeight();
    SetHeight( iHeight );
}

void KD3DSizingBox::SetPosWithoutEdge( D3DXVECTOR2 vPos_ )
{
	vPos_.x -= m_pBoxComponent[SBC_LEFT]->GetWidth();
	vPos_.y -= m_pBoxComponent[SBC_TOP]->GetHeight();
	SetWindowPosDirect(vPos_);

}
