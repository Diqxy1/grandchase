#include "StdAfx.h"
#include ".\triggeronoff.h"

CTriggerOnOff::CTriggerOnOff(void)
{
    Init();
}

CTriggerOnOff::~CTriggerOnOff(void)
{
}

void CTriggerOnOff::Init(void)
{
    m_enType        = TG_EACTION_TRIGGERONOFF; 
    m_bOnOff        = true;
    m_bHasChar      = true;
    ZeroMemory(m_szTriggerName, sizeof(m_szTriggerName));

    m_vecConCharType.clear();
}

void CTriggerOnOff::SetValue(TCHAR *szTriggerName, bool bOnOff)
{
    StringCchCopyN( m_szTriggerName, sizeof(m_szTriggerName), szTriggerName, _tcslen( szTriggerName ) );
    m_bOnOff = bOnOff;
}

void CTriggerOnOff::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);    
    WriteToFileScript(hWrite, "FuctionName", "TGTriggerOnOff", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "Repeat", m_bRepeat, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);
    WriteToFileScript(hWrite, "On", m_bOnOff, iTab+1);
    WriteToFileScript(hWrite, "HasChar", m_bHasChar, iTab+1);
    CString strSubBarceTitle = L"Con_CharType";
    WriteOpenBrace_NoEnterAndNoBackTab(hWrite, strSubBarceTitle, iTab+1);
    {
        std::vector<int>::iterator vecIter = m_vecConCharType.begin();
        for(; vecIter != m_vecConCharType.end(); ++vecIter)
        {
            WriteToFileScript_OnlyValue_Number(hWrite, (*vecIter), 0, false );
        }
    }
    WriteCloseBrace(hWrite, 1, 0);
    WriteToFileScript(hWrite, "TriggerName", CString(m_szTriggerName), iTab+1);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CTriggerOnOff::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CTriggerOnOff *pTriggerOnOff = new CTriggerOnOff;
    *pTriggerOnOff = *(CTriggerOnOff*)this;
    *pDest = (CBaseEventAction*)pTriggerOnOff;
}

HRESULT CTriggerOnOff::LoadFromFile_Item(KLuaManager &luaMgr)
{
    LUA_GET_VALUE("On", m_bOnOff, return E_FAIL);
    luaMgr.GetValue( "HasChar", m_bHasChar );

    if( SUCCEEDED(luaMgr.BeginTable( "Con_CharType" )) )
    {
        int iCharType=-1;
        m_vecConCharType.clear();
        for(int iLoop=1; SUCCEEDED(luaMgr.GetValue( iLoop, iCharType )); ++iLoop)
        {
            m_vecConCharType.push_back( iCharType );
        }
        luaMgr.EndTable();
    }

    CHAR szTriggerName[MAX_TG_STR_LENGTH] = {0};
    if( FAILED(luaMgr.GetValue("TriggerName", szTriggerName, sizeof(szTriggerName))) ) 
        return E_FAIL;

    std::wstring strTrigger = GCUTIL_STR::GCStrCharToWide( szTriggerName );

    StringCchCopyN( m_szTriggerName, sizeof( m_szTriggerName ), strTrigger.c_str(), strTrigger.length() );

    return S_OK;
}