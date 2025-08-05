#pragma once
#include "damage.h"

class CHardDamageLas4DownAtk : public CDamage
{
public:
    CHardDamageLas4DownAtk(void);
    virtual ~CHardDamageLas4DownAtk(void);

    virtual void FrameMove( CDamageInstance* pDamageInstance );
};
