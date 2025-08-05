#include "stdafx.h"

#include "KGCGuildMatch_CapTain.h"

#include "KMci.h"
#include "Headup Display.h"
#include "Monster.h"
#include "KGCGameBoard.h"


#include "KGCRoomManager.h"
#include "gcui/GCGameOverlayUI.h"

#include "KGCRoomManager.h"
#include "KGCEmbarkManager.h"
#include "KGCSubjectManager.h"


KGCGuildMatch_CapTain::KGCGuildMatch_CapTain(void)
:iWinningTeam(-1)
{
	for ( int i = 0; i < TEAM_NUM; ++i )
	{
		m_iCapTainIndex[i] = -1;
	}

	teamSerdin.clear();
	teamCanaban.clear();
}

KGCGuildMatch_CapTain::~KGCGuildMatch_CapTain(void)
{
}

HRESULT KGCGuildMatch_CapTain::InitAtGame()
{
	// 게임 시작시 들어온다! ㅎ
	for ( int i = 0; i < TEAM_NUM; ++i )
	{
		m_iCapTainIndex[i] = -1;
	}

	// 피 초기화 따위 해줘야하나? 
	for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
	{
		g_MyD3D->MyPlayer[i]->SetHPFull();
	}

	EqualizeStat( true, true );

	return S_OK;
}

HRESULT KGCGuildMatch_CapTain::CalcExpAndGP()
{
	int aiScore[MAX_PLAYER_NUM] = { 0, }; // 점수 (기여도에 따른 개별 점수)

	for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
	{
		// 게임 중 나갔던, 계속 있던, 처음에 있었다면
		if ( g_pGameBoard->m_pStartingMember[i].IsLive && g_pGameBoard->m_pStartingMember[i].dwUID == g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID )
		{
			if ( false == g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive ) // 게임 도중에 나갔다면 (패자 조건)
			{
				// 패자의 Exp는 그대로 유지하고 GP를 변화시킨다.
				g_pGameBoard->m_pStartingMember[i].IsWin = false;

				if( g_pGameBoard->m_pStartingMember[i].m_iChar != GC_CHAR_INVALID )
					SetPlayerGameResult( i, g_pGameBoard->m_pStartingMember[i].m_iChar, 0.0f, 0.0f, false );
			}
			else if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam() != IsBlueWin ) // 진 팀의 팀원이라면 (패자 조건)
			{
				// 이겼을 경우의 Exp의 20%만 받고 GP를 변화시킨다. (천사구출작전은 제외)
				g_pGameBoard->m_pStartingMember[i].IsWin = false;

				if( g_pGameBoard->m_pStartingMember[i].m_iChar != GC_CHAR_INVALID )
					SetPlayerGameResult( i, g_pGameBoard->m_pStartingMember[i].m_iChar, 0.2f, 0.0f, false );
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
		// 이긴 팀의 팀원들에게 Exp와 Game point를 분배한다.
		// 여기서 게임 도중에 나간 사람은 제외한다.
		if ( g_pGameBoard->m_pStartingMember[i].IsLive && g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && 
			g_pGameBoard->m_pStartingMember[i].dwUID == g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID &&
			g_MyD3D->MyPlayer[i]->m_kUserInfo.IsBlueTeam() == IsBlueWin )
		{
			// 최고 점수를 가진 사람을 찾는다.
			if ( iBestScore < aiScore[i] ) iBestScore = aiScore[( iMVP = i )];
			if( g_pGameBoard->m_pStartingMember[i].m_iChar != GC_CHAR_INVALID )
				SetPlayerGameResult( i, g_pGameBoard->m_pStartingMember[i].m_iChar, 1.0f, 1.0f, true );
		}
	}

	// MVP를 선정한다.
	g_MyD3D->MVP = iMVP;
	return S_OK;
}

bool KGCGuildMatch_CapTain::CheckGameEnd()
{
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

	// 다 죽으면 끝이다! 나가도 끝이다!
	if ( iBlueTeam == 0 || iRedTeam == 0  )
	{
		IsBlueWin = iBlueTeam != 0 ;
		return true;
	}

	// 대장이 죽으면 끝이다!
	if ( g_MyD3D->MyPlayer[m_iCapTainIndex[TEAM_SERDIN]]->m_cLife <= 0 )
	{
		IsBlueWin = true;
		return true;
	}
	else if ( g_MyD3D->MyPlayer[m_iCapTainIndex[TEAM_CANABAN]]->m_cLife <= 0 )
	{
		IsBlueWin = false;
		return true;
	}

	//대장의 피회복을 막는다.
	g_MyD3D->MyPlayer[m_iCapTainIndex[TEAM_SERDIN]]->m_fIncHPRatio = 0.f;
	g_MyD3D->MyPlayer[m_iCapTainIndex[TEAM_CANABAN]]->m_fIncHPRatio = 0.f;

	// 대장이 나가면 대장을 마이그래이션 해준다~ 
	//if( g_kGlobalValue.m_kUserInfo.bHost == true )
	{
		static DWORD dwStartTime = ::timeGetTime();

		if ( ( 0 == g_pGameBoard->m_pStartingMember[m_iCapTainIndex[TEAM_SERDIN]].dwUID ||
			0 == g_pGameBoard->m_pStartingMember[m_iCapTainIndex[TEAM_CANABAN]].dwUID ) && 
			::timeGetTime() - dwStartTime > 3000 )
		{
			// 어느 팀의 대장이 바뀌었나.. 전달해준다... -_-;
			EqualizeStat( 0 == g_pGameBoard->m_pStartingMember[m_iCapTainIndex[TEAM_SERDIN]].dwUID,
				0 == g_pGameBoard->m_pStartingMember[m_iCapTainIndex[TEAM_CANABAN]].dwUID, true );

			dwStartTime = ::timeGetTime();
		}
	}

	// 엔드 체크할 때 돌려줘도 될듯;; 
	processDebuff();

	return false;
}

void KGCGuildMatch_CapTain::GameEnd()
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

	g_pkGameOverlayUI->SetTimer( false );
}

