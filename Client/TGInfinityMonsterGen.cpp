#include "StdAfx.h"
#include "TGInfinityMonsterGen.h"
#include "Monster.h"
#include "KGCInfinityDungeonManager.h"

CTGInfinityMonsterGen::CTGInfinityMonsterGen()
{
    enType          = TG_INFINITY_MON_GEN;

    m_iStartDelay = -1;             

    m_iMonsterRegenTime = -1;
    m_iMonsterTimer = -1;

    m_iBossRegenRank = -1;
    m_iRankBossRegenWaitTime = -1;
    m_iRankBossTimer = -1;

    m_iMaxBossRegenTime = -1;
    m_iMaxBossTimer = -1;

    m_iBossRegenCoolTime = -1;
    m_iBossCoolTimer = -1;

    m_iAllRegenCoolTime = -1;
    m_iAllCoolTimer = -1;

    m_bEnableRengenMonster = false;
    m_bRegenBoss = false;
    m_bBeforeBossRegenState = false;

    m_bMonsterTimerRun = false;
    m_bRankBossWaitTimerRun = false;
    m_bMaxBossTimerRun = false;
    m_bBossCoolTimerRun = false;
    m_bAllCoolTimerRun = false;
}

CTGInfinityMonsterGen::~CTGInfinityMonsterGen()
{

}

void CTGInfinityMonsterGen::FrameMove( void )
{
    if( IsComplete() == true ) return;

    if( g_kGlobalValue.m_kUserInfo.bHost ) { 
        
        // 시작 몬스터 소환 처리 
        StartMonsterRegenProcess();

        // 몬스터 반복 소환 처리 
        if( m_bEnableRengenMonster ) { 
            BossMonsterRegenProcess();
            NormalMonsterRegenProcess();
        }
        
        // 소환 쿨 처리 
        MonsterRegenCoolProcess();

        // 소환 가능 여부 체크 
        CheckMonsterRegenState();

#if !defined( __PATH__ ) 
        SetTimerText(); // 타이머 상태 테스트용 
#endif

        if( !m_bSendPacket ) { 
            g_kTGPacket->SetValue(m_iTG_ID, g_sTG_MaxQuestID, static_cast<int>(enType) );
            g_kTGPacketPacker.Pack( g_kTGPacket, sizeof(KGC_PID_TG_DEFAULT) );
            m_bSendPacket = true;
        }
    }

    if( !m_bEnableRengenMonster && m_iStartDelay == 0 ) { 
        SetComplete( true );
    }
}

HRESULT CTGInfinityMonsterGen::LoadItemFromLua( KLuaManager &luaMgr )
{
    InitMonsterGenState();
    SiKGCInfinityDungeonManager()->InitBeforeStartGame();
    return S_OK;
}

void CTGInfinityMonsterGen::InitMonsterGenState( void )
{
    m_iStartDelay = 550;    // 55*10 = 10초
    m_iMonsterRegenTime = SiKGCInfinityDungeonManager()->GetRegenMonsterTime( SiKGCRoomManager()->GetGameMode() ) * 55;
    m_iRankBossRegenWaitTime = SiKGCInfinityDungeonManager()->GetRegenBossWaitTime( SiKGCRoomManager()->GetGameMode() ) * 55;
    m_iMaxBossRegenTime = SiKGCInfinityDungeonManager()->GetMaxBossRegenTime( SiKGCRoomManager()->GetGameMode() ) * 55;
    m_iBossRegenCoolTime = SiKGCInfinityDungeonManager()->GetMinBossRegenTime( SiKGCRoomManager()->GetGameMode() ) * 55;
    m_iAllRegenCoolTime = 1650; // 55*30 = 30초 
    m_iBossRegenRank = SiKGCInfinityDungeonManager()->GetRegenBossRank( SiKGCRoomManager()->GetGameMode() );

    m_bEnableRengenMonster = false;
    m_bSummonBossReq = false;
    m_bRegenBoss = false;
    m_bBeforeBossRegenState = false;

    ResetMonsterRegenTimer();
    ResetRankBossWaitTimer();
    ResetMaxBossRegenTimer();
    ResetBossRegenCoolTimer();
    ResetAllRegenCoolTimer();
}

