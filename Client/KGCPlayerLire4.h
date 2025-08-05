#pragma once


class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerLire4
{
public:
    KGCPlayerLire4(void);
    ~KGCPlayerLire4(void);

    static int OnDoubleJump( PLAYER* pPlayer, bool bKeyProc );
    static int OnComboAtk( PLAYER* pPlayer, bool bKeyProc );
    static int OnRun( PLAYER* pPlayer, bool bKeyProc );
    static int OnDash( PLAYER* pPlayer, bool bKeyProc );
    static int OnDashBack( PLAYER* pPlayer, bool bKeyProc );
    static int OnJump( PLAYER* pPlayer, bool bKeyProc );
    static int OnWait( PLAYER* pPlayer, bool bKeyProc );
    static int OnSpecial3( PLAYER* pPlayer, bool bKeyProc );
    static int OnDoubleAttack2( PLAYER* pPlayer, bool bKeyProc );
    static int OnDamageAmplifier(PLAYER* pPlayer, bool bKeyProc);
};
