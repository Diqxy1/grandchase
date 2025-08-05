#pragma once
#include "BaseTrigger.h"

class CLoading : public CBaseEventAction
{
public:
    CLoading()
    {
        Init();
    }
    ~CLoading() {}

    bool            m_bLoad;                        // 로딩 화면을 띄울지 말지 결정
    int             m_iTexID;

    void Init(void);
    void SetValue( bool bLoad, int iTexID );
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);
};