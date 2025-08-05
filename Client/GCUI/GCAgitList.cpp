#include "stdafx.h"
#include "GCAgitList.h"
#include "GCStateGame.h"

IMPLEMENT_CLASSNAME( KGCAgitList );
IMPLEMENT_WND_FACTORY( KGCAgitList );
IMPLEMENT_WND_FACTORY_NAME( KGCAgitList, "gc_agit_list" );

static const int nREQ_AGITINFO_TIME = 10000;	//	10초
static const int nFAVORITE_MAX_NUM = 20;

KGCAgitList::KGCAgitList( void )
: m_dwBackCnt(0)
, m_apkPageBack(NULL)
, m_pkStaticPageDesc(NULL)
, m_pkChannelCombo( NULL )
{
    m_pkNumUserCombo = NULL;
    m_pkStaticFullOption = NULL;
    m_pkStaticHideOption = NULL;
    m_pkStaticNumUserOption = NULL;
    m_pkStaticPage = NULL;
    LINK_CONTROL( "userOptionCombo",        m_pkNumUserCombo );
	LINK_CONTROL( "ChannelOptionCombo",     m_pkChannelCombo );
    LINK_CONTROL( "staticFullAgitOption",   m_pkStaticFullOption );
    LINK_CONTROL( "staticHideAgitOption",   m_pkStaticHideOption );
    LINK_CONTROL( "staticUserAgitOption",   m_pkStaticNumUserOption );
    LINK_CONTROL( "staticPage",             m_pkStaticPage );

    for( int i=0 ; i<NUM_TAB ; ++i )
        m_apkTab[i] = NULL;

    for( int i=0 ; i<NUM_BTN ; ++i )
        m_apkBtn[i] = NULL;

    for( int i=0 ; i<NUM_CHECK ; ++i )
        m_apkCheck[i] = NULL;

    LINK_CONTROL( "tabAgitListDefault",     m_apkTab[ TAB_AGIT_LIST_DEFAULT ] );
    LINK_CONTROL( "tabAgitListActive",      m_apkTab[ TAB_AGIT_LIST_ACTIVE ] );
    LINK_CONTROL( "tabFriendAgitDefault",   m_apkTab[ TAB_FRIEND_AGIT_DEFAULT ] );
    LINK_CONTROL( "tabFriendAgitActive",    m_apkTab[ TAB_FRIEND_AGIT_ACTIVE ] );
    LINK_CONTROL( "tabLikeAgitDefault",     m_apkTab[ TAB_LIKE_AGIT_DEFAULT ] );
    LINK_CONTROL( "tabLikeAgitActive",      m_apkTab[ TAB_LIKE_AGIT_ACTIVE ] );
    LINK_CONTROL( "tabRankAgitDefault",     m_apkTab[ TAB_RANK_AGIT_DEFAULT ] );
    LINK_CONTROL( "tabRankAgitActive",      m_apkTab[ TAB_RANK_AGIT_ACTIVE ] );

    LINK_CONTROL( "btnFirst",       m_apkBtn[ BTN_FIRST ] );
    LINK_CONTROL( "btnPrev",        m_apkBtn[ BTN_PREV ] );
    LINK_CONTROL( "btnEnd",         m_apkBtn[ BTN_END ] );
    LINK_CONTROL( "btnNext",        m_apkBtn[ BTN_NEXT ] );
    LINK_CONTROL( "btnClose",       m_apkBtn[ BTN_COLSE ] );
    LINK_CONTROL( "btnVisit",       m_apkBtn[ BTN_VISIT ] );
    LINK_CONTROL( "btnDelete",      m_apkBtn[ BTN_DELETE ] );

	

    LINK_CONTROL( "fullAgitCheckbox",   m_apkCheck[ CHECKBOX_FULL_OPTION ] );
    LINK_CONTROL( "hideAgitCheckbox",   m_apkCheck[ CHECKBOX_HIDE_OPTION ] );
    LINK_CONTROL( "fullAgitCheck ",     m_apkCheck[ CHECK_FULL_OPTION ] );
    LINK_CONTROL( "hideAgitCheck",      m_apkCheck[ CHECK_HIDE_OPTION ] );

	
	LINK_CONTROL( "pageBack", m_apkPageBack );


    for( int i=0 ; i<NUM_AGIT_SLOT ; ++i )
    {
        m_apkAgitSlot[i] = NULL;

        char szTemp[128] = { 0, };
        sprintf_s( szTemp, 127, "agitSlot%d", i );
        LINK_CONTROL( szTemp, m_apkAgitSlot[i] );
    }

    m_pkActiveSlotFrame = NULL;
    LINK_CONTROL( "activeFrame", m_pkActiveSlotFrame );

    m_bShowNotFullOnly = false;
    m_bShowOpenOnly = false;
    m_usShowMinUser = 0;
	m_nChannel = 0;
	m_nMaxChannel = 1;

    m_usCurPage = 0;
    m_usMaxPage = 0;
    m_vecRegisterPageInfo.clear();
	m_vecFriendPageInfo.clear();
	m_vecFavoritePageInfo.clear();
	m_vecRankPageInfo.clear();

    m_iCurSlotIndex = -1;
    m_eCurTab = TAB_AGIT_LIST_ACTIVE;

    m_pkTodayRankFrame = NULL;
    m_pkTotalRankFrame = NULL;
    m_pkStaticTodayRank = NULL;
    m_pkStaticTotalRank = NULL;
    LINK_CONTROL( "todayRankFrame", m_pkTodayRankFrame );
    LINK_CONTROL( "totalRankFrame", m_pkTotalRankFrame );
    LINK_CONTROL( "staticTodayRank", m_pkStaticTodayRank );
    LINK_CONTROL( "staticTotalRank", m_pkStaticTotalRank );
    LINK_CONTROL( "staticPageDesc", m_pkStaticPageDesc );
}

KGCAgitList::~KGCAgitList( void )
{
}

