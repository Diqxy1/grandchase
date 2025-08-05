#include "stdafx.h"

#include ".\gcrankingdlg.h"
#include "GCRankingReward.h"
#include "GCAutoMatchRankingReward.h"

IMPLEMENT_CLASSNAME			( KGCRankingDlg );
IMPLEMENT_WND_FACTORY		( KGCRankingDlg );
IMPLEMENT_WND_FACTORY_NAME	( KGCRankingDlg, "gc_ranking_dlg" );

KGCRankingDlg::KGCRankingDlg(void)
: m_pkClose			( NULL )
, m_pkRankingList	( NULL )
, m_pkRankingReward	( NULL )
, m_pkAutoMatchRankingReward( NULL )
, m_eActiveRankingTab		( RT_Daily )
, m_pkGraph					( NULL )
, m_pkGuildTitleGuildBattle(NULL)
, m_emRankingMode( EM_RANKING_MODE_CHAR )
{
	LINK_CONTROL( "btn_close",			m_pkClose );
	LINK_CONTROL( "ranking_page",		m_pkRankingList );
	LINK_CONTROL( "reward_page",		m_pkRankingReward );
	LINK_CONTROL( "automatch_reward_page",		m_pkAutoMatchRankingReward );

	LINK_CONTROL( "tab_reward_active",	m_akTab[ RT_Reward	].m_pkActiveTab );
	LINK_CONTROL( "tab_daily_active",	m_akTab[ RT_Daily	].m_pkActiveTab );
	LINK_CONTROL( "tab_weekly_active",	m_akTab[ RT_Weekly	].m_pkActiveTab );
	LINK_CONTROL( "tab_monthly_active",	m_akTab[ RT_Monthly	].m_pkActiveTab );
	LINK_CONTROL( "tab_exp_active",		m_akTab[ RT_Exp		].m_pkActiveTab );
	LINK_CONTROL( "tab_total_active",	m_akTab[ RT_Total	].m_pkActiveTab );		//	길드
	LINK_CONTROL( "tab_automatch_active",	    m_akTab[ RT_AutoMatch	].m_pkActiveTab );		    //	대전 매칭 랭킹
	LINK_CONTROL( "tab_automatch_old_active",	m_akTab[ RT_AutoMatch_old	].m_pkActiveTab );		//	대전 매칭 랭킹


	LINK_CONTROL( "tab_reward",			m_akTab[ RT_Reward	].m_pkTab );
	LINK_CONTROL( "tab_daily",			m_akTab[ RT_Daily	].m_pkTab );
	LINK_CONTROL( "tab_weekly",			m_akTab[ RT_Weekly	].m_pkTab );
	LINK_CONTROL( "tab_monthly",		m_akTab[ RT_Monthly	].m_pkTab );
	LINK_CONTROL( "tab_exp",			m_akTab[ RT_Exp		].m_pkTab );
	LINK_CONTROL( "tab_total",			m_akTab[ RT_Total	].m_pkTab );			//	길드
    LINK_CONTROL( "tab_automatch",	    m_akTab[ RT_AutoMatch	].m_pkTab );		    //	대전 매칭 랭킹
    LINK_CONTROL( "tab_automatch_old",	m_akTab[ RT_AutoMatch_old	].m_pkTab );		//	대전 매칭 랭킹


 	LINK_CONTROL( "toggle_tab_general_active",		m_akRankingModeTab[ RMT_CHARACTER ].m_pkActiveTab );
 	LINK_CONTROL( "toggle_tab_general",				m_akRankingModeTab[ RMT_CHARACTER ].m_pkTab );
 	LINK_CONTROL( "toggle_tab_guild_active",		m_akRankingModeTab[ RMT_GUILD ].m_pkActiveTab );
 	LINK_CONTROL( "toggle_tab_guild",				m_akRankingModeTab[ RMT_GUILD ].m_pkTab );
 	LINK_CONTROL( "toggle_tab_guildbattle_active",	m_akRankingModeTab[ RMT_GUILDBATTLE ].m_pkActiveTab );
 	LINK_CONTROL( "toggle_tab_guildbattle",			m_akRankingModeTab[ RMT_GUILDBATTLE ].m_pkTab );

	LINK_CONTROL( "graph",					m_pkGraph );

	LINK_CONTROL( "guild_title_daily",		 m_pkGuildTitleDaily );
	LINK_CONTROL( "guild_title_weekly",		 m_pkGuildTitleWeekly );
	LINK_CONTROL( "guild_title_monthly",	 m_pkGuildTitleMonthly );
	LINK_CONTROL( "guild_title_guildbattle", m_pkGuildTitleGuildBattle );


}



