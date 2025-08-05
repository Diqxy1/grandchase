#pragma once
#include "BaseTrigger.h"

class CCheckHP : public CBaseCondition
{
public:
	CCheckHP(void);
public:
	~CCheckHP(void);

	int		m_iIndex;
	int		m_iType;
	float	m_fPercent;


	void Init(void);
	void SetValue(int iIndex, int iType, float fPercent );
	void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
	void CopyConditionToDest(CBaseCondition **pDest);
	HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
