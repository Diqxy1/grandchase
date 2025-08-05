#pragma once
#include "BaseTrigger.h"

class CLose : public CBaseEventAction
{
public:
    CLose() { Init(); }
    ~CLose() {}

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};