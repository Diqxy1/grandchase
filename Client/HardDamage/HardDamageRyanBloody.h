#pragma once
#include "damage.h"

class CHardDamageRyanBloody :
	public CDamage
{
public:
	CHardDamageRyanBloody(void);
	virtual ~CHardDamageRyanBloody(void);

	virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /* = 1.0f */ );
	virtual void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );
	virtual void FrameMove( CDamageInstance* pDamageInstance );
	virtual void End( CDamageInstance* pDamageInstance );
};
