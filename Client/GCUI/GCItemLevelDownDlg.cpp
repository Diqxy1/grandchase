#include "stdafx.h"
#include "GCItemLevelDownDlg.h"

IMPLEMENT_CLASSNAME( KGCItemLevelDownDlg );
IMPLEMENT_WND_FACTORY( KGCItemLevelDownDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCItemLevelDownDlg, "gc_itemleveldowndlg" );

const GCITEMID  dwHeroCoinID = 36208;

KGCItemLevelDownDlg::KGCItemLevelDownDlg( void )
{
    m_itemUID = 0;
    m_pkStaticDesc = NULL;
    m_pkStaticItemName = NULL;
    m_pkStaticEquipLevel = NULL;
    m_pkStaticHeroCoin = NULL;
    m_pkStaticRemainLevelDown = NULL;
    m_pkStaticHeroCoinNum = NULL;
    m_pkStaticRemainLevelDownNum = NULL;
    m_pkEquipmentItemWnd = NULL;

    m_pkBtnOK = NULL;
    m_pkBtnCancle = NULL;

    LINK_CONTROL( "static_desc", m_pkStaticDesc );
    LINK_CONTROL( "static_item_name", m_pkStaticItemName );
    LINK_CONTROL( "static_equip_level", m_pkStaticEquipLevel );
    LINK_CONTROL( "static_hero_coin", m_pkStaticHeroCoin );
    LINK_CONTROL( "static_remain_level_down", m_pkStaticRemainLevelDown );
    LINK_CONTROL( "static_hero_coin_num", m_pkStaticHeroCoinNum );
    LINK_CONTROL( "static_remain_level_down_num", m_pkStaticRemainLevelDownNum );

    LINK_CONTROL( "btn_ok", m_pkBtnOK );
    LINK_CONTROL( "btn_cancle", m_pkBtnCancle );

    LINK_CONTROL( "item_img", m_pkEquipmentItemWnd );
}

KGCItemLevelDownDlg::~KGCItemLevelDownDlg( void )
{
}

void KGCItemLevelDownDlg::OnCreate( void )
{
    m_pkStaticDesc->SetFontColor(0xffE9EEF3);
    m_pkStaticDesc->SetAlign(DT_LEFT);
    m_pkStaticDesc->InitState(true, false, this);  
    m_pkStaticDesc->SetMultiLine( true );
    m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_LEVEL_DOWN_DLG_DESC ) );

    m_pkStaticItemName->SetFontColor(0xffE9EEF3);
    m_pkStaticItemName->SetText(L"");
    
    m_pkStaticItemName->SetFontColor(0xffE9EEF3);
    m_pkStaticEquipLevel->SetText(L"");
    
    m_pkStaticHeroCoin->SetFontColor(0xffE9EEF3);
    m_pkStaticHeroCoin->SetFontOutline(true,0xff14212f);
    m_pkStaticHeroCoin->SetFontSpace( 1.0f );
    m_pkStaticHeroCoin->SetAlign(DT_CENTER);
    m_pkStaticHeroCoin->SetText( g_pkStrLoader->GetString( STR_ID_LEVEL_DOWN_DLG_HERO_COIN ));

    m_pkStaticRemainLevelDown->SetFontColor(0xffE9EEF3);
    m_pkStaticRemainLevelDown->SetFontOutline(true,0xff14212f);
    m_pkStaticRemainLevelDown->SetFontSpace( 1.0f );
    m_pkStaticRemainLevelDown->SetAlign(DT_CENTER);
    m_pkStaticRemainLevelDown->SetText( g_pkStrLoader->GetString( STR_ID_LEVEL_DOWN_DLG_REMAIN_DESC ));

    m_pkStaticHeroCoinNum->SetFontColor(0xffE9EEF3);
    m_pkStaticHeroCoinNum->SetAlign(DT_CENTER);
    m_pkStaticHeroCoinNum->SetText( L"" );
    
    m_pkStaticRemainLevelDownNum->SetFontColor(0xffE9EEF3);
    m_pkStaticRemainLevelDownNum->SetAlign(DT_CENTER);
    m_pkStaticRemainLevelDownNum->SetText( L"" );

    m_pkEquipmentItemWnd->InitState( true );

    m_pkBtnOK->InitState( true, true, this );
    m_pkBtnCancle->InitState( true, true, this );
    m_pkBtnCancle->SetHotKey( DIK_ESCAPE );

}

void KGCItemLevelDownDlg::FrameMoveInEnabledState( void )
{
    const D3DXVECTOR2 vMousePos = D3DXVECTOR2( (float)g_pkInput->GetMousePos().x, (float)g_pkInput->GetMousePos().y );

    bool bCheck = false;
    if( m_pkEquipmentItemWnd->CheckPosInWindowBound( vMousePos ) )
    {
        // 아이템 정보 표시창 위치 보정.
        D3DXVECTOR2 vecPos = m_pkEquipmentItemWnd->GetCurrentWindowPos();
        RenderItemInfoBox( vecPos, m_kItemInfo );
        bCheck = true;
    }

    if( !bCheck )
    {
        g_pkUIScene->m_pkItemInformationBox->ToggleRender( false );
    } 
}

void KGCItemLevelDownDlg::OnCreateComplete( void )
{
    UpdateItemInfo();
}

void KGCItemLevelDownDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnOK, KD3DWnd::D3DWE_BUTTON_CLICK, OnOK );
    GCWND_MSG_MAP( m_pkBtnCancle, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );    
}

void KGCItemLevelDownDlg::OnOK()
{
    if ( m_itemUID == 0 ) 
    {
        return;
    }

    //  레벨 다운 할 수 없는 아이템
    if ( !g_pItemMgr->IsLevelDownItem( &m_kItemInfo ) ) 
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_LEVEL_DOWN_ERROR4 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    }

    //  더이상 못 내리는 아이템
    if ( g_pItemMgr->GetLevelDownTableID( m_kItemInfo.m_ItemID ) <= m_kItemInfo.m_sEquipLvDown ) 
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_LEVEL_DOWN_ERROR5 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    }


    int nDuration = g_pItemMgr->GetHeroCoin();
    
    KItem * kItem = NULL;
    g_pItemMgr->FindInventoryForItemID(ITEM_ID_HERO_COIN, &kItem);

    GCItem* pItem = g_pItemMgr->GetItemData( m_kItemInfo.m_ItemID );
    if ( !pItem ) 
        return;

    int nNeedLevel = g_pItemMgr->GetlItemEquipLevel( pItem, kItem, NULL, true );

    // 1레벨 밑으로는 못 내린다.
    if ( nNeedLevel <= 1 ) 
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_LEVEL_DOWN_ERROR5 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    }

    if ( !kItem || nDuration < g_pItemMgr->GetLevelDownReqCoin( pItem, &m_kItemInfo ) )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_LEVEL_DOWN_ERROR3 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    }

    KP2P::GetInstance()->Send_EquipLevelDownReq( m_itemUID, kItem->m_ItemUID );

    OnClose();
}

void KGCItemLevelDownDlg::OnClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCItemLevelDownDlg::UpdateItemInfo()
{
    KItem* pInvenItem = NULL;

    g_pItemMgr->FindInventory( m_itemUID, &pInvenItem);

    if ( !pInvenItem ) 
        return;

    m_kItemInfo = *pInvenItem;

    m_pkEquipmentItemWnd->SetItemInfo(m_kItemInfo.m_ItemID);
    m_pkEquipmentItemWnd->ShowItem(true);

    GCItem* pItem = g_pItemMgr->GetItemData( m_kItemInfo.m_ItemID );
    if ( !pItem ) 
        return;

    m_pkStaticItemName->SetText( pItem->strItemName );

    int nNeedLevel = g_pItemMgr->GetlItemEquipLevel(pItem, pInvenItem);

    std::wstring strLevelColorKey = L"#cE81919";
    std::wstring strColorKeyEnd = L"#cX";

    std::wstringstream stm;
    stm << g_pkStrLoader->GetString( STR_ID_INFO_NEED_LEVEL ) << nNeedLevel;
    
    if ( m_kItemInfo.m_sEquipLvDown > 0 ) 
    {
        stm << strLevelColorKey << g_pkStrLoader->GetReplacedString( STR_ID_LEVEL_DOWN_NUMBER, "i", m_kItemInfo.m_sEquipLvDown )
            << strColorKeyEnd;
    }

    m_pkStaticEquipLevel->SetText( stm.str() );


    int nRemainNum = g_pItemMgr->GetLevelDownTableID( m_kItemInfo.m_ItemID );
    nRemainNum -= m_kItemInfo.m_sEquipLvDown;
    m_pkStaticRemainLevelDownNum->SetText( g_pkStrLoader->GetReplacedString( STR_ID_LEVEL_DOWN_DLG_REMAIN, "i", nRemainNum ) );

    int nReqDuration = g_pItemMgr->GetLevelDownReqCoin( pItem, pInvenItem );
    int nHaveDuration = g_pItemMgr->GetHeroCoin();
    std::wstring strTemp = g_pkStrLoader->GetReplacedString( STR_ID_LEVEL_DOWN_DLG_HERO_COIN_NUM, "ii", nReqDuration, nHaveDuration );

    m_pkStaticHeroCoinNum->SetText( strTemp );

}

bool KGCItemLevelDownDlg::CheckCondition(  IN const KDialogInfo& kInfo_ )
{
    m_itemUID = static_cast<GCITEMUID>( kInfo_.m_lParam );
    return true;
}

void KGCItemLevelDownDlg::RenderItemInfoBox( D3DXVECTOR2& vPos_, KItem pInvenItem_ )
{
    vPos_.x *= m_fWindowScaleX;
    vPos_.y *= m_fWindowScaleY;
    vPos_.x -= g_pkUIScene->m_pkItemInformationBox->GetWidth() * g_pkUIScene->m_pkItemInformationBox->GetWindowScaleX();

    GCItem* pItem = g_pItemMgr->GetItemData(pInvenItem_.m_ItemID);

    g_pkUIScene->m_pkItemInformationBox->SetPointedItemInformation( pItem, -1, &pInvenItem_, NULL, NULL, -1 );
    g_pkUIScene->m_pkItemInformationBox->ToggleRender( true );
    g_pkUIScene->m_pkItemInformationBox->SetEquippedItemInformation();

    g_pkUIScene->m_pkItemInformationBox->SetOriginPosition( vPos_ );
    g_pkUIScene->m_pkItemInformationBox->SetWindowZOrder( KD3DWnd::D3DWZ_TOPMOST );
}

