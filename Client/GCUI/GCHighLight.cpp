#include "stdafx.h"
#include ".\gcHighLight.h"
//
//

IMPLEMENT_CLASSNAME( KGCHighLight );
IMPLEMENT_WND_FACTORY( KGCHighLight );
IMPLEMENT_WND_FACTORY_NAME( KGCHighLight, "gc_highlight" );

KGCHighLight::KGCHighLight(void)
: m_dcARGB( D3DCOLOR_ARGB( 150, 0x00, 0x00, 0x00 ) )
, m_pkCenterRegion( NULL )
, m_pkOutLine( NULL )
, m_iAnimationFrame( 1 )
, m_iFrameCount( 0 )
{
	for( int i = 0 ; i < HL_NUM ; ++i )
	{
		m_pkDarkRegion[i] = NULL;
	}

	LINK_CONTROL( "left_top",		m_pkDarkRegion[HL_LEFT_TOP] );
	LINK_CONTROL( "top",			m_pkDarkRegion[HL_TOP] );
	LINK_CONTROL( "right_top",		m_pkDarkRegion[HL_RIGHT_TOP] );
	LINK_CONTROL( "left",			m_pkDarkRegion[HL_LEFT] );
	LINK_CONTROL( "center",			m_pkDarkRegion[HL_CENTER] );
	LINK_CONTROL( "right",			m_pkDarkRegion[HL_RIGHT] );
	LINK_CONTROL( "left_bottom",	m_pkDarkRegion[HL_LEFT_BOTTOM] );
	LINK_CONTROL( "bottom",			m_pkDarkRegion[HL_BOTTOM] );
	LINK_CONTROL( "right_bottom",	m_pkDarkRegion[HL_RIGHT_BOTTOM] );
	LINK_CONTROL( "inner_center",	m_pkCenterRegion );
	LINK_CONTROL( "outline",		m_pkOutLine );

	m_dwDarkRegionHeight = GC_SCREEN_HEIGHT;
	m_dwDarkRegionWidth = GC_SCREEN_WIDTH;

	m_vDarkRegionStartPos = D3DXVECTOR2(0,0);


}

KGCHighLight::~KGCHighLight(void)
{
}

void KGCHighLight::OnCreate()
{
	for( int i = 0 ; i < HL_NUM ; ++i )
	{
		if( m_pkDarkRegion[i] == NULL )
			continue;

		m_pkDarkRegion[i]->InitState( true );
		m_pkDarkRegion[i]->ForceSetWindowColor( m_dcARGB );

	}

	m_pkCenterRegion->InitState( true );
	m_pkCenterRegion->ForceSetWindowColor( m_dcARGB );
	m_pkCenterRegion->SetWindowPosDirect( m_pkDarkRegion[HL_CENTER]->GetFixedWindowLocalPos() );
	m_pkCenterRegion->SetSize( (int)m_pkDarkRegion[HL_CENTER]->GetWidth(), (int)m_pkDarkRegion[HL_CENTER]->GetHeight() );
	m_pkOutLine->InitState( true );
	m_pkOutLine->SetWindowPosDirect( m_pkDarkRegion[HL_CENTER]->GetFixedWindowLocalPos() );
	m_pkOutLine->SetSize( (int)m_pkDarkRegion[HL_CENTER]->GetWidth(), (int)m_pkDarkRegion[HL_CENTER]->GetHeight() );

	RefreshRegion(m_sCurrentRegion );
}

void KGCHighLight::SetHighLightRegion( float fx, float fy, DWORD dwHeight, DWORD dwWidth )
{
	m_sTargetRegion.m_vPos = D3DXVECTOR2( fx - m_vDarkRegionStartPos.x, fy - m_vDarkRegionStartPos.y );
	m_sTargetRegion.m_dwWidth = dwWidth;
	m_sTargetRegion.m_dwHeight = dwHeight;

	if( dwWidth == 0 )
	{
		m_sCurrentRegion = m_sTargetRegion;
		RefreshRegion( m_sTargetRegion );
	}
}

