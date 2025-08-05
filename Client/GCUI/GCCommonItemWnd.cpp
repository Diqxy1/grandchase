#include "stdafx.h"
#include "GCCommonItemWnd.h"
#include "GCItemImgWnd.h"
#include "GCUINumber.h"

IMPLEMENT_CLASSNAME( KGCCommonItemWnd );
IMPLEMENT_WND_FACTORY( KGCCommonItemWnd );
IMPLEMENT_WND_FACTORY_NAME( KGCCommonItemWnd, "gc_common_item_wnd" );

KGCCommonItemWnd::KGCCommonItemWnd( void )
{
    m_itemID = 0;
    m_itemUID = 0;

    m_pItemImgWnd = NULL;
    m_pkSelItemImg = NULL;
    m_pkSmallDurationNum = NULL;
    m_pkStrongNum = NULL;
    m_pkEquipEnchantItemLv = NULL;
    m_pkCoordiComposeImg = NULL;
    m_pkEquipImg = NULL;

    for ( int i = 0; i < KItem::GRADE_NUM; ++i ) 
    {
        m_pkGradeFrame[i] = NULL;
        LINK_CONTROL_STM( "item_frame_grade"<<i, m_pkGradeFrame[i] );
    }

    LINK_CONTROL( "item_img", m_pItemImgWnd );
    LINK_CONTROL( "inven_slot_sel", m_pkSelItemImg );
    LINK_CONTROL( "number_duration_small", m_pkSmallDurationNum );
    LINK_CONTROL( "number_strong", m_pkStrongNum );
    LINK_CONTROL( "number_equip_enchant_item", m_pkEquipEnchantItemLv );

    LINK_CONTROL( "img_coordi_compose", m_pkCoordiComposeImg );
    LINK_CONTROL( "equip_img", m_pkEquipImg );
}

KGCCommonItemWnd::~KGCCommonItemWnd( void )
{
}

void KGCCommonItemWnd::ActionPerformed( const KActionEvent& event )
{
}

void KGCCommonItemWnd::OnCreate( void )
{
    m_pItemImgWnd->InitState( true, false, this );
    m_pItemImgWnd->ShowItem( false );

    if ( m_pkSelItemImg )
    {
        m_pkSelItemImg->InitState( false );
    }

    if ( m_pkSmallDurationNum )
    {
        m_pkSmallDurationNum->InitState( false );
        m_pkSmallDurationNum->SetStringType( "very_small_num" );
    }

    if ( m_pkStrongNum )
    {
        m_pkStrongNum->InitState( false );
        m_pkStrongNum->SetStringType( "yellow_num" );
    }

    if ( m_pkEquipEnchantItemLv )
    {
        m_pkEquipEnchantItemLv->InitState( false );
        m_pkEquipEnchantItemLv->SetStringType( "normal_num_enchant" );
        m_pkEquipEnchantItemLv->SetWriteToRight( false );
    }

    for ( int i = 0; i < KItem::GRADE_NUM; ++i ) 
    {
        if ( m_pkGradeFrame[i] )
            m_pkGradeFrame[i]->InitState( false );        
    }

    if ( m_pkCoordiComposeImg )
    {
        m_pkCoordiComposeImg->InitState( false );
    }

    if ( m_pkEquipImg )
    {
        m_pkEquipImg->InitState( false );
    }
}

void KGCCommonItemWnd::SetItem( KItem* kItem, KPetInfo* pPetInfo )
{
    if( !kItem )
        return;

    m_itemID = kItem->m_ItemID;
    m_itemUID = kItem->m_ItemUID;

    GCItem* pItem = g_pItemMgr->GetItemData( m_itemID );    
    if ( !pItem )
        return;

    bool bSetItem = (pItem->dwSlotPosition&ESP_SET_ITEM);
    int iSetNumber = pItem->iSetItemNumber;
    char cGrade = kItem->m_cGradeID;

    if ( pPetInfo )
    {
        m_pItemImgWnd->SetPetItemInfo( m_itemID, pPetInfo->m_cPromotion );
    }
    else
    {
        m_pItemImgWnd->SetItemInfo(m_itemID, -1, bSetItem, iSetNumber, m_itemUID, -1, kItem->m_DesignCoordiID );
    }
    
    
    m_pItemImgWnd->ToggleRender( true );
    m_pItemImgWnd->ShowItem( true );

    for ( int i = 0; i < KItem::GRADE_NUM; ++i ) 
    {
        if ( m_pkGradeFrame[i] )
            m_pkGradeFrame[i]->ToggleRender( false );        
    }
    if ( m_pkGradeFrame[cGrade] )
        m_pkGradeFrame[cGrade]->ToggleRender( true );


    SetItemCount( kItem->m_nCount );
    SetItemStrong( pItem->eItemKind == GIK_STRONG_ITEM, pItem->iStrongLevel );
    SetEnchantLevel( kItem );
    SetCoordiCompose( kItem->m_DesignCoordiID != 0 );
    SetColor( -1 );
    SetEquip( kItem );
}

