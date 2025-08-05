// KGCPlayerElesis2.h: interface for the KGCPlayerElesis2 class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCPLAYERELESIS2_H__A95CBF22_45DB_4BE6_993C_38C1AAA15287__INCLUDED_)
//#define AFX_KGCPLAYERELESIS2_H__A95CBF22_45DB_4BE6_993C_38C1AAA15287__INCLUDED_

#ifndef _KGCPLAYERELESIS2_H_
#define _KGCPLAYERELESIS2_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerElesis2  
{
public:
	KGCPlayerElesis2();
	virtual ~KGCPlayerElesis2();

	static int OnComboAtk(PLAYER* pPlayer, bool bKeyProc);

};

#endif // _KGCPLAYERELESIS2_H_
