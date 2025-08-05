#include "stdafx.h"
#include "GCHeroItemUpgradeMaterial.h"

IMPLEMENT_CLASSNAME( KGCHeroItemUpgradeMaterial );
IMPLEMENT_WND_FACTORY( KGCHeroItemUpgradeMaterial );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroItemUpgradeMaterial, "gc_hero_item_upgrade_material" );

#define ENABLE_ITEM_COLOR D3DCOLOR_ARGB( 255, 255, 255, 255 )
#define DISABLE_ITEM_COLOR D3DCOLOR_ARGB( 255, 130, 130, 130 )

KGCHeroItemUpgradeMaterial::KGCHeroItemUpgradeMaterial( void )
{
    m_pkItemImg = NULL;
    m_pkStaticItemCount = NULL;
    LINK_CONTROL( "item_img",     m_pkItemImg );
    LINK_CONTROL( "static_count", m_pkStaticItemCount );

    m_bShowItemInfo = false;

    m_pkBtnSelect = NULL;
    LINK_CONTROL( "btn_select", m_pkBtnSelect );
}

KGCHeroItemUpgradeMaterial::~KGCHeroItemUpgradeMaterial( void )
{
}

void KGCHeroItemUpgradeMaterial::OnCreate( void )
{
    m_pkItemImg->InitState( false, true, this );
    m_pkItemImg->DisconnectAllEventProcedure();
    m_pkItemImg->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCHeroItemUpgradeMaterial::OnClickSelect );

    m_pkStaticItemCount->SetFontColor( 0xffffffff );
    m_pkStaticItemCount->SetShadow( true );
    m_pkStaticItemCount->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
    m_pkStaticItemCount->SetAlign( DT_RIGHT );
    m_pkStaticItemCount->InitState( false );

    m_pkBtnSelect->InitState( false, true, this );
    m_pkBtnSelect->DisconnectAllEventProcedure();
    m_pkBtnSelect->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCHeroItemUpgradeMaterial::OnClickSelect );

    ClearInfo();
}

void KGCHeroItemUpgradeMaterial::ActionPerformed( const KActionEvent& event )
{
}

void KGCHeroItemUpgradeMaterial::FrameMoveInEnabledState( void )
{
    ShowItemInfo();
}

void KGCHeroItemUpgradeMaterial::ClearInfo( void )
{
    m_pkItemImg->ToggleRender( false );
    m_pkItemImg->SetItemInfo( KItem::INVALID_ITEM );
    m_pkStaticItemCount->ToggleRender( false );

    m_kMaterialInfo.m_ItemID = KItem::INVALID_ITEM;
    m_pInvenItem = NULL;

    m_pkBtnSelect->ToggleRender( false );
}

void KGCHeroItemUpgradeMaterial::SetInfo( const KDropItemInfo& Info_ )
{
    ClearInfo();

    if( Info_.m_ItemID == KItem::INVALID_ITEM ) { 
        return;
    }

    m_kMaterialInfo = Info_;
    m_pInvenItem = GetInventoryItem();

    if( m_kMaterialInfo.m_nDuration == -1 && m_kMaterialInfo.m_nPeriod == -1 ) { 
        bool bEnable = ( m_pInvenItem && m_pInvenItem->m_cGradeID == m_kMaterialInfo.m_cGradeID );
        m_pkItemImg->SetItemInfo( m_kMaterialInfo.m_ItemID/10, bEnable ? ENABLE_ITEM_COLOR : DISABLE_ITEM_COLOR );
    }
    else { 
         bool bEnable = ( m_pInvenItem && m_pInvenItem->m_nCount >= m_kMaterialInfo.m_nDuration );
        m_pkItemImg->SetItemInfo( m_kMaterialInfo.m_ItemID/10, bEnable ? ENABLE_ITEM_COLOR : DISABLE_ITEM_COLOR );
    }
    m_pkItemImg->ShowItem( true );
    m_pkItemImg->ToggleRender( true );

    if( m_kMaterialInfo.m_nDuration > 0 ) { 
        m_pkStaticItemCount->SetText( m_kMaterialInfo.m_nDuration );
        m_pkStaticItemCount->ToggleRender( true );
    }

    if( m_pInvenItem && m_pInvenItem->m_cGradeID == m_kMaterialInfo.m_cGradeID && m_kMaterialInfo.m_nDuration == -1 && m_kMaterialInfo.m_nPeriod == -1 ) { 
        m_pkBtnSelect->ToggleRender( true );
    }
}

void KGCHeroItemUpgradeMaterial::SetInfo( GCITEMUID itemUID_ )
{
    KDropItemInfo tempInfo = m_kMaterialInfo;
    ClearInfo();

    if( itemUID_ == 0 ) { 
        return;
    }

    m_kMaterialInfo = tempInfo;
    g_pItemMgr->FindInventory( itemUID_, &m_pInvenItem );
    if( m_pInvenItem == NULL || m_pInvenItem->m_ItemID != m_kMaterialInfo.m_ItemID/10 || m_pInvenItem->m_cGradeID != m_kMaterialInfo.m_cGradeID ) { 
        ClearInfo();
        return;
    }

    m_pkItemImg->SetItemInfo( m_kMaterialInfo.m_ItemID/10, ENABLE_ITEM_COLOR );
    m_pkItemImg->ShowItem( true );
    m_pkItemImg->ToggleRender( true );
    
    m_pkBtnSelect->ToggleRender( true );
}


