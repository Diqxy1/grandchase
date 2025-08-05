#pragma once
#include "TriggerCondition.h"

class CTGKillAllMonster : public CTriggerCondition
{
public:
    CTGKillAllMonster(void);
public:
    ~CTGKillAllMonster(void);

private:
    bool m_bLive;

public:
    virtual void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
