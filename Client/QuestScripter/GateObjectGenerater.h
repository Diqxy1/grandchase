#pragma once
#include "BaseTrigger.h"

class CGateObjectGenerater : public CBaseEventAction
{
public:
    CGateObjectGenerater(void)  { Init();  }
    ~CGateObjectGenerater(void) { }

    bool        m_bOnOff;
    bool        m_bRight;
    bool        m_bGenerate;
    int         m_iSlot;
    int         m_iType;
    int         m_iDirection;
    int         m_iDelayTime;
    float       m_fEntranceX;
    float       m_fEntranceY;
    int         m_iDotaStage;

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
