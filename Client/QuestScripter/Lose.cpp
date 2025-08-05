#include "stdafx.h"
#include "Lose.h"

void CLose::Init(void)
{
    m_enType = TG_EACTION_LOSE;
}

void CLose::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGLose", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CLose::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CLose *pLose = new CLose;
    *pLose = *(CLose*)this;
    *pDest = (CBaseEventAction*)pLose;
}

HRESULT CLose::LoadFromFile_Item(KLuaManager &luaMgr)
{

    return S_OK;
}