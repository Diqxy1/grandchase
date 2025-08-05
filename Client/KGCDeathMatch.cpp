#include "stdafx.h"
//
#include "KGCDeathMatch.h"
#include "Buff/KGCBuffManager.h"



#include "KMci.h"
#include "Headup Display.h"
#include "Monster.h"
#include "KGCGameBoard.h"


#include "KGCRoomManager.h"
#include "gcui/GCGameOverlayUI.h"
#include "gcui/KGCDeathMatchScoreBoard.h"
#include "KGCEmbarkManager.h"
#include "KGCSubjectManager.h"


KGCDeathMatch::KGCDeathMatch(void)
{
    m_iGoal = 20;
    m_fTime = 60.0f * 5.0f;
}

KGCDeathMatch::~KGCDeathMatch(void)
{
}

HRESULT KGCDeathMatch::InitAtGame()
{
    m_bTimeOverChecked = false;
    m_iLastJoinSendTime = (int)m_fTime;
    g_pkGameOverlayUI->m_pkDeathMatchScoreBoard->UpdateGameData();
    g_pkGameOverlayUI->SetTimer( true, m_fTime );

    UpdateStatBalace();
    CheckStartPlayer();
    
    return S_OK;
}

HRESULT KGCDeathMatch::CalcExpAndGP()
{

    if( SiKGCRoomManager()->GetGameMode() == GC_GM_DEATH_SURVIVAL ||
        SiKGCRoomManager()->GetGameMode() == GC_GM_FATAL_DEATH_SURVIVAL )
    {   
        for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
        {
            g_pGameBoard->m_pStartingMember[i].IsWin = false;                    
            // 게임 중 나갔던, 계속 있던, 처음에 있었다면
            if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) )
            {
                if ( g_pGameBoard->GetRank(i) == 0 )
                {
                    g_pGameBoard->m_pStartingMember[i].IsWin = true;
                    //g_MyD3D->MVP = i;
                }
                else
                {
                    g_pGameBoard->m_pStartingMember[i].IsWin = false;                    
                }                
            }
        }
    }
    else
    {
        int iWinningTeam = 2;        
        bool bTeamLiveChecker[2] = { false, false } ;

        //전원 나간 팀이 있으면 그팀은 진팀!
        for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
        {
            if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) )
            {
                bTeamLiveChecker[g_MyD3D->MyPlayer[i]->m_kUserInfo.iTeam] = true;
            }
        }
        int iSerdinScore = g_pGameBoard->GetTeamScore( ETBlue );
        int iCanabanScore = g_pGameBoard->GetTeamScore( ETRed );

        if ( iSerdinScore >= m_iGoal )
        {
            iWinningTeam = ETBlue;
        }
        else if ( iCanabanScore >= m_iGoal )
        {
            iWinningTeam = ETRed;
        }

        if ( iWinningTeam == 2 )
        {
            for ( int i = 0; i < TEAM_NUM; ++i )
            {
                if ( !bTeamLiveChecker[i] )
                {
                    iWinningTeam = !i;
                }
            }
        }

        if ( iWinningTeam == 2 )
        {
            if ( iSerdinScore > iCanabanScore )
            {
                iWinningTeam = ETBlue;
            }
            else
            {
                iWinningTeam = ETRed;
            }
        }        
        
        //iWinningTeam 이 이겻다     
        for ( int i = 0 ; i < MAX_PLAYER_NUM ; i++ )
        {
            if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.iTeam == iWinningTeam )
            {
                g_pGameBoard->m_pStartingMember[i].IsWin = true;            
            }
            else
            {
                g_pGameBoard->m_pStartingMember[i].IsWin = false;
            }
        }

        SiGCClientErrorManager()->ErrorCollectData( KEventErr::ERR_CUSTOM, KClientErr::CE_TEAM_SCORE, L"", g_pGameBoard->GetTeamScore( ETBlue ), g_pGameBoard->GetTeamScore( ETRed ) );
        return true;
        
    }

    return S_OK;
}


