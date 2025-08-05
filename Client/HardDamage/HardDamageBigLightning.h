#pragma once
#include "damage.h"

class CHardDamageBigLightning :
    public CDamage
{
public:
    CHardDamageBigLightning(void);
    ~CHardDamageBigLightning(void);

    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void FrameMove( CDamageInstance* pDamageInstance );
};
