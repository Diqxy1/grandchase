#pragma once
#include "TriggerCondition.h"

class CDamageInstance;

class CTGAddDamage : public CTriggerCondition
{
public:
    CTGAddDamage(void);
    ~CTGAddDamage(void);

    int     m_iDamageID;
    int     m_iEntranceX;
    int     m_iEntranceY;
    DWORD   m_dwRecycleTime;

    // Options - Use Only WaterPole.
    int     m_iLife;
    int     m_iContinueance;
	int		m_iStartTime;
	int		m_iDir;

	
    DWORD                   m_dwTermTime;
    DWORD                   m_dwFirstTime;

    void AddDamage(void);
    void StartDamage(void);
    void FrameMove(void);
    HRESULT LoadItemFromLua(KLuaManager &luaMgr);

private:
	bool m_bIsStart;
};
