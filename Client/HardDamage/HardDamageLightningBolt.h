#pragma once
#include "damage.h"

class CHardDamageLightningBolt :
	public CDamage
{
public:
	CHardDamageLightningBolt(void);
	virtual ~CHardDamageLightningBolt(void);
	virtual void Begin(CDamageInstance* pDamageInstance );
    virtual void BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY );
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
	virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );
};
