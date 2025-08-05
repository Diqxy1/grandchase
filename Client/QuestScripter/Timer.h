#pragma once
#include "BaseTrigger.h"

class CTimer : public CBaseCondition
{
public:
    CTimer() 
    {
        Init();
    }
    ~CTimer() {}

    int           m_iUI;
    int           m_iTime;

    void Init(void);
    void SetValue(int iTime);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyConditionToDest(CBaseCondition **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};