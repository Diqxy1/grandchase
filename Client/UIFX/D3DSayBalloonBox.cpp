#include "stdafx.h"
#include "D3DSayBalloonBox.h"

IMPLEMENT_CLASSNAME( KD3DBalloonBox );
IMPLEMENT_WND_FACTORY( KD3DBalloonBox );
IMPLEMENT_WND_FACTORY_NAME( KD3DBalloonBox, "gc_balloon_box" );

KD3DBalloonBox::KD3DBalloonBox(void) 
{
    LINK_CONTROL("center",		m_pBoxComponent[BBC_CENTER] );
    LINK_CONTROL("left",		m_pBoxComponent[BBC_LEFT] );
    LINK_CONTROL("right",		m_pBoxComponent[BBC_RIGHT] );
    LINK_CONTROL("top",			m_pBoxComponent[BBC_TOP] );
    LINK_CONTROL("bottom",		m_pBoxComponent[BBC_BOTTOM] );
    LINK_CONTROL("left_top",		m_pBoxComponent[BBC_LEFT_TOP] );
    LINK_CONTROL("left_bottom",	m_pBoxComponent[BBC_LEFT_BOTTOM] );
    LINK_CONTROL("left_bottom2",	m_pBoxComponent[BBC_LEFT_BOTTOM_ARROW] );
    LINK_CONTROL("right_top",	m_pBoxComponent[BBC_RIGHT_TOP] );
    LINK_CONTROL("right_bottom",	m_pBoxComponent[BBC_RIGHT_BOTTOM] );
    LINK_CONTROL("right_bottom2",	m_pBoxComponent[BBC_RIGHT_BOTTOM_ARROW] );
}

KD3DBalloonBox::~KD3DBalloonBox(void)
{
}

void KD3DBalloonBox::OnCreate( void )
{	
    m_eType = EAT_NONE;

	DWORD dwWidth = 0;
	DWORD dwHeight = 0;
	dwWidth += m_pBoxComponent[BBC_LEFT]->GetWidth();
	dwWidth += m_pBoxComponent[BBC_RIGHT]->GetWidth();
	dwWidth += m_pBoxComponent[BBC_TOP]->GetWidth();
	dwHeight += m_pBoxComponent[BBC_CENTER]->GetHeight();
	dwHeight += m_pBoxComponent[BBC_TOP]->GetHeight();
	dwHeight += m_pBoxComponent[BBC_BOTTOM]->GetHeight();
	SetWidthDirect( dwWidth );
	SetHeightDirect( dwHeight );
    ChangeLeftArrow(true);
    ChangeRightArrow(false);
}


