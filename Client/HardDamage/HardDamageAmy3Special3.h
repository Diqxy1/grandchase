#pragma once
#include "damage.h"

class CHardDamageAmy3Special3:
	public CDamage
{
public:
	CHardDamageAmy3Special3(void);
	virtual ~CHardDamageAmy3Special3(void);
	virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );
};
