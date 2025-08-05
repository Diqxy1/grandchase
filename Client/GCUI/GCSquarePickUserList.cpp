#include "stdafx.h"
#include "../Square/GCSquarePeople.h"
#include "GCSquarePickUserList.h"

//

//
#include "GCMultiMenu.h"
#include "../Square/GCSquare.h"
//
//
//

#include "../gcui/GCNateOnMultiMenu.h"
#include "../BuddySystem/KFriendShip.h"

#include "GCNateOnAddFriend.h"

#include "GCCoupleSystem.h"

IMPLEMENT_CLASSNAME( KGCSquarePickUserList );
IMPLEMENT_WND_FACTORY( KGCSquarePickUserList );
IMPLEMENT_WND_FACTORY_NAME( KGCSquarePickUserList, "gc_square_pickuserlist" );

KGCSquarePickUserList::KGCSquarePickUserList(void)
:m_pkBG(NULL)
,m_pkMultiMenu(NULL)
, m_dwPickedUserUID( 0 )
{
    LINK_CONTROL( "background",         m_pkBG );
    LINK_CONTROL( "popupbox",           m_pkMultiMenu );
}

KGCSquarePickUserList::~KGCSquarePickUserList(void)
{
}

void KGCSquarePickUserList::OnCreate( void )
{
	m_pkBG->InitState( true, true, this );
	m_pkMultiMenu->InitState( false, true, this );
}

void KGCSquarePickUserList::FrameMoveInEnabledState( void )
{
    if ( m_pkMultiMenu->GetIsRenderMsgBox() )
    {
        this->ToggleRender( false );
        m_pkMultiMenu->SetIsRenderMsgBox( false );
    }
}
 
void KGCSquarePickUserList::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KGCMultiMenu::K3DWE_MENU_CLICKED &&
		event.m_pWnd == m_pkMultiMenu )
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
            g_pkUIScene->m_pkNateOnAddFriend->SetAccountNickName( m_strAccountNickName );
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
			// 커플인 경우 블록 하지 않는다.
			if( SiKGCCoupleSystem()->GetNickName_of_my_honey() == m_strNickName )
			{
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM40 )
					, L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, false, true );
			}
			else if( SiKFriendShip()->FindUIDInContactFriendList(m_dwPickedUserUID) )
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
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COUPLE_PROPOSE, m_strNickName, L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_dwPickedUserUID );
		}
		else if( event.m_dwlParam == CoupleWnd )
		{
			KP2P::GetInstance()->Send_CoupleInfoReq( m_dwPickedUserUID );
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
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetReplacedString(STR_ID_WILL_YOU_RECOMMEND, "l", m_strNickName), L"", KGCUIScene::GC_MBOX_USE_RECOMMEND_PICKED_USER, m_dwPickedUserUID, 0, false, true );
		}
#if defined (PARTY_SYSTEM)
        else if ( event.m_dwlParam == InviteParty )
        {
            SiKGCPartyManager()->SendInviteUser(m_dwPickedUserUID, KPartyData::PT_NORMAL);
            
	}
#endif
	}

    if ( event.m_dwCode == D3DWE_BUTTON_CLICK )
    {
        KD3DStatic* pChildStatic = NULL;
        POINT pt = g_pkInput->GetMousePos();
        D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

        for( int i = 9; i < (int)m_pkBG->GetNumChild(); i++)
        {
            pChildStatic = (KD3DStatic*)m_pkBG->GetChild(i);
            if ( pChildStatic->CheckPosInWindowBound( vMousePos ) )
            {
                D3DXVECTOR2 vTempPos = pChildStatic->GetFixedWindowLocalPos();
                vTempPos.x += (float)pChildStatic->GetWidth() + 7.0f;
                vTempPos.y -= 5.0f;

                m_pkMultiMenu->SetWindowPosDirect( vTempPos );
                m_strNickName = pChildStatic->GetText();

				m_dwPickedUserUID = m_mapUserUID[i];
				SetMenu( m_dwPickedUserUID );
                m_pkMultiMenu->ToggleRender( true );

                GCSquarePeople* pPeoPle = SiGCSquare()->GetSquarePeople( m_dwPickedUserUID );
                if( NULL != pPeoPle ) { 
                    m_strNickName = pPeoPle->GetPeopleInfo().m_strNick.c_str();     // 캐릭터 닉네임
                    m_strAccountNickName = pPeoPle->GetPeopleInfo().m_strNick;      // 계정 닉네임 
                }
            }
        }
    }

    if ( event.m_dwCode == D3DWE_CURSOR_ENTER )
    {
        KD3DStatic* pChildStatic = NULL;
        POINT pt = g_pkInput->GetMousePos();
        D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

        for( int i = 9; i < (int)m_pkBG->GetNumChild(); i++)
        {
            pChildStatic = (KD3DStatic*)m_pkBG->GetChild(i);
            if( pChildStatic->CheckPosInWindowBound( vMousePos ) )
                pChildStatic->SetFontColor( 0xFFFFFF00 );
            else 
                pChildStatic->SetFontColor( 0xFFFFFFFF );
        }
    }
    else if ( event.m_dwCode == D3DWE_CURSOR_LEAVE )
    {
        KD3DStatic* pChildStatic = NULL;
        POINT pt = g_pkInput->GetMousePos();
        D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );

        for( int i = 9; i < (int)m_pkBG->GetNumChild(); i++)
        {
            pChildStatic = (KD3DStatic*)m_pkBG->GetChild(i);
            pChildStatic->SetFontColor( 0xFFFFFFFF );
        }
    }
}

