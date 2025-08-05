#include "stdafx.h"
#include "GCAgitListComboPopup.h"
#include "GCAgitUserElement.h"
#include "GCStateGame.h"

IMPLEMENT_CLASSNAME( KGCAgitListComboPopup );
IMPLEMENT_WND_FACTORY( KGCAgitListComboPopup );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitListComboPopup, "gc_agit_combolist_popup" );

KGCAgitListComboPopup::KGCAgitListComboPopup( void )
: m_pkPopupBox ( NULL )
, m_strNickName( L"" )
, m_strClickedUserName( L"" )
, m_dwSelectedUID(0)
, m_bIsLClickUserList(false)
, m_bPopUp(true)
{

	LINK_CONTROL( "popupbox", m_pkPopupBox );

    m_pkScroll = NULL;
    LINK_CONTROL( "scroll", m_pkScroll );

    m_pkBack = NULL;
    LINK_CONTROL( "back", m_pkBack );

    for( int i=0 ; i<NUM_USER ; ++i )
    {
        m_apkUserList[i] = NULL;

        char szTemp[ 128 ] = { 0, };
        sprintf_s( szTemp, 127, "staticUser%d", i );
        LINK_CONTROL( szTemp, m_apkUserList[i] );
    }

    m_iScrollSpeed = 0;
    m_iScrollPos = 0;
    m_iBeforePos = 0;
    m_iScrollPage = 0;


}

KGCAgitListComboPopup::~KGCAgitListComboPopup( void )
{
}

void KGCAgitListComboPopup::OnCreate( void )
{
    for( int i=0 ; i<NUM_USER ; ++i )
        m_apkUserList[i]->InitState( true, true, this );

    m_pkScroll->InitState( true, true ,this );

    m_pkScroll->SetScrollPos( 0 );

    m_pkScroll->SetScrollPageSize( NUM_USER );

    m_pkScroll->SetScrollRangeMax( m_iScrollPage );
    m_pkScroll->ResizeBtn();

    m_pkBack->InitState( true, false, NULL );
    m_pkBack->SetAllWindowAlpha( 180 );

	if ( !m_pkPopupBox )
	{
		m_bPopUp = false;
	}

	if ( m_bPopUp ) 
		m_pkPopupBox->InitState( false, true, this );
}