std::pair< bool, GCITEMUID > KGCHeroItemUpgradeMaterial::GetMaterialUID( void )
{
    if( m_pInvenItem == NULL ) { 
        return std::pair< bool, GCITEMUID >( false, 0 );
    }

    if( m_kMaterialInfo.m_nDuration == -1 && m_kMaterialInfo.m_nPeriod == -1 && m_pInvenItem->m_cGradeID != m_kMaterialInfo.m_cGradeID ) { 
        return std::pair< bool, GCITEMUID >( false, 0 );
    }

    if( m_kMaterialInfo.m_nDuration != -1 && m_kMaterialInfo.m_nDuration > m_pInvenItem->m_nCount ) { 
        return std::pair< bool, GCITEMUID >( false, m_pInvenItem->m_ItemUID );
    }

    return std::pair< bool, GCITEMUID >( true, m_pInvenItem->m_ItemUID  );
}

void KGCHeroItemUpgradeMaterial::ShowItemInfo( void )
{   
    if( g_pkUIScene->m_pkHeroDungeonDlg->m_pkItemUpgrade->IsRenderOnMaterial() ) { 
        return;
    }

    if( false == m_pkItemImg->CheckMousePosInWindowBound() ) { 
        return;
    }

    GCItem* pItem = g_pItemMgr->GetItemData( m_kMaterialInfo.m_ItemID/10 );
    if( NULL == pItem ) { 
        return;
    }

    bool bShowInvenItem = false;
    bool bSetGrade = false;
    if( m_pInvenItem ) {    // 인벤토리에 재료 아이템 있음 
        if( m_kMaterialInfo.m_nDuration == -1 && m_kMaterialInfo.m_nPeriod == -1 ) {    // 영구
            bShowInvenItem = true;
            bSetGrade = true;
        }
        else {      // 수량
            bShowInvenItem = true;
            bSetGrade = false;
        }
    }
    else { // 인벤토리에 재료 아이템 없음
        if( m_kMaterialInfo.m_nDuration == -1 && m_kMaterialInfo.m_nPeriod == -1 ) {    // 영구
            bShowInvenItem = false;
            bSetGrade = true;
        }
        else {  // 수량
            bShowInvenItem = false;
            bSetGrade = false;
        }
    }


    D3DXVECTOR2 dxvPos = m_pkItemImg->GetRelocatedWindowPos();
    dxvPos.y += 30.0f;

    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, bShowInvenItem ? m_pInvenItem : NULL, NULL, NULL, bSetGrade ? m_kMaterialInfo.m_cGradeID : -1 );
    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( dxvPos );
    g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
    g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
    g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);   
}

void KGCHeroItemUpgradeMaterial::OnClickSelect( void )
{
    if( m_pkBtnSelect->IsRenderOn() == false ) { 
        return;
    }

    if( m_kMaterialInfo.m_ItemID == KItem::INVALID_ITEM ) { 
        return;
    }

    if( m_kMaterialInfo.m_nDuration != -1 ) { 
        return;
    }

    if( g_pkUIScene->m_pkHeroDungeonDlg->m_pkItemUpgrade->IsRenderOnMaterial() ) { 
        g_pkUIScene->m_pkHeroDungeonDlg->m_pkItemUpgrade->CloseMaterialBox();
    }

    D3DXVECTOR2 dxvPos = m_pkItemImg->GetCurrentWindowPos();

    //float fX, fY;
    //KLuaManager luaMgr;
    //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
    //LUA_GET_VALUE_DEF( "X", fX, 0.0f );
    //LUA_GET_VALUE_DEF( "Y", fY, 0.0f );

    dxvPos.x -= 145.0f;
    dxvPos.y -= 70.0f;
    
    g_pkUIScene->m_pkHeroDungeonDlg->m_pkItemUpgrade->ShowMateriaBox( dxvPos, 
        static_cast<KGCHeroItemUpgradeBox*>( this->GetParent() )->GetBoxNumber(), GetSlotNumber(), m_kMaterialInfo.m_ItemID/10, m_kMaterialInfo.m_cGradeID );
}

KItem* KGCHeroItemUpgradeMaterial::GetInventoryItem( void )
{
    if( m_kMaterialInfo.m_ItemID == KItem::INVALID_ITEM ) { 
        return NULL;
    }

    if( m_kMaterialInfo.m_nDuration != -1 ) { // 수량
        KItem* pInvenItem = NULL;
        g_pItemMgr->FindInventoryForItemID( m_kMaterialInfo.m_ItemID/10, &pInvenItem );
        return pInvenItem;
    }
    else {  //영구
        std::vector< KItem* > vecInvenItem;
        if( false == g_pItemMgr->GetInventoryItemListFromItemID( m_kMaterialInfo.m_ItemID/10, vecInvenItem ) ) { 
            return NULL;
        }

        KItem* pInvenItem = NULL;
        std::vector< KItem* >::iterator it = vecInvenItem.begin();
        for( ; it != vecInvenItem.end() ; ++it ) {
            // 코디 제외 
            if( (*it)->CheckItemType( KItem::TYPE_LOOK ) ) { 
                continue;
            }

            if( (*it)->m_cGradeID == m_kMaterialInfo.m_cGradeID ) { 
                pInvenItem = (*it);
                break;
            }
        }

        return pInvenItem;
    }
}