#include "StdAfx.h"
#include "KGCGuildMemberList.h"
#include "KGCGuildMemberInfo.h"
#include "gcui/KGCCheckButton.h"

IMPLEMENT_CLASSNAME( KGCGuildMemberListDlg );
IMPLEMENT_WND_FACTORY( KGCGuildMemberListDlg );
IMPLEMENT_WND_FACTORY_NAME( KGCGuildMemberListDlg, "gc_guild_member_list" );

KGCGuildMemberListDlg::KGCGuildMemberListDlg(void)
:m_pkScrollBar(NULL)
,m_pkListBackground(NULL)
,m_iMaxMemberNum((int)MAX_MEMBER_NUM)
,m_bUsePopup(false)
,m_pkHoverSignBox(NULL)
,m_pkListSubMenu(NULL)
,m_fOriginYPos(0.0f)
,m_fRealYPos(0.0f)
,m_iPrevUserListIndex(-1)
,m_bClickSubMenu(false)
{
	LINK_CONTROL("scroll",				m_pkScrollBar );

	LINK_CONTROL("bg_list_hover",		m_pkHoverSignBox );
	LINK_CONTROL("background_person_3",	m_pkListBackground );
	LINK_CONTROL("guild_list_sub_menu", m_pkListSubMenu);

    char str[MAX_PATH] = {0,};
    for ( int i = 0; i < m_iMaxMemberNum; ++i )
    {
        m_pkGuildMemberInfo[i] = NULL;
        sprintf_s( str, "guild_member_info%d", i );
        LINK_CONTROL( str, m_pkGuildMemberInfo[i] );
    }

	for( int i = 0; i < ABT_MAX; i++ )
	{
		m_pkSortingArrowUp[i] = NULL;
		sprintf_s( str, "btn_sort_up%d", i );
		LINK_CONTROL( str, m_pkSortingArrowUp[i] );

		m_pkSortingArrowDown[i] = NULL;
		sprintf_s( str, "btn_sort_down%d", i );
		LINK_CONTROL( str, m_pkSortingArrowDown[i] );

		m_pkBtnSorting[i] = NULL;
		sprintf_s( str, "btn_sorting%d", i );
		LINK_CONTROL( str, m_pkBtnSorting[i] );		
	}

    m_vecShowGuildUserList.clear();


    m_eAlignType = ABT_BY_GUILDGRADE;

    m_mapSortFunc.insert( std::map< ALIGN_BY_TYPE, 
		std::pair< SORTFUNC, bool> >::value_type( ABT_BY_GUILDGRADE, std::make_pair( KGCGuildMemberListDlg::SortByGuildGrade, true )) );
    m_mapSortFunc.insert( std::map< ALIGN_BY_TYPE, 
		std::pair< SORTFUNC, bool> >::value_type( ABT_BY_GRADE,      std::make_pair( KGCGuildMemberListDlg::SortByGrade, true ) ) );
    m_mapSortFunc.insert( std::map< ALIGN_BY_TYPE, 
		std::pair< SORTFUNC, bool> >::value_type( ABT_BY_NICKNAME,   std::make_pair( KGCGuildMemberListDlg::SortByNickName, true)) );
	m_mapSortFunc.insert( std::map< ALIGN_BY_TYPE, 
		std::pair< SORTFUNC, bool> >::value_type( ABT_BY_ACTIVITY,   std::make_pair( KGCGuildMemberListDlg::SortByGuildActivity, true)) );
	m_mapSortFunc.insert( std::map< ALIGN_BY_TYPE, 
		std::pair< SORTFUNC, bool> >::value_type( ABT_BY_LAST_LOGIN, std::make_pair( KGCGuildMemberListDlg::SortByGuildLastLogin, true)) );
}

KGCGuildMemberListDlg::~KGCGuildMemberListDlg(void)
{
    m_vecShowGuildUserList.clear();
}


