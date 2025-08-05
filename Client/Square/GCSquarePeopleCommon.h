#pragma once

#include "GCSquarePeople.h"

class GCSquarePeopleCommon
{
public:
    GCSquarePeopleCommon(void);
    virtual ~GCSquarePeopleCommon(void);

    static void OnEmpty( GCSquarePeople* pPeople, bool bKeyProc ) { return; }

    static void OnEmptyRemote( GCSquarePeople* pPeople ) { return; }
    static void OnWaitRemote( GCSquarePeople* pPeople );
    static void OnWalkRemote( GCSquarePeople* pPeople );

    static bool CheckDownJump( GCSquarePeople* pPeople );
    static bool CheckDashJump( GCSquarePeople* pPeople );

    static void OnRemoteProcess( GCSquarePeople* pPeople );
    static void OnLoop( GCSquarePeople* pPeople );
    static void OnFrameEndState( GCSquarePeople* pPeople, GCSquarePeople::PEOPLE_STATE eState );
    static void OnJumpLoop( GCSquarePeople* pPeople, int iFrame );
    static void OnWaitStop( GCSquarePeople* pPeople, int iFrame );
    static void OnWaitLoop( GCSquarePeople* pPeople, int iFrame );

    static void OnJumpReadyRemote( GCSquarePeople* pPeople );

	static void OnJumpRemote( GCSquarePeople* pPeople );
	static void OnJump( GCSquarePeople* pPeople, bool bKeyProc );

    static void OnSocialMotion( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnSocialMotionRemote( GCSquarePeople* pPeople );
};
