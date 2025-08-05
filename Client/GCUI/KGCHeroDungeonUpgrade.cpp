#include "StdAfx.h"
#include "KGCHeroDungeonUpgrade.h"
#include "GCHeroItemUpgradeMaterialSelect.h"

IMPLEMENT_CLASSNAME( KGCHeroDungeonUpgrade );
IMPLEMENT_WND_FACTORY( KGCHeroDungeonUpgrade );
IMPLEMENT_WND_FACTORY_NAME( KGCHeroDungeonUpgrade, "gc_hero_dungeon_upgrade" );

KGCHeroDungeonUpgrade::KGCHeroDungeonUpgrade(void)
{
    for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
    {
        m_apkCharacterName[i] = NULL;   

        char szBuf[MAX_PATH];
        sprintf_s(szBuf, MAX_PATH - 1, "character_name%d", i);
        LINK_CONTROL( szBuf, m_apkCharacterName[i] );
    }

    m_pkHeroCoin = NULL;
    m_pkHeroCoinPiece = NULL;
    LINK_CONTROL( "hero_coin", m_pkHeroCoin );
    LINK_CONTROL( "hero_coin_piece", m_pkHeroCoinPiece );

    m_pkBtnPrevPage = NULL;
    m_pkBtnNextPage = NULL;
    m_pkStaticPage = NULL;
    LINK_CONTROL( "btn_prev_page", m_pkBtnPrevPage );
    LINK_CONTROL( "btn_next_page", m_pkBtnNextPage );
    LINK_CONTROL( "static_page", m_pkStaticPage );

    m_pkStaticHeroCoin = NULL;
    m_pkStaticHeroCoinPiece = NULL;
    LINK_CONTROL( "static_hero_coin", m_pkStaticHeroCoin );
    LINK_CONTROL( "static_hero_coin_piece", m_pkStaticHeroCoinPiece );

    for( int i=0 ; i<NUM_UPGRADE_BOX ; ++i ) { 
        m_apkUpgradeBox[i] = NULL;

        char szTemp[MAX_PATH];
        sprintf_s( szTemp, MAX_PATH - 1, "hero_item_upgrade_box%d", i );
        LINK_CONTROL( szTemp, m_apkUpgradeBox[i] );
    }

    m_pkMaterialSelectBox = NULL;
    LINK_CONTROL( "material_select_list", m_pkMaterialSelectBox );

    m_eCurrentCharacter = GC_CHAR_INVALID;
    m_iCurrentPage = -1;
    m_iTotalPage = -1;
}

KGCHeroDungeonUpgrade::~KGCHeroDungeonUpgrade(void)
{

}

void KGCHeroDungeonUpgrade::OnCreate( void )
{
    for( int i = 0 ; i < GC_CHAR_NUM ; ++i )
    {
        if (m_apkCharacterName[i]) {
            m_apkCharacterName[i]->InitState(false, false, this);

        }
    }

    for( int i=0 ; i<NUM_UPGRADE_BOX ; ++i ) { 
        m_apkUpgradeBox[i]->InitState( false, true, this );
        m_apkUpgradeBox[i]->SetBoxNumber( i );
    }

    m_pkHeroCoin->InitState(true, false, this);
    m_pkHeroCoinPiece->InitState(true, false, this);

    m_pkBtnPrevPage->InitState(true, true, this);
    m_pkBtnPrevPage->DisconnectAllEventProcedure();
    m_pkBtnPrevPage->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCHeroDungeonUpgrade::OnPrevPage );

    m_pkBtnNextPage->InitState(true, true, this);
    m_pkBtnNextPage->DisconnectAllEventProcedure();
    m_pkBtnNextPage->ConnectEventProcedure( KD3DWnd::D3DWE_BUTTON_CLICK, this, &KGCHeroDungeonUpgrade::OnNextPage );

    m_pkStaticPage->SetFontColor(0xffffffff);
    m_pkStaticPage->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticPage->SetAlign(DT_CENTER);
    m_pkStaticPage->InitState(true, false, this);    

    m_pkStaticHeroCoin->SetFontColor(0xffffffff);
    m_pkStaticHeroCoin->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticHeroCoin->SetAlign(DT_LEFT);
    m_pkStaticHeroCoin->InitState(true, false, this);  

    m_pkStaticHeroCoinPiece->SetFontColor(0xffffffff);
    m_pkStaticHeroCoinPiece->SetFontSize(SiKGCMultipleLanguages()->GetDefualtFontSize());
    m_pkStaticHeroCoinPiece->SetAlign(DT_LEFT);
    m_pkStaticHeroCoinPiece->InitState(true, false, this);  

    m_pkMaterialSelectBox->InitState( false, true, this );

    // 캐릭터 선택 키 관련
    g_pkUIScene->m_pkToolTip->SetWindowZOrder(D3DWZ_TOPMOST);
}

