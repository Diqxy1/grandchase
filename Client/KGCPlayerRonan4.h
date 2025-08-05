#pragma once

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerRonan4
{
public:
	KGCPlayerRonan4(void);
	virtual ~KGCPlayerRonan4(void);

    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashToJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtkToWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnDashAtkCombo(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumpAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnUniqueAtk(PLAYER* pPlayer, bool bKeyProc);	
	static int OnSpecialCombo(PLAYER* pPlayer, bool bKeyProc);	
	static int OnMonoVolt(PLAYER* pPlayer, bool bKeyProc);	
	static int OnDashAtkToJump(PLAYER* pPlayer, bool bKeyProc);	
	static int OnWait( PLAYER* pPlayer, bool bKeyProc );
};
