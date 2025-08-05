#pragma once
#include "damage.h"

class CHardDamageSelion:
	public CDamage
{
public:
	CHardDamageSelion(void);
	virtual ~CHardDamageSelion(void);
	void CheckAndDebuff(CDamageInstance* pDamageInstance);
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
	virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );
};