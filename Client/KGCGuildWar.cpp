#include "stdafx.h"
//
#include "KGCGuildWar.h"

#include "KMci.h"
#include "Headup Display.h"
#include "Monster.h"

#include "KGCGameBoard.h"

#include "KGCChannelManager.h"
#include "KGCRoomManager.h"
#include "KGCEmbarkManager.h"
#include "KGCSubjectManager.h"


KGCGuildWar::KGCGuildWar(void)
{
}

KGCGuildWar::~KGCGuildWar(void)
{
}

bool KGCGuildWar::CheckGameEnd()
{
	// 길드전에서는 두팀중 다 죽은팀이 있으면 끝난다
	int iRedTeam = 0;
	int iBlueTeam = 0;
	for( int i=0 ; i<MAX_PLAYER_NUM ; ++i )
	{
		if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[i]->m_cLife > 0 )
		{
			if( g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam() )
				++iBlueTeam;
			else
				++iRedTeam;
		}
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
    
    return false;
}

void KGCGuildWar::GameEnd()
{
	// 옵져버일때
	if( g_kGlobalValue.ServerInfo.CheckServerType( ST_BROADCASTING ) && g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN )
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

HRESULT KGCGuildWar::CalcExpAndGP()
{
	g_MyD3D->MVP = MAX_PLAYER_NUM + 1;
    if ( GC_GMT_TEAM_FIGHT == SiKGCRoomManager()->GetGameModeType() ) // 길드전
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
                    // 이겼을 경우의 Exp의 20%만 받고 GP를 변화시킨다.
					g_pGameBoard->m_pStartingMember[i].IsWin = false;

                    if( g_pGameBoard->m_pStartingMember[i].m_iChar != GC_CHAR_INVALID )
                        SetPlayerGameResult( i, g_pGameBoard->m_pStartingMember[i].m_iChar, 0.0f, 0.0f, false );

                    if ( i == g_MyD3D->Get_MyPlayer() )
                    {
                        // 길드전에서 패배하더라도 길드포인트가 2 오른다.
                        g_kGlobalValue.m_kUserInfo.iGuildScore += 2;
                    }
                }
                else // 이긴 팀의 팀원이면서 게임 도중에 나가지 않았다면 (승자 조건)
                {
                    // 승자의 기여도 점수를 계산한다.
                    // 공식은 다음과 같다.
                    // 점수 = 10 * 죽인 사람 수 + 남은 아레나 수 + 1(최소값)
                    aiScore[i] = 10 * g_pGameBoard->m_pStartingMember[i].How_Kill + g_MyD3D->MyPlayer[i]->m_cLife + 1;

                    g_pGameBoard->m_pStartingMember[i].IsWin = true;

                    if ( i == g_MyD3D->Get_MyPlayer() )
                    {
                        g_kGlobalValue.m_kUserInfo.iGuildScore += int( 10.0f * g_pGameBoard->m_pStartingMember[i].fGuildPointPercent );
                    }
                }
            }
        }

        int iBestScore = 0; // 최고 점수
        int iMVP       = 0; // MVP

         for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
        {
            // 이긴 팀의 팀원들에게 Exp와 Game point를 분배한다.
            // 여기서 게임 도중에 나간 사람은 제외한다.
            if ( ( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) || g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) ) &&
                g_pGameBoard->m_pStartingMember[i].dwUID == g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID &&
                g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam() == IsBlueWin )
            {
                // 최고 점수를 가진 사람을 찾는다.
                if ( iBestScore < aiScore[i] ) 
					iBestScore = aiScore[( iMVP = i )];
                if( g_pGameBoard->m_pStartingMember[i].m_iChar != GC_CHAR_INVALID )
                    SetPlayerGameResult( i, g_pGameBoard->m_pStartingMember[i].m_iChar, 1.0f, 1.0f, true );
            }
        }

        // MVP를 선정한다.
        g_MyD3D->MVP = iMVP;
    }
    return S_OK;
}

HRESULT KGCGuildWar::CustomFrameMove( float fElapsedTime )
{
    PROFILE_SET("KGCGuildWar::CustomFrameMove");

	SendP2PPacket( GC_PID_RELAY_PING );
	g_MyD3D->Story++;

    SiKGCEmbarkManager()->FrameMove();

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
		g_MyD3D->m_akPet[i]->FrameMove();
	}

    for(int i=0;i<MAX_OBJECT;i++)
    {
        if( g_MyD3D->m_pObject[i] )
        {
            g_MyD3D->m_pObject[i]->FrameMove();			
        }
    }

    SiKGCSubjectManager()->FrameMove();

    return S_OK;
}


HRESULT KGCGuildWar::InitAtLoading()
{
    //등장하는 대포의 사운드를 로딩해야 합니다.
    return S_OK;
}
