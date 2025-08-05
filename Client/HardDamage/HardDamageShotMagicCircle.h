#pragma once
#include "damage.h"

class CHardDamageShotMagicCircle : public CDamage
{
public:
    CHardDamageShotMagicCircle(void);
    virtual ~CHardDamageShotMagicCircle(void);

	virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void FrameMove( CDamageInstance* pDamageInstance );

    int m_iCrashCount;
};
