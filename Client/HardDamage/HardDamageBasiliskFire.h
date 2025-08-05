#pragma once
#include "damage.h"

class CHardDamageBasiliskFire :
	public CDamage
{
public:
	CHardDamageBasiliskFire(void);
	virtual ~CHardDamageBasiliskFire(void);
	virtual void Begin(CDamageInstance* pDamageInstance );
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /* = 1.0f  */);

};
