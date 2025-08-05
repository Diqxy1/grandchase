#pragma once
#include "damage.h"

class CHardDamageElesis4Special2 : public CDamage
{
public:
    CHardDamageElesis4Special2(void);
    virtual ~CHardDamageElesis4Special2(void);

    virtual void FrameMove( CDamageInstance* pDamageInstance );
};
