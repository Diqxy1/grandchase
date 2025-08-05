#pragma once
//#include <windows.h>
#include "TriggerCondition.h"

class PLAYER;
class MONSTER;

class CTGPosition : public CTriggerCondition
{
public:
    CTGPosition(void);
    ~CTGPosition(void);

    bool        m_bInOut;           // 영역안에 들어가있나 나가있나? [(In == true) (out == false)]
    RECT        m_rtBound;
    bool        m_bPlayer;
    bool        m_bHalfMore;
    int         m_iIndex;
    int         m_iType;
    PLAYER      *m_pPlayer;
    MONSTER     *m_pMonster;
    DWORD       m_dwReaminTime;

    bool CheckInOut(float x, float y);
    bool InRect(float x, float y);
    void SetRect(RECT rtBound);
    void FrameMove(void);
    void MonsterPosCheck(void);
    void PlayerPosCheck(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
    virtual void StartTrigger(void);
};
