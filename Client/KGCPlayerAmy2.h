#pragma once

// 에이미 1차전직 파이팅모드 

class PLAYER;

class KGCPlayerAmy2
{
public:
    KGCPlayerAmy2(void);
    ~KGCPlayerAmy2(void);

    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDoubleJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnRunToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnRunToDoubleJump(PLAYER* pPlayer, bool bKeyProc);
    //static int OnSpecialAtkDeco(PLAYER* pPlayer, bool bKeyProc);
    static int OnUniqueSkill(PLAYER* pPlayer, bool bKeyProc);
    static int OnUniqueSkillAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk1ToWaitJump(PLAYER* pPlayer, bool bKeyProc);
};
