#pragma once
#include "damage.h"

class KHardDamageKamikiLightning:
    public CDamage
{
public:
    KHardDamageKamikiLightning(void);
    virtual ~KHardDamageKamikiLightning(void);

    virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
};
