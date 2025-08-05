#include "stdafx.h"

#include ".\gcrankinglist.h"
#include "gcui/KGCUIHelper.h"

IMPLEMENT_CLASSNAME( KGCRankingList );
IMPLEMENT_WND_FACTORY( KGCRankingList );
IMPLEMENT_WND_FACTORY_NAME( KGCRankingList, "gc_ranking_list" );

KGCRankingList::KGCRankingList(void)
: m_pkGuildDescFrame		( NULL )
, m_pkG_Point				( NULL )
, m_guild_rank_tab			( NULL )
, m_guild_rank_tab_active	( NULL )
, m_line2_2					( NULL )
, m_pkAutoMatchCombo        ( NULL ) 
, m_emRankingMode( EM_RANKING_MODE_CHAR )
{
	LINK_CONTROL( "find_title",	m_pkFindTabActive );
	LINK_CONTROL( "find_tap",	m_pkFindTab );
	LINK_CONTROL( "myrank_title", m_pkMyRankTabActive );
	LINK_CONTROL( "myrank_tap",	m_pkMyRankTab );

	for( int i = 0 ; i < GC_RL_RANKING_NUM ; ++i )
	{
		std::stringstream str;
		str<<"rank_element"<<i;
        m_apkRanking[i] = NULL;
		LINK_CONTROL( str.str().c_str(),  m_apkRanking[i] );
	}

	m_pkTempRanking = NULL;
	LINK_CONTROL( "rank_temp",		m_pkTempRanking );
	LINK_CONTROL( "btn_find",		m_pkFindbtn );
	LINK_CONTROL( "btn_close",		m_pkClosebtn );
	LINK_CONTROL( "edit_find",		m_pkFindtxt );
	LINK_CONTROL( "static_desc",	m_pkDesc );

	for( int i = 0 ; i < GC_RLT_NUM ; ++i )
		m_apkListTitle[i] = NULL;

	LINK_CONTROL( "order",		m_apkListTitle[GC_RLT_ORDER] );
	LINK_CONTROL( "guild",		m_apkListTitle[GC_RLT_GUILD] );
	LINK_CONTROL( "char",		m_apkListTitle[GC_RLT_CHAR] );
	LINK_CONTROL( "nickname",	m_apkListTitle[GC_RLT_NICKNAME] );
	LINK_CONTROL( "win",		m_apkListTitle[GC_RLT_WIN] );
	LINK_CONTROL( "lose",		m_apkListTitle[GC_RLT_LOSE] );
	LINK_CONTROL( "winrate",	m_apkListTitle[GC_RLT_WINRATE] );
	LINK_CONTROL( "exp",		m_apkListTitle[GC_RLT_EXP] );
	LINK_CONTROL( "b_point",	m_apkListTitle[GC_RLT_BPOINT] );


    for ( int i = 0; i < GC_RL_TAB_NUM; ++i ) 
    {
        m_apkDescTitle[i] = NULL;
    }

	LINK_CONTROL( "title_reward",	m_apkDescTitle[4/*KGCRankingDlg::RT_Reward*/] );
	LINK_CONTROL( "title_daily",	m_apkDescTitle[0/*KGCRankingDlg::RT_Daily*/] );
	LINK_CONTROL( "title_weekly",	m_apkDescTitle[1/*KGCRankingDlg::RT_Weekly*/] );
	LINK_CONTROL( "title_monthly",	m_apkDescTitle[2/*KGCRankingDlg::RT_Monthly*/] );
	LINK_CONTROL( "title_exp",		m_apkDescTitle[3/*KGCRankingDlg::RT_Exp*/] );
	LINK_CONTROL( "guild_title_exp",m_apkDescTitle[5] );
    LINK_CONTROL( "title_automatch",m_apkDescTitle[6] );
    LINK_CONTROL( "title_automatch_old",m_apkDescTitle[7] );

	for( int i = 0 ; i < GC_RL_LINE_NUM ; ++i )
	{
		m_apkLine[i] = NULL;
		std::stringstream str;
		str<<"line"<<i;
		LINK_CONTROL( str.str().c_str(), m_apkLine[i] );
	}
    
    m_pkBtnSelectCharacter = NULL;
    LINK_CONTROL( "btn_character_select",   m_pkBtnSelectCharacter );

    m_pkBtnAllCharacter = NULL;
	LINK_CONTROL( "btn_all",                m_pkBtnAllCharacter );

	m_mapTabDesc.insert( std::map< int, DWORD >::value_type( 0, STR_ID_RANKING1 ) );
	m_mapTabDesc.insert( std::map< int, DWORD >::value_type( 1, STR_ID_RANKING2 ) );
	m_mapTabDesc.insert( std::map< int, DWORD >::value_type( 2, STR_ID_RANKING3 ) );
	m_mapTabDesc.insert( std::map< int, DWORD >::value_type( 3, STR_ID_RANKING4 ) );
	m_mapTabDesc.insert( std::map< int, DWORD >::value_type( 4, STR_ID_RANKING5 ) );

	m_mapTabGuildDesc.insert( std::map< int, DWORD >::value_type( 0, STR_ID_NEW_GUILD_DAILY_RANK_INFO ) );
	m_mapTabGuildDesc.insert( std::map< int, DWORD >::value_type( 1, STR_ID_NEW_GUILD_WEEKLY_RANK_INFO ) );
	m_mapTabGuildDesc.insert( std::map< int, DWORD >::value_type( 2, STR_ID_NEW_GUILD_MONTHLY_RANK_INFO ) );
    m_mapTabGuildDesc.insert( std::map< int, DWORD >::value_type( 3, STR_ID_NEW_GUILD_REWARD_INFO ) );
    m_mapTabGuildDesc.insert( std::map< int, DWORD >::value_type( 5, STR_ID_GUILD_TAB_ALL_RANKING_DESC ) );

	m_pkStaticLink = NULL;
	LINK_CONTROL( "pagelink", m_pkStaticLink );

	m_pkMessage = NULL;
	LINK_CONTROL( "static_message",	m_pkMessage );
	
	//#	길드 모드에 따라서 토글해야 하는 UI들
	LINK_CONTROL( "guild_desc_frame",		m_pkGuildDescFrame );
	LINK_CONTROL( "guild_name",				m_pkGuildName );
	LINK_CONTROL( "g_point",				m_pkG_Point );
	LINK_CONTROL( "guild_rank_tab",			m_guild_rank_tab );
	LINK_CONTROL( "guild_rank_tab_active",	m_guild_rank_tab_active );
	LINK_CONTROL( "line2_2",				m_line2_2 );

    LINK_CONTROL( "automatch_combobox",      m_pkAutoMatchCombo );
}

