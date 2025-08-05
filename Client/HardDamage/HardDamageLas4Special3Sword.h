#pragma once
#include "damage.h"

class CHardDamageLas4Special3Sword : public CDamage
{
public:
    CHardDamageLas4Special3Sword(void);
    virtual ~CHardDamageLas4Special3Sword(void);

    virtual void FrameMove( CDamageInstance* pDamageInstance );
    virtual void UVOperation( CDamageInstance* pDamageInstance, int iVertexOffset, SLVERTEX*& v );
};
