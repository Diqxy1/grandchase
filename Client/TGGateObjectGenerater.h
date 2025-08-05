#pragma once
#include "TriggerCondition.h"

class CTGGateObjectGenerater : public CTriggerCondition
{
public:
    CTGGateObjectGenerater(void);
    ~CTGGateObjectGenerater(void);

    bool        m_bOnOff;
    bool        m_bRight;
    bool        m_bGenerate;
    int         m_iSlot;
    int         m_iType;
    int         m_iDirection;
    int         m_iDelayTime;
    float       m_fEntranceX;
    float       m_fEntranceY;
    int         m_iDotaStage;

    void DestroyObj(int iSlot);
    void GenerateObj(int iSlot, int iType);
    void GenObject(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
