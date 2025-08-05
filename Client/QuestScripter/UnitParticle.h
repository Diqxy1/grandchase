#pragma once
#include "BaseTrigger.h"

class CUnitParticle : public CBaseEventAction
{
public:
    CUnitParticle(void);
    ~CUnitParticle(void);

    bool        m_bPlayer;
    int         m_iSlot;
    int         m_iMonsterID;
    int         m_iParticleID;

    void Init(void);
    void SetValue(bool bPlayer, int iSlot, int iMonsterID, int iParticleID);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