KGCRankingList::~KGCRankingList(void){}


//------------------------------------------------------------------------
//	NAME : ActionPerformed()
//------------------------------------------------------------------------
void KGCRankingList::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK )
	{
		if( event.m_pWnd == m_pkBtnAllCharacter )
		{
			m_iCharType = -1;
			// 리스트 갱신
			UpdateRanking();
		}

        if( event.m_pWnd == m_pkStaticLink && event.m_dwlParam2 == KD3DStaticLink::SL_ELEMENT_CLICK )
		{
            std::wstring pageNum = ((KD3DStatic*)event.m_dwlParam)->GetText();
            if ( pageNum == L"▶" )
                ++m_iPage;
            else if ( pageNum == L"◀" )
                --m_iPage;
            else
			    m_iPage = _wtoi( pageNum.c_str() ) - 1;

            m_iPage = std::min<int>( 9, m_iPage );
            m_iPage = std::max<int>( m_iPage, 0 );

			// 리스트 갱신
			UpdateRanking();
		}

		// 닫기 버튼 누질렀음
		if( event.m_pWnd == m_pkClosebtn && g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn() == false )
			SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );

		//	내 랭킹 탭을 누른 경우
		if( event.m_pWnd == m_pkMyRankTab )
		{
			//탭 토글 코드
			m_pkMyRankTab->ToggleRender( false );
			m_pkMyRankTabActive->ToggleRender( true );

			//////////////////////////////////////////////////////////////////////////
			m_pkMyRankTabActive->ToggleRender( true );
			m_pkMyRankTab->ToggleRender( false );

			SetMyRankingInfo();
		}

		//	내 길드 랭킹 탭을 누른 경우
		if( event.m_pWnd == m_guild_rank_tab )
		{
			m_guild_rank_tab->ToggleRender( false );
			m_guild_rank_tab_active->ToggleRender( true );

			m_pkMyRankTabActive->ToggleRender( true );
			m_pkMyRankTab->ToggleRender( false );

			//////////////////////////////////////////////////////////////////////////
			//	아래 2줄을 잊으면 짜증난다.
			m_pkFindTab->ToggleRender( true );
			m_pkFindTabActive->ToggleRender( false );

			m_iCharType = -1;
			SetMyRankingInfo();
		}

		if( event.m_pWnd == m_pkFindTab )
		{
			//------------------------------------------------------------------------
			//	검색 텝을 눌렀다!
			m_pkMyRankTab->ToggleRender( !IsGuildMode() );
			m_pkMyRankTabActive->ToggleRender( false );

			m_guild_rank_tab->ToggleRender( IsGuildMode() );
			m_guild_rank_tab_active->ToggleRender( false );
			//------------------------------------------------------------------------
						
			m_pkFindTab->ToggleRender( false );
			m_pkFindTabActive->ToggleRender( true );

			m_pkTempRanking->ToggleRender( false );
			m_pkMessage->ToggleRender( true );

			m_pkMessage->SetText( g_pkStrLoader->GetString(STR_ID_RANKING6) );
		}

	}

    if( event.m_pWnd == m_pkAutoMatchCombo &&
        event.m_dwCode == D3DWE_COMBO_SELCHANGE )
    {
        ChangeAutoMatchRanking();
    }

    GCWND_MSG_MAP( m_pkBtnSelectCharacter, KD3DWnd::D3DWE_BUTTON_CLICK, OpenCharacterSelcectUI );
	GCWND_CODE_MAP( m_pkFindbtn, OnFindBtn );
}//	FUNTION END	: ActionPerformed()

