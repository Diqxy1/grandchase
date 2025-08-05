// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GCServerManager.h"
#include "KAddServerListDlg.h"
#include "KServerListDlg.h"
#include "KLoginDlg.h"
#include "MainFrm.h"
#include <string>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
    ON_COMMAND(KNotifyMsgDlg::ID_BTN_CLOSE, OnCloseNotifyMsgDlg)
    ON_COMMAND(KBlackListDlg::ID_BTN_CLOSE, OnCloseBlackListDlg)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame( void )
{
    CTime               time = CTime::GetCurrentTime();
    std::ostringstream  strmLogFileName;
    strmLogFileName << time.GetYear() << '-' << time.GetMonth() << '-'
                    << time.GetDay() << '-' << time.GetHour() << '-'
                    << time.GetMinute() << '-' << time.GetSecond() << ".txt";
    std::string strLogFileName = strmLogFileName.str();
//    m_logFile.open( strLogFileName.c_str() );
}

CMainFrame::~CMainFrame( void )
{
//    m_logFile.close();
}

int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

    ReadServerListFile();
    ReadBlackListFile();

    m_bKorean = false;

    KLoginDlg dlg( this );

    if ( IDOK == dlg.DoModal() )
    {
        m_strLogin  = dlg.m_cstrLoginID;
        m_strPasswd = dlg.m_cstrLoginPW;
        m_bKorean = dlg.m_bLangStateKorean;
    }
    else
    {
        PostMessage( WM_CLOSE );
    }

	return 0;
}

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CMDIFrameWnd::PreCreateWindow( cs ) )
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump( CDumpContext& dc ) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnFileNew( void ) 
{
	KAddServerListDlg dlg;

    if ( IDOK == dlg.DoModal() )
    {
        SServerInfo        svrInfo;
        std::wostringstream strmIPAddr;

        strmIPAddr << static_cast<DWORD>( dlg.m_dwField0 ) << L"."
                   << static_cast<DWORD>( dlg.m_dwField1 ) << L"."
                   << static_cast<DWORD>( dlg.m_dwField2 ) << L"."
                   << static_cast<DWORD>( dlg.m_dwField3 );

        wcscpy( svrInfo.m_strIP, strmIPAddr.str().c_str() );
        wcscpy( svrInfo.m_strName, dlg.m_cstrServerName );
        svrInfo.m_dwPort    = dlg.m_dwServerPort;
        svrInfo.m_iVersion  = dlg.m_iVersion;

        m_vecSvrList.push_back( svrInfo );
    }
}

void CMainFrame::OnFileOpen( void )
{
	KServerListDlg dlg;

    dlg.DoModal();
}

void CMainFrame::OnDestroy( void )
{
	CMDIFrameWnd::OnDestroy();

    WriteServerListFile();
    WriteBlackListFile();
}

void CMainFrame::OnCloseNotifyMsgDlg( void )
{
    m_dlgNotifyMsg.DestroyWindow();
}

void CMainFrame::OnCloseBlackListDlg( void )
{
    m_dlgBlackList.DestroyWindow();
}

void CMainFrame::AddBlackList( const std::wstring& strName, const std::wstring& strReason )
{
    CTime               time = CTime::GetCurrentTime();
    std::wostringstream  strmListContent;
    strmListContent << L'[' << time.GetYear() << L'.' << time.GetMonth()
                    << L'.' << time.GetDay() << L']' << strName
                    << L'(' << strReason << L')';
    std::wstring strListContent = strmListContent.str();
    m_vecBlackList.push_back( strListContent );
    if ( TRUE == ::IsWindow( m_dlgBlackList.m_hWnd ) )
    {
        m_dlgBlackList.UpdateBlackList();
    }
}

void CMainFrame::ReadServerListFile( void )
{
    SServerInfo svrInfo;
    CFile fileSvrList( _T("GCServerList.dat"), CFile::modeCreate | CFile::modeNoTruncate );

    fileSvrList.SeekToBegin();

    m_vecSvrList.clear();
    while ( sizeof( SServerInfo ) == fileSvrList.Read( &svrInfo, sizeof( SServerInfo ) ) )
    {
        m_vecSvrList.push_back( svrInfo );
    }

    fileSvrList.Close();
}

void CMainFrame::ReadBlackListFile( void )
{
    CFile       fileBlackList( _T("GCBlackList.dat"), CFile::modeCreate | CFile::modeNoTruncate );
    std::wstring strListContent;
    wchar_t        cByte;

    fileBlackList.SeekToBegin();

    m_vecBlackList.clear();

    while ( 1 == fileBlackList.Read( &cByte, 1 ) )
    {
        if ( L' ' == cByte )
        {
            m_vecBlackList.push_back( strListContent );
            strListContent.erase();
        }
        else
        {
            strListContent += cByte;
        }
    }

    fileBlackList.Close();
}

void CMainFrame::WriteServerListFile( void )
{
    CFile::Remove( _T("GCServerList.dat") );

    CFile fileSvrList( _T("GCServerList.dat"), CFile::modeCreate | CFile::modeWrite );

    fileSvrList.SeekToBegin();

    for ( size_t i = 0; i < m_vecSvrList.size(); i++ )
    {
        fileSvrList.Write( &m_vecSvrList[i], sizeof( SServerInfo ) );
    }

    fileSvrList.Close();
}

void CMainFrame::WriteBlackListFile( void )
{
    CFile::Remove( _T("GCBlackList.dat") );

    CFile fileBlackList( _T("GCBlackList.dat"), CFile::modeCreate | CFile::modeWrite );

    for ( size_t i = 0; i < m_vecBlackList.size(); i++ )
    {
        fileBlackList.Write( m_vecBlackList[i].c_str(), m_vecBlackList[i].size() );
        fileBlackList.Write( " ", 1 );
    }

    fileBlackList.Close();
}