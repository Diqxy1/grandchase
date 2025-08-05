#pragma once
#include "damage.h"

class CHardDamageShotMagicCircleDun : public CDamage
{
public:
    CHardDamageShotMagicCircleDun(void);
    virtual ~CHardDamageShotMagicCircleDun(void);

	virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void FrameMove( CDamageInstance* pDamageInstance );

    int m_iCrashCount;
};
