#pragma once
#include "damage.h"

class CHardDamageLasTradWarmPot:
	public CDamage
{
public:
	CHardDamageLasTradWarmPot(void);
	virtual ~CHardDamageLasTradWarmPot(void);
	virtual void Begin( CDamageInstance* pDamageInstance );
	virtual void FrameMove( CDamageInstance* pDamageInstance );
public:
};