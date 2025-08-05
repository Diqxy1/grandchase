#include "StdAfx.h"
#include ".\titletext.h"

CTitleText::CTitleText(void)
{
    Init();
}

CTitleText::~CTitleText(void)
{
}

void CTitleText::Init(void)
{
    m_enType        = TG_EACTION_TITLETEXT;
    m_iStringID		= -1;
}

void CTitleText::SetValue(int iStringID)
{
    m_iStringID = iStringID;
}

void CTitleText::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TitleText", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "TitleTextStringID", m_iStringID, iTab+1);    
    WriteCloseBrace(hWrite, iTab, 0);
}

void CTitleText::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CTitleText *pTitleText = new CTitleText;
    *pTitleText = *(CTitleText*)this;
    *pDest = (CBaseEventAction*)pTitleText;
}

HRESULT CTitleText::LoadFromFile_Item(KLuaManager &luaMgr)
{
    if( FAILED(luaMgr.GetValue("TitleTextStringID", m_iStringID)) ) return E_FAIL;

    return S_OK;
}
