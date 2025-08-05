#include "stdafx.h"
#include "GCInfoPanel.h"

//
//
#include "../MyD3D.h"


//

#include "gcui/KGCBadUserPopup.h"
#include "Procedure.h"
//

//


#include "KGCUIHelper.h"
#include "GCQuestRankScore.h"
#include "KGCElvis.h"
#include "GCIconImageManager.h"
#include "GCStateMachine.h"
#include "KGCMultipleLanguages.h"

#include "GCUI/GCCharWnd.h"

IMPLEMENT_CLASSNAME( KGCMatchInfoPanelS6 );
IMPLEMENT_WND_FACTORY( KGCMatchInfoPanelS6 );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchInfoPanelS6, "gc_match_info_s6" );

KGCMatchInfoPanelS6::KGCMatchInfoPanelS6( void )
: m_pkstAutoInfo(NULL)
, m_pkstAutoRankInfo(NULL)
, m_pkstAutoValInfo(NULL)
, m_pkRankBack(NULL)
, m_pkRank(NULL)
, m_pkstAutoWin(NULL)
, m_pkstAutoLose(NULL)
, m_pkstAutoRait(NULL)
, m_pkstFreeWin(NULL)
, m_pkstFreeLose(NULL)
, m_pkstFreeRait(NULL)
, m_pkstRank( NULL )
, m_pkRankBtn( NULL )
, m_pkAutotitleBack(NULL)
, m_pkAutoInfoBack(NULL)
{
	LINK_CONTROL( "auto_info_static",			m_pkstAutoInfo);
	LINK_CONTROL( "auto_info_rank_static",			m_pkstAutoRankInfo);
	LINK_CONTROL( "auto_info_val_static",			m_pkstAutoValInfo);
	LINK_CONTROL( "rank_back",			m_pkRankBack);
	LINK_CONTROL( "rank",			m_pkRank);

	LINK_CONTROL( "auto_win_static",			m_pkstAutoWin);
	LINK_CONTROL( "auto_lose_static",			m_pkstAutoLose);
	LINK_CONTROL( "auto_rait_static",			m_pkstAutoRait);

	LINK_CONTROL( "free_win_static",			m_pkstFreeWin);
	LINK_CONTROL( "free_lose_static",			m_pkstFreeLose);
	LINK_CONTROL( "free_rait_static",			m_pkstFreeRait);

	LINK_CONTROL( "rank_static", m_pkstRank);
	LINK_CONTROL( "rank_btn", m_pkRankBtn);
	
    LINK_CONTROL( "auto_title_back" , m_pkAutotitleBack );
    LINK_CONTROL( "auto_info_back", m_pkAutoInfoBack);
}

KGCMatchInfoPanelS6::~KGCMatchInfoPanelS6( void )
{

}

void KGCMatchInfoPanelS6::ActionPerformed( const KActionEvent& event )
{
    GCWND_MSG_MAP( m_pkRankBtn, KD3DWnd::D3DWE_BUTTON_CLICK, OnRankBtn );    
}

void KGCMatchInfoPanelS6::OnCreate( void )
{
    if ( GC_GLOBAL_DEFINE::bAutoMatch )
    {
        m_pkstAutoInfo->InitState(true);
        m_pkstAutoRankInfo->InitState(true);
        m_pkstAutoValInfo->InitState(true);
        m_pkstAutoValInfo->SetAlign( DT_RIGHT );
        m_pkRank->InitState(false);
        m_pkRankBack->InitState(true);

        m_pkstAutoWin->InitState(true);
        m_pkstAutoWin->SetAlign( DT_LEFT );
        m_pkstAutoLose->InitState(true);
        m_pkstAutoLose->SetAlign( DT_CENTER );
        m_pkstAutoRait->InitState(true);
        m_pkstAutoRait->SetAlign( DT_RIGHT );
        m_pkAutotitleBack->InitState(true);
        m_pkAutoInfoBack->InitState(true);
    }
    else
    {
        m_pkstAutoInfo->InitState(false);
        m_pkstAutoRankInfo->InitState(false);
        m_pkstAutoValInfo->InitState(false);
        m_pkRank->InitState(false);
        m_pkRankBack->InitState(false);

        m_pkstAutoWin->InitState(false);
        m_pkstAutoLose->InitState(false);
        m_pkstAutoRait->InitState(false);
        m_pkAutotitleBack->InitState(false);
        m_pkAutoInfoBack->InitState(false);
    }

	m_pkstFreeWin->InitState(true);
	m_pkstFreeWin->SetAlign( DT_LEFT );
	m_pkstFreeLose->InitState(true);
	m_pkstFreeLose->SetAlign( DT_CENTER );
	m_pkstFreeRait->InitState(true);
	m_pkstFreeRait->SetAlign( DT_RIGHT );

    if ( m_pkstRank ) 
    {
        m_pkstRank->InitState(true);
        m_pkstRank->SetAlign( DT_LEFT );

        if ( !GC_GLOBAL_DEFINE::bAutoMatchRanking ) 
        {
            m_pkstRank->ToggleRender( false );
        }
    }

    if ( m_pkRankBtn ) 
    {
        m_pkRankBtn->InitState( true, true, this );

        if ( !GC_GLOBAL_DEFINE::bAutoMatchRanking ) 
        {
            m_pkRankBtn->ToggleRender( false );
        }
    }

	m_pkstAutoInfo->SetForceWordWrapText(g_pkStrLoader->GetString(STR_ID_MATCH_INFO_0) ,true );
    m_pkstAutoInfo->SetToolTip(g_pkStrLoader->GetString(STR_ID_MATCH_INFO_0));

}

