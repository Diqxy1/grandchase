#include "stdafx.h"
#include "GCChristmasDonateDlg.h"

IMPLEMENT_CLASSNAME( KGCChristmasDonateDlg );
IMPLEMENT_WND_FACTORY( KGCChristmasDonateDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCChristmasDonateDlg, "gc_christmas_donate_box" );

KGCChristmasDonateDlg::KGCChristmasDonateDlg( void )
{
    m_pkBtnIncrease = NULL;
    m_pkBtnDecrease = NULL;
    m_pkBtnDonate = NULL;
    m_pkBtnClose = NULL;
    LINK_CONTROL( "btn_increase", m_pkBtnIncrease );
    LINK_CONTROL( "btn_decrease", m_pkBtnDecrease );
    LINK_CONTROL( "btn_donate", m_pkBtnDonate );
    LINK_CONTROL( "btn_close", m_pkBtnClose );

    m_pkStaticDonate = NULL;
    m_pkStaticDescription = NULL;
    m_pkStaticRibbonCount = NULL;
    LINK_CONTROL( "static_donate", m_pkStaticDonate );
    LINK_CONTROL( "static_desc", m_pkStaticDescription );
    LINK_CONTROL( "static_ribbon_count", m_pkStaticRibbonCount );

    // 기부할 수량
    m_dwSledgeCountToDonate = 0;
}

KGCChristmasDonateDlg::~KGCChristmasDonateDlg( void )
{
}



void KGCChristmasDonateDlg::OnCreate( void )
{
    m_pkBtnIncrease->InitState( true, true, this );
    m_pkBtnDecrease->InitState( true, true, this );
    m_pkBtnDonate->InitState( true, true, this );
    m_pkBtnClose->InitState( true, true, this );

    m_pkStaticDonate->InitState( true, false, this );
    m_pkStaticDonate->SetAlign( DT_CENTER );
    m_pkStaticDonate->SetFontColor(D3DCOLOR_ARGB( 255, 255, 255, 255));
    m_pkStaticDonate->SetFontOutline( true );

    m_pkStaticDescription->InitState( true, false, this );
    m_pkStaticDescription->SetAlign( DT_LEFT );
    m_pkStaticDescription->SetFontColor(D3DCOLOR_ARGB( 255, 255, 255, 255));
    m_pkStaticDescription->SetMultiLine( true );


    m_pkStaticRibbonCount->InitState( true, false, this );
    m_pkStaticRibbonCount->SetAlign( DT_RIGHT );
    m_pkStaticRibbonCount->SetFontColor(D3DCOLOR_ARGB( 255, 255, 255, 255));

    InitEventInfo();


}


void KGCChristmasDonateDlg::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnIncrease,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickIncrease );
    _GCWND_MSG_MAP( m_pkBtnDecrease,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickDecrease );
    _GCWND_MSG_MAP( m_pkBtnDonate,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickDonate );
    _GCWND_MSG_MAP( m_pkBtnClose,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );
}

void KGCChristmasDonateDlg::InitEventInfo()
{
    // 제목
    m_pkStaticDonate->SetText( g_pkStrLoader->GetString( STR_ID_DONATE ) );

    // 설명
    m_pkStaticDescription->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_HOW_MANY_DONATE_YOUR_RIBBON, "i", static_cast<int>( dwCountBy + 1 ) ) );

    // 리본 갯수
    m_pkStaticRibbonCount->SetText( m_dwSledgeCountToDonate );
}


void KGCChristmasDonateDlg::OnClickDonate()
{
    g_KDSound.Play( "31" );

    if( g_pkUIScene->m_pkChristmasGiftDlg->IsBonusPointMax() ) {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_YOUR_POINT_IS_MAXIMUM), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    }

    if ( m_dwSledgeCountToDonate > 0 ){
        g_pkUIScene->m_pkChristmasGiftDlg->SendChristmasEventDonateReq( m_dwSledgeCountToDonate );

        m_dwSledgeCountToDonate = 0;
        m_pkStaticRibbonCount->SetText( 0 );
        this->ToggleRender( false );
    }
}

void KGCChristmasDonateDlg::OnClickIncrease()
{
    if( g_pkUIScene->m_pkChristmasGiftDlg->GetCurrentSledgeCount() < dwCountBy ){
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_ID_NOT_ENOUGH_RIBBON), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
        return;
    }

    if( g_pkUIScene->m_pkChristmasGiftDlg->GetCurrentSledgeCount() >= m_dwSledgeCountToDonate + dwCountBy ){
        m_dwSledgeCountToDonate += dwCountBy;
    }

    m_pkStaticRibbonCount->SetText( m_dwSledgeCountToDonate );
}

void KGCChristmasDonateDlg::OnClickDecrease()
{
    if( m_dwSledgeCountToDonate > 0)
        m_dwSledgeCountToDonate -= dwCountBy;

    m_pkStaticRibbonCount->SetText( m_dwSledgeCountToDonate );
}

void KGCChristmasDonateDlg::OnClickClose()
{
    this->ToggleRender( false );
}
