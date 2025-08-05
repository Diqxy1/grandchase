#include "stdafx.h"
#include "../Square/GCSquarePeople.h"
#include "GCSquareUserList.h"
#include "GCSquareUserList_Element.h"


#include "GCMultiMenu.h"
#include "../Square/GCSquare.h"

#include "../gcui/GCNateOnMultiMenu.h"
#include "../BuddySystem/KFriendShip.h"

#include "GCCoupleSystem.h"
#include "KGCPartyManager.h"


IMPLEMENT_CLASSNAME( KGCSquareUserList );
IMPLEMENT_WND_FACTORY( KGCSquareUserList );
IMPLEMENT_WND_FACTORY_NAME( KGCSquareUserList, "gc_square_userlist" );

KGCSquareUserList::KGCSquareUserList(void)
:m_pkScrollbar(NULL)
,m_pkScrollBG(NULL)
,m_pkCloseBtn(NULL)
,m_pkBoxName(NULL)
,m_pkPopupBox(NULL)
,m_pkBackGround(NULL)
,m_iTopIndex(0)
,m_vecUserInfo(NULL)
,m_bRenderUserList(false)
,m_bIsLClickUserList(false)
,m_dwSelectedUID(0)
{
    for ( int i = 0 ; i < SQUARE_USERINFO_ELEMENT_NUM ; ++i )
        m_pkElement[i] = NULL;

    m_strClickedUserName = L"";

    char str[MAX_PATH];
    for ( int i = 0; i < SQUARE_USERINFO_ELEMENT_NUM ; ++i )
    {
        sprintf( str, "userlist%d", i+1 );
        LINK_CONTROL( str, m_pkElement[i] );
    }

    LINK_CONTROL( "scroll",                 m_pkScrollbar );
    LINK_CONTROL( "scroll_background_bar",  m_pkScrollBG );
    LINK_CONTROL( "close_button",           m_pkCloseBtn );
    LINK_CONTROL( "boxname",                m_pkBoxName );
    LINK_CONTROL( "popupbox",               m_pkPopupBox );
    LINK_CONTROL( "back",                   m_pkBackGround );
}

KGCSquareUserList::~KGCSquareUserList(void)
{
}

void KGCSquareUserList::OnCreate( void )
{
    for ( int i = 0 ; i < SQUARE_USERINFO_ELEMENT_NUM ; i++ )
        m_pkElement[i]->InitState( true, true, this );

    m_pkCloseBtn->InitState( true, true, this );
    m_pkBoxName->InitState( true );
    m_pkBoxName->SetText( g_pkStrLoader->GetString(STR_ID_SQUARE_USERLIST_BOXNAME) );
    m_pkBoxName->SetOffset( D3DXVECTOR2( 15.0f, 5.0f ) );
    m_pkBoxName->SetFontSize( SiKGCMultipleLanguages()->GetMiddleFontSize() );
    m_pkBackGround->InitState( true );
    m_pkPopupBox->InitState( false, true, this );

    m_pkScrollBG->InitState( true );
    m_pkScrollbar->InitState( true, false, this );
    m_pkScrollbar->SetScrollPageSize( SQUARE_USERINFO_ELEMENT_NUM );
    m_pkScrollbar->SetScrollPos( m_iTopIndex );
    m_pkScrollbar->SetScrollRangeMin( 0 );
    m_pkScrollbar->SetScrollRangeMax( SQUARE_USERINFO_ELEMENT_NUM );
}
 
