#include "StdAfx.h"
#include "KGCGuildJoinManageDlg.h"

IMPLEMENT_CLASSNAME( KGCGuildJoinManageDlg );
IMPLEMENT_WND_FACTORY( KGCGuildJoinManageDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildJoinManageDlg, "gc_guild_join_manage_dlg" );

KGCGuildJoinManageDlg::KGCGuildJoinManageDlg(void)
:m_pkBtnJoinAllow(NULL)
,m_pkBtnJoinReject(NULL)
,m_pkBtnCancel(NULL)
,m_pkStaticJoinCondition(NULL)
,m_pkStatinJoinWaitingUserNum(NULL)
,m_pkScrollBar(NULL)
,m_eAlignType(ABT_BY_APPLICATION_DATE)
,m_pkHoverSignBox(NULL)
,m_pkListBackground(NULL)
,m_fOriginYPos(0.f)
,m_fRealYPos(0.f)
,m_iPrevUserListIndex(-1)
{
	LINK_CONTROL("button_join_allow",				m_pkBtnJoinAllow );
    LINK_CONTROL("button_join_reject",				m_pkBtnJoinReject );
    LINK_CONTROL("button_cancel",					m_pkBtnCancel );

    LINK_CONTROL("static_join_condition",			m_pkStaticJoinCondition );
	LINK_CONTROL("static_join_waiting_user_number",	m_pkStatinJoinWaitingUserNum );

	LINK_CONTROL("scroll",							m_pkScrollBar );
	LINK_CONTROL("bg_list_hover",					m_pkHoverSignBox );
	LINK_CONTROL("background_person_3",				m_pkListBackground );

	char str[MAX_PATH] = {0,};
	for ( int i = 0; i < MAX_WAITING_USER_NUM; ++i )
	{
		m_pkWaitingUserInfo[i] = NULL;
		sprintf_s( str, "guild_join_waiting_user_info%d", i );
		LINK_CONTROL( str, m_pkWaitingUserInfo[i] );

		m_pkCheckBox[i] = NULL;
		sprintf_s( str, "checkbox_box%d", i );
		LINK_CONTROL(str, m_pkCheckBox[i] );

		m_pkCheckBoxSign[i] = NULL;
		sprintf_s( str, "checkbox_check%d", i );
		LINK_CONTROL(str, m_pkCheckBoxSign[i] );
	}

	for( int i = 0; i < ABT_MAX; ++i )
	{
		m_pkBtnSorting[i] = NULL;
		sprintf_s( str, "btn_sorting%d", i );
		LINK_CONTROL( str, m_pkBtnSorting[i] );

		m_pkSortingUp[i] = NULL;
		sprintf_s( str, "btn_sort_up%d", i );
		LINK_CONTROL( str, m_pkSortingUp[i] );

		m_pkSortingDown[i] = NULL;
		sprintf_s( str, "btn_sort_down%d", i );
		LINK_CONTROL( str, m_pkSortingDown[i] );
	}

	m_vecJoinWaitingUserInfo.clear();

	m_mapSortFunc.insert( std::map< ALIGN_BY_TYPE, 
		std::pair< SORTFUNC, bool> >::value_type( ABT_BY_GRADE, std::make_pair( KGCGuildJoinManageDlg::SortByGrade, true )) );
	m_mapSortFunc.insert( std::map< ALIGN_BY_TYPE, 
		std::pair< SORTFUNC, bool> >::value_type( ABT_BY_NICKNAME,      std::make_pair( KGCGuildJoinManageDlg::SortByNickName, true ) ) );
	m_mapSortFunc.insert( std::map< ALIGN_BY_TYPE, 
		std::pair< SORTFUNC, bool> >::value_type( ABT_BY_APPLICATION_DATE,   std::make_pair( KGCGuildJoinManageDlg::SortByApplicationDate, false)) );
}

KGCGuildJoinManageDlg::~KGCGuildJoinManageDlg(void)
{
}


