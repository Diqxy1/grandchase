#include "stdafx.h"
#include ".\kgceventbox.h"

//

//
//



IMPLEMENT_CLASSNAME( KGCEventBox );
IMPLEMENT_WND_FACTORY( KGCEventBox );
IMPLEMENT_WND_FACTORY_NAME( KGCEventBox, "gc_event_box" );

KGCEventBox::KGCEventBox(void)
{
    m_pkBackGround                = NULL; 
    m_pkStaticTitle                = NULL;
    m_pkStaticRecommend            = NULL;
    m_pkStaticMessage            = NULL;

    m_pkBtnOk                    = NULL;
    m_pkBtnCancel                = NULL;
    m_pkBackRecommend            = NULL;
    m_pkBar                        = NULL;
    m_pkBar2                    = NULL;

    m_pkEditNick                = NULL;
    m_pkEditRecommend            = NULL;

    m_pkCheckBoxNoRecommend        = NULL;

    LINK_CONTROL( "btn_ok",                    m_pkBtnOk );
    LINK_CONTROL( "btn_cancel",                m_pkBtnCancel );    
    LINK_CONTROL( "edit_recommend",         m_pkEditRecommend);
    LINK_CONTROL( "static_title",           m_pkStaticTitle );
    LINK_CONTROL( "static_message",            m_pkStaticMessage );
    LINK_CONTROL( "static_recommend",        m_pkStaticRecommend );
    LINK_CONTROL( "static_no_recommend",    m_pkStaticNoRecommend );
    LINK_CONTROL( "checkbox_no_recommend",  m_pkCheckBoxNoRecommend );
    LINK_CONTROL( "background",             m_pkBackGround );
    LINK_CONTROL( "back_recommend",         m_pkBackRecommend );
    LINK_CONTROL( "bar",                    m_pkBar );
    LINK_CONTROL( "bar2",                    m_pkBar2 );


    m_dwCurrentBoxSetting = -1;
}

KGCEventBox::~KGCEventBox(void)
{
}

void KGCEventBox::FrameMoveInEnabledState()
{
    if( m_dwUse != m_dwCurrentBoxSetting )
    {
        SetBox( m_dwUse );
    }
}

void KGCEventBox::OnCreate( void )
{
    SetBox( m_dwUse );

    m_pkBtnOk->SetHotKey( DIK_RETURN );
    m_pkBtnCancel->SetHotKey( DIK_ESCAPE );
}

void KGCEventBox::OnCreateComplete( void )
{
    m_pkEditRecommend->SetFocus();
    SpeakToActionListener( KActionEvent( this, MBR_CREATED ) );
}

void KGCEventBox::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, MBR_DESTROYED ) );
}

void KGCEventBox::SetBox( DWORD dwUse_ )
{
    switch( m_dwUse )
    {
    case g_pkUIScene->GC_MBOX_USE_EVENT_MSG:
        {
            SetEventMessageBox();
            break;
        }        
    case g_pkUIScene->GC_MBOX_USE_EVENT_RECOMMAND:
        {
            SetEventRecommandBox();
            break;
        }
    default:
        SetEventMessageBox();
        break;
    }
}

void KGCEventBox::SetTitle( std::wstring strTitle_ )
{
    m_pkStaticTitle->SetText( strTitle_ );
}

void KGCEventBox::SetMessage( std::wstring strMsg_ )
{
    m_pkStaticMessage->SetText( strMsg_ );
}

void KGCEventBox::SetRecommandMsg( std::wstring strMsg_ )
{
    m_pkStaticRecommend->SetText( strMsg_ );
}

void KGCEventBox::SetChackBoxMsg( std::wstring strMsg_ )
{
    m_pkStaticNoRecommend->SetText( strMsg_ );
}

void KGCEventBox::ActionPerformed( const KActionEvent& event )
{
    //이벤트를 여러번 먹는것을 막기위한 코드임
    if( KD3DWnd::D3DWS_ENABLED != GetWndState() )
        return;

    switch ( event.m_dwCode )
    {
    case KD3DWnd::D3DWE_CURSOR_ENTER:
        {
            g_KDSound.Play( "30" );
            break;
        }
    case KD3DWnd::D3DWE_BUTTON_DOWN:
        {
            g_KDSound.Play( "31" );
            break;
        }
    }

    GCWND_MSG_MAP( m_pkEditRecommend,       KD3DWnd::D3DWE_EDIT_RETURN,     OnConfirmRecommendNickName );
    GCWND_MSG_MAP( m_pkEditRecommend,       KD3DWnd::D3DWE_BUTTON_CLICK,    OnClickEditRecommend );
    GCWND_MSG_MAP( m_pkBtnOk,               KD3DWnd::D3DWE_BUTTON_CLICK,    OnConfirmRecommendNickName );
    GCWND_MSG_MAP( m_pkBtnCancel,           KD3DWnd::D3DWE_BUTTON_CLICK,    OnCancel );
    GCWND_MSG_MAP( m_pkCheckBoxNoRecommend, KD3DWnd::D3DWE_BUTTON_CLICK,    OnCheck );
}

void KGCEventBox::OnClickEditRecommend()
{
    m_pkEditRecommend->SetFocus();
    m_pkCheckBoxNoRecommend->SetCheck( false );
}

