#include "stdafx.h"
//
#include "KGCMatch.h"

#include "KMci.h"
#include "Headup Display.h"
#include "Monster.h"
#include "KGCGameBoard.h"

#include "KGCChannelManager.h"
#include "KGCRoomManager.h"

#include "../Message.h"
#include "GCUI/KGCPrograssBar.h"
#include "KGCEmbarkManager.h"
#include "KGCSubjectManager.h"


KGCMatch::KGCMatch(void)
{
	m_bFastMode = false;
	m_bFatalMode = false;
    m_uiFrameCount = 0;
}

KGCMatch::~KGCMatch(void)
{
}

HRESULT KGCMatch::InitAtGame()
{
	m_bFastMode = false;
	m_bFatalMode = false;
    m_uiFrameCount = 0;

    CheckStartPlayer();

	return S_OK;

}

HRESULT KGCMatch::CalcExpAndGP()
{
	g_MyD3D->MVP = MAX_PLAYER_NUM + 1;
    
    if( GC_GMT_ALONE_FIGHT == SiKGCRoomManager()->GetGameModeType() ) // 서바이벌 모드
    {
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // Description by parkch
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // 서바이벌 모드의 승패 결정 방식
        // 승자 - 전체 순위에서 반등 이상이고 게임 도중에 나가지 않은 사람
        // 패자 - 전체 순위에서 반등 이하이거나 게임 도중에 나간 사람
        //
        // 서바이벌 모드의 점수 배분 방식
        // 1. 패자들의 Exp는 유지되며 각 레벨에 맞는 하락치로 GP가 하락한다.
        // 2. 승자들의 Exp와 GP는 각 레벨에 맞는 상승치로 상승한다.
        // 3. 순위가 제일 높은 사람이 MVP가 된다.
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        int aiRank[MAX_PLAYER_NUM] = { 0, }; // 순위
        int iNumPlayer             = 0;      // 총 인원

        // 게임에 참여했던 총 인원을 구한다.
        for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
        {
            // 게임을 최초로 시작했을 때의 인원을 기준으로 한다.i
            if ( g_pGameBoard->m_pStartingMember[i].IsLive )
            {
                ++iNumPlayer;
            }
        }

        for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
        {
            // 게임 중 나갔던, 계속 있던, 처음에 있었다면
            if( g_pGameBoard->m_pStartingMember[i].IsLive )
            {
                // 등수를 결정하기 위한 점수를 계산한다.
                // 공식은 다음과 같다.
                // 점수 = 죽인 사람 수 + 남은 아레나 수 * 10
                int iMyScore = static_cast<int>( g_pGameBoard->m_pStartingMember[i].How_Kill )
                    + static_cast<int>( g_MyD3D->MyPlayer[i]->m_cLife ) * 10;

                // 점수로 등수를 결정한다.
                aiRank[i] = 1;
                for ( int j = 0; j < MAX_PLAYER_NUM; ++j )
                {
                    // 다른 사람들의 점수와 비교해서 순위를 조정한다.
                    // (게임 도중에 나간 플레이어는 등수 계산에서 빠진다.)
                    if ( i != j && g_MyD3D->MyPlayer[j]->m_kUserInfo.bLive )
                    {
                        // 다른 사람의 점수를 계산한다.
                        // 공식은 다음과 같다.
                        // 점수 = 죽인 사람 수 + 남은 아레나 수 * 10
                        int iScore = static_cast<int>( g_pGameBoard->m_pStartingMember[j].How_Kill )
                            + static_cast<int>( g_MyD3D->MyPlayer[j]->m_cLife ) * 10;

                        // 자신의 점수보다 높으면 자신의 등수를 떨어뜨린다.
                        // (점수가 같은 경우, 인덱스가 높은 쪽의 등수를 떨어뜨린다.)
                        if ( ( iScore > iMyScore ) || ( iScore == iMyScore && i > j ) )
                        {
                            ++aiRank[i];
                        }
                    }
                }
                if ( aiRank[i] > iNumPlayer / 2 ) // 전체 순위에서 반등 이하이거나 (패자 조건)
                {
					g_pGameBoard->m_pStartingMember[i].IsWin = false;
                }
                else // 전체 순위에서 반등 이상이고 게임 도중에 나가지 않았다면 (승자 조건)
                {
					g_pGameBoard->m_pStartingMember[i].IsWin = true;

                    // 1등을 MVP로 선정한다.
                    if ( 1 == aiRank[i] ) g_MyD3D->MVP = i;
                }
            }
        }
    }
    //else if ( GC_GMT_TEAM_FIGHT == SiKGCRoomManager()->GetGameModeType() ) // 팀 모드
    else if ( GC_GMT_TEAM_FIGHT == SiKGCRoomManager()->GetGameModeType() ) // 팀 모드
    {
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // Description by parkch
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // 길드전의 승패 결정 방식
        // 승자 - 이긴 팀의 팀원이면서 게임 도중에 나가지 않은 사람
        // 패자 - 게임 도중에 나갔거나 진 팀의 팀원
        //
        // 팀 모드의 점수 배분 방식
        // 1. 패자들의 Exp는 유지되며 각 레벨에 맞는 하락치로 GP가 하락한다.
        // 2. 승자들의 Exp와 GP는 각 레벨에 맞는 상승치로 상승한다.
        // 3. 승자들 중 기여도가 제일 높은 사람이 MVP가 된다.
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        int aiScore[MAX_PLAYER_NUM] = { 0, }; // 점수 (기여도에 따른 개별 점수)

        for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
        {
            // 게임 중 나갔던, 계속 있던, 처음에 있었다면
            if( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) || g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) )
            {
                if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam() != IsBlueWin ) // 진 팀의 팀원이라면 (패자 조건)
                {
                    // 이겼을 경우의 Exp의 20%만 받고 GP를 변화시킨다. (천사구출작전은 제외)
					g_pGameBoard->m_pStartingMember[i].IsWin = false;
                }
                else // 이긴 팀의 팀원이면서 게임 도중에 나가지 않았다면 (승자 조건)
                {
                    // 승자의 기여도 점수를 계산한다.
                    // 공식은 다음과 같다.
                    // 점수 = 10 * 죽인 사람 수 + 남은 아레나 수 + 1(최소값)
                    aiScore[i] = 10 * g_pGameBoard->m_pStartingMember[i].How_Kill + g_MyD3D->MyPlayer[i]->m_cLife + 1;

                    g_pGameBoard->m_pStartingMember[i].IsWin = true;
                }
            }
        }

        int iBestScore = 0; // 최고 점수
        int iMVP       = 0; // MVP

        for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
        {
            if( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) || g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) )
            {
                // 최고 점수를 가진 사람을 찾는다.
                if ( iBestScore < aiScore[i] ) iBestScore = aiScore[( iMVP = i )];                
            }
        }

        // MVP를 선정한다.
        g_MyD3D->MVP = iMVP;
    }

    if( SiKGCEventManager()->IsActiveEvent(EVENT_ID_FATAL_MATCH) )
    {
        for ( int i = 0 ; i < MAX_PLAYER_NUM; i++ )
        {
            g_pGameBoard->m_pStartingMember[i].IsWin = true;
        }
    }
    
	return S_OK;
}

