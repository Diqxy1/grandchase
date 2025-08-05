#pragma once
#include "BaseTrigger.h"

class CObjDestory : public CBaseCondition
{
public:
    CObjDestory(void) { Init(); }
    ~CObjDestory(void) {}

    int             m_iObjIndex;
    int             m_iObjType;

    void Init(void);
    void SetValue(int iObjIndex, int iObjType);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyConditionToDest(CBaseCondition **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);

};
