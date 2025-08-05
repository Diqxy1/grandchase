#pragma once
#include "damage.h"

class CHardDamageMari2BuildLV2:
	public CDamage
{
public:
	CHardDamageMari2BuildLV2(void);
	virtual ~CHardDamageMari2BuildLV2(void);
	virtual void AngleFrameMove( CDamageInstance* pDamageInstance );
};


class CHardDamageMari2MagicBeadLv2:
    public CDamage
{
public:
    CHardDamageMari2MagicBeadLv2(void);
    virtual ~CHardDamageMari2MagicBeadLv2(void);
    virtual void FrameMove( CDamageInstance* pDamageInstance );
};