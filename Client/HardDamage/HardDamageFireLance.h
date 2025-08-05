#pragma once
#include "damage.h"

class CHardDamageFireLance :
	public CDamage
{
public:
	CHardDamageFireLance(void);
	virtual ~CHardDamageFireLance(void);
	virtual	void NotAngleFrameMove(CDamageInstance* pDamageInstance );
};
