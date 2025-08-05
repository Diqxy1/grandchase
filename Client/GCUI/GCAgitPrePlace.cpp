#include "stdafx.h"
#include "GCAgitPrePlace.h"
#include "Controls.h"

IMPLEMENT_CLASSNAME( KGCAgitPrePlace );
IMPLEMENT_WND_FACTORY( KGCAgitPrePlace );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitPrePlace, "gc_agit_pre_place" );

KGCAgitPrePlace::KGCAgitPrePlace( void )
{
    m_pkBtnBuy = NULL;
    m_pkBtnCancle = NULL;
    m_pkStaticItemName = NULL;
    m_pkStaticPrice = NULL;
    m_pkItemImg = NULL;
    m_pkItemFrame = NULL;

    LINK_CONTROL( "btnBuy", m_pkBtnBuy );
    LINK_CONTROL( "btnCancle", m_pkBtnCancle );
    LINK_CONTROL( "staticItemName", m_pkStaticItemName );
    LINK_CONTROL( "staticPrice", m_pkStaticPrice );
    LINK_CONTROL( "mainframe", m_pkItemFrame );
    LINK_CONTROL( "item_image", m_pkItemImg );

    m_pkMoneyTex = NULL;

    m_pItemData = NULL;
    m_iMoneyType = 0;
    m_iPrice = 0;
}

KGCAgitPrePlace::~KGCAgitPrePlace( void )
{
    SAFE_RELEASE( m_pkMoneyTex );
}

void KGCAgitPrePlace::OnCreate( void )
{
    m_pkBtnBuy->InitState( true, true, this );
    m_pkBtnCancle->InitState( true, true, this );
    m_pkItemImg->InitState( true );
    m_pkItemFrame->InitState( true, true, this );
    
    m_pkStaticItemName->InitState( true, false, NULL );
    m_pkStaticPrice->InitState( true, false, NULL );
    m_pkStaticItemName->SetAlign( DT_LEFT );
    m_pkStaticPrice->SetAlign( DT_LEFT );

    m_pkBtnCancle->SetHotKey( DIK_ESCAPE );
}

void KGCAgitPrePlace::SetPrePlaceInfo( GCItem* pItem_, int iMoneyType_, int iPrice_, PAIR_USHORT_DWORD pairIndex_ )
{
    if( NULL == pItem_ )
        return;

    m_pItemData = pItem_;
    m_PairIndex = pairIndex_;
    m_iMoneyType = iMoneyType_;
    m_iPrice = iPrice_;

    //m_pkStaticItemName->SetText( m_pItemData->strItemName );
    m_pkStaticItemName->SetForceWordWrapText( m_pItemData->strItemName, true );
    m_pkStaticPrice->SetNumber( m_iPrice );

    m_pkItemImg->SetItemInfo( pItem_->dwGoodsID );
    m_pkItemImg->ToggleRender( true );
    m_pkItemFrame->ToggleRender( true );

    this->ToggleRender( true );
}

void KGCAgitPrePlace::Clear( void )
{
    m_pItemData = NULL;
    m_PairIndex.first = 0;
    m_PairIndex.second = 0;
    m_iMoneyType = 0;
    m_iPrice = 0;

    SAFE_RELEASE( m_pkMoneyTex );
}

void KGCAgitPrePlace::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnBuy,      KD3DWnd::D3DWE_BUTTON_CLICK, OnBuyBtn );
    GCWND_MSG_MAP( m_pkBtnCancle,   KD3DWnd::D3DWE_BUTTON_CLICK, OnCancleBtn );

    GCWND_MSG_MAP( m_pkItemFrame,   KD3DWnd::D3DWE_BUTTON_DOWN, StartDrag );
}

void KGCAgitPrePlace::PostChildDraw( void )
{
    // 화폐 아이콘 표시 
}

void KGCAgitPrePlace::OnBuyBtn( void )
{
    // 선택 아이템 구입
    if( NULL == m_pItemData )
        return;

    if ( g_pkUIScene->m_pkAgitShop ) {
        g_pkUIScene->m_pkAgitShop->SetSavedCurPage();
    }

    SiKGCAgitGameManager()->BuyAgitShopObject( m_pItemData->dwGoodsID, m_PairIndex );
}

void KGCAgitPrePlace::OnCancleBtn( void )
{
    // 미리배치 UI 끄고 상점 다시 열어줌 
    Close();

    if ( g_pkUIScene->m_pkAgitShop ) {
        g_pkUIScene->m_pkAgitShop->SetSavedCurPage();
    }

    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_AGIT_SHOP, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);//, true );
}

void KGCAgitPrePlace::StartDrag()
{
    const POINT& ptMouse = g_pkInput->GetMousePos();
    D3DXVECTOR2 dxvMousePos( static_cast<float>(ptMouse.x), static_cast<float>(ptMouse.y) );
    g_MyD3D->MyCtrl->Picking( dxvMousePos );

    SiKGCAgitGameManager()->AddDummyObject( m_pItemData->dwGoodsID * 10 );
    SiKGCAgitGameManager()->PickUpObject( 0, dxvMousePos );
    m_pkItemFrame->ToggleRender( false );
    m_pkItemImg->ToggleRender( false );
}

void KGCAgitPrePlace::UpdateData()
{
    if( SiKGCAgitGameManager()->IsHideObject( 0 ) ) {
        m_pkItemFrame->ToggleRender( true );
        m_pkItemImg->ToggleRender( true );
    }
}

void KGCAgitPrePlace::Close()
{
    SiKGCAgitGameManager()->SetPrePlace( false );
    this->ToggleRender( false );
    SiKGCAgitGameManager()->RemoveDummyObject();
}