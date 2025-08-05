#pragma once
#include "TriggerCondition.h"

class CTGReward : public CTriggerCondition
{
public:
    CTGReward(void);
    ~CTGReward(void);

    int m_iEXP;
    int m_iGP;

    void Reward(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
