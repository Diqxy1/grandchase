#pragma once
#include "TriggerCondition.h"

class CTGPositionParticle : public CTriggerCondition
{
public:
    CTGPositionParticle(void);
    ~CTGPositionParticle(void);

    int         m_iLeft;
    int         m_iRight;
    int         m_iTop;
    int         m_iBottom;
    int         m_iParticleID;

    void PositionParticle(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
