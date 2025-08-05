#include "stdafx.h"
#include "KGCWelcomBack_Recom.h"

IMPLEMENT_CLASSNAME( KGCWelcomeBackRecom );
IMPLEMENT_WND_FACTORY( KGCWelcomeBackRecom );
IMPLEMENT_WND_FACTORY_NAME( KGCWelcomeBackRecom, "gc_recommender" );

KGCWelcomeBackRecom::KGCWelcomeBackRecom(void)
: m_pkCheck_Mark( NULL )
, m_pkBack( NULL )
{
    LINK_CONTROL( "check_mark",     m_pkCheck_Mark );
    LINK_CONTROL( "back",           m_pkBack );
}

KGCWelcomeBackRecom::~KGCWelcomeBackRecom(void)
{
}

void KGCWelcomeBackRecom::OnCreate( void )
{
    m_pkCheck_Mark->InitState( false );
}

void KGCWelcomeBackRecom::SetSupply( void )
{
    m_pkCheck_Mark->ToggleRender( true );
}

void KGCWelcomeBackRecom::SetMode( KD3DWnd::ED3DWndMode eMode)
{
    m_pkBack->SetWndMode( eMode );
}