void KGCHeroDungeonUpgrade::OnCreateComplete( void )
{
    SetItemUpgradeInfo();   // 업그레이드 정보 세팅 
    UpdateCharacterInfo();  // 케릭터 정보 세팅 
    SetMyHeroCoin();        // 주화 정보 세팅 
}

void KGCHeroDungeonUpgrade::OnDestroy( void )
{
    m_eCurrentCharacter = GC_CHAR_INVALID;

    g_pkUIScene->m_pkToolTip->SetWindowZOrder(D3DWZ_NOTOPMOST);
    g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder(D3DWZ_NOTOPMOST);

    m_iCurrentPage = -1;
    m_iTotalPage = -1;

    CloseMaterialBox();
}

void KGCHeroDungeonUpgrade::ActionPerformed( const KActionEvent& event )
{
}

void KGCHeroDungeonUpgrade::OnDestroyComplete( void )
{
}

void KGCHeroDungeonUpgrade::FrameMoveInEnabledState( void )
{
}

void KGCHeroDungeonUpgrade::PostChildDraw( void )
{
}

void KGCHeroDungeonUpgrade::UpdateCharacterInfo( void )
{
    if ( m_eCurrentCharacter >= 0 && m_eCurrentCharacter < GC_CHAR_NUM) 
    {
        m_apkCharacterName[m_eCurrentCharacter]->ToggleRender(false);
    }

    // 캐릭터 세팅
    m_eCurrentCharacter = static_cast<EGCCharType>( g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType );
    m_apkCharacterName[m_eCurrentCharacter]->ToggleRender(true);

    // 페이지 세팅 
    int iItemCount = GetItemCount( m_eCurrentCharacter );
    ResetPage( iItemCount );

    CloseMaterialBox();
    SetUpgradeBoxInfo();
}

void KGCHeroDungeonUpgrade::OnPrevPage( void )
{
    --m_iCurrentPage;
    if( m_iCurrentPage < 1 ) {
        m_iCurrentPage = 1;
    }

    CloseMaterialBox();
    SetUpgradeBoxInfo();
}

void KGCHeroDungeonUpgrade::OnNextPage( void )
{
    ++m_iCurrentPage;
    if( m_iCurrentPage > m_iTotalPage )  { 
        m_iCurrentPage = m_iTotalPage;
    }

    CloseMaterialBox();
    SetUpgradeBoxInfo();
}

void KGCHeroDungeonUpgrade::ResetPage( int iItemCount_ )
{
    m_iTotalPage = iItemCount_ / NUM_UPGRADE_BOX;

    if( 0 < m_iTotalPage && 0 != iItemCount_ % NUM_UPGRADE_BOX ) {
        ++m_iTotalPage;
    }
    else if( 0 == m_iTotalPage && 0 != iItemCount_ % NUM_UPGRADE_BOX ) {
        m_iTotalPage = 1;
    }

    m_iCurrentPage = 1;
}

void KGCHeroDungeonUpgrade::SetMyHeroCoin( void )
{
    m_pkStaticHeroCoin->SetText( g_pItemMgr->GetInventoryItemDuration(ITEM_ID_HERO_COIN) );
    m_pkStaticHeroCoinPiece->SetText( g_pItemMgr->GetInventoryItemDuration(ITEM_ID_HERO_COIN_PIECE) );
}

void KGCHeroDungeonUpgrade::SetItemUpgradeInfo( void )
{
    if( !m_mapUpgrageInfo.empty() ) { 
        return;
    }

    SendItemUpgradeInfoReq();

    if( g_kGlobalValue.m_kHeroDungeonInfo.m_mapUpgradeInfo.empty() ) { 
        return;
    }

    m_mapUpgrageInfo.clear();
    std::map< std::pair< DWORD, int >, KHeroItemUpgradeInfo >::const_iterator itInfo = g_kGlobalValue.m_kHeroDungeonInfo.m_mapUpgradeInfo.begin();

    for( ; itInfo != g_kGlobalValue.m_kHeroDungeonInfo.m_mapUpgradeInfo.end() ; ++itInfo ) { 
        GCItem* pItemData = g_pItemMgr->GetItemData( itInfo->first.first / 10 );
        if( pItemData == NULL ) { 
            continue;
        }

        EGCCharType eCharType = static_cast<EGCCharType>( pItemData->GetCharType() );

        std::map< EGCCharType, std::vector< KHeroItemUpgradeInfo > >::iterator mit = m_mapUpgrageInfo.find( eCharType );
        if( mit == m_mapUpgrageInfo.end() ) { 
            std::vector< KHeroItemUpgradeInfo > vecInfo;
            vecInfo.push_back( itInfo->second );
            m_mapUpgrageInfo.insert( std::make_pair( eCharType, vecInfo ) );
        }
        else { 
            mit->second.push_back( itInfo->second );
        }
    }
}