void KGCSquarePickUserList::ListHeightChange( void )
{
    const int iFrameThick = 2;
    int iHeight = (m_pkBG->GetNumChild() - 9) * 17;
    m_pkBG->SetHeight( iHeight + iFrameThick * 2 );

    for(int i = 3; i < 9; i++)
    {
        if( i == 3 || i == 4 || i == 8 )
        {
            m_pkBG->GetChild(i)->SetHeight(iHeight);
        }
        else
        {
            D3DXVECTOR2 wndpos = m_pkBG->GetChild(i)->GetFixedWindowLocalPos();
            wndpos.y = (float)(iFrameThick + iHeight);
            m_pkBG->GetChild(i)->SetWindowPosDirect( wndpos );
        }
    }
}

void KGCSquarePickUserList::AddPeople( std::wstring strName, DWORD dwUID_, DWORD dwAlign_ )
{
    KD3DStatic* pStatic = new KD3DStatic;

    pStatic->InitState( true, true, this );
    pStatic->SetAlign( dwAlign_ );

	m_mapUserUID[m_pkBG->GetNumChild()] = dwUID_;
    m_pkBG->AddChildWithRegistToLua( pStatic );

	if( (int)m_pkBG->GetWidth() < g_pkFontMgr->GetWidth( strName.c_str() ) + 15 )
	{
		m_pkBG->SetWidth( g_pkFontMgr->GetWidth( strName.c_str() ) + 15 );
		m_pkMultiMenu->SetWindowPosDirect( m_pkBG->GetFixedWindowLocalPos() + D3DXVECTOR2( (float)(m_pkBG->GetWidth() + 3), 0.0f ) );
	}

    D3DXVECTOR2 vStaticOffset(0, (float)(m_pkBG->GetNumChild() - 10) * 17 + 5);

    ListHeightChange();
    pStatic->SetOffset( D3DXVECTOR2(5, 0) );
    pStatic->SetWindowPosDirect( vStaticOffset );
    pStatic->SetWndState( D3DWS_ENABLED );
    pStatic->SetWidth( m_pkBG->GetWidth() );
    pStatic->SetHeight( 15 );
    pStatic->SetText( strName );
}

void KGCSquarePickUserList::DeletePeople( void )
{
    for ( int i = m_pkBG->GetNumChild() - 1; i >= 9; i-- )
        m_pkBG->DeleteChild( i );

	m_mapUserUID.clear();
	m_pkBG->SetWidth( 0 );
    m_pkMultiMenu->ToggleRender( false );
}

float KGCSquarePickUserList::GetListHeight( void ) const
{
    if ( m_pkBG->GetNumChild() <= 10 )
    {
        if ( m_pkBG->GetHeight() > m_pkMultiMenu->GetHeight() )
            return (float)m_pkBG->GetHeight();
        else
            return (float)m_pkMultiMenu->GetHeight(); 
    }
    else
        return (float)( m_pkBG->GetHeight() + m_pkMultiMenu->GetHeight() / 2 );
}

float KGCSquarePickUserList::GetListWidth( void ) const
{
    return (float)(m_pkBG->GetWidth() + m_pkMultiMenu->GetWidth() + 3);
}

void KGCSquarePickUserList::SetMenu( DWORD dwUID )
{
	m_pkMultiMenu->ClearMenu();
	m_pkMultiMenu->AddMenu( 1, g_pkStrLoader->GetString( STR_ID_FRIEND_ADD_TOOLTIP ) );

#if defined( COUPLE_SYSTEM ) && !defined( NATION_CHINA )
	if( SiKGCCoupleSystem()->Am_I_Solo() && SiKGCCoupleSystem()->Are_you_Solo( dwUID ) )
	{
		m_pkMultiMenu->AddMenu( 4, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM1 ) );
	}
	else if( SiKGCCoupleSystem()->Are_you_Solo( dwUID ) == false )
	{
		m_pkMultiMenu->AddMenu( 5, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM3 ) );
	}
#endif

#if !defined( DISABLE_BADGUY_REPORT )
	m_pkMultiMenu->AddMenu( 2, g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT9 ) );
#else
	m_pkMultiMenu->AddMenu( 2, g_pkStrLoader->GetString( STR_ID_FAIRPLAY_REPORT9 ), 0xFF999999, false );
#endif

	if ( true == g_kGlobalValue.IsBlockUser( m_strNickName ) ||
         true == SiKFriendShip()->IsBlockFriend( m_strNickName ) )
		m_pkMultiMenu->AddMenu( 3, g_pkStrLoader->GetString( STR_ID_RELEASE_BADGUY ) );
	else
		m_pkMultiMenu->AddMenu( 3, g_pkStrLoader->GetString( STR_ID_BLOCK_BADGUY ) );

#if defined( COUPLE_SYSTEM ) && !defined( NATION_CHINA )
	if( SiKGCCoupleSystem()->Are_you_My_Honey( dwUID ) )
	{
		m_pkMultiMenu->ClearMenu();
		m_pkMultiMenu->AddMenu( 6, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM4 ) );
		m_pkMultiMenu->AddMenu( 7, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM2 ) );
	}
#endif

	if(g_pkUIScene->m_pkNickNameBox->GetRecommendRegisterComplete() == false && !g_kGlobalValue.m_bExpAccount)
	{
		m_pkMultiMenu->AddMenu(8, g_pkStrLoader->GetString(STR_ID_RECOMMEND_FRIEND) );
	}
#if defined (PARTY_SYSTEM)
    if ( !(SiKGCPartyManager()->IsParty()) || SiKGCPartyManager()->IsLocalPartyMasterUser() || !SiKGCPartyManager()->IsMaxSize() )
        m_pkMultiMenu->AddMenu(9, g_pkStrLoader->GetString( STR_ID_INVITE_PARTY_MENU ) );
#endif
}