#pragma once
#include "TriggerCondition.h"

class CTGUnitMovement : public CTriggerCondition
{
public:
    CTGUnitMovement(void);
    ~CTGUnitMovement(void);

    bool        m_bPlayer;
    bool        m_bOnOff;
    int         m_iSlot;
    int         m_iMonsterID;

    void UnitMovement(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);

};
