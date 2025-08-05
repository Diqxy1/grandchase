#pragma once
#include "BaseTrigger.h"

class CWin : public CBaseEventAction
{
public:
    CWin() { Init();  }
    ~CWin() {}

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
