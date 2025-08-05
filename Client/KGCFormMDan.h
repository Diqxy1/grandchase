#pragma once

class PLAYER;

class KGCFormMDan
{
public:
    KGCFormMDan(void);
    ~KGCFormMDan(void);

    static int OnWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpDash(PLAYER* pPlayer, bool bKeyProc);
	static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashToJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnRunToJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashToWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk1ToJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk2ToJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk2BackToJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk2BackToWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnDownAndStandUp(PLAYER* pPlayer, bool bKeyProc);
};
