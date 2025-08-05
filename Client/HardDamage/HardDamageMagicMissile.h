#pragma once
#include "damage.h"

class CHardDamageMagicMissile : public CDamage
{
public:
    CHardDamageMagicMissile(void);
    virtual ~CHardDamageMagicMissile(void);

	virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void FrameMove( CDamageInstance* pDamageInstance );
	virtual void End( CDamageInstance* pDamageInstance );
};

