#include "stdafx.h"
#include ".\gctalklist.h"
//
//

//
#include "GCNateOnChat.h"
//

IMPLEMENT_CLASSNAME( GCTalkList );
IMPLEMENT_WND_FACTORY( GCTalkList );
IMPLEMENT_WND_FACTORY_NAME( GCTalkList, "gc_talk_list" );


GCTalkList::GCTalkList(void)
{
	for(int i=0;i<MAX_TALK_LIST;++i)
	{
		char szTemp[MAX_PATH];
		m_pkTalkBack[i] = NULL;
		m_pkTalkTitle[i] = NULL;
		m_pkTalkClose[i] = NULL;
		m_pkAlert[i] = NULL;

		sprintf(szTemp, "talk_back%d", i+1);
		LINK_CONTROL( szTemp, m_pkTalkBack[i] );
		sprintf(szTemp, "talk_title%d", i+1);
		LINK_CONTROL( szTemp, m_pkTalkTitle[i] );
		sprintf(szTemp, "talk_close%d", i+1);
		LINK_CONTROL( szTemp, m_pkTalkClose[i] );
		sprintf(szTemp, "alert%d", i+1);
		LINK_CONTROL( szTemp, m_pkAlert[i] );

		m_TalkKey[i] = std::make_pair( 0, 0 );
	}

	m_pkTalkListLeft 		= NULL;
	m_pkTalkListRight		= NULL;
	m_iTalkListIndex		= 0;
	m_CurTalkKey			= std::make_pair(0, 0);

	LINK_CONTROL( "talk_list_left", m_pkTalkListLeft );
	LINK_CONTROL( "talk_list_right", m_pkTalkListRight );
}

GCTalkList::~GCTalkList(void)
{
}

void GCTalkList::ActionPerformed( const KActionEvent& event )
{
	JIF( KP2P::GetInstance()->m_kMsgServer != NULL );

	if( D3DWE_BUTTON_CLICK == event.m_dwCode )
	{	
		for(int i=0;i<MAX_TALK_LIST;++i)
		{
			if( event.m_pWnd == m_pkTalkClose[i] )
			{
				KP2P::GetInstance()->m_kMsgServer->Send_LeaveChatMemberReq_S2( m_TalkKey[i] );
				break;
			}
		}
	}

	GCWND_MSG_MAP( m_pkTalkListLeft, KD3DWnd::D3DWE_BUTTON_CLICK, LeftTalkList );
	GCWND_MSG_MAP( m_pkTalkListRight, KD3DWnd::D3DWE_BUTTON_CLICK, RightTalkList );
}

void GCTalkList::OnCreate(void)
{
	for(int i=0;i<MAX_TALK_LIST;++i)
	{
		m_pkTalkBack[i]->InitState( false, true, this );
		m_pkTalkTitle[i]->InitState( false, true, this );
		m_pkTalkClose[i]->InitState( false, true, this );
		m_pkAlert[i]->InitState( false, true, this );
	}
	m_pkTalkListLeft->InitState( true, true, this );
	m_pkTalkListRight->InitState( true, true, this );
}

void GCTalkList::LeftTalkList( void )
{
	if( false == IsTalkListIndex( m_iTalkListIndex - 1 ) )
		return;

	m_iTalkListIndex -= 1;

	RefreshTalkList();
}

void GCTalkList::RightTalkList( void )
{
	if( false == IsTalkListIndex( m_iTalkListIndex + 1 ) )
		return;

	m_iTalkListIndex += 1;

	RefreshTalkList();
}

bool GCTalkList::IsTalkListIndex( int iIndex )
{
	if( (iIndex + MAX_TALK_LIST) <= ((int)m_vecTalkList.size()) 
		&& iIndex >= 0 )
		return true;

	return false;
}

bool GCTalkList::IsTalkListSlotIndex( const int iSlotIndex )
{
	if( iSlotIndex >= MAX_TALK_LIST || iSlotIndex < 0 )
		return false;

	return true;
}

