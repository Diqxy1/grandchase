#include "stdafx.h"
#include ".\KGCConnectionMsg.h"
//

//
#include "../Procedure.h"
//
//



IMPLEMENT_CLASSNAME( KGCConnectionMsg );
IMPLEMENT_WND_FACTORY( KGCConnectionMsg );
IMPLEMENT_WND_FACTORY_NAME( KGCConnectionMsg, "gc_connect_msg_box" );

KGCConnectionMsg::KGCConnectionMsg(void)
:m_pkOk( NULL )
,m_pkInfoTitle( NULL )
,m_pkInfoDesc( NULL )
,m_pkBar_Extend( NULL )
,m_pkInfoDesc_Extend( NULL )
,m_pkYellowCard( NULL )
,m_pkRedCard( NULL )
,m_pkBack( NULL )
{
    LINK_CONTROL( "btn_ok",             m_pkOk );
    LINK_CONTROL( "info_static",        m_pkInfoTitle );
    LINK_CONTROL( "info_desc",          m_pkInfoDesc );

    LINK_CONTROL( "bar_extend",         m_pkBar_Extend );
    LINK_CONTROL( "info_desc_extend",   m_pkInfoDesc_Extend );
    LINK_CONTROL( "yellow_card",        m_pkYellowCard );
    LINK_CONTROL( "red_card",           m_pkRedCard );
    LINK_CONTROL( "item_decs_back",     m_pkBack );
}

KGCConnectionMsg::~KGCConnectionMsg(void)
{
}

void KGCConnectionMsg::OnCreate( void )
{
    m_pkOk->InitState( true, true, this );

    m_pkInfoTitle->InitState( true );
    m_pkInfoTitle->SetAlign( DT_CENTER );
    
    m_pkInfoTitle->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );
    m_pkInfoTitle->SetFontColor( D3DCOLOR_XRGB( 255, 255, 0 ) );

    m_pkInfoDesc->InitState( true );
    m_pkInfoDesc->SetMultiLine( true );
    m_pkInfoDesc->SetLineSpace( 1.5f );

    m_pkBar_Extend->InitState( false );

    m_pkInfoDesc_Extend->InitState( false );
    m_pkInfoDesc_Extend->SetMultiLine( true );
    m_pkInfoDesc_Extend->SetLineSpace( 1.5f );

    m_pkYellowCard->InitState( false );
    m_pkRedCard->InitState( false );

    m_pkBack->InitState( false );
}

void KGCConnectionMsg::OnCreateComplete( void )
{
    if ( Result_Case == 0 )
    {
        // 단순 알림 창!
        m_pkInfoDesc->SetAlign( DT_LEFT );
        SetInfoText( Result_Connect_Msg );
    }
    else if ( Result_Case == 1 )
    {
        // 불량 유저창!
        m_pkInfoDesc->SetAlign( DT_CENTER );
        SetInfoText( Result_BadUser_Info );
    }

    Result_Case = INT_MAX;
}

void KGCConnectionMsg::ActionPerformed( const KActionEvent& event )
{
    if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
    {
        if( event.m_pWnd == m_pkOk && m_pkOk->IsRenderOn() )
        {
            g_KDSound.Play( "31" );
            SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
        }
    }
}

void KGCConnectionMsg::SetInfoText( const std::wstring& str )
{
    m_pkInfoTitle->SetText( g_pkStrLoader->GetString( STR_ID_CONNECTION_MSG_TITLE ) );
    m_pkInfoDesc->SetTextAutoMultiline( str );

    m_pkOk->ToggleRender( true );
    m_pkInfoTitle->ToggleRender( true );
    m_pkInfoDesc->ToggleRender( true );

    m_pkBar_Extend->ToggleRender( false );
    m_pkInfoDesc_Extend->ToggleRender( false );
    m_pkBack->ToggleRender( false );
    m_pkYellowCard->InitState( false );
    m_pkRedCard->InitState( false );
}

