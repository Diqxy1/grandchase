#include "stdafx.h"
#include "GCRitasProgressBar.h"

IMPLEMENT_CLASSNAME( KGCRitasProgressBar );
IMPLEMENT_WND_FACTORY( KGCRitasProgressBar );
IMPLEMENT_WND_FACTORY_NAME( KGCRitasProgressBar, "gc_ritasprogressbar" );

KGCRitasProgressBar::KGCRitasProgressBar( void )
{

	m_pkProgressBarBlinder = NULL;
	LINK_CONTROL( "progressbar", m_pkProgressBarBlinder );

	m_pkStaticTime = NULL;
	LINK_CONTROL( "static_time", m_pkStaticTime );
}

KGCRitasProgressBar::~KGCRitasProgressBar( void )
{
}

void KGCRitasProgressBar::ActionPerformed( const KActionEvent& event )
{

}

void KGCRitasProgressBar::OnCreate( void )
{
	m_pkProgressBarBlinder->InitState(true);
	m_pkStaticTime->InitState(true);
	m_pkStaticTime->SetAlign(DT_CENTER);
}

void KGCRitasProgressBar::SetBlindWidth(DWORD dwWidth_)
{
	m_pkProgressBarBlinder->SetWidth(dwWidth_);
}

void KGCRitasProgressBar::SetTime( float fTime_ )
{
	static std::wstring strTime;

	int iTime = static_cast<int>(fTime_);
	int iHour = iTime/60;
	int iMin = iTime%60;

	strTime = boost::str( boost::wformat(L"%02d:%02d")
		% iHour
		% iMin
		);
	m_pkStaticTime->SetText(strTime);

}

void KGCRitasProgressBar::Init( D3DXVECTOR2 vPos_ , DWORD dwWidth_ )
{
	m_pkStaticTime->SetText( L"00:00" );
	m_vWindowPos = vPos_;
	m_pkProgressBarBlinder->SetWidth(dwWidth_);
}
