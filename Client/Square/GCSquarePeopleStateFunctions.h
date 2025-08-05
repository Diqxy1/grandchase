#pragma once
// [12/27/2007] breadceo. 
// state 별 돌아갈 함수를 저장하는 클래스임
// 로컬만 돌아감

//#include <map>
#include <wtypes.h>
//#include "GCEnum.h"

class GCSquarePeople;
typedef void (*GCSQUARESTATEFUNCTION)( GCSquarePeople* pPeople, bool bKeyProc );
typedef void (*GCSQUARESTATEFUNCTIONREMOTE)( GCSquarePeople* pPeople );

class GCSquarePeopleStateFunctions
{
public:
    enum{
        EWAIT_MOTION = 0,
        EWALK_MOTION = 1,
        EJUMP_MOTION = 2,
        EJUMP_READY_MOTION = 3,
        EDASH_MOTION = 4,
        ESOCIAL_MOTION = 5,
    };
public:
    GCSquarePeopleStateFunctions(void);
    virtual ~GCSquarePeopleStateFunctions(void);

    // [12/26/2007] InitLoading에서 한번 Call
    static void SetStateFunstions();
    static std::map<MOTIONID, GCSQUARESTATEFUNCTION> ms_mapStateFunction;
    static std::map<MOTIONID, GCSQUARESTATEFUNCTIONREMOTE> ms_mapStateFunctionRemote;

    static void SetWaitFunc();
    static void SetWalkFunc();
    static void SetJumpFunc();
    static void SetJumpReadyFunc();
    static void SetDashFunc();

    static void SetWaitRemoteFunc();
    static void SetWalkRemoteFunc();
    static void SetJumpRemoteFunc();
    static void SetJumpRemoteReadyFunc();
    static void SetDashRemoteFunc();

    static void SetSquarePeppleMotion();    

	//------------------------------------------------------------------------
	// 20091023 광장 모션을 모션함수와 연결 : 이동석
	static void SetSquareMotion();

    static void SetSocialMotionFunc();

    static std::map<char, UINT> ms_mapWaitCharMotion;
    static std::map<char, UINT> ms_mapWalkCharMotion;
    static std::map<char, UINT> ms_mapJumpReadyCharMotion;
    static std::map<char, UINT> ms_mapJumpCharMotion;
    static std::map<char, UINT> ms_mapDashCharMotion;

	//------------------------------------------------------------------------
	// 20091023 [광장 세트 아이템 모션 작업] : 이동석
	static std::map<int, UINT > ms_mapSquareWaitCharMotion;
	static std::map<int, UINT > ms_mapSquareWalkCharMotion;
	static std::map<int, UINT > ms_mapSquareJumpReadyCharMotion;
	static std::map<int, UINT > ms_mapSquareJumpCharMotion;
	static std::map<int, UINT > ms_mapSquareDashCharMotion;

    static std::map<int, std::set<int>> ms_mapMotionFunc;
};
