// KGCPlayerElesis3.h: interface for the KGCPlayerElesis2 class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCPLAYERELESIS3_H__A95CBF22_45DB_4BE6_993C_38C1AAA15287__INCLUDED_)
//#define AFX_KGCPLAYERELESIS3_H__A95CBF22_45DB_4BE6_993C_38C1AAA15287__INCLUDED_

#ifndef _KGCPLAYERELESIS3_H_
#define _KGCPLAYERELESIS3_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerElesis3  
{
public:
	KGCPlayerElesis3();
	virtual ~KGCPlayerElesis3();

	static int OnRun(PLAYER* pPlayer, bool bKeyProc);
	static int OnJump(PLAYER* pPlayer, bool bKeyProc);
	static int OnComboAtk(PLAYER* pPlayer, bool bKeyProc);

};

#endif // _KGCPLAYERELESIS3_H_
