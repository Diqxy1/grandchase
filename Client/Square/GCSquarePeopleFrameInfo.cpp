#include "stdafx.h"
#include ".\gcsquarepeopleframeinfo.h"


//#define SET_FRAMEINFOPROC( p ) ms_FrameInfoProc[p] = GCSquarePeopleFrameInfo::On##p
//#define SET_FRAMEINFOPROC_EMPTY( p ) ms_FrameInfoProc[p] = GCSquarePeopleFrameInfo::OnEmpty

//std::map< UINT, GCSQUAREPEOPLEFRAMEPROC > GCSquarePeopleFrameInfo::ms_FrameInfoProc;

GCSquarePeopleFrameInfo::GCSquarePeopleFrameInfo(void)
{
    m_pCurStateFunc = NULL;
    m_pCurStateFuncRemote = NULL;
}

GCSquarePeopleFrameInfo::~GCSquarePeopleFrameInfo(void)
{
}

//void GCSquarePeopleFrameInfo::FrameInfoProc( GCSquarePeople* pPeople, UINT uiMotion, int iFrame )
//{
//    if( (int)ms_FrameInfoProc.count( uiMotion ) == 0 )
//    {
//        ASSERT( !"Please Register uiMotion" );
//        return;
//    }
//
//    return (*ms_FrameInfoProc[uiMotion])( pPeople, iFrame );
//}

//void GCSquarePeopleFrameInfo::SetFrameInfoProc()
//{
//    SET_FRAMEINFOPROC_EMPTY( MID_ELESIS_WAIT );
//    SET_FRAMEINFOPROC_EMPTY( MID_ELESIS_WALK );
//    SET_FRAMEINFOPROC_EMPTY( MID_ELESIS_JUMP );
//    SET_FRAMEINFOPROC_EMPTY( MID_ELESIS_JUMPREADY );
//}

// 만들어 놓고 보니 아직은?? 아에 안쓸거 같다..
void GCSquarePeopleFrameInfo::OnEmpty( GCSquarePeople* pPeople, int iFrame )
{
    return;
}
