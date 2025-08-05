#pragma once
#include "damage.h"

class CHardDamageGuardianStrike :
	public CDamage
{
public:
	CHardDamageGuardianStrike(void);
	virtual ~CHardDamageGuardianStrike(void);
	virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );

};
