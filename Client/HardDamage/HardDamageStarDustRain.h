#pragma once
#include "damage.h"

class CHardDamageStarDustRain : public CDamage
{
public:
    CHardDamageStarDustRain(void);
    virtual ~CHardDamageStarDustRain(void);

    virtual void BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY );
    virtual void FrameMove( CDamageInstance* pDamageInstance );
};
