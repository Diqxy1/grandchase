#pragma once
#include "damage.h"

class CHardDamageFire : public CDamage
{
public:
	CHardDamageFire(void);
	virtual ~CHardDamageFire(void);
	
	virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );

};
