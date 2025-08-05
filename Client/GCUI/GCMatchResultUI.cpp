#include "stdafx.h"
#include ".\gcmatchresultui.h"
#include "MyD3D.h"


#include "KGCRoomManager.h"

#include "GCStateGame.h"
//

#include "Controls.h"
#include "GCResultTower.h"

IMPLEMENT_CLASSNAME( KGCMatchResultUI );
IMPLEMENT_WND_FACTORY( KGCMatchResultUI );
IMPLEMENT_WND_FACTORY_NAME( KGCMatchResultUI, "gc_match_result" );

KGCMatchResultUI::KGCMatchResultUI(void)
: m_iCanabanCount( 0 )
, m_iSerdinCount( 0 )
, m_iGameEndCount( 0 )
, m_pkGuildBorder_R( NULL )
, m_pkGuildBorder_L( NULL )
, m_pkGuildmarkIcon_R( NULL )
, m_pkGuildmarkIcon_L( NULL )
, m_pkBPointBar_R( NULL )
, m_pkBPointBar_L( NULL )
, m_pkBPointBackground_R( NULL )
, m_pkBPointBackground_L( NULL )
, m_pkBPoint_R( NULL )
, m_pkBPoitn_L( NULL )
, m_pkVersus( NULL )
, m_pkGuildImage_R( NULL )
, m_pkGuildImage_L( NULL )
, m_pkPlusLeft( NULL )
, m_pkMinusLeft( NULL )
, m_pkPlusRight( NULL )
, m_pkMinusRight( NULL )
, m_StateMark( 0 )
{
    m_bFirstNagative = false;
    m_bSecondNagative = false;

    for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
		m_pkTower[i] = NULL;
		char strTower[MAX_PATH];
		sprintf( strTower, "player_tower%d", i + 1 );
		LINK_CONTROL( strTower,   m_pkTower[i] );
	}

	m_pkBackground = NULL;
	LINK_CONTROL( "background_white" , m_pkBackground );
    
    LINK_CONTROL ( "battlepoint_result_right", m_pkBPointBar_R );
    LINK_CONTROL ( "battlepoint_result_left", m_pkBPointBar_L );
    LINK_CONTROL( "guild_icon_border_right" , m_pkGuildBorder_R );
    LINK_CONTROL( "guild_icon_border_left" , m_pkGuildBorder_L );
    LINK_CONTROL( "guildmark_icon_right" ,   m_pkGuildmarkIcon_R );
    LINK_CONTROL( "guildmark_icon_left" ,  m_pkGuildmarkIcon_L );
    LINK_CONTROL( "battlepoint_background_r" ,  m_pkBPointBackground_R );
    LINK_CONTROL( "battlepoint_background_l" ,  m_pkBPointBackground_L );
    LINK_CONTROL( "battlepoint_R" , m_pkBPoint_R );
    LINK_CONTROL( "battlepoint_L" , m_pkBPoitn_L );
    LINK_CONTROL( "versus_icon" , m_pkVersus );
    LINK_CONTROL( "guildmark_image_right" , m_pkGuildImage_R );
    LINK_CONTROL( "guildmark_image_left" , m_pkGuildImage_L );
    LINK_CONTROL( "plus_left" , m_pkPlusLeft );
    LINK_CONTROL( "minus_left" , m_pkMinusLeft );
    LINK_CONTROL( "plus_right" , m_pkPlusRight);
    LINK_CONTROL( "minus_right" , m_pkMinusRight );

}

KGCMatchResultUI::~KGCMatchResultUI(void)
{
}

void KGCMatchResultUI::OnCreate()
{
	m_pkBackground->InitState( false, false, NULL );
	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
		m_pkTower[i]->InitState( true, true, this );
	}
    
    if ( m_pkGuildBorder_R )
        m_pkGuildBorder_R->InitState( false );
    if ( m_pkGuildBorder_L )
        m_pkGuildBorder_L->InitState( false );
    if ( m_pkGuildmarkIcon_R )
        m_pkGuildmarkIcon_R->InitState( false );
    if ( m_pkGuildmarkIcon_L )
        m_pkGuildmarkIcon_L->InitState( false );
    if ( m_pkBPointBar_R )
        m_pkBPointBar_R->InitState( false );
    if ( m_pkBPointBar_L )
        m_pkBPointBar_L->InitState( false );        
    if ( m_pkBPoint_R )
        m_pkBPoint_R->InitState( false );
    if ( m_pkBPoitn_L )
        m_pkBPoitn_L->InitState( false );
    if ( m_pkVersus )
        m_pkVersus->InitState( false );
    if ( m_pkGuildImage_R )
        m_pkGuildImage_R->InitState( false );
    if ( m_pkGuildImage_L )
        m_pkGuildImage_L->InitState( false );
    if ( m_pkBPointBackground_R )
        m_pkBPointBackground_R->InitState( false );
    if ( m_pkBPointBackground_L )
        m_pkBPointBackground_L->InitState( false );
    if ( m_pkPlusLeft )
        m_pkPlusLeft->InitState( false );
    if ( m_pkMinusLeft )
        m_pkPlusLeft->InitState( false );
    if ( m_pkPlusRight )
        m_pkPlusLeft->InitState( false );
    if ( m_pkMinusRight )
        m_pkPlusLeft->InitState( false );

}

