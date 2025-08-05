#pragma once
#include "BaseTrigger.h"

class CReward : public CBaseEventAction
{
public:
    CReward(void);
    ~CReward(void);

    int m_iEXP;
    int m_iGP;

    void Init(void);
    void SetValue(int iExp, int iGp);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
