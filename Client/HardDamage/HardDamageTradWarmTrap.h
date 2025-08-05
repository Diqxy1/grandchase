#pragma once
#include "damage.h"

class CHardDamageTradWarmTrap :
	public CDamage
{
public:
	CHardDamageTradWarmTrap(void);
	virtual ~CHardDamageTradWarmTrap(void);

	virtual void BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY );
	virtual void FrameMove( CDamageInstance* pDamageInstance );
	virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
    virtual void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );

private:
	DWORD dwTime;
};
