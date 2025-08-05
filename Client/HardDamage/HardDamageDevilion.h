#pragma once
#include "damage.h"

class CHardDamageDevilion:
	public CDamage
{
public:
	CHardDamageDevilion(void);
	virtual ~CHardDamageDevilion(void);
	void CheckAndDebuff(CDamageInstance* pDamageInstance);
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
	virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );
};