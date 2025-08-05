#pragma once
#include "BaseTrigger.h"

class CDialogue : public CBaseEventAction
{
public:
    CDialogue(void);
    ~CDialogue(void);

    bool m_bOnOff;
    int m_iChar;
    std::wstring m_strDialogue;    

    void Init(void);
    void SetValue(std::wstring strDialogue, bool bOnOff, int iChar);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
