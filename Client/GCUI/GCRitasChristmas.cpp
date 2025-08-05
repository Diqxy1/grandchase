#include "stdafx.h"
#include "GCRitasChristmas.h"
#include "GCRitasPanel.h"

IMPLEMENT_CLASSNAME( KGCRitasChristmas );
IMPLEMENT_WND_FACTORY( KGCRitasChristmas );
IMPLEMENT_WND_FACTORY_NAME( KGCRitasChristmas, "gc_ritaschristmas" );

KGCRitasChristmas::KGCRitasChristmas( void ):
m_pkRitasPanel(NULL),
m_pkBtnClose(NULL),
m_pkRitasRankingDlg(NULL),
m_pkRitasReslutDlg(NULL)
{
	LINK_CONTROL("ritaspanel",m_pkRitasPanel);
	LINK_CONTROL("rankingdlg",m_pkRitasRankingDlg);
	LINK_CONTROL( "btn_close", m_pkBtnClose );
	LINK_CONTROL( "resultdlg", m_pkRitasReslutDlg );

	m_backMusic = GC_BGM_MAIN_THEME;
	
}

KGCRitasChristmas::~KGCRitasChristmas( void )
{
}

void KGCRitasChristmas::OnCreate( void )
{

	m_pkRitasPanel->InitState(true,true,this);
	m_pkRitasRankingDlg->InitState(true,true,this);
	m_pkRitasReslutDlg->InitState(false,true,this);

	m_pkBtnClose->InitState( true, true, this );
	//m_pkBtnClose->SetHotKey( DIK_ESCAPE );

}

void KGCRitasChristmas::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );

}

void KGCRitasChristmas::OnClose()
{
	g_KDSound.Play( "31" );
	m_pkRitasPanel->AllSetParticle(false);

	SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
	g_KMci.Play( m_backMusic, true );
}

void KGCRitasChristmas::OnCreateComplete()
{
	m_backMusic = static_cast<EGCBackgroundMusic>(g_KMci.GetTrack());

	if( m_backMusic != GC_BGM_RITAS_CHRISTMAS )
		g_KMci.Play( GC_BGM_RITAS_CHRISTMAS, true );

	m_backMusic = GC_BGM_MAIN_THEME;

}