void KGCSquareUserList::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KGCMultiMenu::K3DWE_MENU_CLICKED &&
		event.m_pWnd == m_pkPopupBox )
	{
		int AddFriend = 1;
		int FairplayReport = 2;
		int BlockFriend = 3;
		int CouplePropose = 4;
		int CoupleWnd = 5;
		int CoupleRingChange = 6;
		int CoupleGoodbye = 7;
        int Recommend = 8;
        int InviteParty = 9;

		std::wstringstream strTemp;
		if ( event.m_dwlParam == AddFriend )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NATEON_ADD_BUDDY, m_strNickName
				, g_pkStrLoader->GetString( STR_ID_NATEON_ADD_BUDDY_DEFMSG )
				, KGCUIScene::GC_MBOX_USE_NATEON_ADD_BUDDY, 0, 0, false, true );
		}
		else if ( event.m_dwlParam == FairplayReport )
		{
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_FAIRPLAY_REPORT, g_kGlobalValue.m_kUserInfo.strNickName, m_strNickName );
		}
		else if ( event.m_dwlParam == BlockFriend)
		{
			if( SiKGCCoupleSystem()->GetNickName_of_my_honey() == m_strNickName )
			{
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM40 )
					, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
			}
            else if( SiKFriendShip()->FindUIDInContactFriendList(m_dwSelectedUID) )
            {
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_CAN_NOT_DELETE_RECOMMENDER )
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
#if defined( COUPLE_SYSTEM ) && !defined( NATION_CHINA )
		else if( event.m_dwlParam == CouplePropose )
		{
            if( SiKGCCoupleSystem()->GetRingList(true, true).empty() )
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COUPLE_MESSAGE_OK, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM22 ), L"" );
            else
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COUPLE_PROPOSE, m_strNickName, L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_dwSelectedUID );
		}
		else if( event.m_dwlParam == CoupleWnd )
		{
            KP2P::GetInstance()->Send_CoupleInfoReq( m_dwSelectedUID );
		}
		else if( event.m_dwlParam == CoupleRingChange )
		{
            if( SiKGCCoupleSystem()->GetRingList(true, true).empty() )
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COUPLE_MESSAGE_OK, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM22 ), L"" );
            else
                g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COUPLE_PROPOSE, m_strNickName, L"", KGCUIScene::GC_MBOX_USE_COUPLE_RING_CHANGE );
		}
		else if( event.m_dwlParam == CoupleGoodbye )
		{
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COUPLE_GOODBYE, L"", L"", KGCUIScene::GC_MBOX_USE_COUPLE_GOODBYE_REQ );

		}
#endif
        else if( event.m_dwlParam == Recommend )
        {
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString(STR_ID_WILL_YOU_RECOMMEND, "l", m_strNickName), L"", KGCUIScene::GC_MBOX_USE_RECOMMEND_PICKED_USER, m_dwSelectedUID, 0, false, true );
        }
#if defined (PARTY_SYSTEM)
        else if ( event.m_dwlParam == InviteParty )
        {
            if ( event.m_dwlParam ) {
                SiKGCPartyManager()->SendInviteUser(m_dwSelectedUID, KPartyData::PT_NORMAL);
	}
        }
#endif
	}

    // 스크롤 바라면~
    POINT ptMPos = g_pkInput->GetMousePos();
    D3DXVECTOR2 vTmpPos;
    vTmpPos.x = (float)ptMPos.x;
    vTmpPos.y = (float)ptMPos.y;
    
    if ( g_pkInput->BtnUp( KInput::MBLEFT ) && !m_pkPopupBox->CheckPosInWindowBound( vTmpPos ) )
        m_pkPopupBox->ToggleRender( false );

    if ( event.m_pWnd == m_pkScrollbar )
    {
        switch ( event.m_dwCode )
        {
            case 0:
            case D3DWE_SCROLLBAR_DOWNBTN:
            case D3DWE_SCROLLBAR_UPBTN:
            case D3DWE_SCROLLBAR_DRAG:
            case D3DWE_SCROLLBAR_WHEELDOWN:
            case D3DWE_SCROLLBAR_WHEELUP:
                m_iTopIndex = m_pkScrollbar->GetScrollPos();
                m_pkScrollbar->SetScrollPos( m_iTopIndex );

                if ( m_vecUserInfo.size() < SQUARE_USERINFO_ELEMENT_NUM)
                    m_pkScrollbar->SetScrollRangeMax( SQUARE_USERINFO_ELEMENT_NUM );
                else
                    m_pkScrollbar->SetScrollRangeMax( m_vecUserInfo.size() );

                PrintUserList();
                break;
        }
    }
    else if ( event.m_pWnd == m_pkCloseBtn )
    {
        switch ( event.m_dwCode )
        {
            case D3DWE_BUTTON_CLICK:
                m_bRenderUserList = false;
                this->ToggleRender( m_bRenderUserList );
                break;
        }
    }

    for ( int i = 0; i < SQUARE_USERINFO_ELEMENT_NUM; i++)
    {
        if ( m_pkElement[i] == event.m_pWnd )
        {
            // 물론 인간 아뒤 정도는 있어줘야한다! 나는 띄우지 말쟈!
            if ( !m_pkElement[i]->IsEmpty() && !IsLocalPlayer( m_pkElement[i]->GetUserUID() ))
            {
                // 네임 배경 박스안에서 우클릭 됐을때!
                if ( m_pkElement[i]->IsRBtnClickOnList() )
                {
                    // 클릭한 아이 바로 오른편에 띄우쟈!
                    D3DXVECTOR2 vTempPos = m_pkElement[i]->GetFixedWindowLocalPos();
                    vTempPos.x = (float)m_pkElement[i]->GetWidth();
                    m_pkPopupBox->SetWindowPosDirect( vTempPos );
                    m_pkPopupBox->InitState( true, true, this );
					m_strNickName = m_pkElement[i]->GetUserName();
					SetMenu( m_pkElement[i]->GetUserUID() );

                    m_pkElement[i]->SetRClick( false );
                }
                // 네임 배경 박스안에서 좌클릭 됐을때!
                else if ( m_pkElement[i]->IsLBtnClickOnList() )
                {
                    m_bIsLClickUserList = true;
                    m_strClickedUserName = m_pkElement[i]->GetUserName();

                    DWORD dwClickedUserUID = m_pkElement[i]->GetUserUID();
                    m_strClickedUserAccountName = SiGCSquare()->GetSquarePeople( dwClickedUserUID )->GetPeopleInfo().m_strNick;

                    m_pkElement[i]->SetLClick( false );
                }
            }
        }
    }
}

