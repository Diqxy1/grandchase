#include "stdafx.h"
#include "GCRoomUserInfoPopupS6.h"
#include "GCRoomUserInfoViewS6.h"

IMPLEMENT_CLASSNAME( KGCRoomUserInfoPopupS6 );
IMPLEMENT_WND_FACTORY( KGCRoomUserInfoPopupS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCRoomUserInfoPopupS6, "gc_room_user_info_popup_s6" );

KGCRoomUserInfoPopupS6::KGCRoomUserInfoPopupS6( void )
: m_pkBtnClose( NULL )
, m_pkBntUserInfo( NULL )
, m_pkBntUserStatus( NULL )
, m_pkInfoPanel( NULL )
, m_pkStatusPanel( NULL )
, m_pkCharWnd( NULL ) 
, m_pkBtnAddFriend( NULL ) 
, m_pkBtnBlockFriend( NULL ) 
, m_pkBtnBadGuy( NULL )
, m_nPlayerIndex( 0 )
{
    LINK_CONTROL( "btn_close", m_pkBtnClose );
    LINK_CONTROL( "user_info_button", m_pkBntUserInfo );
    LINK_CONTROL( "user_status_button", m_pkBntUserStatus );

    LINK_CONTROL( "user_info_view", m_pkInfoPanel );
    LINK_CONTROL( "user_status_info_view", m_pkStatusPanel );

    LINK_CONTROL( "character_window", m_pkCharWnd );

    LINK_CONTROL( "btn_add_friend", m_pkBtnAddFriend );
    LINK_CONTROL( "btn_block_friend", m_pkBtnBlockFriend );
    LINK_CONTROL( "btn_badguy", m_pkBtnBadGuy );
}

KGCRoomUserInfoPopupS6::~KGCRoomUserInfoPopupS6( void )
{
}

void KGCRoomUserInfoPopupS6::ActionPerformed( const KActionEvent& event )
{
	GCWND_MSG_MAP( m_pkBtnClose, KD3DWnd::D3DWE_BUTTON_CLICK, OnClose );
	GCWND_MSG_MAP( m_pkBntUserInfo, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickUserInfo );
	GCWND_MSG_MAP( m_pkBntUserStatus, KD3DWnd::D3DWE_BUTTON_CLICK, OnClickUserStatus );

    GCWND_MSG_MAP( m_pkBtnAddFriend, KD3DWnd::D3DWE_BUTTON_CLICK, OnAddFriend );
    GCWND_MSG_MAP( m_pkBtnBlockFriend, KD3DWnd::D3DWE_BUTTON_CLICK, OnBlockFriend );
    GCWND_MSG_MAP( m_pkBtnBadGuy, KD3DWnd::D3DWE_BUTTON_CLICK, OnBadGuy );
}

void KGCRoomUserInfoPopupS6::OnCreate( void )
{
    m_pkBtnClose->InitState( true, true, this );
    m_pkBtnClose->SetHotKey( DIK_ESCAPE );
    m_pkBntUserInfo->InitState( true, true, this );
    m_pkBntUserStatus->InitState( true, true, this );
    m_pkCharWnd->InitState(true,true);

    m_pkBtnAddFriend->InitState( true, true, this );
    m_pkBtnBlockFriend->InitState( true, true, this );
    m_pkBtnBadGuy->InitState( true, true, this );

    m_pkStatusPanel->SetRoomUserInfo( true );

    SetInfoButtonState(true, false);
}

