#include "stdafx.h"
#include ".\gcresultnick.h"

IMPLEMENT_CLASSNAME( KGCResultNick );
IMPLEMENT_WND_FACTORY( KGCResultNick );
IMPLEMENT_WND_FACTORY_NAME( KGCResultNick, "gc_result_nick" );

KGCResultNick::KGCResultNick(void)
{
	m_pkColorFrame[TEAM_CANABAN] = NULL;
	m_pkColorFrame[TEAM_SERDIN] = NULL;
	m_pkNick = NULL;

	LINK_CONTROL( "name_frame_canaban",		m_pkColorFrame[TEAM_CANABAN] );
	LINK_CONTROL( "name_frame_serdin",		m_pkColorFrame[TEAM_SERDIN]  );
	LINK_CONTROL( "static_nick",			m_pkNick					 );
}

KGCResultNick::~KGCResultNick(void)
{
}

void KGCResultNick::OnCreate()
{
	m_pkColorFrame[TEAM_CANABAN]->InitState( true, true, this );
	m_pkColorFrame[TEAM_SERDIN]->InitState( false, true, this );
	m_pkNick->InitState( true, true, this );
	m_pkNick->SetAlign( DT_CENTER );
}

void KGCResultNick::SetTeam( int iTeam )
{
	for( int i = 0 ; i < TEAM_NUM ; ++i )
	{
		m_pkColorFrame[i]->ToggleRender( i == iTeam );
	}
}

void KGCResultNick::SetNick( std::wstring& strNick )
{
	if( strNick.empty() )
		return;

	m_pkNick->SetText( strNick );
}

void KGCResultNick::Init()
{
	m_pkNick->SetText();
	SetTeam( TEAM_SERDIN );
}