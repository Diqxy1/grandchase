#include "stdafx.h"
#include "GCHeroItemUpgradeBox.h"

IMPLEMENT_CLASSNAME( KGCHeroItemUpgradeBox );
IMPLEMENT_WND_FACTORY( KGCHeroItemUpgradeBox );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroItemUpgradeBox, "gc_hero_item_upgrade_box" );

KGCHeroItemUpgradeBox::KGCHeroItemUpgradeBox( void )
{
    m_pkItemImg = NULL;
    m_pkStaticItemName = NULL;
    m_pkStaticItemType = NULL;
    m_pkBtnUpgrade = NULL;
    LINK_CONTROL( "item_img",           m_pkItemImg );
    LINK_CONTROL( "static_item_name",   m_pkStaticItemName );
    LINK_CONTROL( "static_item_type",   m_pkStaticItemType );
    LINK_CONTROL( "btn_upgrade",        m_pkBtnUpgrade );

    for( int i=0 ; i<MAX_MATERIAL ; ++i ) {
        m_apkMaterial[i] = NULL;
        
        char szTemp[ MAX_PATH ];
        sprintf_s( szTemp, MAX_PATH - 1, "hero_item_upgrade_material%d", i );
        LINK_CONTROL( szTemp, m_apkMaterial[i] );
    }

    m_bShowItemInfo = false;
    m_vecMaterialUID.clear();
}

KGCHeroItemUpgradeBox::~KGCHeroItemUpgradeBox( void )
{
}

void KGCHeroItemUpgradeBox::OnCreate( void )
{
    m_pkItemImg->InitState( true, true, this );
    m_pkBtnUpgrade->InitState( true, true, this );

    m_pkStaticItemName->SetFontColor(0xffffffff);
    m_pkStaticItemName->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticItemName->SetAlign(DT_LEFT);
    m_pkStaticItemName->SetShadow(true);
    m_pkStaticItemName->InitState(true, false, this);

    m_pkStaticItemType->SetFontColor(0xffffffff);
    m_pkStaticItemType->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticItemType->SetAlign(DT_LEFT);
    m_pkStaticItemType->InitState(true, false, this);

    m_pkBtnUpgrade->InitState( true, true, this );
    m_pkBtnUpgrade->DisconnectAllEventProcedure();
    m_pkBtnUpgrade->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCHeroItemUpgradeBox::OnClickUpgrade );

    for( int i=0 ; i<MAX_MATERIAL ; ++i ) { 
        m_apkMaterial[i]->SetSlotNumber( i );
    }

    m_bShowItemInfo = false;
}

void KGCHeroItemUpgradeBox::ActionPerformed( const KActionEvent& event )
{
}

void KGCHeroItemUpgradeBox::FrameMoveInEnabledState( void )
{
    ShowItemInfo();    
}

void KGCHeroItemUpgradeBox::PostChildDraw( void )
{
}

void KGCHeroItemUpgradeBox::ClearInfo( void )
{
    m_pkItemImg->ShowItem( false );
    m_pkItemImg->SetItemInfo( KItem::INVALID_ITEM );

    m_kItemInfo.m_ItemID = KItem::INVALID_ITEM;
    m_kItemInfo.m_vecMaterials.clear();

    m_pkStaticItemName->ToggleRender( false );
    m_pkStaticItemType->ToggleRender( false );

    m_bShowItemInfo = false;
}

void KGCHeroItemUpgradeBox::SetItemInfo( const IN KHeroItemUpgradeInfo& Info_ )
{
    ClearInfo();

    if( Info_.m_vecMaterials.empty() || Info_.m_ItemID == 0 ) { 
        return;
    }

    // 완성품
    m_kItemInfo = Info_;
    GCItem* pItemData = g_pItemMgr->GetItemData( m_kItemInfo.m_ItemID / 10 );
    if( pItemData == NULL ) { 
        return;
    }

    m_pkStaticItemName->SetFontColor( g_pItemMgr->GetItemGradeColorRGB(m_kItemInfo.m_nGrade) );
    m_pkStaticItemName->SetForceWordWrapText( pItemData->strItemName, true );
    m_pkStaticItemType->SetText( g_pItemMgr->GetItemTypeString( pItemData ) );
    m_pkStaticItemName->ToggleRender( true );
    m_pkStaticItemType->ToggleRender( true );

    m_pkItemImg->SetItemInfo( pItemData->dwGoodsID );
    m_pkItemImg->ShowItem( true ) ;
    m_pkItemImg->ToggleRender( true );

    UpdateMaterialInfo();
}

