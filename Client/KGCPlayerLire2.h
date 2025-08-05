// KGCPlayerLire2.h: interface for the KGCPlayerLire2 class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCPLAYERLIRE2_H__E6F734A4_8C07_40CA_9D41_A3D3ED253655__INCLUDED_)
//#define AFX_KGCPLAYERLIRE2_H__E6F734A4_8C07_40CA_9D41_A3D3ED253655__INCLUDED_

#ifndef _KGCPLAYERLIRE2_H_
#define _KGCPLAYERLIRE2_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerLire2  
{
public:
	KGCPlayerLire2();
	virtual ~KGCPlayerLire2();

	static int OnDash(PLAYER* pPlayer, bool bKeyProc);
	static int OnSpecial1(PLAYER* pPlayer, bool bKeyProc);
	static int OnComboAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumpReady(PLAYER* pPlayer, bool bKeyProc);
	static int OnJump(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumpAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumppingDownArrowAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnWait(PLAYER* pPlayer, bool bKeyProc);
	static int OnDamageAmplifier(PLAYER* pPlayer, bool bKeyProc);

};

#endif // _KGCPLAYERLIRE2_H_
