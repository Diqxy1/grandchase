#pragma once
#include "ParticleLib/KTDGParticleSystem.h"
#include "animation/KTDGAnim.h"
#include "GCGameObject.h"

class KGCJumpZone :
	public KGCGameObject
{
public:
	KGCJumpZone(void);
	~KGCJumpZone(void);

	virtual void Create( void );
	virtual void Destroy( void );
	virtual void Enable( void );
	virtual void FrameMove( float fElapsedTime = GC_ELAPSED_TIME );
	virtual void Render( void );	

	void SetJump(float j){m_fJump=j;}

private:

	GCCollisionRect<float> GetCollision( void );

	CParticleEventSeqPTR     m_pJumpParticle1;   	
	CParticleEventSeqPTR     m_pJumpParticle2;   	
	CParticleEventSeqPTR     m_pJumpParticle3;   	
	CParticleEventSeqPTR     m_pJumpEffectParticle1;   	
	CParticleEventSeqPTR     m_pJumpEffectParticle2;
	CKTDGAnim::CAnimInstance*                        m_pJumpZone;	

	float											m_fJump;

};
