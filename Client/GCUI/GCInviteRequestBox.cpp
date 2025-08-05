#include "stdafx.h"
#include ".\GCInviteRequestBox.h"
#include "GCInviteUserList.h"
#include "GCInviteSquareList.h"

//
//

IMPLEMENT_CLASSNAME( KGCInviteRequestBox );
IMPLEMENT_WND_FACTORY( KGCInviteRequestBox );
IMPLEMENT_WND_FACTORY_NAME( KGCInviteRequestBox, "gc_invite_request_box" );

KGCInviteRequestBox::KGCInviteRequestBox(void)
: m_pkCloseBtn(NULL)
, m_pkUserList(NULL)
, m_pkSquareList(NULL)
, m_pkComboPosition(NULL)
{
    LINK_CONTROL( "close_button",   m_pkCloseBtn );
    LINK_CONTROL( "invite_user_list", m_pkUserList );
    LINK_CONTROL( "invite_square_list", m_pkSquareList );
    LINK_CONTROL( "combo_invite_position", m_pkComboPosition );
}

KGCInviteRequestBox::~KGCInviteRequestBox(void)
{

}

void KGCInviteRequestBox::OnCreate( void )
{
    m_pkCloseBtn->InitState( true, true, this );
    m_pkCloseBtn->SetHotKey( DIK_ESCAPE );
    m_pkUserList->InitState( true );
    m_pkSquareList->InitState( true );

    m_pkComboPosition->InitState( true, true, this );

    if( SiKGCInviteManager()->GetInviteType() == KGCInviteManager::INVITE_TYPE_INVALID )
    {
        SiKGCInviteManager()->SetInviteType( KGCInviteManager::INVITE_TYPE_CHANNEL );
    }
}

void KGCInviteRequestBox::ActionPerformed( const KActionEvent& event )
{
     GCWND_MSG_MAP( m_pkComboPosition, D3DWE_COMBO_SELCHANGE, OnComboPosition_SelChange );
     GCWND_MSG_MAP( m_pkCloseBtn, D3DWE_BUTTON_CLICK, OnClose );
}

void KGCInviteRequestBox::SetModeButton( KGCInviteManager::INVITE_TYPE eType )
{
    m_pkSquareList->ToggleRender( false );
    m_pkSquareList->InitSquareList();
    m_pkUserList->ToggleRender( false );
    m_pkUserList->InitUserList();

    switch( eType )
    {
    case KGCInviteManager::INVITE_TYPE_CHANNEL:
        m_pkSquareList->ToggleRender( false );
        m_pkUserList->ToggleRender( true );
        m_pkUserList->SetWindowPosDirect( m_pkSquareList->GetFixedWindowLocalPos() );

        KP2P::GetInstance()->Send_UserListReq( 0 );
        m_pkUserList->SetTitle( g_pkStrLoader->GetString( STR_ID_WAITING_FOR_USER_LIST ) );
        break;

	case KGCInviteManager::INVITE_TYPE_FRIENDS:
        m_pkSquareList->ToggleRender( false );
		m_pkUserList->ToggleRender( true );
		m_pkUserList->SetWindowPosDirect( m_pkSquareList->GetFixedWindowLocalPos() );

		KP2P::GetInstance()->Send_FriendInviteListReq();

		//친구 목록을 얻는 중입니다. //추가 필요.
		m_pkUserList->SetTitle( g_pkStrLoader->GetString( STR_ID_WAITING_FOR_USER_LIST ) );
		break;

	case KGCInviteManager::INVITE_TYPE_GUILD:
		m_pkSquareList->ToggleRender( false );
		m_pkUserList->ToggleRender( true );
		m_pkUserList->SetWindowPosDirect( m_pkSquareList->GetFixedWindowLocalPos() );

		m_pkUserList->SetTitle( g_pkStrLoader->GetString( STR_ID_WAITING_FOR_USER_LIST ) );
		break;

    case KGCInviteManager::INVITE_TYPE_SQUARE:
        m_pkSquareList->ToggleRender( true );
        m_pkUserList->ToggleRender( true );
        D3DXVECTOR2 vPos = m_pkSquareList->GetFixedWindowLocalPos();
        vPos.x += (float)m_pkSquareList->GetWidth();
        vPos.x += 6.0f;
        m_pkUserList->SetWindowPosDirect( vPos );

        KP2P::GetInstance()->Send_SquareListReq( g_kGlobalValue.m_kGuildUserInfo.m_dwGuildUID );
        m_pkUserList->SetTitle( g_pkStrLoader->GetString( STR_ID_CHOICE_SQUARE ) );
        m_pkSquareList->SetTitle( g_pkStrLoader->GetString( STR_ID_WAITING_FOR_SQUARE_LIST ) );
        break;	
    }
}

void KGCInviteRequestBox::OnChannelMode()
{
    SetModeButton( KGCInviteManager::INVITE_TYPE_CHANNEL );
    SiKGCInviteManager()->SetInviteType( KGCInviteManager::INVITE_TYPE_CHANNEL );
}

void KGCInviteRequestBox::OnSquareMode()
{
    SetModeButton( KGCInviteManager::INVITE_TYPE_SQUARE );
    SiKGCInviteManager()->SetInviteType( KGCInviteManager::INVITE_TYPE_SQUARE );
}

