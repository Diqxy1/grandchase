#pragma once

// 에이미 1차전직 준비 

class PLAYER;

class KGCPlayerAmy2_2
{
public:
    KGCPlayerAmy2_2(void);
    ~KGCPlayerAmy2_2(void);

    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    //static int OnJumpDash(PLAYER* pPlayer, bool bKeyProc);    
    static int OnSpecialAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnDoubleAtk(PLAYER* pPlayer, bool bKeyProc);
    //static int OnSpecial2Atk(PLAYER* pPlayer, bool bKeyProc);
    //static int OnDoubleJump(PLAYER* pPlayer, bool bKeyProc);
    //static int OnDoubleDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnRunToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnModeChange(PLAYER* pPlayer, bool bKeyProc);
    static int OnModeChangeToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    
    static int OnSpecial3Atk(PLAYER* pPlayer, bool bKeyProc);
    //static int OnDoubleDashToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    //static int OnRunToDoubleJump(PLAYER* pPlayer, bool bKeyProc);
    //static int OnSpecialAtk2ToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    //static int OnDashAtk1ToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    //static int OnDashAtk2ToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    //static int OnSpecial2(PLAYER* pPlayer, bool bKeyProc);
};
