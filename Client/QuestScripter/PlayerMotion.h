#pragma once
#include "BaseTrigger.h"

class CPlayerMotion : public CBaseEventAction
{
public:
    CPlayerMotion(void);
    ~CPlayerMotion(void);

    int m_iSlot;
    int m_iMotion;

    void Init(void);
    void SetValue(int iSlot, int iMotion);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