void KGCMatchResultUI::ShowResultStart()
{
	m_vecPlayerIndex.clear();
	m_iSerdinCount = 0;
	m_iCanabanCount = 0;
	m_iGameEndCount = 0;
	BackGroundEnable();

	ToggleRender( true );
	//배치 상의 문제로 몇명의 유저가 플레이 했고, 플레이한 유저의 인덱스를 알아야 한다.
	InitTowers();
	InitPlayerIndex();

#if defined(GUILD_RENEW_S4)
    initBattlePoint();
#endif
}

void KGCMatchResultUI::initBattlePoint()
{
#if !defined(GUILD_RENEW_S4)
    return;
#endif
    if ( m_pkBPointBackground_R )
        m_pkBPointBackground_R->ToggleRender( false );
    if ( m_pkBPointBackground_L )
        m_pkBPointBackground_L->ToggleRender( false );

    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) ) {
        m_pkGuildBorder_R->ToggleRender( true );
        m_pkGuildBorder_L->ToggleRender( true );
        m_pkGuildmarkIcon_R->ToggleRender( true );
        m_pkGuildmarkIcon_L->ToggleRender( true );
        m_pkBPointBar_R->ToggleRender( true );
        m_pkBPointBar_L->ToggleRender( true );        
        m_pkBPoint_R->ToggleRender( true );
        m_pkBPoitn_L->ToggleRender( true );
        m_pkVersus->ToggleRender( true );
        m_pkGuildImage_R->ToggleRender( true );
        m_pkGuildImage_L->ToggleRender( true );
        
        if ( m_pkBPointBackground_R )
            m_pkBPointBackground_R->ToggleRender( true );
        if ( m_pkBPointBackground_L )
            m_pkBPointBackground_L->ToggleRender( true );
        
        m_vecDiffPoint = g_pGameBoard->m_vecPlayerDiffGuildPoint;

        for (int iLoop = 0; iLoop < (int)m_vecDiffPoint.size(); iLoop++)
        {
            if ( m_vecDiffPoint[iLoop] < 0 && iLoop == 0)
            {
                m_pkMinusLeft->ToggleRender( true );
                m_pkPlusLeft->ToggleRender( false );
                int iTemp = m_vecDiffPoint[ iLoop ];
                m_vecDiffPoint[iLoop] = -iTemp;
                m_bFirstNagative = true;
            }
            else if ( m_vecDiffPoint[iLoop] > 0 && iLoop == 0)
            {
                m_pkPlusLeft->ToggleRender( true );
                m_pkMinusLeft->ToggleRender( false );
                m_bFirstNagative = false;
            }
            else if ( m_vecDiffPoint[iLoop] < 0 && iLoop == 1)
            {
                m_pkMinusRight->ToggleRender( true );
                m_pkPlusRight->ToggleRender( false );
                int iTemp = m_vecDiffPoint[ iLoop ];
                m_vecDiffPoint[iLoop] = -iTemp;
                m_bSecondNagative = true;
            }
            else if (m_vecDiffPoint[iLoop] > 0 && iLoop == 1)
            {
                m_pkPlusRight->ToggleRender( true );
                m_pkMinusRight->ToggleRender( false );
                m_bSecondNagative = false;
            }
        }
    }
    
    g_pGameBoard->m_vecPlayerDiffGuildPoint.clear();
    
#if defined(GUILD_RENEW_S4)
    if ( SiKGCRoomManager()->GetRoomInfo().m_pairGuildName.second == L"")
    {
        m_pkMinusRight->ToggleRender(false);
        m_pkPlusRight->ToggleRender(false);        
    }

    if ( SiKGCRoomManager()->GetRoomInfo().m_pairGuildName.first == L"")
    {
        m_pkMinusLeft->ToggleRender(false);
        m_pkPlusLeft->ToggleRender(false);
    }    