void KGCCommonItemWnd::SetItem( KPetInfo* pPetInfo )
{
    if ( !pPetInfo )
        return;

    m_itemID = pPetInfo->m_dwID;
    m_itemUID = pPetInfo->m_dwUID;

    GCItem* pItem = g_pItemMgr->GetItemData(m_itemID);
    if (!pItem)
        return;

    bool bSetItem = (pItem->dwSlotPosition & ESP_SET_ITEM);
    int iSetNumber = pItem->iSetItemNumber;
    char cGrade = pItem->cItemGrade;

    m_pItemImgWnd->SetPetItemInfo(m_itemID, pPetInfo->m_cPromotion);

    m_pItemImgWnd->ToggleRender(true);
    m_pItemImgWnd->ShowItem(true);

    for (int i = 0; i < KItem::GRADE_NUM; ++i)
    {
        if (m_pkGradeFrame[i])
            m_pkGradeFrame[i]->ToggleRender(false);
    }
    if (m_pkGradeFrame[cGrade])
        m_pkGradeFrame[cGrade]->ToggleRender(true);

    SetColor(-1);
}

void KGCCommonItemWnd::SetItem(KItem* kItem, char cGrade, KPetInfo* pPetInfo)
{
    if (!kItem)
        return;

    m_itemID = kItem->m_ItemID;
    m_itemUID = kItem->m_ItemUID;

    GCItem* pItem = g_pItemMgr->GetItemData(m_itemID);
    if (!pItem)
        return;

    bool bSetItem = (pItem->dwSlotPosition & ESP_SET_ITEM);
    int iSetNumber = pItem->iSetItemNumber;

    if (pPetInfo)
    {
        m_pItemImgWnd->SetPetItemInfo(m_itemID, pPetInfo->m_cPromotion);
    }
    else
    {
        m_pItemImgWnd->SetItemInfo(m_itemID, -1, bSetItem, iSetNumber, m_itemUID, -1, kItem->m_DesignCoordiID);
    }

    m_pItemImgWnd->ToggleRender(true);
    m_pItemImgWnd->ShowItem(true);

    for (int i = 0; i < KItem::GRADE_NUM; ++i)
    {
        if (m_pkGradeFrame[i])
            m_pkGradeFrame[i]->ToggleRender(false);
    }
    if (m_pkGradeFrame[cGrade])
        m_pkGradeFrame[cGrade]->ToggleRender(true);

    SetItemCount(kItem->m_nCount);
    SetItemStrong(pItem->eItemKind == GIK_STRONG_ITEM, pItem->iStrongLevel);
    SetEnchantLevel(kItem);
    SetCoordiCompose(kItem->m_DesignCoordiID != 0);
    SetColor(-1);
    SetEquip(kItem);
}

void KGCCommonItemWnd::SetItem( GCItem* pItem )
{
    if( !pItem )
        return;

    m_itemID = pItem->dwGoodsID * 10;
    m_itemUID = 0;

    bool bSetItem = (pItem->dwSlotPosition&ESP_SET_ITEM);
    int iSetNumber = pItem->iSetItemNumber;
    char cGrade = pItem->cItemGrade;

    m_pItemImgWnd->SetItemInfo( m_itemID, -1, bSetItem, iSetNumber );
    m_pItemImgWnd->ToggleRender( true );
    m_pItemImgWnd->ShowItem( true );

    for ( int i = 0; i < KItem::GRADE_NUM; ++i ) 
    {
        if ( m_pkGradeFrame[i] )
            m_pkGradeFrame[i]->ToggleRender( false );        
    }

    if ( m_pkGradeFrame[cGrade] )
        m_pkGradeFrame[cGrade]->ToggleRender( true );

    SetItemStrong(pItem->eItemKind == GIK_STRONG_ITEM, pItem->iStrongLevel);

    if ( m_pkSmallDurationNum ) 
        m_pkSmallDurationNum->ToggleRender( false );

    if ( m_pkEquipEnchantItemLv )
        m_pkEquipEnchantItemLv->ToggleRender( false );

    if ( m_pkCoordiComposeImg )
        m_pkCoordiComposeImg->ToggleRender( false );

    if ( m_pkEquipImg )
        m_pkEquipImg->ToggleRender( false );

    SetColor( -1 );
}

