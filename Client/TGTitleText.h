#pragma once
#include "TriggerCondition.h"

class CTGTitleText : public CTriggerCondition
{
public:
    CTGTitleText(void);
    ~CTGTitleText(void);

    int m_iTitleTextString;

    void TitleText(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