void KGCAgitList::OnCreate( void )
{
    m_pkNumUserCombo->InitState( true, true, this );
	m_pkChannelCombo->InitState( true, true, this );

    m_pkStaticFullOption->InitState( true, false, NULL );
    m_pkStaticHideOption->InitState( true, false, NULL );
    m_pkStaticNumUserOption->InitState( true, false, NULL );

    m_pkStaticFullOption->SetAlign( DT_LEFT );
    m_pkStaticHideOption->SetAlign( DT_LEFT );
    m_pkStaticNumUserOption->SetAlign( DT_LEFT );
    m_pkStaticPage->SetAlign( DT_CENTER );

	m_apkPageBack->InitState( true, false, NULL );

    for( int i=0 ; i<NUM_TAB ; ++i )
        m_apkTab[i]->InitState( true, true, this );

    for( int i=0 ; i<NUM_BTN ; ++i )
        m_apkBtn[i]->InitState( true, true, this );

    for( int i=0 ; i<NUM_CHECK ; ++i )
        m_apkCheck[i]->InitState( true, true, this );

    for( int i=0 ; i<NUM_AGIT_SLOT ; ++i )
        m_apkAgitSlot[i]->InitState( true, true, this );

    m_pkActiveSlotFrame->InitState( false, false, NULL );

    m_bShowNotFullOnly = false;
    m_bShowOpenOnly = false;
    m_usShowMinUser = 0;
	m_nChannel = 0;
	m_nMaxChannel = 1;

    m_usCurPage = 0;
    m_usMaxPage = 0;
	m_vecRegisterPageInfo.clear();
	m_vecFriendPageInfo.clear();
	m_vecFavoritePageInfo.clear();
	m_vecRankPageInfo.clear();

    m_iCurSlotIndex = -1;
    m_eCurTab = TAB_AGIT_LIST_ACTIVE;

    m_pkTodayRankFrame->InitState( false, false, NULL );
    m_pkTotalRankFrame->InitState( false, false, NULL );
    m_pkStaticTodayRank->InitState( false, false, NULL );
    m_pkStaticTotalRank->InitState( false, false, NULL );
	m_pkStaticPageDesc->InitState( false, false, NULL );
    m_pkStaticTodayRank->SetAlign( DT_CENTER );
    m_pkStaticTotalRank->SetAlign( DT_CENTER );
	m_pkStaticPageDesc->SetAlign( DT_CENTER );	

	m_dwBackCnt = 0;

	InitControl();

    m_apkBtn[ BTN_COLSE ]->SetHotKey( DIK_ESCAPE );
}

void KGCAgitList::InitControl()
{
	m_pkNumUserCombo->DeleteAllItem();
	m_pkNumUserCombo->SetComboStatic( g_pkStrLoader->GetReplacedString( STR_ID_AGIT_LIST_USER_NUM_OPTION, "i", 1 ) );

	for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
		m_pkNumUserCombo->AddString( g_pkStrLoader->GetReplacedString( STR_ID_AGIT_LIST_USER_NUM_OPTION, "i", i+1 ) );


	m_pkChannelCombo->DeleteAllItem();
	m_pkChannelCombo->SetComboStatic( g_pkStrLoader->GetReplacedString( STR_ID_AGIT_CHANNEL, "i", 1 ) );

	for ( int i=0; i<m_nMaxChannel; ++i ) 
	{
		m_pkChannelCombo->AddString( g_pkStrLoader->GetReplacedString( STR_ID_AGIT_CHANNEL, "i", i+1 ) );
	}

	m_pkStaticNumUserOption->SetText( g_pkStrLoader->GetString( STR_ID_SHOW_NUM_OVER_AGIT ) );
	m_pkStaticFullOption->SetText( g_pkStrLoader->GetString( STR_ID_FULL_AGIT_EXCEPT ) );
	m_pkStaticHideOption->SetText( g_pkStrLoader->GetString( STR_ID_HIDE_AGIT_EXCEPT ) );
	m_pkStaticTodayRank->SetText( g_pkStrLoader->GetString( STR_ID_TODAY_RANK_AGIT ) );
	m_pkStaticTotalRank->SetText( g_pkStrLoader->GetString( STR_ID_TOTAL_RANK_AGIT ) );
	m_pkStaticPageDesc->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_LIST_FRIEND_PAGE_DESC ) );
}

void KGCAgitList::InitData()
{
	m_eCurTab = TAB_AGIT_LIST_ACTIVE;

	UpdateTabUIVisible();
	UpdateAgitList();
}

