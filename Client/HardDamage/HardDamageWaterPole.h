#pragma once
#include "damage.h"

class CHardDamageWaterPole : public CDamage
{
public:
	CHardDamageWaterPole(void);
	virtual ~CHardDamageWaterPole(void);

	
	virtual bool CrashPlayer( CDamageInstance* pDamageInstance , int hitWho, float fDamageRate = 1.0f );
	virtual void End(CDamageInstance* pDamageInstance );
};
