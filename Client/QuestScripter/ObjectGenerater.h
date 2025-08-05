#pragma once
#include "BaseTrigger.h"

class CObjectGenerater : public CBaseEventAction
{
public:
    CObjectGenerater(void);
    ~CObjectGenerater(void);

    bool        m_bOnOff;
    bool        m_bRight;
    bool        m_bGenerate;
    bool        m_bCrashAble;
    int         m_iSlot;
    int         m_iType;
    float       m_fEntranceX;
    float       m_fEntranceY;
	float	    m_fProperty;

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
