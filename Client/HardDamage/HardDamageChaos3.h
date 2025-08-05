#pragma once
#include "damage.h"

class CHardDamageBindWeb:
	public CDamage
{
public:
	CHardDamageBindWeb(void);
	virtual ~CHardDamageBindWeb(void);
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
	virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );
};