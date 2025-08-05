#pragma once

class PLATER;
struct PLAYER_TEMPLATE;

#define SUMMONCOUNT (2)
typedef int (*PLAYERSTATEFUNCTION)(PLAYER*,bool bKeyProc);

class KGCPlayerArme3
{
public:
	KGCPlayerArme3(void);
	virtual ~KGCPlayerArme3(void);

	static int OnWait( PLAYER* pPlayer, bool bKeyProc );
	static int OnWalk( PLAYER* pPlayer, bool bKeyProc );
	static int OnJumpReady( PLAYER* pPlayer, bool bKeyProc );
	static int OnJump( PLAYER* pPlayer, bool bKeyProc );
	//static int OnDash( PLAYER* pPlayer, bool bKeyProc );
	static int OnDashCombo1( PLAYER* pPlayer, bool bKeyProc );
	static int OnDashCombo2( PLAYER* pPlayer, bool bKeyProc );
	static int OnAtkReady( PLAYER* pPlayer, bool bKeyProc );
	static int OnNormal( PLAYER* pPlayer, bool bKeyProc );
	static int OnLowGradeSummon( PLAYER* pPlayer, bool bKeyProc );
	static int OnMiddleGradeSummon( PLAYER* pPlayer, bool bKeyProc );
	static int OnSummonJumpReady( PLAYER* pPlayer, bool bKeyProc );
	static int OnSpecial1( PLAYER* pPlayer, bool bKeyProc );
	static int OnSpecial2( PLAYER* pPlayer, bool bKeyProc );
	static int OnSpecial3( PLAYER* pPlayer, bool bKeyProc );
	static int OnLowSummoning( PLAYER* pPlayer, bool bKeyProc, bool bForceChange = true );
	static int OnUnSummon( PLAYER* pPlayer, bool bKeyProc );
	static int OnMiddleSummoning( PLAYER* pPlayer, bool bKeyProc, bool bForceChange = true );	

	//static int OnChangeHpToMp(PLAYER* pPlayer, bool bKeyProc);

	static KSafeArray<PLAYERSTATEFUNCTION,SUMMONCOUNT + 1> m_pSummonfunc;
};
