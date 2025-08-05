// WMsgServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WMsgServer.h"
#include "WMsgServerDlg.h"

#include <KncUtil.h>
#include <ImportKncLua.h>
#include <ImportKncSerializer.h>
#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>

//#include ".\wmsgserverdlg.h"
#include "./ui/HookStdio.h"
#include <boost/algorithm/string.hpp> //trim

#include "MsgServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#pragma comment( lib, "log4cxxD" ) // log4cxx
#else
#pragma comment( lib, "log4cxx" ) // log4cxx
#endif

#define INPUT_HISTORY_SIZE      (10)
// CWMsgServerDlg dialog
using namespace std;

BOOL		 g_bServerRun = TRUE;
KBaseServer* g_pkServer = NULL;
CHookStdio	 HookStdio(STD_OUTPUT_HANDLE);
CHookStdio   HookError(STD_ERROR_HANDLE);

UINT Thread_STDIO( LPVOID pParam )
{
    char Buf[256];
    DWORD dwRead;
    CWMsgServerDlg* pDlg = ((CWMsgServerDlg*)pParam);
    std::string strBuf;

    while(true)
    {
        if( dwRead = HookStdio.Read(Buf,sizeof(Buf)-2) )
        {           
            Buf[dwRead] = 0;
            Buf[dwRead+1] = 0;
            pDlg->m_editLogger.AddText( Buf, true );
        }
        if( dwRead = HookError.Read(Buf,sizeof(Buf)-2) )
        {           
            Buf[dwRead] = 0;
            Buf[dwRead+1] = 0;
            pDlg->m_editLogger.AddText( Buf, true );
        }

        Sleep(5);

        if( !g_pkServer ) // 서버가 종료되면 스레드도 종료.
            AfxEndThread( 0 );
    }
}


CWMsgServerDlg::CWMsgServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWMsgServerDlg::IDD, pParent),
m_strInput(_T("")),
m_editStrBuf( &m_editLogger ),
m_editStrBufW( &m_editLogger ),
m_editStrBufError( &m_editLogger ),
m_editStrBufWError( &m_editLogger ),
m_iInputCursor(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    InitHistory();
}

void CWMsgServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_INPUT, m_strInput);
    DDX_Control(pDX, IDC_INPUT, m_editInput);
    DDX_Control(pDX, IDC_OUTPUT_EDIT, m_editLog);
    DDX_Control(pDX, IDOK, m_btOK);
}

BEGIN_MESSAGE_MAP(CWMsgServerDlg, CDialog)
	ON_WM_PAINT()
    {
        WM_CLOSE, 0, 0, 0, AfxSig_vw,
            (AFX_PMSG)(AFX_PMSGW) (static_cast< void (AFX_MSG_CALL CWnd::*)(UINT_PTR) > (OnClose))
    },
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_TIMER()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CWMsgServerDlg message handlers

