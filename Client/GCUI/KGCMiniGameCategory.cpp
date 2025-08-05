#include "stdafx.h"
#include "KGCMiniGameCategory.h"

//
//
//

IMPLEMENT_CLASSNAME( KGCMinigameCategory );
IMPLEMENT_WND_FACTORY( KGCMinigameCategory );
IMPLEMENT_WND_FACTORY_NAME( KGCMinigameCategory, "gc_minigame_category" );

KGCMinigameCategory::KGCMinigameCategory(void)
: m_pkGameTitle( NULL )
, m_pkGameDesc( NULL )
{
    LINK_CONTROL( "title", m_pkGameTitle );
    LINK_CONTROL( "desc", m_pkGameDesc );
}

KGCMinigameCategory::~KGCMinigameCategory(void)
{
}

void KGCMinigameCategory::OnCreate( void )
{
    m_pkGameTitle->InitState( true );
    m_pkGameDesc->InitState( true );
}

void KGCMinigameCategory::SetGameDesc( const int GameCategory )
{
    int iDescStr = STR_ID_MINIGAME_DESC0;

    m_pkGameDesc->SetAlign( DT_CENTER );
    m_pkGameDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( iDescStr + GameCategory ) );
}