#endif
}

void KGCMatchResultUI::PostChildDraw()
{
#if !defined(GUILD_RENEW_S4)
    return;
#endif

    if (g_kGlobalValue.ServerInfo.CheckServerType( ST_GUILD ) ) {
        D3DXVECTOR2 vPos;
        std::stringstream str;

        vPos = m_pkBPoitn_L->GetRelocatedWindowPos();
        //vPos.x = 240;
        //vPos.y = 25;
        
        vPos.x += 105 * m_fWindowScaleX* GC_SCREEN_DIV_WIDTH;
        vPos.y += 5 * m_fWindowScaleY * GC_SCREEN_DIV_HEIGHT;
        str.str("");
       if( m_vecDiffPoint.size() <2) return;
        str<<m_vecDiffPoint[0];
        if ( m_vecDiffPoint[0] < 30 && SiKGCRoomManager()->GetRoomInfo().m_pairGuildName.first != L"" )
        {
            if ( m_bFirstNagative )
                g_pkUIMgr->RenderNumber( "red_gradation_num",vPos,str.str().c_str(),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), true, false, -1.0f ,true, true);
            else
                g_pkUIMgr->RenderNumber( "blue_gradation_num",vPos,str.str().c_str(),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), true, false, -1.0f ,true, true);
        }

        vPos.x += 361 * m_fWindowScaleX* GC_SCREEN_DIV_WIDTH;    
        str.str("");
        str<<m_vecDiffPoint[1];
            
        if ( m_vecDiffPoint[1] < 30 && SiKGCRoomManager()->GetRoomInfo().m_pairGuildName.second != L"" ){
            if ( m_bSecondNagative )
                g_pkUIMgr->RenderNumber( "red_gradation_num",vPos,str.str().c_str(),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), true, false, -1.0f ,true, true);
            else
                g_pkUIMgr->RenderNumber( "blue_gradation_num",vPos,str.str().c_str(),D3DCOLOR_ARGB(0xff,0xff,0xff,0xff), true, false, -1.0f ,true, true);
        }

        SetGuildPanel( SiKGCRoomManager()->GetRoomInfo().m_pairGuildMarkName , SiKGCRoomManager()->GetRoomInfo().m_pairGuildName );
    }
}

void KGCMatchResultUI::InitPlayerIndex()
{
#if defined( TOWER_TEST )
	int iPlayerNum = 4;

	for( int i = 0 ; i < iPlayerNum ; ++i )
	{
		AllocPlayerToTower( ( i < (iPlayerNum/2) ? TEAM_CANABAN : TEAM_SERDIN ), 0 );
	}
#else
	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
        //기존에 세르딘은 0, 카나반은 1을 사용하고 있다.
		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) || g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) )
		{
			if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
				AllocPlayerToTower( g_MyD3D->MyPlayer[i]->m_kUserInfo.iTeam , i );	
		}
	}
#endif
}

void KGCMatchResultUI::InitTowers()
{
	m_iSerdinCount = 0;
	m_iCanabanCount = 0;

	for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
	{
		if( m_pkTower[i] == NULL )
			continue;

		m_pkTower[i]->Init();
		if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_CRUSADER )
		{
			m_pkTower[i]->SetTeam( TEAM_SERDIN );
		}
		else if( SiKGCRoomManager()->GetGameModeType() == GC_GMT_TEAM_FIGHT )
		{
			m_pkTower[i]->SetTeam( i < 3 ? TEAM_SERDIN : TEAM_CANABAN );
		}
	}
}

void KGCMatchResultUI::AllocPlayerToTower( int iTeam, int iPlayerIndex )
{
	if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MONSTER_CRUSADER ||
        SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DOTA )
	{
		m_pkTower[iPlayerIndex]->SetPlayerIndex( iPlayerIndex );
		m_pkTower[iPlayerIndex]->SetTowerHeight( RT_DEFAULT_HEIGHT );
	}
	else
	{
		bool bTeamMode = SiKGCRoomManager()->GetGameModeType() == GC_GMT_TEAM_FIGHT ? true : false ;

		int iIndex;
		if( bTeamMode == true )
		{
			iIndex = ((iTeam == TEAM_SERDIN ? m_iSerdinCount : m_iCanabanCount) + 1) % 3;
            iIndex = iTeam == TEAM_SERDIN ? iIndex : 5 - iIndex;

            if ( iTeam == TEAM_SERDIN )
            {
                m_iSerdinCount++;
                iIndex = m_iSerdinCount % 3;
            }
            else
            {
                m_iCanabanCount++;
                iIndex = 5 - (m_iCanabanCount % 3);
            }
		}
		else
			iIndex = iPlayerIndex;

        m_pkTower[iIndex]->SetPlayerIndex( iPlayerIndex );
        m_pkTower[iIndex]->SetTowerHeight( RT_DEFAULT_HEIGHT );
	}
}

