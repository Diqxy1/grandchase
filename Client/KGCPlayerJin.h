#pragma once

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerJin
{
public:
	KGCPlayerJin(void);
	virtual ~KGCPlayerJin(void);

    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnReturnToJumpOrWait(PLAYER* pPlayer, bool bKeyProc);
	static int OnDashAtkCombo(PLAYER* pPlayer, bool bKeyProc);
};