void KGCMatchInfoPanelS6::UpdateMark( void )
{
	SetGradeTex((g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kELOUserData.m_ucGrade - 1) ,m_pkRank);
}

void KGCMatchInfoPanelS6::SetGradeTex( int index, KD3DWnd *pWnd )
{
	if( pWnd == NULL ) return;

	if ( index > 10 || index < 0 ) {
		return;
	}

	int x_index = index % 5 ; 
	int y_index = index / 5 ; 

	float tu = static_cast<float>(x_index) * 17.f + 378.f;
	float tv = static_cast<float>(y_index) * 17.f + 157.f;
	float tu1 = static_cast<float>(x_index) * 17.f + 394.f ;
	float tv1 = static_cast<float>(y_index) * 17.f + 173.f;
	D3DXVECTOR4 vec(tu,tv,tu1,tv1);
	ChangeUVtoTexel(vec);

	pWnd->SetWndTexCoords(D3DWM_DEFAULT,vec);
}


void KGCMatchInfoPanelS6::ChangeUVtoTexel( D3DXVECTOR4& vUV )
{
	float fMagicNumber=0.5f;
	if (vUV.x>vUV.z)
	{
		fMagicNumber*=-1.0f;
	}
	vUV.x = (vUV.x+fMagicNumber ) / 512.0f;
	vUV.z = (vUV.z-fMagicNumber ) / 512.0f;

	fMagicNumber=0.5f;
	if (vUV.y>vUV.w)
	{
		fMagicNumber*=-1.0f;
	}
	vUV.y = (vUV.y+fMagicNumber ) / 512.0f;
	vUV.w = (vUV.w-fMagicNumber ) / 512.0f;

	TEXEL_BOUND(vUV.x);
	TEXEL_BOUND(vUV.y);
	TEXEL_BOUND(vUV.z);
	TEXEL_BOUND(vUV.w);
}



void KGCMatchInfoPanelS6::UpdateInfo( void )
{
	int iWinAuto = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kELOUserData.m_nELOWin;
	int iLoseAuto = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kELOUserData.m_nELOLose;
	int iTotal = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kELOUserData.m_nMatchTotalCount;
	if ( iTotal <= 0 ) iTotal=1;
	int iWinRaitAuto = (iWinAuto * 100) / iTotal;

	m_pkstAutoWin->SetText(g_pkStrLoader->GetReplacedString(STR_ID_MATCH_INFO_5, "i", iWinAuto ));
	m_pkstAutoLose->SetText(g_pkStrLoader->GetReplacedString(STR_ID_MATCH_INFO_6, "i", iLoseAuto ));
	m_pkstAutoRait->SetText(g_pkStrLoader->GetReplacedString(STR_ID_MATCH_INFO_7, "i", iWinRaitAuto ));

	int iWin = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iWin;
	int iLose = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iLose;
	iTotal = iWin+iLose;
	if ( iTotal <= 0 ) iTotal=1;

	int iWinRait = (iWin * 100) / (iTotal);

	m_pkstFreeWin->SetText(g_pkStrLoader->GetReplacedString(STR_ID_MATCH_INFO_5, "i", iWin ));
	m_pkstFreeLose->SetText(g_pkStrLoader->GetReplacedString(STR_ID_MATCH_INFO_6, "i", iLose ));
	m_pkstFreeRait->SetText(g_pkStrLoader->GetReplacedString(STR_ID_MATCH_INFO_7, "i", iWinRait ));
	
	if ( g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kELOUserData.m_nELOType == KELOUserData::MT_PLACEMENTTEST ) {
		m_pkstAutoRankInfo->SetText(g_pkStrLoader->GetString(STR_ID_MATCH_INFO_2));
		m_pkstAutoRankInfo->SetAlign(DT_CENTER);
		m_pkRank->ToggleRender(false);
		m_pkRankBack->ToggleRender(false);

		m_pkstAutoInfo->ToggleRender(true);
		m_pkstAutoValInfo->ToggleRender(true);
		m_pkstAutoValInfo->SetText(g_pkStrLoader->GetReplacedString(STR_ID_MATCH_INFO_3, "i", 
			g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kELOUserData.m_nPlacementTestPlayCount
			- g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().kELOUserData.m_nMatchTotalCount));

        if ( m_pkstRank && m_pkRankBtn ) 
        {
            m_pkstRank->ToggleRender( false );
            m_pkRankBtn->ToggleRender( false );
        }
	}
	else {
		UpdateMark();
        UpdateRank();

        m_pkstAutoInfo->ToggleRender(false);
		m_pkstAutoValInfo->ToggleRender(false);

		m_pkRank->ToggleRender(true);
		m_pkRankBack->ToggleRender(true);
		m_pkstAutoRankInfo->SetText(g_pkStrLoader->GetString(STR_ID_MATCH_INFO_1));
		m_pkstAutoRankInfo->SetAlign(DT_LEFT);

       if ( m_pkstRank && m_pkRankBtn ) 
       {
           if ( GC_GLOBAL_DEFINE::bAutoMatchRanking ) 
           {
               m_pkstRank->ToggleRender( true );
               m_pkRankBtn->ToggleRender( true );
           }
       }
	}



	//m_pkstFreeValInfo->SetText(
	//	g_pkStrLoader->GetReplacedString(STR_ID_MATCH_INFO_4, "iii", iWin, iLose, iWinRait ));
}

