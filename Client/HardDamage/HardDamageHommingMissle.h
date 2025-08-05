#pragma once
#include "damage.h"

class CHardDamageHommingMissle : public CDamage
{
public:
    CHardDamageHommingMissle(void);
    virtual ~CHardDamageHommingMissle(void);

    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void AngleFrameMove( CDamageInstance* pDamageInstance );
};
