#pragma once
#include "damage.h"

class CHardDamageElesis4DownAtk : public CDamage
{
public:
    CHardDamageElesis4DownAtk(void);
    virtual ~CHardDamageElesis4DownAtk(void);

    virtual void FrameMove( CDamageInstance* pDamageInstance );
};
