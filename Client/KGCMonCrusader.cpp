#include "stdafx.h"
#include "KGCMonCrusader.h"

#include "KMci.h"
#include "Headup Display.h"
#include "Monster.h"
#include "KStats.h"


#include "KGCGameBoard.h"

#include "KGCRoomManager.h"
#include "KGCElvis.h"
//
#include "../gcui/GCGameOverlayUI.h"
#include "KGCEmbarkManager.h"
#include "KGCSubjectManager.h"


#define GAME_END_TIME ( 440 )

KGCMonCrusader::KGCMonCrusader(void)
{
    m_fAccumulateTime = 0.0f;
    m_fEventTime = (float)INT_MAX;

    m_iRemainMonster = 0;
    m_iGameEndCheck = 0;

    InitRebirthTimer();
}

KGCMonCrusader::~KGCMonCrusader(void)
{
}

HRESULT KGCMonCrusader::InitAtGame()
{
#if defined(NATION_KOREA)
    m_fAccumulateTime = 0.0f;
    m_fEventTime = 60.0f;
#ifndef __PATH__
    g_kStatsManager.InsertPage( L"GAME TIME" );
    g_kStatsManager.InsertValue( L"AccumulateTime", &m_fAccumulateTime );
#endif
#endif

    m_MonsterNumToStart     = -1;
    m_NextMonsterStartTime  = 0;
    //m_iRebirthCount = 0;
    m_dwLastRebirthTime = 0;

    m_bRequestRebirth = false;
    SiKGCElvis()->SetRevivalCount(0);

    m_iRemainMonster = SiKGCRoomManager()->GetRoomInfo().iMonsterCount - 1;

    m_iGameEndCheck = 0;

    InitRebirthTimer();	
	
	g_pkGameOverlayUI->UpdateBossList();
    g_pkQuestManager->m_bClearStage = false;

    return S_OK;
}

HRESULT KGCMonCrusader::CalcExpAndGP()
{
    g_MyD3D->MVP = MAX_PLAYER_NUM + 1;
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // Description by parkch
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // 가도센 퇴치 작전/몬스터 원정대의 승패 결정 방식
    // 승자 - 몬스터를 모두 물리친 경우 게임 도중에 나가지 않은 사람
    // 패자 - 몬스터를 물리치지 못한 경우의 모든 사람
    // 탈락자 - 게임 도중에 나간 사람
    //
    // 가도센 퇴치 작전/몬스터 원정대의 점수 배분 방식
    // 1. 물리친 몬스터가 몇 마리인지 확인하여 가산치를 계산한다.
    // 2. 승자는 각 레벨별로 받을 수 있는 Exp/GP에 가산치를 부가해서 받는다.
    // 3. 탈락자를 제외하고 승자/패자 모두 Exp/GP를 최소 1씩 받는다.
    // 4. MVP는 선정하지 않는다.
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    bool bWin       = true;
    float fRatio    = 0.0f;

    if( m_iRemainMonster )
    {
        bWin = false;
    }
    else
    {
		for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
			if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
				continue;
			MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

			if(pMonster == NULL) continue;

			if( pMonster->m_bLive )
			{
				bWin = false;
				break;
			}
		}
    }


    for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        if ( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive == false )
            continue;

        if( g_pGameBoard->m_pStartingMember[i].m_iChar != GC_CHAR_INVALID )
            SetPlayerGameResult( i, g_pGameBoard->m_pStartingMember[i].m_iChar, 1.0f, 1.0f, bWin );

		g_pGameBoard->m_pStartingMember[i].IsWin = bWin;
    }

    return S_OK;
}

