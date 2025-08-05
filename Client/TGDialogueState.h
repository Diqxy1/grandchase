#pragma once
#include "TriggerCondition.h"

class CTGDialogueState : public CTriggerCondition
{
public:
    CTGDialogueState(void);
    ~CTGDialogueState(void);

    bool    m_bOnOff;

    void SetOnOff( bool bOnOff );
    void OnOff(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