void KGCGuildJoinManageDlg::OnCreate( void )
{  
	m_pkScrollBar->InitState(true, true, this);
	m_pkBtnJoinAllow->InitState(true, true, this);
	m_pkBtnJoinReject->InitState(true, true, this);
	m_pkBtnCancel->InitState(true, true, this);	

	m_pkStaticJoinCondition->InitState(true);
	m_pkStaticJoinCondition->SetAlign(DT_LEFT);
	m_pkStatinJoinWaitingUserNum->InitState(true);
	m_pkStatinJoinWaitingUserNum->SetAlign(DT_RIGHT);

	m_pkHoverSignBox->InitState(false);
	m_pkListBackground->InitState(true);

	for ( int i = 0; i < MAX_WAITING_USER_NUM; ++i )
	{
		m_pkWaitingUserInfo[i]->InitState(false, true, this);
		m_pkCheckBox[i]->InitState(true, true, this);
		m_pkCheckBoxSign[i]->InitState(false);
	}

	for ( int i = 0; i < ABT_MAX; ++i )
	{
		m_pkBtnSorting[i]->InitState(true, true, this);
		m_pkSortingUp[i]->InitState(false, true, this);
		m_pkSortingDown[i]->InitState(false, true, this);
	}

	m_fOriginYPos = m_pkHoverSignBox->GetFixedWindowLocalPos().y;
}

void KGCGuildJoinManageDlg::OnCreateComplete( void )
{

}

void KGCGuildJoinManageDlg::ActionPerformed( const KActionEvent& event )
{
	if( m_vecJoinWaitingUserInfo.empty() )
		return;

	GCWND_MSG_MAP( m_pkBtnJoinAllow,		KD3DWnd::D3DWE_BUTTON_CLICK, OnJoinAllow );
	GCWND_MSG_MAP( m_pkBtnJoinReject,		KD3DWnd::D3DWE_BUTTON_CLICK, OnJoinReject );
	GCWND_MSG_MAP( m_pkBtnCancel,			KD3DWnd::D3DWE_BUTTON_CLICK, OnCancel );
	GCWND_MAP( m_pkScrollBar, OnScroll );

	for(int i = 0 ; i < ABT_MAX; ++i )
	{
		if( event.m_pWnd == m_pkBtnSorting[i] && ( D3DWE_BUTTON_CLICK == event.m_dwCode ||  D3DWE_DOUBLE_CLICK == event.m_dwCode ) )
		{
			if( (ALIGN_BY_TYPE)i == ABT_BY_GRADE )
				continue;

			OnSortBtnClick( static_cast< ALIGN_BY_TYPE >(i) );
			g_KDSound.Play( "31" );
			return;
		}
	}
}

void KGCGuildJoinManageDlg::OnDestroyComplete( void )
{
	m_pkHoverSignBox->SetWindowPosY(m_fOriginYPos);
	m_vecJoinWaitingUserInfo.clear();
}

