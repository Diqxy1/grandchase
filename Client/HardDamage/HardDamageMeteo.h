#pragma once
#include "damage.h"

class CHardDamageMeteo :
	public CDamage
{
public:
	CHardDamageMeteo(void);
	virtual ~CHardDamageMeteo(void);

	virtual void Begin( CDamageInstance* pDamageInstance );
	virtual void NotAngleFrameMove(CDamageInstance* pDamageInstance );
	virtual void AngleFrameMove(CDamageInstance* pDamageInstance);

    virtual void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );

};