void KGCHeroItemUpgradeBox::UpdateMaterialInfo( void )
{
    if( m_kItemInfo.m_ItemID == KItem::INVALID_ITEM ) { 
        return;
    }

    // 재료
    for( int i=0 ; i<MAX_MATERIAL ; ++i ) { 
        m_apkMaterial[i]->ClearInfo();

        if( static_cast<int>( m_kItemInfo.m_vecMaterials.size() ) -1 < i ) { 
            continue;
        }
        else { 
            m_apkMaterial[i]->SetInfo( m_kItemInfo.m_vecMaterials[i] );
        }
    }
}

void KGCHeroItemUpgradeBox::OnClickUpgrade( void )
{
    g_pkUIScene->m_pkHeroDungeonDlg->m_pkItemUpgrade->CloseMaterialBox();

    // < 만들 아이템 ID, 만들 아이템 등급 >
    //std::pair< GCITEMID, int > prKey( m_kItemInfo.m_ItemID, m_kItemInfo.m_nGrade );
    m_vecMaterialUID.clear();
    for( int i=0 ; i<MAX_MATERIAL ; ++i ) { 
        std::pair< bool, GCITEMUID > prUID = m_apkMaterial[i]->GetMaterialUID();

        if( prUID.first == false || prUID.second == 0 ) { 
            continue;
        }
        
        m_vecMaterialUID.push_back( prUID.second );
    }

    if( m_vecMaterialUID.empty() || m_kItemInfo.m_vecMaterials.size() > m_vecMaterialUID.size() ) { 
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ITEM_CREATION_ERROR4 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    }

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_HERO_ITEM_UPGRADE_CONFIRM ), L"",
                             KGCUIScene::GC_MBOX_USE_BUY_HERO_ITEM_UPGRADE, m_kItemInfo.m_ItemID, m_kItemInfo.m_nGrade, true, true);
}

void KGCHeroItemUpgradeBox::ShowItemInfo( void )
{
    if( false == m_pkItemImg->CheckMousePosInWindowBound() ) { 
        return;
    }

    GCItem* pItem = g_pItemMgr->GetItemData( m_kItemInfo.m_ItemID/10 );
    if( NULL == pItem ) { 
        return;
    }

    D3DXVECTOR2 dxvPos = m_pkItemImg->GetRelocatedWindowPos();
    dxvPos.x += 10.0f;
    dxvPos.y += 50.0f;

    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, NULL, NULL, NULL, m_kItemInfo.m_nGrade );
    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( dxvPos );
    g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
    g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();
    g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_TOPMOST);   
}

void KGCHeroItemUpgradeBox::SetSelectMaterial( int iSlotNumber_, GCITEMUID itemUID_ )
{
    if( iSlotNumber_ < 0 || iSlotNumber_ > MAX_MATERIAL - 1 ) { 
        return;
    }

    m_apkMaterial[ iSlotNumber_ ]->SetInfo( itemUID_ );
}

std::pair< GCITEMID, int > KGCHeroItemUpgradeBox::GetUpgradeItemKey( void )
{
    if( m_kItemInfo.m_ItemID == KItem::INVALID_ITEM ) { 
        return std::pair< GCITEMID, int >( 0, 0 );
    }

    return std::pair< GCITEMID, int >( m_kItemInfo.m_ItemID, m_kItemInfo.m_nGrade );
}

bool KGCHeroItemUpgradeBox::GetUpgradeMaterialUID( OUT std::vector< GCITEMUID >& vecUIDList_ )
{
    vecUIDList_.clear();

    if( m_kItemInfo.m_ItemID == KItem::INVALID_ITEM ) { 
        return false;
    }

    if( m_vecMaterialUID.empty() ) { 
        return false;
    }

    vecUIDList_ = m_vecMaterialUID;
    return true;
}

