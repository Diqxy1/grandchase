#pragma once
#include "BaseTrigger.h"
//#include <vector>

class CNextStage : public CBaseEventAction
{
public:
    CNextStage(void);
    ~CNextStage(void);

    int m_iLevel;
    std::vector<int> m_vtStage;
    std::vector<int> m_vtPercent;

    void Init(void);
    void SetValue(int iLevel, std::vector<int> &vtStage, std::vector<int> &vtPercent);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
    CNextStage & operator=(const CNextStage &rhs);
};
