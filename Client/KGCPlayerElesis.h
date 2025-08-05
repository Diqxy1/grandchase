// KGCPlayerElesis.h: interface for the KGCPlayerElesis class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCPLAYERELESIS_H__429CC77B_A902_4190_8902_56AAA014F1EB__INCLUDED_)
//#define AFX_KGCPLAYERELESIS_H__429CC77B_A902_4190_8902_56AAA014F1EB__INCLUDED_

#ifndef _KGCPLAYERELESIS_H_
#define _KGCPLAYERELESIS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PLAYER;
struct PLAYER_TEMPLATE;

class KGCPlayerElesis  
{
public:
	KGCPlayerElesis();
	virtual ~KGCPlayerElesis();

	static int OnRun(PLAYER* pPlayer, bool bKeyProc);
	static int OnComboAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnJumpAtk(PLAYER* pPlayer, bool bKeyProc);
    static int OnSkillDashAtk1(PLAYER* pPlayer, bool bKeyProc);
    static int OnDash(PLAYER* pPlayer, bool bKeyProc);
    static int OnSkillFastInput(PLAYER* pPlayer, bool bKeyProc);
    static int OnSkillSpecialAtk2Dash(PLAYER* pPlayer, bool bKeyProc);
    static int OnSkillSpecialAtk2DashCombo(PLAYER* pPlayer, bool bKeyProc);
    static int OnSkillSpecial2_Lv2(PLAYER* pPlayer, bool bKeyProc);
    static int OnSkillComboAttack(PLAYER* pPlayer, bool bKeyProc);    
};

#endif // _KGCPLAYERELESIS_H_