void KGCMatchInfoPanelS6::UpdateRank( void )
{
    int iCharType = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType;
    const KMatchRank* pMatchRank = g_kGlobalValue.m_kRanking.GetMyAutoMatchRanking( KMatchRank::MRT_RP, iCharType );
    
    if ( !pMatchRank ) 
        return;


    std::wstring strTemp = g_pkStrLoader->GetReplacedString( STR_ID_AUTOMATCH_RANKING_INFO, "i", pMatchRank->m_nRank );

    if ( m_pkstRank ) 
        m_pkstRank->SetText( strTemp );
}

void KGCMatchInfoPanelS6::OnRankBtn()
{
    g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_RANKING );
}

IMPLEMENT_CLASSNAME( KGCInfoPanel );
IMPLEMENT_WND_FACTORY( KGCInfoPanel );
IMPLEMENT_WND_FACTORY_NAME( KGCInfoPanel, "gc_info_panel" );


KGCInfoPanel::KGCInfoPanel( void )
{
    m_pkLevel		= NULL;
    m_pkTeamID      = NULL;
	m_pkGuildName	= NULL;

	m_pkGP			= NULL;
	m_pkCrystal		= NULL;
	m_pkGem			= NULL;
	m_pkChange		= NULL;
    m_pkHeroCoin    = NULL;

	m_pkWin			= NULL;
	m_pkLose		= NULL;
	m_pkWinPercent	= NULL;
    m_pkPoint       = NULL;

    for( int i = 0 ; i < GC_CHAR_NUM; ++i)
    {
        m_pkTopChar[i] = NULL;
    }

    for(int i = 0 ; i < GC_GRADE_NUM; ++i)
    {
        m_pkGrade[i] = NULL;
    }

    m_pkGCCoinBackground = NULL;
    m_pkCash_img = NULL;
    m_pkStaticCash = NULL;
    m_pkChargeCashBtn = NULL;

    m_pkGashBackground = NULL;
    m_pkGGCoin_img = NULL;
    m_pkStaticGCCoin = NULL;


    m_pkPointMark   = NULL;
    m_pkBonusMark   = NULL;
    m_pkBonus       = NULL;

    m_pkBadUserInfo = NULL;
    m_pkYellowCard  = NULL;
    m_pkRedCard     = NULL;

	m_pkBtnGacha = NULL;
    m_pkWndGachaCoin = NULL;
    m_pkStaticGachaCoin = NULL;
    m_pkWndGachaVip = NULL;
    m_pkStaticGachaVip = NULL;
	
    m_pkShowVirtualCashBtn = NULL;


	m_iLevel = 0;
	m_iWin = 0;
	m_iLose = 0;
	m_iGP = 0;
	m_iGem = 0;
	m_iCrystal = 0;
    m_iHeroCoin = 0;
	m_iChange = NULL;

    LINK_CONTROL( "level_static",			m_pkLevel);
    LINK_CONTROL( "team_id_static",			m_pkTeamID);
	LINK_CONTROL( "guildname_static",		m_pkGuildName);
	LINK_CONTROL( "gp_static",				m_pkGP);
	LINK_CONTROL( "cristal_static",			m_pkCrystal);
	LINK_CONTROL( "gem_static",				m_pkGem);
	LINK_CONTROL( "change_static",			m_pkChange);
	LINK_CONTROL( "win_static",				m_pkWin);
	LINK_CONTROL( "lose_static",			m_pkLose);
	LINK_CONTROL( "win_percent_static",		m_pkWinPercent);
    LINK_CONTROL( "point_static",		    m_pkPoint);
    LINK_CONTROL( "herocoin_static",       m_pkHeroCoin );

    
	LINK_CONTROL( "user_info_elisis",	m_pkTopChar[GC_CHAR_ELESIS]);
	LINK_CONTROL( "user_info_lire",		m_pkTopChar[GC_CHAR_LIRE]);
	LINK_CONTROL( "user_info_arme",		m_pkTopChar[GC_CHAR_ARME]);	
	LINK_CONTROL( "user_info_las",		m_pkTopChar[GC_CHAR_LAS]);
	LINK_CONTROL( "user_info_ryan",		m_pkTopChar[GC_CHAR_RYAN]);
    LINK_CONTROL( "user_info_ronan",	m_pkTopChar[GC_CHAR_RONAN]);
    LINK_CONTROL( "user_info_amy",	    m_pkTopChar[GC_CHAR_AMY]);
	LINK_CONTROL( "user_info_jin",	    m_pkTopChar[GC_CHAR_JIN]);
    for( int i = GC_CHAR_SIEG; i < GC_CHAR_NUM; i++ ) {
        m_pkTopChar[i] = NULL;
        char strCharName[MAX_PATH];
        sprintf(strCharName,"user_info_%d",i);
        LINK_CONTROL( strCharName, m_pkTopChar[i] );
    }

	LINK_CONTROL( "player_grade_holy",		m_pkGrade[GC_GRADE_HOLY]);
	LINK_CONTROL( "player_grade_royal",		m_pkGrade[GC_GRADE_ROYAL]);
	LINK_CONTROL( "player_grade_mercenary",	m_pkGrade[GC_GRADE_MERCENARY]);
	LINK_CONTROL( "player_grade_vagabond",	m_pkGrade[GC_GRADE_VAGABOND]);

	LINK_CONTROL( "cash_background",		m_pkGCCoinBackground );
	LINK_CONTROL( "cash_img",				m_pkCash_img );
	LINK_CONTROL( "static_cash_point",      m_pkStaticCash );
	LINK_CONTROL( "cash_charge_btn",        m_pkChargeCashBtn );

	LINK_CONTROL( "gash_background",		m_pkGashBackground );
	LINK_CONTROL( "gash_img",				m_pkGGCoin_img );
	LINK_CONTROL( "static_gash_point",      m_pkStaticGCCoin );

    LINK_CONTROL( "user_info_point",        m_pkPointMark );
    LINK_CONTROL( "user_info_bonus",        m_pkBonusMark );

    LINK_CONTROL( "baduser_info",           m_pkBadUserInfo );
    LINK_CONTROL( "yellow_card",            m_pkYellowCard );
    LINK_CONTROL( "red_card",               m_pkRedCard );

    LINK_CONTROL( "show_virtual_cash",      m_pkShowVirtualCashBtn );

	LINK_CONTROL( "btn_gacha", m_pkBtnGacha );
	LINK_CONTROL( "gacha_coin", m_pkWndGachaCoin );
	LINK_CONTROL( "coin_static", m_pkStaticGachaCoin );
	LINK_CONTROL( "gacha_vip", m_pkWndGachaVip );
	LINK_CONTROL( "gacha_vip_static", m_pkStaticGachaVip );


	m_pkExpGaugeRect = NULL;
	LINK_CONTROL( "exp_gauge_rect", m_pkExpGaugeRect );
	m_pkExpGauge = NULL;
	LINK_CONTROL( "exp_gauge", m_pkExpGauge );
	m_pkStaticExpValue = NULL;
	LINK_CONTROL( "exp_value", m_pkStaticExpValue );

	m_pkGuildIconDrawRect = NULL;
	LINK_CONTROL( "guild_icon_draw_rect", m_pkGuildIconDrawRect );

    m_pkGuildIconBack = NULL;
    LINK_CONTROL( "guild_icon_back", m_pkGuildIconBack );

	m_pkMatchInfo = NULL;
	LINK_CONTROL( "match_info", m_pkMatchInfo );

	m_pkStaticChinaWebCash = NULL;
	LINK_CONTROL( "static_china_web_cash_point", m_pkStaticChinaWebCash );

	m_pkChinaWebCashImg = NULL;
	LINK_CONTROL( "china_web_cash_img", m_pkChinaWebCashImg );

	m_pkTexGuildMark = NULL;
}