HRESULT KGCGuildMatch_CapTain::CustomFrameMove( float fElapsedTime )
{
    PROFILE_SET("KGCGuildMatch_CapTain::CustomFrameMove");

    SendP2PPacket( GC_PID_RELAY_PING );
	g_MyD3D->Story++;

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


void KGCGuildMatch_CapTain::EqualizeStat( bool blue, bool red, bool IsMigration /*= false */ )
{
	teamSerdin.clear();
	teamCanaban.clear();
	int serdinNum = 0;
	int canabanNum = 0;
	int oldCaptainIndex[TEAM_NUM] = { -1, -1 };

	for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
	{
		// 나간놈을 제외하고 팀 맴버에 넣은 후~
		if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[i]->m_kUserInfo.dwUID != 0 )
		{
			if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.iTeam == TEAM_SERDIN && blue )
			{
				teamSerdin.push_back(i);
				serdinNum++;
			}
			else if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.iTeam == TEAM_CANABAN && red )
			{
				teamCanaban.push_back(i);
				canabanNum++;
			}
		}
	}

	if( (canabanNum == 0 && red) || (serdinNum == 0 && blue) )
		return;

	// 대장 마이그래이션 중이면 예전 대장의 인덱스 구한다~!
	if ( IsMigration )
	{
		if ( blue )
			oldCaptainIndex[TEAM_SERDIN] = m_iCapTainIndex[TEAM_SERDIN];
		if ( red )
			oldCaptainIndex[TEAM_CANABAN] = m_iCapTainIndex[TEAM_CANABAN];
	}

	// 대장을 정한다!
	if ( blue )
		m_iCapTainIndex[TEAM_SERDIN] = teamSerdin[ SiKGCRoomManager()->GetRoomInfo().uiRandSeed % serdinNum ];
	if ( red )
		m_iCapTainIndex[TEAM_CANABAN] = teamCanaban[ SiKGCRoomManager()->GetRoomInfo().uiRandSeed % canabanNum ];

	for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
	{
		// 대장이면 대장 버프를 건다!
		if ( i == m_iCapTainIndex[TEAM_SERDIN] && blue )
		{
			if ( true == g_MyD3D->MyPlayer[m_iCapTainIndex[TEAM_SERDIN]]->m_kUserInfo.bLive )
			{
				g_pMagicEffect->SetMagicEffect( i, EGC_EFFECT_CAPTAIN_BUFF, -1.0f );

				// 대장의 피를 회복시키지 않는다.
				g_MyD3D->MyPlayer[i]->m_fIncHPRatio = 0.f;

				if ( IsMigration )
				{
					g_MyD3D->MyPlayer[i]->m_cLife = g_MyD3D->MyPlayer[oldCaptainIndex[TEAM_SERDIN]]->m_cLife;

					//대장이 죽어가다가 나가버리면 Life는 깎여있고 피는 아직 안찬 상태이다.
					//죽어가다가 나간 것이라면 피는 채워주자.
					if( g_MyD3D->MyPlayer[oldCaptainIndex[TEAM_SERDIN]]->m_iArenaCoolTime > 0 )
						g_MyD3D->MyPlayer[i]->SetHPFull();
					else
						g_MyD3D->MyPlayer[i]->SetHP( g_MyD3D->MyPlayer[oldCaptainIndex[TEAM_SERDIN]]->GetHP() );
				}
				else
					g_MyD3D->MyPlayer[i]->m_cLife = 2;
			}
		}
		// 대장이면 대장 버프를 건다!
		else if ( i == m_iCapTainIndex[TEAM_CANABAN] && red )
		{
			if ( true == g_MyD3D->MyPlayer[m_iCapTainIndex[TEAM_CANABAN]]->m_kUserInfo.bLive )
			{
				g_pMagicEffect->SetMagicEffect( i, EGC_EFFECT_CAPTAIN_BUFF, -1.0f );

				// 대장의 피를 회복시키지 않는다.
				g_MyD3D->MyPlayer[i]->m_fIncHPRatio = 0.f;

				if ( IsMigration )
				{
					g_MyD3D->MyPlayer[i]->m_cLife = g_MyD3D->MyPlayer[oldCaptainIndex[TEAM_CANABAN]]->m_cLife;

					if  ( g_MyD3D->MyPlayer[oldCaptainIndex[TEAM_CANABAN]]->GetHP() == 0.0f )
						g_MyD3D->MyPlayer[i]->SetHP( 0.1f );
					else
						g_MyD3D->MyPlayer[i]->SetHP( g_MyD3D->MyPlayer[oldCaptainIndex[TEAM_CANABAN]]->GetHP() );
				}
				else
					g_MyD3D->MyPlayer[i]->m_cLife = 2;
			}
		}
		// 대장이 아니면 무한 생명!
		else if ( i != m_iCapTainIndex[TEAM_SERDIN] && i != m_iCapTainIndex[TEAM_CANABAN] 
		&& i != oldCaptainIndex[TEAM_SERDIN] && i != oldCaptainIndex[TEAM_CANABAN] )
		{
			g_MyD3D->MyPlayer[i]->m_cLife = 120;
		}
	}
}

