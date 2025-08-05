#pragma once
#include "BaseTrigger.h"

class CWater : public CBaseEventAction
{
public:
    CWater() { Init();  }
    ~CWater() {}

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);

    bool    m_bOnOff;
    int     m_iWaterHeight;
};
