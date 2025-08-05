#pragma once
#include "damage.h"
////#include <vector>

class CHardDamageElectronicBall : public CDamage
{
public:
    CHardDamageElectronicBall(void);
    virtual ~CHardDamageElectronicBall(void);

    virtual void Begin( CDamageInstance* pDamageInstance );
    virtual bool CrashPlayer( CDamageInstance* pDamageInstance, int hitWho, float fDamageRate = 1.0f );
    virtual void CrashMonster(CDamageInstance* pDamageInstance, int Monsteri );

    std::vector<int> m_vecFirstHit;
    std::vector<int> m_vecTwiceHit;

    int              m_iSoundOutput;
};
