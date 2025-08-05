#include "StdAfx.h"
#include "KGCGuildSquareNotice.h"

IMPLEMENT_CLASSNAME( KGCGuildSquareNotice );
IMPLEMENT_WND_FACTORY( KGCGuildSquareNotice );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildSquareNotice, "gc_guild_square_enter_notice" );

KGCGuildSquareNotice::KGCGuildSquareNotice(void)
: m_pkGuildMarkIcon( NULL )
{
	LINK_CONTROL( "guild_mark_icon",		m_pkGuildMarkIcon );
}

KGCGuildSquareNotice::~KGCGuildSquareNotice(void)
{
}

void KGCGuildSquareNotice::OnCreate( void )
{
	m_pkGuildMarkIcon->InitState( true, true, this );
}

void KGCGuildSquareNotice::OnCreateComplete( void )
{
	//길드마크
	SiKGCIconImageManager()->LoadIconImageFromHTTP(g_kGlobalValue.m_kGuildInfo.m_strFileName);
	m_pkGuildMarkIcon->SetTextureName(g_kGlobalValue.m_kGuildInfo.m_strFileName);
}

HRESULT KGCGuildSquareNotice::FrameMove( void )
{

	if( g_MyD3D->m_pStateMachine->GetState() != GS_SQUARE )
	{
		Close();		
	}
	return S_OK;
}

void KGCGuildSquareNotice::PostChildDraw( void )
{
}

void KGCGuildSquareNotice::ActionPerformed( const KActionEvent& event )
{

}

void KGCGuildSquareNotice::Close()
{
	SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}