void KGCCommonItemWnd::SetItemCount( int nCount )
{
    if( !m_pkSmallDurationNum )
        return;

    if( nCount > -1 ) {
        m_pkSmallDurationNum->ToggleRender( true );
        m_pkSmallDurationNum->SetNumber( nCount );
    }
    else {
        m_pkSmallDurationNum->ToggleRender( false );
    }
}

void KGCCommonItemWnd::SetItemStrong( bool bStrong, int nStrong )
{    
    if( !m_pkStrongNum )
        return;

    m_pkStrongNum->ToggleRender( false );
    if( nStrong != 0 || bStrong ) {
        m_pkStrongNum->SetNumber( nStrong );
        m_pkStrongNum->ToggleRender( true );
    } 
}

void KGCCommonItemWnd::SetEnchantLevel( KItem* kItem )
{
    if( !m_pkEquipEnchantItemLv )
        return;

    if ( !kItem )
        return;

    m_pkEquipEnchantItemLv->ToggleRender( false );

    GCItem* pItem = g_pItemMgr->GetItemData( m_itemID );
    if ( !pItem )
        return;

    bool bCheck = false;
    if( pItem->eItemKind == GIK_ENCHANT_ENABLE_ITEM )
        bCheck = true;
    else if( pItem->eItemKind != GIK_ENCHANT_ENABLE_ITEM && kItem->m_EnchantEquipItemUID != 0 )
        bCheck = true;

    if( bCheck )
    {
        m_pkEquipEnchantItemLv->ToggleRender(true);

        if( pItem->eItemKind == GIK_ENCHANT_ENABLE_ITEM )
        {
            if( kItem->m_cGradeID == KItem::GRADE_NORMAL )
                m_pkEquipEnchantItemLv->SetStringType("normal_num_enchant");
            else if( kItem->m_cGradeID == KItem::GRADE_EPIC )
                m_pkEquipEnchantItemLv->SetStringType("epic_num_enchant");
        }
        else
        {
            if( kItem->m_cEnchantEquipGradeID == KItem::GRADE_NORMAL )
                m_pkEquipEnchantItemLv->SetStringType("normal_num_enchant");
            else if( kItem->m_cEnchantEquipGradeID == KItem::GRADE_EPIC )
                m_pkEquipEnchantItemLv->SetStringType("epic_num_enchant");
        }

        m_pkEquipEnchantItemLv->SetNumber(kItem->m_cEnchantLevel);
    }
    else
        m_pkEquipEnchantItemLv->ToggleRender(false);

}

void KGCCommonItemWnd::ResetItem()
{
    m_itemID = 0;
    m_itemUID = 0;

    m_pItemImgWnd->ToggleRender( false );
    m_pItemImgWnd->ShowItem( false );

    if ( m_pkSmallDurationNum )
        m_pkSmallDurationNum->ToggleRender( false );

    if ( m_pkStrongNum )
        m_pkStrongNum->ToggleRender( false );

    if ( m_pkEquipEnchantItemLv )
        m_pkEquipEnchantItemLv->ToggleRender( false );

    for ( int i = 0; i < KItem::GRADE_NUM; ++i ) 
    {
        if ( m_pkGradeFrame[i] )
            m_pkGradeFrame[i]->ToggleRender( false );        
    }

    if ( m_pkCoordiComposeImg )
        m_pkCoordiComposeImg->ToggleRender( false );

    if ( m_pkEquipImg )
        m_pkEquipImg->ToggleRender( false );
}

void KGCCommonItemWnd::SelectedItem( bool bShow )
{
    if ( m_pkSelItemImg )
        m_pkSelItemImg->ToggleRender( bShow );
}


void KGCCommonItemWnd::SetCoordiCompose( bool bShow )
{
    if ( m_pkCoordiComposeImg )
        m_pkCoordiComposeImg->ToggleRender( bShow );
}

void KGCCommonItemWnd::SetColor( IN const DWORD dwColor_ )
{
    m_pItemImgWnd->SetColor( dwColor_ );
}

void KGCCommonItemWnd::SetEquip( KItem* kItem )
{
    if ( m_pkEquipImg )
    {
        bool bEquip = g_pItemMgr->IsEquipedItem( &g_kGlobalValue.m_kUserInfo.GetCurrentChar(), kItem->m_ItemUID );
        m_pkEquipImg->ToggleRender( bEquip );
    }
}