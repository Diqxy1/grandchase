// KGCPlayerRyan.h: interface for the KGCPlayerRyan class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCPLAYERRYAN_H__381A0398_F871_4320_9420_2219A0BF8EE3__INCLUDED_)
//#define AFX_KGCPLAYERRYAN_H__381A0398_F871_4320_9420_2219A0BF8EE3__INCLUDED_

#ifndef _KGCPLAYERRYAN_H_
#define _KGCPLAYERRYAN_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PLAYER;

class KGCPlayerRyan  
{
public:
	KGCPlayerRyan();
	virtual ~KGCPlayerRyan();

	static int OnNatureShield(PLAYER* pPlayer, bool bKeyProc);
	static int OnWait(PLAYER* pPlayer, bool bKeyProc);
	static int OnJump(PLAYER* pPlayer, bool bKeyProc);
	static int OnCombo1(PLAYER* pPlayer, bool bKeyProc);
	static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumpAtk1(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumpAtk2(PLAYER* pPlayer, bool bKeyProc);
	static int OnJumpAtk2Land(PLAYER* pPlayer, bool bKeyProc);
	static int OnDash(PLAYER* pPlayer, bool bKeyProc);
	static int OnFormChange(PLAYER* pPlayer, bool bKeyProc);
	static int OnReturnToJumpOrWait(PLAYER* pPlayer, bool bKeyProc);
	static int OnDashAtkCancel(PLAYER* pPlayer, bool bKeyProc);
	static int OnChargeX(PLAYER* pPlayer, bool bKeyProc);
	static int OnRun(PLAYER* pPlayer, bool bKeyProc);

	static DWORD m_dwSuperTime;
	static float m_fChargeElapsedTime;
	
};

#endif // _KGCPLAYERRYAN_H_
