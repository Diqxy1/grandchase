#pragma once
#include "BaseTrigger.h"

class CKillAllMonster : public CBaseCondition
{
public:
    CKillAllMonster() { Init(); }
    ~CKillAllMonster() {}

    bool			m_bAlive;

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyConditionToDest(CBaseCondition **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
