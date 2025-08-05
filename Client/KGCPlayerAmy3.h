#pragma once

// 파이팅모드 

class PLAYER;

class KGCPlayerAmy3
{
public:
    KGCPlayerAmy3(void);
    ~KGCPlayerAmy3(void);

    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumpUniqueSkill(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumpUniqueSkill2(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDoubleJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnRunToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnRunToDoubleJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtkToWaitJump(PLAYER* pPlayer, bool bKeyProc);
};
