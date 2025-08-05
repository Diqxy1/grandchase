#pragma once
#include "BaseTrigger.h"

class CInfinityMonsterGen : public CBaseEventAction
{
public:
    CInfinityMonsterGen() { Init();  }
    ~CInfinityMonsterGen() {}

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);

public:
    bool m_bInfinityMonserGen;
};
