#pragma once

class PLAYER;

class KGCFormNephilim
{
public:
    KGCFormNephilim(void);
    ~KGCFormNephilim(void);

    static int OnNephilimFormEnd(PLAYER* pPlayer, bool bKeyProc);
    static int OnWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnLand(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpAtk1(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpAtk2(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpAtk3(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpAtk4(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnWalk(PLAYER* pPlayer, bool bKeyProc);
	static int OnDash(PLAYER* pPlayer, bool bKeyProc);
};
