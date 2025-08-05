#include "stdafx.h"
#include "GCOptionSound.h"
//

//

//

IMPLEMENT_CLASSNAME( KGCOptionSound );
IMPLEMENT_WND_FACTORY( KGCOptionSound );
IMPLEMENT_WND_FACTORY_NAME( KGCOptionSound, "gc_sound" );

KGCOptionSound* g_pOptionSound = NULL;

KGCOptionSound::KGCOptionSound(void)
{
	g_pOptionSound = this;

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

KGCOptionSound::~KGCOptionSound(void)
{
}

void KGCOptionSound::OnCreate( void )
{
	m_pkTitle->SetText( g_pkStrLoader->GetString(STR_ID_SOUND) );
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

void KGCOptionSound::SetStartGagePos()
{
	if( m_pkGage )
		m_fStartX = m_pkGage->GetFixedWindowLocalPos().x;
}

void KGCOptionSound::SetStartRangePos()
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

void KGCOptionSound::ActionPerformed( const KActionEvent& event )
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

void KGCOptionSound::SetPosition( float fPos )
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
	g_KDSound.SetVol( (int)m_fVolume );

	if( m_fVolume == -5000.0f )
	{
		SetEnable( false, -5000.0f);
	}
}

void KGCOptionSound::SetVolume( float fVolume )
{
	if( fVolume < -5000.0f )
		fVolume = -5000.0f;
	if( fVolume > 0.0f )
		fVolume = 0.0f;

	m_fVolume = fVolume;
	g_KDSound.SetVol( (int)fVolume );

	// 게이지 설정
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

void KGCOptionSound::SetEnable( bool bEnable, float fVolume )
{
	Is_Sound = bEnable;
	
	if( !EnableSound )
		Is_Sound = false;

    if( m_pkCheck )
	    m_pkCheck->ToggleRender( Is_Sound );

	SetVolume( fVolume );
}

void KGCOptionSound::SetToggle()
{
	SetEnable( !Is_Sound, m_fVolume );
}
