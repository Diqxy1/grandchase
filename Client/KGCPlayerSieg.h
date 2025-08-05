// KGCPlayerSieg1.h: interface for the KGCPlayerSieg1 class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCPlayerSieg1_H__381A0398_F871_4320_9420_2219A0BF8EE3__INCLUDED_)
//#define AFX_KGCPlayerSieg1_H__381A0398_F871_4320_9420_2219A0BF8EE3__INCLUDED_

#ifndef _KGCPlayerSieg1_H_
#define _KGCPlayerSieg1_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PLAYER;

class KGCPlayerSieg1  
{
public:
	KGCPlayerSieg1();
	virtual ~KGCPlayerSieg1();

	static int OnCombo(PLAYER* pPlayer, bool bKeyProc);	
	static int OnJumpAtk(PLAYER* pPlayer, bool bKeyProc);	
	static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnSpecial3(PLAYER* pPlayer, bool bKeyProc);
};

class KGCPlayerSieg2 :public KGCPlayerSieg1
{
public:
    KGCPlayerSieg2();
    virtual ~KGCPlayerSieg2();
    static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnPole(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpDownAtk(PLAYER* pPlayer, bool bKeyProc);
};

#endif // _KGCPlayerSieg1_H_

