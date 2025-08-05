#include "stdafx.h"
#include "GCEclipseTime.h"
#include "GCEclipseEventManager.h"

IMPLEMENT_CLASSNAME( KGCEclipseTime );
IMPLEMENT_WND_FACTORY( KGCEclipseTime );
IMPLEMENT_WND_FACTORY_NAME( KGCEclipseTime, "gc_eclipse_time" );

KGCEclipseTime::KGCEclipseTime( void )
: m_pkMoonGauge(NULL)
, m_pkSunGauge(NULL)
, m_pkEclipse(NULL)
, m_pkEclipse_ac(NULL)
, m_pkParticle(NULL)
, m_bBigger(true)
, m_fCurrentScale(1.0f)
{
    LINK_CONTROL( "sun_gauge", m_pkSunGauge );
    LINK_CONTROL( "moon_gauge", m_pkMoonGauge );
    LINK_CONTROL( "eclipse", m_pkEclipse );
    LINK_CONTROL( "eclipse_ac", m_pkEclipse_ac );
}

KGCEclipseTime::~KGCEclipseTime( void )
{
}

void KGCEclipseTime::ActionPerformed( const KActionEvent& event )
{

}

void KGCEclipseTime::FrameMoveInEnabledState( void )
{
    EclipseScaling();
}

void KGCEclipseTime::OnCreate( void )
{
    m_pkSunGauge->InitState(true,false,NULL);
    m_pkMoonGauge->InitState(true,false,NULL);
    m_pkEclipse->InitState(false,true,this);
    m_pkEclipse_ac->InitState(false,true,this);

    g_pkUIMgr->ConnectGlobalEventProcedure( KGCUIScene::D3DWE_TIME_TICK_ONE_MINUTE, this, &KGCEclipseTime::Tick );
    g_pkUIMgr->ConnectGlobalEventProcedure( KD3DWnd::D3DWE_SCENE_CHANGE, this, &KGCEclipseTime::Tick );
	m_pkEclipse->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCEclipseTime::ClickEvent);
	m_pkEclipse_ac->ConnectEventProcedure(KD3DWnd::D3DWE_BUTTON_CLICK,this,&KGCEclipseTime::ClickEvent);
}

void KGCEclipseTime::OnDestroy()
{
	if(m_pkParticle)
		g_ParticleManager->DeleteSequence( m_pkParticle );
	m_pkParticle = NULL;

}

void KGCEclipseTime::SetGauge()
{
    int iPercent = SiGCEclipseEventManager()->GetLeftEventTimePercent();
    
    const float fBaseWidth = 0.88f;
    const float fBaseSunXPosition = 271.3f;

    float fWidth = fBaseWidth * static_cast<float>(iPercent) ;
    DWORD dwWidth = static_cast<DWORD>(fWidth);

    m_pkSunGauge->SetWidth(dwWidth + 24);
    m_pkMoonGauge->SetWidth(dwWidth + 24);

    m_pkSunGauge->SetWindowPosX(fBaseSunXPosition - static_cast<float>(dwWidth));
}

void KGCEclipseTime::ClickEvent()
{
	g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EVENT_ECLIPSE_PLOT_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false );
	
}

void KGCEclipseTime::Tick()
{
    if(SiGCEclipseEventManager()->IsVeiwMode() == false){
        ToggleRender(false);
		if(m_pkParticle)
			g_ParticleManager->DeleteSequence( m_pkParticle );
		m_pkParticle = NULL;
        return; 
    }
    else
        ToggleRender(true);

    SetGauge();
    SetActiveIcon();
}

void KGCEclipseTime::SetActiveIcon()
{
    bool bActiveEvent = SiGCEclipseEventManager()->IsActiveEvent();

    m_pkEclipse->ToggleRender(!bActiveEvent);
    m_pkEclipse_ac->ToggleRender(bActiveEvent);
	if(bActiveEvent)
	{	
		if(m_pkParticle == NULL)
			m_pkParticle = g_ParticleManager->CreateSequence( "UI_Eclipse_01", -0.073f, 0.009f, 0.5f );
		g_ParticleManager->SetLayer(  m_pkParticle, GC_LAYER_UI);
		g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence( "UI_Eclipse_02", -0.073f, 0.009f, 0.5f ), GC_LAYER_UI);
	}
    else
    {
        if(m_pkParticle){
            g_ParticleManager->DeleteSequence( m_pkParticle );
            m_pkParticle = NULL;
        }
    }

}

void KGCEclipseTime::EclipseScaling()
{
    if ( m_bBigger ) {
        m_fCurrentScale += ( MAX_ECLIPSE_SCALE - 1.0f ) / FRAME_PER_REVOLUTION;

        if ( m_fCurrentScale > MAX_ECLIPSE_SCALE ) {
            m_fCurrentScale = MAX_ECLIPSE_SCALE;
            m_bBigger = false;
        }
    } else {
        m_fCurrentScale -= ( MAX_ECLIPSE_SCALE - 1.0f ) / FRAME_PER_REVOLUTION;

        if ( m_fCurrentScale < MIN_ECLIPSE_SCALE ) {
            m_fCurrentScale = MIN_ECLIPSE_SCALE;
            m_bBigger = true;
        }
    }

    D3DXVECTOR2 vNewPos;
    static D3DXVECTOR2 vOriginalPos = m_pkEclipse->GetFixedWindowLocalPos();
    static D3DXVECTOR2 vOriginalPos_ac = m_pkEclipse_ac->GetFixedWindowLocalPos();
    static float fOriginalWidth_ac = static_cast<float>( m_pkEclipse_ac->GetWidth() );
    static float fOriginalHeight_ac = static_cast<float>( m_pkEclipse_ac->GetHeight() );
    static float fOriginalWidth = static_cast<float>( m_pkEclipse->GetWidth() );
    static float fOriginalHeight = static_cast<float>( m_pkEclipse->GetHeight() );

    if( SiGCEclipseEventManager()->IsActiveEvent() ){
        m_pkEclipse_ac->SetWidth( static_cast<DWORD>( fOriginalWidth_ac * m_fCurrentScale ) );
        m_pkEclipse_ac->SetHeight( static_cast<DWORD>( fOriginalHeight_ac * m_fCurrentScale ) );

        vNewPos.x = vOriginalPos_ac.x - ( ( m_fCurrentScale - 1.0f ) * fOriginalWidth_ac * 0.5f );
        vNewPos.y = vOriginalPos_ac.y - ( ( m_fCurrentScale - 1.0f ) * fOriginalHeight_ac * 0.5f );

        m_pkEclipse_ac->SetWindowPos( vNewPos );

    } else {
        m_pkEclipse->SetWidth( static_cast<DWORD>( fOriginalWidth * m_fCurrentScale ) );
        m_pkEclipse->SetHeight( static_cast<DWORD>( fOriginalHeight * m_fCurrentScale ) );

        vNewPos.x = vOriginalPos.x - ( ( m_fCurrentScale - 1.0f ) * fOriginalWidth * 0.5f );
        vNewPos.y = vOriginalPos.y - ( ( m_fCurrentScale - 1.0f ) * fOriginalHeight * 0.5f );

        m_pkEclipse->SetWindowPos( vNewPos );
    }

}