void KGCHeroDungeonUpgrade::SendItemUpgradeInfoReq( void )
{
    KP2P::GetInstance()->Send_HeroItemUpgradeInfoReq();

    Result_HeroItemList = INT_MAX;
    g_MyD3D->WaitForServerAck(Result_HeroItemList, INT_MAX, 3000, INT_MAX);
}

int KGCHeroDungeonUpgrade::GetItemCount( EGCCharType eCharType_ )
{
    std::map< EGCCharType, std::vector< KHeroItemUpgradeInfo > >::iterator mit = m_mapUpgrageInfo.find( eCharType_ );

    int iItemNum = 0;

    if ( mit != m_mapUpgrageInfo.end() ) {
        iItemNum += static_cast<int>( mit->second.size() );
    }

    return iItemNum;
}

void KGCHeroDungeonUpgrade::SetUpgradeBoxInfo( void )
{
    std::map< EGCCharType, std::vector< KHeroItemUpgradeInfo > >::iterator mit = m_mapUpgrageInfo.find( m_eCurrentCharacter );
    if( mit == m_mapUpgrageInfo.end() ) { 
        return;
    }

    std::wostringstream strm;
    strm << m_iCurrentPage << L" / " << m_iTotalPage ;
    m_pkStaticPage->SetText( strm.str() );

    int iStartIndex = m_iCurrentPage;
    --iStartIndex;
    iStartIndex *= NUM_UPGRADE_BOX;

    int iItemCount = static_cast<int>( mit->second.size() );

    for( int i = 0 ; i < NUM_UPGRADE_BOX ; ++i )
    {
        m_apkUpgradeBox[i]->ClearInfo();

        if( iStartIndex + i < iItemCount  ) 
        {
            if( static_cast<int>( mit->second.size() ) <= iStartIndex ) { 
                break;
            }

            m_apkUpgradeBox[i]->SetItemInfo( mit->second[ iStartIndex + i ] );
            m_apkUpgradeBox[i]->ToggleRender( true );
        }
        else { 
            m_apkUpgradeBox[i]->ToggleRender( false );
        }
    }
}

void KGCHeroDungeonUpgrade::SetSelectMaterial( int iBoxNumber_, int iSlotNumber_, GCITEMUID itemUID_ )
{
    if( iBoxNumber_ < 0 || iBoxNumber_ > NUM_UPGRADE_BOX -1 ||
        iSlotNumber_ < 0 || iSlotNumber_ > KGCHeroItemUpgradeBox::MAX_MATERIAL - 1 ) { 
            return;
    }

    m_apkUpgradeBox[ iBoxNumber_ ]->SetSelectMaterial( iSlotNumber_, itemUID_ );
}

void KGCHeroDungeonUpgrade::ShowMateriaBox( D3DXVECTOR2 dxvPos_, int iBoxNumber_, int iSlotNumber_, GCITEMID itemID_, int iGrade_ )
{
    m_pkMaterialSelectBox->SetPos( dxvPos_ );
    m_pkMaterialSelectBox->SetInfo( iBoxNumber_, iSlotNumber_, itemID_, iGrade_ );
}

bool KGCHeroDungeonUpgrade::IsRenderOnMaterial( void )
{
    return m_pkMaterialSelectBox->IsRenderOn();
}

void KGCHeroDungeonUpgrade::CloseMaterialBox( void )
{
    m_pkMaterialSelectBox->OnClose();
}

void KGCHeroDungeonUpgrade::UptateUpgradeMaterial( void )
{
    for( int i=0 ; i<NUM_UPGRADE_BOX ; ++i ) { 
        m_apkUpgradeBox[i]->UpdateMaterialInfo();
    }
}

bool KGCHeroDungeonUpgrade::GetUpgradeMaterialUID( IN const std::pair< GCITEMID, int > prKey_, OUT std::vector< GCITEMUID >& vecUIDList_ )
{
    int iIndex = -1;
    for( int i=0 ; i<NUM_UPGRADE_BOX ; ++i ) { 
        std::pair< GCITEMID, int > prBox = m_apkUpgradeBox[i]->GetUpgradeItemKey();

        if( prBox.first == prKey_.first && prBox.second == prKey_.second ) { 
            iIndex = i;
            break;
        }
    }

    if( iIndex == -1 ) { 
        return false;
    }

    return m_apkUpgradeBox[ iIndex ]->GetUpgradeMaterialUID( vecUIDList_ );
}