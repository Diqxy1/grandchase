#pragma once
#include "BaseTrigger.h"

class CTitleText : public CBaseEventAction
{
public:
    CTitleText(void);
    ~CTitleText(void);

    int m_iStringID;

    void Init(void);
    void SetValue(int iStringID);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
