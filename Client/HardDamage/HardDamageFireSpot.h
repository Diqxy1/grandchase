#pragma once
#include "damage.h"

class HardDamageFireSpot : public CDamage
{
private:
	float m_fStartAngle;
public:
	HardDamageFireSpot(void);
	virtual ~HardDamageFireSpot(void);

	virtual void AngleFrameMove( CDamageInstance* pDamageInstance );
};
