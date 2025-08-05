#pragma once
#include "BaseTrigger.h"

class CTriggerOnOff : public CBaseEventAction
{
public:
    CTriggerOnOff(void);
    ~CTriggerOnOff(void);

    TCHAR m_szTriggerName[MAX_TG_STR_LENGTH];
    bool m_bOnOff;
    bool m_bHasChar;

    void Init(void);
    void SetValue(TCHAR *szTriggerName, bool bOnOff);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);

public:
    std::vector<int>    m_vecConCharType;
};