void KGCMatchResultUI::BackGroundEnable( bool bRender /*= true */ )
{
	m_pkBackground->ForceSetWindowColor( D3DCOLOR_ARGB( 180, 0x00, 0x00, 0x00 ) );
	m_pkBackground->ToggleRender( bRender );
}

void KGCMatchResultUI::OnDestroy()
{
	BackGroundEnable( false );
}

void KGCMatchResultUI::FrameMoveInEnabledState()
{
    if( m_iGameEndCount > GAME_END_FRAME_COUNT )	//결과창을 X초 동안 보여 줍니다.  FPS = 55
	{
		 g_pStateGame->EndGame();
		 m_iGameEndCount = 0;
		 return;
	}
	++m_iGameEndCount;


    // 오전 10:05 2008-03-18    by jemitgge
    // 죽었을때 이모티콘 쓸 수 있게 해주쟈!!
    if( g_MyD3D->MyCtrl->k_ShortCut_Emot != KEY_UP )
    {
        g_MyD3D->GetMyShortCutSlot()->UseEmot(static_cast<INT>(g_MyD3D->MyCtrl->k_ShortCut_Emot));
        g_MyD3D->MyCtrl->k_ShortCut_Emot = KEY_UP;
    }
}

void KGCMatchResultUI::SetGuildPanel( std::pair<std::wstring, std::wstring>& prGuildMarkName_, std::pair<std::wstring, std::wstring>& prGuildName_ )
{            
    SiKGCIconImageManager()->LoadIconImageFromHTTP(prGuildMarkName_.first);
    SiKGCIconImageManager()->LoadIconImageFromHTTP(prGuildMarkName_.second);

    if ( prGuildMarkName_.first != L""  && prGuildMarkName_.second !=L"" )
    {
        m_pkGuildmarkIcon_R->ToggleRender( false );
        m_pkGuildmarkIcon_L->ToggleRender( false );
        m_pkGuildImage_L->SetTextureName(prGuildMarkName_.first);
        m_pkGuildImage_R->SetTextureName(prGuildMarkName_.second);
        m_pkGuildmarkIcon_R->ToggleRender( false );
        m_pkGuildmarkIcon_L->ToggleRender( false );
        CheckGuildMark( prGuildMarkName_ , prGuildName_ );
    }
    else if (prGuildName_.first == L"" &&  prGuildName_.second != L"" )
    {
        m_pkGuildImage_L->ToggleRender( false );
        m_pkGuildmarkIcon_L->ToggleRender( false );
        m_pkGuildmarkIcon_R->ToggleRender( true );
        m_pkGuildImage_R->SetTextureName( prGuildMarkName_.second );
        CheckGuildMark( prGuildMarkName_ , prGuildName_ );
    }
    else if ( prGuildName_.second == L"" && prGuildName_.first != L"" )
    {
        m_pkGuildImage_R->ToggleRender( false );
        m_pkGuildmarkIcon_R->ToggleRender( false );
        m_pkGuildmarkIcon_L->ToggleRender( true);
        m_pkGuildImage_L->SetTextureName( prGuildMarkName_.first );
        CheckGuildMark( prGuildMarkName_ , prGuildName_ );
    }
    else
    {
        m_pkGuildmarkIcon_L->ToggleRender( true );
        m_pkGuildmarkIcon_R->ToggleRender( true );
        m_pkGuildImage_L->ToggleRender( false );
        m_pkGuildImage_R->ToggleRender( false );
    }
}

void KGCMatchResultUI::CheckGuildMark( std::pair<std::wstring, std::wstring>& prGuildMarkName_ , std::pair<std::wstring, std::wstring>& prGuildName_)
{
    if (prGuildMarkName_.first == L"defaultmark.dds" || ( prGuildName_.first != L"" && prGuildMarkName_.first == L"")  ) {
        m_pkGuildmarkIcon_L->ToggleRender( true );
        m_pkGuildImage_L->ToggleRender( false );
    }
    else if (prGuildMarkName_.second == L"defaultmark.dds" || ( prGuildName_.second != L"" && prGuildMarkName_.second == L"") ) {
        m_pkGuildmarkIcon_R->ToggleRender( true );
        m_pkGuildImage_R->ToggleRender( false );
    }
}