bool KGCMonCrusader::CheckGameEnd()
{
    // 몬스터원정대에서는 몬스터가 다 죽었거나 플레이어가 다 죽었으면 게임이 끝난다.

    // 다 죽이고 나서 GAME_END_TIME 만큼의 프레임이 흐르면 게임을 끝내 준다..
    // 끝나고 딜레이 쪼금 주려고..
    if( m_iGameEndCheck > 0 )
    {
        ++m_iGameEndCheck;
        if( m_iGameEndCheck >= GAME_END_TIME )
            return true;
    }

    // 몬스터 체크
    // m_iRemainMonster은 리젠 시키고 1씩 줄여준다
    if( m_iRemainMonster <= 0 )
    {
        if( g_MyD3D->Story > 55 * 20 ) // 20초?
        {
            bool bLiveMonster = false;
			
			for ( MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
				if ( g_kMonsterManager.IsSummonMonsterByPlayer( mit->first ) )
					continue;
				MONSTER* pMonster = g_kMonsterManager.GetMonster(mit->first);

				if(pMonster == NULL) continue;

				if( pMonster->m_bLive )
				{
					bLiveMonster = true;
					break;
				}
			}

             //몬스터가 다 죽었다면 게임 끝
            if( !bLiveMonster )
			{				
                ++m_iGameEndCheck;				
			}
        }
    }

    // 플레이어 체크
    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && g_MyD3D->MyPlayer[i]->m_cLife > 0 )
            return false; // 사람이 하나라도 살아있다면 게임 끝나지 않음
    }

    if( m_nRebirthTimer > 0 )
    {
        return false;
    }

    return true;
}

void KGCMonCrusader::GameEnd()
{
    // 옵져버일때
    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_BROADCASTING ) && g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN )
    {
        g_MyD3D->MyHead->Start_Text("GAME OVER");
        return;
    }

    if( g_pGameBoard->m_pStartingMember[g_MyD3D->Get_MyPlayer()].IsWin )
    {
        g_KMci.Play( GC_BGM_GAME_WIN, false );
        g_MyD3D->MyHead->Start_Text("COMPLETE");
    }
    else
    {
        g_KMci.Play( GC_BGM_GAME_LOSE, false );
        g_MyD3D->MyHead->Start_Text("FAILED");
    }

    g_pGameBoard->ClearMonKillNum();
}

HRESULT KGCMonCrusader::CustomFrameMove( float fElapsedTime )
{
    PROFILE_SET("KGCMonCrusader::CustomFrameMove");

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


    // 살아있는 사람이 없으면 게임 종료
    int iTotalLife = 0;
    for ( int i = 0; i < MAX_PLAYER_NUM; ++i )
    {
        if( g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive )
            iTotalLife += g_MyD3D->MyPlayer[i]->m_cLife;
    }

    // [12/18/2007] breadceo. 다 죽었으면 RebirthProcess
    if( !iTotalLife )
    {
        ProcessLastRebirthChance();
    }
    else
    {
        InitRebirthTimer();
    }
    CheckRebirth();

    SiKGCSubjectManager()->FrameMove();

    return S_OK;
}

void KGCMonCrusader::ProcessLastRebirthChance()
{
    if( m_bRebirthProcessing == false )
    {
        m_bRebirthProcessing = true;
        m_nRebirthTimer = LAST_REBIRTH_CHANCE_TERM;
        return;
    }

    m_nRebirthTimer--;
}

void KGCMonCrusader::LoadScript()
{
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr , "Enum.stg" ) == false )
    {
        MessageBoxA( NULL, "Enum.stg Load Failed!", "", MB_OK );
        return;
    }

    if( GCFUNC::LoadLuaScript( luaMgr, "MonsterCrusader.stg" ) == false )
    {
        MessageBoxA( NULL, "MonsterCrusader.stg Load Failed!", "", MB_OK );
        return;
    }

    g_kGlobalValue.m_mapMonsterHuntInfo.clear();

    luaMgr.BeginTable( "MonsterCrusader" );
    {
        int iMonsterCrusaderIndex = 1;
        while( true )
        {
            if( FAILED( luaMgr.BeginTable( iMonsterCrusaderIndex ) ) ) break;
            {
                SMonsterCrusaderInfo kMonsterCrusaderInfo;

                luaMgr.GetValue( "Map",             kMonsterCrusaderInfo.strMap );
                luaMgr.GetValue( "MonsterID",       kMonsterCrusaderInfo.iMonsterID );
                luaMgr.GetValue( "TextureIndex",    kMonsterCrusaderInfo.iTextureIndex );
                luaMgr.GetValue( "ExpRatio",        kMonsterCrusaderInfo.fExpRatio );
                luaMgr.GetValue( "GPRatio",         kMonsterCrusaderInfo.fGPRatio );

                g_kGlobalValue.m_mapMonsterCrusaderInfo.push_back( kMonsterCrusaderInfo );
            }
            luaMgr.EndTable();

            ++iMonsterCrusaderIndex;
        }
    }
    luaMgr.EndTable();
}
