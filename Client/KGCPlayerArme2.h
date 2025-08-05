// KGCPlayerArme2.h: interface for the KGCPlayerArme2 class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCPLAYERARME2_H__381A0398_F871_4320_9420_2219A0BF8EE3__INCLUDED_)
//#define AFX_KGCPLAYERARME2_H__381A0398_F871_4320_9420_2219A0BF8EE3__INCLUDED_

#ifndef _KGCPLAYERARME2_H_
#define _KGCPLAYERARME2_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerArme2  
{
public:
	KGCPlayerArme2();
	virtual ~KGCPlayerArme2();

	static int OnComboAtk(PLAYER* pPlayer, bool bKeyProc);
	static int OnWait(PLAYER* pPlayer, bool bKeyProc);
	static int OnWalk(PLAYER* pPlayer, bool bKeyProc);
	//static int OnChangeHpToMp(PLAYER* pPlayer, bool bKeyProc);
};

#endif // _KGCPLAYERARME2_H_
