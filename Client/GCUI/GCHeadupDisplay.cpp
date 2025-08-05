#include "stdafx.h"
#include "GCHeadupDisplay.h"
#include "GCOnGameExpBar.h"
#include "GCPlayerStatusBar.h"

#include "KGCComboEffect.h"
//
#include "../MyD3D.h"


#include "Replay.h"
#include "KGCRoomManager.h"
#include "GCMyHeadup.h"
#include "KGCPingAntenna.h"
#include "gcui/KGCPlayerRank.h"
#include <KNC/ClientErr.h>

IMPLEMENT_CLASSNAME( KGCHeadupDisplay );
IMPLEMENT_WND_FACTORY( KGCHeadupDisplay );
IMPLEMENT_WND_FACTORY_NAME( KGCHeadupDisplay, "gc_headup_display" );

KGCHeadupDisplay* g_pkHUD = NULL;

KGCHeadupDisplay::KGCHeadupDisplay( void )
{
	g_pkHUD             = this;	
	m_iPlayerIdx        = 0;

	m_pMyHeadup         = NULL;
	m_pkVS              = NULL;
	m_bShowPlayerStatus	= true;
    for( int i = 0 ; i < (int)m_apkPlayerStatusBar.size() ; ++i )
    {
        m_apkPlayerStatusBar[i] = NULL;
    }

    m_pkPlayerRank = NULL;

    m_pkMyPingAntenna = NULL;
    m_pkComboUIS6 = NULL;
	
	LINK_CONTROL( "player_status_bar0",  m_apkPlayerStatusBar[0] );
	LINK_CONTROL( "player_status_bar1",  m_apkPlayerStatusBar[1] );
	LINK_CONTROL( "player_status_bar2",  m_apkPlayerStatusBar[2] );
	LINK_CONTROL( "player_status_bar3",  m_apkPlayerStatusBar[3] );
	LINK_CONTROL( "player_status_bar4",  m_apkPlayerStatusBar[4] );
	LINK_CONTROL( "player_status_bar5",  m_apkPlayerStatusBar[5] );
	LINK_CONTROL( "headup_vs",           m_pkVS );

    LINK_CONTROL( "on_game_exp_bar",     m_pkExpBar );
	LINK_CONTROL( "my_head",             m_pMyHeadup );

    LINK_CONTROL( "my_ping_antenna",     m_pkMyPingAntenna );
    LINK_CONTROL( "player_rank" ,        m_pkPlayerRank );
    
    LINK_CONTROL( "combo_ui_s6",         m_pkComboUIS6 );
	
}

KGCHeadupDisplay::~KGCHeadupDisplay( void )
{
	g_pkHUD = NULL;
}

void KGCHeadupDisplay::OnCreate( void )
{    
	m_iPlayerIdx = g_MyD3D->Get_MyPlayer();    
}

void KGCHeadupDisplay::OnInitialize()
{
    m_iPlayerIdx = g_MyD3D->Get_MyPlayer();

	float fScalex = 1.0f, fScaley = 1.0f;
	// 게임내 UI크기는  1024x768베이스로 맞춘다. 
	if (g_pGCDeviceManager2->GetWindowScaleX() >= 1024.0f ) 
		fScalex = g_pGCDeviceManager2->GetWindowScaleX() / (1024.0f);

	// 게임내 UI크기는 1024x768베이스로 맞춘다. 
	if (g_pGCDeviceManager2->GetWindowScaleY() >= 768.0f ) 
		fScaley = g_pGCDeviceManager2->GetWindowScaleY() / (768.0f );

	m_pkExpBar->SetAlignType(WNDUIAT_POSBOTTOMBASE);
	m_pkExpBar->SetWindowScale(D3DXVECTOR2(fScalex,fScaley));
	//m_pkExpBar->SetAlignGap(m_pkExpBar->GetCurrentWindowPos());

	for (int i=0;i<6;i++)
	{
		m_apkPlayerStatusBar[i]->SetAlignType(WNDUIAT_POSRIGHTTOPBASE);
		m_apkPlayerStatusBar[i]->SetWindowScale(D3DXVECTOR2(fScalex,fScaley));
		//m_apkPlayerStatusBar[i]->SetAlignGap(m_apkPlayerStatusBar[i]->GetCurrentWindowPos());
	}
	m_pkVS->SetAlignType(WNDUIAT_POSZEROBASE);
	m_pkVS->SetWindowScale(D3DXVECTOR2(fScalex,fScaley));

    for ( int i = 0; i < NUM_STATUS_BAR; ++i )
    {
        m_apkPlayerStatusBar[i]->SetSelfInputCheck( false );
        m_apkPlayerStatusBar[i]->ToggleRender( false );
    }

    m_pkVS->SetSelfInputCheck( false );
    m_pkVS->ToggleRender( false );

#if defined(NATION_BRAZIL) || defined(NATION_IDN)
	if( g_MyD3D->IsLocalPlayerObserver() == true)
	{
		m_pMyHeadup->ToggleRender( false );		
	}
	else
	{		
		m_pMyHeadup->ToggleRender( true );
	}

#else
    m_pMyHeadup->ToggleRender( true );
#endif

    m_pkExpBar->InitGauge();   

    if( SiKGCRoomManager()->GetGameMode() == GC_GM_TUTORIAL ||
        g_MyD3D->MyReplay->m_iCurrState == RP_VIEW ||
        g_MyD3D->MyReplay->m_iCurrState == RP_PREPARE ||
        g_MyD3D->IsLocalPlayerObserver()  )
    {
        m_pkExpBar->ToggleRender( false );
    }
    else
    {
        // 만렙이라면 EXP 바가 필요없다!! 훗..-ㅅ -
        if( Exp_2_Level( g_MyD3D->MyPlayer[m_iPlayerIdx]->GetCurrentChar().biExp ) >= g_kGlobalValue.GetMaxLevel() )
        {
            m_pkExpBar->ToggleRender( false );
        }
        else
        {
            m_pkExpBar->ToggleRender( true );
            m_pkExpBar->SetAllWindowColor( 0x99FFFFFF );
            float fExp = (float)g_MyD3D->MyPlayer[m_iPlayerIdx]->GetCurrentChar().biExp;
            m_pkExpBar->SetExp( fExp );
            m_pkExpBar->SetVirtualExp( fExp );
        }
    }

    m_pkMyPingAntenna->SetAlignType(WNDUIAT_POSZEROBASE);
	m_pkMyPingAntenna->SetWindowScale(D3DXVECTOR2(fScalex,fScaley));

#if defined(USE_PING_STATE_UI)
    m_pkMyPingAntenna->InitState(true);
#else
    m_pkMyPingAntenna->InitState(false);
#endif

	if(SiKGCRoomManager()->GetGameMode() == GC_GM_QUEST82 )
		m_pkPlayerRank->InitState(false);
	else
		m_pkPlayerRank->InitState(true);
}

