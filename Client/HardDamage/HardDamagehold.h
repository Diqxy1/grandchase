#pragma once
#include "damage.h"

class CHardDamageHold :
	public CDamage
{
public:
	CHardDamageHold(void);
	virtual ~CHardDamageHold(void);

	virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
    virtual void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );
};
