// KGCPlayerElesis.h: interface for the KGCPlayerElesis class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _KGCPLAYERLAS_H_
#define _KGCPLAYERLAS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerLas  
{
public:
	KGCPlayerLas();
	virtual ~KGCPlayerLas();

	static int OnWait(PLAYER* pPlayer, bool bKeyProc);
	static int OnRun(PLAYER* pPlayer, bool bKeyProc);
	static int OnDash(PLAYER* pPlayer, bool bKeyProc);
	static int OnDashCombo1(PLAYER* pPlayer, bool bKeyProc);
	static int OnDoubleJump(PLAYER* pPlayer, bool bKeyProc);
	static int OnEvasion(PLAYER* pPlayer, bool bKeyProc);
	static int OnSpiningSlash(PLAYER* pPlayer, bool bKeyProc);
	static int OnThunderSlash(PLAYER* pPlayer, bool bKeyProc);
	static int OnThunderSlashInit(PLAYER* pPlayer, bool bKeyProc);
	static int OnNewDashAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumpAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnNewComboAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnDropping(PLAYER* pPlayer, bool bKeyProc);
	static int OnDodge(PLAYER* pPlayer, bool bKeyProc);
	static int OnTradWarm(PLAYER* pPlayer, bool bKeyProc);
	static int OnSuperJump(PLAYER* pPlayer, bool bKeyProc);

private:
	static float bodyAngle;
};

#endif // _KGCPLAYER_LASH_
