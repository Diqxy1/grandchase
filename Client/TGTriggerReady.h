#pragma once
#include "TriggerCondition.h"

class CTGTriggerReady : public CTriggerCondition
{
public:
    CTGTriggerReady(void);
    ~CTGTriggerReady(void);

    char        m_szTriggerName[MAX_TG_STR_LENTH];

    void TriggerReady(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
