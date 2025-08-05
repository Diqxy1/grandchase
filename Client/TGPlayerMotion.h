#pragma once
#include "TriggerCondition.h"

// 사용자 입력이 Disable 인 상태에서는 작동이 안되고 있습니다.
// 수정해야 하지만 시간이 걸려 차후로 미룹니다.
class CTGPlayerMotion : public CTriggerCondition
{
public:
    CTGPlayerMotion(void);
    ~CTGPlayerMotion(void);

    int         m_iSlot;
    int         m_iMotion;

    void PlayerMotion(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);

};
