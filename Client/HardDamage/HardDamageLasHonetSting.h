#pragma once
#include "damage.h"

class CHardDamageLasHonetSting :
	public CDamage
{
public:
	CHardDamageLasHonetSting(void);
	virtual ~CHardDamageLasHonetSting(void);

	virtual void FrameMove( CDamageInstance* pDamageInstance );
};
