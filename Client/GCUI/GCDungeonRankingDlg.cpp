#include "stdafx.h"
#include "GCDungeonRankingDlg.h"
#include "GCDungeonRankingClearData.h"
#include "GCDungeonRankingPanel.h"

IMPLEMENT_CLASSNAME( KGCDungeonRankingDlg );
IMPLEMENT_WND_FACTORY( KGCDungeonRankingDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCDungeonRankingDlg, "gc_dungeonrankingdlg" );

KGCDungeonRankingDlg::KGCDungeonRankingDlg( void )
{
    m_pkCloseBtn = NULL;
    m_pkHelpBtn = NULL;
    m_pkHelpOnBtn = NULL;

    m_pkCurRankignBtn = NULL;
    m_pkOldRankingBtn = NULL;

    m_pkCurRankImg = NULL;
    m_pkOldRankImg = NULL;

    m_pkEclipse = NULL;

    m_nState = EM_CURRENT_RANK;

    m_pkClearData = NULL;
    m_pkRankingPanel = NULL;

    char str[MAX_PATH] = {0,};
    for ( int i = 0; i < EM_TOOLTIP_NUM; ++i ) 
    {
        m_pkToolTip[i] = NULL;
        m_pkToolTipText[i] = NULL;

        sprintf( str, "help_tooltip%d", i );  
        LINK_CONTROL( str, m_pkToolTip[i] );


        sprintf( str, "static_help%d", i );  
        LINK_CONTROL( str, m_pkToolTipText[i] );
    }


    LINK_CONTROL( "close_btn", m_pkCloseBtn );
    LINK_CONTROL( "help_btn", m_pkHelpBtn );
    LINK_CONTROL( "help_on_btn", m_pkHelpOnBtn );
    
    LINK_CONTROL( "current_ranking_btn", m_pkCurRankignBtn );
    LINK_CONTROL( "old_ranking_btn", m_pkOldRankingBtn );

    LINK_CONTROL( "current_ranking_on", m_pkCurRankImg );
    LINK_CONTROL( "old_ranking_on", m_pkOldRankImg );

    LINK_CONTROL( "background_eclipse", m_pkEclipse );


    LINK_CONTROL( "clear_data", m_pkClearData );
    LINK_CONTROL( "ranking_panel", m_pkRankingPanel );
}

KGCDungeonRankingDlg::~KGCDungeonRankingDlg( void )
{
}

void KGCDungeonRankingDlg::OnCreate( void )
{
    m_pkCloseBtn->InitState( true, true, this );
    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );
    
    m_pkHelpBtn->InitState( true, true, this );
    m_pkHelpOnBtn->InitState( false, true, this );
    m_pkHelpOnBtn->SetHotKey( DIK_ESCAPE );

    m_pkCurRankignBtn->InitState( false, true, this );
    m_pkOldRankingBtn->InitState( true, true, this );

    m_pkCurRankImg->InitState( true, false, NULL );
    m_pkOldRankImg->InitState( false, false, NULL );

    m_pkEclipse->InitState( false, false, NULL );
    m_pkEclipse->ForceSetWindowColor( D3DCOLOR_ARGB( BACKGROUND_ALPHA, 0x00, 0x00, 0x00 ) );

    for ( int i = 0; i < EM_TOOLTIP_NUM; ++i ) 
    {
        m_pkToolTip[i]->InitState( false, false, NULL );
        m_pkToolTipText[i]->InitState( false, false, NULL );
        m_pkToolTipText[i]->SetForceWordWrapText( g_pkStrLoader->GetString( STR_ID_DUNGEON_RANK_TOOLTIP0 + i ), true );
    }

    m_pkClearData->InitState( true, true, this );
    m_pkRankingPanel->InitState( true, true, this );

    m_nState = EM_CURRENT_RANK;
}

void KGCDungeonRankingDlg::OnCreateComplete( void )
{
    m_pkClearData->UpdateUI();
    m_pkRankingPanel->UpdateRankingData();
}   

void KGCDungeonRankingDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkCloseBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkHelpBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnHelp );
    GCWND_MSG_MAP( m_pkHelpOnBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnHelpClose );

    GCWND_MSG_MAP( m_pkCurRankignBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnCurRanking );
    GCWND_MSG_MAP( m_pkOldRankingBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnOldRanking );    
}

void KGCDungeonRankingDlg::UpdateBtnUI()
{
    bool bCurrent = m_nState == EM_CURRENT_RANK;

    m_pkCurRankignBtn->ToggleRender( !bCurrent );
    m_pkOldRankingBtn->ToggleRender( bCurrent );

    m_pkCurRankImg->ToggleRender( bCurrent );
    m_pkOldRankImg->ToggleRender( !bCurrent );
}

void KGCDungeonRankingDlg::UpdateHelpUI( bool bRender )
{
    m_pkEclipse->ToggleRender( bRender );

    for ( int i = 0; i < EM_TOOLTIP_NUM; ++i )
    {
        m_pkToolTip[i]->ToggleRender( bRender );
        m_pkToolTipText[i]->ToggleRender( bRender );
    }

    m_pkHelpBtn->ToggleRender( !bRender );
    m_pkHelpOnBtn->ToggleRender( bRender );
}

void KGCDungeonRankingDlg::UpdateClearData()
{
    m_pkClearData->UpdateUI();
}

void KGCDungeonRankingDlg::UpdateRankingData()
{
    m_pkRankingPanel->UpdateRankingData();
}

void KGCDungeonRankingDlg::UpdateRankingUI()
{
    m_pkRankingPanel->UpdateUI();
}

void KGCDungeonRankingDlg::SetPage( int nPage )
{
    m_pkRankingPanel->SetPage( nPage );
}

void KGCDungeonRankingDlg::LockUI( bool bLock )
{
    m_pkRankingPanel->LockUI( bLock );
}

void KGCDungeonRankingDlg::OnClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCDungeonRankingDlg::OnHelp()
{
    UpdateHelpUI( true );
}

void KGCDungeonRankingDlg::OnHelpClose()
{
    UpdateHelpUI( false );
}

void KGCDungeonRankingDlg::OnCurRanking()
{
    m_nState = EM_CURRENT_RANK;
    
    m_pkRankingPanel->SetRankingState( EM_CURRENT_RANK );
    UpdateBtnUI();
    UpdateRankingData();
}

void KGCDungeonRankingDlg::OnOldRanking()
{
    m_nState = EM_OLD_RANK;

    m_pkRankingPanel->SetRankingState( EM_OLD_RANK );
    UpdateBtnUI();
    UpdateRankingData();
}