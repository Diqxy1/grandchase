#pragma once
#include "../Damage.h"

class CHardDamageSummonInstrument : public CDamage
{
private:
    int m_iTime;
    float m_fWeight;
public:
    CHardDamageSummonInstrument(void);
    ~CHardDamageSummonInstrument(void);

    virtual bool CrashPlayer(CDamageInstance* pDamageInstance, int hitWho, float fDamageRate );
    virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );
    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void FrameMove( CDamageInstance* pDamageInstance );
};
