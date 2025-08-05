#pragma once
#include "damage.h"

class CHardDamageKamikiLightning:
    public CDamage
{
public:
    CHardDamageKamikiLightning(void);
    virtual ~CHardDamageKamikiLightning(void);
    virtual void FrameMove( CDamageInstance* pDamageInstance );

private:
    DWORD dwTime;
};
class CHardDamageKamikiLightningStarter:
    public CDamage
{
public:
    CHardDamageKamikiLightningStarter(void);
    virtual ~CHardDamageKamikiLightningStarter(void);
    virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate /* = 1.0f */ );
};
