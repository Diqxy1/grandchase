#pragma once
#include "BaseTrigger.h"


//************************************************************************
// 이벤트, 액션 클래스 생성
//************************************************************************
class CClearOfStage : public CBaseEventAction
{
public:
    CClearOfStage() { Init(); }
    ~CClearOfStage() {}

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};