void KD3DBalloonBox::SetSize(int iWidth, int iHeight)
{
    iWidth += (iWidth % 2);		//짝수를 맞춰주자~!

    DWORD dwEdgeWidth =  0; //m_pBoxComponent[BBC_LEFT_TOP]->GetWidth() + m_pBoxComponent[BBC_RIGHT_TOP]->GetWidth();
    DWORD dwEdgeHeight = 0; //높이는 위아래꺼 빼지 않는다.

    //m_pBoxComponent[BBC_LEFT_TOP]->GetHeight() + m_pBoxComponent[BBC_LEFT_BOTTOM]->GetHeight();

    if( iWidth < (int)(dwEdgeWidth) )
    {
        //이렇게 되면 DWORD에 음수가 들어가게 되고 MAX값을 가지게 된다.
        iWidth = dwEdgeWidth;
    }

    if( iHeight < (int)(dwEdgeHeight) )
    {
        iHeight = dwEdgeHeight;
    }

    DWORD dwWidth = iWidth - dwEdgeWidth;
    DWORD dwHeight = iHeight - dwEdgeHeight;

    //iWidth += dwEdgeWidth;

    //dwEdgeWidth -= 1;
    //dwEdgeHeight -= 1;

    D3DXVECTOR2 wndpos;
    D3DXVECTOR2 pivot;

    // 'BBC_TOP'	
    wndpos = m_pBoxComponent[BBC_LEFT_TOP]->GetFixedWindowLocalPos();
    wndpos.x += m_pBoxComponent[BBC_LEFT_TOP]->GetWidth();
    m_pBoxComponent[BBC_TOP]->SetWindowPosDirect( wndpos );
    m_pBoxComponent[BBC_TOP]->SetWidthDirect( dwWidth );

    // 'BBC_RIGHT_TOP'
    wndpos.x += dwWidth;
    m_pBoxComponent[BBC_RIGHT_TOP]->SetWindowPosDirect( wndpos );

    // BBC_LEFT
    wndpos = m_pBoxComponent[BBC_LEFT_TOP]->GetFixedWindowLocalPos();
    wndpos.y += m_pBoxComponent[BBC_LEFT_TOP]->GetHeight();
    m_pBoxComponent[BBC_LEFT]->SetHeightDirect( dwHeight );
    m_pBoxComponent[BBC_LEFT]->SetWindowPosDirect( wndpos );

    // BBC_CENTER
    wndpos = m_pBoxComponent[BBC_LEFT]->GetFixedWindowLocalPos();
    wndpos.x += m_pBoxComponent[BBC_LEFT]->GetWidth();
    m_pBoxComponent[BBC_CENTER]->SetWindowPosDirect( wndpos );
    m_pBoxComponent[BBC_CENTER]->SetWidthDirect( dwWidth );
    m_pBoxComponent[BBC_CENTER]->SetHeightDirect( dwHeight );

    // BBC_RIGHT
    wndpos = m_pBoxComponent[BBC_LEFT]->GetFixedWindowLocalPos();
    wndpos.x += dwWidth + m_pBoxComponent[BBC_LEFT]->GetWidth();
    m_pBoxComponent[BBC_RIGHT]->SetWindowPosDirect( wndpos );
    m_pBoxComponent[BBC_RIGHT]->SetHeightDirect( dwHeight );

    // BBC_LEFT_BOTTOM
    wndpos = m_pBoxComponent[BBC_LEFT_TOP]->GetFixedWindowLocalPos();
    wndpos.y += dwHeight + m_pBoxComponent[BBC_LEFT_TOP]->GetHeight();
    m_pBoxComponent[BBC_LEFT_BOTTOM]->SetWindowPosDirect( wndpos );

    // BBC_LEFT_BOTTOM_ARROW
    m_pBoxComponent[BBC_LEFT_BOTTOM_ARROW]->SetWindowPosDirect( wndpos );

    // BBC_BOTTOM,
    wndpos.x += m_pBoxComponent[BBC_LEFT_TOP]->GetWidth();
    m_pBoxComponent[BBC_BOTTOM]->SetWindowPosDirect( wndpos );
    m_pBoxComponent[BBC_BOTTOM]->SetWidthDirect( dwWidth );

    // BBC_RIGHT_BOTTOM,
    wndpos.x += dwWidth;
    m_pBoxComponent[BBC_RIGHT_BOTTOM]->SetWindowPosDirect( wndpos );
    // BBC_RIGHT_BOTTOM_ARROW
    m_pBoxComponent[BBC_RIGHT_BOTTOM_ARROW]->SetWindowPosDirect( wndpos );

    SetWidthDirect(iWidth);
    SetHeightDirect(iHeight);
}

void KD3DBalloonBox::DeleteArrow()
{
    m_eType=EAT_NONE;
    m_pBoxComponent[BBC_LEFT_BOTTOM]->ToggleRender(true);
    m_pBoxComponent[BBC_LEFT_BOTTOM_ARROW]->ToggleRender(false);
    m_pBoxComponent[BBC_RIGHT_BOTTOM]->ToggleRender(true);
    m_pBoxComponent[BBC_RIGHT_BOTTOM_ARROW]->ToggleRender(false);
}

void KD3DBalloonBox::ChangeLeftArrow(bool bArrow)
{
    if (bArrow==true) m_eType=EAT_LEFT;
    m_pBoxComponent[BBC_LEFT_BOTTOM]->ToggleRender(!bArrow);
    m_pBoxComponent[BBC_LEFT_BOTTOM_ARROW]->ToggleRender(bArrow);
}

void KD3DBalloonBox::ChangeRightArrow(bool bArrow)
{
    if (bArrow==true) m_eType=EAT_RIGHT;

    m_pBoxComponent[BBC_RIGHT_BOTTOM]->ToggleRender(!bArrow);
    m_pBoxComponent[BBC_RIGHT_BOTTOM_ARROW]->ToggleRender(bArrow);
}

void KD3DBalloonBox::OnSetWidth( DWORD dwWidth )
{
    //2007.11.14 코드 중복이라 생각되어 삭제
    SetSize( dwWidth, this->GetHeight() );
}

void KD3DBalloonBox::OnSetHeight( DWORD dwHeight )
{
    SetSize( this->GetWidth(), dwHeight );
}