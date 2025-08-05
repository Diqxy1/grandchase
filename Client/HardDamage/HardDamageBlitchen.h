#pragma once
#include "damage.h"

class CHardDamageBlitchen :
    	public CDamage
{
public:
    CHardDamageBlitchen(void);
    ~CHardDamageBlitchen(void);
    virtual void FrameMove( CDamageInstance* pDamageInstance );
    void CheckAndDebuff(CDamageInstance* pDamageInstance);
    virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
    virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );

private:
    DWORD dwTime;
};