template< class T , class D >
void CompareAndChange( T& first, T& second, D delta )
{
	if( first > second && first - second > (T)delta )
	{
		first -= (T)delta;
	}
	else if( first < second && second - first > (T)delta )
	{
		first += (T)delta;
	}
	else
	{
		first = second;
	}
}

bool SGCHighLightRegion::operator <<(SGCHighLightRegion& arg )
{

	SettingDelta();
	CompareAndChange( m_vPos.x, arg.m_vPos.x, m_fDeltaForMovingHightLight );
	CompareAndChange( m_vPos.y, arg.m_vPos.y, m_fDeltaForMovingHightLight );
	CompareAndChange( m_dwWidth, arg.m_dwWidth, 20.0f );
	CompareAndChange( m_dwHeight, arg.m_dwHeight, 20.0f );
	return true;
}

void SGCHighLightRegion::SettingDelta()
{
	m_iCntFrame++;
	m_fDeltaForMovingHightLight =static_cast<float>(std::pow(m_iCntFrame,2));
}


void KGCHighLight::FrameMoveInEnabledState()
{
	if( !(m_sCurrentRegion == m_sTargetRegion) )
	{
		m_sCurrentRegion<<m_sTargetRegion;
		RefreshRegion(m_sCurrentRegion );
	}
	else
	{
		m_sCurrentRegion.m_iCntFrame = 0;
	}
}

