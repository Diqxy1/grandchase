#pragma once
#include "TriggerCondition.h"

class CTGDialogue : public CTriggerCondition
{
public:
    CTGDialogue(void);
    ~CTGDialogue(void);

    char    m_szDialgue[MAX_TG_STR_LENTH];
    bool    m_bOnOff;
    int     m_iShowChar;

    void Dialogue(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