KGCInfoPanel::~KGCInfoPanel( void )
{
}

void KGCInfoPanel::OnCreate( void )
{    
	m_pkLevel->SetSelfInputCheck( false );
    m_pkTeamID->SetSelfInputCheck( false );
	m_pkGuildName->SetSelfInputCheck( false );
	m_pkGP->SetSelfInputCheck( false );
	m_pkCrystal->SetSelfInputCheck( false );
	m_pkGem->SetSelfInputCheck( false );
	m_pkChange->SetSelfInputCheck( false );
	m_pkWin->SetSelfInputCheck( false );
	m_pkLose->SetSelfInputCheck( false );
	m_pkWinPercent->SetSelfInputCheck( false );
    m_pkPoint->SetSelfInputCheck( false );
	
    
    //유저 보창에 자동매치 관련된 UI가 포함 되어 있고, 자동매칭 사용함 - 대전 매칭
	if (m_pkMatchInfo && GC_GLOBAL_DEFINE::bAutoMatch )
    {
		m_pkMatchInfo->InitState(true);
    }

    if ( m_pkHeroCoin ) 
    {
        m_pkHeroCoin->SetSelfInputCheck( false );
        m_pkCrystal->ToggleRender( false );
    }

	for(int i = 0; i < GC_CHAR_NUM ; i++ )
	{	
		m_pkTopChar[i]->SetSelfInputCheck( false );	
	}

	for(int i = 0; i < GC_GRADE_NUM; i++)
	{
		m_pkGrade[i]->SetSelfInputCheck(false);
	}

	m_pkLevel->SetFontColor( 0xFFFFFFFF );
    m_pkTeamID->SetFontColor( 0xFFFFFFFF );
	m_pkGuildName->SetFontColor( 0xFFFFFFFF );
	m_pkGP->SetFontColor( 0xFFFFFFFF );
	m_pkCrystal->SetFontColor( 0xFFFFFFFF );
	m_pkGem->SetFontColor( 0xFFFFFFFF );
	m_pkChange->SetFontColor( 0xFFFFFFFF );
	m_pkWin->SetFontColor( 0xFFFFFFFF );
	m_pkLose->SetFontColor( 0xFFFFFFFF );
	m_pkWinPercent->SetFontColor( 0xFFFFFFFF );
    m_pkStaticCash->SetFontColor( 0xFFFFFFFF );
#ifdef NATION_CHINA
	if ( m_pkStaticChinaWebCash ) {
		m_pkStaticChinaWebCash->SetFontColor(0xFFFFFFFF);
		m_pkStaticChinaWebCash->SetAlign(DT_RIGHT);
	}
#endif

    m_pkPoint->SetFontColor( 0xFFFFFFFF );

#if defined( NATION_KOREA )
    m_pkTeamID->SetFontSize( 8 );
    m_pkLevel->SetFontSize( 14 );
#endif

    if ( m_pkHeroCoin ) 
        m_pkHeroCoin->SetFontColor( 0xFFFFFFFF );
    
    
    m_pkStaticCash->SetAlign( DT_RIGHT );
    m_pkStaticCash->ToggleRender( true );
    m_pkChargeCashBtn->InitState( true, true, this );
	m_pkGCCoinBackground->InitState( true, true, this );
	m_pkCash_img->InitState( true, false, false );
    m_pkShowVirtualCashBtn->InitState( false, false, this );

	m_pkGashBackground->InitState( false, true, this );
	m_pkGGCoin_img->InitState( false, false, false );
	m_pkStaticGCCoin->InitState( false, true, this );
	m_pkStaticGCCoin->SetAlign( DT_RIGHT );

    m_pkCash_img->SetToolTip( g_pkStrLoader->GetString(STR_ID_MONEYTYPE_GASH) );
    m_pkGGCoin_img->SetToolTip( g_pkStrLoader->GetString(STR_ID_BUY_CASH_ITEM_S6_STR_6) );


    #if ( !defined( GAME_CASH ) && !defined( WEB_CASH ) ) || defined( NATION_KOREA )
    {
        m_pkStaticCash->ToggleRender(false);
        m_pkChargeCashBtn->ToggleRender(false);
        m_pkGCCoinBackground->ToggleRender(false);
        m_pkCash_img->ToggleRender(false);
    }
    #endif

#if defined ( USE_VIP_POINT ) || defined( VIRTUAL_CASH )
	m_pkGashBackground->ToggleRender( true );
	m_pkGGCoin_img->ToggleRender( true );
	m_pkStaticGCCoin->ToggleRender( true );
#endif
    
#if defined( GAME_CASH )
    m_pkCash_img->SetWndMode( D3DWM_SELECT );
    m_pkCash_img->SetSelfInputCheck( false );
    m_pkChargeCashBtn->SetSelfInputCheck( true );
    m_pkShowVirtualCashBtn->SetSelfInputCheck( true );
#endif

    #if !defined( ENABLE_POINT_SYSTEM )
    {
        m_pkPoint->InitState( false );
        m_pkPointMark->InitState( false );
    }
    #endif

    // [12/18/2007] breadceo. 보여지는 부분 설정
    m_pkBonusMark->InitState( true );
    m_pkBonus = static_cast<KGCQuestRankScore*>( m_pkBonusMark->GetChildByName( "bonus" ) );
    assert( m_pkBonus != NULL );
    m_pkBonus->SetScore( 0 );
    m_pkBonus->SetNumPos( D3DXVECTOR2( 12.0f, 2.0f ) );
    m_pkBonus->SetNumWidth( 15.36f );
    m_pkBonus->SetNumHeight( 19.2f );
    m_pkBonus->SetNumInterval( 11.52f );
    m_pkBonus->SetNumOfNumber( 2 );
    m_pkBonus->InitState( true );

    // 불량유저 ㅎㅎ
    m_pkBadUserInfo->InitState( false );
    m_pkYellowCard->InitState( false, true, this );
    m_pkRedCard->InitState( false, true, this );

	m_pkExpGaugeRect->InitState(false);
	m_pkExpGauge->InitState(true);
	m_pkStaticExpValue->InitState(true);
	m_pkStaticExpValue->SetFontSize(9);
	m_pkStaticExpValue->SetAlign(DT_CENTER);
    m_pkStaticExpValue->SetFontOutline( true );

	m_pkGuildIconDrawRect->InitState(false);
    
    if( m_pkGuildIconBack )
        m_pkGuildIconBack->InitState(false);

	if ( m_pkStaticChinaWebCash )
		m_pkStaticChinaWebCash->InitState(false);

	if ( m_pkChinaWebCashImg )
		m_pkChinaWebCashImg->InitState(false);
}

