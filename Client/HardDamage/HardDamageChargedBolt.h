#pragma once
#include "damage.h"

class HardDamageChargedBolt :
    public CDamage
{
public:
    HardDamageChargedBolt(void);
    virtual ~HardDamageChargedBolt(void);

    virtual void FrameMove( CDamageInstance* pDamageInstance );
};