int KGCGuildMatch_CapTain::IsCaptainIndex( int _playerIndex )
{
	if ( _playerIndex == m_iCapTainIndex[0] )
		return TEAM_SERDIN;
	else if ( _playerIndex == m_iCapTainIndex[1] )
		return TEAM_CANABAN;

	return -1;
}

void KGCGuildMatch_CapTain::processDebuff( void )
{
	// 대장이 죽으면 상대편 대장에게 디버프를 건다.. ( 나쁜 자식!! 곱게죽지.. )
	int myIndex = g_MyD3D->Get_MyPlayer();
	if ( myIndex == m_iCapTainIndex[TEAM_CANABAN] )
	{
		if ( PS_DEAD == g_MyD3D->MyPlayer[m_iCapTainIndex[TEAM_SERDIN]]->GetPlayerState() &&
			!g_pMagicEffect->IsMagicEffect( myIndex, EGC_EFFECT_CAPTAIN_DEBUFF ) )
		{
			g_pMagicEffect->SetMagicEffect( myIndex, EGC_EFFECT_CAPTAIN_DEBUFF, 10.0f );
		}
	}
	else if ( myIndex == m_iCapTainIndex[TEAM_SERDIN] )
	{
		if ( PS_DEAD == g_MyD3D->MyPlayer[m_iCapTainIndex[TEAM_CANABAN]]->GetPlayerState() &&
			!g_pMagicEffect->IsMagicEffect( myIndex, EGC_EFFECT_CAPTAIN_DEBUFF ) )
		{
			g_pMagicEffect->SetMagicEffect( myIndex, EGC_EFFECT_CAPTAIN_DEBUFF, 10.0f );
		}
	}
}