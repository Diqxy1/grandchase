#pragma once
#include "TriggerCondition.h"

class CTGPlayerMovingToArea : public CTriggerCondition
{
public:
    CTGPlayerMovingToArea(void);
    ~CTGPlayerMovingToArea(void);

    bool m_bPlayer;
    int m_iSlot;
    int m_iMonsterID;
    int m_iLeft;
    int m_iRight;
    int m_iTop;
    int m_iBottom;

    bool CheckExistNextBlock(float fPosX, float fPosY, bool bRight, float fWidth);
    void PlayerMovingToArea(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
