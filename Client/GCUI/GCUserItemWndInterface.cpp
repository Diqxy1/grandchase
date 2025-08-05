#include "stdafx.h"
#include "GCItemImgWnd.h"
#include "GCUserItemWndInterface.h"

IMPLEMENT_CLASSNAME( KGCUserItemWndInterface );
IMPLEMENT_WND_FACTORY( KGCUserItemWndInterface );
IMPLEMENT_WND_FACTORY_NAME( KGCUserItemWndInterface, "gc_useritemwndinterface" );

KGCUserItemWndInterface::KGCUserItemWndInterface( void )
: m_ItemInfo(NULL)
, m_pkItemImg( NULL )
{
    LINK_CONTROL( "item_img",           m_pkItemImg );

    m_bHotItem = false;
    m_bNewItem = false;

#if defined( USE_ONGAME_INVENTORY )
    m_pkIconNewItem = NULL;
    m_pkIconHotItem = NULL;
    LINK_CONTROL( "icon_hotItem", m_pkIconHotItem );
    LINK_CONTROL( "icon_newItem", m_pkIconNewItem );
#endif
}

KGCUserItemWndInterface::~KGCUserItemWndInterface( void )
{
}

void KGCUserItemWndInterface::SetItem( KItem* pInventoryItem )
{
    m_kInvenItem = *pInventoryItem;
    m_ItemInfo = g_pItemMgr->GetItemData( pInventoryItem->m_ItemID );

    SetItemImg( pInventoryItem );
    m_pkItemImg->ToggleRender( true );
    m_pkItemImg->ShowItem( true );

    OnSetItem( pInventoryItem );
}

void KGCUserItemWndInterface::SetItem( GCItem* pItem )
{
    if( !pItem ) 
        return;
    m_kItem = *pItem;
    m_pkItemImg->ToggleRender( true );
    m_pkItemImg->ShowItem( true );
    SetItemImg( pItem );
}

void KGCUserItemWndInterface::SetItem()
{
    m_pkItemImg->ToggleRender( false );
    OnSetItem();
}

void KGCUserItemWndInterface::SetItemImg( KItem* pInventoryItem )
{
    GCItem* pItemInfo = g_MyD3D->m_kItemMgr.GetItemData( pInventoryItem->m_ItemID );
    if ( !pItemInfo ) return;

    if( pItemInfo->eItemKind == GIK_PET && pItemInfo->ePetItemKind == EPET_KIND_PET )
    {
        std::map<GCITEMUID, KPetInfo>::iterator mit = g_MyD3D->m_mapPetInfo.find( pInventoryItem->m_ItemUID );

        // 펫이지만 펫 정보가 없다면 일반 텍스쳐를 로딩한다
        if( mit == g_MyD3D->m_mapPetInfo.end() )
        {
            m_pkItemImg->SetItemInfo( pInventoryItem->m_ItemID );
        }
        else
        {
            // 펫 정보가 있다면 펫의 진화레벨에 따라서 다른 이미지를 보여준다
            KPetInfo kPetInfo = mit->second;
            m_pkItemImg->SetPetItemInfo( pInventoryItem->m_ItemID, kPetInfo.m_cPromotion );
        }
    }
    else
    {
        m_pkItemImg->SetItemInfo( pInventoryItem->m_ItemID, -1, (pItemInfo->dwSlotPosition&ESP_SET_ITEM), pItemInfo->iSetItemNumber, 0, -1, pInventoryItem->m_DesignCoordiID );
    }
}

void KGCUserItemWndInterface::SetItemImg( GCItem* pItem )
{
    m_pkItemImg->SetItemInfo( pItem->dwGoodsID, -1, (pItem->dwSlotPosition&ESP_SET_ITEM), pItem->iSetItemNumber );
}

void KGCUserItemWndInterface::OnCreate( void )
{
    //m_pkItemImg->InitState( true );
    m_bNewItem = false;
    m_bHotItem = false;

#if defined( USE_ONGAME_INVENTORY )
    m_pkIconNewItem->InitState( false );
    m_pkIconHotItem->InitState( false );
#endif
}

#if defined( USE_ONGAME_INVENTORY )
void KGCUserItemWndInterface::OnSetItemState( bool bHot_ /*= false*/, bool bNew_ /*= false*/ )
{
    m_bHotItem = bHot_;
    m_bNewItem = bNew_;

    if( m_pkIconNewItem && m_pkIconHotItem ) { 
        if( m_bHotItem ) { 
            m_pkIconHotItem->ToggleRender( true );
            m_pkIconNewItem->ToggleRender( false );
        }
        else if( m_bNewItem ) { 
            m_pkIconHotItem->ToggleRender( false );
            m_pkIconNewItem->ToggleRender( true );
        }
        else { 
            m_pkIconHotItem->ToggleRender( false );
            m_pkIconNewItem->ToggleRender( false );
        }
    }
}
#endif