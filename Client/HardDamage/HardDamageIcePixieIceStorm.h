#pragma once
#include "damage.h"

class KHardDamageIcePixieIceStorm :
    public CDamage
{
public:
    KHardDamageIcePixieIceStorm(void);
    virtual ~KHardDamageIcePixieIceStorm(void);

    virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
    virtual void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );
};