void KGCRankingList::OpenCharacterSelcectUI( void )
{
    if(g_pkUIScene->m_pkCharacterSelectBox == NULL)
        return;
    if(g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn())
        return;

	//KLuaManager luaMgr;
	//GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );

	float x = 285.0f;
	float y = 180.0f;

	//LUA_GET_VALUE_DEF( "x", x, 0.5f );    
	//LUA_GET_VALUE_DEF( "y", y, 0.5f );   

    D3DXVECTOR2 vPos = m_pkBtnSelectCharacter->GetFixedWindowLocalPos();

    vPos.x += x;
    vPos.y += y;

    g_pkUIScene->m_pkCharacterSelectBox->InitCharacterSelectBox(g_MyD3D->m_pStateMachine->GetState(), 
        boost::bind(&KGCRankingList::SendChangeChange,this,_1), 
        boost::bind(&KGCRankingList::UpdateCharacterSelect,this), vPos, KGCCharacterSelectBox::STATE_POSITION_DOWN, KGCCharacterSelectBox::EXPERT_SKILL_OPEN_CHARACTER_ALL);
    g_pkUIScene->m_pkCharacterSelectBox->SetWindowPosDirect(vPos);
    g_pkUIScene->m_pkCharacterSelectBox->SetWindowZOrder(D3DWZ_TOPMOST);
    g_pkUIScene->m_pkCharacterSelectBox->ToggleRender(true);
    m_pkBtnSelectCharacter->SetWndMode(D3DWM_LOCK);
}

void KGCRankingList::SendChangeChange(int iChar)
{
    m_iCharType = iChar;
    g_pkUIScene->m_pkCharacterSelectBox->SpeakToActionListener( KActionEvent(g_pkUIScene->m_pkCharacterSelectBox, EWNDMESSAGE_CLOSE) );
}

void KGCRankingList::UpdateCharacterSelect( void )
{
    UpdateRanking();
    m_pkBtnSelectCharacter->SetWndMode(D3DWM_DEFAULT);
}


//------------------------------------------------------------------------
//	NAME : OnCreate()
//------------------------------------------------------------------------
void KGCRankingList::OnCreate( void )
{
	//#	랭킹 정보를 담는 UI
	for( int i = 0 ; i < GC_RL_RANKING_NUM ; ++i )
	{
		m_apkRanking[i]->InitState( true, true, this );
		m_apkRanking[i]->WinrateRankingMode();
	}

	//	검색, 검색 내용 탭
	m_pkFindTabActive->InitState( false, true, this );
	m_pkFindTab->InitState( true, true, this );
	m_pkMyRankTabActive->InitState( true, true, this );
	m_pkMyRankTab->InitState( false, true, this );

	//	Temp인데, 검색한 내용의 정보를 담을 곳이다.
	m_pkTempRanking ->InitState( true, true );
	m_pkTempRanking->WinrateRankingMode();

	m_pkFindbtn ->InitState( true, true, this );
	m_pkFindbtn->SetHotKey( DIK_RETURN );
	m_pkClosebtn ->InitState( true, true, this );
	m_pkClosebtn->SetHotKey( DIK_ESCAPE );
	m_pkFindtxt ->InitState( true, true, this );
	m_pkDesc ->InitState( true, true );

	//	위에 조그맣게 표시될 문자들
	m_apkListTitle[GC_RLT_ORDER] ->InitState( true );
	m_apkListTitle[GC_RLT_GUILD] ->InitState( true );
	m_apkListTitle[GC_RLT_CHAR] ->InitState( true );
	m_apkListTitle[GC_RLT_NICKNAME] ->InitState( true );
	m_apkListTitle[GC_RLT_WIN] ->InitState( true );
	m_apkListTitle[GC_RLT_LOSE] ->InitState( true );
	m_apkListTitle[GC_RLT_WINRATE] ->InitState( true );
	m_apkListTitle[GC_RLT_EXP] ->InitState( false );
    if(m_apkListTitle[GC_RLT_BPOINT])
	    m_apkListTitle[GC_RLT_BPOINT] ->InitState( false );

	//	구분선들
	for( int i = 0 ; i < GC_RL_LINE_NUM ; ++i ){
		m_apkLine[i] ->InitState( true );
	}

	//	
	m_pkBtnAllCharacter->InitState( true, true, this );

    m_pkBtnSelectCharacter->InitState(true, true, this);

	m_pkStaticLink->InitState( true, true, this );
	m_pkStaticLink->SetDefaultColor( 0xFF134855 );
	m_pkStaticLink->SetHoverColor( 0xff1795A1 );
    m_pkStaticLink->SetFontSize( SiKGCMultipleLanguages()->GetLargeFontSize() );

	m_pkStaticLink->Clear();
    m_pkStaticLink->AddLinkString( L"◀" );
	m_pkStaticLink->AddLinkString( L"1" );
	m_pkStaticLink->AddLinkString( L"2" );
	m_pkStaticLink->AddLinkString( L"3" );
	m_pkStaticLink->AddLinkString( L"4" );
	m_pkStaticLink->AddLinkString( L"5" );
	m_pkStaticLink->AddLinkString( L"6" );
	m_pkStaticLink->AddLinkString( L"7" );
	m_pkStaticLink->AddLinkString( L"8" );
	m_pkStaticLink->AddLinkString( L"9" );
	m_pkStaticLink->AddLinkString( L"10" );
    m_pkStaticLink->AddLinkString( L"▶" );

	m_pkMessage->InitState( false );

    if ( EnableAutoMatchRanking() ) 
    {
	    m_iRankingType = KGCRankingDlg::RT_AutoMatch;
    }
    else
    {
        m_iRankingType = KGCRankingDlg::RT_Daily;
    }

	m_iCharType = -1;
	m_iPage = 0;


	//------------------------------------------------------------------------
	//	[ 길드 광장 랭킹 UI 추가 작업 ] 20091118 : 이동석
	//	길드인지 광장인지 조사

	//	생성될때 길드인지 아닌지의 정보는 여기 있다.
	m_pkGuildDescFrame->InitState( true, true, this );
	m_pkGuildName->InitState( true, true, this );

	m_pkG_Point->InitState( true, true, this );
	
	
	m_guild_rank_tab->InitState( false, true, this );
	m_guild_rank_tab_active->InitState( true, true, this );

	m_pkFindTabActive->InitState( false, true, this );
	m_pkFindTab->InitState( true, true, this );
	m_pkMyRankTabActive->InitState( false, true, this );
	m_pkMyRankTab->InitState( false, true, this );

	

	m_line2_2->InitState( true, true, this );

	m_apkLine[2]->InitState( false, true, this );
	
	ToggleUseForGuild( m_emRankingMode );

    //m_pkBtnSelectCharacter->SetHotKey( DIK_UP );
    g_pkUIScene->m_pkToolTip->SetWindowZOrder(D3DWZ_TOPMOST);
    m_pkBtnSelectCharacter->SetToolTip( g_pkStrLoader->GetString( STR_ID_CHARACTER_SELECT_UP) );


    if ( m_pkAutoMatchCombo ) 
    {
        m_pkAutoMatchCombo->AddActionListener( this );
        m_pkAutoMatchCombo->DeleteAllItem();
        m_pkAutoMatchCombo->SetComboStatic( g_pkStrLoader->GetString( STR_ID_AUTOMATCH_RANKING_RANK1 ) );
        m_pkAutoMatchCombo->AddString( g_pkStrLoader->GetString( STR_ID_AUTOMATCH_RANKING_RANK1 ) );
        m_pkAutoMatchCombo->AddString( g_pkStrLoader->GetString( STR_ID_AUTOMATCH_RANKING_RANK2 ) );
        m_pkAutoMatchCombo->SetCurSel( 0 );
    }
}



