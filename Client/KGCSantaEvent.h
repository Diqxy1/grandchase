#ifndef _KGCSANTAEVENT_H_
#define _KGCSANTAEVENT_H_

#pragma once

#include <d3dx9.h>
#include <stdlib.h>
//#include <map>
#include "animation/KTDGAnim.h"
#include "Particle/KGCParticleManager.h"

class KGCSantaEvent
{

public:
//변수
    CKTDGAnim::CAnimInstance*   m_pSantaAnim;
    D3DXVECTOR2                 m_vGap;
    D3DXVECTOR2                 m_vCurPos;
    D3DXVECTOR2                 m_vEndPos;
    bool                        m_bLive;
    int                         m_iLatency;
    float                       m_fAccumulateTime;
    float                       m_fEventTime;
    float                       m_fGiftDropTime;
    char                        m_cAppearCount;
	CParticleEventSeqPTR	m_pSantaEff;
//함수
    KGCSantaEvent(void);
    virtual ~KGCSantaEvent(void);
    void Create( D3DXVECTOR2 vStartPos_, D3DXVECTOR2 vEndPos_ );
    void FrameMove( float fElapsedTime_ );
    void Delete();
    void Init();
};
#endif//_KGCSANTAEVENT_H_
