#include "StdAfx.h"
#include ".\Subject.h"

void CSubject::Init(void)
{
    m_enType            = TG_EACTION_SUBJECT;

    m_vecConCharType.clear();
    m_vecConLevel.clear();
    m_vecConJobLevel.clear();
    m_vecSubject.clear();
}

void CSubject::SaveToFile(HANDLE hWrite, int iTab, int iCnt, int &iTG_ID)
{
    CString strBraceTitle;
    strBraceTitle.Format(L"Function%d", iCnt);
    WriteOpenBrace(hWrite, strBraceTitle, iTab);
    WriteToFileScript(hWrite, "FuctionName", "TGSubject", iTab+1);
    WriteToFileScript(hWrite, "FuctionType", (int)m_enType, iTab+1);
    WriteToFileScript(hWrite, "FuctionDesc", m_strDesc, iTab+1);
    WriteToFileScript(hWrite, "TG_ID", iTG_ID++, iTab+1);

    // 캐릭터 타입조건 정보
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

    // 캐릭터 레벨조건 정보
    strSubBarceTitle = L"Con_Level";
    WriteOpenBrace_NoEnterAndNoBackTab(hWrite, strSubBarceTitle, iTab+1);
    {
        std::vector<int>::iterator vecIter = m_vecConLevel.begin();
        for(; vecIter != m_vecConLevel.end(); ++vecIter)
        {
            WriteToFileScript_OnlyValue_Number(hWrite, (*vecIter), 0, false );
        }
    }
    WriteCloseBrace(hWrite, 1, 0);

    // 캐릭터 전직조건 정보
    strSubBarceTitle = L"Con_JobLevel";
    WriteOpenBrace_NoEnterAndNoBackTab(hWrite, strSubBarceTitle, iTab+1);
    {
        std::vector<int>::iterator vecIter = m_vecConJobLevel.begin();
        for(; vecIter != m_vecConJobLevel.end(); ++vecIter)
        {
            WriteToFileScript_OnlyValue_Number(hWrite, (*vecIter), 0, false );
        }
    }
    WriteCloseBrace(hWrite, 1, 0);

    // 달성과제 정보
    strSubBarceTitle = L"Subject";
    WriteOpenBrace_NoEnterAndNoBackTab(hWrite, strSubBarceTitle, iTab+1);
    {
        std::vector<std::pair<int, int>>::iterator vecIter = m_vecSubject.begin();
        for(; vecIter != m_vecSubject.end(); ++vecIter)
        {
            WriteOnlyOpenBrace_NoEnter(hWrite, 0);
            WriteToFileScript_OnlyValue_Number(hWrite, vecIter->first, 0, false );
            WriteToFileScript_OnlyValue_Number(hWrite, vecIter->second, 0, false );
            WriteCloseBrace_NoEnter(hWrite, 0);
        }
    }
    WriteCloseBrace(hWrite, 1, 0);
    WriteCloseBrace(hWrite, iTab, 0);
}

void CSubject::CopyEventActionToDest(CBaseEventAction **pDest)
{
    CSubject *pSubject = new CSubject;
    *pSubject = *(CSubject*)this;
    *pDest = (CBaseEventAction*)pSubject;
}

HRESULT CSubject::LoadFromFile_Item(KLuaManager &luaMgr)
{
    Init();

    if( SUCCEEDED(luaMgr.BeginTable( "Con_CharType" )) )
    {
        int iCharType=-1;
        for(int iLoop=1; SUCCEEDED(luaMgr.GetValue( iLoop, iCharType )); ++iLoop)
        {
            m_vecConCharType.push_back( iCharType );
        }
        luaMgr.EndTable();
    }

    if( SUCCEEDED(luaMgr.BeginTable( "Con_Level" )) )
    {
        int iLevel = -1;
        for(int iLoop=1; SUCCEEDED(luaMgr.GetValue( iLoop, iLevel )); ++iLoop)
        {
            m_vecConLevel.push_back( iLevel );
        }
        luaMgr.EndTable();
    }

    if( SUCCEEDED(luaMgr.BeginTable( "Con_JobLevel" )) )
    {
        int iJobLevel = -1;
        for(int iLoop=1; SUCCEEDED(luaMgr.GetValue( iLoop, iJobLevel )); ++iLoop)
        {
            m_vecConJobLevel.push_back( iJobLevel );
        }
        luaMgr.EndTable();
    }

    if( SUCCEEDED(luaMgr.BeginTable( "Subject" )) )
    {
        int iRadio=100;
        int iSubjectType=0;
        for(int iLoop=1; SUCCEEDED(luaMgr.BeginTable( iLoop )); ++iLoop)
        {
            LUA_GET_VALUE_DEF( 1, iSubjectType, -1 );
            LUA_GET_VALUE_DEF( 2, iRadio, 100 );
            m_vecSubject.push_back( std::make_pair(iSubjectType, iRadio) );
            luaMgr.EndTable();
        }
        luaMgr.EndTable();
    }
    
    return S_OK;
}