void KGCGuildJoinManageDlg::FrameMoveInEnabledState( void )
{
	if( m_vecJoinWaitingUserInfo.empty() )
		return; 

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x, (float)pt.y );	

	m_pkHoverSignBox->ToggleRender(false);

	int iMaxWaitingUser = MAX_WAITING_USER_NUM;
	if( (int)m_vecJoinWaitingUserInfo.size() < MAX_WAITING_USER_NUM )
		iMaxWaitingUser = (int)m_vecJoinWaitingUserInfo.size();

	for( int i = 0; i < iMaxWaitingUser; i++ )
	{
		float fOffsetY;

		if( m_pkCheckBox[i]->CheckPosInWindowBound(vMousePos) )
		{
			if( g_pkInput->BtnUp( KInput::MBLEFT ) )
			{
				int iMemberRealindex = m_pkScrollBar->GetScrollPos();
				int iCurMemberIndex = i + iMemberRealindex;
				bool bRenderCheck = false;

				if( m_pkCheckBoxSign[i]->IsRenderOn() )
				{
					m_pkCheckBoxSign[i]->ToggleRender(false);
					bRenderCheck = false;				
				}
				else
				{
					m_pkCheckBoxSign[i]->ToggleRender(true);
					bRenderCheck = true;
				}

				if( iCurMemberIndex < (int)m_vecJoinWaitingUserInfo.size() )
					m_vecJoinWaitingUserInfo[iCurMemberIndex].m_bSelectCheckBox = bRenderCheck;

				continue;
			}
		}
		
		if( m_pkWaitingUserInfo[i]->CheckPosInWindowBound(vMousePos) )
		{
			fOffsetY = m_fRealYPos + i * m_pkHoverSignBox->GetHeight();
			m_pkHoverSignBox->SetWindowPosY(fOffsetY);
			m_pkHoverSignBox->ToggleRender(true);

			

			if( g_pkInput->BtnUp( KInput::MBLEFT ) )
			{
				int iMemberRealindex = m_pkScrollBar->GetScrollPos();
				int iCurMemberIndex = i + iMemberRealindex;
				bool bRenderCheck = false;

				for( int j = 0; j < (int)m_pkWaitingUserInfo.size(); j++ )
				{
					if( m_iPrevUserListIndex != iCurMemberIndex )
						m_pkWaitingUserInfo[j]->SetSelectCursorBox(false);
				}

				if( m_iPrevUserListIndex != -1 && m_iPrevUserListIndex != iCurMemberIndex )
					m_vecJoinWaitingUserInfo[m_iPrevUserListIndex].m_bSelectList = false;				

				if( m_pkWaitingUserInfo[i]->IsSelectListBoxRender() )
				{
					m_pkWaitingUserInfo[i]->SetSelectCursorBox(false);
					bRenderCheck = false;				
				}
				else
				{
					m_pkWaitingUserInfo[i]->SetSelectCursorBox(true);
					bRenderCheck = true;
				}

				if( iCurMemberIndex < (int)m_vecJoinWaitingUserInfo.size() )
				{					 
					if(m_vecJoinWaitingUserInfo[iCurMemberIndex].m_bSelectList == false )
					{
						g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_INTRODUCE_EDIT, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );
						//리스트 클릭 시 소개를 보여준다.
						g_pkUIScene->m_pkGuildIntroduceEdit->SetCommonEditType( KGCCommonEditBox::ECEB_MY_INTRODUCE );
						g_pkUIScene->m_pkGuildIntroduceEdit->SetEditBoxLimitText(NEW_GUILD_MY_COMMENT_LIMIT);
						g_pkUIScene->m_pkGuildIntroduceEdit->SetTextColor(D3DCOLOR_RGBA(255,255,255,255));
						g_pkUIScene->m_pkGuildIntroduceEdit->SetDefaultText(m_vecJoinWaitingUserInfo[iCurMemberIndex].m_sJoinUserInfo.m_strMyComment);
						g_pkUIScene->m_pkGuildIntroduceEdit->SetTitleText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MY_COMMENT), 
							DT_CENTER, D3DCOLOR_RGBA(184,134,11,255));

						if( m_vecJoinWaitingUserInfo[iCurMemberIndex].m_sJoinUserInfo.m_dwUserUID == g_kGlobalValue.m_kGuildUserInfo.m_dwUserUID )
							g_pkUIScene->m_pkGuildIntroduceEdit->RenderEditBtn(true);
						else
							g_pkUIScene->m_pkGuildIntroduceEdit->RenderEditBtn(false);
					}

                    m_vecJoinWaitingUserInfo[iCurMemberIndex].m_bSelectList = bRenderCheck;
				}
				m_iPrevUserListIndex = iCurMemberIndex;
			}
		}
	}

	if( m_pkListBackground->CheckPosInWindowBound(vMousePos) )
	{
		if( g_pkInput->ISWheelUp() )
		{
			if( m_pkScrollBar->GetScrollPos() > 0 )
			{
				m_pkScrollBar->SetScrollPos(m_pkScrollBar->GetScrollPos() - 1);
				OnScroll();
				g_KDSound.Play( "73" );
			}
		}
		else if( g_pkInput->ISWheelDown() )
		{
			//if( m_pkScrollBar->GetScrollPos() + m_iMaxMemberNum - 1 < m_vecShowGuildUserList.size() )
			if( m_pkScrollBar->GetScrollPos() < m_pkScrollBar->GetScrollRangeMax() )
			{
				m_pkScrollBar->SetScrollPos(m_pkScrollBar->GetScrollPos() + 1);
				OnScroll();
				g_KDSound.Play( "73" );
			}
		}	
	}
}

void KGCGuildJoinManageDlg::PostChildDraw( void )
{
}

int KGCGuildJoinManageDlg::GetShowWaitingUserCount(int iMemberIndex_)
{
	int iViewMemberCount = static_cast<int>( m_vecJoinWaitingUserInfo.size() ) - iMemberIndex_;

	if(iViewMemberCount < MAX_WAITING_USER_NUM)
		return iViewMemberCount;

	return MAX_WAITING_USER_NUM;
}

