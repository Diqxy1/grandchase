#include "stdafx.h"
#include "GCItemCreationSelectBox.h"

IMPLEMENT_CLASSNAME( KGCItemCreationSelectBox );
IMPLEMENT_WND_FACTORY( KGCItemCreationSelectBox );
IMPLEMENT_WND_FACTORY_NAME( KGCItemCreationSelectBox, "gc_item_creation_select_box" );

KGCItemCreationSelectBox::KGCItemCreationSelectBox( void )
: m_dxvPos( 0.0f, 0.0f )
, m_pkBackGround( NULL )
, m_pkItemData( NULL )
, m_iItemCount( 0 )
, m_iSlotLineCount( 0 )
{
    LINK_CONTROL( "background", m_pkBackGround );

    m_vecInvenItem.clear();
    m_vecItemBack.clear();
    m_vecItemImg.clear();
}

KGCItemCreationSelectBox::~KGCItemCreationSelectBox( void )
{
}

void KGCItemCreationSelectBox::OnCreate( void )
{
    m_pkBackGround->InitState( true );

    std::vector< KD3DWnd* > vecBack = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_item_creation_select_dlg.stg", "", "item_creation_select_box\\itembox", true, NUM_MAX_SLOT );
    std::vector< KD3DWnd* > vecImg = g_pkUIMgr->CreateTemplateUIListByFindName(this, "ui_item_creation_select_dlg.stg", "", "item_creation_select_box\\itemImg", true, NUM_MAX_SLOT );

    std::vector< KD3DWnd* >::iterator itBack = vecBack.begin();
    std::vector< KD3DWnd* >::iterator itImg = vecImg.begin();


    for( ; itBack != vecBack.end() ; ++itBack, ++itImg ) { 
        m_vecItemBack.push_back( std::make_pair( 0, *itBack ) );
        m_vecItemImg.push_back( std::make_pair( 0, static_cast< KGCItemImgWnd* >( *itImg ) ) );
    }
}

bool KGCItemCreationSelectBox::InitDialog( IN const KDialogInfo& kInfo_ )
{
    m_vecInvenItem.clear();
    m_pkItemData = g_pItemMgr->GetItemData( static_cast< GCITEMID >( kInfo_.m_lParam ) );
    if( m_pkItemData == NULL ) { 
        return false;
    }

    if( false == g_pItemMgr->GetInventoryItemListFromItemID( m_pkItemData->dwGoodsID, m_vecInvenItem ) ) { 
        return false;
    }

    std::vector< KItem* >::iterator it = m_vecInvenItem.begin();
    for( ; it != m_vecInvenItem.end() ; ) { 

        if( m_pkItemData->eItemKind == GIK_COORDI_ITEM && (*it)->m_cItemType == KItem::TYPE_NORMAL ) { 
            m_vecInvenItem.erase( it );
            continue;
        }

        if( m_pkItemData->eItemKind != GIK_COORDI_ITEM && (*it)->m_cItemType == KItem::TYPE_LOOK ) { 
            m_vecInvenItem.erase( it );
            continue;
        }

        if( g_pItemMgr->CheckEquipItemByItemUID( &g_kGlobalValue.m_kUserInfo.GetCurrentChar(), (*it)->m_ItemUID ) ) { 
            m_vecInvenItem.erase( it );
            continue;
        }

        ++it;
    }

    this->SetWindowPos( m_dxvPos );
    InitUI();

    return true;
}

void KGCItemCreationSelectBox::ActionPerformed( const KActionEvent& event )
{

}

void KGCItemCreationSelectBox::FrameMoveInEnabledState( void )
{
    ShowItemInfo();
}

void KGCItemCreationSelectBox::OnClose( void )
{
    ClearData();
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}

void KGCItemCreationSelectBox::OnClickItem( const KActionEvent& event )
{
    std::vector< std::pair< GCITEMUID, KGCItemImgWnd* > >::const_iterator it = m_vecItemImg.begin();
    for( ; it != m_vecItemImg.end() ; ++it ) { 
        if( it->second == event.m_pWnd ) { 
            g_pkUIScene->m_pkForgeDlg->SetSelectedMaterial( it->first );
            break;
        }
    }
    
    ClearData();
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
}
void KGCItemCreationSelectBox::MakeItemSlotList( void )
{
    if( m_vecInvenItem.empty() ) { 
        return ;
    }

    m_iItemCount = static_cast<int>( m_vecInvenItem.size() );
    m_iSlotLineCount = m_iItemCount / NUM_SLOT_PER_LINE;
    if( m_iItemCount % NUM_SLOT_PER_LINE ) { 
        ++m_iSlotLineCount;
    }

    int iNumItemSlot = static_cast<int>( m_vecInvenItem.size() );

    std::vector< std::pair< GCITEMUID, KD3DWnd* > >::iterator itBack = m_vecItemBack.begin();
    std::vector< std::pair< GCITEMUID, KGCItemImgWnd* > >::iterator itImg = m_vecItemImg.begin();
    for( int i = 0 ; i< m_iItemCount ; ++i, ++itBack, ++itImg ) { 
    
        if( i > NUM_MAX_SLOT ) { 
            break;
        }

        itBack->first = m_vecInvenItem[i]->m_ItemUID;
        itImg->first = m_vecInvenItem[i]->m_ItemUID;
    }
}

