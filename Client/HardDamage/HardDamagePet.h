#pragma once
#include "damage.h"

class CHardDamagePetSeamusBuff:
    public CDamage
{
public:
    CHardDamagePetSeamusBuff(void){};
    virtual ~CHardDamagePetSeamusBuff(void){};	

    virtual void End( CDamageInstance* pDamageInstance );
};