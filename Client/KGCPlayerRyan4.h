#pragma once

class PLAYER;

class KGCPlayerRyan4
{
public:
	KGCPlayerRyan4();
	virtual ~KGCPlayerRyan4();

    static int OnWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk1(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk2(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk3(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnFormChange(PLAYER* pPlayer, bool bKeyProc);
    static int OnFormChangeComplete(PLAYER* pPlayer, bool bKeyProc);
};