void KGCRankingList::ExpRankingMode()
{
	m_apkListTitle[GC_RLT_WIN]->ToggleRender( false );
	m_apkListTitle[GC_RLT_LOSE] ->ToggleRender( false );
	m_apkListTitle[GC_RLT_WINRATE] ->ToggleRender( false );
	m_apkListTitle[GC_RLT_EXP] ->ToggleRender( true );

	m_apkLine[4]->ToggleRender( false );
	m_apkLine[5]->ToggleRender( false );

	for( int i = 0 ; i < GC_RL_RANKING_NUM ; ++i )
	{
		m_apkRanking[i]->ExpRankingMode();
	}
	m_pkTempRanking->ExpRankingMode();
}

void KGCRankingList::WinrateRankingMode()
{
	m_apkListTitle[GC_RLT_WIN]->ToggleRender( true );
	m_apkListTitle[GC_RLT_LOSE] ->ToggleRender( true );
	m_apkListTitle[GC_RLT_WINRATE] ->ToggleRender( true );
	m_apkListTitle[GC_RLT_EXP] ->ToggleRender( false );

	m_apkLine[4]->ToggleRender( true );
	m_apkLine[5]->ToggleRender( true );

	//#	출력 원소들의 렌더 모드를 세팅
	for( int i = 0 ; i < GC_RL_RANKING_NUM ; ++i ){
		m_apkRanking[i]->WinrateRankingMode();
	}
	m_pkTempRanking->WinrateRankingMode();
}

void KGCRankingList::SetTabInfo( int tab )
{
	ASSERT( tab >= 0 && tab < GC_RL_TAB_NUM );

	for( int i = 0 ; i < GC_RL_TAB_NUM ; ++i )
    {
        if ( m_apkDescTitle[i] )
		    m_apkDescTitle[i]->ToggleRender( i == tab );
    }
	
	m_iRankingType = tab;
	m_iCharType	=	g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;
	m_iPage		=	0;
    
    if ( m_pkAutoMatchCombo ) 
        m_pkAutoMatchCombo->SetCurSel( 0 );

	ToggleUseForGuild( m_emRankingMode );
	UpdateRanking();
}