void KGCInfoPanel::OnPostChildInitialize()
{
	SetUserInfo();
}

void KGCInfoPanel::PostChildDraw( void )
{
    if( g_kGlobalValue.m_kGuildInfo.m_dwUID != 0 ){
        if( m_pkGuildIconBack )
            m_pkGuildIconBack->ToggleRender( true );
    } else
        return;

	if (m_pkTexGuildMark == NULL)
		return;

	D3DXVECTOR2 vPos = GetRelocatedWindowPos();
	//vPos += m_pkGuildMarkRect->GetFixedWindowLocalPos();
	vPos.x += m_pkGuildIconDrawRect->GetFixedWindowLocalPos().x * m_fWindowScaleX;
	vPos.y += m_pkGuildIconDrawRect->GetFixedWindowLocalPos().y * m_fWindowScaleY;

	g_pGCDeviceManager2->DrawInScreen(m_pkTexGuildMark,
		vPos.x , vPos.y, 
		vPos.x + m_pkGuildIconDrawRect->GetWidth() * m_fWindowScaleX, vPos.y + m_pkGuildIconDrawRect->GetHeight() * m_fWindowScaleY,
		0.0f, 0.0f, 1.0f, 1.0f ,false,10.0f,0xffffffff);
}

void KGCInfoPanel::SetUserInfo()
{
	std::wostringstream strWinRate;
	std::wostringstream strWin;
	std::wostringstream strLose;
	std::wostringstream strmLV;
	int iWinRate = 0;

    GCEXPTYPE biExp = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().biExp;
	m_iLevel = static_cast<int>(Exp_2_Level( biExp ));

	m_iWin = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iWin;
	m_iLose = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iLose;	
    
    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_INDIGO ) )
    {
        m_iWin = g_MyD3D->m_TempPlayer.m_kUserInfo.iIndigoWin;
        m_iLose = g_MyD3D->m_TempPlayer.m_kUserInfo.iIndigoLose;
    }

	if( m_iWin + m_iLose > 0 )
		iWinRate = ( m_iWin * 100 ) / ( m_iWin + m_iLose );

	strWinRate << g_pkStrLoader->GetString( STR_ID_WINRATE ).c_str() << iWinRate << '%';

	m_iGP = g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().GetCurrentGP();
	m_iGem = g_MyD3D->m_kItemMgr.GetVipNum();
	m_iCrystal = g_MyD3D->m_kItemMgr.GetCrystalNum();
    m_iHeroCoin = g_MyD3D->m_kItemMgr.GetHeroCoin();

    m_dwCashPoint = g_MyD3D->m_kItemMgr.GetCrystalNum();
    m_dwVirtualCash = g_kGlobalValue.m_dwVirtualCash;
    m_iGCPoint = g_kGlobalValue.m_iGCPoint;

	strmLV << g_pkStrLoader->GetString( STR_ID_LEVEL ).c_str() << L" " << m_iLevel;
	strWin << g_pkStrLoader->GetString( STR_ID_WIN ).c_str() << m_iWin;
	strLose << g_pkStrLoader->GetString( STR_ID_LOSE ).c_str() << m_iLose;

