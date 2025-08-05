#pragma once
#include "BaseTrigger.h"

class CSubject : public CBaseEventAction
{
public:
    CSubject() { Init();  }
    ~CSubject() {}

    void Init(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void CopyEventActionToDest(CBaseEventAction **pDest);
    HRESULT LoadFromFile_Item(KLuaManager &luaMgr);

public:
    std::vector<int> m_vecConCharType;
    std::vector<int> m_vecConLevel;
    std::vector<int> m_vecConJobLevel;
    std::vector<std::pair<int, int>> m_vecSubject;   //pair( 과제ID, 확률 )
};
