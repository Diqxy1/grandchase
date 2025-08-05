#include "stdafx.h"
//
#include ".\kgcminigame.h"
#include "MyD3D.h"



#include "GCCameraOrtho.h"
#include "KGCMinigameManager.h"
#include "KGCGameBoard.h"
#include ".\gcui\KGCMiniGameResult.h"
#include "gcui/KGCMiniGameScoreBoard.h"
#include ".\gcui\GCGameOverlayUI.h"
#include "GCStateMachine.h"

KGCMiniGame::KGCMiniGame(void)
:bEndOneGame( false )
,m_bShowStartMsg( false )
{
}

KGCMiniGame::~KGCMiniGame(void)
{
}

HRESULT KGCMiniGame::CustomFrameMove( float fElapsedTime )
{
    PROFILE_SET("KGCMiniGame::CustomFrameMove");

    g_MyD3D->Story++;
	if ( false == g_pkGameOverlayUI->GetMinigameResult()->IsRenderOn() )
	{
		g_pGameBoard->AdvancePlayTime();
	}

    g_MyD3D->MyPlayer[0]->Calculate_Body_Rect();
	g_MyD3D->MyPlayer[0]->Frame_Move();

	// 플레이어의 충돌을 체크한다.
	g_MyD3D->MyPlayer[0]->Crash_Check();

	// 펫에 대한 프레임 업데이트
	g_MyD3D->m_akPet[0]->FrameMove();


    for(int i=0;i<MAX_OBJECT;i++)
    {
        if( g_MyD3D->m_pObject[i] )
        {
            g_MyD3D->m_pObject[i]->FrameMove();			
        }
    }


	g_kCamera.SetCameraPosition( 0.0f, 0.0f );

    g_pkGameOverlayUI->GetMinigameScoreBoard()->SetCurrentScore( SiKGCMinigameManager()->GetCurrentScore() );
    SetCurrentGP();

	SiKGCMinigameManager()->FrameMove();

    ShowStartMsg();

	return S_OK;
}

HRESULT KGCMiniGame::CalcExpAndGP()
{
	return S_OK;
}

bool KGCMiniGame::CheckGameEnd()
{
	if( g_MyD3D->MyPlayer[0]->m_cLife == 0 )
    {
        return true;
    }
    else if ( true == SiKGCMinigameManager()->GetExitGame() )
    {
        return true;
    }
    else
        return false;
     /*   if ( false == bEndOneGame )
        {
            SetScoreInfo();
        }
        bEndOneGame = true;

        if ( true == SiKGCMinigameManager()->GetExitGame() )
        {
            return true;
        }
    }
	else if ( g_MyD3D->MyPlayer[0]->m_cLife != 0 && true == SiKGCMinigameManager()->GetExitGame() )
	{
		return true;
	}
    else
    {
        bEndOneGame = false;
    }

	return false;
    */
}

void KGCMiniGame::GameEnd()
{
	SiKGCMinigameManager()->EndMiniGame();
    g_pkGameOverlayUI->ToggleRender( false );
    g_pkGameOverlayUI->Clear();
    g_pkGameOverlayUI->SetWndState( KD3DWnd::D3DWS_DISABLED );


    g_pkGameOverlayUI->GetMinigameScoreBoard()->ToggleRender( false );
    SiKGCMinigameManager()->SetExitGame( false );
    g_kGlobalValue.m_kUserInfo = SiKGCMinigameManager()->GetUserInfo();
    SiKGCMinigameManager()->GoPrevState();
	//g_MyD3D->m_TempPlayer.SetMyPlayer();
    // 결과를 서버로 전송
    SiKGCMinigameManager()->SendResult();
}

HRESULT KGCMiniGame::InitAtGame()
{
	SiKGCMinigameManager()->InitLua();
	SiKGCMinigameManager()->SetPlayerPosition();
    InitGameData();

    return S_OK;
}

void KGCMiniGame::InitGameData( void )
{
    bEndOneGame = false;
    m_bShowStartMsg = false;
    g_pkGameOverlayUI->GetMinigameResult()->ToggleRender( false );
    g_pkGameOverlayUI->GetMinigameResult()->SetRankLowDesc( false );
    g_pkGameOverlayUI->GetMinigameScoreBoard()->ToggleRender( true );
    //g_pkGameOverlayUI->GetMinigameScoreBoard()->SetHighScore( SiKGCMinigameManager()->GetMyHighScore( SiKGCMinigameManager()->GetMinigameModeID() - GC_GM_MINIGAME_TREEDROP ) );
    g_pkGameOverlayUI->GetMinigameScoreBoard()->SetCurrentScore( 0 );
}

