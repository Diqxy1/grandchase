#include "stdafx.h"
#include "Win.h"

void CWin::Init(void)
{
    m_enType = TG_EACTION_WIN;
}

void CWin::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGWin", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CWin::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CWin *pWin = new CWin;
    *pWin = *(CWin*)this;
    *pDest = (CBaseEventAction*)pWin;
}

HRESULT CWin::LoadFromFile_Item(KLuaManager &luaMgr)
{
    return S_OK;
}