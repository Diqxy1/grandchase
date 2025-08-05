#pragma once
#include "damage.h"

class KGCHardDamagePoisonBreath :
	public CDamage
{
public:
	KGCHardDamagePoisonBreath(void);
	virtual ~KGCHardDamagePoisonBreath(void);

	virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );

	int m_iBreathIndex;
};