void KGCEventBox::OnCheck()
{
    if( m_pkCheckBoxNoRecommend->GetCheck() == false )
    {
        m_pkEditRecommend->SetText( L"" );
        m_pkEditRecommend->Activate( false );
    }
    m_pkCheckBoxNoRecommend->SetCheck( !m_pkCheckBoxNoRecommend->GetCheck() );
}

void KGCEventBox::OnCancel()
{
    SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_RETURN_CANCEL ) );
}

void KGCEventBox::OnConfirm()
{
    g_KDSound.Play( "31" );

    switch( GetUse() ) 
    {
    case KGCUIScene::GC_MBOX_USE_EVENT_MSG:
        {
            SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_USE_EVENT_MSG ) );
            break;
        }
    case KGCUIScene::GC_MBOX_USE_EVENT_RECOMMAND:
        {
            if( !m_pkCheckBoxNoRecommend->GetCheck() )
            {
                std::wstring strRecommendedUser = m_pkEditRecommend->GetText();
                if( strRecommendedUser.size() != 0 )
                    KP2P::GetInstance()->Send_GCPointRecommendReq( strRecommendedUser );
                else
                    return;
            }
            else
            {
                KP2P::GetInstance()->Send_GCPointRecommendReq( L"" );
            }
            SpeakToActionListener( KActionEvent( this, KGCUIScene::GC_MBOX_USE_EVENT_RECOMMAND ) );
            break;
        }
    }
}

void KGCEventBox::OnConfirmRecommendNickName()
{
    OnConfirm();
}

void KGCEventBox::SetEventMessageBox()
{
    //이벤트 메시지만 보인다면 이렇게 합니다.
    m_pkBackGround->InitState( true, false, NULL );

    m_pkStaticTitle->InitState( true, false, NULL );
    m_pkStaticTitle->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );
    m_pkStaticMessage->InitState( true, false, NULL );
    m_pkStaticMessage->SetMultiLine( true );
    m_pkStaticMessage->SetLineSpace( 1.4f );

    m_pkBtnOk->InitState( true, true, this );
    m_pkBtnCancel->InitState( false, false, NULL  );
    m_pkBar->InitState( true, false, NULL );
    m_pkBar2->InitState( false, false, NULL );

    // 추천관련 UI는 가립니다.
    m_pkStaticRecommend->InitState( false, false, NULL );
    m_pkStaticNoRecommend->InitState( false, false, NULL );
    m_pkEditRecommend->InitState( false, false, NULL );
    m_pkBackRecommend->InitState( false, false, NULL );
    m_pkCheckBoxNoRecommend->InitState( false, false, NULL );

    //위치를 잡아줍니다.( 가려진 만큼 위로 땡겨주기 )
    m_pkBackGround->SetSize( 260, 180 );
    m_pkBar->SetFixedWindowLocalPos( D3DXVECTOR2( 13, 143 ) );
    m_pkBar2->SetFixedWindowLocalPos( D3DXVECTOR2( 123, 149) );
    m_pkBtnOk->SetFixedWindowLocalPos( D3DXVECTOR2( 110, 148 ) );
    m_pkBtnCancel->SetFixedWindowLocalPos( D3DXVECTOR2(139, 148) );

    m_dwCurrentBoxSetting = m_dwUse;
}

void KGCEventBox::SetEventRecommandBox()
{
    // 추천자를 받는 다면 UI 가 모두 보입니다.
    m_pkBackGround->InitState( true, false, NULL );

    m_pkStaticTitle->InitState( true, false, NULL );
    m_pkStaticTitle->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );
    m_pkStaticMessage->InitState( true, false, NULL );
    m_pkStaticMessage->SetMultiLine( true );
    m_pkStaticMessage->SetLineSpace( 1.4f );

    m_pkBtnOk->InitState( true, true, this );
    m_pkBtnCancel->InitState( false, false, NULL );
    m_pkBar->InitState( true, false, NULL );
    m_pkBar2->InitState( false, false, NULL );

    m_pkStaticRecommend->InitState( true, false, NULL );
    m_pkStaticNoRecommend->InitState( true, false, NULL );
    m_pkEditRecommend->InitState( true, true, this );
    m_pkBackRecommend->InitState( true, false, NULL );
    m_pkCheckBoxNoRecommend->InitState( true, true, this );
    m_pkCheckBoxNoRecommend->SetCheck( false );

    m_pkStaticRecommend->SetText( g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT10 ) );
    m_pkStaticNoRecommend->SetText( g_pkStrLoader->GetString( STR_ID_NEWTERM_EVENT11 ) );

    //위치를 잡아줍니다.
    m_pkBackGround->SetSize( 260, 250 );
    m_pkBar->SetFixedWindowLocalPos( D3DXVECTOR2( 13, 218 ) );
    m_pkBar2->SetFixedWindowLocalPos( D3DXVECTOR2( 123, 224) );
    m_pkBtnOk->SetFixedWindowLocalPos( D3DXVECTOR2( 110, 222 ) );
    m_pkBtnCancel->SetFixedWindowLocalPos( D3DXVECTOR2(139, 223) );

    m_dwCurrentBoxSetting = m_dwUse;
}

bool KGCEventBox::InitDialog( IN const KDialogInfo& kInfo_ )
{
    this->SetUse( kInfo_.m_nMsgBoxUse );
    this->SetTitle( kInfo_.m_strText1 );
    this->SetMessage( kInfo_.m_strText2 );
    return true;
}