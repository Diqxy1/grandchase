#pragma once
#include "TriggerCondition.h"

class CTGDeleteMonster : public CTriggerCondition
{
public:
    CTGDeleteMonster(void);
    ~CTGDeleteMonster(void);

    bool    m_bAllSlot;
    int     m_iSlot;
    int     m_iMonsterID;

    void DeleteMonster(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
