#include "stdafx.h"
#include "GCBuyCashItemChooseCashTypeDlg.h"
#include "GCBuyCashItemDlgS6.h"


IMPLEMENT_CLASSNAME( KGCBuyCashItemChooseCashTypeDlg );
IMPLEMENT_WND_FACTORY( KGCBuyCashItemChooseCashTypeDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCBuyCashItemChooseCashTypeDlg, "gc_choose_cash_type" );

KGCBuyCashItemChooseCashTypeDlg::KGCBuyCashItemChooseCashTypeDlg( void )
{
    m_pkBtnClose = NULL;
    m_pkBtnUseCash = NULL;
    m_pkBtnUseGC = NULL;
    m_pkDot = NULL;
    LINK_CONTROL( "btn_close", m_pkBtnClose );
    LINK_CONTROL( "btn_cash", m_pkBtnUseCash );
    LINK_CONTROL( "btn_GC", m_pkBtnUseGC );
    LINK_CONTROL( "wnd_dot", m_pkDot );

    m_pkStaticItemName = NULL;
    m_pkStaticDesc1 = NULL;
    m_pkStaticDesc2 = NULL;
    m_pkStaticDesc3 = NULL;
    LINK_CONTROL( "static_item_name", m_pkStaticItemName );
    LINK_CONTROL( "static_desc1", m_pkStaticDesc1 );
    LINK_CONTROL( "static_desc2", m_pkStaticDesc2 );
    LINK_CONTROL( "static_desc3", m_pkStaticDesc3 );

    m_pkItemImgWnd = NULL;
    m_pkSetItemImgWnd = NULL;
    LINK_CONTROL( "item_img", m_pkItemImgWnd );
    LINK_CONTROL( "setitem_img", m_pkSetItemImgWnd );
}

KGCBuyCashItemChooseCashTypeDlg::~KGCBuyCashItemChooseCashTypeDlg( void )
{
}

void KGCBuyCashItemChooseCashTypeDlg::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey (DIK_ESCAPE );

    m_pkBtnUseCash->InitState( true, true, this );
    m_pkBtnUseGC->InitState( true, true, this );

    m_pkDot->InitState( false );

    m_pkStaticItemName->InitState( true );
    m_pkStaticItemName->SetAlign( DT_CENTER );
    m_pkStaticItemName->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize() );

    m_pkStaticDesc1->InitState( true );
    m_pkStaticDesc2->InitState( true );
    m_pkStaticDesc3->InitState( true );
    m_pkStaticDesc3->SetFontColor( D3DCOLOR_ARGB(255, 153, 153, 153) ); // 회색

    m_pkItemImgWnd->InitState( false );
    m_pkSetItemImgWnd->InitState( false );
}

void KGCBuyCashItemChooseCashTypeDlg::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnClose,           KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );
    _GCWND_MSG_MAP( m_pkBtnUseCash,         KD3DWnd::D3DWE_BUTTON_CLICK, OnClickUseCash );
    _GCWND_MSG_MAP(m_pkBtnUseGC, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickUseGC);
}

void KGCBuyCashItemChooseCashTypeDlg::OnCreateComplete()
{
    SetGoodsInfo();
}

void KGCBuyCashItemChooseCashTypeDlg::FrameMoveInEnabledState( void )
{
}

void KGCBuyCashItemChooseCashTypeDlg::OnClickClose()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
}

void KGCBuyCashItemChooseCashTypeDlg::OnClickUseCash()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
    g_pkUIScene->EnableBuyCashItemWnd( m_kItem.dwGoodsID, false ); // 선물하기 x
}

void KGCBuyCashItemChooseCashTypeDlg::OnClickUseGC()
{
    SpeakToActionListener(KActionEvent(this, KD3DWnd::EWNDMESSAGE_CLOSE));
    //선물하기 결제 타입 선택 창에서 VP 결제를 눌렀을 경우
    if (g_pItemMgr->m_bIsInPresentState)
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_PRESENT_CASH_ITEM_WND, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false);
    }
    //일반 구입 결제 타입 선택 창에서 VP 결제를 눌렀을 경우
    else
    {
        g_pkUIScene->MessageBox(KGCUIScene::GC_MBOX_BUY_CASH_ITEM_DLG_S6, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true);
    }
}

