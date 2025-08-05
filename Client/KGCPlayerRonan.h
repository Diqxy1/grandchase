#pragma once

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerRonan
{
public:
	KGCPlayerRonan(void);
	virtual ~KGCPlayerRonan(void);

    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnWalk(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    //static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnBackStep(PLAYER* pPlayer, bool bKeyProc);
    static int OnBackStepAtk(PLAYER* pPlayer, bool bKeyProc);
};
