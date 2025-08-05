#include "stdafx.h"
#include "GCInfinityGuideDlg.h"

IMPLEMENT_CLASSNAME( KGCInfinityGuideDlg );
IMPLEMENT_WND_FACTORY( KGCInfinityGuideDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCInfinityGuideDlg, "gc_infinityguidedlg" );

KGCInfinityGuideDlg::KGCInfinityGuideDlg( void )
{
    m_pkBtnClose = NULL;
    m_pkBtnQuickJoin = NULL;
    
    LINK_CONTROL( "btn_close", m_pkBtnClose );
    LINK_CONTROL( "quick_join_btn", m_pkBtnQuickJoin );
}

KGCInfinityGuideDlg::~KGCInfinityGuideDlg( void )
{
}

void KGCInfinityGuideDlg::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );

    m_pkBtnQuickJoin->InitState( true, true, this );
}

void KGCInfinityGuideDlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkBtnQuickJoin, KD3DWnd::D3DWE_BUTTON_CLICK, OnQuickJoin );
}


void KGCInfinityGuideDlg::OnClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCInfinityGuideDlg::OnQuickJoin()
{
    SiKGCRoomManager()->CreateRoomDirect( GC_GMC_DUNGEON, static_cast<EGCGameMode>(SiKGCInfinityDungeonManager()->GetGuideGameMode()) );
    OnClose();
}