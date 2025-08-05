#pragma once
#include "TriggerCondition.h"

class CTGKillMaster : public CTriggerCondition
{
public:
    CTGKillMaster();
    ~CTGKillMaster();

    int         m_iMonIndex;                    // 몬스터의 인덱스
	int         m_iMonType;                     // 몬스터 종류
	bool         m_bAlive;                     // 몬스터 죽었는지 살았는지 체크

    void SetMonster( int iMonType, int iMonIndex );
    void FrameMove(void);
    void EndTrigger();
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);
};

