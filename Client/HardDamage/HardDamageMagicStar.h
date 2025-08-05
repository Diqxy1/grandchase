#pragma once
#include "damage.h"

class CHardDamageMagicStar :
	public CDamage
{
public:
	CHardDamageMagicStar(void);
	virtual ~CHardDamageMagicStar(void);
	bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /* = 1.0f  */);
	void NotAngleFrameMove(CDamageInstance* pDamageInstance );
};
