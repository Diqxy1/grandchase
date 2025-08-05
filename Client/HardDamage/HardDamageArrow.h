#pragma once
#include "damage.h"

class CHardDamageArrow : public CDamage
{
public:
    CHardDamageArrow(void);
    virtual ~CHardDamageArrow(void);

	virtual void CustomizeBeforeCreate( CDamageInstance* pDamageInstance );
	virtual void Begin( CDamageInstance* pDamageInstance );
    virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
    virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );
    static bool IsArrow( int iDamageType );
    static bool IsBolt( int iDamageType );
    static bool IsHugeArrow( int iDamageType );
    static bool IsNovaBolt_Arrow( int iDamageType );
};
