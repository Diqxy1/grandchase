#pragma once
#include "TriggerCondition.h"

class CTGInfinityMonsterGen : public CTriggerCondition
{
public:
    CTGInfinityMonsterGen();
    ~CTGInfinityMonsterGen();

    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);

private:
    int m_iStartDelay;              // 첫 소한 딜레이 시간   

    int m_iMonsterRegenTime;        // 일반 몬스터 소환 주가 + 타이머 
    int m_iMonsterTimer;

    int m_iBossRegenRank;           // 랭킹 보스 소환 위한 필요 랭킹 
    int m_iRankBossRegenWaitTime;   // 랭크 등장 보스 소한 대기시간 + 타이머 
    int m_iRankBossTimer;

    int m_iMaxBossRegenTime;        // 일정 시간 간격 보스 소환 주기 + 타이머 
    int m_iMaxBossTimer;

    int m_iBossRegenCoolTime;       // 보스 몬스터 잡은 후 다음 보스 등장까지 최소 쿨타임 + 타이머 
    int m_iBossCoolTimer;

    int m_iAllRegenCoolTime;        // 보스 잡은 후 모든 몬스터 소환 되지 않는 쿨타임 + 타이머 
    int m_iAllCoolTimer;

    bool m_bEnableRengenMonster;    // 몬스터 소환 가능한가
    bool m_bSummonBossReq;          // 보스 소환 요청 보낸 상태인가 
    bool m_bRegenBoss;              // 보스 소환 되어있나 
    bool m_bBeforeBossRegenState;   // 이전 프레임에 보스 소환되어있었나 

    bool m_bMonsterTimerRun;        // 일반 몬스터 타이머 돌수 있나 
    bool m_bRankBossWaitTimerRun;   // 랭킹 보스 대기시간 타이머 돌수 있나 
    bool m_bMaxBossTimerRun;        // 시간 보스 타이머 돌수 있나 
    bool m_bBossCoolTimerRun;       // 보스 소환 쿨타임 타이머 돌수 있나 
    bool m_bAllCoolTimerRun;        // 모든 몬스터 소환 대기 쿨타임 타이머 돌수 있나   

    void InitMonsterGenState( void );

    void ResetMonsterRegenTimer( void );
    void RunMonsterRegenTimer( void );
    bool CheckMonsterRegenTimerRun( void );
    bool CheckEnableRegenMonster( void );

    void ResetRankBossWaitTimer( void );
    void RunRankBossWaitTimer( void );
    bool CheckRankBossWaitTimerRun( void );
    bool CheckEnableRegenRankBoss( void );

    void ResetMaxBossRegenTimer( void );
    void RunMaxBossRegenTimer( void );
    bool CheckMaxBossRegenTimerRun( void );
    bool CheckEnableRegenMaxBoss( void );

    void ResetBossRegenCoolTimer( void );
    void RunBossRegenCoolTimer( void );
    bool CheckBossRegenCoolTimerRun( void );

    void ResetAllRegenCoolTimer( void );
    void RunAllRegenCoolTimer( void );
    bool CheckAllRegenCoolTimerRun( void );

    void SetBossState( void );
    bool CheckBossSummonReqState( void );
    bool CheckBossState( void );
    bool CheckBossStateInBeforeFrame( void );

    void StartMonsterRegenProcess( void );
    void BossMonsterRegenProcess( void );
    void NormalMonsterRegenProcess( void );
    void NormalMonsterRegenDirect( void );
    void MonsterRegenCoolProcess( void );
    void CheckMonsterRegenState( void );
    void SetMonsterRegenCoolTimeMessage( void );

    void SetTimerText( void );
};