BOOL CWMsgServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    m_pOldBuf       = std::cout.rdbuf( &m_editStrBuf ); // 070727. woosh. 어떤 용도인지 모르겠음
    m_pOldBufW      = std::wcout.rdbuf( &m_editStrBufW );
    m_pOldBufError  = std::cerr.rdbuf( &m_editStrBufError );
    m_pOldBufWError = std::wcerr.rdbuf( &m_editStrBufWError );

    // Set the Edit-control as logger destination
    m_editLogger.SetEditCtrl( m_editLog.m_hWnd ); 

    SetTimer(0,1000,NULL);

    m_editLog.SetBackColor( RGB(0,0,0) );
    m_editLog.SetTextColor( RGB(255,255,255) );
	
    ReSizeWnd();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWMsgServerDlg::OnClose(UINT nForceClose) 
{
	// 강제 종료
	if (nForceClose != NULL)
	{
		CDialog::OnClose();
		return;
	}

	int iRet = MessageBox( L"확실히 종료하시겠습니까 ?", L"확인", MB_OKCANCEL );
	if (iRet == IDOK)
		CDialog::OnClose();
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWMsgServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWMsgServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWMsgServerDlg::OnBnClickedOk()
{
    UpdateData();

    if( m_strInput.GetLength() != 0 )
    {
        std::string strLua = KncUtil::toNarrowString( (LPCTSTR)m_strInput );
        std::cout << strLua << std::endl;
        boost::trim( strLua );
        lua_dostring( g_pkServer->GetLuaState(), strLua.c_str() );

        std::cout << "> "; // 프롬프트..ㅋㅋㅋ

        AddHistory( m_strInput );
        ResetCursor();
    }

    m_strInput = "";
    UpdateData(FALSE);
    m_editInput.SetFocus();
}

void CWMsgServerDlg::OnBnClickedCancel()
{
    //if( g_pkServer )
    //{
    //    if( AfxMessageBox(L"■Shutdown Server■",MB_YESNO) == IDYES )
    //    {
    //        ShutdownServer();
    //        if( AfxMessageBox(L"▶Restart Server?▶",MB_YESNO) == IDYES )
    //        {
    //            //m_ListBox.ResetContent();
    //            StartupServer();
    //            return;
    //        }
    //    }
    //    else
    //        return;
    //}
    CDialog::OnCancel();
}

BOOL CWMsgServerDlg::DestroyWindow()
{	
    ShutdownServer();

    cout.rdbuf( m_pOldBuf );
    wcout.rdbuf( m_pOldBufW );
    std::cerr.rdbuf( m_pOldBufError );
    std::wcerr.rdbuf( m_pOldBufWError );

    g_bServerRun = FALSE;

    return CDialog::DestroyWindow();
}

void CWMsgServerDlg::StartupServer()
{
    //------------------------------------------------------------------
    g_pkServer = SiKMsgServer();

    if( !g_pkServer->Init( L"config_msg" ) ) {
        this->PostMessage( WM_CLOSE,1,NULL );
        return;
    }
    g_pkServer->Run( false ); // true -> lua_interpreter

    m_pThreadStdIO = AfxBeginThread( Thread_STDIO, this );

    //g_pkServer->Run( false ); // true -> lua_interpreter
    //------------------------------------------------------------------
}

void CWMsgServerDlg::ShutdownServer()
{
    if( g_pkServer == NULL )
        return;

    g_pkServer->ShutDown();
    KMsgServer::ReleaseInstance();
    g_pkServer = NULL;
}

void CWMsgServerDlg::OnTimer(UINT nIDEvent)
{
    if( nIDEvent == 0 )
    {		
        StartupServer();
        KillTimer(nIDEvent);
    }
    else
    {
        //m_ListBox.Display();
    }

    CDialog::OnTimer(nIDEvent);
}

BOOL CWMsgServerDlg::PreTranslateMessage(MSG* pMsg)
{
    if( pMsg->message == WM_KEYDOWN )
    {
        if ( pMsg->wParam == VK_ESCAPE )
        {
            m_strInput = "";

            UpdateData(FALSE);
            return TRUE;
        }
        else if( pMsg->wParam == VK_UP )
        {
            if( m_vecInputHistory.size() == 0 || IsHistoryEmpty() ) 
                return CDialog::PreTranslateMessage(pMsg);

            m_iInputCursor = GetNextCursor();

            m_strInput = m_vecInputHistory[m_iInputCursor];
            UpdateData(FALSE);
            m_editInput.SetSel(-1,-1);
            return TRUE;
        }
        else if( pMsg->wParam == VK_DOWN )
        {
            if( m_vecInputHistory.size() == 0 || IsHistoryEmpty() )
                return CDialog::PreTranslateMessage(pMsg);

            m_iInputCursor = GetBeforeCursor();
            m_strInput = m_vecInputHistory[m_iInputCursor];
            UpdateData(FALSE);
            m_editInput.SetSel(-1,-1);
            return TRUE;
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
}

void CWMsgServerDlg::InitHistory()
{
    m_nTail = m_nStart = 0;
    m_vecInputHistory.resize( INPUT_HISTORY_SIZE );
}

void CWMsgServerDlg::AddHistory( CString& strInput_ )
{
    m_vecInputHistory[m_nTail] = strInput_;

    if( IsHistoryEmpty() )
    {
        ++m_nTail;
        return;
    }

    ++m_nTail;
    if( m_nTail == m_vecInputHistory.size() )
        m_nTail = 0;

    if( GetFreeSize() == 0 ) // 남은 공간이 있다면..
    {
        ++m_nStart;
        if( m_nStart == m_vecInputHistory.size() )
            m_nStart = 0;
    }
}

int CWMsgServerDlg::GetLastHistory()
{
    int nLast = m_nTail -1;

    if( nLast == -1 )
        nLast = (int)(m_vecInputHistory.size() -1);

    return nLast;
}

int CWMsgServerDlg::GetNextCursor()
{
    if( IsHistoryEmpty() )
        return -1;

    if( m_iInputCursor == -1 )
    {
        m_iInputCursor = m_nStart;
        goto end_proc;
    }

    ++m_iInputCursor;

    if( m_nStart > m_nTail )
    {
        //if( m_iInputCursor >= m_nTail && m_iInputCursor < m_nStart )
        //    m_iInputCursor = m_nStart;
        if( (int)m_vecInputHistory.size() == m_iInputCursor )
            m_iInputCursor = 0;
    }
    else
    {
        if( (int)m_vecInputHistory.size() == m_iInputCursor )
            m_iInputCursor = 0;

        if( m_iInputCursor < m_nStart || m_iInputCursor >= m_nTail )
            m_iInputCursor = 0;
    }

end_proc:
    return m_iInputCursor;

}

int CWMsgServerDlg::GetBeforeCursor()
{
    if( IsHistoryEmpty() )
        return -1;

    if( m_iInputCursor == -1 )
    {
        m_iInputCursor = GetLastHistory();
        goto end_proc;
    }

    --m_iInputCursor;

    if( m_nStart > m_nTail )
    {
        //if( m_iInputCursor >= m_nTail && m_iInputCursor < m_nStart )
        //    m_iInputCursor = m_nTail -1;

        if( m_iInputCursor < 0)
            m_iInputCursor = (int)(m_vecInputHistory.size() - 1);
    }
    else
    {
        //if( m_iInputCursor < m_nStart || m_iInputCursor >= m_nTail )
        //    m_iInputCursor = m_nTail -1 ;
        if( m_iInputCursor < 0)
            m_iInputCursor = m_nTail -1;
    }


end_proc:
    return m_iInputCursor;
}

void CWMsgServerDlg::ResetCursor()
{
    m_iInputCursor = -1;
}

int CWMsgServerDlg::GetFreeSize()
{
    if( m_vecInputHistory.empty() ) return 0;
    return (m_nStart - m_nTail + m_vecInputHistory.size() - 1) % m_vecInputHistory.size(); 
}
void CWMsgServerDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.

    ReSizeWnd();
}

void CWMsgServerDlg::ReSizeWnd( void )
{
    CRect rtClient;
    GetClientRect( rtClient );

    if( m_editLog.GetSafeHwnd() )
    {
        m_editLog.MoveWindow( DG_BOARDER_SIZE, DG_BOARDER_SIZE + DG_SERVER_NAME_HEIGHT
            , rtClient.Width() - DG_BOARDER_SIZE*2, rtClient.Height() - DG_BOARDER_SIZE*3 - DG_OK_BUTTON_SIZE - DG_SERVER_NAME_HEIGHT, true );
    }

    if( m_editInput.GetSafeHwnd() )
    {
        m_editInput.MoveWindow( DG_BOARDER_SIZE, rtClient.bottom - (DG_BOARDER_SIZE + DG_OK_BUTTON_SIZE  )
            , rtClient.right - (DG_BOARDER_SIZE + DG_OK_BUTTON_SIZE) - DG_BOARDER_SIZE*2, DG_OK_BUTTON_SIZE, true );
    }

    if( m_btOK.GetSafeHwnd() )
    {
        m_btOK.MoveWindow( rtClient.right - (DG_BOARDER_SIZE + DG_OK_BUTTON_SIZE), rtClient.bottom - (DG_BOARDER_SIZE + DG_OK_BUTTON_SIZE)
            , DG_OK_BUTTON_SIZE, DG_OK_BUTTON_SIZE, true );
    }

    //if( m_editServerName.GetSafeHwnd() )
    //{
    //    m_editServerName.MoveWindow( DG_BOARDER_SIZE, DG_BOARDER_SIZE, DG_SERVER_NAME_WIDTH,DG_SERVER_NAME_HEIGHT, true );
    //}
}
