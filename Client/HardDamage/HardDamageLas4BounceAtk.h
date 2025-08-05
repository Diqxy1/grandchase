#pragma once
#include "damage.h"

class CHardDamageLas4BounceAtk : public CDamage
{
public:
    CHardDamageLas4BounceAtk(void);
    virtual ~CHardDamageLas4BounceAtk(void);

    virtual void FrameMove( CDamageInstance* pDamageInstance );
};
