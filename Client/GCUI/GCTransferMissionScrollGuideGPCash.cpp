#include "stdafx.h"
#include "GCTransferMissionScrollGuideGPCash.h"
#include "GCTransferMissionScrollItemWnd.h"

IMPLEMENT_CLASSNAME( KGCTransferMissionScrollGuideGPCash );
IMPLEMENT_WND_FACTORY( KGCTransferMissionScrollGuideGPCash );
IMPLEMENT_WND_FACTORY_NAME( KGCTransferMissionScrollGuideGPCash, "gc_mission_guide_gp_cash" );

KGCTransferMissionScrollGuideGPCash::KGCTransferMissionScrollGuideGPCash( void )
{
    m_pkBtnClose = NULL;
    LINK_CONTROL( "btn_close", m_pkBtnClose );

    m_pkItemBoxGP = NULL;
    LINK_CONTROL( "mission_scroll_item_wnd0", m_pkItemBoxGP );

    m_pkItemBoxCash = NULL;
    LINK_CONTROL( "mission_scroll_item_wnd1", m_pkItemBoxCash );

}

KGCTransferMissionScrollGuideGPCash::~KGCTransferMissionScrollGuideGPCash( void )
{
}

void KGCTransferMissionScrollGuideGPCash::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnClose,           KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );
}

void KGCTransferMissionScrollGuideGPCash::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey (DIK_ESCAPE );

    m_pkItemBoxGP->InitState( true, true, this );
    m_pkItemBoxCash->InitState( true, true, this );
}

void KGCTransferMissionScrollGuideGPCash::FrameMoveInEnabledState( void )
{
    //
}

void KGCTransferMissionScrollGuideGPCash::OnClickClose()
{
    this->ToggleRender( false );
    g_pkUIScene->m_pkTransferMissionScrollGuideDlg->OnClickClose();
}

void KGCTransferMissionScrollGuideGPCash::SetMissionItemID( DWORD& dwGPMissionItemID, DWORD& dwCashMissionItemID )
{
    m_pkItemBoxGP->SetItemID( dwGPMissionItemID );
    m_pkItemBoxCash->SetItemID( dwCashMissionItemID );
}
