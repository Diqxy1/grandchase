#pragma once

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerRonan3
{
public:
	KGCPlayerRonan3(void);
	virtual ~KGCPlayerRonan3(void);

    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashToJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtkToWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnGuard(PLAYER* pPlayer, bool bKeyProc);
    static int OnPsychicForce(PLAYER* pPlayer, bool bKeyProc);
    static int OnCriticalAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnSpecialAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnWait( PLAYER* pPlayer, bool bKeyProc );
};
