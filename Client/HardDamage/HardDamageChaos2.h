#pragma once
#include "damage.h"

class CHardDamageChaosMapRail:
	public CDamage
{
public:
	CHardDamageChaosMapRail(void);
	virtual ~CHardDamageChaosMapRail(void);
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
	virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );
};