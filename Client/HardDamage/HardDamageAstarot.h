#pragma once
#include "damage.h"

class CHardDamageAstarot:
	public CDamage
{
public:
	CHardDamageAstarot(void);
	virtual ~CHardDamageAstarot(void);
	void CheckAndDebuff(CDamageInstance* pDamageInstance);
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
	virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );
};