void CTGInfinityMonsterGen::ResetMonsterRegenTimer( void )
{
    m_bMonsterTimerRun = false;
    m_iMonsterTimer = 0;
}

void CTGInfinityMonsterGen::RunMonsterRegenTimer( void )
{
    if( m_bMonsterTimerRun ) { 
        ++m_iMonsterTimer;
    }
}

bool CTGInfinityMonsterGen::CheckMonsterRegenTimerRun( void )
{
    return m_bMonsterTimerRun;
}

bool CTGInfinityMonsterGen::CheckEnableRegenMonster( void )
{
    if( m_iMonsterTimer == m_iMonsterRegenTime ) { 
        ResetMonsterRegenTimer();
        return true;
    }

    return false;
}

void CTGInfinityMonsterGen::ResetRankBossWaitTimer( void )
{
    m_bRankBossWaitTimerRun = false;
    m_iRankBossTimer = 0;
}

void CTGInfinityMonsterGen::RunRankBossWaitTimer( void )
{
    if( m_bRankBossWaitTimerRun ) { 
        ++m_iRankBossTimer;
    }
}

bool CTGInfinityMonsterGen::CheckRankBossWaitTimerRun( void )
{
    return m_bRankBossWaitTimerRun;
}

bool CTGInfinityMonsterGen::CheckEnableRegenRankBoss( void )
{
    if( m_iRankBossTimer == m_iRankBossRegenWaitTime ) { 
        ResetRankBossWaitTimer();
        ResetMaxBossRegenTimer();
        ResetMonsterRegenTimer();
        return true;
    }

    return false;
}

void CTGInfinityMonsterGen::ResetMaxBossRegenTimer( void )
{
    m_bMaxBossTimerRun = false;
    m_iMaxBossTimer = 0;
}

void CTGInfinityMonsterGen::RunMaxBossRegenTimer( void )
{
    if( m_bMaxBossTimerRun ) { 
        ++m_iMaxBossTimer;
    }
}

bool CTGInfinityMonsterGen::CheckMaxBossRegenTimerRun( void )
{
    return m_bMaxBossTimerRun;
}

bool CTGInfinityMonsterGen::CheckEnableRegenMaxBoss( void )
{
    if( m_iMaxBossTimer == m_iMaxBossRegenTime ) { 
        ResetRankBossWaitTimer();
        ResetMaxBossRegenTimer();
        ResetMonsterRegenTimer();
        return true;
    }

    return false;
}

void CTGInfinityMonsterGen::ResetBossRegenCoolTimer( void )
{
    m_bBossCoolTimerRun = false;
    m_bMaxBossTimerRun = true;
    m_iBossCoolTimer = 0;
}

void CTGInfinityMonsterGen::RunBossRegenCoolTimer( void )
{
    if( m_bBossCoolTimerRun ) { 
        ++m_iBossCoolTimer;
        if( m_iBossCoolTimer == m_iBossRegenCoolTime ) { 
            ResetBossRegenCoolTimer();
        }
    }
}

bool CTGInfinityMonsterGen::CheckBossRegenCoolTimerRun( void )
{
    return m_bBossCoolTimerRun;
}

void CTGInfinityMonsterGen::ResetAllRegenCoolTimer( void )
{
    m_bAllCoolTimerRun = false;
    m_bMonsterTimerRun = true;
    m_iAllCoolTimer = 0;

    if( m_bEnableRengenMonster ) { 
        NormalMonsterRegenDirect();
    }
}

void CTGInfinityMonsterGen::RunAllRegenCoolTimer( void )
{
    if( m_bAllCoolTimerRun ) { 
        ++m_iAllCoolTimer;
        if( m_iAllCoolTimer == m_iAllRegenCoolTime ) { 
            ResetAllRegenCoolTimer();
        }
    }
}

bool CTGInfinityMonsterGen::CheckAllRegenCoolTimerRun( void )
{
    return m_bAllCoolTimerRun;
}