void KGCGuildJoinManageDlg::OnSortWaitingUser( SORTFUNC pFunc_ , bool bAscending_ )
{
	std::sort( m_vecJoinWaitingUserInfo.begin(), m_vecJoinWaitingUserInfo.end(), boost::bind( pFunc_, _1, _2, bAscending_ ) );
}

void KGCGuildJoinManageDlg::OnSortBtnClick( ALIGN_BY_TYPE eAlignType_, bool bOpenDlg_ )
{
	m_eAlignType = eAlignType_;

	std::map< ALIGN_BY_TYPE, std::pair< SORTFUNC, bool> >::iterator mit = m_mapSortFunc.find( m_eAlignType );

	if( m_mapSortFunc.end() != mit )
	{        
		if( !bOpenDlg_ )
		{
			OnSortWaitingUser( mit->second.first, mit->second.second );
			ToggleRenderSortArrow(eAlignType_, mit->second.second);
		}
		else
		{
			OnSortWaitingUser( mit->second.first, bOpenDlg_ );
			ToggleRenderSortArrow( eAlignType_, bOpenDlg_ );
		}
		
		InitScroll();
		OnScroll();

		if( mit->second.second )
			mit->second.second = false;
		else
			mit->second.second = true;
	}	
}

void KGCGuildJoinManageDlg::UpdateWaitingUserInfo()
{
	InitScroll();

	std::map< ALIGN_BY_TYPE, std::pair< SORTFUNC, bool> >::iterator mit = m_mapSortFunc.find( m_eAlignType );

	if( m_mapSortFunc.end() != mit )
	{
		OnSortWaitingUser( mit->second.first, mit->second.second );
		OnScroll();
	}
}

void KGCGuildJoinManageDlg::InitScroll( void )
{
	m_pkScrollBar->SetScrollPos( 0 );
	m_pkScrollBar->SetScrollRangeMin( 0 );
	m_pkScrollBar->SetScrollPageSize( 1 );

	int iFileCount = static_cast<int>( m_vecJoinWaitingUserInfo.size() );
	int iScrollMax = 0;
	int iCount =  iFileCount - MAX_WAITING_USER_NUM;

	if(iCount > 0)
	{
		iScrollMax = iCount;
	}

	m_pkScrollBar->SetScrollRangeMax( iScrollMax + 1 );
}

void KGCGuildJoinManageDlg::OnScroll(void)
{
	int iMemberRealindex = m_pkScrollBar->GetScrollPos();
	int iMaxMemberView = GetShowWaitingUserCount( iMemberRealindex );

	for(int i = 0; i < iMaxMemberView; ++i )
	{
		int iMemberIndex = i + iMemberRealindex;

		if( 0 <= iMemberIndex && iMemberIndex < (int)m_vecJoinWaitingUserInfo.size() )
		{
			m_pkWaitingUserInfo[i]->SetJoinWaitingUserInfo( m_vecJoinWaitingUserInfo[iMemberIndex].m_sJoinUserInfo );
			m_pkWaitingUserInfo[i]->ToggleRender( true );

			if( m_vecJoinWaitingUserInfo[iMemberIndex].m_bSelectCheckBox )
				m_pkCheckBoxSign[i]->ToggleRender(true);
			else
				m_pkCheckBoxSign[i]->ToggleRender(false);

			if( m_vecJoinWaitingUserInfo[iMemberIndex].m_bSelectList )
				m_pkWaitingUserInfo[i]->SetSelectCursorBox(true);
			else
				m_pkWaitingUserInfo[i]->SetSelectCursorBox(false);
		}
	}

	for(int i = iMaxMemberView; i < MAX_WAITING_USER_NUM; ++i)
	{
		m_pkWaitingUserInfo[i]->ToggleRender( false );
	}
}

void KGCGuildJoinManageDlg::OnJoinAllow(void)
{
	KP2P::GetInstance()->Send_AcceptGuildJoinerReq(GetCheckUserSet());
	g_KDSound.Play( "31" );
}

void KGCGuildJoinManageDlg::OnJoinReject(void)
{
	KP2P::GetInstance()->Send_RejectGuildJoinerReq(GetCheckUserSet());
	g_KDSound.Play( "31" );
}

