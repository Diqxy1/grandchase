#pragma once
//#include <windows.h>
#include "TriggerCondition.h"

class CTGTriggerOnOff : public CTriggerCondition
{
public:
    CTGTriggerOnOff(void);
    ~CTGTriggerOnOff(void);

    bool        m_bOnOff;
    bool        m_bHasChar;
    char        m_szTriggerName[MAX_TG_STR_LENTH];
    std::vector<int>    m_vecCharType;

    void TriggerOnOff(void);
    bool CheckCharType();

public:
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