void KGCRankingList::UpdateRanking()
{
	if( m_iRankingType == KGCRankingDlg::RT_Reward )
        return;

	//#	길드 탭일 경우는 스트링이 분리된다.
    switch ( m_emRankingMode ) 
    {
    case EM_RANKING_MODE_CHAR:
        {
		    m_pkDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( m_mapTabDesc[m_iRankingType] ) );
        }
        break;
    case EM_RANKING_MODE_GUILD:
        {
		    m_pkDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( m_mapTabGuildDesc[m_iRankingType] ) );
        }
        break;
    case EM_RANKING_MODE_GUILD_BATTLE:
        {
		    m_pkDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_GUILDBATTLE_DESC ) );
        }
        break;
    case EM_RANKING_MODE_AUTO_MATCH:
        {
		    m_pkDesc->SetTextAutoMultiline( g_pkStrLoader->GetString( STR_ID_AUTOMATCH_RANKING_DESC ) );
        }
        break;    
    }

	switch( m_iRankingType )
	{
	case KGCRankingDlg::RT_Exp:
	case KGCRankingDlg::RT_Total:
		if( IsGuildMode() )
			UpdateGuildTotalRanking();
		else
			UpdateExpRanking();
		break;

	case KGCRankingDlg::RT_Daily:
	case KGCRankingDlg::RT_Weekly:
	case KGCRankingDlg::RT_Monthly:
		if( IsGuildMode() ){
			if		( KGCRankingDlg::RT_Daily	==	m_iRankingType ){
				UpdateGuildWinreateRanking( KNGuildRankInfo::DAILY_RANK );
			}else if( KGCRankingDlg::RT_Weekly	==	m_iRankingType ){
				UpdateGuildWinreateRanking( KNGuildRankInfo::WEEKLY_RANK );
			}else if( KGCRankingDlg::RT_Monthly	==	m_iRankingType ){
				UpdateGuildWinreateRanking( KNGuildRankInfo::MONTHLY_RANK );
			}
		}else{
			UpdateWinreateRanking();
		}
		break;
    case KGCRankingDlg::RT_AutoMatch:
        {
            if ( m_pkAutoMatchCombo )
            {
                bool bWinCount = m_pkAutoMatchCombo->GetCurSel() ? true : false;
                UpdateAutoMatchWinreateRanking( bWinCount, false );
            }
        }
        break;
    case KGCRankingDlg::RT_AutoMatch_old:
        {
            if ( m_pkAutoMatchCombo )
            {
                bool bWinCount = m_pkAutoMatchCombo->GetCurSel() ? true : false;
                UpdateAutoMatchWinreateRanking( bWinCount, true );
            }
        }
        break;
	default:
		break;
	}
	SetMyRankingInfo();

    // page : 0~9 text : 1~10
    WCHAR buf[MAX_PATH];
    _itow( m_iPage + 1, buf, 10 );
    m_pkStaticLink->SelectLink( buf );
}

void KGCRankingList::SetMyRankingInfo()
{  
    //닥치고 일단 초기화
    m_pkMyRankTab->ToggleRender( false );
    m_pkMyRankTabActive->ToggleRender( !IsGuildMode() );
    m_pkFindTab->ToggleRender( true );
    m_pkFindTabActive->ToggleRender( false );
    m_guild_rank_tab->ToggleRender( false );
    m_guild_rank_tab_active->ToggleRender( IsGuildMode() );



    if( g_kGlobalValue.m_bExpAccount ) //체험 계정이라면 랭킹에 관한 정보를 출력않고 밑에 스트링을 출력 - exp_id
    {
        m_pkMessage->SetText( g_pkStrLoader->GetString(STR_ID_EXPID_NOT_RANKING) );
        m_pkMessage->ToggleRender( true );
        m_pkTempRanking->ToggleRender( false );
    }
	else
    {
	    void* pVarify = NULL;

        switch ( m_iRankingType ) 
        {
        case KGCRankingDlg::RT_Exp:
            {
                pVarify = (void*)g_kGlobalValue.m_kRanking.GetMyExpRanking( m_iCharType );
                m_pkTempRanking->SetRankingInfo( (KExpRank*)pVarify );
                m_pkTempRanking->ExpRankingMode();
            }
            break;
        case KGCRankingDlg::RT_AutoMatch:
        case KGCRankingDlg::RT_AutoMatch_old:
            {
                if ( m_pkAutoMatchCombo ) 
                {
                    bool bWinCount = m_pkAutoMatchCombo->GetCurSel() ? true : false;
                    bool bOldSeason = m_iRankingType == KGCRankingDlg::RT_AutoMatch ? false : true;
                    char cRankType = GetMatchRankType( bWinCount, bOldSeason ); 

                    pVarify = (void*)g_kGlobalValue.m_kRanking.GetMyAutoMatchRanking( cRankType, m_iCharType );
                    m_pkTempRanking->SetRankingInfo( (KMatchRank*)pVarify );
                    m_pkTempRanking->WinrateRankingMode();
                }
            }
            break;
        default:
            {
                pVarify = (void*)g_kGlobalValue.m_kRanking.GetMyWinrateRanking( m_iRankingType, m_iCharType );
                m_pkTempRanking->SetRankingInfo( (KWinRateRank*)pVarify );
                m_pkTempRanking->WinrateRankingMode();
            }
            break;

        }

		if( pVarify == NULL )
	    {        
		    if( m_iCharType == -1 )
		    {
		        m_pkMessage->SetText( g_pkStrLoader->GetString(STR_ID_RANKING8) );
		    }
		    else
		    {
		        m_pkMessage->SetText( g_pkStrLoader->GetString(STR_ID_RANKING7) );
		    }

		    m_pkMessage->ToggleRender( true );
		    m_pkTempRanking->ToggleRender( false );
	    }
	    else
	    {
		    m_pkMessage->ToggleRender( false );
		    m_pkTempRanking->ToggleRender( true );
	    }
    }
}

