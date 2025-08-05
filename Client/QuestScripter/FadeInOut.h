#pragma once
#include "BaseTrigger.h"

class CFadeInOut : public CBaseEventAction
{
public:
    CFadeInOut(void);
    ~CFadeInOut(void);

    bool m_bOnOff;

	int         m_iSpeed;
	int			m_Red;
	int			m_Green;
	int			m_Blue;

    void Init(void);
    void SetValue(int iSpeed, bool bOnOff, int iRed, int iGreen, int iBlue);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
