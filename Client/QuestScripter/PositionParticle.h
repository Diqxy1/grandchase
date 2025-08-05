#pragma once
#include "BaseTrigger.h"

class CPositionParticle : public CBaseEventAction
{
public:
    CPositionParticle(void);
    ~CPositionParticle(void);

    int m_iLeft;
    int m_iRight;
    int m_iTop;
    int m_iBottom;
    int m_iParticleID;

    void Init(void);
    void SetValue(int iLeft, int iRight, int iTop, int iBottom, int iParticleID);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
