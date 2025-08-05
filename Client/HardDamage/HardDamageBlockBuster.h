#pragma once
#include "damage.h"

class CHardDamageBlockBuster :
	public CDamage
{
public:
	CHardDamageBlockBuster(void);
	virtual ~CHardDamageBlockBuster(void);
	virtual void AngleFrameMove(CDamageInstance* pDamageInstance );
};
