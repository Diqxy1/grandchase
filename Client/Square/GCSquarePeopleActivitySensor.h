#pragma once

// [12/27/2007] breadceo. 
// 광장에서 행동하는 것들을 감지해서 동기화를 맞춰주기 위한 클래스

// [12/27/2007] breadceo. 광장 UDP 동기화 개념
// 가정 : State가 바뀔때 마다 Keep해서 보내면 싱크 맞출 수 있다
// + 사기신공 ( 좌/우 이동한 프레임을 합산해서 정해진 스피드대로 돌려주는 방식 )
// UDP라서 안맞는건? -> 서버 싱크로 해결


#include <wtypes.h>
////#include <vector>
#include "Packet.h"
#include "GCSquarePeople.h"
//#include <sstream>

typedef struct SQUARE_PEOPLE_ACTIVITY_SAMPLE
{
    GCSquarePeople::PEOPLE_STATE eState;
    int nFrame;
    bool bIsRight;
    float fX;
    float fY;
    float fXSpeed;
    float fYSpeed;

    SQUARE_PEOPLE_ACTIVITY_SAMPLE()
    {
        Init();
    };

    void Init()
    {
        eState = GCSquarePeople::INVALID_PEOPLE;
        nFrame = 0;
        bIsRight = true;
        fX = 0.0f;
        fY = 0.0f;
        fXSpeed = 0.0f;
        fYSpeed = 0.0f;
    }
} SQUARE_PEOPLE_ACTIVITY_SAMPLE, *LPSQUARE_PEOPLE_ACTIVITY_SAMPLE;

#define IMPL_ACTIVITY_FUNC(p) bool GCSquarePeopleActivitySensor::p( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter )
typedef bool (*GCSQUAREPEOPLEACTIVITYTRANSITION)( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter );

class GCSquarePeopleActivitySensor
{
public:
    GCSquarePeopleActivitySensor(void);
    virtual ~GCSquarePeopleActivitySensor(void);

    // functions
    void AddData( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kSample );
    void AddRemoteData( const std::vector< SQUARE_PEOPLE_ACTIVITY_SAMPLE >& vecRemoteData );
    void AddRemoteData( const KGC_PID_SQUARE_ACTION& kRemoteData );
    //void AddInterpolationData( const SQUARE_PEOPLE_ACTIVITY_SAMPLE& kData );
    void FrameMove();
    void Summary();
    bool GetCurrentActivity( SQUARE_PEOPLE_ACTIVITY_SAMPLE& OUT kRet );
    void MakePacket( const SQUARE_PEOPLE_ACTIVITY_SAMPLE& kSample, OUT KGC_PID_SQUARE_ACTION& kPacket );
    void MakeRemoteData( OUT SQUARE_PEOPLE_ACTIVITY_SAMPLE& kSample, const KGC_PID_SQUARE_ACTION& kPacket );
    float GetJumpXSpeed() const;
    void DoSomethingatStateTransition( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kDoSomething, const SQUARE_PEOPLE_ACTIVITY_SAMPLE& kSample );
    void DoSomethingatResult( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kDoSomething, const SQUARE_PEOPLE_ACTIVITY_SAMPLE& kSample );
    bool DoNextFrame( SQUARE_PEOPLE_ACTIVITY_SAMPLE& OUT kRet );
    bool empty() const { return m_vecRemoteData.empty(); }
    void Clear();

    typedef std::map< GCSquarePeople::PEOPLE_STATE, GCSQUAREPEOPLEACTIVITYTRANSITION >::iterator IterTrans;
    static std::map< GCSquarePeople::PEOPLE_STATE, GCSQUAREPEOPLEACTIVITYTRANSITION > ms_pfActivityTransition;
    static void SetActivityTransitionFunc();

    // Transition Functions
    static bool TRANSITION_SOCIAL( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter );
    static bool TRANSITION_WAIT( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter );
    static bool TRANSITION_WALK( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter );
    static bool TRANSITION_JUMP_READY( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter );
    static bool TRANSITION_JUMP( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter );
    static bool TRANSITION_DASH( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter );
    static bool TRANSITION_DASH_JUMP( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter );

    // for debug
    void DumpOneActivity( std::wstringstream& stmLog, const SQUARE_PEOPLE_ACTIVITY_SAMPLE kSample );

private:
    typedef std::vector< SQUARE_PEOPLE_ACTIVITY_SAMPLE >::iterator IterSample;
    std::vector< SQUARE_PEOPLE_ACTIVITY_SAMPLE > m_vecCollectData;
    std::vector< SQUARE_PEOPLE_ACTIVITY_SAMPLE > m_vecResult;
    SQUARE_PEOPLE_ACTIVITY_SAMPLE m_kBeforeAct;
    int m_iCollectFrame;
    int m_iJumpCheckCounter;
    std::vector< SQUARE_PEOPLE_ACTIVITY_SAMPLE > m_vecRemoteData;
    int m_iRemoteActFrame;
};
