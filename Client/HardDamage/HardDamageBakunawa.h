#pragma once
#include "damage.h"

class CHardDamageBakunawa:
	public CDamage
{
public:
	CHardDamageBakunawa(void);
	virtual ~CHardDamageBakunawa(void);
	void CheckAndDebuff(CDamageInstance* pDamageInstance);
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
	virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );
};