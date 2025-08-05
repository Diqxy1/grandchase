#pragma once
#include "damage.h"

class CHardDamageElesis4Special2Shoot : public CDamage
{
public:
    CHardDamageElesis4Special2Shoot(void);
    virtual ~CHardDamageElesis4Special2Shoot(void);

    virtual void FrameMove( CDamageInstance* pDamageInstance );
};
