#pragma once
#include "TriggerCondition.h"

class CTGEmbark : public CTriggerCondition
{
public:
    CTGEmbark(void);
    ~CTGEmbark(void);

public:
    void InitData(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);

private:
    void LoadEmbark();

private:
    bool m_bBeginTG;
    bool m_bIsRight;
    int m_iEmbarkType;
    int m_iEmbarkSlot;
    int m_iEmbarkationCharSlot;
    D3DXVECTOR2 m_vPos;
};
