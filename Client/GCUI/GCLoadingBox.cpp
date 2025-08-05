#include "stdafx.h"
#include "../MyD3D.h"
#include "GCLoadingBox.h"




IMPLEMENT_CLASSNAME( KGCLoadingBox );
IMPLEMENT_WND_FACTORY( KGCLoadingBox );
IMPLEMENT_WND_FACTORY_NAME( KGCLoadingBox, "gc_loading_box" );

KGCLoadingBox::KGCLoadingBox( void )
{
    m_dwOrgLen  = 0;
    m_pkGauge   = NULL;
    
    LINK_CONTROL( "loading_box_gauge",          m_pkGauge );
}

KGCLoadingBox::~KGCLoadingBox( void )
{
    // empty
}

void KGCLoadingBox::OnCreate( void )
{
	m_dwOrgLen = 0;
    m_pkGauge->SetSelfInputCheck( false );   
}

void KGCLoadingBox::OnCreateComplete( void )
{
    m_dwOrgLen = m_pkGauge->GetWidth(); 
    SpeakToActionListener( KActionEvent( this, LBR_CREATED ) );
}

void KGCLoadingBox::OnDestroyComplete( void )
{
    m_pkGauge->SetWidth( m_dwOrgLen );
    SpeakToActionListener( KActionEvent( this, LBR_DESTROYED ) );
}

void KGCLoadingBox::FrameMoveInEnabledState( void )
{
    if ( g_MyD3D->m_bRPLoadFinished && m_dwOrgLen >= MAX_GAUGE_LEN )
    {
        SpeakToActionListener( KActionEvent( this, LBR_SUCCESS ) );
        g_MyD3D->m_bRPLoadFinished = false;
		m_dwOrgLen = 0;
		return;
    }
    
	if( m_dwOrgLen < MAX_GAUGE_LEN )
		++m_dwOrgLen;

	if( m_dwOrgLen > GC_FPS_LIMIT * MAX_LOADING_TIME )
	{
		SpeakToActionListener( KActionEvent( this, LBR_FAILED ) );
	}

    m_pkGauge->SetWidth( m_dwOrgLen  );
}

void KGCLoadingBox::ActionPerformed( const KActionEvent& event )
{
    switch ( event.m_dwCode )
    {
        case KD3DWnd::D3DWE_CURSOR_ENTER:
        {
            g_KDSound.Play( "30" );
            break;
        }
        case KD3DWnd::D3DWE_BUTTON_DOWN:
        {
            g_KDSound.Play( "31" );
            break;
        }        
    }
}
