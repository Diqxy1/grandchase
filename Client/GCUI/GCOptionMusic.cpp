#include "stdafx.h"
#include "GCOptionMusic.h"
//

#include "../KMci.h"
//

//

IMPLEMENT_CLASSNAME( KGCOptionMusic );
IMPLEMENT_WND_FACTORY( KGCOptionMusic );
IMPLEMENT_WND_FACTORY_NAME( KGCOptionMusic, "gc_music" );

KGCOptionMusic* g_pOptionMusic = NULL;

KGCOptionMusic::KGCOptionMusic(void)
{
	g_pOptionMusic = this;

	m_pkTitle = NULL;
	m_pkCheckBox = NULL;
	m_pkCheck = NULL;
	m_pkThumb = NULL;
	m_pkGage = NULL;
	m_pkGageLeft = NULL;
	m_pkGageCenter = NULL;
	m_pkGageRight = NULL;

	m_fVolume = 0.0f;
	m_fRange =  0.0f;
	m_fStartX = 0.0f;
	m_bDrag = false;

	LINK_CONTROL( "title_static", m_pkTitle );
	LINK_CONTROL( "check_box", m_pkCheckBox );
	LINK_CONTROL( "check", m_pkCheck );
	LINK_CONTROL( "gage_btn", m_pkThumb );
	LINK_CONTROL( "gage", m_pkGage );
	LINK_CONTROL( "gage_left", m_pkGageLeft );
	LINK_CONTROL( "gage_center", m_pkGageCenter );
	LINK_CONTROL( "gage_right", m_pkGageRight );

}

KGCOptionMusic::~KGCOptionMusic(void)
{
}

void KGCOptionMusic::OnCreate( void )
{
	m_pkTitle->SetText( g_pkStrLoader->GetString(STR_ID_MUSIC) );
    m_pkTitle->InitState( true , true , this );

	m_pkCheckBox->SetSelfInputCheck( true );
	m_pkCheckBox->AddActionListener( this );
	m_pkCheck->SetSelfInputCheck( true );
	m_pkCheck->AddActionListener( this );
	m_pkThumb->SetSelfInputCheck( true );
	m_pkThumb->AddActionListener( this );

	m_pkGageLeft->InitState( true , true , this );
	m_pkGageCenter->InitState( true , true , this );
	m_pkGageRight->InitState( true , true , this );
	m_pkGage->InitState( true , true , this );

	SetStartGagePos();
	SetStartRangePos();

	SetVolume( m_fVolume );
}

void KGCOptionMusic::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		if( event.m_pWnd == m_pkCheckBox || event.m_pWnd == m_pkCheck || event.m_pWnd == m_pkTitle )
		{
			g_KDSound.Play( "31" );
			SetToggle();
		}
	}

	if( m_pkCheck->IsRenderOn() == false )
		return;

	static float fBefore = 0.0f;
	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

	if( m_pkThumb->CheckPosInWindowBound( vMousePos ) )
	{
		if( g_pkInput->BtnDown( KInput::MBLEFT ) )
		{
			m_bDrag = true;
			fBefore = (float)pt.x;
		}
		else if ( g_pkInput->BtnUp( KInput::MBLEFT ) )
			m_bDrag = false;
	}
	else if( g_pkInput->BtnDown( KInput::MBLEFT ) ) 
		m_bDrag = false;

	if( m_bDrag && g_pkInput->IsMouseMoving() && g_pkInput->BtnPressed( KInput::MBLEFT ) )
	{
		float fDiff = fBefore - (float)pt.x;
		fBefore = (float)pt.x;
		SetPosition( m_pkThumb->GetFixedWindowLocalPos().x - fDiff );
	}
}

void KGCOptionMusic::SetStartGagePos()
{
	if( m_pkGage )
		m_fStartX = m_pkGage->GetFixedWindowLocalPos().x;
}

void KGCOptionMusic::SetStartRangePos()
{
	if( m_pkGage )
	{
		float fWidth = static_cast<float>(m_pkGage->GetWidth());
		if( fWidth > 0.0f )
		{
			m_fRange = fWidth;
		}
	}
}

void KGCOptionMusic::SetPosition( float fPos )
{
	if( fPos < m_fStartX )
		fPos = m_fStartX;

	if( fPos > ( (m_fRange+m_fStartX) - m_pkThumb->GetWidth() ) )
		fPos = m_fRange+m_fStartX;

	m_fVolume = ((fPos-m_fStartX)*5000.0f)/m_fRange;
	m_fVolume -= 5000.0f;

	D3DXVECTOR2 vPos = m_pkThumb->GetFixedWindowLocalPos();

	float fPosX = fPos; 

	if( fPos == (m_fStartX + m_fRange) )
		fPosX = fPos - m_pkThumb->GetWidth();	

	vPos.x = fPosX;
	m_pkThumb->SetFixedWindowLocalPos( vPos );

	float fRangeX = (fPosX-m_fStartX) - m_pkGageLeft->GetWidth();
	m_pkGageCenter->SetWidth( (DWORD)fRangeX );

	vPos = m_pkGageCenter->GetFixedWindowLocalPos();
	vPos.x += fRangeX;
	m_pkGageRight->SetFixedWindowLocalPos(vPos);
	g_KMci.SetVolume( m_fVolume );

	if( m_fVolume == -5000.0f )
	{
		SetEnable( false, -5000.0f);
	}
}

void KGCOptionMusic::SetVolume( float fVolume )
{
	if( fVolume < -5000.0f )
		fVolume = -5000.0f;
	if( fVolume > 0.0f )
		fVolume = 0.0f;

	m_fVolume = fVolume;
	g_KMci.SetVolume( fVolume );

	// ������ ����
	float fTemp = fVolume + 5000.0f;
	float fX = (fTemp*m_fRange)/5000.0f;

	if (m_pkThumb == NULL)
		return;

	D3DXVECTOR2 vPos = m_pkThumb->GetFixedWindowLocalPos();

	fX += m_fStartX;

	if( m_fRange == fX )
	{
		fX -= m_pkThumb->GetWidth();
	}

	vPos.x = fX;
	m_pkThumb->SetFixedWindowLocalPos( vPos );

	float fRangeX = (fX - m_fStartX) - m_pkGageLeft->GetWidth();
	m_pkGageCenter->SetWidth( (DWORD)fRangeX );

	vPos = m_pkGageCenter->GetFixedWindowLocalPos();
	vPos.x += fRangeX;
	m_pkGageRight->SetFixedWindowLocalPos(vPos);
}

void KGCOptionMusic::SetEnable( bool bEnable, float fVolume )
{
	g_KMci.SetPlay( bEnable );
	m_pkCheck->ToggleRender( g_KMci.IsPlay() );
	SetVolume( fVolume );
}

void KGCOptionMusic::SetToggle()
{
	bool bEnable = !g_KMci.IsPlay();
	SetEnable( bEnable, m_fVolume );

	if( g_KMci.IsPlay() )
		g_KMci.Continue();
	else 
		g_KMci.Stop();
}
