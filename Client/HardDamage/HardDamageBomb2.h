#pragma once
#include "damage.h"

class CHardDamageBomb2 :
	public CDamage
{
public:
	CHardDamageBomb2(void);
	virtual ~CHardDamageBomb2(void);
	virtual void FrameMove(CDamageInstance* pDamageInstance );
	virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
	virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );
};