void GCTalkList::RefreshTalkList( void )
{
	for(int i=0;i<MAX_TALK_LIST;++i)
		ToggleRenderTalkList( i, false );

	InitSelectState();

	int iCnt = 0;
	int iSkipCnt = m_iTalkListIndex;
	for(int i=0;i<(int)m_vecTalkList.size();++i)
	{
		if( 0 < iSkipCnt )
		{
			--iSkipCnt;
		}
		else
		{
			if( iCnt >= MAX_TALK_LIST )
				break;

			ToggleRenderTalkList( iCnt, true );
			SetTalkListTitle( iCnt, m_vecTalkList[i].second.first, m_vecTalkList[i].second.second);
			m_TalkKey[ iCnt ] = m_vecTalkList[i].first;

			NATE_TALK_BOX* pTalkBox 
				= g_pkUIScene->m_pkNateOn->GetNateOnChatWindow()->GetTalkBox( m_vecTalkList[i].first );
			if( pTalkBox )
			{
				if( pTalkBox->IsNew() )
				{
					m_pkAlert[ iCnt ]->ToggleRender( true );
				}
			}

			++iCnt;
		}
	}

	for(int i=0;i<MAX_TALK_LIST;++i)
	{
		if( m_TalkKey[i] == GetCurTalkKey() )
		{
			SelectTalkList( i );
			break;
		}
	}
}

void GCTalkList::SetFirstIndexByKey( std::pair< DWORD, DWORD > Key )
{
	m_iTalkListIndex	= 0;
	for(int i=0;i<(int)m_vecTalkList.size();++i)
	{
		if( Key == m_vecTalkList[i].first )
		{
			if( IsTalkListIndex( i ) )
			{
				m_iTalkListIndex = i;
				break;
			}
		}
	}

	RefreshTalkList();
}

void GCTalkList::SetTalkListTitle( const int iSlotIndex, const std::wstring wstrNickName, std::wstring wstrNickColor )
{
	if( IsTalkListSlotIndex(iSlotIndex) )
	{
		m_pkTalkTitle[iSlotIndex]->SetForceWordWrapText( wstrNickName, true );
		m_pkTalkTitle[iSlotIndex]->SetFontColor(SiGCTextColorManager()->StringToColor(wstrNickColor) );
	}
}

void GCTalkList::AddTalkList( std::pair<DWORD, DWORD> Key, std::wstring wstrTalkListName, std::wstring wstrTalkListColor )
{
	m_vecTalkList.push_back( std::make_pair( Key, std::make_pair(wstrTalkListName, wstrTalkListColor) ) );
}

void GCTalkList::DeleteTalkList( std::pair<DWORD, DWORD> UID )
{
	for(int i=0;i<(int)m_vecTalkList.size();++i)
	{
		if( UID == m_vecTalkList[i].first )
		{
			m_vecTalkList.erase( m_vecTalkList.begin() + i );

			if( (int)m_vecTalkList.size() <= m_iTalkListIndex )
			{
				--m_iTalkListIndex;

				if( m_iTalkListIndex < 0 )
					m_iTalkListIndex = 0;
			}

			break;
		}
	}

	for(int i=0;i<MAX_TALK_LIST;++i)
	{
		if( UID == m_TalkKey[i] )
			m_TalkKey[i] = std::make_pair( 0, 0 );
	}
}

void GCTalkList::DeleteAllTalkList()
{
	m_vecTalkList.clear();
}

void GCTalkList::ToggleRenderTalkList( const int iSlotIndex, const bool bShow )
{
	if( IsTalkListSlotIndex(iSlotIndex) )
	{
		// 대화창 리스트를 관리 해주는 버튼
		m_pkTalkBack[iSlotIndex]->ToggleRender( bShow );
		m_pkTalkTitle[iSlotIndex]->ToggleRender( bShow );
		m_pkTalkClose[iSlotIndex]->ToggleRender( bShow );
		m_TalkKey[iSlotIndex] = std::make_pair( 0, 0 );
	}
}

void GCTalkList::SelectTalkList( const int iSlotIndex )
{
	if( false == IsTalkListSlotIndex( iSlotIndex ) )
		return;

	for(int i=0;i<MAX_TALK_LIST;++i)
	{
		m_pkTalkBack[i]->SetWndMode( D3DWM_DEFAULT );
		m_pkTalkBack[i]->SetSelfInputCheck( true );
		m_pkTalkBack[i]->ParentRendeOff();
	}

	m_pkTalkBack[iSlotIndex]->SetWndMode( D3DWM_HOVER );
	m_pkTalkBack[iSlotIndex]->SetSelfInputCheck( false );
	m_pkAlert[iSlotIndex]->ToggleRender( false );

	NATE_TALK_BOX* pTalkBox 
		= g_pkUIScene->m_pkNateOn->GetNateOnChatWindow()->GetTalkBox( m_TalkKey[iSlotIndex] );
	if( pTalkBox )
		pTalkBox->SetNew( false );

}

