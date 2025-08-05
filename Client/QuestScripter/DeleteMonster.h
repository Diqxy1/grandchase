#pragma once
#include "BaseTrigger.h"

class CDeleteMonster : public CBaseEventAction
{
public:
    CDeleteMonster(void);
    ~CDeleteMonster(void);

    bool    m_bAllSlot;
    int     m_iSlot;
    int     m_iMonsterID;

    void Init(void);
    void SetValue(bool bAllSlot, int iSlot, bool iMonsterID);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
