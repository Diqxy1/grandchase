#pragma once
#include "damage.h"

class CHardDamageOctusWarcry :
    public CDamage
{
public:
    CHardDamageOctusWarcry(void);
    virtual ~CHardDamageOctusWarcry(void);

    virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );
};
