#pragma once

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerLas4
{
public:
	KGCPlayerLas4(void);
	virtual ~KGCPlayerLas4(void);

    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashEnd(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashEndJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnDoubleDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDoubleDashEnd(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo1(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo2(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo3(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo4(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnDoubleDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnChangeBodytoLog(PLAYER* pPlayer, bool bKeyProc);
	static int OnWait(PLAYER* pPlayer, bool bKeyProc);
};
