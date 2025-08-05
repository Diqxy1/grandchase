#include "stdafx.h"
#include "GCTransferMissionScrollGuideGPCash.h"
#include "GCTransferMissionScrollGuideCash.h"

IMPLEMENT_CLASSNAME( KGCTransferMissionScrollGuideDlg );
IMPLEMENT_WND_FACTORY( KGCTransferMissionScrollGuideDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCTransferMissionScrollGuideDlg, "gc_transfer_mission_scroll_dlg" );

KGCTransferMissionScrollGuideDlg::KGCTransferMissionScrollGuideDlg( void )
{
    m_pkStaticMent = NULL;
    LINK_CONTROL( "static_ment", m_pkStaticMent );

    m_pkTransferMissionScrollGuideCash = NULL;
    LINK_CONTROL( "mission_guide_cash", m_pkTransferMissionScrollGuideCash );

    m_pkTransferMissionScrollGuideGpCash = NULL;
    LINK_CONTROL( "mission_guide_gp_cash", m_pkTransferMissionScrollGuideGpCash );

    m_dwGPMissionItemID = 0;
    m_dwCashMissionItemID = 0;

    m_bGotMsg = false;
    m_nGuideType = MSGT_MAX;
}

KGCTransferMissionScrollGuideDlg::~KGCTransferMissionScrollGuideDlg( void )
{
}
\
void KGCTransferMissionScrollGuideDlg::ActionPerformed( const KActionEvent& event )
{
    //_GCWND_MSG_MAP( m_pkBtnClose,           KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );
    //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TRANSFER_MISSION_SCROLL_GUIDE_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, false );
}

void KGCTransferMissionScrollGuideDlg::OnCreate( void )
{
    m_pkStaticMent->InitState( true );
    m_pkStaticMent->SetMultiLine( true );
    m_pkStaticMent->SetFontColor(D3DCOLOR_ARGB( 255, 0, 0, 0));
    m_pkStaticMent->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_TRANSFER_MISSION_SCROLL_GUIDE_2 ) );

    m_pkTransferMissionScrollGuideCash->InitState( false, true, this );
    m_pkTransferMissionScrollGuideGpCash->InitState( false, true, this );

    if ( m_dwGPMissionItemID == -1 ){
        m_nGuideType = MSGT_CASH;
        m_pkTransferMissionScrollGuideCash->SetMissionItemID( m_dwCashMissionItemID );
    } 
	else if ( m_dwCashMissionItemID == -1) {
		m_nGuideType = MSGT_GP;
		m_pkTransferMissionScrollGuideCash->SetMissionItemID( m_dwGPMissionItemID );
	}
	else {
        m_nGuideType = MSGT_GP_CASH;
        m_pkTransferMissionScrollGuideGpCash->SetMissionItemID( m_dwGPMissionItemID, m_dwCashMissionItemID );
    }

    if ( m_nGuideType == MSGT_GP_CASH ) {
        m_pkTransferMissionScrollGuideGpCash->ToggleRender( true );
        m_pkTransferMissionScrollGuideCash->ToggleRender( false );
    } else if ( m_nGuideType == MSGT_CASH || m_nGuideType == MSGT_GP ) {
        m_pkTransferMissionScrollGuideCash->ToggleRender( true );
        m_pkTransferMissionScrollGuideGpCash->ToggleRender( false );
    }
}

void KGCTransferMissionScrollGuideDlg::FrameMoveInEnabledState( void )
{
    //
}

void KGCTransferMissionScrollGuideDlg::SetMissionGuideInfo( KEVENT_JOB_MISSION_GUIDE_NOT& kRecv_ )
{
    m_bGotMsg = true;

    m_dwGPMissionItemID = kRecv_.m_dwGPMissionID;
    m_dwCashMissionItemID = kRecv_.m_dwCashMissionID;
}

void KGCTransferMissionScrollGuideDlg::ShowMissionGuide()
{
    if( m_bGotMsg == false )
        return;

    g_pkUIScene->EndMessageBox();

    g_pkUIScene->m_pkTransferMissionScrollGuideDlg->SetWindowZOrder(KD3DWnd::D3DWZ_TOPMOST);
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_TRANSFER_MISSION_SCROLL_GUIDE_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );

    m_bGotMsg = false;
}

void KGCTransferMissionScrollGuideDlg::OnClickClose()
{
    SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE));
}