void KGCAgitListComboPopup::ActionPerformed( const KActionEvent& event )
{
	if ( m_bPopUp )
	{
		if( event.m_dwCode == KGCMultiMenu::K3DWE_MENU_CLICKED &&
			event.m_pWnd == m_pkPopupBox )
		{
			if ( event.m_dwlParam == EM_AGIT_USER_MENU_INVITE_PARTY )
			{
				//	파티 초대
#if defined (PARTY_SYSTEM)
				if ( event.m_dwlParam ) {
					SiKGCPartyManager()->SendInviteUser(m_dwSelectedUID, KPartyData::PT_NORMAL );
				}
#endif
			}
			else if ( event.m_dwlParam == EM_AGIT_USER_MENU_AGIT_VISIT )
			{
				//	다른 유저 아지트 방문	
                if ( SiKGCPartyManager()->IsParty() )
                    SiKGCPartyManager()->LeaveParty();
                SiKGCPartyManager()->SetInviteState( true );
				g_pStateGame->OnExitGame(); // 내아지트에서 나온 다음
				SiKGCAgitGameManager()->EnterAgit( m_dwSelectedUID ); // 이동~
				return;
			}
			else if ( event.m_dwlParam == EM_AGIT_USER_MENU_ADD_FRIEND)
			{
				g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_NATEON_ADD_BUDDY, m_strNickName
					, g_pkStrLoader->GetString( STR_ID_NATEON_ADD_BUDDY_DEFMSG )
					, KGCUIScene::GC_MBOX_USE_NATEON_ADD_BUDDY, 0, 0, false, true );
			}
			else if( event.m_dwlParam == EM_AGIT_USER_MENU_COUPLE_PROPOSE )
			{
#if defined( COUPLE_SYSTEM )
				if( SiKGCCoupleSystem()->GetRingList(true, true).empty() )
					g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COUPLE_MESSAGE_OK, g_pkStrLoader->GetString( STR_ID_COUPLE_SYSTEM22 ), L"" );
				else
					g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_COUPLE_PROPOSE, m_strNickName, L"", KGCUIScene::GC_MBOX_USE_NORMAL, m_dwSelectedUID );
#endif
			}
			else if( event.m_dwlParam == EM_AGIT_USER_MENU_BLOCK_FRIEND )
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
			else if( event.m_dwlParam == EM_AGIT_USER_MENU_AGIT_BAN )
			{
				//	아지트 추방하기
				SiKGCAgitGameManager()->BanUser( m_dwSelectedUID );
			}
		}

		// 스크롤 바라면~
		POINT ptMPos = g_pkInput->GetMousePos();
		D3DXVECTOR2 vTmpPos;
		vTmpPos.x = (float)ptMPos.x;
		vTmpPos.y = (float)ptMPos.y;

		if ( g_pkInput->BtnUp( KInput::MBLEFT ) && !m_pkPopupBox->CheckPosInWindowBound( vTmpPos ) )
			m_pkPopupBox->ToggleRender( false );
	}


    if ( event.m_pWnd == m_pkScroll && (event.m_dwCode >= KD3DWnd::D3DWE_SCROLLBAR_WHEELUP 
        || event.m_dwCode == KD3DWnd::D3DWE_BUTTON_DOWN
        || event.m_dwCode == KD3DWnd::D3DWE_BUTTON_PRESS
        || event.m_dwCode == 0
        ) )
    {
        // 스크롤 위치 
        int iTemp = m_pkScroll->GetScrollPos();

        if( m_pkScroll->IsRenderOn() )
        {
            if( iTemp == 0 ) 
                m_iScrollPos = 0;
            else if( iTemp == m_pkScroll->GetScrollRangeMax() )
                m_iScrollPos = m_pkScroll->GetScrollRangeMax();
            else if( (m_iBeforePos>iTemp) && (m_iScrollPos>0) )
                --m_iScrollPos;
            else if( (m_iBeforePos<iTemp) && (m_iScrollPos<m_iScrollPage) )
                ++m_iScrollPos;
            m_iBeforePos = iTemp;

        }
        else if( m_iScrollPos != iTemp )
        {
            m_iScrollPos = iTemp;
            g_KDSound.Play( "73" );
        }

        // 스크롤 위치 따라 보여질 목록 조절 
        SetElementList( m_iScrollPos );
	}

	if ( m_bPopUp )
	{
		for ( int i = 0; i < NUM_USER; i++)
		{
			if ( m_apkUserList[i] == event.m_pWnd )
			{
				// 물론 인간 아뒤 정도는 있어줘야한다! 나는 띄우지 말쟈!
				if ( !m_apkUserList[i]->IsEmpty() && !IsLocalPlayer( m_apkUserList[i]->GetUserUID() ))
				{
					// 네임 배경 박스안에서 우클릭 됐을때!
					if ( m_apkUserList[i]->IsRBtnClickOnList() )
					{
						// 클릭한 아이 바로 오른편에 띄우쟈!
						D3DXVECTOR2 vTempPos = m_apkUserList[i]->GetFixedWindowLocalPos();
						vTempPos.x = (float)m_apkUserList[i]->GetWidth();
						m_pkPopupBox->SetWindowPosDirect( vTempPos );
						m_pkPopupBox->InitState( true, true, this );
						m_strNickName = m_apkUserList[i]->GetUserName();
						SetMenu( m_apkUserList[i]->GetUserUID() );

						m_apkUserList[i]->SetRClick( false );
					}
					// 네임 배경 박스안에서 좌클릭 됐을때!
					else if ( m_apkUserList[i]->IsLBtnClickOnList() )
					{
						m_bIsLClickUserList = true;
						m_strClickedUserName = m_apkUserList[i]->GetUserName();
						m_apkUserList[i]->SetLClick( false );
					}
				}
			}
		}
	}
}

