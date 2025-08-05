#pragma once
#include "BaseTrigger.h"

class CPlayerPosition : public CBaseEventAction
{
public:
    CPlayerPosition();
    ~CPlayerPosition();

    bool    m_bRight;
    int     m_iPlayerIndex;
    float   m_fEntranceX;
    float   m_fEntranceY;

    void Init(void);
    void SetValue(int iPlayerIndex, float fEntranceX, float fEntranceY);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
