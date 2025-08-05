#pragma once
#include "TriggerCondition.h"

class CTGMonsterGenerater : public CTriggerCondition
{
public:
    CTGMonsterGenerater();
    ~CTGMonsterGenerater();

    bool            m_bRight;
    int				m_iBoss;
    int             m_iMonIndex;
    int             m_iMonType;
    int             m_iMonLevel;
    float           m_fEntranceX;
    float           m_fEntranceY;
    bool            m_bRandom;
    int             m_iPercent;
    bool            m_bRelease;

    //void SetMonsterList(bool bRight, bool bBoss, int iMonIndex, int iMonType, int iMonLevel, float fEntranceX, float fEntranceY);
    void SetRelease( bool bRelease_ ) { m_bRelease = bRelease_; }
    void GenMonster( bool bRelease = false );
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};
