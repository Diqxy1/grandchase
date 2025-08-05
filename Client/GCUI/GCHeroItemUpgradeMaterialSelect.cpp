#include "stdafx.h"
#include "GCHeroItemUpgradeMaterial.h"

IMPLEMENT_CLASSNAME( KGCHeroItemUpgradeMaterialSelect );
IMPLEMENT_WND_FACTORY( KGCHeroItemUpgradeMaterialSelect );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroItemUpgradeMaterialSelect, "gc_hero_upgrade_material_select" );

KGCHeroItemUpgradeMaterialSelect::KGCHeroItemUpgradeMaterialSelect( void )
: m_iItemCount( 0 )
, m_iSlotLineCount( 0 )
, m_pItemData( NULL )
, m_SelectItemUID( 0 )
, m_pkBackGround( NULL )
, m_dxvPos( 0.0f, 0.0f )
, m_iUpgradeSlot( -1 )
, m_iMaterialSlot( -1 )
{
    LINK_CONTROL( "background", m_pkBackGround );

    m_vecInvenItem.clear();
    m_vecItemBox.clear();
    m_vecItemImg.clear();
}

KGCHeroItemUpgradeMaterialSelect::~KGCHeroItemUpgradeMaterialSelect( void )
{

}

void KGCHeroItemUpgradeMaterialSelect::OnCreate( void )
{
    m_pkBackGround->InitState( true );

    std::vector< KD3DWnd* > vecBox = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_hero_dungeon_dlg.stg", "", "hero_dungeon_dlg\\hero_dungeon_upgrade\\material_select_list\\box", true, NUM_MAX_SLOT );
    std::vector< KD3DWnd* > vecImg = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_hero_dungeon_dlg.stg", "", "hero_dungeon_dlg\\hero_dungeon_upgrade\\material_select_list\\item_img", true, NUM_MAX_SLOT );

    std::vector< KD3DWnd* >::iterator itBox = vecBox.begin();
    std::vector< KD3DWnd* >::iterator itImg = vecImg.begin();


    for( ; itBox != vecBox.end() ; ++itBox, ++itImg ) { 
        m_vecItemBox.push_back( std::make_pair( 0, *itBox ) );
        m_vecItemImg.push_back( std::make_pair( 0, static_cast< KGCItemImgWnd* >( *itImg ) ) );
    }
}


void KGCHeroItemUpgradeMaterialSelect::SetInfo(  int iUpgradeSlot_, int iMaterialSlot_, GCITEMID itemID_, int iGrade_ )
{
    ClearData();
    m_pItemData = g_pItemMgr->GetItemData( static_cast< GCITEMID >( itemID_) );
    if( m_pItemData == NULL ) { 
        return;
    }

    if( false == g_pItemMgr->GetInventoryItemListFromItemID( m_pItemData->dwGoodsID, m_vecInvenItem ) ) { 
        return;
    }

    std::vector< KItem* >::iterator it = m_vecInvenItem.begin();
    for( ; it != m_vecInvenItem.end() ; ) { 
        if( (*it)->m_cItemType !=  KItem::TYPE_NORMAL ){ 
            m_vecInvenItem.erase( it );
            continue;
        }

        if( (*it)->m_cGradeID != iGrade_ ) { 
            m_vecInvenItem.erase( it );
            continue;
        }

        ++it;
    }

    m_iUpgradeSlot = iUpgradeSlot_;
    m_iMaterialSlot = iMaterialSlot_;
    
    this->SetWindowPosDirect( m_dxvPos );
    InitUI();
}

void KGCHeroItemUpgradeMaterialSelect::ActionPerformed( const KActionEvent& event )
{

}

void KGCHeroItemUpgradeMaterialSelect::FrameMoveInEnabledState( void )
{
    ShowItemInfo();
}

void KGCHeroItemUpgradeMaterialSelect::InitUI( void )
{
    MakeItemList();
    SetBackGroundHeigt();
    SetItemPos();
    
    this->ToggleRender( true );    
}

void KGCHeroItemUpgradeMaterialSelect::OnClickItem( const KActionEvent& event )
{
    std::vector< std::pair< GCITEMUID, KGCItemImgWnd* > >::const_iterator it = m_vecItemImg.begin();
    for( ; it != m_vecItemImg.end() ; ++it ) { 
        if( it->second == event.m_pWnd ) { 
            g_pkUIScene->m_pkHeroDungeonDlg->m_pkItemUpgrade->SetSelectMaterial( m_iUpgradeSlot, m_iMaterialSlot, it->first );
            break;
        }
    }

    ClearData();
    this->ToggleRender( false );
}

void KGCHeroItemUpgradeMaterialSelect::OnClose( void )
{
    ClearData();
    this->ToggleRender( false );
}

void KGCHeroItemUpgradeMaterialSelect::ClearData( void )
{
    m_pItemData = NULL;
    m_vecInvenItem.clear();

    m_iItemCount = 0;
    m_iSlotLineCount = 0;

    m_iUpgradeSlot = -1;
    m_iMaterialSlot = -1;

    std::vector< std::pair< GCITEMUID, KD3DWnd* > >::iterator itBox = m_vecItemBox.begin();
    std::vector< std::pair< GCITEMUID, KGCItemImgWnd* > >::iterator itImg = m_vecItemImg.begin();

    for( ; itBox != m_vecItemBox.end() ; ++itBox ) { 
        itBox->first = 0;
        itBox->second->ToggleRender( false );
    }

    for( ; itImg != m_vecItemImg.end() ; ++itImg ) { 
        itImg->first = 0;
        itImg->second->ToggleRender( false );
    }
}