void KGCRankingList::UpdateExpRanking()
{
	const std::vector< KExpRank >& ExpRank = g_kGlobalValue.m_kRanking.GetExpRanking( m_iCharType, m_iPage );

	for( int i = 0 ; i < GC_RL_RANKING_NUM ; ++i )
		m_apkRanking[i]->reset();

	for( int i = 0 ; i < (int)GC_RL_RANKING_NUM && i < (int)ExpRank.size() ; ++i )
	{
		m_apkRanking[i]->SetRankingInfo( &ExpRank[i] );
		m_apkRanking[i]->ToggleUseForGuild( IsGuildMode() );
	}
}

void KGCRankingList::UpdateWinreateRanking()
{
	const std::vector< KWinRateRank >& WinrateRank = g_kGlobalValue.m_kRanking.GetWinrateRanking( m_iRankingType, m_iCharType, m_iPage );

	for( int i = 0 ; i < GC_RL_RANKING_NUM ; ++i )
		m_apkRanking[i]->reset();

	for( int i = 0 ; i < (int)GC_RL_RANKING_NUM && i < (int)WinrateRank.size() ; ++i )
	{
		m_apkRanking[i]->SetRankingInfo( &WinrateRank[i] );
		m_apkRanking[i]->ToggleUseForGuild( IsGuildMode() );
	}
}

void KGCRankingList::UpdateAutoMatchWinreateRanking( bool bWinCount, bool bOldSeason )
{
    //  임시 코드
//     MRT_RP = 0, // RP 랭킹.( 전체, 캐릭터별 )
//     MRT_WINRECORD, // 다승 랭킹.( 전체, 캐릭터별 )
//     MRT_BEFORE_RP, // 이전 시즌 RP 랭킹.( 전체, 캐릭터별 )
//     MRT_BEFORE_WINRECORD, // 이전 시즌 다승 랭킹.( 전체, 캐릭터별
    
    char cRankType = GetMatchRankType( bWinCount, bOldSeason ); 

    const std::vector< KMatchRank >& vecMatchRank = g_kGlobalValue.m_kRanking.GetAutoMatchRanking( cRankType, m_iCharType, m_iPage );

    for( int i = 0 ; i < GC_RL_RANKING_NUM ; ++i )
        m_apkRanking[i]->reset();

    for( int i = 0 ; i < (int)GC_RL_RANKING_NUM && i < (int)vecMatchRank.size() ; ++i )
    {
        m_apkRanking[i]->SetRankingInfo( &vecMatchRank[i] );
        m_apkRanking[i]->ToggleUseForGuild( IsGuildMode() );
    }
}


