#ifndef _GCDIRECTIONARROW_H_
#define _GCDIRECTIONARROW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma once


#include "ParticleLib/KTDGParticleSystem.h"
#include "GCGameObject.h"


class KGCDirectionArrow : public KGCGameObject
{

public:
	KGCDirectionArrow();
	virtual ~KGCDirectionArrow();

	enum ARROW_PARTICLE { ARROW_RIGHT = 0, ARROW_DOWN, ARROW_LEFT, ARROW_UP, NUM_OF_ARROW };

	virtual void Destroy();		
	virtual void Create();
	virtual void Render(){};
	virtual void Enable(){};
	virtual void FrameMove(float fElapsedTime /* = GC_ELAPSED_TIME  */){};

	void ShowArrowParticle(int iDirection, bool bShow);
	void AllHideArrowParticle(void);	

	void SetPosition(float x,float y,float z);

private:
	KSafeArray<CParticleEventSeqPTR,NUM_OF_ARROW> m_pArrowParticle;
	D3DXVECTOR3 m_Pos;
};

#endif