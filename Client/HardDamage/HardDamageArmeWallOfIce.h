#pragma once
#include "damage.h"

class CHardDamageArmeWallOfIce:
	public CDamage
{
public:
	CHardDamageArmeWallOfIce(void);
	virtual ~CHardDamageArmeWallOfIce(void);
	virtual void Begin( CDamageInstance* pDamageInstance );
	virtual void FrameMove( CDamageInstance* pDamageInstance );
public:
};