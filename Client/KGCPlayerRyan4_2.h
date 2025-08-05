#pragma once

class PLAYER;

class KGCPlayerRyan4_2
{
public:
	KGCPlayerRyan4_2();
	virtual ~KGCPlayerRyan4_2();

    static int OnWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    //static int OnBackDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnFormChange(PLAYER* pPlayer, bool bKeyProc);
    static int OnFormChangeComplete(PLAYER* pPlayer, bool bKeyProc);
};