void KGCMiniGame::SetScoreInfo( void )
{
    // 한판 끝났다... =+=
    KMiniGameRankInfo kTemp;
    kTemp.m_cGameMode = static_cast<char>(SiKGCMinigameManager()->GetMinigameModeID() - GC_GM_MINIGAME_TREEDROP);
    kTemp.m_cCharType = SiKGCMinigameManager()->GetMinigameChar() / NUM_JOB_LEVEL;
    kTemp.m_dwScore = SiKGCMinigameManager()->GetCurrentScore();
    kTemp.m_nClearStage = SiKGCMinigameManager()->GetCurrentLevel();
    KP2P::GetInstance()->Send_EndMiniGameReq( kTemp );

    g_MyD3D->WaitForServerAck( Result_End_MIniGame, INT_MAX, 10000, INT_MAX );

    if ( Result_End_MIniGame == 0 )
    {
        kTemp.m_nRank = Result_MyMinigame_info.m_nRank;
        kTemp.m_dwBestScore = Result_MyMinigame_info.m_dwBestScore;

        // jemitgge 좀;;; 그렇네;; ㅋ 
        g_pkGameOverlayUI->GetMinigameResult()->ToggleRender( true );
        g_pkGameOverlayUI->GetMinigameResult()->SetTotalStar( SetGradeInfo() );

        if ( kTemp.m_nRank == -1 )
        {
            g_pkGameOverlayUI->GetMinigameResult()->SetRankLowDesc( true );
            // 날라온 랭크가 -1이라는 말은 최고기록을 갱신 하지 못했다는 말이다.. 그러므로 랭킹은 기존껄 쓰쟈;; 
            //g_pkGameOverlayUI->GetMinigameResult()->SetMyRanking( 0 );
        }
        else
        {
            g_pkGameOverlayUI->GetMinigameResult()->SetRankLowDesc( false );
            g_pkGameOverlayUI->GetMinigameResult()->SetMyRanking( kTemp.m_nRank );
			SiKGCMinigameManager()->SetMyRanking( kTemp.m_cGameMode, kTemp.m_nRank );
			SiKGCMinigameManager()->SetMyHighScore( kTemp.m_cGameMode, kTemp.m_dwBestScore );
        }
        g_pkGameOverlayUI->GetMinigameResult()->SetHighScore( kTemp.m_dwBestScore );
        g_pkGameOverlayUI->GetMinigameResult()->SetCurrentScore( kTemp.m_dwScore );
        g_pkGameOverlayUI->GetMinigameResult()->SetPlayTime();

        Result_End_MIniGame = INT_MAX;
    }
}

int KGCMiniGame::SetGradeInfo( void )
{
    int iScore = SiKGCMinigameManager()->GetCurrentScore();
    switch ( SiKGCMinigameManager()->GetMinigameModeID() )
    {
    case GC_GM_MINIGAME_TREEDROP:
        {
            if ( iScore >= 201 )
                return 5;
            else if ( iScore > 161 && iScore <= 200 )
                return 4;
            else if ( iScore > 121 && iScore <= 160 )
                return 3;
            else if ( iScore > 81 && iScore <= 120 )
                return 2;
            else
                return 1;
        }
        break;
    case GC_GM_MINIGAME_BALLOON:
        {
            if ( iScore >= 241 )
                return 5;
            else if ( iScore > 201 && iScore <= 240 )
                return 4;
            else if ( iScore > 161 && iScore <= 200 )
                return 3;
            else if ( iScore > 121 && iScore <= 160 )
                return 2;
            else
                return 1;
        }
        break;
    }
 
	return 1;
}

void KGCMiniGame::SetCurrentGP( void )
{
    int iLevel = SiKGCMinigameManager()->GetCurrentLevel();
    int iGP = 0;

    for ( int i = 0; i < iLevel; ++i )
    {
        iGP += 1 + ( ( i + 1 ) * 2 );
    }

    g_pkGameOverlayUI->GetMinigameScoreBoard()->SetGP( iGP );
}

void KGCMiniGame::ShowStartMsg()
{
    if( m_bShowStartMsg == false ) {
        g_pkUIScene->m_pkGameOverlayUI->SetTitleText( g_pkStrLoader->GetString( STR_ID_OLYMPIC_GAME_MAX_POINT_PER_GAME ) );
        m_bShowStartMsg = true;
    }
}