void KGCBuyCashItemChooseCashTypeDlg::SetGoodsInfo()
{
    // 현재 상품에 대한 정보를 얻자
    GCItem* pkItem = g_MyD3D->m_kItemMgr.GetItemData( g_pItemMgr->GetCurrentGoodsID() );

    if( pkItem == NULL )
        return;

    // 상품정보 저장
    m_kItem = *pkItem;

    // 아이템 이름
    m_pkStaticItemName->SetTextAutoMultiline( m_kItem.strItemName );

    // 캐쉬금액
    m_pkStaticDesc1->SetText( g_pkStrLoader->GetReplacedString( STR_ID_BUY_CASH_ITEM_S6_STR_1, "i", m_kItem.dwPrice ) );

    //// GC적립 : @1원(@2%)
    // 첫구매 적립율 > 이벤트 적립율 > 일반 적립율  우선 적용
    int nBonusGCRatio = 0;

    // 첫구매 유저이고 첫구매 보너스 GC 지급기간이라면
    if( g_kGlobalValue.m_nCashbackRatioFirstBuying != 0 ){
        nBonusGCRatio = g_kGlobalValue.m_nCashbackRatioFirstBuying;
    } else {
        std::map< GCITEMID, int >::iterator it = g_kGlobalValue.m_kCashbackRatioInfo.m_mapCashbackEventRatio.find( m_kItem.dwGoodsID * 10 );
        if( it == g_kGlobalValue.m_kCashbackRatioInfo.m_mapCashbackEventRatio.end() ) { // 이벤트 비율이 없으므로 일반 비율로 적립
            it = g_kGlobalValue.m_kCashbackRatioInfo.m_mapCashbackDefaultRatio.find( m_kItem.dwGoodsID * 10 );

            if( it == g_kGlobalValue.m_kCashbackRatioInfo.m_mapCashbackDefaultRatio.end() ){ // 일반 비율 적립도 없으면 그냥 0
                nBonusGCRatio = 0;
            } else { // 일반 비율로 적립
                nBonusGCRatio = (*it).second;
            }
        } else { // 이벤트 비율로 적립
            nBonusGCRatio = (*it).second;
        }
    }

    // 캐쉬백은 0% 이상일 때만 표시
    if( nBonusGCRatio != 0 ){
        float fBonusGC = std::ceil( static_cast< float >( m_kItem.dwPrice ) * static_cast< float >( nBonusGCRatio ) / 100.0f );
        m_pkStaticDesc2->SetText( g_pkStrLoader->GetReplacedString( STR_ID_BUY_CASH_ITEM_S6_STR_2, "ii", static_cast<int>( fBonusGC ), nBonusGCRatio ) );
        m_pkStaticDesc3->SetText( g_pkStrLoader->GetString( STR_ID_BUY_CASH_ITEM_S6_STR_3 ) );
        m_pkDot->ToggleRender( true );
    } else {
        m_pkStaticDesc2->SetText( L"" );
        m_pkStaticDesc3->SetText( L"" );
        m_pkDot->ToggleRender( false );
    }

    // 아이템 이미지
    if ( m_kItem.dwSlotPosition & ESP_SET_ITEM )
    {
        m_pkSetItemImgWnd->ToggleRender( true );
        m_pkSetItemImgWnd->SetItemInfo( m_kItem.iSetItemNumber, m_kItem.dwGoodsID , 150.f, 150.f );
        m_pkSetItemImgWnd->ShowItem( true );
        m_pkItemImgWnd->ToggleRender( false );
        m_pkItemImgWnd->ShowItem( false );
    } else {
        m_pkItemImgWnd->ToggleRender( true );
        m_pkItemImgWnd->SetItemInfo( m_kItem.dwGoodsID );
        m_pkItemImgWnd->ShowItem( true );
        m_pkSetItemImgWnd->ToggleRender( false );
        m_pkSetItemImgWnd->ShowItem( false );
    }
}
