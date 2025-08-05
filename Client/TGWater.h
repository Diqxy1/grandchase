#pragma once
#include "TriggerCondition.h"

class CTGWater : public CTriggerCondition
{
public:
    CTGWater(void);
    ~CTGWater(void);

    void Water(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);

    bool    m_bOnOff;
    int     m_iWaterHeight;
};
