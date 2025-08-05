// KGCPlayerCommon.h: interface for the KGCPlayerCommon class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KGCPLAYERCOMMON_H__08DC5B59_CC91_4A25_8C1D_C3635E4136B1__INCLUDED_)
//#define AFX_KGCPLAYERCOMMON_H__08DC5B59_CC91_4A25_8C1D_C3635E4136B1__INCLUDED_

#ifndef _KGCPLAYERCOMMON_H_
#define _KGCPLAYERCOMMON_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "GCTeleporter.h"
#include "GCSKT.h"

class KGCPlayerCommon  
{
public:
	KGCPlayerCommon();
	virtual ~KGCPlayerCommon();

	static int OnEmpty(PLAYER* pPlayer, bool bKeyProc);
	static int OnEmptyNoContactCheck(PLAYER* pPlayer, bool bKeyProc);
	static int OnEmptyEndFrameLock(PLAYER* pPlayer, bool bKeyProc);
	static int OnEmptyLoop(PLAYER* pPlayer, bool bKeyProc);
    static int OnEmptyCancel(PLAYER* pPlayer, bool bKeyProc);

	static int OnJump(PLAYER* pPlayer, bool bKeyProc);	
	static int OnDownAndStandUp(PLAYER* pPlayer, bool bKeyProc);
	static int OnFrontDownAndStandUp(PLAYER* pPlayer, bool bKeyProc);
	static int OnStoneCurse(PLAYER* pPlayer, bool bKeyProc);
	static int OnDash(PLAYER* pPlayer, bool bKeyProc);
	static int OnWalk(PLAYER* pPlayer, bool bKeyProc);
	static int OnWait(PLAYER* pPlayer, bool bKeyProc);
    static int OnRun(PLAYER* pPlayer, bool bKeyProc);
	static int OnPortal(PLAYER* pPlayer, bool bKeyProc);
	static int OnWaitPortal(PLAYER* pPlayer, bool bKeyProc);
	static int OnIntoPortal(PLAYER* pPlayer, bool bKeyProc);

    static int OnFreezedByPixie(PLAYER* pPlayer, bool bKeyProc);

	static bool LoadPlayerTemplate( const std::vector< EGCSkillTree >& setSkill, std::string strFileName, PLAYER_TEMPLATE& Template, char cCharNum = 0 );
    static void MakeSoundLoadList();

	static bool CheckPortal(PLAYER* pPlayer, bool bKeyProc);

	static int OnElesisCommonCombo(PLAYER* pPlayer, bool bKeyProc);
	static int OnVerticalDashToJump(PLAYER* pPlayer, bool bKeyProc);

	static int OnHold(PLAYER* pPlayer, bool bKeyProc);
	static int OnBackTeleport(PLAYER* pPlayer, bool bKeyProc);

	static int OnBurnningCharge(PLAYER* pPlayer, bool bKeyProc);

	static int OnMotionToChangeBodyAngle( PLAYER* pPlayer, bool bKeyProc );
	static int OnMotionToJumpEnd( PLAYER* pPlayer, bool bKeyProc );

	static int OnContinueousCombo( PLAYER* pPlayer, bool bKeyProc );
    static int OnPositionFix( PLAYER* pPlayer, bool bKeyProc );

    static int DashJumpFunc( PLAYER* pPlayer, int iStateID = 0);
    static int DoubleJumpFunc( PLAYER* pPlayer, int iStateID = 0);

    static int OnSavingStance(PLAYER* pPlayer, bool bKeyProc);

    static void RegisterLuaBind();
    static std::set<std::string> ms_setLoadedTemplate;
    static KLuaManager ms_kLuaMgr_PlayerTemplate;
};

extern unsigned char    g_Push_Count;
extern unsigned char    g_Max_Push;
extern int              g_Hot_Count;

#endif // _KGCPLAYERCOMMON_H_
