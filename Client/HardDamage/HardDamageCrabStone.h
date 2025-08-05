#pragma once
#include "damage.h"

class HardDamageCrabStone:
    public CDamage
{
public:
    HardDamageCrabStone(void);
    virtual ~HardDamageCrabStone(void);

    virtual void FrameMove( CDamageInstance* pDamageInstance );
};
