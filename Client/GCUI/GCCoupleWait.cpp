#include "StdAfx.h"
#include "GCCoupleWait.h"
#include "GCSizingThreeBox.h"
#include "GCCoupleSystem.h"

IMPLEMENT_CLASSNAME( KGCCoupleWait );
IMPLEMENT_WND_FACTORY( KGCCoupleWait );
IMPLEMENT_WND_FACTORY_NAME( KGCCoupleWait, "gc_couple_wait" );

KGCCoupleWait::KGCCoupleWait(void)
: m_bFirst( true )
, m_iFullGuageLength( 100 )
{
	m_pkStatic = NULL;
	m_pkWaitGuage = NULL;

	LINK_CONTROL( "static", m_pkStatic );
	LINK_CONTROL( "bg_guage", m_pkWaitGuage );
}

KGCCoupleWait::~KGCCoupleWait(void)
{
}

void KGCCoupleWait::OnCreate( void )
{
	if( m_bFirst )
	{
		m_iFullGuageLength = m_pkWaitGuage->GetWidth();
		m_bFirst = false;
	}
	m_pkWaitGuage->InitState( true );
	m_pkStatic->InitState( true );
	m_pkStatic->SetText( g_pkStrLoader->GetString(STR_ID_COUPLE_SYSTEM24) );
	m_iWaitCount = 1300;
}

void KGCCoupleWait::ActionPerformed( const KActionEvent& event )
{

}

void KGCCoupleWait::FrameMoveInEnabledState( void )
{
	if( m_iWaitCount > 0 )
	{
		--m_iWaitCount;
		m_pkWaitGuage->SetWidth( (DWORD)(((float)m_iWaitCount/1300.0f) * m_iFullGuageLength) );
	}

	if( SiKGCCoupleSystem()->GetState() != KGCCoupleSystem::GCCS_WAIT )
		SpeakToActionListener( KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}
