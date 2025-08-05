#pragma once
#include "BaseTrigger.h"

class CPosition : public CBaseCondition
{
public:
    CPosition()
    {
        Init();
    }
    ~CPosition() {}

    bool            m_bInOut;                   // 영역안에 들어가있나 나가있나? [(In == true) (out == false)]
    RECT            m_rtBound;
    bool            m_bPlayer;
    bool            m_bHalfMore;
    int             m_iIndex;
    int             m_iType;
    void Init(void);
    void SetValue(bool bInOut, bool bPlayer, bool bHalfMore, int iIndex, int iType, RECT rtBound);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyConditionToDest(CBaseCondition **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};