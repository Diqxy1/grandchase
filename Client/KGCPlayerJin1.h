#pragma once

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerJin1
{
public:
	KGCPlayerJin1(void);
	virtual ~KGCPlayerJin1(void);

    static int OnWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnReturnToJumpOrWait(PLAYER* pPlayer, bool bKeyProc);
	static int OnDashAtkCombo(PLAYER* pPlayer, bool bKeyProc);
	static int OnUniqueSkillTumbling(PLAYER* pPlayer, bool bKeyProc);
	static int OnTumblingToAction( PLAYER* pPlayer, bool bKeyProc);
	static int OnAerialStep( PLAYER* pPlayer, bool bKeyProc);
	static int OnHujin( PLAYER* pPlayer, bool bKeyProc);
	static int OnAerialCombo( PLAYER* pPlayer, bool bKeyProc);

private:
    static int m_iAerialStepCount;
};