void KGCHighLight::RefreshRegion( SGCHighLightRegion sRegion )
{
	DWORD dwPosX ,dwPosY;
	//소수점을 버르는 연산때문에 ui 간격이 벌어져 있음 보간을 위한 방법.
	dwPosX = static_cast<DWORD>(sRegion.m_vPos.x);
	dwPosY = static_cast<DWORD>(sRegion.m_vPos.y);
	if (dwPosX < sRegion.m_vPos.x) dwPosX+=1;
	if (dwPosY < sRegion.m_vPos.y) dwPosY+=1;


	m_pkDarkRegion[HL_LEFT_TOP]->SetWindowPosDirect( D3DXVECTOR2( m_vDarkRegionStartPos.x , m_vDarkRegionStartPos.y ) );
	m_pkDarkRegion[HL_LEFT_TOP]->SetWidthDirect( dwPosX );
	m_pkDarkRegion[HL_LEFT_TOP]->SetHeightDirect( dwPosY );

	m_pkDarkRegion[HL_TOP]->SetWindowPosDirect( D3DXVECTOR2( m_vDarkRegionStartPos.x + dwPosX, m_vDarkRegionStartPos.y ) );
	m_pkDarkRegion[HL_TOP]->SetWidthDirect( sRegion.m_dwWidth );
	m_pkDarkRegion[HL_TOP]->SetHeightDirect( dwPosY );

	m_pkDarkRegion[HL_RIGHT_TOP]->SetWindowPosDirect( D3DXVECTOR2(m_vDarkRegionStartPos.x + dwPosX + sRegion.m_dwWidth, m_vDarkRegionStartPos.y ) );
	m_pkDarkRegion[HL_RIGHT_TOP]->SetWidthDirect( m_dwDarkRegionWidth - ( dwPosX + sRegion.m_dwWidth ) );
	m_pkDarkRegion[HL_RIGHT_TOP]->SetHeightDirect( dwPosY );

	m_pkDarkRegion[HL_LEFT]->SetWindowPosDirect( D3DXVECTOR2( m_vDarkRegionStartPos.x,m_vDarkRegionStartPos.y+ dwPosY ) );
	m_pkDarkRegion[HL_LEFT]->SetWidthDirect( dwPosX );
	m_pkDarkRegion[HL_LEFT]->SetHeightDirect( sRegion.m_dwHeight );

	m_pkDarkRegion[HL_CENTER]->SetWindowPosDirect( D3DXVECTOR2(m_vDarkRegionStartPos.x+ dwPosX, m_vDarkRegionStartPos.y+dwPosY ) );
	m_pkDarkRegion[HL_CENTER]->SetWidthDirect( sRegion.m_dwWidth );
	m_pkDarkRegion[HL_CENTER]->SetHeightDirect( sRegion.m_dwHeight );

	m_pkDarkRegion[HL_RIGHT]->SetWindowPosDirect( D3DXVECTOR2( m_vDarkRegionStartPos.x+dwPosX + sRegion.m_dwWidth , m_vDarkRegionStartPos.y+dwPosY ) );
	m_pkDarkRegion[HL_RIGHT]->SetWidthDirect( m_dwDarkRegionWidth - ( dwPosX + sRegion.m_dwWidth ) );
	m_pkDarkRegion[HL_RIGHT]->SetHeightDirect( sRegion.m_dwHeight );

	m_pkDarkRegion[HL_LEFT_BOTTOM]->SetWindowPosDirect( D3DXVECTOR2( m_vDarkRegionStartPos.x, m_vDarkRegionStartPos.y+dwPosY + (float)sRegion.m_dwHeight ) );
	m_pkDarkRegion[HL_LEFT_BOTTOM]->SetWidthDirect( dwPosX );
	m_pkDarkRegion[HL_LEFT_BOTTOM]->SetHeightDirect( m_dwDarkRegionHeight - ( dwPosY + sRegion.m_dwHeight ) );

	m_pkDarkRegion[HL_BOTTOM]->SetWindowPosDirect( D3DXVECTOR2( m_vDarkRegionStartPos.x + dwPosX, m_vDarkRegionStartPos.y+dwPosY + (float)sRegion.m_dwHeight ) );
	m_pkDarkRegion[HL_BOTTOM]->SetWidthDirect( sRegion.m_dwWidth );
	m_pkDarkRegion[HL_BOTTOM]->SetHeightDirect( m_dwDarkRegionHeight - ( dwPosY + sRegion.m_dwHeight ) );

	m_pkDarkRegion[HL_RIGHT_BOTTOM]->SetWindowPosDirect( D3DXVECTOR2( m_vDarkRegionStartPos.x + dwPosX + (float)sRegion.m_dwWidth, m_vDarkRegionStartPos.y+dwPosY + (float)sRegion.m_dwHeight ) );
	m_pkDarkRegion[HL_RIGHT_BOTTOM]->SetWidthDirect( m_dwDarkRegionWidth - ( dwPosX + sRegion.m_dwWidth ) );
	m_pkDarkRegion[HL_RIGHT_BOTTOM]->SetHeightDirect( m_dwDarkRegionHeight - ( dwPosY + sRegion.m_dwHeight ) );

	m_pkCenterRegion->ToggleRender( sRegion.m_dwWidth != 0 );
	m_pkCenterRegion->SetWindowPosDirect( m_pkDarkRegion[HL_CENTER]->GetFixedWindowLocalPos());
	m_pkCenterRegion->SetSize( m_pkDarkRegion[HL_CENTER]->GetWidth(), m_pkDarkRegion[HL_CENTER]->GetHeight() );
	m_pkOutLine->ToggleRender( sRegion.m_dwWidth != 0 );
	m_pkOutLine->SetWindowPosDirect( m_pkDarkRegion[HL_CENTER]->GetFixedWindowLocalPos() );
	m_pkOutLine->SetSize( m_pkDarkRegion[HL_CENTER]->GetWidth(), m_pkDarkRegion[HL_CENTER]->GetHeight() );
}

void KGCHighLight::SetDarkReginon( DWORD dwWidth_ , DWORD dwHeight_ )
{
	m_dwDarkRegionWidth = dwWidth_;
	m_dwDarkRegionHeight = dwHeight_;

}

