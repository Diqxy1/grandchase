#pragma once
#include "damage.h"

class CHardDamageKindPet:
	public CDamage
{
public:
	CHardDamageKindPet(void);
	virtual ~CHardDamageKindPet(void);
	virtual void FrameMove( CDamageInstance* pDamageInstance );

private:
	DWORD dwTime;
};

