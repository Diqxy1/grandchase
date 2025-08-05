#pragma once

class PLAYER;

class KGCPlayerAmy
{
public:
    KGCPlayerAmy(void);
    ~KGCPlayerAmy(void);

    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnSpecial1Atk(PLAYER* pPlayer, bool bKeyProc);
    static int OnSpecial2Atk(PLAYER* pPlayer, bool bKeyProc);
    static int OnDoubleJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDoubleDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnRunToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDoubleDashToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnRunToDoubleJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnSpecialAtk2ToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk1ToWaitJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk2ToWaitJump(PLAYER* pPlayer, bool bKeyProc);
};