void KGCHeroItemUpgradeMaterialSelect::MakeItemList( void )
{
    if( m_vecInvenItem.empty() ) { 
        return ;
    }

    m_iItemCount = static_cast<int>( m_vecInvenItem.size() );
    m_iSlotLineCount = m_iItemCount / NUM_SLOT_LINE;
    if( m_iItemCount % NUM_SLOT_LINE ) { 
        ++m_iSlotLineCount;
    }

    int iNumItemSlot = static_cast<int>( m_vecInvenItem.size() );

    std::vector< std::pair< GCITEMUID, KD3DWnd* > >::iterator itBox = m_vecItemBox.begin();
    std::vector< std::pair< GCITEMUID, KGCItemImgWnd* > >::iterator itImg = m_vecItemImg.begin();
    for( int i = 0 ; i< m_iItemCount ; ++i, ++itBox, ++itImg ) { 

        if( i > NUM_MAX_SLOT ) { 
            break;
        }

        itBox->first = m_vecInvenItem[i]->m_ItemUID;
        itImg->first = m_vecInvenItem[i]->m_ItemUID;
    }
}

void KGCHeroItemUpgradeMaterialSelect::SetBackGroundHeigt( void )
{
    if( m_iSlotLineCount <= 0 ) { 
        return;
    }

    if( m_iSlotLineCount == 1 ) { 
        m_pkBackGround->SetHeight( BACKGROUND_START_HEIGHT );
    }
    else { 
        DWORD dwHeight = BACKGROUND_START_HEIGHT + ( ROUND_OFFSET * ( m_iSlotLineCount - 1 ) ) + ( BOX_HEIGHT * ( m_iSlotLineCount - 1 ) );
        m_pkBackGround->SetHeight( dwHeight );
    }
}

void KGCHeroItemUpgradeMaterialSelect::SetItemPos( void )
{
    for( int i=0 ; i<m_iSlotLineCount ; ++i ) { 
        for( int j=0 ; j<NUM_SLOT_LINE ; ++j ) { 

            int iIndex = j + ( i * NUM_SLOT_LINE );

            if( iIndex >= m_iItemCount || m_vecItemBox[ iIndex ].first == 0 || m_vecItemBox[ iIndex ].first == 0 ) { 
                m_vecItemBox[ iIndex ].second->ToggleRender( false );
                m_vecItemImg[ iIndex ].second->ToggleRender( false );
            }
            else { 
                KItem* kItem = g_pItemMgr->GetInventoryItem( m_vecItemImg[iIndex].first );
                if ( !kItem )
                    continue;

                float fBackX = static_cast<float>( START_POS_X + ( SLOT_OFFEST * j ) + ( IMG_WIDTH * j ) );
                float fBackY = static_cast<float>( START_POS_Y + ( ROUND_OFFSET * i ) + ( BOX_HEIGHT * i) );

                float fImgX = fBackX + ( ( START_POS_X - START_POS_Y ) * ( j + 1 ) );

                D3DXVECTOR2 dxvBoxPos( fBackX, fBackY );
                D3DXVECTOR2 dxvImgPos( fImgX, fBackY );

                m_vecItemBox[ iIndex ].second->InitState( true, true, this );
                m_vecItemBox[ iIndex ].second->SetWindowPos( dxvBoxPos );

                m_vecItemImg[ iIndex ].second->InitState( true, true, this );
                m_vecItemImg[ iIndex ].second->SetWindowPos( dxvImgPos );
                m_vecItemImg[ iIndex ].second->SetItemInfo( m_pItemData->dwGoodsID,-1,false,-1, kItem->m_ItemUID, -1, kItem->m_DesignCoordiID );
                m_vecItemImg[ iIndex ].second->ShowItem( true );

                m_vecItemImg[ iIndex ].second->DisconnectAllEventProcedure();
                m_vecItemImg[ iIndex ].second->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCHeroItemUpgradeMaterialSelect::OnClickItem );
            }
        }
    }
}

void KGCHeroItemUpgradeMaterialSelect::ShowItemInfo( void )
{
    bool bCheck = false;
    std::vector< std::pair< GCITEMUID, KGCItemImgWnd* > >::const_iterator it = m_vecItemImg.begin();
    for( ; it != m_vecItemImg.end() ; ++it ) { 
        if( it->second->CheckMousePosInWindowBound() ) { 
            KItem* pInvenItem = NULL;
            if( false == g_pItemMgr->FindInventory( it->first, &pInvenItem ) ) { 
                continue;
            }

            g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( m_pItemData, -1, pInvenItem );

            D3DXVECTOR2 dxvPos = it->second->GetCurrentWindowPos();

            //float fX, fY;
            //KLuaManager luaMgr;
            //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
            //LUA_GET_VALUE_DEF( "X", fX, 0.0f );
            //LUA_GET_VALUE_DEF( "Y", fY, 0.0f );

            dxvPos.x *= m_fWindowScaleX;
            dxvPos.y *= m_fWindowScaleY;
            dxvPos.y += 30;

            g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( dxvPos );
            g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);

            bCheck = true;
            break;
        }
    }

    g_pkUIScene->m_pkItemInformationBox->ToggleRender( bCheck );
}