SET_DWORD KGCGuildJoinManageDlg::GetCheckUserSet()
{
	SET_DWORD setUser;
	setUser.clear();
	for( int i = 0; i < (int)m_vecJoinWaitingUserInfo.size(); i++ )
	{
		if( m_vecJoinWaitingUserInfo[i].m_bSelectCheckBox )
			setUser.insert(m_vecJoinWaitingUserInfo[i].m_sJoinUserInfo.m_dwUserUID);
	}
	return setUser;
}

void KGCGuildJoinManageDlg::OnCancel(void)
{
	for( int i = 0; i < (int)m_vecJoinWaitingUserInfo.size(); i++ )
		m_vecJoinWaitingUserInfo[i].m_bSelectCheckBox = false;
	OnScroll();
	g_KDSound.Play( "31" );
}

void KGCGuildJoinManageDlg::ToggleRenderSortArrow( ALIGN_BY_TYPE eWhatSorting_, bool bUpSorting_ )
{
	for( int i = 0; i < ABT_MAX; i++ )
	{		
		m_pkSortingUp[i]->ToggleRender(false);
		m_pkSortingDown[i]->ToggleRender(false);
	}

	if( eWhatSorting_ < ABT_MAX )
	{
		m_pkSortingUp[(int)eWhatSorting_]->ToggleRender(bUpSorting_);
		m_pkSortingDown[(int)eWhatSorting_]->ToggleRender(!bUpSorting_);
	}	
}

void KGCGuildJoinManageDlg::SetJoinWaitingUserInfo( std::map<DWORD, KNGuildUserInfo>& mapJoinWaitingUserInfo_ )
{
	m_vecJoinWaitingUserInfo.clear();
	std::map<DWORD, KNGuildUserInfo >::iterator mit;
	for( mit = mapJoinWaitingUserInfo_.begin(); mit != mapJoinWaitingUserInfo_.end(); ++mit )
	{
		KWaitingJoinUser sJoinUserInfo;	
		if( mit->second.m_cMemberLevel == KNGuildUserInfo::GL_JOINER )
		{
			sJoinUserInfo.m_sJoinUserInfo = mit->second;
			sJoinUserInfo.m_bSelectCheckBox = false;
			sJoinUserInfo.m_bSelectList = false;
			m_vecJoinWaitingUserInfo.push_back(sJoinUserInfo);
		}
	}

	if( g_kGlobalValue.m_kGuildInfo.m_ucJoinSetting == KNGuildInfo::JS_AUTO_ACCEPT )
		m_pkStaticJoinCondition->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_DIRECTLY));
	else if( g_kGlobalValue.m_kGuildInfo.m_ucJoinSetting == KNGuildInfo::JS_AFTER_DECISION )
		m_pkStaticJoinCondition->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_AFTER));
	else
		m_pkStaticJoinCondition->SetText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_JOIN_STOP));

	m_pkStatinJoinWaitingUserNum->SetText(g_pkStrLoader->GetReplacedString(STR_ID_NEW_GUILD_WAITING_JOINER_NUM, "i", m_vecJoinWaitingUserInfo.size()));

	for( int i = 0; i < MAX_WAITING_USER_NUM; i++ )
		m_pkCheckBoxSign[i]->ToggleRender(false);

	OnSortBtnClick(ABT_BY_APPLICATION_DATE, true);
}

void KGCGuildJoinManageDlg::SetCursorBoxOffset(D3DXVECTOR2 vOffset_ )
{
	m_fRealYPos = m_fOriginYPos + vOffset_.y;
}

bool KGCGuildJoinManageDlg::SortByNickName( const KWaitingJoinUser& lhs, const KWaitingJoinUser& rhs, bool bAscending_)
{
	if(bAscending_)
	{
		if( lhs.m_sJoinUserInfo.m_strNickName == rhs.m_sJoinUserInfo.m_strNickName)
		{
			return lhs.m_sJoinUserInfo.m_cMemberLevel > rhs.m_sJoinUserInfo.m_cMemberLevel;
		}
		else
		{
			return lhs.m_sJoinUserInfo.m_strNickName > rhs.m_sJoinUserInfo.m_strNickName;
		}
	}
	else
	{
		if( lhs.m_sJoinUserInfo.m_strNickName == rhs.m_sJoinUserInfo.m_strNickName)
		{
			return lhs.m_sJoinUserInfo.m_cMemberLevel < rhs.m_sJoinUserInfo.m_cMemberLevel;
		}
		else
		{
			return lhs.m_sJoinUserInfo.m_strNickName < rhs.m_sJoinUserInfo.m_strNickName;
		}
	}
}