bool KGCDeathMatch::CheckGameEnd()
{
    int iLeadScore = 0;
    if( SiKGCRoomManager()->GetGameMode() == GC_GM_DEATH_SURVIVAL || 
        SiKGCRoomManager()->GetGameMode() == GC_GM_FATAL_DEATH_SURVIVAL )
	{
        int iPlayingPlayerNum = 0;
        for (int i = 0; i < MAX_PLAYER_NUM; i++ )
        {
            if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) || g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) )
            {
                iPlayingPlayerNum++;
                if( g_pGameBoard->GetHowKill(i) >= m_iGoal )
                {
                    return true;
                }

                if ( iLeadScore < g_pGameBoard->GetHowKill(i) )
                {
                    iLeadScore = g_pGameBoard->GetHowKill(i);
                }

                
            }
            
        }
        if ( g_pkGameOverlayUI->GetLimitedTime() <= 0.0f )
        {
            return true;
        }

        if ( iPlayingPlayerNum < 2 )
        {
            return true;
        }
        
	}
    // if( SiKGCRoomManager()->GetGameMode() == GC_GM_DEATH_TEAM || 
    //     SiKGCRoomManager()->GetGameMode() == GC_GM_FATAL_DEATH_TEAM )
	else
	{
        int iSerdinScore = g_pGameBoard->GetTeamScore( ETBlue );
        int iCanabanScore = g_pGameBoard->GetTeamScore( ETRed );

        int iSerdinPlayerNum = 0;
        int iCanabanPlayerNum = 0;
        int iLive = 0;
        bool bPublicEnemy = false;
        for (int i = 0; i < MAX_PLAYER_NUM; i++ )
        {
            if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_READY ) ||
                 g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) )
            {
                if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.iTeam == ETBlue )
                {
                    iSerdinPlayerNum++;                    
                }
                else
                {
                    iCanabanPlayerNum++;                    
                }

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

        if ( iSerdinScore >= m_iGoal || iCanabanScore >= m_iGoal )
        {
            return true;
        }     
        if ( g_pkGameOverlayUI->GetLimitedTime() <= 0.0f )
        {
            if ( iSerdinScore != iCanabanScore )
                return true;
        }
        if ( iSerdinPlayerNum == 0 || iCanabanPlayerNum == 0 )
        {
            return true;            
        }
	}


    if ( g_pkGameOverlayUI->GetLimitedTime() < m_iLastJoinSendTime - 10 ) // 매 10초 마다 보내드림~
    {
        int iWinningTeamScore;

        if( SiKGCRoomManager()->GetGameMode() == GC_GM_DEATH_SURVIVAL ||
            SiKGCRoomManager()->GetGameMode() == GC_GM_FATAL_DEATH_SURVIVAL )
        {
            iWinningTeamScore = iLeadScore;

        }
        else
        {
            iWinningTeamScore = g_pGameBoard->GetTeamScore( ETBlue ) > g_pGameBoard->GetTeamScore( ETRed ) ?
                                  g_pGameBoard->GetTeamScore( ETBlue ) : g_pGameBoard->GetTeamScore( ETRed );

        }

        m_iLastJoinSendTime = (int)g_pkGameOverlayUI->GetLimitedTime();
        KP2P::GetInstance()->Send_UpdateDeathScore( iWinningTeamScore, m_iLastJoinSendTime );
        
    }
    return false;
}

void KGCDeathMatch::GameEnd()
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

HRESULT KGCDeathMatch::CustomFrameMove( float fElapsedTime )
{
    PROFILE_SET("KGCDeathMatch::CustomFrameMove");

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

    if ( g_pkGameOverlayUI->GetLimitedTime() <= 0.0f && !m_bTimeOverChecked)
    {        
        if( !IsDetermineWin )
            g_MyD3D->MyHead->Start_Text("MATCH POINT");

        m_bTimeOverChecked = true;
    }

    // 페이탈 처리
    if ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
    {
        g_MyD3D->GetMyPlayer()->SetHP( 0.0f );
        g_MyD3D->MyHead->Level = 0;
    }

    SiKGCSubjectManager()->FrameMove();

	return S_OK;
}

void KGCDeathMatch::UpdateStatBalace()
{
    if ( !SiKGCRoomManager()->GetRoomInfo().bDeathMatchBalancing )
        return;
    

    KSafeArray<float,MAX_PLAYER_NUM> afOldHPRatio;
    

    for ( int i =0 ; i < MAX_PLAYER_NUM; i++ )
    {
        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) )
        {
            afOldHPRatio[i] = g_MyD3D->MyPlayer[i]->GetHP() / g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_HP];
            g_MyD3D->MyPlayer[i]->Calcualte_Point();
        }
    }

