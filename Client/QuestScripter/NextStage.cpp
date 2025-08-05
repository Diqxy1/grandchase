#include "StdAfx.h"
#include ".\nextstage.h"

CNextStage::CNextStage(void)
{
    Init();
}

CNextStage::~CNextStage(void)
{
    m_vtStage.clear();
    m_vtPercent.clear();
}

void CNextStage::Init(void)
{
    m_iLevel = 0;
    m_enType = TG_EACTION_NEXTSTAGE;
}

void CNextStage::SetValue(int iLevel, std::vector<int> &vtStage, std::vector<int> &vtPercent)
{
    m_iLevel = iLevel;
    m_vtStage.clear();
    m_vtPercent.clear();

    for(int i=0;i<(int)vtStage.size();++i)
        m_vtStage.push_back(vtStage[i]);

    for(int i=0;i<(int)vtPercent.size();++i)
        m_vtPercent.push_back(vtPercent[i]);
}

void CNextStage::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "NextStage", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "Level", m_iLevel, iTab+1);
    CString str = L"{";
    for(int i=0;i<(int)m_vtStage.size();i++)
    {
        CString strNum;
        strNum.Format(L"%d, ", m_vtStage[i]);
        str += strNum;
    }
    str += "}";
    WriteToFileScript_WithOut_QTMark(hWrite, "Stage", str, iTab+1);

    str = "{";
    for(int i=0;i<(int)m_vtPercent.size();i++)
    {
        CString strNum;
        strNum.Format(L"%d, ", m_vtPercent[i]);
        str += strNum;
    }
    str += "}";
    WriteToFileScript_WithOut_QTMark(hWrite, "Percent", str, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CNextStage::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CNextStage *pNextStage = new CNextStage;
    *pNextStage = *(CNextStage*)this;
    *pDest = (CBaseEventAction*)pNextStage;
}

CNextStage &CNextStage::operator=(const CNextStage &rhs)
{
    m_enType            = rhs.m_enType;
    m_strName           = rhs.m_strName;
    m_strDesc           = rhs.m_strDesc;

    m_vtStage.clear();
    m_vtPercent.clear();

    for(int i=0;i<(int)rhs.m_vtStage.size();++i)
        m_vtStage.push_back(rhs.m_vtStage[i]);

    for(int i=0;i<(int)rhs.m_vtPercent.size();++i)
        m_vtPercent.push_back(rhs.m_vtPercent[i]);

    m_iLevel = rhs.m_iLevel;

    return *this;
}

HRESULT CNextStage::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_BEGIN_TABLE("Stage", return E_FAIL);
    m_vtStage.clear();
    for(int i=1;;i++)
    {
        int iValue = 0;
        if( FAILED(luaMgr.GetValue(i, iValue)) ) 
            break;
        m_vtStage.push_back(iValue);
    }
    LUA_END_TABLE(return E_FAIL);

    LUA_BEGIN_TABLE("Percent", return E_FAIL);
    m_vtPercent.clear();
    for(int i=1;;i++)
    {
        int iValue = 0;
        if( FAILED(luaMgr.GetValue(i, iValue)) ) 
            break;
        m_vtPercent.push_back(iValue);
    }
    LUA_END_TABLE(return E_FAIL);

    return S_OK;
}