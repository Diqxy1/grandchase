#pragma once
#include "TriggerCondition.h"

class CTGClearOfStage : public CTriggerCondition
{
public:
    CTGClearOfStage();
    ~CTGClearOfStage();

    void ClearStage(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
