#include "stdafx.h"
#include ".\gcsquarepeopleactivitysensor.h"


#include "Message.h"

std::map< GCSquarePeople::PEOPLE_STATE, GCSQUAREPEOPLEACTIVITYTRANSITION > GCSquarePeopleActivitySensor::ms_pfActivityTransition;

GCSquarePeopleActivitySensor::GCSquarePeopleActivitySensor(void)
: m_iCollectFrame(0)
, m_iJumpCheckCounter(0)
, m_iRemoteActFrame(0)
{
    Clear();
    m_vecCollectData.reserve(55);
    m_vecRemoteData.reserve(20);
}

GCSquarePeopleActivitySensor::~GCSquarePeopleActivitySensor(void)
{
    Clear();
}

void GCSquarePeopleActivitySensor::FrameMove()
{
    m_iCollectFrame++;

    if( m_iCollectFrame == GC_FPS_LIMIT )
    {
        m_iCollectFrame = 0;
        Summary();
    }
}

void GCSquarePeopleActivitySensor::AddData( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kSample )
{
    m_vecCollectData.push_back( kSample );
}

void GCSquarePeopleActivitySensor::Summary()
{
    for( IterSample vit = m_vecCollectData.begin(); vit != m_vecCollectData.end(); ++vit )
    {
        if( vit == m_vecCollectData.begin() )
        {
            m_kBeforeAct = *vit;
            m_kBeforeAct.nFrame++;
            continue;
        }

        IterTrans mit = ms_pfActivityTransition.find( m_kBeforeAct.eState );
        ASSERT( mit != ms_pfActivityTransition.end() );
        // 만약에 스테이트 전이가 일어나서 담아야 되면
        if( true == (*mit->second)( m_kBeforeAct, *vit ) )
        {
            if( m_kBeforeAct.nFrame > 0 )
            {
                DoSomethingatStateTransition( m_kBeforeAct, *vit );
                m_vecResult.push_back( m_kBeforeAct );
            }
            m_kBeforeAct = *vit;
        }

        if( vit == m_vecCollectData.end() - 1 )
        {
            DoSomethingatResult( m_kBeforeAct, *vit );
        }
    }

    m_vecCollectData.clear();
    m_vecCollectData.reserve(55);

    if( !m_vecResult.empty() )
    {
        for( int i = 0; i < (int)m_vecResult.size(); ++i )
        {
            KGC_PID_SQUARE_ACTION kPacket;
            MakePacket( m_vecResult[i], kPacket );
            g_kSquarePacketPacker.Pack( &kPacket, sizeof(KGC_PID_SQUARE_ACTION ) );
        }
        // 전송
        SendNetworkData( GC_PID_SQUARE_ACTION, 0 );
    }
    m_vecResult.clear();
}

#define SET_ACTIVITY_TRANSITION_FUNC(p, q) ms_pfActivityTransition[GCSquarePeople::p] = q
void GCSquarePeopleActivitySensor::SetActivityTransitionFunc()
{
    SET_ACTIVITY_TRANSITION_FUNC( WAIT_PEOPLE, GCSquarePeopleActivitySensor::TRANSITION_WAIT );
    SET_ACTIVITY_TRANSITION_FUNC( WALK_PEOPLE, GCSquarePeopleActivitySensor::TRANSITION_WALK );
    SET_ACTIVITY_TRANSITION_FUNC( JUMP_READY_PEOPLE, GCSquarePeopleActivitySensor::TRANSITION_JUMP_READY );
    SET_ACTIVITY_TRANSITION_FUNC( JUMP_PEOPLE, GCSquarePeopleActivitySensor::TRANSITION_JUMP );
    SET_ACTIVITY_TRANSITION_FUNC( DASH_PEOPLE, GCSquarePeopleActivitySensor::TRANSITION_DASH );
    SET_ACTIVITY_TRANSITION_FUNC( DASH_JUMP_PEOPLE, GCSquarePeopleActivitySensor::TRANSITION_DASH_JUMP );
    SET_ACTIVITY_TRANSITION_FUNC( SOCIAL_PEOPLE, GCSquarePeopleActivitySensor::TRANSITION_SOCIAL );
}