void CTGInfinityMonsterGen::SetBossState( void )
{
    m_bBeforeBossRegenState = m_bRegenBoss;
    if( g_kMonsterManager.GetLiveInfinityMonsterBossNum() > 0 ) { 
        m_bRegenBoss = true;
        m_bSummonBossReq = false;
    }
    else { 
        m_bRegenBoss = false;
    }
}

bool CTGInfinityMonsterGen::CheckBossSummonReqState( void )
{
    return m_bSummonBossReq;
}

bool CTGInfinityMonsterGen::CheckBossState( void )
{
    return m_bRegenBoss;
}

bool CTGInfinityMonsterGen::CheckBossStateInBeforeFrame( void )
{
    return m_bBeforeBossRegenState;
}

void CTGInfinityMonsterGen::StartMonsterRegenProcess( void )
{
    if( m_iStartDelay > 0 ) {   
        -- m_iStartDelay;
        if( m_iStartDelay == 0 ) { 
            SiKGCInfinityDungeonManager()->SummonStartMonsterReq( SiKGCRoomManager()->GetGameMode(), g_MyD3D->GetLivePlayerNum(), g_pkQuestManager->GetPartyAvgRank() );
            m_bMonsterTimerRun = true;
            m_bMaxBossTimerRun = true;
            m_bEnableRengenMonster = true;

            // 포탈 몬스터 깰수 있는 상태로 
            MONSTER* pMonster = g_kMonsterManager.GetMonsterType( MON_INFINITY_DUNGEON_GATE );
            if( pMonster ) { 
                pMonster->SetState( "START_DAMAGE" );
            }
        }
    }
}

void CTGInfinityMonsterGen::BossMonsterRegenProcess( void )
{   
    if( !CheckBossState() && !CheckBossSummonReqState() && !CheckAllRegenCoolTimerRun() && !CheckBossRegenCoolTimerRun() ) { 
        // 랭크 보스 
        if( g_pkQuestManager->GetPartyAvgRank() >= m_iBossRegenRank ) { 
            m_bRankBossWaitTimerRun = true;
        }
        else { 
            ResetRankBossWaitTimer();
        }

        RunRankBossWaitTimer();

        if( CheckEnableRegenRankBoss() ) { 
            m_bSummonBossReq =  SiKGCInfinityDungeonManager()->SummonMonsterReq( SiKGCRoomManager()->GetGameMode(), g_MyD3D->GetLivePlayerNum(), g_pkQuestManager->GetPartyAvgRank(), true, true );
        }

        // 일반 시간 보스 
        if( !CheckBossState() && !CheckBossSummonReqState() ) { 
            RunMaxBossRegenTimer();
        }

        if( CheckEnableRegenMaxBoss() ) { 
            m_bSummonBossReq = SiKGCInfinityDungeonManager()->SummonMonsterReq( SiKGCRoomManager()->GetGameMode(), g_MyD3D->GetLivePlayerNum(), g_pkQuestManager->GetPartyAvgRank(), true, false );
        }

        // 보스 등장 알림 
        if( m_bSummonBossReq ) { 
            MONSTER* pMonster = g_kMonsterManager.GetMonsterType( MON_INFINITY_DUNGEON_GATE );
            if( pMonster ) { 
                pMonster->SetState( "BOSS_ALRAM" );
            }
        }
    }
}

void CTGInfinityMonsterGen::NormalMonsterRegenProcess( void )
{
    if( !CheckBossState() && !CheckBossSummonReqState() && !CheckAllRegenCoolTimerRun() ) { 
        RunMonsterRegenTimer();

        if( CheckEnableRegenMonster() ) { 
            SiKGCInfinityDungeonManager()->SummonMonsterReq( SiKGCRoomManager()->GetGameMode(), g_MyD3D->GetLivePlayerNum(), g_pkQuestManager->GetPartyAvgRank(), false, false );
                m_bMonsterTimerRun = true;
        }
    }
}

