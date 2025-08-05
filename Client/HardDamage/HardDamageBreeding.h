#pragma once

#include "damage.h"

class CHardDamageBreeding : public CDamage
{
public:
	CHardDamageBreeding(void);
	virtual ~CHardDamageBreeding(void);

	virtual void AngleFrameMove(CDamageInstance* pDamageInstance);

private:
	bool m_bCrash;
	float m_fCrashPosY;
};
