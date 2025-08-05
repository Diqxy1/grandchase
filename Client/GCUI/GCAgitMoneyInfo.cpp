#include "stdafx.h"
#include "GCAgitMoneyInfo.h"

IMPLEMENT_CLASSNAME( KGCAgitMoneyInfo );
IMPLEMENT_WND_FACTORY( KGCAgitMoneyInfo );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitMoneyInfo, "gc_agit_money_info" );

KGCAgitMoneyInfo::KGCAgitMoneyInfo( void )
{
    m_pkFrame = NULL;
    m_pkStaticSeed = NULL;
    m_pkStaticStar = NULL;
    LINK_CONTROL( "innerFrame", m_pkFrame );
    LINK_CONTROL( "staticSeed", m_pkStaticSeed );
    LINK_CONTROL( "staticStar", m_pkStaticStar );
}

KGCAgitMoneyInfo::~KGCAgitMoneyInfo( void )
{
}

void KGCAgitMoneyInfo::OnCreate( void )
{
    m_pkFrame->InitState( true, false, NULL );
    m_pkFrame->SetAllWindowAlpha( 150 );

    m_pkStaticSeed->InitState( true, false, NULL );
    m_pkStaticStar->InitState( true, false, NULL );
    m_pkStaticSeed->SetAlign( DT_RIGHT );
    m_pkStaticStar->SetAlign( DT_RIGHT );
}

void KGCAgitMoneyInfo::ActionPerformed( const KActionEvent& event )
{

}

void KGCAgitMoneyInfo::UpdateAgitMoney( void )
{
    m_pkFrame->SetAllWindowAlpha( 150 );

    // ¾¾¾Ñ °¹¼ö 
    int     iNumSeed = g_pItemMgr->GetInventoryItemDuration( KGCItemManager::ITEM_AGIT_FLOWER_POT_SEED );
    int     iNumStar = g_pItemMgr->GetInventoryItemDuration( KGCItemManager::ITEM_AGIT_MONEY_STAR );
    
    // Ãâ·Â 
    m_pkStaticSeed->SetNumber( iNumSeed );
    m_pkStaticStar->SetNumber( iNumStar );
}