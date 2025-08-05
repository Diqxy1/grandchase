#pragma once

class PLAYER;

class KGCPlayerAmy4_2
{
public:
    KGCPlayerAmy4_2(void);
    ~KGCPlayerAmy4_2(void);

    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
	static int OnDoubleJump(PLAYER* pPlayer, bool bKeyProc);
	static int OnRunToDoubleJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
	static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnDashAtkToWaitJump(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumpDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnSpecialAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnDoubleAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnRunToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnModeChange(PLAYER* pPlayer, bool bKeyProc);
    static int OnModeChangeToWaitJump(PLAYER* pPlayer, bool bKeyProc);
};
