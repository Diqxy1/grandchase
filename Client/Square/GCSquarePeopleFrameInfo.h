#pragma once

// [12/27/2007] breadceo. 
// 예전에 액션 스크립트화 되어 있던거를 cpp로 넣은거
// [1/11/2008] breadceo. 
// 특정 프레임에 공통으로 돌아야 할 것들을 넣는다.
// 점프 같은거는 리모트 플레이어는 받은거대로 처리해야 잘작동하므로 뺐다

//#include <map>
#include <wtypes.h>
//#include "KAssert.h"
//#include "GCEnum.h"
#include "GCSquarePeopleStateFunctions.h"

class GCSquarePeople;

typedef void (*GCSQUAREPEOPLEFRAMEPROC)( GCSquarePeople*, int );
#define DECL_FRAMEINFOPROC(p) static void On##p( GCSquarePeople* pPeople, int iFrame );
#define IMPL_FRAMEINFOPROC(p) void GCSquarePeopleFrameInfo::On##p( GCSquarePeople* pPeople, int iFrame )

class GCSquarePeopleFrameInfo
{
public:
    GCSquarePeopleFrameInfo(void);
    virtual ~GCSquarePeopleFrameInfo(void);

    // 기본 함수
    static void FrameInfoProc( GCSquarePeople* pPeople, UINT uiMotion, int iFrame );
    static void SetFrameInfoProc();

    // 예전 ActionScript.stg에 있던 특이한 함수들
    // [1/11/2008] breadceo. GCSquarePeople로 빼냄
    //static void Jump( GCSquarePeople* pPeople );

    // 예전 ActionScript.stg의 function Player_State%d
    //DECL_FRAMEINFOPROC( MID_ELESIS_WAIT );
    //DECL_FRAMEINFOPROC( MID_ELESIS_WALK );
    //DECL_FRAMEINFOPROC( MID_ELESIS_JUMP );
    //DECL_FRAMEINFOPROC( MID_ELESIS_JUMPREADY );

    static void OnEmpty( GCSquarePeople* pPeople, int iFrame );

protected:
    //static std::map< UINT, GCSQUAREPEOPLEFRAMEPROC > ms_FrameInfoProc;
    GCSQUARESTATEFUNCTION m_pCurStateFunc;
    GCSQUARESTATEFUNCTIONREMOTE m_pCurStateFuncRemote;
};