void KGCHeadupDisplay::OnDestroy( void )
{
}

void KGCHeadupDisplay::FrameMoveInEnabledState( void )
{
	if ( m_bShow && ( m_iPlayerIdx < 0 || MAX_PLAYER_NUM <= m_iPlayerIdx ) ) 
		return;
	
	// vs 표시
	m_pkVS->ToggleRender( (GC_GMT_TEAM_FIGHT == SiKGCRoomManager()->GetGameModeType()) || (GC_GM_TUTORIAL == SiKGCRoomManager()->GetGameMode()) );

	// 플레이어 상태바
	SetPlayerStatusBar();
}

void KGCHeadupDisplay::PostChildDraw( void )
{
}

void KGCHeadupDisplay::ShowHUD( bool bShow, bool bShowPlayerStatus )
{
	m_bShow = bShow;
	m_pMyHeadup->ToggleRender( bShow );
	m_bShowPlayerStatus = bShowPlayerStatus;
}

void KGCHeadupDisplay::SetPlayerStatusBar( void )
{	
	if( !m_bShowPlayerStatus )
		return;

	int iOffset = ( m_bShow ? 1 : 0 );

	for ( int i = 0; i < NUM_STATUS_BAR; ++i )
	{
		m_apkPlayerStatusBar[i]->ToggleRender( false );
		m_apkPlayerStatusBar[i]->SetPlayerIndex( -1 );
	}

	if ( GC_GMT_TEAM_FIGHT == SiKGCRoomManager()->GetGameModeType() ) // 팀을 이루어 싸우는 모드라면
	{
		for ( int cnt1 = iOffset, cnt2 = 0, i = 0; i < MAX_PLAYER_NUM; ++i )
		{
			if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING )
                continue;

			if ( m_bShow && m_iPlayerIdx == i )
				continue;

			// 같은 팀이라면 왼쪽에 위치, 다른 팀이라면 오른쪽에 위치
			if ( g_MyD3D->IsSameOriginalTeam(m_iPlayerIdx, i) )
			{
				m_apkPlayerStatusBar[cnt1]->ToggleRender( true );
				m_apkPlayerStatusBar[cnt1]->SetPlayerIndex( i );
				++cnt1;
			}
			else
			{
				m_apkPlayerStatusBar[cnt2+3]->ToggleRender( true );
				m_apkPlayerStatusBar[cnt2+3]->SetPlayerIndex( i );
				++cnt2;
			}
		}
	}
	else // 개별적으로 싸우는 모드라면
	{
		for ( int cnt = iOffset, cnt2 = 0, i = 0; i < MAX_PLAYER_NUM; ++i )
		{
            if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState != GC_RUS_PLAYING )
                continue;

			if ( m_bShow && m_iPlayerIdx == i ) 
				continue; // 나 자신은 제외

            
			if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TUTORIAL )
			{
                ++cnt2;
                m_apkPlayerStatusBar[3]->ToggleRender( true );
                m_apkPlayerStatusBar[3]->SetPlayerIndex( 1 );
			}
            else
            {
                m_apkPlayerStatusBar[cnt]->ToggleRender( true );
                m_apkPlayerStatusBar[cnt]->SetPlayerIndex( i );
                ++cnt;
            }
		}
	}
}

int KGCHeadupDisplay::GetPlayerIndex(int slot)
{
	if(slot < 0 || slot >= MAX_PLAYER_NUM)
		return -1;

	return m_apkPlayerStatusBar[slot]->GetPlayerIndex();
}

void KGCHeadupDisplay::SetRoomMemberPingInfo(std::vector<std::pair<DWORD,DWORD>>& vecRoomMemberPingInfo_ )
{
    for( int i = 0; i < static_cast<int>(vecRoomMemberPingInfo_.size()); i++ )
    {
        if( g_kGlobalValue.m_kUserInfo.dwUID == vecRoomMemberPingInfo_[i].first )
            m_pkMyPingAntenna->SetPingState(vecRoomMemberPingInfo_[i].second);
    }

    for( int i = 0; i < NUM_STATUS_BAR; i++ )
    {
        if( m_apkPlayerStatusBar[i] )
        {
            m_apkPlayerStatusBar[i]->SetRoomMemberPingInfo(vecRoomMemberPingInfo_);
        }
    }
}

void KGCHeadupDisplay::SetPlayerRank( float fRankPoint )
{
	//printf("%f rank point\n", fRankPoint);
    m_pkPlayerRank->SetPlayerRank(fRankPoint);
}