void KGCGuildMemberListDlg::OnCreate( void )
{
    m_pkScrollBar->InitState(true, true, this );
	m_pkHoverSignBox->InitState(false);
	m_pkListBackground->InitState(true);
	m_pkListSubMenu->InitState(false, true, this);
	m_pkListSubMenu->ClearMenu();
	m_pkListSubMenu->AddMenu( 1, g_pkStrLoader->GetString(STR_ID_NEW_GUILD_DRUM_OUT));
	
	for( int i = 0; i < ABT_MAX; i++ )
	{
		m_pkSortingArrowDown[i]->InitState(false, true, this);
		m_pkSortingArrowUp[i]->InitState(false, true, this);
		m_pkBtnSorting[i]->InitState( true, true, this );
	}

	for ( int i = 0; i < m_iMaxMemberNum; ++i )
	{
		m_pkGuildMemberInfo[i]->InitState( false, true, this );
	}

	m_fOriginYPos = m_pkHoverSignBox->GetFixedWindowLocalPos().y;
}

void KGCGuildMemberListDlg::OnCreateComplete( void )
{	
}

void KGCGuildMemberListDlg::ActionPerformed( const KActionEvent& event )
{
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

	if( event.m_pWnd == m_pkListSubMenu && event.m_dwCode == KGCMultiMenu::K3DWE_MENU_CLICKED )
	{
		switch( event.m_dwlParam )
		{
		case ELIST_DRUM_OUT:			
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_PUBLIC_MBOX, 
				g_pkStrLoader->GetString(STR_ID_NEW_GUILD_DRUM_OUT), g_pkStrLoader->GetString(STR_ID_NEW_GUILD_BREAK_UP_REQ), 
				KGCUIScene::GC_MBOX_GUILD_BREAK_UP, KGCGuildPublicMbox::MBR_CONFIRM_CANCEL, 0, false, true );
			break;
		}
	}
}