#if !defined(NATION_TAIWAN)
    for (int i = 0 ; i < MAX_PLAYER_NUM ; i++ )
    {
        g_pMagicEffect->SetMagicEffect( i, EGC_EFFECT_STAT_BALANCE ,999999.0f  );
    }
#endif

    if ( SiKGCRoomManager()->GetGameMode() == GC_GM_DEATH_TEAM ||
         SiKGCRoomManager()->GetGameMode() == GC_GM_FATAL_DEATH_TEAM )
    {
        std::vector<int> vecRed;
        std::vector<int> vecBlue;
        std::vector<int>& vecWeak = vecRed; // weak는 강하게 해줄거다 ㅡ
        std::vector<int>& vecStrong = vecBlue; //strong는 약하게 만들어 줄거다ㅡ헷갈리지마센

        int iTeamNumGap = 0;

        for ( int i = 0 ; i < MAX_PLAYER_NUM; i++ )
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[i];
            if ( pPlayer->m_kUserInfo.CheckState( GC_RUS_PLAYING ) )
            {
                if ( pPlayer->m_kUserInfo.IsBlueTeam() )
                {
                    vecBlue.push_back(i);
                }
                else
                {
                    vecRed.push_back(i);
                }
            }            
        }

        if ( vecBlue.size() > vecRed.size() )
        {
            vecWeak     = vecRed;
            vecStrong   = vecBlue;
        }
        else
        {
            vecWeak     = vecBlue;
            vecStrong   = vecRed;
        }
        iTeamNumGap = vecStrong.size() - vecWeak.size();

        float fAtkUpgradeValue = 1.0f;
        float fDefUpgradeValue = 1.0f;

        if ( iTeamNumGap == 1 )
        {
            if ( vecWeak.size() == 1 )
            {
                fAtkUpgradeValue = 1.3f;
                fDefUpgradeValue = 1.3f;
            }
            else if ( vecWeak.size() == 2 )
            {
                //1.2배
                fAtkUpgradeValue = 1.2f;
                fDefUpgradeValue = 1.2f;
            }
        }
        else if ( iTeamNumGap == 2 )
        {
            fAtkUpgradeValue = 1.4f;
            fDefUpgradeValue = 1.5f;
        }
        
        for ( int i = 0 ; i < (int)vecWeak.size() ; i++ )
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[vecWeak[i]];
            pPlayer->m_fAbility[ABILITY_ATK] *= fAtkUpgradeValue;
            pPlayer->m_fAbility[ABILITY_DEF] *= fDefUpgradeValue;            
        }
    }

    for ( int i = 0; i < MAX_PLAYER_NUM; i++ )
    {
        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.CheckState( GC_RUS_PLAYING ) )
        {
            g_MyD3D->MyPlayer[i]->SetHP( g_MyD3D->MyPlayer[i]->Old_HP = g_MyD3D->MyPlayer[i]->m_fAbility[ABILITY_HP] * afOldHPRatio[i] );
        }
    }
}

