#pragma once
#include "damage.h"

class CHardDamageRonan2MagicSpecial1 : public CDamage
{
public:
    CHardDamageRonan2MagicSpecial1(void);
    virtual ~CHardDamageRonan2MagicSpecial1(void);

    virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /* = 1.0f  */);
};
