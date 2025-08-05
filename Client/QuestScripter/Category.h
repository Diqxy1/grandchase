#pragma once
#include "BaseTrigger.h"
//#include <string>

class CCategory
{
public:
    CCategory() { m_hItem = NULL; m_iStage = 0; }
    virtual ~CCategory() { ClearCategory(); }

    HTREEITEM               m_hItem;
    int                     m_iStage;
    std::wstring             m_strName;
    std::vector<CBaseTrigger*>   m_vtBaseTrigger;

    CCategory & operator=(const CCategory &rhs);
    void AddTrigger(CBaseTrigger *pBaseTrigger);
    void AddTriggerToNextItem(CBaseTrigger *pBaseTrigger, HTREEITEM hDest);
    void ClearCategory(void);
    void SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID);
    void ChangeTGOnOffName(std::wstring strChanged, std::wstring strTGName);
    HRESULT LoadFromFile(KLuaManager& luaMgr);
};