#include "stdafx.h"
#include "KGCMiniGameRanker.h"
//
//

IMPLEMENT_CLASSNAME( KGCMinigameRanker );
IMPLEMENT_WND_FACTORY( KGCMinigameRanker );
IMPLEMENT_WND_FACTORY_NAME( KGCMinigameRanker, "gc_minigame_ranker" );

KGCMinigameRanker::KGCMinigameRanker(void)
: m_pNickname( NULL )
, m_pScore( NULL )
{
    LINK_CONTROL( "nick",   m_pNickname );
    LINK_CONTROL( "score",  m_pScore );
}

KGCMinigameRanker::~KGCMinigameRanker(void)
{
}

void KGCMinigameRanker::OnCreate( void )
{
    m_pNickname->InitState( true );
    m_pScore->InitState( true );
    m_pScore->SetAlign( DT_RIGHT );
    m_pScore->SetFontColor( D3DCOLOR_ARGB(255,255,255,0) );
}
void KGCMinigameRanker::SetScore( const int iScore )
{
	if( iScore > 9999 )
		m_pScore->SetText( 9999 ); 
	else if( iScore < 0 )
		m_pScore->SetText( 0 ); 
	else
		m_pScore->SetText( iScore ); 
}
