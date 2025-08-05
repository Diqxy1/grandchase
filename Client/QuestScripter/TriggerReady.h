#pragma once
#include "BaseTrigger.h"

class CTriggerReady : public CBaseEventAction
{
public:
    CTriggerReady(void);
    ~CTriggerReady(void);

    TCHAR m_szTriggerName[MAX_TG_STR_LENGTH];

    void Init(void);
    void SetValue(TCHAR *szTriggerName);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
