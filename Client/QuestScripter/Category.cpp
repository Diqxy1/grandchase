#include "stdafx.h"
#include "Category.h"

void CCategory::AddTrigger(CBaseTrigger *pBaseTrigger)
{
    m_vtBaseTrigger.push_back(pBaseTrigger);
}

void CCategory::AddTriggerToNextItem(CBaseTrigger *pBaseTrigger, HTREEITEM hDest)
{
    int iMaxElem = static_cast<int>(m_vtBaseTrigger.size());
    for(int iLoop = iMaxElem-1; iLoop >= 0; --iLoop)
    {
        if( m_vtBaseTrigger[iLoop]->m_hItem == hDest )
        {
            if( iLoop+1 == iMaxElem )
            {
                m_vtBaseTrigger.push_back( pBaseTrigger );
            }
            else
            {
                std::vector<CBaseTrigger*>::iterator vecIter = find(m_vtBaseTrigger.begin(), m_vtBaseTrigger.end(), m_vtBaseTrigger[iLoop+1]);
                m_vtBaseTrigger.insert( vecIter, pBaseTrigger );
            }
            break;
        }
    }
}

void CCategory::ClearCategory(void)
{
    for(int i=0;i<(int)(int)m_vtBaseTrigger.size();++i)
        SAFE_DELETE(m_vtBaseTrigger[i]);
    m_vtBaseTrigger.clear();
}

CCategory &CCategory::operator=(const CCategory &rhs)
{
    m_hItem             = rhs.m_hItem;
    this->m_strName     = rhs.m_strName;

    ClearCategory();
    for(int i=0;i<(int)rhs.m_vtBaseTrigger.size();i++)
    {
        CBaseTrigger *pBaseTrigger = new CBaseTrigger;
        *pBaseTrigger = *rhs.m_vtBaseTrigger[i];
        this->m_vtBaseTrigger.push_back(pBaseTrigger);
    }

    return *this;
}

void CCategory::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Category%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);
    WriteToFileScript(hWrite, "CategoryName", m_strName, iTab+1);
    for(int i=0;i<(int)m_vtBaseTrigger.size();++i)
        m_vtBaseTrigger[i]->SaveToFile(hWrite, iTab+1, i, iTG_ID);
    WriteCloseBrace(hWrite, iTab, 0);
}

HRESULT CCategory::LoadFromFile(KLuaManager& luaMgr)
{   

    std::string strName;
    LUA_GET_VALUE("CategoryName", strName, return E_FAIL);

    m_strName = GCUTIL_STR::GCStrCharToWide( strName.c_str() );

    return S_OK;
}

void CCategory::ChangeTGOnOffName(std::wstring strChanged, std::wstring strTGName)
{
    for(int i=0;i<(int)m_vtBaseTrigger.size();++i)
        m_vtBaseTrigger[i]->ChangeTGOnOffName(strChanged, strTGName);
}