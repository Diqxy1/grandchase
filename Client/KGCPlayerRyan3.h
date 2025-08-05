#pragma once

class PLAYER;

class KGCPlayerRyan3
{
public:
	KGCPlayerRyan3();
	virtual ~KGCPlayerRyan3();

	static int OnWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashEnd(PLAYER* pPlayer, bool bKeyProc);
};
