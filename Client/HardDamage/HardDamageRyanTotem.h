#pragma once
#include "damage.h"

class CHardDamageRyanTotem :
	public CDamage
{
public:
	CHardDamageRyanTotem(void);
	virtual ~CHardDamageRyanTotem(void);

	virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
	virtual void FrameMove( CDamageInstance* pDamageInstance );
	virtual void End( CDamageInstance* pDamageInstance );
};
