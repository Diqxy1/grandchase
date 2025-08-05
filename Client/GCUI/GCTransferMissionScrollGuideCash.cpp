#include "stdafx.h"
#include "GCTransferMissionScrollGuideCash.h"
#include "GCTransferMissionScrollItemWnd.h"


IMPLEMENT_CLASSNAME( KGCTransferMissionScrollGuideCash );
IMPLEMENT_WND_FACTORY( KGCTransferMissionScrollGuideCash );
IMPLEMENT_WND_FACTORY_NAME( KGCTransferMissionScrollGuideCash, "gc_mission_guide_cash" );

KGCTransferMissionScrollGuideCash::KGCTransferMissionScrollGuideCash( void )
{
    m_pkBtnClose = NULL;
    LINK_CONTROL( "btn_close", m_pkBtnClose );

    m_pkItemBoxCash = NULL;
    LINK_CONTROL( "mission_scroll_item_wnd", m_pkItemBoxCash );

}

KGCTransferMissionScrollGuideCash::~KGCTransferMissionScrollGuideCash( void )
{
}

void KGCTransferMissionScrollGuideCash::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnClose,           KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );
}

void KGCTransferMissionScrollGuideCash::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey (DIK_ESCAPE );

    m_pkItemBoxCash->InitState( true, true, this );
}

void KGCTransferMissionScrollGuideCash::FrameMoveInEnabledState( void )
{
    //
}

void KGCTransferMissionScrollGuideCash::OnClickClose()
{
    this->ToggleRender( false );
    g_pkUIScene->m_pkTransferMissionScrollGuideDlg->OnClickClose();
}

void KGCTransferMissionScrollGuideCash::SetMissionItemID( DWORD& dwCashMissionItemID )
{
    m_pkItemBoxCash->SetItemID( dwCashMissionItemID );
}
