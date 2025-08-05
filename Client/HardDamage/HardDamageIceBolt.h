#pragma once
#include "damage.h"

class HardDamageIceBolt : public CDamage
{
public:
	HardDamageIceBolt(void);
	virtual ~HardDamageIceBolt(void);

	virtual void AngleFrameMove( CDamageInstance* pDamageInstance );
};
