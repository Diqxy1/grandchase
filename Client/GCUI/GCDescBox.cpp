#include "stdafx.h"
#include ".\gcdescbox.h"

IMPLEMENT_CLASSNAME( KGCDescBox );
IMPLEMENT_WND_FACTORY( KGCDescBox );
IMPLEMENT_WND_FACTORY_NAME( KGCDescBox, "gc_desc_box" );

KGCDescBox::KGCDescBox(void)
: m_pkTitle( NULL )
, m_pkDesc( NULL )
, m_pkLeft( NULL )
, m_pkRight( NULL )
, m_pkBackground( NULL )
, m_pkDescBack( NULL )
, m_pkMiddleDescBack( NULL )
{
	LINK_CONTROL( "title", m_pkTitle );
	LINK_CONTROL( "desc", m_pkDesc );
	LINK_CONTROL( "desc_middle", m_pkMiddleDesc );
	LINK_CONTROL( "Left_handle", m_pkLeft );
	LINK_CONTROL( "Right_handle", m_pkRight );
	LINK_CONTROL( "background", m_pkBackground );
	LINK_CONTROL( "desc_back", m_pkDescBack );
	LINK_CONTROL( "desc_middle_back", m_pkMiddleDescBack );
}

KGCDescBox::~KGCDescBox(void)
{
}

void KGCDescBox::OnCreate( void )
{
	m_pkMiddleDesc->SetLineSpace( 1.2f );
	m_pkDesc->SetLineSpace( 1.2f );
	SetAllWindowAlpha( 70 );
	m_pkDescBack->SetWindowColor( D3DCOLOR_ARGB( 30,255,255,255 ) );
	m_pkMiddleDescBack->SetWindowColor( D3DCOLOR_ARGB( 30,255,255,255 ) );
}

void KGCDescBox::SetTitle( std::wstring str )
{
	m_pkTitle->SetText( str );
}

void KGCDescBox::SetDesc( std::wstring str )
{
	m_pkDesc->SetTextAutoMultiline( str );
	m_pkDescBack->SetHeight( ( g_pkFontMgr->GetHeight( str.c_str() ) + 2 ) * m_pkDesc->GetLineNum() + 10 );

	RefreshSizeAndPosition();
}

void KGCDescBox::SetTargetPosition( D3DXVECTOR2 vPos, DWORD dwTargetWidth, DWORD dwTargetHeight )
{
	bool bRight = vPos.x > (GC_SCREEN_WIDTH / 2);
	m_pkLeft->ToggleRender( !bRight );
	m_pkRight->ToggleRender( bRight );

	RefreshSizeAndPosition();

	// 크기에 따른 윈도우 위치 보정
	//D3DXVECTOR2 vPos = GetFixedWindowLocalPos();
	m_pkLeft->SetWindowPosDirect( D3DXVECTOR2( m_pkLeft->GetFixedWindowLocalPos().x, (float)((GetHeight()-m_pkLeft->GetHeight()) / 2 ) ) );
	m_pkRight->SetWindowPosDirect( D3DXVECTOR2( m_pkRight->GetFixedWindowLocalPos().x, (float)((GetHeight()-m_pkRight->GetHeight()) / 2 ) ) );

	float fGab = (vPos.y + (float)(GetHeight() - dwTargetHeight/2) - GC_SCREEN_FLOAT_HEIGHT)*0.5f;
	if( fGab > 0.0f )
	{
		vPos.y -= fGab;
		m_pkLeft->SetWindowPosDirect( m_pkLeft->GetFixedWindowLocalPos() + D3DXVECTOR2(0, fGab + (float)m_pkLeft->GetHeight()/2.0f ) );
		m_pkRight->SetWindowPosDirect( m_pkRight->GetFixedWindowLocalPos() + D3DXVECTOR2(0, fGab + (float)m_pkRight->GetHeight()/2.0f ) );
	}
	else if( vPos.y + ((float)(GetHeight() - dwTargetHeight ) / -2.0f) < 0.0f )
	{
		fGab = vPos.y + ((float)(GetHeight() - dwTargetHeight ) / -2.0f);
		vPos.y = ((float)(GetHeight() - dwTargetHeight ) / 2.0f);

		m_pkLeft->SetWindowPosDirect( m_pkLeft->GetFixedWindowLocalPos() + D3DXVECTOR2(0, fGab + (float)m_pkLeft->GetHeight()/2.0f ) );
		m_pkRight->SetWindowPosDirect( m_pkRight->GetFixedWindowLocalPos() + D3DXVECTOR2(0, fGab + (float)m_pkRight->GetHeight()/2.0f ) );
	}


	D3DXVECTOR2 vPosTemp;
	vPosTemp.x = ( ( -1.0f * (float)GetWidth() * (float)bRight ) + (float)( dwTargetWidth * !bRight ));
	vPosTemp.y = ((float)(GetHeight() - dwTargetHeight ) / -2.0f);

	//  최종 결과물의 GlobalPos;
	vPosTemp = vPos + vPosTemp;


	//  부모의 GlobalPos를 빼면 LocalPos가 나온다.
	KD3DWnd* pParent = GetParent();
	if ( pParent ) 
	{
		vPosTemp -= pParent->GetCurrentWindowPos();
	}

	this->SetWindowPosDirect( vPosTemp  );

}

void KGCDescBox::SetMiddleDesc( std::wstring str )
{
	m_pkMiddleDesc->SetTextAutoMultiline( str );
	POINT ptSize = m_pkMiddleDesc->GetStringWidthAndHeightInPixel(false);
	m_pkMiddleDescBack->SetHeight(  ptSize.y + 10);

	RefreshSizeAndPosition();
}

void KGCDescBox::RefreshSizeAndPosition()
{
	// 하단 설명 공간의 위치 조정
	m_pkDesc->SetWindowPosDirect( D3DXVECTOR2( m_pkDesc->GetFixedWindowLocalPos().x, m_pkMiddleDescBack->GetFixedWindowLocalPos().y + (float)( m_pkMiddleDescBack->GetHeight() + 10 ) ) );
	m_pkDescBack->SetWindowPosDirect( D3DXVECTOR2( m_pkDescBack->GetFixedWindowLocalPos().x, m_pkMiddleDescBack->GetFixedWindowLocalPos().y + (float)( m_pkMiddleDescBack->GetHeight() + 3 ) ) );

	// 설명을 둘러싼 상자의 크기 조정
	int iBackGroundHeight = 0;
	iBackGroundHeight += m_pkMiddleDescBack->GetHeight();
	iBackGroundHeight += m_pkDescBack->GetHeight();
	iBackGroundHeight += 55;

	m_pkBackground->SetHeight( iBackGroundHeight );
	SetHeight( iBackGroundHeight );
}

