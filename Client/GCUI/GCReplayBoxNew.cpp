#include "stdafx.h"
#include "GCReplayBoxNew.h"

IMPLEMENT_CLASSNAME( KGCReplayBoxNew );
IMPLEMENT_WND_FACTORY( KGCReplayBoxNew );
IMPLEMENT_WND_FACTORY_NAME( KGCReplayBoxNew, "gc_replay_box_new" );

KGCReplayBoxNew::KGCReplayBoxNew( void )
{
    m_pkOkBtn   = NULL;
    LINK_CONTROL( "ok_btn", m_pkOkBtn );

    m_pkOldReplayBtn = NULL;
    LINK_CONTROL( "old_replay_btn", m_pkOldReplayBtn );

    m_pkStaticDesc  = NULL;
    LINK_CONTROL( "static_desc", m_pkStaticDesc );
}

KGCReplayBoxNew::~KGCReplayBoxNew( void )
{
}

void KGCReplayBoxNew::OnCreate( void )
{
    m_pkOkBtn->InitState(true, true, this);
    m_pkOkBtn->SetHotKey( DIK_ESCAPE );
    m_pkOldReplayBtn->InitState(true, true, this);

    m_pkStaticDesc->InitState(true, false, NULL);
    m_pkStaticDesc->SetMultiLine(true);
    m_pkStaticDesc->SetShadow(true);
    m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString(STR_ID_REPLAY_BOX_NEW_DESC) );
}

void KGCReplayBoxNew::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkOkBtn,    KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkOldReplayBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnOldReplay );
}

void KGCReplayBoxNew::OnOldReplay( void )
{
    // 현재 창 닫고 
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );

    // 이전 리플레이 재생 화면 
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_REPLAY_S6 );
}

void KGCReplayBoxNew::OnClose( void )
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