void KGCSquareUserList::SetUserList( int iIndex, std::wstring strUserNick, int iGradeMark, DWORD dwUID )
{
    m_pkElement[iIndex]->SetUserInfo( strUserNick, iGradeMark, dwUID );
}

void KGCSquareUserList::PushUserListVector( const KSquareUserInfo &userInfo )
{
    // 넣을 때 같은놈 검사 안해도 되겠지? ㅎ 
    USERLIST_INFO temp;
    temp.strNick = userInfo.m_strNick;
    temp.strNickColor = userInfo.m_strNickColor;
    temp.dwPeopleUID = userInfo.m_dwUID;
    temp.iGradeMark = static_cast<int>(userInfo.m_cGrade);

    m_vecUserInfo.push_back( temp );

    PrintUserList();
}

void KGCSquareUserList::PopUserListVector( DWORD dwPeopleUID )
{
    std::vector<USERLIST_INFO>::iterator tmpItr = m_vecUserInfo.begin();
    for (;tmpItr != m_vecUserInfo.end(); tmpItr++ )
    {
        if (tmpItr->dwPeopleUID == dwPeopleUID)
        {
            // 나간놈을 우클릭해서 팝업 박스가 떠있다면 없애버려!
            if ( tmpItr->strNick == m_strNickName )
                m_pkPopupBox->ToggleRender( false );

            m_vecUserInfo.erase( tmpItr );
            break;
        }
    }

    PrintUserList();
}

void KGCSquareUserList::ClearUserListVector()
{
    m_vecUserInfo.clear();
}

void KGCSquareUserList::PrintUserList()
{
    // 지우고 새로 그리쟈!
    for ( int i = 0; i < SQUARE_USERINFO_ELEMENT_NUM; i++)
        m_pkElement[i]->initElement();

    // 이름 이쁘게 넣어염!
    while ( m_iTopIndex + SQUARE_USERINFO_ELEMENT_NUM > (int)m_vecUserInfo.size() )
        m_iTopIndex--;

    if ( m_vecUserInfo.size() < SQUARE_USERINFO_ELEMENT_NUM)
        m_pkScrollbar->SetScrollRangeMax( SQUARE_USERINFO_ELEMENT_NUM );
    else
        m_pkScrollbar->SetScrollRangeMax( m_vecUserInfo.size() );

    // 유저벡터 몇번째 부터 찍을까?
    std::vector<USERLIST_INFO>::iterator tmpItr = m_vecUserInfo.begin();
    for ( int i = 0; (tmpItr != m_vecUserInfo.end()) && (i < m_iTopIndex); i++, tmpItr++) {}

    // 엥? 왜 이리와? -_-
    if (tmpItr == m_vecUserInfo.end())
        return;

    std::wstring strNickColorTemp;

    if (tmpItr->strNickColor == L"")
        strNickColorTemp = L"FFFFFF";
    else
        strNickColorTemp = tmpItr->strNickColor;

    for ( int idx = 0; (tmpItr != m_vecUserInfo.end()) && (idx < SQUARE_USERINFO_ELEMENT_NUM); tmpItr++, idx++ )
        SetUserList( idx, L"#c" + tmpItr->strNickColor + tmpItr->strNick + L"#cx", tmpItr->iGradeMark, tmpItr->dwPeopleUID);
}

