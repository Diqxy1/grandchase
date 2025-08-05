#pragma once
#include "damage.h"

class CHardDamageSaintBreeding :
	public CDamage
{
public:
	CHardDamageSaintBreeding(void);
	virtual ~CHardDamageSaintBreeding(void);

	//virtual void Begin( CDamageInstance* pDamageInstance );
	virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );

	int m_iDamageTime;
};
