#pragma once
#include "TriggerCondition.h"

class CTGWall : public CTriggerCondition
{
public:
    CTGWall(void);
    ~CTGWall(void);

public:
    void InitData(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);

private:
    void LoadWall();

private:
    bool m_bBeginTG;
    bool m_bDestroy;
    bool m_bIsCheckInBound;
    int m_iWallType;
    int m_iWallSlot;
    float m_fPosX;
};