void GCTalkList::InitSelectState( void )
{
	for(int i=0;i<MAX_TALK_LIST;++i)
	{
		m_pkTalkBack[i]->SetWndMode( D3DWM_DEFAULT );
		m_pkTalkBack[i]->SetSelfInputCheck( true );
		m_pkTalkBack[i]->ParentRendeOff();
	}
}

void GCTalkList::FrameMoveInEnabledState(void)
{
	FrameMoveAlert();

	if( g_pkInput->BtnUp(KInput::MBLEFT) )
	{
		const POINT ptMPos = g_pkInput->GetMousePos();
		const D3DXVECTOR2 vPos = D3DXVECTOR2( (float)ptMPos.x, (float)ptMPos.y );

		for(int i=0;i<MAX_TALK_LIST;++i)
		{
			if( m_pkTalkBack[i]->IsRenderOn() )
			{
				if( m_pkTalkBack[i]->CheckPosInWindowBound( vPos )
					|| m_pkTalkTitle[i]->CheckPosInWindowBound( vPos ) )
				{
					SetCurTalkKey( m_TalkKey[i] );
					SelectTalkList( i );
					SpeakToActionListener( KActionEvent( this, GCNateOn::NATEON_TALK_BOX_SELECT ) );

					break;
				}
			}
		}
	}
}

void GCTalkList::SelectTalkList( std::pair< DWORD, DWORD > Key )
{
	for(int i=0;i<MAX_TALK_LIST;++i)
	{
		if( Key == m_TalkKey[i] )
		{
			m_pkTalkBack[i]->SetWndMode( D3DWM_HOVER );
			m_pkTalkBack[i]->SetSelfInputCheck( false );
			m_pkTalkBack[i]->ParentRendeOff();
			m_pkAlert[i]->ToggleRender( false );

			NATE_TALK_BOX* pTalkBox 
				= g_pkUIScene->m_pkNateOn->GetNateOnChatWindow()->GetTalkBox( m_TalkKey[i] );
			if( pTalkBox )
				pTalkBox->SetNew( false );
		}
		else
		{
			m_pkTalkBack[i]->SetWndMode( D3DWM_DEFAULT );
			m_pkTalkBack[i]->SetSelfInputCheck( true );
		}
	}
}

std::pair<DWORD, DWORD> GCTalkList::GetCurTalkKey( void )
{
	return m_CurTalkKey;
}

void GCTalkList::SetCurTalkKey( std::pair<DWORD, DWORD> Key )
{
	m_CurTalkKey = Key;
}

std::pair<DWORD, DWORD> GCTalkList::GetTalkListKey( const int iIndex )
{
	if( (int)m_vecTalkList.size() <= iIndex )
		return std::make_pair(0, 0);

	return m_vecTalkList[iIndex].first;
}

void GCTalkList::SetNewTalk( const std::pair<DWORD, DWORD> CurTalkKey, const bool bVisible )
{
	for(int i=0;i<MAX_TALK_LIST;++i)
	{
		if( CurTalkKey == m_TalkKey[i] )
		{
			m_pkAlert[i]->ToggleRender( bVisible );
			break;
		}
	}
}

void GCTalkList::FrameMoveAlert( void )
{
	int iVisibleCnt = 0;
	for(int i=0;i<MAX_TALK_LIST;++i)
	{
		if( m_pkAlert[i]->IsRenderOn() )
		{
			m_pkAlert[i]->SetWindowColor( D3DCOLOR_ARGB(s_bAlpha, 255, 255, 255) );
			++iVisibleCnt;
		}
	}

	if( 0 >= iVisibleCnt )
		return;

	if( abs( (long int)(timeGetTime() - s_dwLastUpdateTime) ) > 25 )
	{
		if( 255 <= s_bAlpha )
			s_bAlphaDir = false;
		else if( 0 >= s_bAlpha )
			s_bAlphaDir = true;

		int iResultValue = (int)s_bAlpha;

		if( s_bAlphaDir )
			iResultValue += 15;
		else
			iResultValue -= 15;

		if( iResultValue > 255 )
		{
			s_bAlpha = 255;
			s_bAlphaDir = false;
		}
		else if( iResultValue < 0 )
		{
			s_bAlpha = 0;
			s_bAlphaDir = true;
		}
		else
		{
			s_bAlpha = iResultValue;
		}

		s_dwLastUpdateTime = timeGetTime();
	}
}
