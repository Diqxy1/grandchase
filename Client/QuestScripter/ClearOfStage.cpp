#include "stdafx.h"
#include "ClearOfStage.h"

void CClearOfStage::Init(void)
{
    m_enType = TG_EACTION_CLEAROFSTAGE;
}

void CClearOfStage::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGClearOfStage", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CClearOfStage::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CClearOfStage *pClearOfStage = new CClearOfStage;
    *pClearOfStage = *(CClearOfStage*)this;
    *pDest = (CBaseEventAction*)pClearOfStage;
}

HRESULT CClearOfStage::LoadFromFile_Item(KLuaManager &luaMgr)
{
    return S_OK;
}