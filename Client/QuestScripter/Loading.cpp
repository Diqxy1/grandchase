#include "stdafx.h"
#include "Loading.h"

void CLoading::Init(void)
{
    m_enType = TG_EACTION_LOADING;
    m_iTexID = 0;
    m_bLoad = true;
}

void CLoading::SetValue( bool bLoad, int iTexID )
{
    m_bLoad = bLoad;
    m_iTexID = iTexID;
}

void CLoading::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGLoading", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "On", m_bLoad, iTab+1);
    WriteToFileScript(hWrite, "LoadingID", m_iTexID, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CLoading::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CLoading *pLoading = new CLoading;
    *pLoading = *(CLoading*)this;
    *pDest = (CBaseEventAction*)pLoading;
}

HRESULT CLoading::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("On", m_bLoad, return E_FAIL);
    LUA_GET_VALUE("LoadingID", m_iTexID, return E_FAIL);

    return S_OK;
}