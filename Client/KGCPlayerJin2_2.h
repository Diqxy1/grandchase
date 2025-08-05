#pragma once

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerJin2_2
{
public:
	KGCPlayerJin2_2(void);
	virtual ~KGCPlayerJin2_2(void);

	static int OnWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnReturnToJumpOrWait(PLAYER* pPlayer, bool bKeyProc);
	//static int OnDragonKick(PLAYER* pPlayer, bool bKeyProc);
	//static int OnDragonKickToJump(PLAYER* pPlayer, bool bKeyProc);
	static int OnDashAtkCombo(PLAYER* pPlayer, bool bKeyProc);
};