void KGCAgitList::ActionPerformed( const KActionEvent& event )
{
    for( int i=0 ; i<NUM_TAB ; ++i )
    {
        if( event.m_pWnd == m_apkTab[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
            OnClickTab( static_cast< TAB_TYPE >( i ) );
    }

    for( int i=0 ; i<NUM_AGIT_SLOT ; ++i )
    {
        if( event.m_pWnd == m_apkAgitSlot[i] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
        {
            OnClickSlot( i );
        }
    }

	GCWND_MSG_MAP( m_apkCheck[CHECKBOX_FULL_OPTION],   KD3DWnd::D3DWE_BUTTON_CLICK,    OnCheckFull );
	GCWND_MSG_MAP( m_apkCheck[CHECKBOX_HIDE_OPTION],   KD3DWnd::D3DWE_BUTTON_CLICK,    OnCheckHide );
	GCWND_MSG_MAP( m_apkCheck[CHECK_FULL_OPTION],   KD3DWnd::D3DWE_BUTTON_CLICK,    OnCheckFull );
	GCWND_MSG_MAP( m_apkCheck[CHECK_HIDE_OPTION],   KD3DWnd::D3DWE_BUTTON_CLICK,    OnCheckHide );

	if ( event.m_pWnd == m_pkNumUserCombo && event.m_dwCode == D3DWE_COMBO_SELCHANGE ) 
	{
		OnNumUserComboChange();
	}

	if ( event.m_pWnd == m_pkChannelCombo && event.m_dwCode == D3DWE_COMBO_SELCHANGE ) 
	{
		OnChannelComboChange();
	}

	



	//	페이지 버튼 처리
	if ( event.m_pWnd == m_apkBtn[BTN_FIRST] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK) 
	{
		UpdatePage( BTN_FIRST );
	}
	else if ( event.m_pWnd == m_apkBtn[BTN_PREV] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK) 
	{
		UpdatePage( BTN_PREV );
	}
	else if ( event.m_pWnd == m_apkBtn[BTN_END] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK) 
	{
		UpdatePage( BTN_END );
	}
	else if ( event.m_pWnd == m_apkBtn[BTN_NEXT] && event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK) 
	{
		UpdatePage( BTN_NEXT );
	}

	GCWND_MSG_MAP( m_apkBtn[ BTN_VISIT ],   KD3DWnd::D3DWE_BUTTON_CLICK,    OnVisit );
    GCWND_MSG_MAP( m_apkBtn[ BTN_COLSE ],   KD3DWnd::D3DWE_BUTTON_CLICK,    OnClose );
	GCWND_MSG_MAP( m_apkBtn[ BTN_DELETE ],   KD3DWnd::D3DWE_BUTTON_CLICK,    OnDeleteFavority );
	
}

void KGCAgitList::UpdatePage( BTN_TYPE emBtnType )
{
	if ( emBtnType < BTN_FIRST || emBtnType > BTN_NEXT ) 
		return;

	switch ( emBtnType ) 
	{
	case BTN_FIRST:
		{
			if ( m_usCurPage <= 1 ) 
				return;

			UpdatePageFirst();
		}
		break;
	case BTN_PREV:
		{
			if ( m_usCurPage <= 1 ) 
				return;

			UpdatePagePrev();
		}
		break;
	case BTN_END:
		{
			if ( m_usCurPage >= m_usMaxPage ) 
				return;

			UpdatePageEnd();
		}
		break;
	case BTN_NEXT:
		{
			if ( m_usCurPage >= m_usMaxPage ) 
				return;

			UpdatePageNext();
		}
		break;
	}
}

void KGCAgitList::UpdatePageFirst()
{
	switch ( m_eCurTab ) 
	{
	case TAB_AGIT_LIST_ACTIVE:
		{
			KP2P::GetInstance()->Send_AgitPageListReq( m_nChannel, m_bShowOpenOnly, m_bShowNotFullOnly, m_usShowMinUser, 1);
		}
		break;
	case TAB_FRIEND_AGIT_ACTIVE:
		{
			m_usCurPage = 1;
			UpdateAgitList();
		}
		break;
	case TAB_LIKE_AGIT_ACTIVE:
		{
			m_usCurPage = 1;
			UpdateAgitList();
		}
		break;
	case TAB_RANK_AGIT_ACTIVE:
		break;
	}
}

void KGCAgitList::UpdatePagePrev()
{
	switch ( m_eCurTab ) 
	{
	case TAB_AGIT_LIST_ACTIVE:
		KP2P::GetInstance()->Send_AgitPageListReq( m_nChannel, m_bShowOpenOnly, m_bShowNotFullOnly, m_usShowMinUser, m_usCurPage-1 );
		break;
	case TAB_FRIEND_AGIT_ACTIVE:
		m_usCurPage -= 1;
		UpdateAgitList();
		break;
	case TAB_LIKE_AGIT_ACTIVE:
		m_usCurPage -= 1;
		UpdateAgitList();
		break;
	case TAB_RANK_AGIT_ACTIVE:
		break;
	}
}

void KGCAgitList::UpdatePageEnd()
{
	switch ( m_eCurTab ) 
	{
	case TAB_AGIT_LIST_ACTIVE:
		KP2P::GetInstance()->Send_AgitPageListReq( m_nChannel, m_bShowOpenOnly, m_bShowNotFullOnly, m_usShowMinUser, m_usMaxPage);
		break;
	case TAB_FRIEND_AGIT_ACTIVE:
		m_usCurPage = m_usMaxPage;
		UpdateAgitList();
		break;
	case TAB_LIKE_AGIT_ACTIVE:
		m_usCurPage = m_usMaxPage;
		UpdateAgitList();
		break;
	case TAB_RANK_AGIT_ACTIVE:
		break;
	}
}

void KGCAgitList::UpdatePageNext()
{
	switch ( m_eCurTab ) 
	{
	case TAB_AGIT_LIST_ACTIVE:
		KP2P::GetInstance()->Send_AgitPageListReq( m_nChannel, m_bShowOpenOnly, m_bShowNotFullOnly, m_usShowMinUser, m_usCurPage+1);
		break;
	case TAB_FRIEND_AGIT_ACTIVE:
		m_usCurPage += 1;
		UpdateAgitList();
		break;
	case TAB_LIKE_AGIT_ACTIVE:
		m_usCurPage += 1;
		UpdateAgitList();
		break;
	case TAB_RANK_AGIT_ACTIVE:
		break;
	}
}

void KGCAgitList::SetMaxChannel( int nMaxChannel )
{
	m_nMaxChannel = nMaxChannel;
	m_nChannel = 0;

	m_pkChannelCombo->DeleteAllItem();
	m_pkChannelCombo->SetComboStatic( g_pkStrLoader->GetReplacedString( STR_ID_AGIT_CHANNEL, "i", m_nChannel+1 ) );

	for ( int i=0; i<nMaxChannel; ++i ) 
	{
		m_pkChannelCombo->AddString( g_pkStrLoader->GetReplacedString( STR_ID_AGIT_CHANNEL, "i", i+1 ) );
	}

	

}

void KGCAgitList::SetRegisterAgitList( KAgitPageList& PageInfo_, std::vector< KAgitPageInfo >& vecPageList_ )
{
	m_nChannel = PageInfo_.m_nSID;
	m_bShowNotFullOnly = PageInfo_.m_bNotFull;
	m_bShowOpenOnly = PageInfo_.m_bOpen;
	m_usShowMinUser = PageInfo_.m_usMinUser;
	m_usCurPage = PageInfo_.m_usPageNum;
	m_usMaxPage = PageInfo_.m_usMaxPageNum;
	m_vecRegisterPageInfo = vecPageList_;

	m_eCurTab = TAB_AGIT_LIST_ACTIVE;

	UpdateTabUIVisible();
	UpdateAgitList();
}

void KGCAgitList::SetFriendAgitList ( std::vector< KAgitPageInfo >& vecFriendPageInfo_ )
{
	m_vecFriendPageInfo = vecFriendPageInfo_;

	if ( m_vecFriendPageInfo.empty() ) 
	{
		m_usCurPage = 0;
		m_usMaxPage = 0;
	}
	else
	{
		m_usCurPage = 1;
		m_usMaxPage = static_cast<unsigned short>(m_vecFriendPageInfo.size()) / NUM_AGIT_SLOT;

		if ( m_vecFriendPageInfo.size() % NUM_AGIT_SLOT ) 
			m_usMaxPage += 1;
	}

	m_eCurTab = TAB_FRIEND_AGIT_ACTIVE;

	UpdateTabUIVisible();
	UpdateAgitList();
}

void KGCAgitList::SetFavoriteAgitList ( std::vector< KAgitPageInfo >& vecFavoritePageInfo_ )
{
	m_vecFavoritePageInfo = vecFavoritePageInfo_;

	if ( m_vecFavoritePageInfo.empty() ) 
	{
		m_usCurPage = 0;
		m_usMaxPage = 0;
	}
	else
	{
		m_usCurPage = 1;
		m_usMaxPage = static_cast<unsigned short>(m_vecFavoritePageInfo.size()) / NUM_AGIT_SLOT;

		if ( m_vecFavoritePageInfo.size() % NUM_AGIT_SLOT ) 
			m_usMaxPage += 1;
	}

	m_eCurTab = TAB_LIKE_AGIT_ACTIVE;

	UpdateTabUIVisible();
	UpdateAgitList();
}

void KGCAgitList::SetRankAgitList ( std::vector< KAgitPageInfo >& vecRankPageInfo_ )
{
	m_vecRankPageInfo = vecRankPageInfo_;

	if ( m_vecRankPageInfo.empty() ) 
	{
		m_usCurPage = 0;
		m_usMaxPage = 0;
	}
	else
	{
		m_usCurPage = 1;
		m_usMaxPage = 1;
	}

	m_eCurTab = TAB_RANK_AGIT_ACTIVE;

	UpdateTabUIVisible();
	UpdateAgitList();
}

void KGCAgitList::UpdateAgitList()
{
	UpdateUIVisible();
	
	switch ( m_eCurTab )
	{
	case TAB_AGIT_LIST_ACTIVE:
		{
			UpdateRegisterAgit();
		}
		break;
	case TAB_FRIEND_AGIT_ACTIVE:
		{
			UpdateFriendAgit();
		}
		break;
	case TAB_LIKE_AGIT_ACTIVE:
		{
			UpdateFavoriteAgit();
		}
		break;
	case TAB_RANK_AGIT_ACTIVE:
		{
			UpdateRankAgit();
		}
		break;
	}
}

void KGCAgitList::UpdateRegisterAgit()
{
//아지트의 유저 리스트 보여지는 부분이 첫 공개 아지트 리스트를 받아올때만 정보를 제대로 받고 
//그외에는 SimpleAgitList를 받는데 이곳에는 유저 리스트 정보가 없어서 이 부분을 안보여 주게 끄는 부분을 함수로 만들었다.
#if defined (NATION_BRAZIL)
    VisibleUserList( true );
#endif

    int nUser = m_usShowMinUser;
	m_pkNumUserCombo->SetComboStatic( g_pkStrLoader->GetReplacedString( STR_ID_AGIT_LIST_USER_NUM_OPTION, "i", nUser ) );

	m_pkChannelCombo->SetComboStatic( g_pkStrLoader->GetReplacedString( STR_ID_AGIT_CHANNEL, "i", m_nChannel+1 ) );
	m_pkChannelCombo->SetCurSel( m_nChannel );

	//	꽉찬 아지트
	if ( m_bShowNotFullOnly ) 
	{
		m_apkCheck[CHECK_FULL_OPTION]->ToggleRender(true);
	}
	else
	{
		m_apkCheck[CHECK_FULL_OPTION]->ToggleRender(false);
	}
	
	// 공개된 아지트만 보기
	if ( m_bShowOpenOnly ) 
	{
		m_apkCheck[CHECK_HIDE_OPTION]->ToggleRender(true);
	}
	else
	{
		m_apkCheck[CHECK_HIDE_OPTION]->ToggleRender(false);
	}



	std::wostringstream stm;
	stm << m_usCurPage << L" / " << m_usMaxPage;
	m_pkStaticPage->SetText( stm.str() );

	UpdateRegisterAgitSlot();
}

void KGCAgitList::UpdateFriendAgit()
{
	std::wostringstream stm;
	stm << m_usCurPage << L" / " << m_usMaxPage;
	m_pkStaticPage->SetText( stm.str() );

	m_pkStaticPageDesc->SetText( g_pkStrLoader->GetString( STR_ID_AGIT_LIST_FRIEND_PAGE_DESC ) );

	UpdateFriendAgitSlot();
#if defined (NATION_BRAZIL)
    VisibleUserList( false );
#endif

}
void KGCAgitList::UpdateFavoriteAgit()
{
	std::wostringstream stm;
	stm << m_usCurPage << L" / " << m_usMaxPage;
	m_pkStaticPage->SetText( stm.str() );

	m_pkStaticPageDesc->SetText( g_pkStrLoader->GetReplacedString( STR_ID_AGIT_LIST_FAVORITE_PAGE_DESC, "ii", 
		m_vecFavoritePageInfo.size(), nFAVORITE_MAX_NUM ) );	

	UpdateFavoriteAgitSlot();
#if defined (NATION_BRAZIL)
    VisibleUserList( false );
#endif

}
void KGCAgitList::UpdateRankAgit()
{
	UpdateRankAgitSlot();
#if defined (NATION_BRAZIL)
    VisibleUserList( false );
#endif

}

void KGCAgitList::UpdateRegisterAgitSlot( void )
{
	m_iCurSlotIndex = -1;

	for( int i=0 ; i<NUM_AGIT_SLOT ; ++i )
		m_apkAgitSlot[i]->Clear();

	m_pkActiveSlotFrame->ToggleRender ( false );

	int iIndex = 0;
	for( std::vector< KAgitPageInfo >::iterator it = m_vecRegisterPageInfo.begin() ; it != m_vecRegisterPageInfo.end() ; ++it )
	{
		if ( iIndex >= NUM_AGIT_SLOT ) return;

		m_apkAgitSlot[ iIndex++ ]->SetAgitSlotInfo( (*it) );
	}
}


void KGCAgitList::UpdateFriendAgitSlot( void )
{
	m_iCurSlotIndex = -1;

	for( int i=0 ; i<NUM_AGIT_SLOT ; ++i )
		m_apkAgitSlot[i]->Clear();

	m_pkActiveSlotFrame->ToggleRender ( false );

	int iIndex = 0;
	int vecIndex = 0;


	for( std::vector< KAgitPageInfo >::iterator it = m_vecFriendPageInfo.begin() ; it != m_vecFriendPageInfo.end() ; ++it )
	{
		if ( iIndex >= NUM_AGIT_SLOT ) return;

		//	페이지 첫인덱스
		if ( vecIndex < ( (m_usCurPage - 1) * NUM_AGIT_SLOT ) ) 
		{
			vecIndex++;
			continue;
		}

		if ( vecIndex >= ( m_usCurPage * NUM_AGIT_SLOT ) ) 
			return;

		m_apkAgitSlot[ iIndex++ ]->SetAgitSlotInfo( (*it) );

		vecIndex++;
	}
}

void KGCAgitList::UpdateFavoriteAgitSlot( void )
{
	m_iCurSlotIndex = -1;

	for( int i=0 ; i<NUM_AGIT_SLOT ; ++i )
		m_apkAgitSlot[i]->Clear();

	m_pkActiveSlotFrame->ToggleRender ( false );

	int iIndex = 0;
	int vecIndex = 0;

	for( std::vector< KAgitPageInfo >::iterator it = m_vecFavoritePageInfo.begin() ; it != m_vecFavoritePageInfo.end() ; ++it )
	{
		if ( iIndex >= NUM_AGIT_SLOT ) return;

		//	페이지 첫인덱스
		if ( vecIndex < ( (m_usCurPage - 1) * NUM_AGIT_SLOT ) ) 
		{
			vecIndex++;
			continue;
		}

		if ( vecIndex >= ( m_usCurPage * NUM_AGIT_SLOT ) ) 
			return;

		m_apkAgitSlot[ iIndex++ ]->SetAgitSlotInfo( (*it) );

		vecIndex++;
	}
}

void KGCAgitList::UpdateRankAgitSlot( void )
{
	m_iCurSlotIndex = -1;

	for( int i=0 ; i<NUM_AGIT_SLOT ; ++i )
		m_apkAgitSlot[i]->Clear();

	m_pkActiveSlotFrame->ToggleRender ( false );

	int iIndex = 0;
	for( std::vector< KAgitPageInfo >::iterator it = m_vecRankPageInfo.begin() ; it != m_vecRankPageInfo.end() ; ++it )
	{
		if ( iIndex >= NUM_AGIT_SLOT ) return;

		//	타이틀바 때문에 이 슬롯은 사용하지 않는다.
		if ( iIndex == 5 )
		{
			iIndex++;
		}

		m_apkAgitSlot[ iIndex++ ]->SetAgitSlotInfo( (*it) );
	}
}

void KGCAgitList::UpdateUIVisible()
{
	switch( m_eCurTab )
	{
	case TAB_AGIT_LIST_ACTIVE:
		{
			m_pkStaticTotalRank->ToggleRender(false);
			m_pkStaticTodayRank->ToggleRender(false);
			m_pkTodayRankFrame->ToggleRender(false);
			m_pkTotalRankFrame->ToggleRender(false);
			m_pkStaticPageDesc->ToggleRender(false);
			
			m_pkNumUserCombo->ToggleRender(true);
			m_pkChannelCombo->ToggleRender(true);
			//m_pkStaticFullOption->ToggleRender(true);
			m_pkStaticFullOption->ToggleRender(false);
			m_pkStaticHideOption->ToggleRender(true);
			m_pkStaticNumUserOption->ToggleRender(true);
			

//			m_apkCheck[CHECKBOX_FULL_OPTION]->ToggleRender(true);
			m_apkCheck[CHECKBOX_FULL_OPTION]->ToggleRender(false);
			m_apkCheck[CHECKBOX_HIDE_OPTION]->ToggleRender(true);
//			m_apkCheck[CHECK_FULL_OPTION]->ToggleRender(true);
			m_apkCheck[CHECK_FULL_OPTION]->ToggleRender(false);
			m_apkCheck[CHECK_HIDE_OPTION]->ToggleRender(true);

			m_apkBtn[BTN_FIRST]->ToggleRender(true);
			m_apkBtn[BTN_PREV]->ToggleRender(true);
			m_apkBtn[BTN_END]->ToggleRender(true);
			m_apkBtn[BTN_NEXT]->ToggleRender(true);
			m_apkBtn[BTN_DELETE]->ToggleRender(false);

			m_apkPageBack->ToggleRender(true);
			m_pkStaticPage->ToggleRender(true);

		}
		break;
	case TAB_FRIEND_AGIT_ACTIVE:
		{
			m_pkStaticTotalRank->ToggleRender(false);
			m_pkStaticTodayRank->ToggleRender(false);
			m_pkTodayRankFrame->ToggleRender(false);
			m_pkTotalRankFrame->ToggleRender(false);
			m_pkStaticPageDesc->ToggleRender(true);

			m_pkNumUserCombo->ToggleRender(false);
			m_pkChannelCombo->ToggleRender(false);
			m_pkStaticFullOption->ToggleRender(false);
			m_pkStaticHideOption->ToggleRender(false);
			m_pkStaticNumUserOption->ToggleRender(false);
			

			m_apkCheck[CHECKBOX_FULL_OPTION]->ToggleRender(false);
			m_apkCheck[CHECKBOX_HIDE_OPTION]->ToggleRender(false);
			m_apkCheck[CHECK_FULL_OPTION]->ToggleRender(false);
			m_apkCheck[CHECK_HIDE_OPTION]->ToggleRender(false);

			m_apkBtn[BTN_FIRST]->ToggleRender(true);
			m_apkBtn[BTN_PREV]->ToggleRender(true);
			m_apkBtn[BTN_END]->ToggleRender(true);
			m_apkBtn[BTN_NEXT]->ToggleRender(true);
			m_apkBtn[BTN_DELETE]->ToggleRender(false);

			m_apkPageBack->ToggleRender(true);
			m_pkStaticPage->ToggleRender(true);
		}
		break;
	case TAB_LIKE_AGIT_ACTIVE:
		{
			m_pkStaticTotalRank->ToggleRender(false);
			m_pkStaticTodayRank->ToggleRender(false);
			m_pkTodayRankFrame->ToggleRender(false);
			m_pkTotalRankFrame->ToggleRender(false);
			m_pkStaticPageDesc->ToggleRender(true);

			m_pkNumUserCombo->ToggleRender(false);
			m_pkChannelCombo->ToggleRender(false);
			m_pkStaticFullOption->ToggleRender(false);
			m_pkStaticHideOption->ToggleRender(false);
			m_pkStaticNumUserOption->ToggleRender(false);

			m_apkCheck[CHECKBOX_FULL_OPTION]->ToggleRender(false);
			m_apkCheck[CHECKBOX_HIDE_OPTION]->ToggleRender(false);
			m_apkCheck[CHECK_FULL_OPTION]->ToggleRender(false);
			m_apkCheck[CHECK_HIDE_OPTION]->ToggleRender(false);

			m_apkBtn[BTN_FIRST]->ToggleRender(true);
			m_apkBtn[BTN_PREV]->ToggleRender(true);
			m_apkBtn[BTN_END]->ToggleRender(true);
			m_apkBtn[BTN_NEXT]->ToggleRender(true);
			m_apkBtn[BTN_DELETE]->ToggleRender(true);

			m_apkPageBack->ToggleRender(true);
			m_pkStaticPage->ToggleRender(true);
		}
		break;
	case TAB_RANK_AGIT_ACTIVE:
		{
			m_pkStaticTotalRank->ToggleRender(true);
			m_pkStaticTodayRank->ToggleRender(true);
			m_pkTodayRankFrame->ToggleRender(true);
			m_pkTotalRankFrame->ToggleRender(true);
			m_pkStaticPageDesc->ToggleRender(false);

			m_pkNumUserCombo->ToggleRender(false);
			m_pkChannelCombo->ToggleRender(false);
			m_pkStaticFullOption->ToggleRender(false);
			m_pkStaticHideOption->ToggleRender(false);
			m_pkStaticNumUserOption->ToggleRender(false);

			m_apkCheck[CHECKBOX_FULL_OPTION]->ToggleRender(false);
			m_apkCheck[CHECKBOX_HIDE_OPTION]->ToggleRender(false);
			m_apkCheck[CHECK_FULL_OPTION]->ToggleRender(false);
			m_apkCheck[CHECK_HIDE_OPTION]->ToggleRender(false);

			m_apkBtn[BTN_FIRST]->ToggleRender(false);
			m_apkBtn[BTN_PREV]->ToggleRender(false);
			m_apkBtn[BTN_END]->ToggleRender(false);
			m_apkBtn[BTN_NEXT]->ToggleRender(false);
			m_apkBtn[BTN_DELETE]->ToggleRender(false);

			m_apkPageBack->ToggleRender(false);
			m_pkStaticPage->ToggleRender(false);

		}
		break;
	}
}

void KGCAgitList::UpdateTabUIVisible()
{
	switch ( m_eCurTab )
	{
	case TAB_AGIT_LIST_ACTIVE:
		{
			m_apkTab[ TAB_AGIT_LIST_DEFAULT ]->ToggleRender( false );
			m_apkTab[ TAB_AGIT_LIST_ACTIVE ]->ToggleRender( true );
			m_apkTab[ TAB_FRIEND_AGIT_DEFAULT ]->ToggleRender( true );
			m_apkTab[ TAB_FRIEND_AGIT_ACTIVE ]->ToggleRender( false );
			m_apkTab[ TAB_LIKE_AGIT_DEFAULT ]->ToggleRender( true );
			m_apkTab[ TAB_LIKE_AGIT_ACTIVE ]->ToggleRender( false );
			m_apkTab[ TAB_RANK_AGIT_DEFAULT ]->ToggleRender( true );
			m_apkTab[ TAB_RANK_AGIT_ACTIVE ]->ToggleRender( false );
		}
		break;
	case TAB_FRIEND_AGIT_ACTIVE:
		{
			m_apkTab[ TAB_AGIT_LIST_DEFAULT ]->ToggleRender( true );
			m_apkTab[ TAB_AGIT_LIST_ACTIVE ]->ToggleRender( false );
			m_apkTab[ TAB_FRIEND_AGIT_DEFAULT ]->ToggleRender( false );
			m_apkTab[ TAB_FRIEND_AGIT_ACTIVE ]->ToggleRender( true );
			m_apkTab[ TAB_LIKE_AGIT_DEFAULT ]->ToggleRender( true );
			m_apkTab[ TAB_LIKE_AGIT_ACTIVE ]->ToggleRender( false );
			m_apkTab[ TAB_RANK_AGIT_DEFAULT ]->ToggleRender( true );
			m_apkTab[ TAB_RANK_AGIT_ACTIVE ]->ToggleRender( false );
		}
		break;
	case TAB_LIKE_AGIT_ACTIVE:
		{
			m_apkTab[ TAB_AGIT_LIST_DEFAULT ]->ToggleRender( true );
			m_apkTab[ TAB_AGIT_LIST_ACTIVE ]->ToggleRender( false );
			m_apkTab[ TAB_FRIEND_AGIT_DEFAULT ]->ToggleRender( true );
			m_apkTab[ TAB_FRIEND_AGIT_ACTIVE ]->ToggleRender( false );
			m_apkTab[ TAB_LIKE_AGIT_DEFAULT ]->ToggleRender( false );
			m_apkTab[ TAB_LIKE_AGIT_ACTIVE ]->ToggleRender( true );
			m_apkTab[ TAB_RANK_AGIT_DEFAULT ]->ToggleRender( true );
			m_apkTab[ TAB_RANK_AGIT_ACTIVE ]->ToggleRender( false );
		}
		break;
	case TAB_RANK_AGIT_ACTIVE:
		{
			m_apkTab[ TAB_AGIT_LIST_DEFAULT ]->ToggleRender( true );
			m_apkTab[ TAB_AGIT_LIST_ACTIVE ]->ToggleRender( false );
			m_apkTab[ TAB_FRIEND_AGIT_DEFAULT ]->ToggleRender( true );
			m_apkTab[ TAB_FRIEND_AGIT_ACTIVE ]->ToggleRender( false );
			m_apkTab[ TAB_LIKE_AGIT_DEFAULT ]->ToggleRender( true );
			m_apkTab[ TAB_LIKE_AGIT_ACTIVE ]->ToggleRender( false );
			m_apkTab[ TAB_RANK_AGIT_DEFAULT ]->ToggleRender( false );
			m_apkTab[ TAB_RANK_AGIT_ACTIVE ]->ToggleRender( true );
		}
		break;
	}
}

bool KGCAgitList::IsReqAgitInfo( TAB_TYPE eCurTab )
{
	//	일단 무조건 요청
	return true;

	DWORD dwTick = GetTickCount();

	bool bTick = false;

	//	10초 한번씩 요청 가능하다.
	if ( (int)(dwTick - m_dwBackCnt) > nREQ_AGITINFO_TIME ) 
	{
		m_dwBackCnt = dwTick;
		return true;
	}

	return false;
}

void KGCAgitList::ReqUpdateFavoritePage()
{
	if ( m_eCurTab != TAB_LIKE_AGIT_ACTIVE ) 
		return;

	KP2P::GetInstance()->Send_AgitFavoriteListReq();

}

void KGCAgitList::ChangePage( TAB_TYPE eCurTab )
{
	switch ( eCurTab )
	{
	case TAB_AGIT_LIST_ACTIVE:
		{
			if ( IsReqAgitInfo( TAB_AGIT_LIST_ACTIVE ) ) 
			{
				KP2P::GetInstance()->Send_AgitPageListReq( m_nChannel, m_bShowOpenOnly, m_bShowNotFullOnly, m_usShowMinUser, 1);
			}
			
		}
		break;
	case TAB_FRIEND_AGIT_ACTIVE:
		{
			if ( IsReqAgitInfo( TAB_FRIEND_AGIT_ACTIVE ) ) 
			{
				std::vector< DWORD > vecOnlineFriendList;
				SiKFriendShip()->GetOnlienFriendList(GCNateOn::FRIEND_MODE, vecOnlineFriendList);
				
				if ( !vecOnlineFriendList.empty() ) 
				{
					std::set<DWORD> setFriendUserUID(vecOnlineFriendList.begin(),vecOnlineFriendList.end());
					KP2P::GetInstance()->Send_AgitFriendListReq( setFriendUserUID );

					return;
				}
			}

			//	접속한 친구가 없을경우 바로 리스트 갱신
			std::vector<KAgitPageInfo> vecTemp;
			SetFriendAgitList( vecTemp );

		}
		break;
	case TAB_LIKE_AGIT_ACTIVE:
		{
			if ( IsReqAgitInfo( TAB_LIKE_AGIT_ACTIVE ) ) 
			{
				KP2P::GetInstance()->Send_AgitFavoriteListReq();
			}
		}
		break;
	case TAB_RANK_AGIT_ACTIVE:
		{
			if ( IsReqAgitInfo( TAB_RANK_AGIT_ACTIVE ) ) 
			{
				KP2P::GetInstance()->Send_AgitTopRankListReq();
			}
		}
		break;
	}
}

void KGCAgitList::OnClickTab( TAB_TYPE eType_ )
{
    if( eType_ < TAB_AGIT_LIST_DEFAULT || eType_ > TAB_RANK_AGIT_ACTIVE )
        eType_ = TAB_AGIT_LIST_DEFAULT;

	TAB_TYPE eCurTab = TAB_AGIT_LIST_DEFAULT;

    switch ( eType_ )
    {
    case TAB_AGIT_LIST_DEFAULT:
		eCurTab = TAB_AGIT_LIST_ACTIVE;
        break;

    case TAB_FRIEND_AGIT_DEFAULT:
        eCurTab = TAB_FRIEND_AGIT_ACTIVE;
        break;

    case TAB_LIKE_AGIT_DEFAULT:       
        eCurTab = TAB_LIKE_AGIT_ACTIVE;
        break;

    case TAB_RANK_AGIT_DEFAULT:
		eCurTab = TAB_RANK_AGIT_ACTIVE;
        break;
    }

	//	같은 탭이면 동작할 필요없다.
	if ( m_eCurTab == eCurTab ) return;

	ChangePage( eCurTab );
}




void KGCAgitList::Clear( void )
{
	for( int i=0 ; i<NUM_AGIT_SLOT ; ++i )
		m_apkAgitSlot[i]->Clear();
	m_pkActiveSlotFrame->ToggleRender( false );

	m_bShowNotFullOnly = false;
	m_bShowOpenOnly = false;
	m_usShowMinUser = 0;
	m_nChannel = 0;
	m_nMaxChannel = 0;

	m_usCurPage = 0;
	m_usMaxPage = 0;
	m_vecRegisterPageInfo.clear();
	m_vecFriendPageInfo.clear();
	m_vecFavoritePageInfo.clear();
	m_vecRankPageInfo.clear();

	m_eCurTab = TAB_AGIT_LIST_ACTIVE;

	m_dwBackCnt = 0;
}


void KGCAgitList::OnClose( void )
{
    Clear();
    this->ToggleRender( false );
    g_pkAgitOverlayUI->ShowAgitInviteVisitUI( true );
}

void KGCAgitList::OnClickSlot( int iIndex_ )
{
    if( 0 > iIndex_ || iIndex_ >= NUM_AGIT_SLOT )
        return;

	if( !m_apkAgitSlot[ iIndex_ ]->IsValid() ) 
		return;

	//	이미 선택되어 있는 항목에 대해서 바로 방문
	if ( iIndex_ == m_iCurSlotIndex ) 
		OnVisit();

        
    if( 0 <= m_iCurSlotIndex && m_iCurSlotIndex < NUM_AGIT_SLOT )
	{
        m_apkAgitSlot[ m_iCurSlotIndex ]->SetCurSel( false );
        m_apkAgitSlot[ m_iCurSlotIndex ]->SetActive( false );
	}

    m_iCurSlotIndex = iIndex_;
    m_apkAgitSlot[m_iCurSlotIndex]->SetCurSel( true );
    m_apkAgitSlot[m_iCurSlotIndex]->SetActive( true );

    D3DXVECTOR2 dxvSlotPos = m_apkAgitSlot[ m_iCurSlotIndex ]->GetFixedWindowLocalPos();
    m_pkActiveSlotFrame->SetWindowPos( dxvSlotPos );
    m_pkActiveSlotFrame->ToggleRender( true );
}

void KGCAgitList::OnVisit( void )
{
    if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_AGIT )
        return;

    if( m_iCurSlotIndex < 0 || m_iCurSlotIndex >= NUM_AGIT_SLOT )
        return;

	DWORD dwDestUID = m_apkAgitSlot[m_iCurSlotIndex]->m_kAgitInfo.m_dwUID;

	//	현재 아지트랑 동일하면 무시한다.
	if ( SiKGCAgitGameManager()->IsSameAgit( dwDestUID ) )	
		return;

	
	if ( m_apkAgitSlot[m_iCurSlotIndex]->m_kAgitInfo.m_ucOpenType == KAgitInfo::OT_CLOSE ) 
	{
		//	비공개 아지트 입니다.
		//	비밀번호????
		g_pkAgitOverlayUI->ShowAgitEnterPass( dwDestUID );
		return;

	}
	//else if ( m_apkAgitSlot[m_iCurSlotIndex]->m_kAgitInfo.m_ucOpenType == KAgitInfo::OT_FRIEND ) 
	//{
	//	//	친구인지 확인 
	//	if ( !SiKFriendShip()->FindUIDInContactFriendList( dwDestUID ) ) 
	//	{
	//		g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString( STR_ID_AGIT_ENTER_ACK_ERROR_6 ), L"", KGCUIScene::GC_MBOX_USE_NORMAL, 0, 0, true, true, true );
	//		return;
	//	}
	//}	

    g_pkAgitOverlayUI->CloseInviteVisitUI();
    if ( SiKGCPartyManager()->IsParty() )
        SiKGCPartyManager()->LeaveParty();
    SiKGCPartyManager()->SetInviteState( true );
    g_pStateGame->OnExitGame(); // 내아지트에서 나온 다음
    
	SiKGCAgitGameManager()->EnterAgit( dwDestUID ); // 이동~
}

void KGCAgitList::OnDeleteFavority( void )
{
	if( m_iCurSlotIndex < 0 || m_iCurSlotIndex >= NUM_AGIT_SLOT )
		return;

	DWORD dwDestUID = m_apkAgitSlot[m_iCurSlotIndex]->m_kAgitInfo.m_dwUID;

	KP2P::GetInstance()->Send_AgitRemoveFavoriteReq( dwDestUID );

}

void KGCAgitList::OnCheckFull( void )
{
	if ( m_eCurTab != TAB_AGIT_LIST_ACTIVE ) 
		return;

	if ( m_apkCheck[CHECK_FULL_OPTION]->IsRenderOn() ) 
	{
		m_apkCheck[CHECK_FULL_OPTION]->ToggleRender( false );
		KP2P::GetInstance()->Send_AgitPageListReq( m_nChannel, m_bShowOpenOnly, false, m_usShowMinUser, 1);
	}
	else 
	{
		m_apkCheck[CHECK_FULL_OPTION]->ToggleRender( true );
		KP2P::GetInstance()->Send_AgitPageListReq( m_nChannel, m_bShowOpenOnly, true, m_usShowMinUser, 1);
	}


}

void KGCAgitList::OnCheckHide( void )
{
	if ( m_eCurTab != TAB_AGIT_LIST_ACTIVE ) 
		return;

	if ( m_apkCheck[CHECK_HIDE_OPTION]->IsRenderOn() ) 
	{
		m_apkCheck[CHECK_HIDE_OPTION]->ToggleRender( false );
		KP2P::GetInstance()->Send_AgitPageListReq( m_nChannel, false, m_bShowNotFullOnly, m_usShowMinUser, 1);
	}
	else 
	{
		m_apkCheck[CHECK_HIDE_OPTION]->ToggleRender( true );
		KP2P::GetInstance()->Send_AgitPageListReq( m_nChannel, true, m_bShowNotFullOnly, m_usShowMinUser, 1);
	}	
}

void KGCAgitList::OnNumUserComboChange( void )
{
	int nIndex = m_pkNumUserCombo->GetCurSel();

	if ( nIndex < 0 )
		return;

	if ( nIndex+1 == m_usShowMinUser )
		return;

	KP2P::GetInstance()->Send_AgitPageListReq( m_nChannel, m_bShowOpenOnly, m_bShowNotFullOnly, nIndex+1, 1 );
}

void KGCAgitList::OnChannelComboChange( void )
{
	int nIndex = m_pkChannelCombo->GetCurSel();

	if ( nIndex < 0 )
		return;

	if ( nIndex == m_nChannel )
		return;

	KP2P::GetInstance()->Send_AgitPageListReq( nIndex, m_bShowOpenOnly, m_bShowNotFullOnly, m_usShowMinUser, 1 );
}

void KGCAgitList::VisibleUserList( bool bIsVisible )
{
    for( int i=0 ; i<NUM_AGIT_SLOT ; ++i )
    {
        m_apkAgitSlot[i]->VisibleUserComboList( bIsVisible );
    }
}
