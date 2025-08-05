#pragma once

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerJin2
{
public:
	KGCPlayerJin2(void);
	virtual ~KGCPlayerJin2(void);

	static int OnWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumpAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnReturnToJumpOrWait(PLAYER* pPlayer, bool bKeyProc);
	static int OnUniqueSkillTumbling(PLAYER* pPlayer, bool bKeyProc);
	static int OnTumblingToAction( PLAYER* pPlayer, bool bKeyProc);
	static int OnAerialCombo( PLAYER* pPlayer, bool bKeyProc);
	static int OnThrowBong( PLAYER* pPlayer, bool bKeyProc);
};
