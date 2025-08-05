#pragma once
#include "TriggerCondition.h"

class CTGUnitParticle : public CTriggerCondition
{
public:
    CTGUnitParticle(void);
    ~CTGUnitParticle(void);

    bool m_bPlayer;
    int m_iSlot;
    int m_iMonsterID;
    int m_iParticleID;

    void UnitParticle(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
