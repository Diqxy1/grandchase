#pragma once
#include "BaseTrigger.h"

class CPlayerMovingToArea : public CBaseEventAction
{
public:
    CPlayerMovingToArea(void);
    ~CPlayerMovingToArea(void);

    bool m_bPlayer;
    int m_iSlot;
    int m_iMonsterID;
    int m_iLeft;
    int m_iRight;
    int m_iTop;
    int m_iBottom;

    void Init(void);
    void SetValue(bool bPlayer, int iSlot, int iMonsterID, int iLeft, int iRight, int iTop, int iBottom);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