//------------------------------------------------------------------------
//	DlgModeTabActive()
//------------------------------------------------------------------------
void KGCRankingDlg::DlgModeTabActive( GCRankingModeTab eRankingMode_ )
{
	for( int eI = RMT_CHARACTER; eI < RMT_NUM; ++eI ){
		bool bActive = ( eI == eRankingMode_ );
            
		m_akRankingModeTab[eI].Active( bActive );
	}

    switch ( eRankingMode_ ) 
    {
    case RMT_CHARACTER:
        {
            if ( EnableAutoMatchRanking() )
            {
                m_emRankingMode = EM_RANKING_MODE_AUTO_MATCH;
            }
            else
            {
                m_emRankingMode = EM_RANKING_MODE_CHAR;
            }
        }
        break;
    case RMT_GUILD:
        m_emRankingMode = EM_RANKING_MODE_GUILD;
        break;
    case RMT_GUILDBATTLE:
        m_emRankingMode = EM_RANKING_MODE_GUILD_BATTLE;
        break;
    };

	RankingModeTabToggleRender();
}



//------------------------------------------------------------------------
//	RankingModeTabToggleRender()
//------------------------------------------------------------------------
void KGCRankingDlg::RankingModeTabToggleRender()
{
    //  길드전이면 다 끈다.
    if ( IsGuildBattleMode() ) 
    {
        for( int eI=RT_Daily; eI<RT_NUM; ++eI )
        {
            m_akTab[eI].ToggleRender( false );
        }
    }
    else
    {
        if ( IsAutoMatchMode() ) 
        {
            m_akTab[RT_Daily].ToggleRender( false );
            m_akTab[RT_Weekly].ToggleRender( false );
            m_akTab[RT_Monthly].ToggleRender( false );
            m_akTab[RT_Exp].ToggleRender( true );
            m_akTab[RT_Reward].ToggleRender( true );
            m_akTab[RT_Total].ToggleRender( false );
            m_akTab[RT_AutoMatch].ToggleRender( true );
            m_akTab[RT_AutoMatch_old].ToggleRender( true );
        }
        else
        {
            m_akTab[RT_Daily].ToggleRender( true );
            m_akTab[RT_Weekly].ToggleRender( true );
            m_akTab[RT_Monthly].ToggleRender( true );
            m_akTab[RT_Exp].ToggleRender( !IsGuildMode() );
            m_akTab[RT_Reward].ToggleRender( true );
            m_akTab[RT_Total].ToggleRender( IsGuildMode() );
            m_akTab[RT_AutoMatch].ToggleRender( false );
            m_akTab[RT_AutoMatch_old].ToggleRender( false );
        }
    }
}




