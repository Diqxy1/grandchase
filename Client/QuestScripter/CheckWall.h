#pragma once
#include "BaseTrigger.h"

class CCheckWall : public CBaseCondition
{
public:
    CCheckWall(void);
public:
    ~CCheckWall(void);

    int		m_iSlot;
    bool    m_bLive;

    void Init(void);
    void SetValue( int iSlot, bool bLive );
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyConditionToDest(CBaseCondition **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
