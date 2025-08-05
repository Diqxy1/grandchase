#pragma once
#include "damage.h"

class CHardDamageJinSpecial3Burnning:
	public CDamage
{
public:
	CHardDamageJinSpecial3Burnning(void);
	virtual ~CHardDamageJinSpecial3Burnning(void);
	virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );
};
