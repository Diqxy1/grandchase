// ListBoxLog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WGameServer.h"
#include "ListBoxLog.h"
#include ".\listboxlog.h"
#include <KncUtil.h>

#define MAX_LOG_LINE        (55)

using namespace std;

// CListBoxLog
IMPLEMENT_DYNAMIC(CListBoxLog, CListBox)
CListBoxLog::CListBoxLog() // : m_EditStrBuf(this), m_EditStrBufW(this),m_EditStrBufError(this), m_EditStrBufErrorW(this)
{
    ::InitializeCriticalSection( &m_csLock );           
}

CListBoxLog::~CListBoxLog()
{
    ::DeleteCriticalSection( &m_csLock );   
}

void CListBoxLog::AddMsg(char* strMsg)
{   
    AddMsg( (wchar_t*)KncUtil::toWideString(strMsg).c_str());   
}

void CListBoxLog::AddMsg(wchar_t* strMsg)
{       
    ::EnterCriticalSection( &m_csLock );
    {
        m_vecQ.push_back(strMsg);
    }
    ::LeaveCriticalSection( &m_csLock );    
}

void CListBoxLog::Display()
{
    // Copy List : 이걸 복사하지 않고 바로 AddString을 부르면, Deadlock의 원인이 된다.  
    ::EnterCriticalSection( &m_csLock );
    {
        for(int i = 0; i < (int)m_vecQ.size(); i++)
        {
            m_vecDisplay.push_back(m_vecQ[i]);      
        }
        m_vecQ.clear();     
    }
    ::LeaveCriticalSection( &m_csLock );    

    if( m_vecDisplay.empty() )
        return;

    // Add List
    for(int i = 0; i < (int)m_vecDisplay.size(); i++)
    {       
        if( GetCount() > MAX_LOG_LINE -1 )
            DeleteString(0);
        AddString(m_vecDisplay[i].c_str());
    }
    m_vecDisplay.clear();   
}

//void CListBoxLog::BeginRedirect()
//{
//    // Redirect cout to our Edit-Control
//    m_pOldBuf = std::cout.rdbuf( &m_EditStrBuf );   
//    m_pOldBufW = std::wcout.rdbuf( &m_EditStrBufW );
//    m_pOldBufError = std::cerr.rdbuf( &m_EditStrBufError ); 
//    m_pOldBufErrorW = std::wcerr.rdbuf( &m_EditStrBufErrorW );
//    
//    printf("이것도 된다\n");
//    wcout << L"---Output Redirection Complete---" << endl;
//    
//}

//void CListBoxLog::EndRedirect()
//{   
//    cout.rdbuf( m_pOldBuf );
//    wcout.rdbuf( m_pOldBufW );
//    cerr.rdbuf( m_pOldBufError );   
//    wcerr.rdbuf( m_pOldBufErrorW );
//}

BEGIN_MESSAGE_MAP(CListBoxLog, CListBox)    
END_MESSAGE_MAP()
// CListBoxLog 메시지 처리기입니다.

BOOL CListBoxLog::DestroyWindow()
{   
    //EndRedirect();
    return CListBox::DestroyWindow();
}
