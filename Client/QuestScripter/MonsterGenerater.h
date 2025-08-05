#pragma once
#include "BaseTrigger.h"

class CMonsterGenerater : public CBaseEventAction
{
public:
    CMonsterGenerater()
    {
        Init();
    }
    ~CMonsterGenerater() {}

    bool            m_bRight;
    int				m_iBoss;
    int             m_iMonIndex;
    int             m_iMonType;
    int             m_iMonLevel;
    float           m_fEntranceX;
    float           m_fEntranceY;
	bool			 m_bNotChampion;
    bool            m_bRandom;
    int             m_iPercent;

    void Init(void);
    void SetValue(bool bRight, int iBoss, int iMonIndex, int iMonType, int iMonLevel, int iX, int iY, bool bChamp_ = false, bool bRandom = false, int iPercent = 100 );
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