bool GCSquarePeopleActivitySensor::TRANSITION_SOCIAL( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter )
{
    // 이전 상태 WAIT
    switch( kAfter.eState )
    {
    case GCSquarePeople::DASH_PEOPLE:
    case GCSquarePeople::WALK_PEOPLE:
    case GCSquarePeople::JUMP_READY_PEOPLE:
    case GCSquarePeople::JUMP_PEOPLE:    
    case GCSquarePeople::WAIT_PEOPLE:
        {
            kAfter.nFrame++;
            return true;
        }
        break; 
    case GCSquarePeople::SOCIAL_PEOPLE:
        {
            kBefore.nFrame++;
            return false;
        }
        break;
    case GCSquarePeople::DASH_JUMP_PEOPLE:
    default:
        {
            ASSERT( !"????? Invalid Transition" );
            return false;
        }
        break;
    }

    // 상태 전이 하지마!
    return false;
}

bool GCSquarePeopleActivitySensor::TRANSITION_WAIT( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter )
{
    // 이전 상태 WAIT
    switch( kAfter.eState )
    {
    case GCSquarePeople::DASH_PEOPLE:
    case GCSquarePeople::WALK_PEOPLE:
    case GCSquarePeople::JUMP_READY_PEOPLE:
    case GCSquarePeople::JUMP_PEOPLE:
    case GCSquarePeople::SOCIAL_PEOPLE:
        {
            kAfter.nFrame++;
            return true;
        }
        break;
    case GCSquarePeople::WAIT_PEOPLE:    
        {
            kBefore.nFrame++;
            return false;
        }
        break;
    case GCSquarePeople::DASH_JUMP_PEOPLE:
    default:
        {
            ASSERT( !"????? Invalid Transition" );
            return false;
        }
        break;
    }

    // 상태 전이 하지마!
    return false;
}

bool GCSquarePeopleActivitySensor::TRANSITION_WALK( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter )
{
    // 이전 상태 WALK
    switch( kAfter.eState )
    {
    case GCSquarePeople::WALK_PEOPLE:
        {
            if( kBefore.bIsRight == kAfter.bIsRight )
            {
                kBefore.nFrame++;
            }
            else
            {
                kBefore.nFrame--;
            }

            if( kBefore.nFrame == 0 )
            {
                kBefore.bIsRight = kAfter.bIsRight;
            }
            return false;
        }
        break;
    case GCSquarePeople::DASH_PEOPLE:
    case GCSquarePeople::JUMP_PEOPLE:
    case GCSquarePeople::JUMP_READY_PEOPLE:    
        {
            kAfter.nFrame++;
            return true;
        }
        break;
    case GCSquarePeople::WAIT_PEOPLE:
    case GCSquarePeople::SOCIAL_PEOPLE:
        {
            return false;
        }
        break;
    case GCSquarePeople::DASH_JUMP_PEOPLE:
    default:
        {
            ASSERT( !"????? Invalid Transition" );
            return false;
        }
        break;
    }

    // 상태 전이 하지마!
    return false;
}

bool GCSquarePeopleActivitySensor::TRANSITION_JUMP_READY( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter )
{
    // 이전 상태 JUMP_READY
    switch( kAfter.eState )
    {
    case GCSquarePeople::JUMP_READY_PEOPLE:
        {
            kBefore.nFrame++;
            return false;
        }
        break;
    case GCSquarePeople::JUMP_PEOPLE:
        {
            kAfter.nFrame++;
            kBefore.fX = kAfter.fX;
            kBefore.fY = kAfter.fY;
            return true;
        }
        break;
    case GCSquarePeople::WAIT_PEOPLE:
    case GCSquarePeople::WALK_PEOPLE:
    case GCSquarePeople::SOCIAL_PEOPLE:
        {
            kAfter.nFrame++;
            return true;
        }
        break;
    case GCSquarePeople::DASH_JUMP_PEOPLE:
    case GCSquarePeople::DASH_PEOPLE:
    default:
        {
            ASSERT( !"????? Invalid Transition" );
            return false;
        }
        break;
    }

    // 상태 전이 하지마!
    return false;
}

bool GCSquarePeopleActivitySensor::TRANSITION_JUMP( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter )
{
    // 이전 상태 JUMP
    switch( kAfter.eState )
    {
    case GCSquarePeople::JUMP_PEOPLE:
        {
            kBefore.nFrame++;
            return false;
        }
        break;
    case GCSquarePeople::DASH_PEOPLE:
    case GCSquarePeople::JUMP_READY_PEOPLE:
    case GCSquarePeople::WAIT_PEOPLE:
    case GCSquarePeople::WALK_PEOPLE:
    case GCSquarePeople::SOCIAL_PEOPLE:
        {
            kAfter.nFrame++;
            return true;
        }
        break;
    case GCSquarePeople::DASH_JUMP_PEOPLE:
    default:
        {
            ASSERT( !"????? Invalid Transition" );
            return false;
        }
        break;
    }

    // 상태 전이 하지마!
    return false;
}