void CTGInfinityMonsterGen::NormalMonsterRegenDirect( void )
{
    SiKGCInfinityDungeonManager()->SummonMonsterReq( SiKGCRoomManager()->GetGameMode(), g_MyD3D->GetLivePlayerNum(), g_pkQuestManager->GetPartyAvgRank(), false, false );
}

void CTGInfinityMonsterGen::MonsterRegenCoolProcess( void )
{
    // 보스 상태에 따라 쿨타임 처리 
    SetBossState();
    if( !CheckBossState() && CheckBossStateInBeforeFrame() && g_kMonsterManager.GetLiveInfinityMonsterBossNum() == 0 ) {  // 보스 사망 타이밍 - 쿨타임 돌리자 
        ResetMonsterRegenTimer();
        ResetRankBossWaitTimer();
        ResetMaxBossRegenTimer();
        m_bBossCoolTimerRun = true;
        m_bAllCoolTimerRun = true;

        g_kMonsterManager.AllKillInfinityNormalMonster();
        SetMonsterRegenCoolTimeMessage();
    }
    RunBossRegenCoolTimer();
    RunAllRegenCoolTimer();
}

void CTGInfinityMonsterGen::CheckMonsterRegenState( void )
{
    // 특정 몬스터가 죽으면 소환을 멈춘다. 
    // 일단 코드에 넣어두겠음 
    if( !g_kMonsterManager.IsLiveMonsterToType( MON_INFINITY_DUNGEON_GATE ) ) { 
        m_bEnableRengenMonster = false;
    }
}

void CTGInfinityMonsterGen::SetMonsterRegenCoolTimeMessage( void )
{
    // 게이트 몬스터가 처리해주자 
    MONSTER* pMonster = g_kMonsterManager.GetMonsterType( MON_INFINITY_DUNGEON_GATE );
    if( pMonster ) { 
        pMonster->SetState( "COOL" );
    }
}

void CTGInfinityMonsterGen::SetTimerText( void )
{
#if !defined( __PATH__ )
    if( g_pkGameOverlayUI->IsRenderOnStaticInternalTest() ) { 
        std::wostringstream stm;
        stm << L"몬스터 소환 할수 있음  : " << ( m_bEnableRengenMonster ? L"On" : L"Off" ) << std::endl;
        stm << L"보스 소환 되어 있음  : " << ( m_bRegenBoss ? L"On" : L"Off" ) << std::endl;

        stm << L"일반 몬스터 소환 타이머 상태 : " << ( m_bMonsterTimerRun ? L"On" : L"Off" ) << std::endl;
        stm << L"일반 몬스터 소환 타이머 : " << static_cast<float>( m_iMonsterTimer ) / 55.0f << std::endl;
        stm << std::endl;

        stm << L"일반 보스 소환 타이머 상태 : " << ( m_bMaxBossTimerRun ? L"On" : L"Off" ) << std::endl;
        stm << L"일반 보스 소환 타이머 : " << static_cast<float>( m_iMaxBossTimer ) / 55.0f << std::endl;
        stm << std::endl; 

        stm << L"랭킹 보스 소환 대기 타이머 상태 : " << ( m_bRankBossWaitTimerRun ? L"On" : L"Off" ) << std::endl;
        stm << L"랭킹 보스 소환 대기 타이머 : " << static_cast<float>( m_iRankBossTimer ) / 55.0f << std::endl;
        stm << std::endl; 

        stm << L"보스 소환 쿨타임 타이머 상태 : " << ( m_bBossCoolTimerRun ? L"On" : L"Off" ) << std::endl;
        stm << L"보스 소환 쿨타임 타이머 : " << static_cast<float>( m_iBossCoolTimer ) / 55.0f << std::endl;
        stm << std::endl; 

        stm << L"모든 몬스터 소환 쿨타임 타이머 상태 : " << ( m_bAllCoolTimerRun ? L"On" : L"Off" ) << std::endl;
        stm << L"모든 몬스터 쿨타임 타이머 : " << static_cast<float>( m_iAllCoolTimer ) / 55.0f << std::endl;
        stm << std::endl; 

        g_pkGameOverlayUI->SetStaticInternalTest( stm.str() );
    }
#endif
}



