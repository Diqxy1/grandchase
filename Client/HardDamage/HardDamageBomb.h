#pragma once
#include "damage.h"

class CHardDamageBomb :
	public CDamage
{
public:
	CHardDamageBomb(void);
	virtual ~CHardDamageBomb(void);
	virtual void FrameMove(CDamageInstance* pDamageInstance );
};
