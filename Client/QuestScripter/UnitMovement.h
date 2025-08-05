#pragma once
#include "BaseTrigger.h"

class CUnitMovement : public CBaseEventAction
{
public:
    CUnitMovement(void);
    ~CUnitMovement(void);

    bool    m_bPlayer;
    bool    m_bOnOff;
    int     m_iSlot;
    int     m_iMonsterID;

    void Init(void);
    void SetValue(bool bPlayer, bool bOnOff, int iSlot, int iMonsterID);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
