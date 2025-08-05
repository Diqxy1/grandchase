// KGCPlayerLire.h: interface for the KGCPlayerLire class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCPLAYERLIRE_H__86BCB4AA_376B_4745_A225_7A83555D713E__INCLUDED_)
//#define AFX_KGCPLAYERLIRE_H__86BCB4AA_376B_4745_A225_7A83555D713E__INCLUDED_

#ifndef _KGCPLAYERLIRE_H_
#define _KGCPLAYERLIRE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerLire  
{
public:
	KGCPlayerLire();
	virtual ~KGCPlayerLire();

	static int OnDoubleJump(PLAYER* pPlayer, bool bKeyProc);
	static int OnComboAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnSiegeMode(PLAYER* pPlayer, bool bKeyProc);
	static int OnSiegeModeAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnDodge(PLAYER* pPlayer, bool bKeyProc);
	static int OnJump(PLAYER* pPlayer, bool bKeyProc);
	static int OnDamageAmplifier(PLAYER* pPlayer, bool bKeyProc);
	static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnDash(PLAYER* pPlayer, bool bKeyProc);
	static int OnWait(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumpDash(PLAYER* pPlayer, bool bKeyProc);
	static int OnShotMagicCircle(PLAYER* pPlayer, bool bKeyProc);
	static int OnShotMagicCircleInit(PLAYER* pPlayer, bool bKeyProc);
	static int OnContinuousCombo(PLAYER* pPlayer, bool bKeyProc);

	static float bodyAngle;
};

#endif // _KGCPLAYERLIRE_H_