void KGCRoomUserInfoPopupS6::OnClose()
{
    m_Player.ReleasePlayerResource();
    g_KDSound.Play( "31" );
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCRoomUserInfoPopupS6::OnClickUserInfo()
{
    SetInfoButtonState( true, false );
}

void KGCRoomUserInfoPopupS6::OnClickUserStatus()
{
    SetInfoButtonState( false, true );
}

void KGCRoomUserInfoPopupS6::SetInfoButtonState(bool bUserInfo_, bool bStatusInfo_)
{
    m_pkBntUserInfo->Activate(bUserInfo_);
    m_pkInfoPanel->ToggleRender(bUserInfo_);

    m_pkBntUserStatus->Activate(bStatusInfo_);
    m_pkStatusPanel->ToggleRender(bStatusInfo_);
}


bool KGCRoomUserInfoPopupS6::InitDialog( IN const KDialogInfo& kInfo_ )
{
    m_nPlayerIndex = static_cast<int>(kInfo_.m_lParam);
    UpdateData();

    return true;
}

bool KGCRoomUserInfoPopupS6::ValidPlayerCheck()
{
    if ( m_nPlayerIndex < 0 || m_nPlayerIndex >= MAX_PLAYER_NUM ) 
        return false;

    if ( !g_MyD3D->MyPlayer[m_nPlayerIndex]->m_kUserInfo.bLive )
        return false;

    if ( g_MyD3D->MyPlayer[m_nPlayerIndex]->m_kUserInfo.cCharIndex == -1 )
        return false;

    return true;
}

void KGCRoomUserInfoPopupS6::UpdateData()
{
    if( !ValidPlayerCheck() )
    {
        OnClose();
        return;
    }

    m_Player.m_kUserInfo = g_MyD3D->MyPlayer[m_nPlayerIndex]->m_kUserInfo;

    m_Player.SetMyPlayer(-1, true);
    m_Player.ReleasePlayerResource();
    m_Player.RemovePlayerObject();

    m_Player.UpdatePlayerResourceClean();

    UpdatePlayerData();
}

void KGCRoomUserInfoPopupS6::UpdatePlayerData()
{
    m_pkStatusPanel->ResetInfo();
    m_pkStatusPanel->SetStartUserInfo( g_MyD3D->MyPlayer[m_nPlayerIndex] );
    m_pkStatusPanel->SetChangeUserInfo( g_MyD3D->MyPlayer[m_nPlayerIndex] );
    m_pkStatusPanel->SetText();
    
    m_pkInfoPanel->SetUserInfo( g_MyD3D->MyPlayer[m_nPlayerIndex] );
    m_pkCharWnd->SetPreviewWindow( &m_Player );

    m_strNickName = g_MyD3D->MyPlayer[m_nPlayerIndex]->GetStrUserName();
    UpdateMyChar( g_MyD3D->MyPlayer[m_nPlayerIndex] );
}

void KGCRoomUserInfoPopupS6::UpdateMyChar( PLAYER* pPlayer )
{
    bool bMyPlayer = ( g_MyD3D->Get_MyPlayer() == pPlayer->m_iPlayerIndex );

    m_pkBtnAddFriend->ToggleRender( !bMyPlayer );
    m_pkBtnBlockFriend->ToggleRender( !bMyPlayer );

#if !defined( DISABLE_BADGUY_REPORT )
    m_pkBtnBadGuy->ToggleRender( !bMyPlayer );    
#else
    m_pkBtnBadGuy->ToggleRender( false );    
#endif
}

void KGCRoomUserInfoPopupS6::OnAddFriend()
{    
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NATEON_ADD_BUDDY, m_strNickName
        , g_pkStrLoader->GetString( STR_ID_NATEON_ADD_BUDDY_DEFMSG )
        , KGCUIScene::GC_MBOX_USE_NATEON_ADD_BUDDY, 0, 0, false, true );
}

void KGCRoomUserInfoPopupS6::OnBlockFriend()
{
    if( SiKGCCoupleSystem()->GetNickName_of_my_honey() == m_strNickName )
    {
        g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM40 )
            , L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
    }
    else
    {
        KBuddy FData;
        if( SiKFriendShip()->GetFriendBuddyByNickName( m_strNickName, FData ) )
        {					
            const int iGroupUID = SiKFriendShip()->GetFriendBuddyGroupID( FData.GetUID() );
            SiKFriendShip()->BlockMyFriendReq( iGroupUID, FData.GetUID() );
        }
        else
        {
            SiKFriendShip()->BlockNoFriendReq( m_strNickName );
        }
        g_pkUIScene->m_pkNateOn->GetMultiMenu()->BlockFriendAck();
    }
}

void KGCRoomUserInfoPopupS6::OnBadGuy()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_FAIRPLAY_REPORT, g_kGlobalValue.m_kUserInfo.strNickName, m_strNickName );
}