void KGCSquareUserList::SetRenderUserList( bool bRenderUserList )
{
    m_bRenderUserList = bRenderUserList;
}

bool KGCSquareUserList::IsLocalPlayer( DWORD dwUID ) const
{
    return SiGCSquare()->GetSquarePeople( dwUID )->IsLocalPlayer();
}

void KGCSquareUserList::SetLClickUserlist( bool isClick )
{
    m_bIsLClickUserList = isClick;
}

bool KGCSquareUserList::CheckPosPopupBoxBound( const D3DXVECTOR2& vTempPos )
{
    return m_pkPopupBox->CheckPosInWindowBound( vTempPos );
}

void KGCSquareUserList::SetPopupBoxRender( bool isRender )
{
    m_pkPopupBox->ToggleRender( isRender );
}

void KGCSquareUserList::SetMenu( DWORD dwUID )
{
	m_pkPopupBox->ClearMenu();
	m_pkPopupBox->AddMenu( 1, g_pkStrLoader->GetString( STR_ID_FRIEND_ADD_TOOLTIP ) );
    m_dwSelectedUID = dwUID;

#if defined( COUPLE_SYSTEM ) && !defined( NATION_CHINA )
	if( SiKGCCoupleSystem()->Am_I_Solo() && SiKGCCoupleSystem()->Are_you_Solo( dwUID ) )
	{
		m_pkPopupBox->AddMenu( 4, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM1 ) );
	}
	else if( SiKGCCoupleSystem()->Are_you_Solo( dwUID ) == false )
	{
		m_pkPopupBox->AddMenu( 5, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM3 ) );
	}
#endif

#if !defined( DISABLE_BADGUY_REPORT )
	m_pkPopupBox->AddMenu( 2, g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT9 ) );
#else
	m_pkPopupBox->AddMenu( 2, g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT9 ), 0xFF999999, false );
#endif

	if ( true == g_kGlobalValue.IsBlockUser( m_strNickName ) ||
         true == SiKFriendShip()->IsBlockFriend( m_strNickName) )
		m_pkPopupBox->AddMenu( 3, g_pkStrLoader->GetString( STR_ID_RELEASE_BADGUY ) );
	else
		m_pkPopupBox->AddMenu( 3, g_pkStrLoader->GetString( STR_ID_BLOCK_BADGUY ) );

#if defined( COUPLE_SYSTEM ) && !defined( NATION_CHINA )
	if( SiKGCCoupleSystem()->Are_you_My_Honey( dwUID ) )
	{
		m_pkPopupBox->ClearMenu();
		m_pkPopupBox->AddMenu( 6, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM4 ) );
		m_pkPopupBox->AddMenu( 7, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM2 ) );
	}
#endif

    if(g_pkUIScene->m_pkNickNameBox->GetRecommendRegisterComplete() == false && !g_kGlobalValue.m_bExpAccount)
    {
        m_pkPopupBox->AddMenu(8, g_pkStrLoader->GetString(STR_ID_RECOMMEND_FRIEND) );
    }
#if defined (PARTY_SYSTEM)
    if ( !(SiKGCPartyManager()->IsParty()) || SiKGCPartyManager()->IsLocalPartyMasterUser() || !SiKGCPartyManager()->IsMaxSize() )
        m_pkPopupBox->AddMenu(9, g_pkStrLoader->GetString( STR_ID_INVITE_PARTY_MENU ) );
#endif

}