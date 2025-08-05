#pragma once
#include "TriggerCondition.h"

class CTGLose : public CTriggerCondition
{
public:
    CTGLose();
    ~CTGLose();

    void Lose(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
