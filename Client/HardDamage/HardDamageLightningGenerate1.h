#pragma once
#include "damage.h"

class CHardDamageLightningGenerate1 :
	public CDamage
{
public:
	CHardDamageLightningGenerate1(void);
	virtual ~CHardDamageLightningGenerate1(void);
	virtual void Begin(CDamageInstance* pDamageInstance );

	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
	virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );


};