void KGCMatch::CheckFastMode()
{
	// 방장만 관리한다.
	if( false == g_kGlobalValue.m_kUserInfo.bHost )
		return;

	// 승패가 결정났으면 Fast Match 검사 안합니다.
	if( IsDetermineWin )
		return;

	if ((g_MyD3D->Num_Alive == 2 && m_iNumNet > 2 ) ||
		//대회 진행시에는 3분이 지나면 패스트 모드가 됨.
		( g_MyD3D->Story >=  GC_FPS_LIMIT * 60 * 3 &&
		( g_kGlobalValue.ServerInfo.CheckServerType( ST_BROADCASTING ) &&
		( g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_BALANCE )))))
	{
		unsigned char Prepare = 3;

		// 제일 작은 Life를 가진 플레이어를 찾아내고 살아있는 두명의 플레이어의 IsBlueTeam을 빼냄.
		bool First = true;
		for( int i = 0; i < MAX_PLAYER_NUM; ++i )
		{
			if( !g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
				continue;
			if( g_MyD3D->MyPlayer[i]->m_cLife )
			{
				if( g_MyD3D->MyPlayer[i]->m_cLife < Prepare )
					Prepare = g_MyD3D->MyPlayer[i]->m_cLife;
			}
		}

        LTP_MATCH_QUICK_MODE sMatchMode;
        sMatchMode.cQuickType = LTP_MATCH_QUICK_MODE::FAST_MODE;
        sMatchMode.nLife = Prepare - 1;
        g_LatencyTransfer.PushPacket( &sMatchMode, 1 );

		m_bFastMode = true;
	}
}

void KGCMatch::CheckFatalMode()
{
    if ( m_bFatalMode )
        return;

    if ( !g_kGlobalValue.m_kUserInfo.bHost )
        return;


    // 대회 진행시에는 5분이 지나면 페이탈 모드가 됨
    if(	(g_MyD3D->Story >=  GC_FPS_LIMIT * 60 * 5 &&  
        ( g_kGlobalValue.ServerInfo.CheckServerType( ST_BROADCASTING ) &&
        ( g_kGlobalValue.ServerInfo.CheckServerType( ST_PVP_BALANCE ) ) )))
    {
        
        LTP_MATCH_QUICK_MODE sMatchMode;
        sMatchMode.cQuickType = LTP_MATCH_QUICK_MODE::FATAL_MODE;
        g_LatencyTransfer.PushPacket( &sMatchMode, 1 );
        
        m_bFatalMode = true;
    }
}