//------------------------------------------------------------------------
//	ActionPerformed()
//------------------------------------------------------------------------
void KGCRankingDlg::ActionPerformed( const KActionEvent& event )
{
	if( event.m_dwCode == KD3DWnd::D3DWE_BUTTON_CLICK ){
		for( int eI = RMT_CHARACTER; eI < RMT_NUM; ++eI ){
			if( event.m_pWnd != m_akRankingModeTab[eI].m_pkTab ){
                continue;
			}
            this->DlgModeTabActive( static_cast<GCRankingModeTab>(eI) );


            switch ( m_emRankingMode ) 
            {
            case EM_RANKING_MODE_CHAR: 
                {
                    if ( m_eActiveRankingTab == RT_Total ) 
                    {
                        m_eActiveRankingTab = RT_Exp;
                    }   
                }
                break;

            case EM_RANKING_MODE_GUILD: 
                {
#if defined(NATION_THAILAND) 
                    m_eActiveRankingTab = IsGuildMode() ? RT_Total : RT_Daily;    
#else
                    if ( m_eActiveRankingTab == RT_Exp ) 
                    {
                        m_eActiveRankingTab = RT_Total;
                    }

                    if ( m_eActiveRankingTab == RT_AutoMatch || m_eActiveRankingTab == RT_AutoMatch_old ) 
                    {
                         m_eActiveRankingTab = RT_Daily;
                    }
#endif
                }
                break;
            case EM_RANKING_MODE_GUILD_BATTLE: 
                {
                    m_eActiveRankingTab = RT_Daily;
                }
                break;
            case EM_RANKING_MODE_AUTO_MATCH: 
                {
                    m_eActiveRankingTab = RT_AutoMatch;
                }
                break;
            };

			ActiveTab( GCRankingTab(m_eActiveRankingTab) );
			this->RankingTabToogleRender();
		}//for end





		if( event.m_pWnd == m_pkClose && g_pkUIScene->m_pkCharacterSelectBox->IsRenderOn() == false ){
			SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
		}

		for( int i = 0 ; i < RT_NUM ; ++i ){

#if defined(NATION_THAILAND) //태국선 길드 탭이 아니거나, 길드탭일땐 Total일때, 만 버튼 동작.
			if( event.m_pWnd == m_akTab[i].m_pkTab && (i == RT_Total || IsGuildMode()==false )) {
#else
			if( event.m_pWnd == m_akTab[i].m_pkTab ){
#endif
				ActiveTab((GCRankingTab)i);
				this->RankingTabToogleRender();
				break;
			}// if end
		}// for end
	}//if end


	if( event.m_dwCode == KD3DWnd::EWNDMESSAGE_CLOSE ){
		SpeakToActionListener( KActionEvent( this, KD3DWnd::EWNDMESSAGE_CLOSE ) );
	}
}


//------------------------------------------------------------------------
//	OnCreate()
//------------------------------------------------------------------------
void KGCRankingDlg::OnCreate( void )
{
	m_pkClose->InitState( true, true, this );
	m_pkClose->SetHotKey( DIK_ESCAPE );

	for( int i = 0 ; i < RT_NUM ; ++i )
    {
        if ( !m_akTab[i].m_pkActiveTab || !m_akTab[i].m_pkTab ) 
        {
            continue;
        }

		m_akTab[i].m_pkActiveTab->InitState( false );
		m_akTab[i].m_pkTab->InitState( true, true, this );
		m_akTab[i].ToggleRender(true);
	}//	for end

	for( int i = 0 ; i < RMT_NUM ; ++i ){
        if(m_akRankingModeTab[i].m_pkActiveTab)
		    m_akRankingModeTab[i].m_pkActiveTab->InitState( false );
        if(m_akRankingModeTab[i].m_pkTab)
		    m_akRankingModeTab[i].m_pkTab->InitState( true, true, this );
		m_akRankingModeTab[i].ToggleRender(true);
	}//	for end

#if defined( GUILD_RENEW_S4 )
    //Do not anything
    m_akRankingModeTab[RMT_GUILDBATTLE].ToggleRender( true );
#else
    m_akRankingModeTab[RMT_GUILDBATTLE].ToggleRender(false);
#endif
	
    IsUserInGuild();

	m_pkRankingReward->InitState( false, true, this );
    
    if ( m_pkAutoMatchRankingReward )
        m_pkAutoMatchRankingReward->InitState( false, true, this );
	
    
    m_pkRankingList->InitState( true,  true, this );

    if ( EnableAutoMatchRanking() ) 
    {
    	ActiveTab( RT_AutoMatch );	//기본적으로 일일 랭킹을 띄워 줍니다.	
    }
    else
    {
        ActiveTab( RT_Daily );
    }

	//#	처음 길드, 일반 토글 텝의 상태는 생성할때 결정

	this->DlgModeTabActive( RMT_CHARACTER );
	
    m_pkGraph->InitState(  false, true, this );

	//# 길드 사용 여부를 토글 해 주자.
	this->RankingTabToogleRender();

	m_pkRankingReward->ToggleUseForGuild( false );	//	리워드 페이지
	m_pkRankingList->ToggleUseForGuild( m_emRankingMode );	//	랭킹 페이지
	
	//#	요 놈들은 일단 꺼지세요.
	m_pkGuildTitleDaily->	InitState( false );
	m_pkGuildTitleWeekly->	InitState( false );
	m_pkGuildTitleMonthly->	InitState( false );
    if(m_pkGuildTitleGuildBattle)
	    m_pkGuildTitleGuildBattle->InitState( false );

	this->Update();
}


//------------------------------------------------------------------------
//	ActiveTab()
//------------------------------------------------------------------------
void KGCRankingDlg::ActiveTab( GCRankingTab tab )
{
	ASSERT( tab>=0 || tab<RT_NUM );

	for( int i = 0 ; i < RT_NUM ; ++i )
		m_akTab[i].Active( tab == i );

	m_pkRankingList->SetTabInfo( tab );
	m_eActiveRankingTab	=	tab;

	//#	리퀘 요청할 랭킹 타입
	KNGuildRankInfo::RANK_TYPE	eType = ( tab < RT_Exp ) ? KNGuildRankInfo::RANK_TYPE(tab) : KNGuildRankInfo::TOTAL_RANK;
	GuildRankReq( eType );

	//#	랭킹 리워드의 길드 토글
	m_pkRankingReward->ToggleUseForGuild( IsGuildMode() );

	//#	탭 종류에 따른 분기 처리
	if		( RT_Exp	==	tab ){	//	경험치( 전체 랭킹과 공존할수 없다. )
		m_pkRankingList->ExpRankingMode();
		m_pkRankingList->ToggleRender( true );
		m_pkRankingReward->ToggleRender( false );

        if ( m_pkAutoMatchRankingReward )
            m_pkAutoMatchRankingReward->ToggleRender( false );

	}else if( RT_Total	==	tab ){	//	전체 랭킹( 경험치 일반과 공존할수 없다. )
		m_pkRankingList->ToggleRender( true );
		m_pkRankingReward->ToggleRender( false );

        if ( m_pkAutoMatchRankingReward )
            m_pkAutoMatchRankingReward->ToggleRender( false );

	}else if( RT_Reward	==	tab ){	//	상품 내역
		m_pkRankingList->ToggleRender( false );
		
        if ( EnableAutoMatchRanking() && !IsGuildMode() )
        {
            m_pkRankingReward->ToggleRender( false );

            if ( m_pkAutoMatchRankingReward )
            {
                m_pkAutoMatchRankingReward->ToggleRender( true );
                m_pkAutoMatchRankingReward->ResetRewardItem();
            }
        }
        else
        {
            m_pkRankingReward->ToggleRender( true );
            m_pkRankingReward->UpdateRankerReward();

            if ( m_pkAutoMatchRankingReward )
                m_pkAutoMatchRankingReward->ToggleRender( false );
        }       

	}else{							//	랭킹 페이지
		m_pkRankingList->WinrateRankingMode();
		m_pkRankingList->ToggleRender( true );
		m_pkRankingReward->ToggleRender( false );
        if ( m_pkAutoMatchRankingReward )
            m_pkAutoMatchRankingReward->ToggleRender( false );
	}// if end
}


//------------------------------------------------------------------------
//	Update()
//------------------------------------------------------------------------
void KGCRankingDlg::Update()
{
	m_pkRankingReward->UpdateRankerReward();
	m_pkRankingList->UpdateRanking();

    if ( m_pkAutoMatchRankingReward )
        m_pkAutoMatchRankingReward->UpdateRankerReward();

	for( int eI = RT_Daily; eI<RT_NUM; ++eI ){
		m_akTab[eI].Update();
	}
}


//------------------------------------------------------------------------
//	NAME : FindErrorMsg()
//------------------------------------------------------------------------
void KGCRankingDlg::FindErrorMsg( int iStringID )	{ m_pkRankingList->SetFindErrorMsg( iStringID ); }


//------------------------------------------------------------------------
//	ToggleUseForGuild()
//	DESC : 여기에 전달되는 인자값으로 길드로 사용할지,
//			일반으로 사용할지 결정하자 ( 이동석 )
//------------------------------------------------------------------------
void KGCRankingDlg::RankingTabToogleRender()
{
	//모든 UI의 렌더 상태를 토글해야 한다.( 오직 렌더 상태만 )


	//#	랭킹 보상, 랭킹 리스트 두개의 하위 페이지들 설정
    bool bGuildMode = IsGuildMode();
    bool bGuildBattleMode = IsGuildBattleMode();

	m_pkRankingList->ToggleUseForGuild( m_emRankingMode );
	m_pkRankingReward->ToggleUseForGuild( bGuildMode );

	//#	탭상태에 따른 렌더 설정 : 랭킹탭 타입이 정해져 있어야 한다.
	//	일단 다 끄자.
	m_pkGraph->ToggleRender( false );
	m_pkGuildTitleDaily->ToggleRender( false );
	m_pkGuildTitleWeekly->ToggleRender( false );
	m_pkGuildTitleMonthly->ToggleRender( false );


    if(m_pkGuildTitleGuildBattle)
	    m_pkGuildTitleGuildBattle->ToggleRender( bGuildBattleMode );

	switch( m_eActiveRankingTab )
	{
	case RT_Daily:
		m_pkGuildTitleDaily->ToggleRender( bGuildMode );
		break;
	case RT_Weekly:
		m_pkGuildTitleWeekly->ToggleRender( bGuildMode );
		break;
	case RT_Monthly:
		m_pkGuildTitleMonthly->ToggleRender( bGuildMode );
		break;
	case RT_Reward:
		break;

	//#	Exp 나 Total 인 경우
	case RT_Total:
        m_pkGraph->ToggleRender( bGuildMode );
        m_akTab[ RT_Total ].Active( true );
        break;
	case RT_Exp:
        m_pkGraph->ToggleRender( bGuildMode );
        m_akTab[ RT_Exp ].Active( true );
        break;


	default:
		ASSERT( !"Invalid Ranking tab type" );
		break;
	}

	//#	사용법이 달라졌으니, 정보를 업데이트 해줘야 겠지?
	m_pkRankingList->UpdateRanking();	//	랭킹 페이지 업데이트
}


//------------------------------------------------------------------------
//	SetGuildRankingInfo()
//	DESC : On_EVENT_GUILD_RANK_ACK 에서 호출
//------------------------------------------------------------------------
void KGCRankingDlg:: SetGuildRankingInfo( 
	KNGuildRankInfo::RANK_TYPE		eType_,					//	정보 타입	
	std::vector< KNGuildRankInfo >&	vecRefKNGuildRnakInfo_	//	랭킹 벡터
){
	this->m_mapGuildRankInfo[ eType_ ]	=	vecRefKNGuildRnakInfo_;
}


//------------------------------------------------------------------------
//	IsUserInGuild()
//------------------------------------------------------------------------
bool KGCRankingDlg::IsUserInGuild()
{
#if defined( NATION_EU )
    m_akRankingModeTab[RMT_GUILD].ToggleRender( false );
	return false;
#endif

	//#	길갑 된 유저 아니면 말입니다, 아예 탭을 그리지 말죠.
	if( !this->IsHaveGuild() ){
        m_akRankingModeTab[RMT_GUILD].ToggleRender(false);
#if defined( GUILD_RENEW_S4 )
        m_akRankingModeTab[RMT_GUILDBATTLE].ToggleRender(false);
#endif
		return false;
	}

    return true;
}


//------------------------------------------------------------------------
//	GuildRankReq()
//	DESC : 리퀘 보내기 전에 map에서 find를 먼저 하기 때문에 같은 타입은
//			중복으로 리퀘를 보내지 않는다.
//------------------------------------------------------------------------
void KGCRankingDlg::GuildRankReq( KNGuildRankInfo::RANK_TYPE eType )
{
	typedef std::map< KNGuildRankInfo::RANK_TYPE, std::vector< KNGuildRankInfo > >::iterator ItrFind;

	static const DWORD s_dwOneSecond = 3000;

	ItrFind itrFind = this->m_mapGuildRankInfo.find( eType );
	if( itrFind	==	this->m_mapGuildRankInfo.end() ){ 
		KP2P::GetInstance()->Send_GuildRankReq( eType );
		Result_GuildRankingInfo_Req = INT_MAX;
		g_MyD3D->WaitForServerAck( Result_GuildRankingInfo_Req, INT_MAX, s_dwOneSecond, TIME_OUT_VALUE );
	}
}


//------------------------------------------------------------------------
//	IsHaveGuild()
//------------------------------------------------------------------------
bool KGCRankingDlg::IsHaveGuild()
{
#if defined( NATION_EU )
    return false;
#endif
	//# 길드가 없으면 m_dwUID 가 0이 란다.
	return ( g_kGlobalValue.m_kGuildInfo.m_dwUID ) ? true : false;
}


//------------------------------------------------------------------------
//	GuildRankReqAll()
//	DESC : 모든 길드 랭킹 정보를 요청
//------------------------------------------------------------------------
void KGCRankingDlg::GuildRankReqAll()
{
	static int arrRankType[] = {
		KNGuildRankInfo::TOTAL_RANK,	KNGuildRankInfo::DAILY_RANK, 
		KNGuildRankInfo::WEEKLY_RANK,	KNGuildRankInfo::MONTHLY_RANK,
		KNGuildRankInfo::GUILD_BATTLE_RANK,
	};
	static const int numRankType = sizeof(arrRankType)/sizeof(int);

	for( int i=0; i<numRankType; ++i ){
		this->GuildRankReq( static_cast<KNGuildRankInfo::RANK_TYPE>(arrRankType[i]) );
	}
}


//------------------------------------------------------------------------
//	소멸자, static 길드 정보
//------------------------------------------------------------------------
KGCRankingDlg::~KGCRankingDlg(){}







//------------------------------------------------------------------------
//	KGCRankingTab
//------------------------------------------------------------------------
KGCRankingTab::KGCRankingTab()
: m_pkActiveTab(NULL)
, m_pkTab(NULL)
, m_bRender(false)
, m_bActive(false)
{}

void KGCRankingTab::Active( bool bActive_ )
{
	m_bActive = bActive_;
	this->Update();
}

void KGCRankingTab::ToggleRender( const bool bRender_ )
{
	m_bRender	=	bRender_;
    if(m_pkActiveTab)
	    m_pkActiveTab->ToggleRender( m_bRender );
    if(m_pkTab)
	    m_pkTab->ToggleRender( m_bRender );
}

void KGCRankingTab::Update()
{
    if(m_pkActiveTab)
	    m_pkActiveTab->ToggleRender( m_bActive && m_bRender );
    if(m_pkTab)
	    m_pkTab->ToggleRender( !m_bActive && m_bRender );
}


