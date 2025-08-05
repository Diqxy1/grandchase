#include "stdafx.h"
#include "KGCWelcomBack_Reward.h"
//

IMPLEMENT_CLASSNAME( KGCWelcomeBackReward );
IMPLEMENT_WND_FACTORY( KGCWelcomeBackReward );
IMPLEMENT_WND_FACTORY_NAME( KGCWelcomeBackReward, "gc_recom_reward" );

KGCWelcomeBackReward::KGCWelcomeBackReward(void)
: m_pkDesc( NULL )
, m_pkBack( NULL )
, m_pkCheckMark( NULL )
{
    LINK_CONTROL( "desc",           m_pkDesc );
    LINK_CONTROL( "back",           m_pkBack );
    LINK_CONTROL( "check_mark",     m_pkCheckMark);
}

KGCWelcomeBackReward::~KGCWelcomeBackReward(void)
{
}

void KGCWelcomeBackReward::OnCreate( void )
{
    m_pkDesc->InitState( true, true, this );
    m_pkBack->InitState( true, true, this );
    m_pkCheckMark->ToggleRender( false );
}

void KGCWelcomeBackReward::SetLock()
{
    m_pkDesc->Lock( true );
    m_pkBack->Lock( true );
    m_pkCheckMark->ToggleRender(true);
}

void KGCWelcomeBackReward::ActionPerformed( const KActionEvent& event )
{
    SpeakToActionListener( KActionEvent( this, event.m_dwCode, event.m_dwlParam, event.m_dwlParam2 ) );
}