void KGCGuildMemberListDlg::FrameMoveInEnabledState( void )
{
    if( m_vecShowGuildUserList.empty() )
        return;

	POINT pt = g_pkInput->GetMousePos();
	D3DXVECTOR2 vMousePos( (float)pt.x , (float)pt.y );	

	m_pkHoverSignBox->ToggleRender(false);
	for( int i = 0; i < m_iMaxMemberNum; i++ )
	{
		float fOffsetY;
		if( m_pkGuildMemberInfo[i]->CheckPosInWindowBound(vMousePos) )
		{
			fOffsetY = m_fRealYPos + i * m_pkHoverSignBox->GetHeight();
			m_pkHoverSignBox->SetWindowPosY(fOffsetY);
			m_pkHoverSignBox->ToggleRender(true);

			//서브메뉴를 띄우자!
			if( g_pkInput->BtnUp( KInput::MBRIGHT ) && !m_bUsePopup )
			{
				int iMemberRealindex = m_pkScrollBar->GetScrollPos();
				int iCurMemberIndex = i + iMemberRealindex;
				m_pkListSubMenu->ToggleRender(false);
				m_bClickSubMenu = false;

                if( iCurMemberIndex < (int)m_vecShowGuildUserList.size() && iCurMemberIndex > 0 )
                {
				    //길드마스터는 길드탈퇴 서브메뉴를 사용하지 않겠다
				    if( m_vecShowGuildUserList[iCurMemberIndex].m_sGuildMemberInfo.m_dwUserUID == g_kGlobalValue.m_kGuildUserInfo.m_dwUserUID &&
					    m_vecShowGuildUserList[iCurMemberIndex].m_sGuildMemberInfo.m_cMemberLevel != KNGuildUserInfo::GL_MASTER )
				    {
					    m_pkListSubMenu->SetFixedWindowLocalPos(D3DXVECTOR2(vMousePos.x / m_fWindowScaleX / GC_SCREEN_DIV_WIDTH,vMousePos.y / m_fWindowScaleY / GC_SCREEN_DIV_HEIGHT));
					    m_pkListSubMenu->ToggleRender(true);
					    m_bClickSubMenu = true;
				    }
                }
			}

			//서브메뉴가 띄워져 있으면 아래 동작들은 무시하자.
			if( m_bClickSubMenu )
				continue;

			if( g_pkInput->BtnUp( KInput::MBLEFT ) )
			{
				int iMemberRealindex = m_pkScrollBar->GetScrollPos();
				int iCurMemberIndex = i + iMemberRealindex;
				bool bRenderCheck = false;				

				for( int j = 0; j < m_iMaxMemberNum; j++ )
				{
					if( m_iPrevUserListIndex != iCurMemberIndex )
					{
						m_pkGuildMemberInfo[j]->SetSelectCursorBox(false);						
					}
				}
	
				// 이전에 클릭되었던 팝업메뉴는 랜더를 꺼준다.
				if( m_iPrevUserListIndex - iMemberRealindex >= 0 && m_iPrevUserListIndex - iMemberRealindex < m_iMaxMemberNum && iCurMemberIndex != m_iPrevUserListIndex)
					m_pkGuildMemberInfo[m_iPrevUserListIndex - iMemberRealindex]->SetRenderPopupMenu(false);

				if( m_iPrevUserListIndex != -1 && m_iPrevUserListIndex != iCurMemberIndex && m_iPrevUserListIndex < (int)m_vecShowGuildUserList.size() )
					m_vecShowGuildUserList[m_iPrevUserListIndex].m_bSelectList = false;

				if( m_pkGuildMemberInfo[i]->IsSelectListBoxRender() )
				{
					m_pkGuildMemberInfo[i]->SetSelectCursorBox(false);					
					bRenderCheck = false;				
				}
				else
				{
					m_pkGuildMemberInfo[i]->SetSelectCursorBox(true);
					bRenderCheck = true;
				}

				if( iCurMemberIndex < (int)m_vecShowGuildUserList.size() && iCurMemberIndex >= 0 )
				{							
					if( !m_bUsePopup && m_vecShowGuildUserList[iCurMemberIndex].m_bSelectList == false )
					{
						g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_GUILD_INTRODUCE_EDIT, L"", L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true );						

						g_pkUIScene->m_pkGuildIntroduceEdit->SetCommonEditType( KGCCommonEditBox::ECEB_MY_INTRODUCE );
						g_pkUIScene->m_pkGuildIntroduceEdit->SetEditBoxLimitText(NEW_GUILD_MY_COMMENT_LIMIT);
						g_pkUIScene->m_pkGuildIntroduceEdit->SetTextColor(D3DCOLOR_RGBA(255,255,255,255));
						g_pkUIScene->m_pkGuildIntroduceEdit->SetDefaultText(m_vecShowGuildUserList[iCurMemberIndex].m_sGuildMemberInfo.m_strMyComment);
						g_pkUIScene->m_pkGuildIntroduceEdit->SetTitleText(g_pkStrLoader->GetString(STR_ID_NEW_GUILD_MY_COMMENT), 
							DT_CENTER, D3DCOLOR_RGBA(184,134,11,255));						

						if( m_vecShowGuildUserList[iCurMemberIndex].m_sGuildMemberInfo.m_dwUserUID == g_kGlobalValue.m_kGuildUserInfo.m_dwUserUID )
							g_pkUIScene->m_pkGuildIntroduceEdit->RenderEditBtn(true);
						else
							g_pkUIScene->m_pkGuildIntroduceEdit->RenderEditBtn(false);
					}
                    m_vecShowGuildUserList[iCurMemberIndex].m_bSelectList = bRenderCheck;
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
			if( m_pkScrollBar->GetScrollPos() < m_pkScrollBar->GetScrollRangeMax() )
			{
				m_pkScrollBar->SetScrollPos(m_pkScrollBar->GetScrollPos() + 1);
				OnScroll();
				g_KDSound.Play( "73" );
			}
		}	
	}
}

void KGCGuildMemberListDlg::OnDestroyComplete( void )
{
	m_pkHoverSignBox->SetWindowPosY(m_fOriginYPos);
	m_vecShowGuildUserList.clear();
}

void KGCGuildMemberListDlg::PostChildDraw( void )
{
}

int KGCGuildMemberListDlg::GetMemberViewCount(int iMemberIndex_)
{
    int iViewMemberCount = static_cast<int>(m_vecShowGuildUserList.size() ) - iMemberIndex_;

    if(iViewMemberCount < m_iMaxMemberNum)
        return iViewMemberCount;

    return m_iMaxMemberNum;
}

void KGCGuildMemberListDlg::InitScroll( void )
{
    m_pkScrollBar->SetScrollPos( 0 );
    m_pkScrollBar->SetScrollRangeMin( 0 );
    m_pkScrollBar->SetScrollPageSize( 1 );

    int iFileCount = static_cast<int>( m_vecShowGuildUserList.size() );
    int iScrollMax = 0;
    int iCount =  iFileCount - m_iMaxMemberNum;

    if(iCount > 0)
    {
        iScrollMax = iCount;
    }

    m_pkScrollBar->SetScrollRangeMax( iScrollMax + 1 );
}

void KGCGuildMemberListDlg::OnScroll()
{
    int iMemberRealindex = m_pkScrollBar->GetScrollPos();
    int iMaxMemberView = GetMemberViewCount( iMemberRealindex );

    for(int i = 0; i < iMaxMemberView; ++i )
    {
        int iMemberIndex = i + iMemberRealindex;

        if( 0 <= iMemberIndex && iMemberIndex < (int)m_vecShowGuildUserList.size() )
        {		
			m_pkGuildMemberInfo[i]->UsePopUpMenu(m_bUsePopup);
            m_pkGuildMemberInfo[i]->SetUserInfo( m_vecShowGuildUserList[iMemberIndex].m_sGuildMemberInfo );
            m_pkGuildMemberInfo[i]->ToggleRender( true );

			if( m_vecShowGuildUserList[iMemberIndex].m_bSelectList )
				m_pkGuildMemberInfo[i]->SetSelectCursorBox(true);
			else
			{
				m_pkGuildMemberInfo[i]->SetSelectCursorBox(false);
				m_pkGuildMemberInfo[i]->SetRenderPopupMenu(false);
			}
        }
    }

    for(int i = iMaxMemberView; i < m_iMaxMemberNum; ++i)
    {
        m_pkGuildMemberInfo[i]->ToggleRender( false );
    }	

	m_pkListSubMenu->ToggleRender(false);
}

void KGCGuildMemberListDlg::UpdateList()
{
    InitScroll();

    std::map< ALIGN_BY_TYPE, std::pair< SORTFUNC, bool> >::iterator mit = m_mapSortFunc.find( m_eAlignType );

    if( m_mapSortFunc.end() != mit )
    {
        OnSortGuildMember( mit->second.first, mit->second.second );
        OnScroll();
    }
}

void KGCGuildMemberListDlg::OnSortGuildMember( SORTFUNC pFunc_ , bool bAscending_ )
{
    std::sort( m_vecShowGuildUserList.begin(), m_vecShowGuildUserList.end(), boost::bind( pFunc_, _1, _2, bAscending_ ) );
}

void KGCGuildMemberListDlg::OnSortBtnClick( ALIGN_BY_TYPE eAlignType_, bool bOpenDlg_ )
{
    m_eAlignType = eAlignType_;

    std::map< ALIGN_BY_TYPE, std::pair< SORTFUNC, bool> >::iterator mit = m_mapSortFunc.find( m_eAlignType );

    if( m_mapSortFunc.end() != mit )
    {        
		if( !bOpenDlg_ )
		{
			OnSortGuildMember( mit->second.first, mit->second.second );
			ToggleRenderSortArrow(eAlignType_, mit->second.second);        
		}
		else
		{
			OnSortGuildMember( mit->second.first, true );
			ToggleRenderSortArrow(eAlignType_, true );    
		}

		InitScroll();
		OnScroll();

		if( mit->second.second )
			mit->second.second = false;
		else
			mit->second.second = true;
    }	
}

void KGCGuildMemberListDlg::ToggleRenderSortArrow( ALIGN_BY_TYPE eWhatSorting_, bool bUpSorting_ )
{
	for( int i = 0; i < ABT_MAX; i++ )
	{		
		m_pkSortingArrowUp[i]->ToggleRender(false);
		m_pkSortingArrowDown[i]->ToggleRender(false);
	}

	if( eWhatSorting_ < ABT_MAX )
	{
		m_pkSortingArrowUp[(int)eWhatSorting_]->ToggleRender(bUpSorting_);
		m_pkSortingArrowDown[(int)eWhatSorting_]->ToggleRender(!bUpSorting_);
	}	
}

void KGCGuildMemberListDlg::SetUsePopupMenu(bool bUsePopup_ )
{ 
	m_bUsePopup = bUsePopup_; 
	OnScroll();
}

void KGCGuildMemberListDlg::SetGuildUserInfo() 
{ 
	m_vecShowGuildUserList.clear();	
	std::map<DWORD, KNGuildUserInfo >::iterator mit;
	for( mit = g_kGlobalValue.m_mapNGuildUserInfo.begin(); mit != g_kGlobalValue.m_mapNGuildUserInfo.end(); ++mit )
	{
		KGuildMemberList sGuildUserinfo;	
        if( mit->second.m_cMemberLevel != KNGuildUserInfo::GL_JOINER )
        {
            sGuildUserinfo.m_sGuildMemberInfo = mit->second;
            sGuildUserinfo.m_bSelectList = false;
            m_vecShowGuildUserList.push_back(sGuildUserinfo);
        }
    }

	OnSortBtnClick(ABT_BY_GUILDGRADE, true);        
}

void KGCGuildMemberListDlg::SetCursorBoxOffset(D3DXVECTOR2 vOffset_ )
{
	m_fRealYPos = m_fOriginYPos + vOffset_.y;
}

bool KGCGuildMemberListDlg::SortByNickName( const KGuildMemberList& lhs, const KGuildMemberList& rhs, bool bAscending_)
{
    if(bAscending_)
    {
        if( lhs.m_sGuildMemberInfo.m_strNickName == rhs.m_sGuildMemberInfo.m_strNickName)
        {
            return lhs.m_sGuildMemberInfo.m_cMemberLevel > rhs.m_sGuildMemberInfo.m_cMemberLevel;
        }
        else
        {
            return lhs.m_sGuildMemberInfo.m_strNickName > rhs.m_sGuildMemberInfo.m_strNickName;
        }
    }
    else
    {
        if( lhs.m_sGuildMemberInfo.m_strNickName == rhs.m_sGuildMemberInfo.m_strNickName)
        {
            return lhs.m_sGuildMemberInfo.m_cMemberLevel < rhs.m_sGuildMemberInfo.m_cMemberLevel;
        }
        else
        {
            return lhs.m_sGuildMemberInfo.m_strNickName < rhs.m_sGuildMemberInfo.m_strNickName;
        }
    }
}

bool KGCGuildMemberListDlg::SortByGrade( const KGuildMemberList& lhs, const KGuildMemberList& rhs, bool bAscending_)
{
    if(bAscending_)
    {
        if( lhs.m_sGuildMemberInfo.m_ucCharacterGrade == rhs.m_sGuildMemberInfo.m_ucCharacterGrade)
        {
            return lhs.m_sGuildMemberInfo.m_strNickName > rhs.m_sGuildMemberInfo.m_strNickName;
        }
        else
        {
            return lhs.m_sGuildMemberInfo.m_ucCharacterGrade > rhs.m_sGuildMemberInfo.m_ucCharacterGrade;
        }
    }
    else
    {
        if( lhs.m_sGuildMemberInfo.m_ucCharacterGrade == rhs.m_sGuildMemberInfo.m_ucCharacterGrade)
        {
            return lhs.m_sGuildMemberInfo.m_strNickName < rhs.m_sGuildMemberInfo.m_strNickName;
        }
        else
        {
            return lhs.m_sGuildMemberInfo.m_ucCharacterGrade < rhs.m_sGuildMemberInfo.m_ucCharacterGrade;
        }
    }
}

bool KGCGuildMemberListDlg::SortByGuildGrade( const KGuildMemberList& lhs, const KGuildMemberList& rhs, bool bAscending_)
{
    if(bAscending_)
    {
        if( lhs.m_sGuildMemberInfo.m_cMemberLevel == rhs.m_sGuildMemberInfo.m_cMemberLevel)
        {
            return lhs.m_sGuildMemberInfo.m_strNickName > rhs.m_sGuildMemberInfo.m_strNickName;
        }
        else
        {
            return lhs.m_sGuildMemberInfo.m_cMemberLevel > rhs.m_sGuildMemberInfo.m_cMemberLevel;
        }
    }
    else
    {
        if( lhs.m_sGuildMemberInfo.m_cMemberLevel == rhs.m_sGuildMemberInfo.m_cMemberLevel)
        {
            return lhs.m_sGuildMemberInfo.m_strNickName < rhs.m_sGuildMemberInfo.m_strNickName;
        }
        else
        {
            return lhs.m_sGuildMemberInfo.m_cMemberLevel < rhs.m_sGuildMemberInfo.m_cMemberLevel;
        }
    }
}

bool KGCGuildMemberListDlg::SortByGuildActivity( const KGuildMemberList& lhs, const KGuildMemberList& rhs, bool bAscending_)
{
	if(bAscending_)
	{
		if( lhs.m_sGuildMemberInfo.m_dwContributePoint == rhs.m_sGuildMemberInfo.m_dwContributePoint)
		{
			return lhs.m_sGuildMemberInfo.m_strNickName > rhs.m_sGuildMemberInfo.m_strNickName;
		}
		else
		{
			return lhs.m_sGuildMemberInfo.m_dwContributePoint > rhs.m_sGuildMemberInfo.m_dwContributePoint;
		}
	}
	else
	{
		if( lhs.m_sGuildMemberInfo.m_dwContributePoint == rhs.m_sGuildMemberInfo.m_dwContributePoint)
		{
			return lhs.m_sGuildMemberInfo.m_strNickName < rhs.m_sGuildMemberInfo.m_strNickName;
		}
		else
		{
			return lhs.m_sGuildMemberInfo.m_dwContributePoint < rhs.m_sGuildMemberInfo.m_dwContributePoint;
		}
	}
}

bool KGCGuildMemberListDlg::SortByGuildLastLogin( const KGuildMemberList& lhs, const KGuildMemberList& rhs, bool bAscending_)
{
	if(bAscending_)
	{
		if( lhs.m_sGuildMemberInfo.m_kLastLoginDate.m_sYear == rhs.m_sGuildMemberInfo.m_kLastLoginDate.m_sYear )
		{
			if( lhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cMonth == rhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cMonth )
			{
				if( lhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cDay == rhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cDay )
					return lhs.m_sGuildMemberInfo.m_strNickName > rhs.m_sGuildMemberInfo.m_strNickName;
				else
					return lhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cDay > rhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cDay;
			}
			else
			{
				return lhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cMonth > rhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cMonth;
			}
		}
		else
		{
			return lhs.m_sGuildMemberInfo.m_kLastLoginDate.m_sYear > rhs.m_sGuildMemberInfo.m_kLastLoginDate.m_sYear;
		}
	}
	else
	{
		if( lhs.m_sGuildMemberInfo.m_kLastLoginDate.m_sYear == rhs.m_sGuildMemberInfo.m_kLastLoginDate.m_sYear )
		{
			if( lhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cMonth == rhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cMonth )
			{
				if( lhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cDay == rhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cDay )
					return lhs.m_sGuildMemberInfo.m_strNickName < rhs.m_sGuildMemberInfo.m_strNickName;
				else
					return lhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cDay < rhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cDay;
			}
			else
			{
				return lhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cMonth < rhs.m_sGuildMemberInfo.m_kLastLoginDate.m_cMonth;
			}
		}
		else
		{
			return lhs.m_sGuildMemberInfo.m_kLastLoginDate.m_sYear < rhs.m_sGuildMemberInfo.m_kLastLoginDate.m_sYear;
		}
	}
}

