// KGCPlayerArme.h: interface for the KGCPlayerArme class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCPLAYERARME_H__8DC7B5AA_E18D_4D26_8521_85272454B9BD__INCLUDED_)
//#define AFX_KGCPLAYERARME_H__8DC7B5AA_E18D_4D26_8521_85272454B9BD__INCLUDED_

#ifndef _KGCPLAYERARME_H_
#define _KGCPLAYERARME_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerArme  
{
public:
	KGCPlayerArme();
	virtual ~KGCPlayerArme();


	static int OnWait(PLAYER* pPlayer, bool bKeyProc);
	static int OnWalk(PLAYER* pPlayer, bool bKeyProc);
	static int OnSpecial2Black(PLAYER* pPlayer, bool bKeyProc);
	static int OnComboAtk(PLAYER* pPlayer, bool bKeyProc);	
	static int OnBlackMagicFear(PLAYER* pPlayer, bool bKeyProc);	
	static int OnSpecialW2IceTrap(PLAYER* pPlayer, bool bKeyProc);	
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);

    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
};

#endif // _KGCPLAYERARME_H_
