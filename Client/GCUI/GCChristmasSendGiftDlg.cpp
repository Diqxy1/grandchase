#include "stdafx.h"
#include "GCChristmasSendGiftDlg.h"
#include "GCChristmasFriendList.h"


IMPLEMENT_CLASSNAME( KGCChristmasSendGiftDlg );
IMPLEMENT_WND_FACTORY( KGCChristmasSendGiftDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCChristmasSendGiftDlg, "gc_send_christmas_gift" );

KGCChristmasSendGiftDlg::KGCChristmasSendGiftDlg( void )
{
    m_pkBtnFriendList = NULL;
    m_pkBtnSendGift = NULL;
    m_pkBtnClose = NULL;
    LINK_CONTROL( "btn_friend_list", m_pkBtnFriendList );
    LINK_CONTROL( "btn_send", m_pkBtnSendGift );
    LINK_CONTROL( "btn_close", m_pkBtnClose );

    m_pkStaticSendGift = NULL;
    m_pkStaticDesc = NULL;
    m_pkStaticEditDesc = NULL;
    LINK_CONTROL( "static_send_gift", m_pkStaticSendGift );
    LINK_CONTROL( "static_desc", m_pkStaticDesc );
    LINK_CONTROL( "static_edit_desc", m_pkStaticEditDesc );

    m_pkEditName = NULL;
    LINK_CONTROL( "edit_name", m_pkEditName );

    m_pkGiftItemImgWnd = NULL;
    LINK_CONTROL( "item_img_wnd", m_pkGiftItemImgWnd );

    m_pkFriendList = NULL;
    LINK_CONTROL( "christmas_friend_list", m_pkFriendList );

    m_dwReceiveUserUID = 0;
    m_strEditName.clear();

}

KGCChristmasSendGiftDlg::~KGCChristmasSendGiftDlg( void )
{
}


void KGCChristmasSendGiftDlg::OnCreate( void )
{
    // 버튼
    m_pkBtnFriendList->InitState( true, true, this );
    m_pkBtnSendGift->InitState( true, true, this );
    m_pkBtnClose->InitState( true, true, this );

    // static
    m_pkStaticSendGift->InitState( true, false, this );
    m_pkStaticSendGift->SetAlign( DT_CENTER );
    m_pkStaticSendGift->SetFontColor(D3DCOLOR_ARGB( 255, 255, 255, 255));
    m_pkStaticSendGift->SetFontOutline( true );

    m_pkStaticDesc->InitState( true, false, this );
    m_pkStaticDesc->SetAlign( DT_LEFT );
    m_pkStaticDesc->SetFontColor(D3DCOLOR_ARGB( 255, 255, 255, 255));
    m_pkStaticDesc->SetMultiLine( true );

    m_pkStaticEditDesc->InitState( true, false, this );
    m_pkStaticEditDesc->SetAlign( DT_LEFT );
    m_pkStaticEditDesc->SetFontColor(D3DCOLOR_ARGB( 256, 128, 128, 128));

    // edit
    m_pkEditName->InitState( true, false, this );

    m_pkGiftItemImgWnd->InitState( true );

    m_pkFriendList->InitState( false, true, this );

    // 이벤트 기본정보 세팅
    InitEventInfo();

}



void KGCChristmasSendGiftDlg::ActionPerformed( const KActionEvent& event )
{
    _GCWND_MSG_MAP( m_pkBtnSendGift,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickSendGift );
    _GCWND_MSG_MAP( m_pkBtnFriendList,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickFriendList );
    _GCWND_MSG_MAP( m_pkBtnClose,          KD3DWnd::D3DWE_BUTTON_CLICK, OnClickClose );

}


void KGCChristmasSendGiftDlg::InitEventInfo()
{
    m_pkStaticSendGift->SetText( g_pkStrLoader->GetString( STR_ID_SEND_GIFT ) );
    
#if defined( USE_POST_LETTER_SYSTEM )
    m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_SEND_PRESEND_DESC ) );
#else
    m_pkStaticDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_TYPE_YOUR_FRIENDS_NAME ) );
#endif

    m_pkEditName->SetText( g_pkStrLoader->GetString( STR_ID_TYPE_NICKNAME ).c_str() );

}


void KGCChristmasSendGiftDlg::OnClickSendGift()
{
    g_KDSound.Play( "31" );

    m_strEditName = m_pkEditName->GetText();

    // 이름의 길이가 0이면 무시
    if( m_strEditName.size() == 0 )
        return;

    // 디폴트 이름이면 무시 (닉네임 입력)
    if( m_strEditName.compare( g_pkStrLoader->GetString( STR_ID_TYPE_NICKNAME ).c_str() ) == 0 )
        return;

    PLAYER* pPlayer = &g_MyD3D->m_TempPlayer;

    // 자기한테 보내는거면 안됨. 이름검사
    if ( m_strEditName.compare( pPlayer->GetPlayerName() ) ) {
        // 플레이어 이름으로 UID를 찾아보자
        KP2P::GetInstance()->Send_CharismasEventConnectCheckReq( m_strEditName );
    } else {
        // 자기 자신이면 종료
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_YOU_CANNOT_SEND_THE_GIFT_TO_YOURSELF ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }
}


void KGCChristmasSendGiftDlg::SendGift( DWORD dwReceiveUserUID )
{
    PLAYER* pPlayer = &g_MyD3D->m_TempPlayer;

    // 만약 자기 UID라면 자신한테 보내는 것이므로 종료
    if( dwReceiveUserUID == pPlayer->m_kUserInfo.dwUID ){
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_YOU_CANNOT_SEND_THE_GIFT_TO_YOURSELF ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
        return;
    }

    KEVENT_CHARISMAS_EVENT_PRESENT_REQ kPacket_;

    kPacket_.m_dwReceiveUserUID = dwReceiveUserUID;
    kPacket_.m_kDropItemInfo = m_kDropItemInfo;



    KP2P::GetInstance()->Send_CharismasEventPresentReq( kPacket_ );

    m_pkEditName->SetText( g_pkStrLoader->GetString( STR_ID_TYPE_NICKNAME ).c_str() );
    this->ToggleRender( false );
}

void KGCChristmasSendGiftDlg::OnClickFriendList()
{
#if defined ( USE_POST_LETTER_SYSTEM )
    m_pkFriendList->ToggleFriendList();
#else
    m_pkFriendList->ToggleRender( !(m_pkFriendList->IsRenderOn()) );
#endif
}

void KGCChristmasSendGiftDlg::SetSendGiftItemInfo( GCITEMID dwItemID, KDropItemInfo kDropItemInfo_ )
{

    m_kDropItemInfo = kDropItemInfo_;

    // 아이템 그려주기
    m_pkGiftItemImgWnd->SetItemInfo( dwItemID / 10 );
    m_pkGiftItemImgWnd->ShowItem (true );
}

void KGCChristmasSendGiftDlg::GetNameFromFriendList( wstring& strEditName )
{
    m_pkEditName->SetText( strEditName.c_str() );
    m_strEditName = strEditName;
    m_pkFriendList->ToggleRender( false );
}

void KGCChristmasSendGiftDlg::OnClickClose()
{
    this->ToggleRender( false );
}
