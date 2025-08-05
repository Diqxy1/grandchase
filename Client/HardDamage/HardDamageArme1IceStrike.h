#pragma once
#include "damage.h"

class CHardDamageArme1IceStrike :
    public CDamage
{
public:	
    CHardDamageArme1IceStrike(void);
    virtual ~CHardDamageArme1IceStrike(void);

    virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
    virtual void CrashMonster( CDamageInstance* pDamageInstance, int Monsteri );

	bool m_bIsOn;
};
