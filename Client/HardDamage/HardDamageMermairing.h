#pragma once
#include "damage.h"

class CHardDamageMermairing :
	public CDamage
{
public:
    CHardDamageMermairing(void){};
    virtual ~CHardDamageMermairing(void){};
	virtual void FrameMove(CDamageInstance* pDamageInstance );
};
