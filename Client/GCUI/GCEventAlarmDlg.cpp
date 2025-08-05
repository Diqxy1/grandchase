#include "stdafx.h"
#include "GCEventAlarmDlg.h"
#include "GCEventAlarmManager.h"

IMPLEMENT_CLASSNAME( KGCEventAlarmDlg );
IMPLEMENT_WND_FACTORY( KGCEventAlarmDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCEventAlarmDlg, "gc_eventalarmdlg" );

const int nTEXT_DISPLAY_FRAME = 30;
const int nDISPLAY_FRAME = 55 * 4;

KGCEventAlarmDlg::KGCEventAlarmDlg( void )
: m_pkEventTitle ( NULL ) 
, m_pkEventDesc( NULL ) 
, m_dwFrameCount ( 0 )
{
	LINK_CONTROL( "static_title", m_pkEventTitle );
	LINK_CONTROL( "static_desc", m_pkEventDesc );
}

KGCEventAlarmDlg::~KGCEventAlarmDlg( void )
{
}

void KGCEventAlarmDlg::ActionPerformed( const KActionEvent& event )
{
}

void KGCEventAlarmDlg::OnCreate( void )
{
	m_pkEventTitle->InitState( false );
	m_pkEventDesc->InitState( false );
	m_pkEventDesc->SetMultiLine( true );
}

void KGCEventAlarmDlg::OnCreateComplete( void )
{
	SetEventAlarm();
}

void KGCEventAlarmDlg::PreDraw( void )
{
	g_pGCDeviceManager2->PushState();
	{
		g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		g_pGCDeviceManager2->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

		g_ParticleManager->Render( GC_LAYER_UNDER_UI );     
	}
	g_pGCDeviceManager2->PopState();
}

void KGCEventAlarmDlg::FrameMoveInEnabledState( void )
{
	m_dwFrameCount++;
	
	if ( m_dwFrameCount > nDISPLAY_FRAME ) 
	{
		OnClose();
		m_dwFrameCount = 0;
	}

	if ( m_dwFrameCount > nTEXT_DISPLAY_FRAME ) 
	{
		m_pkEventTitle->InitState( true );
		m_pkEventDesc->InitState( true );

		m_pkEventTitle->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);
		m_pkEventDesc->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);
	}
}

void KGCEventAlarmDlg::OnClose()
{
	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCEventAlarmDlg::SetEventAlarm()
{
	//	최초 한개만 셋팅되도록 한다.
	bool bEvent = false;
    SEventAlarm sEventAlarm;

	for ( int i = 0; i < (int)g_kGlobalValue.m_vecEventOpenInfo.size(); ++i ) 
	{
		int nEventID = g_kGlobalValue.m_vecEventOpenInfo[i].m_dwEventID;
		

		//	이벤트가 있다면 출력한다.
		bEvent = g_kGlobalValue.m_cEventAlarmManager.GetEventAlarm( nEventID, sEventAlarm );
		if ( bEvent && sEventAlarm.InVaild() ) 
		{
            SetEventData( sEventAlarm.m_strParticleName, sEventAlarm.m_nTitleString, sEventAlarm.m_nDescString );            
			break;
		}
	}

	//	설정된 엠블럼이 없다면 종료
	if ( !bEvent ) 
	{
		OnClose();
	}
}

void KGCEventAlarmDlg::SetEventData( std::string strParticle, int nTitleString, int nDescString )
{
    g_ParticleManager->SetLayer(g_ParticleManager->CreateSequence( strParticle, 0.015f, -0.42f, 0.5f), GC_LAYER_UNDER_UI );
    
    m_pkEventTitle->SetText( g_pkStrLoader->GetString( nTitleString ) );
    m_pkEventDesc->SetText( g_pkStrLoader->GetString( nDescString ) );

    m_pkEventTitle->SetAlign( DT_CENTER );
    m_pkEventDesc->SetAlign( DT_CENTER );
}
