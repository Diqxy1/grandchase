#pragma once
#include "damage.h"

class CHardDamageBigLightningReady :
    public CDamage
{
    int m_iFrameCount;
    int m_iTargetCount;
    bool bRight;
    int m_iWho;
public:
    CHardDamageBigLightningReady(void);
    ~CHardDamageBigLightningReady(void);

    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void End( CDamageInstance* pDamageInstance );
    virtual void FrameMove( CDamageInstance* pDamageInstance );
};