void KGCItemCreationSelectBox::SetBackGroundHeight( void )
{
    if( m_iSlotLineCount <= 0 ) { 
        return;
    }

    if( m_iSlotLineCount == 1 ) { 
        m_pkBackGround->SetHeight( BACKGROUND_START_HEIGHT );
    }
    else { 
        DWORD dwHeight = BACKGROUND_START_HEIGHT + ( SLOT_ROUND_OFFSET * ( m_iSlotLineCount - 1 ) ) + ( IMG_BACK_HEIGHT * ( m_iSlotLineCount - 1 ) );
        m_pkBackGround->SetHeight( dwHeight );
    }
}

void KGCItemCreationSelectBox::SetItemSlotPos( void )
{
    for( int i=0 ; i<m_iSlotLineCount ; ++i ) { 
        for( int j=0 ; j<NUM_SLOT_PER_LINE ; ++j ) { 

            int iIndex = j + ( i * NUM_SLOT_PER_LINE );

            if( iIndex >= m_iItemCount || m_vecItemBack[ iIndex ].first == 0 || m_vecItemImg[ iIndex ].first == 0 ) { 
                m_vecItemBack[ iIndex ].second->ToggleRender( false );
                m_vecItemImg[ iIndex ].second->ToggleRender( false );
            }
            else { 

                KItem* kItem = g_pItemMgr->GetInventoryItem( m_vecItemImg[iIndex].first );
                if ( !kItem )
                    continue;

                float fBackX = static_cast<float>( BACK_START_X + ( SLOT_OFFSET * j ) + ( IMG_BACK_WIDTH * j ) );
                float fBackY = static_cast<float>( BACK_START_Y + ( SLOT_ROUND_OFFSET * i ) + ( IMG_BACK_HEIGHT * i) );

                float fImgX = fBackX + ( ( IMG_START_X - BACK_START_X ) * ( j + 1 ) );

                D3DXVECTOR2 dxvBackPos( fBackX, fBackY );
                D3DXVECTOR2 dxvImgPos( fImgX, fBackY );

                m_vecItemBack[ iIndex ].second->InitState( true, true, this );
                m_vecItemBack[ iIndex ].second->SetWindowPos( dxvBackPos );

                m_vecItemImg[ iIndex ].second->InitState( true, true, this );
                m_vecItemImg[ iIndex ].second->SetWindowPos( dxvImgPos );
                m_vecItemImg[ iIndex ].second->SetItemInfo( m_pkItemData->dwGoodsID,-1,false,-1, kItem->m_ItemUID, kItem->m_nCount, kItem->m_DesignCoordiID );
                m_vecItemImg[ iIndex ].second->ShowItem( true );
                
                m_vecItemImg[ iIndex ].second->DisconnectAllEventProcedure();
                m_vecItemImg[ iIndex ].second->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCItemCreationSelectBox::OnClickItem );
            }
        }
    }
}

void KGCItemCreationSelectBox::InitUI( void )
{
    MakeItemSlotList();
    SetBackGroundHeight();
    SetItemSlotPos();
}

void KGCItemCreationSelectBox::ShowItemInfo( void )
{
    bool bCheck = false;
    std::vector< std::pair< GCITEMUID, KGCItemImgWnd* > >::const_iterator it = m_vecItemImg.begin();
    for( ; it != m_vecItemImg.end() ; ++it ) { 
        if( it->second->CheckMousePosInWindowBound() ) { 
            KItem* pInvenItem = NULL;
            if( false == g_pItemMgr->FindInventory( it->first, &pInvenItem ) ) { 
                continue;
            }

            g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( m_pkItemData, -1, pInvenItem );

            D3DXVECTOR2 dxvPos = it->second->GetCurrentWindowPos();

            //float fX, fY;
            //KLuaManager luaMgr;
            //GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );
            //LUA_GET_VALUE_DEF( "X", fX, 0.0f );
            //LUA_GET_VALUE_DEF( "Y", fY, 0.0f );

            dxvPos.x *= m_fWindowScaleX;
            dxvPos.y *= m_fWindowScaleY;
            dxvPos.y += 50.0f;

            g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( dxvPos );
            g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);
            
            bCheck = true;
            break;
        }
    }

    g_pkUIScene->m_pkItemInformationBox->ToggleRender( bCheck );
}

void KGCItemCreationSelectBox::ClearData( void )
{
    m_dxvPos = D3DXVECTOR2( 0.0f, 0.0f );
    m_pkItemData = NULL;
    m_vecInvenItem.clear();
    
    m_iItemCount = 0;
    m_iSlotLineCount = 0;

    std::vector< std::pair< GCITEMUID, KD3DWnd* > >::iterator itBack = m_vecItemBack.begin();
    std::vector< std::pair< GCITEMUID, KGCItemImgWnd* > >::iterator itImg = m_vecItemImg.begin();

    for( ; itBack != m_vecItemBack.end() ; ++itBack ) { 
        itBack->first = 0;
        itBack->second->ToggleRender( false );
    }

    for( ; itImg != m_vecItemImg.end() ; ++itImg ) { 
        itImg->first = 0;
        itImg->second->ToggleRender( false );
    }
}