void KGCInviteRequestBox::OnFriendMode()
{
	std::vector< DWORD > vecOnlineFriendList;
	SiKFriendShip()->GetOnlienFriendList(GCNateOn::FRIEND_MODE, vecOnlineFriendList);
	KP2P::GetInstance()->Send_OnlineFriendList(vecOnlineFriendList);

	SetModeButton( KGCInviteManager::INVITE_TYPE_FRIENDS );
	SiKGCInviteManager()->SetInviteType( KGCInviteManager::INVITE_TYPE_FRIENDS );
}

void KGCInviteRequestBox::OnGuildMode()
{
	SetModeButton( KGCInviteManager::INVITE_TYPE_GUILD );
	SiKGCInviteManager()->SetInviteType( KGCInviteManager::INVITE_TYPE_GUILD );		
}

void KGCInviteRequestBox::SetGuildMemberList()
{
	std::vector< DWORD > vecOnlineFriendList;
	std::vector<KInviteUserInfo> vecInviteUserInfo;
	SiKFriendShip()->GetOnlienFriendList(GCNateOn::GUILD_MODE, vecOnlineFriendList);

	for( int i = 0; i < (int)vecOnlineFriendList.size(); i++ )
	{
		std::map<DWORD, KNGuildUserInfo>::iterator mit = g_kGlobalValue.m_mapNGuildUserInfo.find(vecOnlineFriendList[i]);

		if( mit != g_kGlobalValue.m_mapNGuildUserInfo.end() )
		{
			KInviteUserInfo vecTemp;
			vecTemp.m_dwUserUID		 = mit->second.m_dwUserUID;
			vecTemp.m_strNickName	 = mit->second.m_strNickName;
			vecTemp.m_cBestCharGrade = mit->second.m_ucCharacterGrade;
			vecTemp.m_cGrade		 = mit->second.m_ucCharacterGrade;

			vecInviteUserInfo.push_back(vecTemp);
		}
	}	
	SiKGCInviteManager()->SetInviteFriendUserList( vecInviteUserInfo, GCNateOn::GUILD_MODE, false);
	UpdateFriendUserList();
	SiKGCInviteManager()->SetCurrentPage(0);
	SetUserListTitle( g_pkStrLoader->GetString( STR_ID_INVITING ) );
}

void KGCInviteRequestBox::OnClose()
{	
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_CLOSE ) );
}

void KGCInviteRequestBox::OnDestroyComplete()
{
    SpeakToActionListener( KActionEvent( this, EWNDMESSAGE_DESTROYED ) );
}

void KGCInviteRequestBox::UpdateSquareInfo()
{
    m_pkSquareList->SetSquareList();
    m_pkSquareList->SetTitle( g_pkStrLoader->GetString( STR_ID_INVITING ) );
}

void KGCInviteRequestBox::OnCreateComplete()
{
	OnInitInviteType();

	if( SiKGCRoomManager()->IsChannelRoomList() )
	{
		SetGuildMemberList();
	}
}

void KGCInviteRequestBox::FrameMoveInEnabledState()
{
    //m_dwListReqTime++;
    //if( m_dwListReqTime >= GC_FPS_LIMIT*10 )
    //{
    //    UpdateList();
    //}
}

void KGCInviteRequestBox::UpdateList()
{
    //m_dwListReqTime = 0;
}

void KGCInviteRequestBox::SetUserListTitle( const std::wstring& strTitle )
{
    m_pkUserList->SetTitle( strTitle );
}

void KGCInviteRequestBox::UpdateFriendUserList()
{
	m_pkUserList->SetFriendUserList();
}

void KGCInviteRequestBox::UpdateUserList()
{
    m_pkUserList->SetUserList();
}

void KGCInviteRequestBox::SetSquareListTitle( const std::wstring& strTitle )
{
    m_pkSquareList->SetTitle( strTitle );
}

void KGCInviteRequestBox::OnComboPosition_SelChange()
{
    std::wstring strText = m_pkComboPosition->GetCurStaticText();

    if( strText == g_pkStrLoader->GetString( STR_ID_MESSENGER_USER_LOCATION8 ) )
    {
        OnChannelMode();
    }
    else if( strText == g_pkStrLoader->GetString( STR_ID_FRIEND ) )
    {
        OnFriendMode();
    }
    else if( strText == g_pkStrLoader->GetString( STR_ID_SQUARE ) )
    {
        OnSquareMode();
    }
    else if( strText == g_pkStrLoader->GetString( STR_ID_GUILD ) )
    {
		OnGuildMode();
    }
}

void KGCInviteRequestBox::OnInitInviteType()
{
    m_pkComboPosition->DeleteAllItem();
    if( SiKGCRoomManager()->IsChannelRoomList() == false )
    {
        m_pkComboPosition->AddString( g_pkStrLoader->GetString( STR_ID_MESSENGER_USER_LOCATION8 ), DT_CENTER );
        m_pkComboPosition->AddString( g_pkStrLoader->GetString( STR_ID_FRIEND ), DT_CENTER );
        m_pkComboPosition->AddString( g_pkStrLoader->GetString( STR_ID_SQUARE ), DT_CENTER );

    }
	else
	{
		m_pkComboPosition->AddString( g_pkStrLoader->GetString( STR_ID_GUILD ), DT_CENTER );
	}
    m_pkComboPosition->SetCurSel( 0 );
}

bool KGCInviteRequestBox::InitDialog( IN const KD3DDialog::KDialogInfo& kInfo_ )
{
    OnInitInviteType();
    UpdateList();
    return true;
}