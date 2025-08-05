#pragma once
#include "damage.h"

class CHardDamageXeniaMapDamage :
    public CDamage
{
public:	
    CHardDamageXeniaMapDamage(void);
    virtual ~CHardDamageXeniaMapDamage(void);

    virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
	virtual void FrameMove( CDamageInstance* pDamageInstance );

private:
	float fCheckContactTime;
	CDamageInstance* pDamage;
};
