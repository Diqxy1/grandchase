#pragma once
#include "BaseTrigger.h"

class CDialogueState : public CBaseCondition
{
public:
    CDialogueState(void) { Init(); }
    ~CDialogueState(void) {  }

    bool m_bOnOff;

    void Init(void);
    void SetValue(bool bOnOff);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyConditionToDest(CBaseCondition **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
