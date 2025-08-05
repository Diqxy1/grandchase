#include "stdafx.h"

#include "GCChallengeErrandDlg.h"
#include "GCChallengeErrandPanel.h"


IMPLEMENT_CLASSNAME( KGCChallengeErrandDlg );
IMPLEMENT_WND_FACTORY( KGCChallengeErrandDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCChallengeErrandDlg, "gc_challenge_errand_dlg" );


KGCChallengeErrandDlg::KGCChallengeErrandDlg( void ):
m_pkBtnClose( NULL ),
m_pkBtnStart( NULL ),
m_pkBtnReward( NULL ),
m_pkBtnHelp( NULL ),
m_pkErrandHelpDlg( NULL ),
m_pkErrandPanel( NULL )
{
    LINK_CONTROL( "btn_close", m_pkBtnClose );
    LINK_CONTROL( "errand_panel", m_pkErrandPanel );
    LINK_CONTROL( "btn_start", m_pkBtnStart );
    LINK_CONTROL( "btn_reward", m_pkBtnReward );
    LINK_CONTROL( "btn_help", m_pkBtnHelp );

    LINK_CONTROL( "errand_help", m_pkErrandHelpDlg );

}

KGCChallengeErrandDlg::~KGCChallengeErrandDlg( void )
{
    
}

void KGCChallengeErrandDlg::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkErrandPanel->InitState( true, true, this );
    m_pkBtnStart->InitState(true, true, this );
    m_pkBtnReward->InitState( true, true, this );
    m_pkBtnHelp->InitState( true, true, this );
    m_pkErrandHelpDlg->InitState( false, false, this );


    m_pkBtnStart->Lock( true );
    m_pkBtnReward->Lock( true );
}

void KGCChallengeErrandDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, IsClose );
    GCWND_MSG_MAP( m_pkBtnStart, KD3DWnd::D3DWE_BUTTON_CLICK, OnStart );
    GCWND_MSG_MAP( m_pkBtnReward, KD3DWnd::D3DWE_BUTTON_CLICK, OnReward );
    GCWND_MSG_MAP( m_pkBtnHelp, KD3DWnd::D3DWE_BUTTON_CLICK, OnHelp );
}

void KGCChallengeErrandDlg::FrameMoveInEnabledState( void )
{
    if (m_pkErrandHelpDlg->IsRenderOn())
        m_pkBtnClose->Lock(true);
    else
        m_pkBtnClose->Lock(false);

    if( m_pkErrandPanel->GetPanelState() == KGCChallengeErrandPanel::ERRAND_CHECK_GAME )
    {
        m_pkBtnStart->Lock( false );
        m_pkBtnReward->Lock( true );
    }

    if( m_pkErrandPanel->GetPanelState() == KGCChallengeErrandPanel::ERRAND_PLAYING_GAME )
    {
        m_pkBtnStart->Lock( true );
        m_pkBtnReward->Lock( true );
    }

    if( m_pkErrandPanel->GetPanelState() == KGCChallengeErrandPanel::ERRAND_END_GAME )
    {
        m_pkBtnStart->Lock( true );
        m_pkBtnReward->Lock( false );
    }

    if( m_pkErrandPanel->GetPanelState() == KGCChallengeErrandPanel::ERRAND_WAIT_REMAIN )
    {
        m_pkBtnStart->Lock( true );
        m_pkBtnReward->Lock( true );
    }
}

void KGCChallengeErrandDlg::OnClose()
{
    m_pkErrandPanel->ClearShop();
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCChallengeErrandDlg::OnStart()
{
    if( m_pkErrandPanel != NULL )
    {
        m_pkErrandPanel->OnStart();

    }
}

void KGCChallengeErrandDlg::OnReward()
{
    if( m_pkErrandPanel != NULL )
    {
        m_pkErrandPanel->OnReward();
    }
}

void KGCChallengeErrandDlg::OnHelp()
{
    m_pkErrandHelpDlg->InitState( true );
}

void KGCChallengeErrandDlg::GamExitRewardPacket()
{
    if( m_pkErrandPanel->GetPanelState() == KGCChallengeErrandPanel::ERRAND_END_GAME )
    {
        m_pkErrandPanel->GameRewardReq();
    }
}

void KGCChallengeErrandDlg::HelpRewardList()
{
    if( m_pkErrandHelpDlg != NULL )
        m_pkErrandHelpDlg->SetRewardItemImg();
}

void KGCChallengeErrandDlg::IsClose()
{
    if( m_pkErrandPanel->GetReward() )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_ERRAND_EXIT_COMPLATE ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
        OnClose();
    }
    else
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO ,g_pkStrLoader->GetString( STR_ID_ERRAND_EXIT ), L"", KGCUIScene::GC_MBOX_USE_ERRAND_EXIT,0, 0, false, true );
    }
}
