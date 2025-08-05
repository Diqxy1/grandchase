#pragma once
#include "BaseTrigger.h"

class CChangeObjState : public CBaseEventAction
{
public:
    CChangeObjState(void) { Init();  }
    ~CChangeObjState(void) {}

    int m_iSlot;
	int m_iType;
	int m_iOBJType; // 몬스터인가 오브젝트 인가?
    int m_iState;

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};