void KGCConnectionMsg::SetInfoText( KEVENT_BAD_USER_INFO_NOT kRecv )
{
    /*
    m_cLastGrade, m_cCurrentGrade 설명
    0 : 일반 유저
    1, 2 : 랭커 (랭킹등급이 높은 일반 유저 )
    3 : 운영자
    -1 : 경고
    -2 : 불량 유저
    -3 : 블럭 유저
    */
    switch ( static_cast<int>( kRecv.m_cCurrentGrade ) )
    {
    case 0:
    case 1:
    case 2:
        if ( kRecv.m_cLastGrade < 0 )
        {
            // 블럭 해제 됐다는 메시지 띄우쟈!
            m_pkInfoTitle->SetText( g_pkStrLoader->GetString( STR_ID_BADUSER_ALERT2 ) );   // [경고해제]
            m_pkInfoDesc->SetText( g_pkStrLoader->GetString( STR_ID_BADUSER_REASON5 ) );
            m_pkInfoDesc_Extend->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_BADUSER_DESC6 ) + g_pkStrLoader->GetString( STR_ID_BADUSER_DESC5 ) );
            m_pkYellowCard->InitState( false );
            m_pkRedCard->InitState( false );
        }
        break;
    case -1:
        {
            m_pkInfoTitle->SetText( g_pkStrLoader->GetString( STR_ID_BADUSER_ALERT0 ) );         // 1차경고
            m_pkInfoDesc_Extend->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_BADUSER_DESC0, "i", 1 ) + g_pkStrLoader->GetString( STR_ID_BADUSER_DESC1 ) +
                                                       g_pkStrLoader->GetString( STR_ID_BADUSER_DESC4 ) + g_pkStrLoader->GetString( STR_ID_BADUSER_DESC5 ) );
            m_pkYellowCard->InitState( true );
            m_pkRedCard->InitState( false );
        }
    	break;
    case -2:
        {
            m_pkInfoTitle->SetText( g_pkStrLoader->GetString( STR_ID_BADUSER_ALERT1 ) );         // 2차경고
            m_pkInfoDesc_Extend->SetTextAutoMultiline( g_pkStrLoader->GetReplacedString( STR_ID_BADUSER_DESC0, "i", 2 ) + g_pkStrLoader->GetString( STR_ID_BADUSER_DESC2 ) + 
                                                       g_pkStrLoader->GetString( STR_ID_BADUSER_DESC4 ) + g_pkStrLoader->GetString( STR_ID_BADUSER_DESC5 ) );
            m_pkYellowCard->InitState( false );
            m_pkRedCard->InitState( true );
        }
        break;
    case -3:
        {
            m_pkInfoTitle->SetText( g_pkStrLoader->GetString( STR_ID_CONNECTION_MSG_TITLE ) );   // 알림
            m_pkInfoDesc_Extend->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_BADUSER_DESC3 ) + g_pkStrLoader->GetString( STR_ID_BADUSER_DESC5 ) );
            m_pkYellowCard->InitState( false );
            m_pkRedCard->InitState( false );
        }
        break;
    }
    
    /*
    m_cCause 변수 설명
    0 : 해당 없음.
    1 : 욕설
    2 : 도배
    3 : 허위 신고
    4 : 해킹툴 사용
    5 : 운영자 사칭
    6 : 캐시 사기
    */
    switch ( static_cast<int>( kRecv.m_cCause ) )
    {
    case 1:
        m_pkInfoDesc->SetText( g_pkStrLoader->GetString( STR_ID_BADUSER_REASON0 ) );
        break;
    case 2:
        m_pkInfoDesc->SetText( g_pkStrLoader->GetString( STR_ID_BADUSER_REASON1 ) );
        break;
    case 3:
        m_pkInfoDesc->SetText( g_pkStrLoader->GetString( STR_ID_BADUSER_REASON2 ) );
        break;
    case 4:
    case 5:
    case 6:
        m_pkInfoDesc->SetText( g_pkStrLoader->GetString( STR_ID_BADUSER_REASON4 ) );
        break;
    }

    // 접속할 때 사용하는 UI모음!
    m_pkBar_Extend->ToggleRender( true );
    m_pkInfoDesc_Extend->ToggleRender( true );
    m_pkBack->ToggleRender( true );
}

void KGCConnectionMsg::OnDestroyComplete( void )
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}