//------------------------------------------------------------------------
//	NAME : OnFindBtn()
//	DESC : 랭킹 페이지에서 검색 버튼을 누르거나 엔터키를 쳤을때 들어온다.
//	DATE : 20091207
//------------------------------------------------------------------------
void KGCRankingList::OnFindBtn( DWORD code )
{
	if( code == KD3DWnd::D3DWE_BUTTON_CLICK &&
		g_pkUIHelper->GetUILockTime( LOCK_RANKING_FIND ) == 0 )
	{
		std::wstring strFind = m_pkFindtxt->GetText();
		if( strFind.empty() )
			return;

		//#	검색 버튼을 누른상태이므로, 랭킹 탭활성화는 다 꺼지세효
		m_guild_rank_tab->			ToggleRender(  IsGuildMode() );	//	길드 랭킹 탭
		m_guild_rank_tab_active->	ToggleRender(  false );				//	길드 랭킹 탭 활성화
		m_pkFindTab->				ToggleRender(  false );				//	검색 탭
		m_pkFindTabActive->			ToggleRender(  true  );				//	검색 탭 활성화
		m_pkMyRankTab->				ToggleRender( !IsGuildMode() );	//	내 랭킹 탭
		m_pkMyRankTabActive->		ToggleRender(  false );				//	내 랭킹 탭 활성화

		if( IsGuildMode() ){
			//------------------------------------------------------------------------
			//	검색된 정보를 세팅하는 코드 시작
			//#	현재 페이지를 알아낸다.
			KNGuildRankInfo::RANK_TYPE	eType = KNGuildRankInfo::DAILY_RANK;
			switch( m_iRankingType )
			{
			case KNGuildRankInfo::DAILY_RANK:
			case KNGuildRankInfo::WEEKLY_RANK:
			case KNGuildRankInfo::MONTHLY_RANK:
				eType	=	KNGuildRankInfo::RANK_TYPE( m_iRankingType );
				m_pkTempRanking->WinrateRankingMode();
				break;
			case KGCRankingDlg::RT_Total:
			case KGCRankingDlg::RT_Exp:
				eType	=	KNGuildRankInfo::TOTAL_RANK;
				m_pkTempRanking->TotalRankingMode();
				break;
			default:
				ASSERT(!"Invalid Rank Type, and type set by DAILY_RANK");
				break;
			}

			//#	페이지에 해당하는 랭킹 정보가 있는지 알아낸다.
			std::map< KNGuildRankInfo::RANK_TYPE, std::vector< KNGuildRankInfo > >::iterator itr;
			itr = g_pkUIScene->m_pkRanking->m_mapGuildRankInfo.find( eType );
			//#	해당 벡터가 없다.
			if( g_pkUIScene->m_pkRanking->m_mapGuildRankInfo.end() == itr ){
				m_pkMessage->ToggleRender( true );
                m_pkMessage->SetText( g_pkStrLoader->GetString(STR_ID_RANKING6) );
				m_pkTempRanking->reset();
				m_pkTempRanking->ToggleRender( false );

			//#	해당 벡터가 있다.
			}else{
				std::vector< KNGuildRankInfo >& rRank = itr->second;
				size_t	iLimit = rRank.size();
				bool	bFind = false;
				for( size_t i=0; i<iLimit; ++i ){
					if( rRank[i].m_strName	==	strFind	){

						m_pkTempRanking->SetRankingInfo( &rRank[i] );
						m_pkTempRanking->ToggleUseForGuild( IsGuildMode() );
						m_pkTempRanking->ToggleRender( true );

						m_pkMessage->ToggleRender( false );

						bFind = true;
						break;
					}
				}
				//	못찾았을 경우
				if( !bFind ){
					m_pkMessage->ToggleRender( true );
                    m_pkMessage->SetText( g_pkStrLoader->GetString(STR_ID_RANKING6) );
					m_pkTempRanking->reset();
					m_pkTempRanking->ToggleRender( false );
				}
			}
			//	검색된 정보를 세팅하는 코드 끗!
			//========================================================================

		}else{
			// 자신을 검색했다 ! 있는거 쓰쟈 ~
			if ( strFind == g_kGlobalValue.m_kUserInfo.strNickName )
				SetMyRankingInfo();
			else
            {
                if ( EnableAutoMatchRanking() )
                {
                    bool bWinCount = m_pkAutoMatchCombo->GetCurSel() ? true : false;
                    bool bOldSeason = m_iRankingType == KGCRankingDlg::RT_AutoMatch ? false : true;
                    char cRankType = GetMatchRankType( bWinCount, bOldSeason ); 
				    g_kGlobalValue.m_kRanking.FindMatchRankingInfo( cRankType, m_iCharType, strFind );
                }
                else
                {
                    g_kGlobalValue.m_kRanking.FindRankingInfo( m_iRankingType, m_iCharType, strFind );
                }
            }

			m_pkFindtxt->SetText( L"" );

			g_pkUIHelper->SetUILockTime( LOCK_RANKING_FIND, GC_FPS_LIMIT );
		}//	if( m_bUseForGuild ){ END

	}//	if( code == KD3DWnd::D3DWE_BUTTON_CLICK && END

}//FUNCTIONEND : void KGCRankingList::OnFindBtn( DWORD code )



void KGCRankingList::SetFindErrorMsg( int iStringID )
{
	m_pkMessage->ToggleRender( true );
	m_pkTempRanking->ToggleRender( false );

	m_pkMessage->SetText( g_pkStrLoader->GetString( iStringID ) );
}

void KGCRankingList::OnCreateComplete()
{
	// 개인 랭킹 정보를 셋팅 합니다.
	UpdateRanking();
}

//------------------------------------------------------------------------
//	NAME : ToggleUserForGuild()
//	DESC : 길드일때 표시되는 UI들을 몽땅 토글하자.
//------------------------------------------------------------------------
void KGCRankingList::ToggleUseForGuild( int emRankingMode )
{
    m_emRankingMode = emRankingMode;

	//#길드 모드에 따라 토글되는 UI
	m_pkGuildDescFrame->ToggleRender(  IsGuildMode() );
	m_pkGuildName->		ToggleRender(  IsGuildMode() );
	m_pkG_Point->		ToggleRender(  IsGuildMode() );
	m_apkLine[2]->		ToggleRender( !IsGuildMode() );
	m_line2_2->			ToggleRender(  IsGuildMode() );
    if(m_apkListTitle[GC_RLT_BPOINT])
	    m_apkListTitle[GC_RLT_BPOINT] ->InitState( IsGuildBattleMode() );

	//#	일단 랭킹 검색 탭은 토글 끄고
	m_guild_rank_tab->	ToggleRender( false );
	m_guild_rank_tab_active->ToggleRender( false );

	//#탭 종류에 따른 UI 처리
	switch( m_iRankingType )
	{
	case KGCRankingDlg::RT_Daily:
	case KGCRankingDlg::RT_Weekly:
	case KGCRankingDlg::RT_Monthly:
	case KGCRankingDlg::RT_Reward:
        if ( m_pkAutoMatchCombo ) 
            m_pkAutoMatchCombo->ToggleRender( false );
		break;

	case KGCRankingDlg::RT_Total:
	case KGCRankingDlg::RT_Exp:
		if( IsGuildMode() )
			TotalRankingMode();

        if ( m_pkAutoMatchCombo ) 
            m_pkAutoMatchCombo->ToggleRender( false );
		break;

    case KGCRankingDlg::RT_AutoMatch:
    case KGCRankingDlg::RT_AutoMatch_old:
        if ( m_pkAutoMatchCombo ) 
            m_pkAutoMatchCombo->ToggleRender( true );
	default:
		break;
	}

	//닥치고 일단 초기화
	m_pkMyRankTab->				ToggleRender( false );
	m_pkMyRankTabActive->		ToggleRender( !IsGuildMode() );
	m_pkFindTab->				ToggleRender( true );
	m_pkFindTabActive->			ToggleRender( false );
	m_guild_rank_tab->			ToggleRender( false );
	m_guild_rank_tab_active->	ToggleRender( IsGuildMode() );

	m_pkTempRanking->reset();
	m_pkTempRanking->ToggleUseForGuild( IsGuildMode() );
	
	//	랭킹 원소들 토글
	for( size_t i = 0; i<GC_RL_RANKING_NUM; ++i ){
		m_apkRanking[i]->ToggleUseForGuild( IsGuildMode() );
	}//for end

	m_iCharType = ( IsGuildMode() ) ? -1 : m_iCharType;
}


