#pragma once
#include "damage.h"

class CHardDamageManyFruit :
	public CDamage
{
public:
	CHardDamageManyFruit(void);
	virtual ~CHardDamageManyFruit(void);
	virtual void AngleFrameMove(CDamageInstance* pDamageInstance );
};
