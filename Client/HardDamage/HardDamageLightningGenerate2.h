#pragma once
#include "damage.h"

class CHardDamageLightningGenerate2 :
	public CDamage
{
public:
	CHardDamageLightningGenerate2(void);
	virtual ~CHardDamageLightningGenerate2(void);
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
	virtual void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );


};
