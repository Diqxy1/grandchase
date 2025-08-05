#pragma once
#include "damage.h"

class CHardDamageFlingShield :
	public CDamage
{
public:
	CHardDamageFlingShield(void);
	virtual ~CHardDamageFlingShield(void);
	virtual	void NotAngleFrameMove(CDamageInstance* pDamageInstance );
	virtual void End(CDamageInstance* pDamageInstance );
};
