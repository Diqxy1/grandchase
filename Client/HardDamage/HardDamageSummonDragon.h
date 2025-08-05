#pragma once
#include "../Damage.h"

class CHardDamageSummonDragon : public CDamage
{
private:
    int m_iFrameCount;
    int m_iTargetCount;
    bool bRight;
    int m_iWho;
public:
    CHardDamageSummonDragon(void);
    ~CHardDamageSummonDragon(void);

    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void FrameMove( CDamageInstance* pDamageInstance );
};


class CHardDamageSummonBlackDragon : public CDamage
{
private:
    int m_iFrameCount;
    int m_iTargetCount;
    bool bRight;
    int m_iWho;
public:
    CHardDamageSummonBlackDragon(void);
    ~CHardDamageSummonBlackDragon(void);

    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual void FrameMove( CDamageInstance* pDamageInstance );
};
