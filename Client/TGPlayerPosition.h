#pragma once
#include "TriggerCondition.h"

class CTGPlayerPosition : public CTriggerCondition
{
public:
    CTGPlayerPosition(void);
    ~CTGPlayerPosition(void);

    bool    m_bRight;
    int     m_iPlayerIndex;
    float   m_fEntranceX;
    float   m_fEntranceY;

    void SetPlayerPosition(int iPlayerIndex, float fEntranceX, float fEntranceY);
    void PlayerPosition(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
