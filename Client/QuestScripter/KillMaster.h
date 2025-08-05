#pragma once
#include "BaseTrigger.h"

class CKillMaster : public CBaseCondition
{
public:
    CKillMaster() { Init(); }
    ~CKillMaster() {}

    int             m_iMonIndex;
    int             m_iMonType;
	bool			m_bAlive;

    void Init(void);
	void SetValue(int iMonIndex, int iMonType);
	void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyConditionToDest(CBaseCondition **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
