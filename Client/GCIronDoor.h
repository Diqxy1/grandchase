#ifndef _GCIRONDOOR_H_
#define _GCIRONDOOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParticleLib/KTDGParticleSystem.h"
#include "animation/KTDGAnim.h"
//#include "GCGameObject.h"
#include "GCDirectionArrow.h"
#include "GCDoor.h"

class KGCIronDoor : public KGCDoor
{
public:
    KGCIronDoor( void );
    virtual ~KGCIronDoor( void );

public:
    virtual void Create( void );
    virtual void Destroy( void );
    virtual void Enable( void );
    virtual void FrameMove( float fElapsedTime = GC_ELAPSED_TIME );
    virtual void Render( void );
    virtual GCCollisionRect<float> GetCollisionRect( void );
    virtual bool OnDamage( int iPlayerIndex, float fAttackPower
                            , EGCAttackType eAttackType, EGCAttackDirection eAttackDirection
                            , float fPushTargetX, float fPushTargetY, bool bSpecialAttack );
    virtual bool IsLive( void ) const;


    
	virtual void SetDoorState(KGCDoor::IRON_DOOR_STATE enState);

    virtual bool IsCrashCheck(void) const;

private:
    CKTDGAnim::CAnimInstance*                        m_pIronDoorNormal;
    CKTDGAnim::CAnimInstance*                        m_pIronDoorDamage;
    CKTDGAnim::CAnimInstance*                        m_pIronDoorCrash;
    CParticleEventSeqPTR                              m_pDoorLockParticle;   	

};


#endif // _GCIRONDOOR_H_
