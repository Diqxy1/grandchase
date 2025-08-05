#pragma once
#include "TriggerCondition.h"

class CTGSuperPlayer : public CTriggerCondition
{
public:
    CTGSuperPlayer(void);
    ~CTGSuperPlayer(void);

    bool        m_bOnOff;
    int         m_iPlayerIndex;
    
    void SuperPlayer(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
