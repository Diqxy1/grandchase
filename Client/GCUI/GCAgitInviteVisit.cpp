#include "stdafx.h"
#include "GCAgitInviteVisit.h"
#include "GCAgitOption.h"

IMPLEMENT_CLASSNAME( KGCAgitInviteVisit );
IMPLEMENT_WND_FACTORY( KGCAgitInviteVisit );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitInviteVisit, "gc_agit_invite_visit" );

KGCAgitInviteVisit::KGCAgitInviteVisit( void )
{
    m_pkStaticInvite = NULL;
    m_pkStaticVisit = NULL;

    LINK_CONTROL( "staticInvite", m_pkStaticInvite );
    LINK_CONTROL( "staticVisit",  m_pkStaticVisit );

    for( int i=0 ; i<NUM_BTN ; ++i )
        m_apkBtn[i] = NULL;

    LINK_CONTROL( "btnRegister",    m_apkBtn[ BTN_REGIST ] );
    LINK_CONTROL( "btnFriendInvite",m_apkBtn[ BTN_FRIEND_INVITE ] );
    LINK_CONTROL( "btnAgitList",    m_apkBtn[ BTN_AGIT_LIST ] );
    LINK_CONTROL( "btnRandomAgit",  m_apkBtn[ BTN_RANDOM_AGIT ] );
    LINK_CONTROL( "btnClose",       m_apkBtn[ BTN_CLOSE ] );

    for( int i=0 ; i<NUM_BTN - 1 ; ++i )
        m_apkBtnStatic[i] = NULL;

    LINK_CONTROL( "staticRegister",     m_apkBtnStatic[ BTN_REGIST ] );
    LINK_CONTROL( "staticFriendInvite", m_apkBtnStatic[ BTN_FRIEND_INVITE ] );
    LINK_CONTROL( "staticAgitList",     m_apkBtnStatic[ BTN_AGIT_LIST ] );
    LINK_CONTROL( "staticRandomAgit",   m_apkBtnStatic[ BTN_RANDOM_AGIT ] );
}

KGCAgitInviteVisit::~KGCAgitInviteVisit( void )
{
}

void KGCAgitInviteVisit::OnCreate( void )
{
    m_pkStaticInvite->InitState( true, false, NULL );
    m_pkStaticVisit->InitState( true, false, NULL );
    m_pkStaticInvite->SetAlign( DT_CENTER );
    m_pkStaticVisit->SetAlign( DT_CENTER );

    m_pkStaticInvite->SetText( g_pkStrLoader->GetString( STR_ID_INVITE_AGIT ) );
    m_pkStaticVisit->SetText( g_pkStrLoader->GetString( STR_ID_VISIT_AGIT ) );
    m_pkStaticInvite->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );
    m_pkStaticVisit->SetFontOutline( true, D3DCOLOR_ARGB( 255, 39, 32, 26 ) );

    for( int i=0 ; i<NUM_BTN ; ++i )
        m_apkBtn[i]->InitState( true, true, this );
    m_apkBtn[BTN_RANDOM_AGIT]->InitState( false );

    for( int i=0 ; i<NUM_BTN - 1 ; ++i )
    {
        m_apkBtnStatic[i]->InitState( true, false, NULL );
        m_apkBtnStatic[i]->SetAlign( DT_CENTER );
    }

    m_apkBtnStatic[ BTN_REGIST ]->SetText( g_pkStrLoader->GetString( STR_ID_REGISTER_CHATTING_AGIT ) );
    m_apkBtnStatic[ BTN_FRIEND_INVITE ]->SetText( g_pkStrLoader->GetString( STR_ID_INVITE_FRIEND_AGIT ) );
    m_apkBtnStatic[ BTN_AGIT_LIST ]->SetText( g_pkStrLoader->GetString( STR_ID_CHATTING_AGIT ) );
    m_apkBtnStatic[ BTN_RANDOM_AGIT ]->ToggleRender(false);//->SetText( g_pkStrLoader->GetString( STR_ID_RANDOM_AGIT ) );

    m_apkBtnStatic[ BTN_REGIST ]->SetFontOutline( true, D3DCOLOR_ARGB( 255, 36, 25, 19 ) );
    m_apkBtnStatic[ BTN_FRIEND_INVITE ]->SetFontOutline( true, D3DCOLOR_ARGB( 255, 36, 25, 19 ) );
    m_apkBtnStatic[ BTN_AGIT_LIST ]->SetFontOutline( true, D3DCOLOR_ARGB( 255, 36, 25, 19 ) );
    //m_apkBtnStatic[ BTN_RANDOM_AGIT ]->SetFontOutline( true, D3DCOLOR_ARGB( 255, 36, 25, 19 ) );

    m_apkBtn[ BTN_CLOSE ]->SetHotKey( DIK_ESCAPE );
}

void KGCAgitInviteVisit::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_apkBtn[ BTN_REGIST ],          KD3DWnd::D3DWE_BUTTON_CLICK,    OnRegister );
    GCWND_MSG_MAP( m_apkBtn[ BTN_FRIEND_INVITE ],   KD3DWnd::D3DWE_BUTTON_CLICK,    OnFriendInvite );
    GCWND_MSG_MAP( m_apkBtn[ BTN_AGIT_LIST ],       KD3DWnd::D3DWE_BUTTON_CLICK,    OnAgitList );
    //GCWND_MSG_MAP( m_apkBtn[ BTN_RANDOM_AGIT ],     KD3DWnd::D3DWE_BUTTON_CLICK,    OnRandomAgit );
    GCWND_MSG_MAP( m_apkBtn[ BTN_CLOSE ],           KD3DWnd::D3DWE_BUTTON_CLICK,   OnClose );
}

void KGCAgitInviteVisit::OnRegister( void )
{
    // 내 아지트인가?
    if( SiKGCAgitGameManager()->GetAgitUID() != g_kGlobalValue.m_kUserInfo.dwUID )
        return;

    // 끄고 등록 UI
    this->ToggleRender( false );
    g_pkAgitOverlayUI->ShowAgitOption( true, KGCAgitOption::TYPE_REGISTER );
}

void KGCAgitInviteVisit::OnFriendInvite( void )
{
    this->ToggleRender( false );
    //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_AGIT_INVITE_REQ_DLG, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true);
    //g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_AGIT_INVITE_REQ_DLG );
    g_pkAgitOverlayUI->ShowInviteAgitReqDlg( true );
}

void KGCAgitInviteVisit::OnAgitList( void )
{
    // 끄고
    this->ToggleRender( false );
    
    // 전체 아지트 리스트 정보 요청
    KP2P::GetInstance()->Send_AgitPageListReq( 0, false, false, 1, 1);
}

void KGCAgitInviteVisit::OnRandomAgit( void )
{    
}

void KGCAgitInviteVisit::OnClose( void )
{
    g_pkAgitOverlayUI->ToggleRenderBrownieBtn( true );
    this->ToggleRender( false );
}