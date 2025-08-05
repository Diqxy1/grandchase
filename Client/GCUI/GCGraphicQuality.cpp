#include "stdafx.h"
#include "GCGraphicQuality.h"
//

#include "../MyD3D.h"

IMPLEMENT_CLASSNAME( KGCGraphicQuality );
IMPLEMENT_WND_FACTORY( KGCGraphicQuality );
IMPLEMENT_WND_FACTORY_NAME( KGCGraphicQuality, "gc_graphic_quality" );

KGCGraphicQuality* g_pGraphicQuality = NULL;

static int s_aiQualityOptionStrID[] =
{
	STR_ID_OPTION_QUALITY_VERY_BAD,
	STR_ID_OPTION_QUALITY_BAD,
	STR_ID_OPTION_QUALITY_NORMAL,
	STR_ID_OPTION_QUALITY_GOOD,
	STR_ID_OPTION_QUALITY_GREAT,
	STR_ID_OPTION_QUALITY_VERY_GREAT,
	STR_ID_OPTION_QUALITY_BEST,
};

KGCGraphicQuality::KGCGraphicQuality( void )
{
	g_pGraphicQuality = this;
	m_pkThumb = NULL;
	for( int i=0 ; i<MAX_QUALITY ; ++i )
	{
		m_ppkGage[i] = NULL;
		
		char strName[MAX_PATH];
		sprintf( strName, "gage%d", i+1 );
		LINK_CONTROL( strName, m_ppkGage[i] );
	}

	LINK_CONTROL( "gage_btn", m_pkThumb );

	m_pkBtnMinus = NULL;
	LINK_CONTROL ( "minus", m_pkBtnMinus );

	m_pkBtnMajor = NULL;
	LINK_CONTROL ( "plus", m_pkBtnMajor );

	m_iGraphicQuality = 0;
	m_bDrag = false;
}

KGCGraphicQuality::~KGCGraphicQuality( void )
{
}

void KGCGraphicQuality::OnCreate( void )
{
	for( int i=0 ; i<MAX_QUALITY ; ++i )
		m_ppkGage[i]->SetSelfInputCheck( true );

	m_pkThumb->SetSelfInputCheck( true );

	m_pkBtnMinus->InitState( true, true, this );
	m_pkBtnMajor->InitState( true, true, this );

	SetQuality( g_MyD3D->m_KGCOption.GetQualityCount() );
}

void KGCGraphicQuality::ActionPerformed( const KActionEvent& event )
{
	if(event.m_pWnd == m_pkBtnMinus && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {
		//SetQuality(m_iGraphicQuality - 1);
		if( m_iGraphicQuality > 0 )
			g_MyD3D->m_KGCOption.Quality_Change( - 1 );
	}

	if(event.m_pWnd == m_pkBtnMajor && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ) {
		//SetQuality(m_iGraphicQuality + 1);
		if( m_iGraphicQuality < MAX_QUALITY )
			g_MyD3D->m_KGCOption.Quality_Change( 1 );
	}

	SetQuality( g_MyD3D->m_KGCOption.GetQualityCount() );
}

void KGCGraphicQuality::FrameMoveInEnabledState(void)
{
	POINT pt = g_pkInput->GetMousePos();

	D3DXVECTOR2 vMousePos((float)pt.x, (float)pt.y);

	if (m_pkThumb->CheckPosInWindowBound(vMousePos))
	{
		if (g_pkInput->BtnDown(KInput::MBLEFT))
			m_bDrag = true;
		else if (g_pkInput->BtnUp(KInput::MBLEFT))
			m_bDrag = false;
	}
	else if (g_pkInput->BtnDown(KInput::MBLEFT))
		m_bDrag = false;

	if (m_bDrag && g_pkInput->IsMouseMoving() && g_pkInput->BtnPressed(KInput::MBLEFT))
	{
		for (int i = 0; i < MAX_QUALITY; ++i)
		{
			D3DXVECTOR2 vPos = m_ppkGage[i]->GetRelocatedWindowPos();
			if (vPos.x <= pt.x && vPos.x + m_ppkGage[i]->GetWidth() >= pt.x)
			{
				if (i != m_iGraphicQuality)
				{
					g_MyD3D->m_KGCOption.Quality_Change(i - m_iGraphicQuality);
					break;
				}
			}
		}
	}
}

void KGCGraphicQuality::SetQuality(int iQuality)
{
	if (iQuality >= MAX_QUALITY)
		iQuality = MAX_QUALITY;
	if (iQuality < 0)
		iQuality = 0;

	m_iGraphicQuality = iQuality;

	for (int i = 0; i < MAX_QUALITY; ++i)
		m_ppkGage[i]->Lock(i < iQuality);

	D3DXVECTOR2 vPosThumb = m_pkThumb->GetFixedWindowLocalPos();
	vPosThumb.x = m_ppkGage[iQuality]->GetFixedWindowLocalPos().x;
	m_pkThumb->SetFixedWindowLocalPos(vPosThumb);
}