#if defined(USE_VIP_POINT) || defined( VIRTUAL_CASH )
	m_pkStaticGCCoin->SetText( (int)m_dwVirtualCash );
#endif
    m_pkStaticCash->SetText( (int)m_dwCashPoint );

#ifdef NATION_CHINA
	if ( m_pkStaticChinaWebCash ) {
		float fWebCash = static_cast<float>(g_kGlobalValue.m_dwChinaWebCash) /1000.f;
		WCHAR szWebCashTemp[128];
		memset(szWebCashTemp, 0, sizeof(char) * 128);
		swprintf_s( szWebCashTemp, 256, L"%.1f", fWebCash);
		std::wstring wstrWebCash = szWebCashTemp;
		m_pkStaticChinaWebCash->SetText(wstrWebCash);
	}

#endif
    std::wstring wstrTeamID = L"#c" + g_kGlobalValue.m_kUserInfo.strNickColor + g_kGlobalValue.m_kUserInfo.strNickName + L"#cX";
    m_pkTeamID->SetText( wstrTeamID );

#if !defined( NATION_KOREA )
	strmLV << " " << GCFUNC::GetCharName(g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType) ;
#endif
	m_pkLevel->SetText( strmLV.str() );

    if( g_kGlobalValue.m_kGuildInfo.m_dwUID == 0 || 
        g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_JOINER ||
        g_kGlobalValue.m_kGuildUserInfo.m_cMemberLevel == KNGuildUserInfo::GL_DRUMOUT )
	{
		m_pkGuildName->SetText(g_pkStrLoader->GetString( STR_ID_NOT_JOIN_GUILD ));		
	}
	else
	{
		m_pkGuildName->ToggleRender(true);
		m_pkGuildName->SetText( g_kGlobalValue.m_kGuildInfo.m_strName );	

		m_pkTexGuildMark = SiKGCIconImageManager()->GetTexture(g_kGlobalValue.m_kGuildInfo.m_strFileName);
		if (m_pkTexGuildMark == NULL)
		{
			SiKGCIconImageManager()->LoadIconImageFromHTTP(g_kGlobalValue.m_kGuildInfo.m_strFileName);
			m_pkTexGuildMark = SiKGCIconImageManager()->GetTexture(g_kGlobalValue.m_kGuildInfo.m_strFileName); 
		}
	}
	m_pkGP->SetText(m_iGP);
	m_pkCrystal->SetText(L"");
	m_pkGem->SetText(m_iGem);
    m_pkPoint->SetText( m_iGCPoint );

    if ( m_pkHeroCoin )
        m_pkHeroCoin->SetText( m_iHeroCoin );

	//전직 블레싱이 이제는 아이템으로 바꼈음.
	m_pkChange->ToggleRender(false);
	GetChildByName("user_info_gp_change")->ToggleRender(false);

	m_pkWin->SetText(strWin.str());
	m_pkLose->SetText(strLose.str());
	m_pkWinPercent->SetText( strWinRate.str() );

	UpdateCharMark();
	UpdateGrade();

	if ( m_pkMatchInfo != NULL && GC_GLOBAL_DEFINE::bAutoMatch)
		m_pkMatchInfo->UpdateInfo();

	PLAYER* pPlayer = NULL;
	pPlayer = &g_MyD3D->m_TempPlayer;
	float fPercent = GetCurrentExpPercent( pPlayer->GetCurrentChar().biExp );
	DWORD dwPercentWidth = static_cast<DWORD>(m_pkExpGaugeRect->GetWidth() * fPercent / 100);
	D3DXVECTOR2 vGaugePos = m_pkExpGaugeRect->GetFixedWindowLocalPos();

	m_pkExpGauge->SetWidth(dwPercentWidth);
	m_pkExpGauge->SetFixedWindowLocalPos(vGaugePos);

	WCHAR strTemp[10];
	swprintf(strTemp, 10, L"%3.1f%%", fPercent);
	m_pkStaticExpValue->SetText(strTemp);

    // [12/18/2007] breadceo. 현재 보너스 가지고 와서 Set

    int nBonus = SiKGCElvis()->GetCurrentBonusPoint();
    if ( nBonus > MAX_BONUS_NUM )
    {
        nBonus = MAX_BONUS_NUM;
    }

    m_pkBonus->SetScore( nBonus );
}

