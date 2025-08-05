// KGCFormWolf.h: interface for the KGCFormWolf class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCFORMWOLF_H__381A0398_F871_4320_9420_2219A0BF8EE3__INCLUDED_)
//#define AFX_KGCFORMWOLF_H__381A0398_F871_4320_9420_2219A0BF8EE3__INCLUDED_

#ifndef _KGCFORMWOLF_H_
#define _KGCFORMWOLF_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PLAYER;

class KGCFormWolf  
{
public:
	KGCFormWolf();
	virtual ~KGCFormWolf();

	static int OnWait( PLAYER* pPlayer, bool bKeyProc );
	static int OnJump( PLAYER* pPlayer, bool bKeyProc );
	static int OnDash( PLAYER* pPlayer, bool bKeyProc );
	static int OnCombo1( PLAYER* pPlayer, bool bKeyProc );
	static int OnJumpAtk2( PLAYER* pPlayer, bool bKeyProc );
	static int OnWolfFormEnd( PLAYER* pPlayer, bool bKeyProc );
};

#endif // _KGCFORMWOLF_H_
