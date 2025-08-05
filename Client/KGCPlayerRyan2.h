#pragma once

class PLAYER;

class KGCPlayerRyan2
{
public:
    KGCPlayerRyan2(void);
    virtual ~KGCPlayerRyan2(void);

	static int OnWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc );
    static int OnJumpAtk1(PLAYER* pPlayer, bool bKeyProc );
    static int OnJumpAtk2(PLAYER* pPlayer, bool bKeyProc );
    static int OnJumpDashAtk(PLAYER* pPlayer, bool bKeyProc );
    static int OnJumpDash(PLAYER* pPlayer, bool bKeyProc );
    static int OnCombo(PLAYER* pPlayer, bool bKeyProc );
    static int OnFormChange(PLAYER* pPlayer, bool bKeyProc );
    static int OnSpecial3(PLAYER* pPlayer, bool bKeyProc );
};
