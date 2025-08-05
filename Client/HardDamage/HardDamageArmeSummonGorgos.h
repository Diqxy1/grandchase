#pragma once
#include "damage.h"

class CHardDamageArmeSummonGorgos:
	public CDamage
{
public:
	CHardDamageArmeSummonGorgos(void);
	virtual ~CHardDamageArmeSummonGorgos(void);
	virtual void Begin( CDamageInstance* pDamageInstance );
	virtual void FrameMove( CDamageInstance* pDamageInstance );
public:
};