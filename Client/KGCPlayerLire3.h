// KGCPlayerLire3.h: interface for the KGCPlayerLire2 class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCPLAYERLIRE3_H__E6F734A4_8C07_40CA_9D41_A3D3ED253655__INCLUDED_)
//#define AFX_KGCPLAYERLIRE3_H__E6F734A4_8C07_40CA_9D41_A3D3ED253655__INCLUDED_

#ifndef _KGCPLAYERLIRE3_H_
#define _KGCPLAYERLIRE3_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerLire3  
{
public:
	KGCPlayerLire3();
	virtual ~KGCPlayerLire3();

	static int OnWait(PLAYER* pPlayer, bool bKeyProc);
	static int OnRun(PLAYER* pPlayer, bool bKeyProc);
	static int OnJump(PLAYER* pPlayer, bool bKeyProc);
	static int OnComboAtk(PLAYER* pPlayer, bool bKeyProc);
	//static int OnLand(PLAYER* pPlayer, bool bKeyProc);
	static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnDashAtk1(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumpAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnSpecial2(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumpDash(PLAYER* pPlayer, bool bKeyProc);
	static int OnDamageAmplifier(PLAYER* pPlayer, bool bKeyProc);
};

#endif // _KGCPLAYERLIRE3_H_
