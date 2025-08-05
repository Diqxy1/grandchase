#pragma once

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerLas3
{
public:
	KGCPlayerLas3(void);
	virtual ~KGCPlayerLas3(void);

	static int OnWait( PLAYER* pPlayer, bool bKeyProc );
	static int OnDash( PLAYER* pPlayer, bool bKeyProc );
	static int OnJump( PLAYER* pPlayer, bool bKeyProc );
	static int OnJumpDash( PLAYER* pPlayer, bool bKeyProc );
	static int OnJumpAtk( PLAYER* pPlayer, bool bKeyProc );
	static int OnCombo1( PLAYER* pPlayer, bool bKeyProc );
	static int OnCombo2( PLAYER* pPlayer, bool bKeyProc );
	static int OnCombo3( PLAYER* pPlayer, bool bKeyProc );
	static int OnDashCombo1( PLAYER* pPlayer, bool bKeyProc );
	static int OnDashCombo2( PLAYER* pPlayer, bool bKeyProc );
	static int OnLanding( PLAYER* pPlayer, bool bKeyProc );
	static int OnSpecial1( PLAYER* pPlayer, bool bKeyProc );
};
