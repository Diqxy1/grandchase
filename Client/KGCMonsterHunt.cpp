#include "stdafx.h"
#include "KGCMonsterHunt.h"

#include "KMci.h"
#include "Headup Display.h"
#include "Monster.h"

#include "KGCGameBoard.h"
#include "KGCEmbarkManager.h"
#include "KGCSubjectManager.h"


KGCMonsterHunt::KGCMonsterHunt(void)
{
    m_fAccumulateTime = 0.0f;
}

KGCMonsterHunt::~KGCMonsterHunt(void)
{
}

HRESULT KGCMonsterHunt::InitAtGame()
{
    m_dwLastRebirthTime = 0;

    return S_OK;
}

HRESULT KGCMonsterHunt::CalcExpAndGP()
{
    // MVP는 없다!
    g_MyD3D->MVP = MAX_PLAYER_NUM + 1;

    return S_OK;
}

bool KGCMonsterHunt::CheckGameEnd()
{
    return false;

    return true;
}

void KGCMonsterHunt::GameEnd()
{
    // 옵져버일때
    if( g_kGlobalValue.ServerInfo.CheckServerType( ST_BROADCASTING ) &&
        g_kGlobalValue.m_kUserInfo.eGCUserLevel == USERLEVEL_ADMIN )
    {
        g_MyD3D->MyHead->Start_Text("GAME OVER");
        return;
    }

    if( g_pGameBoard->m_pStartingMember[g_MyD3D->Get_MyPlayer()].IsWin )
    {
        g_KMci.Play( GC_BGM_GAME_WIN, false );
        g_MyD3D->MyHead->Start_Text( "COMPLETE" );
    }
    else
    {
        g_KMci.Play( GC_BGM_GAME_LOSE, false );
        g_MyD3D->MyHead->Start_Text( "FAILED" );
    }

    g_pGameBoard->ClearMonKillNum();
}

HRESULT KGCMonsterHunt::CustomFrameMove( float fElapsedTime )
{
    PROFILE_SET("KGCMonsterHunt::CustomFrameMove");

	SendP2PPacket( GC_PID_RELAY_PING );
    ++g_MyD3D->Story;

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
    for( int i = 0; i < MAX_PLAYER_NUM; ++i )
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


    //RisenMonster();
    CheckRebirth();

    SiKGCSubjectManager()->FrameMove();

    return S_OK;
}

void KGCMonsterHunt::LoadScript()
{
    KLuaManager luaMgr;

    if( GCFUNC::LoadLuaScript( luaMgr , "Enum.stg" ) == false )
    {
        MessageBoxA( NULL, "Enum.stg Load Failed!", "", MB_OK );
        return;
    }

    if( GCFUNC::LoadLuaScript( luaMgr, "MonColosseum.stg" ) == false )
    {
        MessageBoxA( NULL, "MonColosseum.stg Load Failed!", "", MB_OK );
        return;
    }

    g_kGlobalValue.m_mapMonsterHuntInfo.clear();

    luaMgr.BeginTable( "MonsterHunt" );
    {
        int iMonsterHuntInfoIndex = 1;
        while( true )
        {
            if( FAILED( luaMgr.BeginTable( iMonsterHuntInfoIndex ) ) ) break;
            {
                SMonsterHuntInfo kMonsterHuntInfo;

                luaMgr.GetValue( "Level",      kMonsterHuntInfo.iLevel );
                luaMgr.GetValue( "Map",        kMonsterHuntInfo.strMap );
                luaMgr.GetValue( "TotalTime",  kMonsterHuntInfo.iTotalTime );
                luaMgr.GetValue( "ClearPoint", kMonsterHuntInfo.iClearPoint );
                luaMgr.GetValue( "ArenaPoint", kMonsterHuntInfo.iArenaPoint );

                // 몬스터의 메쉬, 텍스쳐, 카툰텍스쳐 정보를 읽어온다
                if( SUCCEEDED(luaMgr.BeginTable( "Monster" )) )
                {
                    int iMonsterIndex = 1;

                    while( true )
                    {
                        if( FAILED( luaMgr.BeginTable( iMonsterIndex ) ) ) break;
                        {
                            SMonsterHuntMonsterInfo kMonsterHuntMonsterInfo;

                            luaMgr.GetValue( 1, kMonsterHuntMonsterInfo.iMonsterType );
                            luaMgr.GetValue( 2, kMonsterHuntMonsterInfo.iPoint );
                            luaMgr.GetValue( 3, kMonsterHuntMonsterInfo.iRegenTime );

                            kMonsterHuntInfo.vecMonsterInfo.push_back( kMonsterHuntMonsterInfo );

                            ++iMonsterIndex;
                        }
                        luaMgr.EndTable();
                    }
                    luaMgr.EndTable(); // "Monster"
                }

                g_kGlobalValue.m_mapMonsterHuntInfo.push_back( kMonsterHuntInfo );
            }
            luaMgr.EndTable();

            ++iMonsterHuntInfoIndex;
        }
    }
    luaMgr.EndTable();
}
