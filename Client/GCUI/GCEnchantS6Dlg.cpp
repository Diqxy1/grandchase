#include "stdafx.h"
#include "GCEnchantS6Dlg.h"

IMPLEMENT_CLASSNAME( KGCEnchantS6Dlg );
IMPLEMENT_WND_FACTORY( KGCEnchantS6Dlg );
IMPLEMENT_WND_FACTORY_NAME( KGCEnchantS6Dlg, "gc_enchants6dlg" );

KGCEnchantS6Dlg::KGCEnchantS6Dlg( void )
{
    m_pkBtnClose = NULL;
    m_pkBtnEnchant = NULL;
    m_pkBtnBreakup = NULL;
    m_pkBtnAlchemy = NULL;

    LINK_CONTROL( "btn_close", m_pkBtnClose );
    LINK_CONTROL( "btn_enchant", m_pkBtnEnchant );
    LINK_CONTROL( "btn_breakup", m_pkBtnBreakup );
    LINK_CONTROL( "btn_alchemy", m_pkBtnAlchemy );
}

KGCEnchantS6Dlg::~KGCEnchantS6Dlg( void )
{
}

void KGCEnchantS6Dlg::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );

    m_pkBtnEnchant->InitState( true, true, this );
    m_pkBtnBreakup->InitState( true, true, this );
    m_pkBtnAlchemy->InitState( true, true, this );
}

void KGCEnchantS6Dlg::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
    GCWND_MSG_MAP( m_pkBtnEnchant, KD3DWnd::D3DWE_BUTTON_CLICK, OnEnchant );
    GCWND_MSG_MAP( m_pkBtnBreakup, KD3DWnd::D3DWE_BUTTON_CLICK, OnBreakup );
    GCWND_MSG_MAP( m_pkBtnAlchemy, KD3DWnd::D3DWE_BUTTON_CLICK, OnAlchemy );
}

void KGCEnchantS6Dlg::OnClose()
{
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCEnchantS6Dlg::OnEnchant()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ENCHANT_SUB_S6_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 
        KGCEnchantSubS6Dlg::EM_STATE_ENCHANT, 0, true, true ); 
}

void KGCEnchantS6Dlg::OnBreakup()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ENCHANT_SUB_S6_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 
        KGCEnchantSubS6Dlg::EM_STATE_BREAKEUP, 0, true, true ); 
}

void KGCEnchantS6Dlg::OnAlchemy()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_ENCHANT_SUB_S6_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 
        KGCEnchantSubS6Dlg::EM_STATE_ALCHEMY, 0, true, true ); 
}