void GCSquarePeopleActivitySensor::DumpOneActivity( std::wstringstream& stmLog, const SQUARE_PEOPLE_ACTIVITY_SAMPLE kSample )
{
    stmLog << L"STATE : " << GCSquarePeople::GetPeopleStateStr( kSample.eState )
        << L", X : " << kSample.fX << L", Y : " << kSample.fY
        << L", IsRight : " << kSample.bIsRight << L", Frame : " << kSample.nFrame << std::endl;
}

// @return 버퍼 다 썼는지
bool GCSquarePeopleActivitySensor::GetCurrentActivity( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kRet )
{
    if( m_vecRemoteData.empty() )
    {
        // WALK to WAIT 은 스테이트 전환 추가를 안하기 때문에 이렇게 처리한다
        if( kRet.eState == GCSquarePeople::WALK_PEOPLE )
        {
            kRet.eState = GCSquarePeople::WAIT_PEOPLE;
        }
        return true;
    }

    kRet = m_vecRemoteData[0];
    return false;
}

void GCSquarePeopleActivitySensor::AddRemoteData( const std::vector< SQUARE_PEOPLE_ACTIVITY_SAMPLE >& vecRemoteData )
{
    for( int i = 0; i < (int)vecRemoteData.size(); ++i )
    {
        m_vecRemoteData.push_back( vecRemoteData[i] );
    }
}

void GCSquarePeopleActivitySensor::AddRemoteData( const KGC_PID_SQUARE_ACTION& kRemoteData )
{
    SQUARE_PEOPLE_ACTIVITY_SAMPLE kSample;
    MakeRemoteData( kSample, kRemoteData );
    m_vecRemoteData.push_back( kSample );
}

void GCSquarePeopleActivitySensor::MakePacket( const SQUARE_PEOPLE_ACTIVITY_SAMPLE& kSample, OUT KGC_PID_SQUARE_ACTION& kPacket )
{
    kPacket.ucState = (UCHAR)kSample.eState;
    kPacket.ucRemainFrame = (UCHAR)kSample.nFrame;
    kPacket.usPosX = GCUTIL_MATH::floatToHalf( kSample.fX );
    kPacket.usPosY = GCUTIL_MATH::floatToHalf( kSample.fY );

    if( kSample.bIsRight )
    {
        kPacket.ucState += 128;
    }
}

void GCSquarePeopleActivitySensor::MakeRemoteData( OUT SQUARE_PEOPLE_ACTIVITY_SAMPLE& kSample, const KGC_PID_SQUARE_ACTION& kPacket_ )
{
    KGC_PID_SQUARE_ACTION kPacket;
    kPacket = kPacket_;

    if( kPacket.ucState & 128 )
    {
        kSample.bIsRight = true;
        kPacket.ucState -= 128;
    }
    else
    {
        kSample.bIsRight = false;
    }
    kSample.eState = (GCSquarePeople::PEOPLE_STATE)kPacket.ucState;
    kSample.nFrame = (int)kPacket.ucRemainFrame;
    kSample.fX = GCUTIL_MATH::halfToFloat( kPacket.usPosX );
    kSample.fY = GCUTIL_MATH::halfToFloat( kPacket.usPosY );
}

float GCSquarePeopleActivitySensor::GetJumpXSpeed() const
{
    if( m_vecRemoteData.empty() )
        return 0.0f;

    const SQUARE_PEOPLE_ACTIVITY_SAMPLE& kSample = (*m_vecRemoteData.begin());
    if( kSample.eState != GCSquarePeople::JUMP_PEOPLE &&
        kSample.eState != GCSquarePeople::DASH_JUMP_PEOPLE )
        return 0.0f;

    return kSample.fX / static_cast<float>(kSample.nFrame);
}

void GCSquarePeopleActivitySensor::DoSomethingatStateTransition( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kDoSomething, const SQUARE_PEOPLE_ACTIVITY_SAMPLE& kSample )
{
    if( kDoSomething.eState == GCSquarePeople::JUMP_PEOPLE || kDoSomething.eState == GCSquarePeople::DASH_JUMP_PEOPLE )
    {
        kDoSomething.fX = kSample.fX - kDoSomething.fX;

        // [1/10/2008] breadceo. 제자리 점프는 또 빼면 안된다 젠장
        if( kDoSomething.fX != 0.0f )
        {
            if( kDoSomething.bIsRight == true )
            {
                kDoSomething.fX += 0.012f;
            }
            else
            {
                kDoSomething.fX -= 0.012f;
            }
        }
    }
}

