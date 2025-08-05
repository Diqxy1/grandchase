#pragma once
#include "TriggerCondition.h"

class CTGMp : public CTriggerCondition
{
public:
    CTGMp(void);
    ~CTGMp(void);

    bool        m_bPlayer;
    int         m_iSlot;
    int         m_iMonsterID;
    float       m_fMp;

    void MP(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
