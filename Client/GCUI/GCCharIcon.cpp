#include "stdafx.h"
#include "GCCharIcon.h"

IMPLEMENT_CLASSNAME( KGCCharIcon );
IMPLEMENT_WND_FACTORY( KGCCharIcon );
IMPLEMENT_WND_FACTORY_NAME( KGCCharIcon, "gc_char_icon" );

KGCCharIcon::KGCCharIcon( void )
{
    m_pkLock = NULL;
    m_pkOpen = NULL;
    LINK_CONTROL( "lock", m_pkLock );
    LINK_CONTROL( "open", m_pkOpen );

	m_pkLockIcon = NULL;
	LINK_CONTROL( "lock_icon", m_pkLockIcon );
	m_pkNewIcon = NULL;
	LINK_CONTROL( "new_icon", m_pkNewIcon );

	m_pkCashIcon = NULL;
	LINK_CONTROL( "cash_icon", m_pkCashIcon );

	m_pkSaleIcon = NULL;
	LINK_CONTROL( "sale_icon", m_pkSaleIcon );

	m_pkSelIcon = NULL;
	LINK_CONTROL( "sel_icon", m_pkSelIcon );
	
	m_pkDisableIcon = NULL;
	LINK_CONTROL( "disable_icon", m_pkDisableIcon );
}

KGCCharIcon::~KGCCharIcon( void )
{
}

void KGCCharIcon::OnCreate( void )
{
	if (m_pkLock)
		m_pkLock->InitState( false );
	if (m_pkOpen)
		m_pkOpen->InitState( false );

	if (m_pkNewIcon)
		m_pkNewIcon->InitState(false);
	if (m_pkLockIcon)
		m_pkLockIcon->InitState(false);

	if (m_pkCashIcon)
		m_pkCashIcon->InitState(false);

	if (m_pkSaleIcon)
		m_pkSaleIcon->InitState(false);

	if (m_pkSelIcon)
		m_pkSelIcon->InitState(false);

	if (m_pkDisableIcon)
		m_pkDisableIcon->InitState(false);
}


void KGCCharIcon::ActionPerformed( const KActionEvent& event )
{

}

void KGCCharIcon::SetShow( bool bLock )
{
    if( bLock )
    {
        m_pkLock->ToggleRender( true );
        m_pkOpen->ToggleRender( false );
    }
    else
    {
        m_pkLock->ToggleRender( false );
        m_pkOpen->ToggleRender( true );
    }
}

void KGCCharIcon::SetCashIcon( bool bLockIcon_ )
{
	m_pkCashIcon->ToggleRender(bLockIcon_);
}

void KGCCharIcon::InitMark()
{
	if (m_pkLock)
		m_pkLock->ToggleRender(false);
	if (m_pkNewIcon)
		m_pkNewIcon->ToggleRender(false);
	if (m_pkCashIcon)
		m_pkCashIcon->ToggleRender(false);
	if (m_pkSaleIcon)
		m_pkSaleIcon->ToggleRender(false);
	if (m_pkSelIcon)
		m_pkSelIcon->ToggleRender(false);
	if (m_pkDisableIcon)
		m_pkDisableIcon->ToggleRender(false);
}