void KGCInfoPanel::UpdateCharMark()
{
	for(int i = 0; i < GC_CHAR_NUM; i++)
	{
        if (m_pkTopChar[i])
		    m_pkTopChar[i]->ToggleRender(i == g_MyD3D->m_TempPlayer.m_kUserInfo.GetCurrentChar().iCharType);
	}
}

void KGCInfoPanel::UpdateGrade()
{
	// 2007.02.08 wony
	// 국내에서는 계급을 예전처럼 보여준다
	// 나머지는 승패에 따라서..
	{
		g_pkUIHelper->SetCharacterGrade( m_pkGrade, g_MyD3D->m_TempPlayer.m_kUserInfo.GetCharacterGrade() );
	}

	if ( g_MyD3D->m_pStateMachine->GetState() == GS_MATCH && GC_GLOBAL_DEFINE::bAutoMatch ) {

		for ( int i = 0; i < GC_GRADE_NUM; ++i )
		{
			m_pkGrade[i]->ToggleRender( false );
		}
	}
}

void KGCInfoPanel::ActionPerformed( const KActionEvent& event )
{
	/*if( event.m_pWnd == m_pkMissionButton )
	{
		if( event.m_dwCode == D3DWE_BUTTON_DOWN )
		{
			g_KDSound.Play( "30" );
		}
		else if( event.m_dwCode == D3DWE_BUTTON_CLICK )
		{
			g_pkUIScene->m_pkMissionDlg->SetReviewMode( -1 );
			g_pkUIScene->MessageBox( KGCUIScene::GC_MISSION_BOX );
		}
	}*/
    if ( event.m_pWnd == m_pkYellowCard || event.m_pWnd == m_pkRedCard )
    {
        if ( event.m_dwCode == D3DWE_CURSOR_ENTER )
        {
            m_pkBadUserInfo->SetInfoText( Result_BadUser_Info );
            m_pkBadUserInfo->ToggleRender( true );
        }
        else
        {
            m_pkBadUserInfo->ToggleRender( false );
        }
    }

    if ( event.m_pWnd == m_pkChargeCashBtn )
    {
        if( event.m_dwCode == D3DWE_BUTTON_CLICK )
        {
            g_KDSound.Play( "30" );
#ifdef	__PANHO__
			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_OK, g_pkStrLoader->GetString(STR_PANHO_MESSAGE_0), L"" ,KGCUIScene::GC_MBOX_USE_NORMAL,0,0,false,true);
			return;
#endif 

#if defined( NATION_USA ) || defined(NATION_PHILIPPINE) || defined( NATION_EU )
			CashRechargeWebOpen();
#elif defined( NATION_LATINAMERICA )
            g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_YES_NO, g_pkStrLoader->GetString( STR_ID_MOVE_CASH_CARGE_WEB_MSG ) , L"", 
                KGCUIScene::GC_MBOX_USE_CLOSE_CLIENT_FOR_CASH_CHARGE , 0, 0, false, true );
#elif defined(NATION_CHINA)
            int iChinaWebCash = static_cast<int>(g_kGlobalValue.m_dwChinaWebCash);
            if ( iChinaWebCash < 0 )
            {
			    KP2P::GetInstance()->Send_CYOU_GetWebPoint_Req();
			    Result_CYOU_WebCash = INT_MAX;
			    g_MyD3D->WaitForServerAck( Result_CYOU_WebCash, INT_MAX, 5000, TIME_OUT_VALUE );
            }

			g_pkUIScene->MessageBox( KGCUIScene::GC_MBOX_CHANGE_WEB_CASH );
			return;
#else
            PopUpWebPage( HOMEPAGE_CHARGE_CASH );
#endif
        }
    }
}

void KGCInfoPanel::FrameMoveInEnabledState( void )
{
    if ( Result_BadUser_Info.m_cCurrentGrade == -1 )
    {
        m_pkYellowCard->ToggleRender( true );
    }
    else if ( Result_BadUser_Info.m_cCurrentGrade == -2 )
    {
        m_pkRedCard->ToggleRender( true );
    }
    else
    {
        m_pkYellowCard->ToggleRender( false );
        m_pkRedCard->ToggleRender( false );
    }
}

bool KGCInfoPanel::IsSelectVirtualCash(void)
{
    return ( m_pkShowVirtualCashBtn->IsRenderOn() && ( D3DWM_SELECT == m_pkShowVirtualCashBtn->GetWndMode() ) ); 
}

void KGCInfoPanel::HideAllCashGashInfo(void)
{
    m_pkGGCoin_img->ToggleRender( false );
    m_pkGashBackground->ToggleRender( false );
    m_pkStaticGCCoin->ToggleRender( false );
    m_pkGCCoinBackground->ToggleRender( false );
    m_pkCash_img->ToggleRender( false );
    m_pkStaticCash->ToggleRender( false );
    m_pkChargeCashBtn->ToggleRender( false );
}