bool KGCMatch::CheckGameEnd()
{
	if( SiKGCRoomManager()->GetGameMode() == GC_GM_SURVIVAL_MODE )
	{
		// 서바이벌 모드일때는 한명빼고 다 죽으면 게임이 끝난다
		int iLive = 0;
		for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
		{
            if( ( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) || g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) )
                && g_MyD3D->MyPlayer[i]->m_cLife > 0 )
			{
				++iLive;
			}
		}

		if( iLive <= 1 )
			return true;
		else 
			return false;
	}
	else if( SiKGCRoomManager()->GetGameMode() == GC_GM_TEAM_MODE || SiKGCRoomManager()->GetGameMode() == GC_GM_AUTOMATCH_TEAM )
	{
		// 팀 모드일때는 다죽은 팀이 있으면 게임이 끝난다.
		int iRedTeam = 0;
		int iBlueTeam = 0;
        int iLive = 0;
        bool bPublicEnemy = false;
		for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
		{            
            if( ( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) || g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) )
                && g_MyD3D->MyPlayer[i]->m_cLife > 0 )
			{
				if( g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam() )
					++iBlueTeam;
				else
					++iRedTeam;

                if( g_MyD3D->MyPlayer[i]->m_kUserInfo.iTeam == TEAM_PUBLIC_ENEMY ) {
                    bPublicEnemy = true;
                }

                iLive++;
			}
		}

        // 공용적이 존재하고 2명이상이 남아 있으면 게임을 종료시키지 말자.
        if( iLive >= 2 && bPublicEnemy ) {
            return false;
        }

		if( iBlueTeam == 0 || iRedTeam == 0 )
		{
			IsBlueWin = ( iBlueTeam != 0 );

			
            return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void KGCMatch::GameEnd()
{
	// 옵져버일때
	if( g_MyD3D->IsLocalPlayerObserver() )
	{                            
		g_MyD3D->MyHead->Start_Text("GAME OVER");
		return;
	}

	if( g_pGameBoard->m_pStartingMember[g_MyD3D->Get_MyPlayer()].IsWin )
	{
		g_KDSound.Play( "89" );
		g_KMci.Play( GC_BGM_GAME_WIN, false );
		g_MyD3D->MyHead->Start_Text("WIN");
	}
	else
	{
		g_KDSound.Play( "90" );
		g_KMci.Play( GC_BGM_GAME_LOSE, false );
		g_MyD3D->MyHead->Start_Text("LOSE");
	}
}

HRESULT KGCMatch::CustomFrameMove( float fElapsedTime )
{
    PROFILE_SET("KGCMatch::CustomFrameMove");

	SendP2PPacket( GC_PID_RELAY_PING );
	g_MyD3D->Story++;
    ++m_uiFrameCount;

	if( m_bFastMode == false ){
        CheckFastMode();
	}

    if ( m_bFatalMode == false )
    {
        CheckFatalMode();
    }

    SiKGCEmbarkManager()->FrameMove();

	// 플레이어에 대한 프레임 업데이트 및 충돌처리용 영역을 계산한다.
	for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
	{
		g_MyD3D->MyPlayer[i]->Calculate_Body_Rect();
        g_MyD3D->MyPlayer[i]->Frame_Move();
	}


    // 몬스터에 대한 프레임 업데이트
    g_kMonsterManager.FrameMove();	

	// 플레이어의 충돌을 체크한다.
	for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
	{
		g_MyD3D->MyPlayer[i]->Crash_Check();
	}

	// 펫에 대한 프레임 업데이트
	for( int i = 0; i < MAX_PLAYER_NUM; i++ )
	{
		if ( g_MyD3D->m_akPet[i]  == NULL)
            continue;

        g_MyD3D->m_akPet[i]->FrameMove();
	}
	
    // 프로그래스바 프레임무브
    SiKGCInGamePrograssBar()->FrameMove();

    for(int i=0;i<MAX_OBJECT;i++)
    {
        if( g_MyD3D->m_pObject[i] )
        {
            g_MyD3D->m_pObject[i]->FrameMove();			
        }
    }
	if(m_bFatalMode)
		g_MyD3D->GetMyPlayer()->SetHP( 0.0f );


	if( SiKGCEventManager()->IsActiveEvent(EVENT_ID_FATAL_MATCH)	)
    {
        float fDecreaseValue = g_MyD3D->GetMyPlayer()->m_fAbility[ABILITY_HP] / 275.0f;

        g_MyD3D->GetMyPlayer()->SetHP( g_MyD3D->GetMyPlayer()->GetHP() - fDecreaseValue );

        if ( g_MyD3D->GetMyPlayer()->GetHP() < 0.0f ||
            fDecreaseValue < 0.0f )
        {
            g_MyD3D->GetMyPlayer()->SetHP( 0.0f );
        }

        else
        {
            if( m_uiFrameCount == 150 )
            {
                g_pkUIScene->m_pkGameOverlayUI->SetTitleText( g_pkStrLoader->GetString( STR_ID_FATAL_MATCH_ALL_WIN ) );
            }
        }
    }

    SiKGCSubjectManager()->FrameMove();

	return S_OK;
}