HRESULT KGCDeathMatch::ON_RENDER_GC_LAYER_TEXT()
{
    KGCGameModeInterface::ON_RENDER_GC_LAYER_TEXT();

    if ( SiKGCRoomManager()->GetGameMode() == GC_GM_DEATH_TEAM ||
         SiKGCRoomManager()->GetGameMode() == GC_GM_FATAL_DEATH_TEAM )
    {

        D3DXVECTOR2 vPosGoalScore( 400.0f* g_pGCDeviceManager2->GetWindowScaleX() * GC_SCREEN_DIV_WIDTH, 160.0f * g_pGCDeviceManager2->GetWindowScaleY() * GC_SCREEN_DIV_HEIGHT);    
        

        D3DXVECTOR2 vPosTeamScore[2] = { 
			D3DXVECTOR2( 340.0f * g_pGCDeviceManager2->GetWindowScaleX() * GC_SCREEN_DIV_WIDTH, 193.0f * g_pGCDeviceManager2->GetWindowScaleY()), 
			D3DXVECTOR2( 435.0f * g_pGCDeviceManager2->GetWindowScaleX() * GC_SCREEN_DIV_WIDTH, 193.0f * g_pGCDeviceManager2->GetWindowScaleY()) };
		D3DXVECTOR2 vPosTeamBar[2] = {  
			D3DXVECTOR2( 340.0f * g_pGCDeviceManager2->GetWindowScaleX() * GC_SCREEN_DIV_WIDTH, 210.0f * g_pGCDeviceManager2->GetWindowScaleY()),
			D3DXVECTOR2( 435.0f * g_pGCDeviceManager2->GetWindowScaleX() * GC_SCREEN_DIV_WIDTH, 210.0f * g_pGCDeviceManager2->GetWindowScaleY()) };                
        D3DXVECTOR2 vPosBackPan( 340.5f * g_pGCDeviceManager2->GetWindowScaleX() * GC_SCREEN_DIV_WIDTH, 155.0f * g_pGCDeviceManager2->GetWindowScaleY());    

        g_pGCDeviceManager2->PushState();
        {
            g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
            g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

            g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
            g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

            g_pkUIMgr->RenderNumber( "death_match_team_back", vPosBackPan, 1, D3DXCOLOR(0xffffffff), true, false, -1.0f );
            g_pkUIMgr->RenderNumber( "death_match_goal_num", vPosGoalScore, m_iGoal, D3DXCOLOR(0xffffffff), false, false, -1.0f );   

            for ( int i = 0; i < 2 ; i++ )
            {
                if ( g_pGameBoard->GetTeamScore(i) >= 10 )
                {
                    vPosTeamScore[i].x -= 15.0f * g_pGCDeviceManager2->GetWindowScaleX() * GC_SCREEN_DIV_WIDTH;
                }                
            }

            g_pkUIMgr->RenderNumber( "serdin_score_num", vPosTeamScore[0], g_pGameBoard->GetTeamScore(0) , D3DXCOLOR(0xffffffff), true, true, -1.0f );
            g_pkUIMgr->RenderNumber( "canaban_score_num", vPosTeamScore[1], g_pGameBoard->GetTeamScore(1) , D3DXCOLOR(0xffffffff), true, true, -1.0f );

            g_pkUIMgr->RenderNumber( "death_match_score_underbar", vPosTeamBar[g_kGlobalValue.m_kUserInfo.iTeam], !g_kGlobalValue.m_kUserInfo.iTeam, D3DXCOLOR(0xffffffff), true, false, -1.0f );
            

        }
        g_pGCDeviceManager2->PopState();
    }
    else
    {
        D3DXVECTOR2 vPosGoalScore( 400.0f* g_pGCDeviceManager2->GetWindowScaleX() * GC_SCREEN_DIV_WIDTH, 130.0f * g_pGCDeviceManager2->GetWindowScaleY() * GC_SCREEN_DIV_HEIGHT);    
        D3DXVECTOR2 vPosBackPan( 370.5f * g_pGCDeviceManager2->GetWindowScaleX() * GC_SCREEN_DIV_WIDTH, 130.0f * g_pGCDeviceManager2->GetWindowScaleY() * GC_SCREEN_DIV_HEIGHT);    
        D3DXVECTOR2 vPosMyScore( 387.5f * g_pGCDeviceManager2->GetWindowScaleX() * GC_SCREEN_DIV_WIDTH, 171.0f * g_pGCDeviceManager2->GetWindowScaleY() * GC_SCREEN_DIV_HEIGHT);

        if ( g_pGameBoard->GetHowKill( g_MyD3D->Get_MyPlayer() ) >= 10 )
        {
            vPosMyScore.x -= 15.0f * g_pGCDeviceManager2->GetWindowScaleX() * GC_SCREEN_DIV_WIDTH;
        }     

        g_pGCDeviceManager2->PushState();
        {
            g_pGCDeviceManager2->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
            g_pGCDeviceManager2->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

            g_pGCDeviceManager2->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
            g_pGCDeviceManager2->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
            g_pkUIMgr->RenderNumber( "death_match_solo_back", vPosBackPan, 1, D3DXCOLOR(0xffffffff), true, false, -1.0f );
            g_pkUIMgr->RenderNumber( "death_match_goal_num", vPosGoalScore, m_iGoal, D3DXCOLOR(0xffffffff), false, false, -1.0f );   

            g_pkUIMgr->RenderNumber( "serdin_score_num", vPosMyScore, g_pGameBoard->GetHowKill( g_MyD3D->Get_MyPlayer() ), D3DXCOLOR(0xffffffff), true, true, -1.0f );
        }
        g_pGCDeviceManager2->PopState();


    }
    return S_OK;
}
