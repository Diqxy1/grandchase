#include "stdafx.h"
#include "GCBuyCashItemAccumulatedGCDlg.h"

IMPLEMENT_CLASSNAME( KGCBuyCashItemAccumulatedGCDlg );
IMPLEMENT_WND_FACTORY( KGCBuyCashItemAccumulatedGCDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCBuyCashItemAccumulatedGCDlg, "gc_accumulated_gc" );

KGCBuyCashItemAccumulatedGCDlg::KGCBuyCashItemAccumulatedGCDlg( void )
{
    m_pkBtnClose = NULL;
    m_pkBtnOk = NULL;
    LINK_CONTROL( "btn_close", m_pkBtnClose );
    LINK_CONTROL( "btn_ok", m_pkBtnOk );

    //m_pkStaticGC1 = NULL;
    //m_pkStaticGC2 = NULL;
    //LINK_CONTROL( "static_GC1", m_pkStaticGC1 );
    //LINK_CONTROL( "static_GC2", m_pkStaticGC2 );

    m_pkStaticGainedDesc = NULL;
    m_pkStaticTotalDesc = NULL;
    m_pkStaticGained = NULL;
    m_pkStaticTotal = NULL;
    LINK_CONTROL( "static_gc_gained_desc", m_pkStaticGainedDesc );
    LINK_CONTROL( "static_gc_total_desc", m_pkStaticTotalDesc );
    LINK_CONTROL( "static_gc_gained", m_pkStaticGained );
    LINK_CONTROL( "static_gc_total", m_pkStaticTotal );

}

KGCBuyCashItemAccumulatedGCDlg::~KGCBuyCashItemAccumulatedGCDlg( void )
{
}

void KGCBuyCashItemAccumulatedGCDlg::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey (DIK_ESCAPE );

    m_pkBtnOk->InitState( true, true, this );

    //m_pkStaticGC1->InitState( true );
    //m_pkStaticGC2->InitState( true );

    m_pkStaticGainedDesc->InitState( true );
    m_pkStaticTotalDesc->InitState( true );
    m_pkStaticGained->InitState( true );
    m_pkStaticGained->SetAlign( DT_RIGHT );
    m_pkStaticTotal->InitState( true );
    m_pkStaticTotal->SetAlign( DT_RIGHT );
    m_pkStaticTotal->SetFontColor( D3DCOLOR_ARGB(255, 0, 255, 0) ); // 녹색

}

void KGCBuyCashItemAccumulatedGCDlg::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnClose,           KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );
    _GCWND_MSG_MAP( m_pkBtnOk,           KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );

}

void KGCBuyCashItemAccumulatedGCDlg::OnClickClose()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
}

void KGCBuyCashItemAccumulatedGCDlg::SetAccumulateInfo( IN KVirtualCashInfo& m_kVirtualCashInfo_ )
{
    //m_pkStaticGC1->SetText( g_pkStrLoader->GetString( STR_ID_BUY_CASH_ITEM_S6_STR_4 ) );
    //m_pkStaticGC2->SetText( g_pkStrLoader->GetString( STR_ID_BUY_CASH_ITEM_S6_STR_4 ) );

    m_pkStaticGainedDesc->SetText( g_pkStrLoader->GetString( STR_ID_BUY_CASH_ITEM_S6_STR_8 ) );
    m_pkStaticTotalDesc->SetText( g_pkStrLoader->GetString( STR_ID_BUY_CASH_ITEM_S6_STR_6 ) );
    m_pkStaticGained->SetText( m_kVirtualCashInfo_.m_dwAddCashPoint );
    m_pkStaticTotal->SetText( m_kVirtualCashInfo_.m_dwCashPoint );

    // 현재 가상캐쉬량 갱신
    g_kGlobalValue.m_dwVirtualCash = m_kVirtualCashInfo_.m_dwCashPoint;
    if ( g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_CASH )
        g_pkUIScene->m_pkCashShopScene->UpdateUserInfo();
    if ( g_MyD3D->m_pStateMachine->GetState() == GS_SHOP_GP )
        g_pkUIScene->m_pkGPShopScene->UpdateUserInfo();
}


