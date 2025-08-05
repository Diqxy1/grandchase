#pragma once
#include "BaseTrigger.h"

class CHP : public CBaseEventAction
{
public:
    CHP(void);
    ~CHP(void);

    bool        m_bPlayer;
    int         m_iSlot;
    int         m_iMonsterID;
    float       m_fHp;

    void Init(void);
    void SetValue(bool bPlayer, int iSlot, int iMonsterID, float fHp);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