//------------------------------------------------------------------------
//	NAME : UpdateGuildRanking()
//	DESC : 전역 맵에서 정보를 가져온다.
//------------------------------------------------------------------------
void KGCRankingList::UpdateGuildWinreateRanking( KNGuildRankInfo::RANK_TYPE eType )
{
	std::map< KNGuildRankInfo::RANK_TYPE, std::vector< KNGuildRankInfo > >::iterator itr;
	itr = g_pkUIScene->m_pkRanking->m_mapGuildRankInfo.find( IsGuildBattleMode() ?
		KNGuildRankInfo::GUILD_BATTLE_RANK : eType );

	//	일단 정보는 리V하자.
	for( int i = 0 ; i < GC_RL_RANKING_NUM ; ++i )
		m_apkRanking[i]->reset();

	//	찾는 맵이 없으면 리턴
	if( g_pkUIScene->m_pkRanking->m_mapGuildRankInfo.end() == itr )
		return;

	const std::vector< KNGuildRankInfo >& guildRank = itr->second;

	size_t	numRank	=	guildRank.size();
	size_t	jLoop	=	m_iPage * GC_RL_RANKING_NUM;

	for( int i = 0 ; i < (int)GC_RL_RANKING_NUM && jLoop < numRank; ++i )
	{
		m_apkRanking[i]->SetRankingInfo( &guildRank[jLoop] );
		m_apkRanking[i]->ToggleUseForGuild( IsGuildMode() );

		++jLoop;
	}
}


//------------------------------------------------------------------------
//	NAME : UpdateGuildTotalRanking()
//	DESC : 길드 전체 랭킹
//------------------------------------------------------------------------
void KGCRankingList::UpdateGuildTotalRanking()
{
	std::map< KNGuildRankInfo::RANK_TYPE, std::vector< KNGuildRankInfo > >::iterator itr;

	itr = g_pkUIScene->m_pkRanking->m_mapGuildRankInfo.find( IsGuildBattleMode() ?
		KNGuildRankInfo::GUILD_BATTLE_RANK : KNGuildRankInfo::TOTAL_RANK );

	for( int i = 0 ; i < GC_RL_RANKING_NUM ; ++i )
		m_apkRanking[i]->reset();

	if( g_pkUIScene->m_pkRanking->m_mapGuildRankInfo.end() == itr )
		return;

	const std::vector< KNGuildRankInfo >& guildRank = itr->second;

	size_t	numRank		=	guildRank.size();
	size_t	jLoop		=	m_iPage * GC_RL_RANKING_NUM;
	float	topPoint	=	numRank ? static_cast<float>(guildRank[0].m_dwPoint ) : 0.0f;

	for( int i = 0 ; i < (int)GC_RL_RANKING_NUM && jLoop < numRank ; ++i )
	{
		m_apkRanking[i]->SetRankingInfo( &guildRank[jLoop], topPoint );	//	데이터 채우고
		m_apkRanking[i]->TotalRankingMode();							//	렌더링 모드 바꾼다.

		++jLoop;
	}
}


//------------------------------------------------------------------------
//	NAME : TotalRankingMode()
//	DESC : 전체 랭킹 모드로 렌더링 바꿈
//------------------------------------------------------------------------
void KGCRankingList::TotalRankingMode()
{
	m_apkListTitle[GC_RLT_WIN]->ToggleRender( false );
	m_apkListTitle[GC_RLT_LOSE] ->ToggleRender( false );
	m_apkListTitle[GC_RLT_WINRATE] ->ToggleRender( false );

	m_apkLine[4]->ToggleRender( false );
	m_apkLine[5]->ToggleRender( false );

	m_apkListTitle[GC_RLT_EXP]->ToggleRender( true );
}


void KGCRankingList::ChangeAutoMatchRanking()
{
    m_iCharType	=	g_kGlobalValue.m_kUserInfo.GetCurrentChar().iCharType;
    m_iPage		=	0;

    UpdateRanking();
}

char KGCRankingList::GetMatchRankType( bool bWinCount, bool bOldSeason )
{
    char cRankType = KMatchRank::MRT_RP;

    if ( bWinCount ) 
    {
        cRankType = bOldSeason ? KMatchRank::MRT_BEFORE_WINRECORD : KMatchRank::MRT_WINRECORD;
    }
    else
    {
        cRankType = bOldSeason ? KMatchRank::MRT_BEFORE_RP : KMatchRank::MRT_RP;
    }

    return cRankType;
}