#include "stdafx.h"
#include "GCMonsterCard.h"
#include "GCItemImgWnd.h"
#include "GCItemManager.h"


IMPLEMENT_CLASSNAME( KGCMonsterCard );
IMPLEMENT_WND_FACTORY( KGCMonsterCard );
IMPLEMENT_WND_FACTORY_NAME( KGCMonsterCard, "gc_monster_card" );

KGCMonsterCard::KGCMonsterCard( void )
: m_pkItemName( NULL )
, m_pkItemAttribute( NULL )
, m_pkItemDecs( NULL )
, m_pkItemImg( NULL )
{
    LINK_CONTROL( "monster_card_name",      m_pkItemName );
    LINK_CONTROL( "monster_card_ability",   m_pkItemAttribute );
    LINK_CONTROL( "monster_card_decs",      m_pkItemDecs );
    LINK_CONTROL( "monster_card_img",       m_pkItemImg );

    for( int i = 0 ; i < KItem::GRADE_NUM; ++i )
    {
        m_pkEdge[i] = NULL;

        char strTemp[ MAX_PATH ] = "";
        sprintf_s( strTemp, MAX_PATH, "monster_card_edge%d", i );

        LINK_CONTROL( strTemp, m_pkEdge[i] );
    }
}

KGCMonsterCard::~KGCMonsterCard( void )
{
}

void KGCMonsterCard::OnCreate( void )
{
    m_pkItemName->SetSelfInputCheck( true );
    m_pkItemAttribute->SetSelfInputCheck( true );
    m_pkItemDecs->SetSelfInputCheck( true );
    m_pkItemImg->SetSelfInputCheck( true );
}

void KGCMonsterCard::SetMonsterCard( GCITEMID ItemID_ )
{
    GCItem* pItem = g_pItemMgr->GetItemData( ItemID_ );

    if( !pItem || GIK_MONSTER_CARD != pItem->eItemKind)
        return;

    m_pkItemName->SetText( pItem->strItemName );

    std::wstring strItemAttribute = g_pItemMgr->GetItemLevelString( pItem );
    strItemAttribute += g_pItemMgr->GetItemGradeString( pItem->cItemGrade );
    strItemAttribute += L"\n";
    strItemAttribute += g_pItemMgr->GetItemAtrributeString( pItem );
    m_pkItemAttribute->SetTextAutoMultiline( strItemAttribute );

    m_pkItemDecs->SetTextAutoMultiline( pItem->strItemDesc );
    m_pkItemImg->SetItemInfo( ItemID_ );

    for(int i = 0 ; i < KItem::GRADE_NUM; ++i)
    {
        m_pkEdge[i]->ToggleRender( false );
    }

    if( 0 <= pItem->cItemGrade && pItem->cItemGrade < KItem::GRADE_NUM ) 
        m_pkEdge[ pItem->cItemGrade ]->ToggleRender( true );
}

void KGCMonsterCard::FrameMoveInEnabledState( void )
{}

void KGCMonsterCard::ActionPerformed( const KActionEvent& event )
{}
