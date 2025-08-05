#include "stdafx.h"
#include "KGCAskUseVirtualCash.h"
//
//
//
//
//
#include "MyD3D.h"


IMPLEMENT_CLASSNAME( KGCAskUseVirtualCash );
IMPLEMENT_WND_FACTORY( KGCAskUseVirtualCash );
IMPLEMENT_WND_FACTORY_NAME( KGCAskUseVirtualCash, "gc_ask_use_virtualcash" );

KGCAskUseVirtualCash::KGCAskUseVirtualCash(void)
{
    LINK_CONTROL( "background",                 m_pkBackGround );
    LINK_CONTROL( "static_title",               m_pkTitle );
    LINK_CONTROL( "static_content",             m_pkMainContent );
    LINK_CONTROL( "btn_ok",                     m_pkOkBtn );
    LINK_CONTROL( "btn_cancel",                 m_pkCancelBtn );
    LINK_CONTROL( "checkbox_use_virtualcash",   m_pkCheckBox );

    m_iItemID = -1;
    m_iPeriod = -1;
}

KGCAskUseVirtualCash::~KGCAskUseVirtualCash(void)
{
}

void KGCAskUseVirtualCash::OnCreate( void )
{
    m_pkBackGround->InitState( true, false, this );
    m_pkTitle->InitState( true, false, this );
    m_pkMainContent->InitState( true, false, this );

    m_pkOkBtn->InitState( true, true, this );
    m_pkCancelBtn->InitState( true, true, this );
    m_pkCheckBox->InitState( true, true, this );

    m_pkTitle->SetText( g_pkStrLoader->GetString(STR_ID_DLG_VIRTUAL_CASH_TITLE) );
    m_pkMainContent->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_DLG_VIRTUAL_CASH_CONTENT) );
    m_pkCheckBox->SetCheck( true );
    m_bNoDiagBox = false;
}

void KGCAskUseVirtualCash::ActionPerformed( const KActionEvent& event )
{
    if( event.m_pWnd == m_pkOkBtn
        || event.m_pWnd == m_pkCancelBtn )
    {
        if( KD3DWnd::D3DWE_BUTTON_CLICK == event.m_dwCode )
        {
            if( !m_bNoDiagBox )
            SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
            else
                this->ToggleRender(false);

            if( event.m_pWnd == m_pkOkBtn )
                BuyItem( m_pkCheckBox->GetCheck() );
        }
    }
    else if( event.m_pWnd == m_pkCheckBox )
    {
        if( KD3DWnd::D3DWE_BUTTON_CLICK == event.m_dwCode )
        {
            m_pkCheckBox->SetCheck( !m_pkCheckBox->GetCheck() );
        }
    }
}

void KGCAskUseVirtualCash::FrameMoveInEnabledState( void )
{
}

void KGCAskUseVirtualCash::BuyItem( bool bVirtualCash )
{
    if( -1 == m_iItemID )
        return;

    GCItem* kItem = g_MyD3D->m_kItemMgr.GetItemData( m_iItemID );

#ifdef VIRTUAL_CASH
    g_MyD3D->SetUseVirtualCash( bVirtualCash );

    if( bVirtualCash && kItem )
    {
        // 구매프로세스...
        if( !m_bNoDiagBox ){
        g_pkUIScene->EnableBuyCashItemWnd( kItem->dwGoodsID, false );
        } else {
            g_pkUIScene->EnableBuyCashItemWnd( kItem->dwGoodsID, false, true );
    }
    }
    else
#endif
    {
#if defined ( USE_VIP_POINT )
        g_pItemMgr->BuyCashItemS6( m_iItemID );
#else
    #ifdef GAME_CASH
        g_pkUIScene->EnableBuyCashItemWnd( kItem->dwGoodsID, false );
    #else    
        g_MyD3D->m_kItemMgr.BuyItem( m_iItemID, m_iPeriod  );
    #endif
#endif
    }

    m_iItemID = -1;
    m_iPeriod = -1;
}
