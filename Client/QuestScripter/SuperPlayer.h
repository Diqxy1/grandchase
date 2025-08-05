#pragma once
#include "BaseTrigger.h"

class CSuperPlayer : public CBaseEventAction
{
public:
    CSuperPlayer(void);
    ~CSuperPlayer(void);

    bool m_bOnOff;
    int m_iPlayerIndex;

    void Init(void);
    void SetValue(int iPlayerIndex);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
