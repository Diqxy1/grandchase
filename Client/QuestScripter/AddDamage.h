#pragma once
#include "BaseTrigger.h"

class CAddDamage : public CBaseEventAction
{
public:
    CAddDamage() { Init();  }
    ~CAddDamage() {}

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);

    int     m_iDamageID;
    int     m_iEntranceX;
    int     m_iEntranceY;
    DWORD   m_dwRecycleTime;

    // Options - Use Only WaterPole.
    int     m_iLife;
    int     m_iMySpeed;
    int     m_iStartTime;
    int     m_iJumpFrame;
    int     m_iCrashStart;
    int     m_iContinueance;

	int		m_iDir;

};