void KGCAgitListComboPopup::SetUserList( std::map< DWORD, std::wstring >& mapInfo_ )
{
    if( mapInfo_.empty() )
        return;

    for( std::map< DWORD, std::wstring >::iterator mit = mapInfo_.begin() ; mit != mapInfo_.end() ; ++mit )
        m_vecUserList.push_back( std::make_pair( mit->first, mit->second  ) );

	m_iScrollPos = 0;

	m_pkScroll->SetScrollRangeMax( m_vecUserList.size() );

    SetElementList( m_iScrollPos );
}

void KGCAgitListComboPopup::Clear( void )
{
    for( int i=0 ; i<NUM_USER ; ++i )
        m_apkUserList[i]->Clear();

    m_iScrollSpeed = 0;
    m_iScrollPos = 0;
    m_iBeforePos = 0;
    m_iScrollPage = 0;

    m_vecUserList.clear();
}

void KGCAgitListComboPopup::SetElementList( int iStarIndex_ )
{
    for( int i=0 ; i<NUM_USER ; ++i )
    {
        if( (i + iStarIndex_) >= static_cast<int>( m_vecUserList.size() ) )
            break;

		m_apkUserList[i]->SetUserName( m_vecUserList[ i+iStarIndex_ ].second, m_vecUserList[ i+iStarIndex_ ].first );
    }

	m_pkScroll->SetScrollPos( iStarIndex_ );
}

bool KGCAgitListComboPopup::IsLocalPlayer( DWORD dwUID ) const
{
	if ( g_kGlobalValue.m_kUserInfo.dwUID == dwUID )
		return true;
	return false;
}


void KGCAgitListComboPopup::SetLClickUserlist( bool isClick )
{
	m_bIsLClickUserList = isClick;
}

bool KGCAgitListComboPopup::CheckPosPopupBoxBound( const D3DXVECTOR2& vTempPos )
{
	return m_pkPopupBox->CheckPosInWindowBound( vTempPos );
}

void KGCAgitListComboPopup::SetPopupBoxRender( bool isRender )
{
	if ( m_bPopUp ) 
	{
		m_pkPopupBox->ToggleRender( isRender );
	}
}


void KGCAgitListComboPopup::SetMenu( DWORD dwUID )
{
	m_pkPopupBox->ClearMenu();
	m_dwSelectedUID = dwUID;

#if defined (PARTY_SYSTEM)
	if ( !(SiKGCPartyManager()->IsParty()) || SiKGCPartyManager()->IsLocalPartyMasterUser() || !SiKGCPartyManager()->IsMaxSize() )
        m_pkPopupBox->AddMenu(1, g_pkStrLoader->GetString( STR_ID_AGIT_USER_MENU01 ) );
#endif

	m_pkPopupBox->AddMenu( 2, g_pkStrLoader->GetString( STR_ID_AGIT_USER_MENU02 ) );
	m_pkPopupBox->AddMenu( 3, g_pkStrLoader->GetString( STR_ID_AGIT_USER_MENU03 ) );

#if defined( COUPLE_SYSTEM )
//	m_pkPopupBox->AddMenu( 4, g_pkStrLoader->GetString( STR_ID_AGIT_USER_MENU04 ) );
#endif

	m_pkPopupBox->AddMenu( 5, g_pkStrLoader->GetString( STR_ID_AGIT_USER_MENU06 ) );

	if ( SiKGCAgitGameManager()->IsAgitOwner() )
	{
		m_pkPopupBox->AddMenu( 6, g_pkStrLoader->GetString( STR_ID_AGIT_USER_MENU07 ) );
	}
}