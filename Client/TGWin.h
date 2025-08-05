#pragma once
#include "TriggerCondition.h"

class CTGWin : public CTriggerCondition
{
public:
    CTGWin();
    ~CTGWin();

    void Win(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
