#include "StdAfx.h"
#include ".\triggerready.h"

CTriggerReady::CTriggerReady(void)
{
    Init();
}

CTriggerReady::~CTriggerReady(void)
{
}

void CTriggerReady::Init(void)
{
    m_enType        = TG_EACTION_TRIGGERREADY;
    ZeroMemory(m_szTriggerName, sizeof(m_szTriggerName));
}

void CTriggerReady::SetValue(TCHAR *szTriggerName)
{
    StringCchCopyN( m_szTriggerName, sizeof(m_szTriggerName), szTriggerName, _tcslen(szTriggerName) );
}

void CTriggerReady::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;

    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGTriggerReady", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "TriggerName", CString(m_szTriggerName), iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CTriggerReady::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CTriggerReady *pTriggerReady = new CTriggerReady;
    *pTriggerReady = *(CTriggerReady*)this;
    *pDest = (CBaseEventAction*)pTriggerReady;
}

HRESULT CTriggerReady::LoadFromFile_Item(KLuaManager &luaMgr)
{
    CHAR szTriggerName[MAX_TG_STR_LENGTH] = {0};

    if( FAILED(luaMgr.GetValue("TriggerName", szTriggerName, sizeof(szTriggerName))) ) 
        return E_FAIL;

    std::wstring strTrigger = GCUTIL_STR::GCStrCharToWide( szTriggerName );

    StringCchCopyN( m_szTriggerName, sizeof( m_szTriggerName ), strTrigger.c_str(), strTrigger.length() );

    return S_OK;
}
