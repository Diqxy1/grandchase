#pragma once
#include "BaseTrigger.h"

class CEmbark : public CBaseEventAction
{
public:
    CEmbark() { Init();  }
    ~CEmbark() {}

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);

public:
    bool m_bIsRight;

    int m_iEmbarkType;
    int m_iEmbarkSlot;
    int m_iEmbarkationCharSlot;
    
    D3DXVECTOR2 m_vPos;
};
