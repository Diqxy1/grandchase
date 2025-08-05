// KGCPlayerElesis4.h: interface for the KGCPlayerElesis class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCPLAYERELESIS4_H__429CC77B_A902_4190_8902_56AAA014F1EB__INCLUDED_)
//#define AFX_KGCPLAYERELESIS4_H__429CC77B_A902_4190_8902_56AAA014F1EB__INCLUDED_

#ifndef _KGCPLAYERELESIS4_H_
#define _KGCPLAYERELESIS4_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PLAYER;
struct PLAYER_TEMPLATE;

#define ELESIS4_SPECIAL2_SHOOT_FRAME (82)

class KGCPlayerElesis4
{
public:
    KGCPlayerElesis4();
    virtual ~KGCPlayerElesis4();

    static int OnWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
    static int OnJump(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpAtk1(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpAtk2(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpAtk5(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo1(PLAYER* pPlayer, bool bKeyProc);
    static int OnCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnBackStep(PLAYER* pPlayer, bool bKeyProc);
    static int OnSpecial2(PLAYER* pPlayer, bool bKeyProc);
    //static int OnCriticalDoubleAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpDash(PLAYER* pPlayer, bool bKeyProc);
    //static int OnDashAtk(PLAYER* pPlayer, bool bKeyProc);
};

#endif // _KGCPLAYERELESIS4_H_
