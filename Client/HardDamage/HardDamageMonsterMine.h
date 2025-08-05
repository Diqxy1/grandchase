#pragma once
#include "damage.h"

class CHardDamageMonsterMine :	public CDamage
{
public:
	CHardDamageMonsterMine(void);
public:
	~CHardDamageMonsterMine(void);

	virtual void FrameMove(CDamageInstance* pDamageInstance );
};
