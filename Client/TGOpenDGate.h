#pragma once
#include "TriggerCondition.h"

class CTGOpenDGate : public CTriggerCondition
{
public:
    CTGOpenDGate(void);
    ~CTGOpenDGate(void);

    int       m_iDirection;

    void OpenGate(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
