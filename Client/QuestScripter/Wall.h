#pragma once
#include "BaseTrigger.h"

class CWall : public CBaseEventAction
{
public:
    CWall() { Init();  }
    ~CWall() {}

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);

public:
    bool m_bDestroy;
    bool m_bIsCheckInBound;
    int m_iWallType;
    int m_iWallSlot;
    float m_fPosX;
};