void GCSquarePeopleActivitySensor::DoSomethingatResult( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kDoSomething, const SQUARE_PEOPLE_ACTIVITY_SAMPLE& kSampleEnd )
{
    // [1/4/2008] breadceo. 스테이트가 딱 하나뿐이면 여기서 비어있다.
    if( m_vecResult.empty() )
    {
        if( m_kBeforeAct.eState != GCSquarePeople::WAIT_PEOPLE )
        {
            DoSomethingatStateTransition( kDoSomething, kSampleEnd );
            m_vecResult.push_back( kDoSomething );
        }
    }
    else
    {
        if( (m_vecResult[ m_vecResult.size() - 1 ].eState != kDoSomething.eState) &&
            (kDoSomething.nFrame != 0) )
        {
            DoSomethingatStateTransition( kDoSomething, kSampleEnd );
            m_vecResult.push_back( kDoSomething );
        }
    }
}

bool GCSquarePeopleActivitySensor::DoNextFrame( SQUARE_PEOPLE_ACTIVITY_SAMPLE& OUT kRet )
{
    if( m_vecRemoteData.empty() )
        return false;

    SQUARE_PEOPLE_ACTIVITY_SAMPLE& kSample = m_vecRemoteData[0];
    if( ++m_iRemoteActFrame >= kSample.nFrame )
    {
        m_iRemoteActFrame = 0;
        m_vecRemoteData.erase( m_vecRemoteData.begin() );

        if( m_vecRemoteData.empty() )
            return false;

        kRet = m_vecRemoteData[0];
        if( kRet.eState == GCSquarePeople::WALK_PEOPLE ||
            kRet.eState == GCSquarePeople::JUMP_PEOPLE ||
            kRet.eState == GCSquarePeople::DASH_JUMP_PEOPLE ||
            kRet.eState == GCSquarePeople::DASH_PEOPLE ||
            kRet.eState == GCSquarePeople::SOCIAL_PEOPLE )
        {
            return false;
        }
        return true;
    }
    return false;
}

void GCSquarePeopleActivitySensor::Clear()
{
    m_vecCollectData.clear();
    m_kBeforeAct.Init();
    m_vecResult.clear();
    m_vecRemoteData.clear();
}

bool GCSquarePeopleActivitySensor::TRANSITION_DASH( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter )
{
    // 이전 상태 DASH
    switch( kAfter.eState )
    {
    case GCSquarePeople::DASH_PEOPLE:
        {
            kBefore.nFrame++;
            return false;
        }
        break;
    case GCSquarePeople::JUMP_PEOPLE:
    case GCSquarePeople::DASH_JUMP_PEOPLE:
        {
            kAfter.nFrame++;
            kBefore.fX = kAfter.fX;
            kBefore.fY = kAfter.fY;
            return true;
        }
        break;
    case GCSquarePeople::WAIT_PEOPLE:
    case GCSquarePeople::WALK_PEOPLE:
    case GCSquarePeople::SOCIAL_PEOPLE:
        {
            kAfter.nFrame++;
            return true;
        }
        break;
    case GCSquarePeople::JUMP_READY_PEOPLE:
    default:
        {
            ASSERT( !"????? Invalid Transition" );
            return false;
        }
        break;
    }

    // 상태 전이 하지마!
    return false;
}

bool GCSquarePeopleActivitySensor::TRANSITION_DASH_JUMP( SQUARE_PEOPLE_ACTIVITY_SAMPLE& kBefore, SQUARE_PEOPLE_ACTIVITY_SAMPLE& kAfter )
{
    // 이전 상태 JUMP
    switch( kAfter.eState )
    {
    case GCSquarePeople::DASH_JUMP_PEOPLE:
        {
            kBefore.nFrame++;
            return false;
        }
        break;
    case GCSquarePeople::DASH_PEOPLE:
    case GCSquarePeople::JUMP_READY_PEOPLE:
    case GCSquarePeople::WAIT_PEOPLE:
    case GCSquarePeople::WALK_PEOPLE:
    case GCSquarePeople::SOCIAL_PEOPLE:
        {
            kAfter.nFrame++;
            return true;
        }
        break;
    case GCSquarePeople::JUMP_PEOPLE:
    default:
        {
            ASSERT( !"????? Invalid Transition" );
            return false;
        }
        break;
    }

    // 상태 전이 하지마!
    return false;
}