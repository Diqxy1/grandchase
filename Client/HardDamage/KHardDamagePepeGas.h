#pragma once
#include "damage.h"

class KHardDamagePepeGas :
    public CDamage
{
public:
    KHardDamagePepeGas(void);
    virtual ~KHardDamagePepeGas(void);

    virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
    virtual void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );
};