bool KGCGuildJoinManageDlg::SortByGrade( const KWaitingJoinUser& lhs, const KWaitingJoinUser& rhs, bool bAscending_)
{
	if(bAscending_)
	{
		if( lhs.m_sJoinUserInfo.m_ucCharacterGrade == rhs.m_sJoinUserInfo.m_ucCharacterGrade)
		{
			return lhs.m_sJoinUserInfo.m_strNickName > rhs.m_sJoinUserInfo.m_strNickName;
		}
		else
		{
			return lhs.m_sJoinUserInfo.m_ucCharacterGrade > rhs.m_sJoinUserInfo.m_ucCharacterGrade;
		}
	}
	else
	{
		if( lhs.m_sJoinUserInfo.m_ucCharacterGrade == rhs.m_sJoinUserInfo.m_ucCharacterGrade)
		{
			return lhs.m_sJoinUserInfo.m_strNickName < rhs.m_sJoinUserInfo.m_strNickName;
		}
		else
		{
			return lhs.m_sJoinUserInfo.m_ucCharacterGrade < rhs.m_sJoinUserInfo.m_ucCharacterGrade;
		}
	}
}

bool KGCGuildJoinManageDlg::SortByApplicationDate( const KWaitingJoinUser& lhs, const KWaitingJoinUser& rhs, bool bAscending_)
{
	if(bAscending_)
	{
		if( lhs.m_sJoinUserInfo.m_kLastLoginDate.m_sYear == rhs.m_sJoinUserInfo.m_kLastLoginDate.m_sYear )
		{
			if( lhs.m_sJoinUserInfo.m_kLastLoginDate.m_cMonth == rhs.m_sJoinUserInfo.m_kLastLoginDate.m_cMonth )
			{
				if( lhs.m_sJoinUserInfo.m_kLastLoginDate.m_cDay == rhs.m_sJoinUserInfo.m_kLastLoginDate.m_cDay )
					return lhs.m_sJoinUserInfo.m_strNickName > rhs.m_sJoinUserInfo.m_strNickName;
				else
					return lhs.m_sJoinUserInfo.m_kLastLoginDate.m_cDay > rhs.m_sJoinUserInfo.m_kLastLoginDate.m_cDay;
			}
			else
			{
				return lhs.m_sJoinUserInfo.m_kLastLoginDate.m_cMonth > rhs.m_sJoinUserInfo.m_kLastLoginDate.m_cMonth;
			}
		}
		else
		{
			return lhs.m_sJoinUserInfo.m_kLastLoginDate.m_sYear > rhs.m_sJoinUserInfo.m_kLastLoginDate.m_sYear;
		}
	}
	else
	{
		if( lhs.m_sJoinUserInfo.m_kLastLoginDate.m_sYear == rhs.m_sJoinUserInfo.m_kLastLoginDate.m_sYear )
		{
			if( lhs.m_sJoinUserInfo.m_kLastLoginDate.m_cMonth == rhs.m_sJoinUserInfo.m_kLastLoginDate.m_cMonth )
			{
				if( lhs.m_sJoinUserInfo.m_kLastLoginDate.m_cDay == rhs.m_sJoinUserInfo.m_kLastLoginDate.m_cDay )
					return lhs.m_sJoinUserInfo.m_strNickName < rhs.m_sJoinUserInfo.m_strNickName;
				else
					return lhs.m_sJoinUserInfo.m_kLastLoginDate.m_cDay < rhs.m_sJoinUserInfo.m_kLastLoginDate.m_cDay;
			}
			else
			{
				return lhs.m_sJoinUserInfo.m_kLastLoginDate.m_cMonth < rhs.m_sJoinUserInfo.m_kLastLoginDate.m_cMonth;
			}
		}
		else
		{
			return lhs.m_sJoinUserInfo.m_kLastLoginDate.m_sYear < rhs.m_sJoinUserInfo.m_kLastLoginDate.m_sYear;
		}
	}
}
