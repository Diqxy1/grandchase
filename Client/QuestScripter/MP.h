#pragma once
#include "BaseTrigger.h"

class CMP : public CBaseEventAction
{
public:
    CMP(void);
    ~CMP(void);

    bool        m_bPlayer;
    int         m_iSlot;
    int         m_iMonsterID;
    float       m_fMp;

    void Init(void);
    void SetValue(bool bPlayer, int iSlot, int iMonsterID, float iMp);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
