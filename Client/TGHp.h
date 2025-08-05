#pragma once
#include "TriggerCondition.h"

class CTGHp : public CTriggerCondition
{
public:
    CTGHp(void);
    ~CTGHp(void);

    bool    m_bPlayer;
    int     m_iSlot;
    int     m_iMonsterID;
    float   m_fHp;

    void HP(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
