#include "stdafx.h"
#include "GCEscMenuDlg.h"

IMPLEMENT_CLASSNAME( KGCEscMenuDlg );
IMPLEMENT_WND_FACTORY( KGCEscMenuDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCEscMenuDlg, "gc_escmenudlg" );

KGCEscMenuDlg::KGCEscMenuDlg( void )
: m_pkBtnChangeChar( NULL ) 
, m_pkBtnChangeServer( NULL ) 
, m_pkBtnChangeOption( NULL ) 
, m_pkBtnExitGame( NULL ) 
, m_pkBtnClose( NULL ) 
{
    LINK_CONTROL( "change_char", m_pkBtnChangeChar );
    LINK_CONTROL( "change_server", m_pkBtnChangeServer );
    LINK_CONTROL( "change_option", m_pkBtnChangeOption );
    LINK_CONTROL( "exit_game", m_pkBtnExitGame );
    LINK_CONTROL( "btn_close", m_pkBtnClose );
}

KGCEscMenuDlg::~KGCEscMenuDlg( void )
{
}

void KGCEscMenuDlg::OnCreate( void )
{
    m_pkBtnChangeChar->InitState( true, true, this );
    m_pkBtnChangeServer->InitState( true, true, this );
    m_pkBtnChangeOption->InitState( true, true, this );
    m_pkBtnExitGame->InitState( true, true, this );
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
}

void KGCEscMenuDlg::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnChangeChar, KD3DWnd::D3DWE_BUTTON_CLICK, OnChangeChar );
    _GCWND_MSG_MAP( m_pkBtnChangeServer, KD3DWnd::D3DWE_BUTTON_CLICK, OnChangeServer );
    _GCWND_MSG_MAP( m_pkBtnChangeOption, KD3DWnd::D3DWE_BUTTON_CLICK, OnChangeOption );
    _GCWND_MSG_MAP( m_pkBtnExitGame, KD3DWnd::D3DWE_BUTTON_CLICK, OnExitGame );
    _GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
}

void KGCEscMenuDlg::OnChangeChar()
{
    g_MyD3D->OnChaSelectSceneBtn();
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCEscMenuDlg::OnChangeServer()
{
    g_MyD3D->OnServerSceneBtn();
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCEscMenuDlg::OnChangeOption()
{
    g_MyD3D->OnOptionButton();
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCEscMenuDlg::OnExitGame()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_EXIT_GAME, g_pkStrLoader->GetString(STR_ID_CONFIRM_EXIT), L""
        , KGCUIScene::GC_MBOX_USE_GAMEEXIT, 0, 0, false, false );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCEscMenuDlg::OnClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

