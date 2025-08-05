// WCenterServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "WCenterServer.h"
#include "WCenterServerDlg.h"

#include <KncUtil.h>
//---------------------------------------------------------------------
#include <ImportKncLua.h>
#include <ImportKncSerializer.h>
#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>

//#include "ImportNMLink.h"           // 넥슨저팬 인증 라이브러리
#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "CPSSOLib" )  // 넷마블 인증 라이브러리

#ifdef _DEBUG
#pragma comment( lib, "log4cxxD" ) // log4cxx
#else
#pragma comment( lib, "log4cxx" ) // log4cxx
#endif

#include "..\CenterServer\CenterServer.h"
#include ".\wcenterserverdlg.h"
#include ".\ui\HookStdio.h"
#include <boost/algorithm/string.hpp> //trim

#include "DuplicateProcessChecker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define INPUT_HISTORY_SIZE      (10)
#define WM_PATCH ( WM_USER + 1 )


using namespace std;


BOOL		 g_bServerRun = TRUE;
KBaseServer* g_pkServer = NULL;
CHookStdio	 HookStdio(STD_OUTPUT_HANDLE);
CHookStdio   HookError(STD_ERROR_HANDLE);


// WCenterServerDlg 대화 상자

UINT Thread_STDIO( LPVOID pParam )
{
    char Buf[256];
    DWORD dwRead;
    WCenterServerDlg* pDlg = ((WCenterServerDlg*)pParam);
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
        //if( dwRead = HookStdio.Read(Buf,sizeof(Buf)-1) )
        //{
        //	Buf[dwRead] = 0;
        //	strBuf += Buf;
        //	int iFind;
        //	while( ( iFind = strBuf.find('\n') ) != -1 )
        //	{
        //		iFind++;
        //		std::string out = strBuf.substr(0, iFind);
        //		//pDlg->m_ListBox.AddMsg((char*)out.c_str());
        //              pDlg->m_editLogger.AddText( Buf, true );
        //		strBuf.erase(0,iFind);
        //	}
        //}
        Sleep(5);

        if( !g_pkServer ) // 서버가 종료되면 스레드도 종료.
            AfxEndThread( 0 );
    }
}


WCenterServerDlg::WCenterServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(WCenterServerDlg::IDD, pParent),
m_strInput(_T("")),
m_editStrBuf( &m_editLogger ),
m_editStrBufW( &m_editLogger ),
m_editStrBufError( &m_editLogger ),
m_editStrBufWError( &m_editLogger )
, m_strServerName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
    InitHistory();
}

void WCenterServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_INPUT, m_strInput);
    DDX_Control(pDX, IDC_INPUT, m_editInput);
    DDX_Control(pDX, IDC_OUTPUT_EDIT, m_editLog);
    DDX_Control(pDX, IDOK, m_btOK);
    DDX_Control(pDX, IDC_NAME, m_editServerName);
    DDX_Text(pDX, IDC_NAME, m_strServerName);
}

BEGIN_MESSAGE_MAP(WCenterServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
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
    /*{
        WM_PATCH, 0, 0, 0, AfxSig_vw,
            (AFX_PMSG)(AFX_PMSGW) (static_cast< void (AFX_MSG_CALL CWnd::*)() > (OnPatch))
    },*/
    ON_MESSAGE( WM_PATCH, OnPatch )

END_MESSAGE_MAP()


// WCenterServerDlg 메시지 처리기

BOOL WCenterServerDlg::OnInitDialog()
{
    CDuplicateProcessChecker kDuplicateProcessChecker;

	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
    m_pOldBuf       = std::cout.rdbuf( &m_editStrBuf ); // 070727. woosh. 어떤 용도인지 모르겠음
    m_pOldBufW      = std::wcout.rdbuf( &m_editStrBufW );
    m_pOldBufError  = std::cerr.rdbuf( &m_editStrBufError );
    m_pOldBufWError = std::wcerr.rdbuf( &m_editStrBufWError );
	
    m_editLogger.SetEditCtrl( m_editLog.m_hWnd ); 

    SetTimer(0,1000,NULL);


    // 윈도우 타이틀 변경
    /*{
    WCHAR acBuf[256] = { 0 };
    GetWindowText( acBuf, 256 );

    std::wstring strText = acBuf;
    strText += L"_Loading";
    SetWindowText( strText.c_str() );
    }*/
    m_editLog.SetBackColor( RGB(0,0,0) );
    m_editLog.SetTextColor( RGB(255,255,255) );

    ReSizeWnd();
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void WCenterServerDlg::OnClose(UINT nForceClose) 
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
void WCenterServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR WCenterServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void WCenterServerDlg::OnBnClickedCancel()
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
void WCenterServerDlg::OnBnClickedOk()
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

BOOL WCenterServerDlg::DestroyWindow()
{	
    ShutdownServer();

    cout.rdbuf( m_pOldBuf );
    wcout.rdbuf( m_pOldBufW );
    std::cerr.rdbuf( m_pOldBufError );
    std::wcerr.rdbuf( m_pOldBufWError );

    g_bServerRun = FALSE;

    return CDialog::DestroyWindow();
}

void WCenterServerDlg::StartupServer()
{
    //------------------------------------------------------------------
    
    g_pkServer = SiKCenterServer();

    if( !g_pkServer->Init( L"config_center" ) ) {
        this->PostMessage( WM_CLOSE,1,NULL );
        return;
    }

    g_pkServer->Run( false ); // true -> lua_interpreter

    m_pThreadStdIO = AfxBeginThread( Thread_STDIO, this );
    //------------------------------------------------------------------

    //std::wstring strServerName; // or Port
    std::wstring strServerPort;

    g_pkServer->GetServerPort( strServerPort );

    strServerPort = L"Port : " + strServerPort;

    m_strServerName = strServerPort.c_str();
    UpdateData(FALSE);
}

void WCenterServerDlg::ShutdownServer()
{
    if( g_pkServer == NULL )
        return;

    g_pkServer->ShutDown();
    KCenterServer::ReleaseInstance();
    g_pkServer = NULL;
}

void WCenterServerDlg::OnTimer(UINT nIDEvent)
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

//void CWGameServerDlg::OnLbnSelchangeOutput()
//{
//	int iSelect = m_ListBox.GetCurSel();
//
//	if(  iSelect < 0 && iSelect >= m_ListBox.GetCount() )
//		return;
//
//	//m_ListBox.GetText(iSelect, m_strOutput);
//	UpdateData(FALSE);
//}

BOOL WCenterServerDlg::PreTranslateMessage(MSG* pMsg)
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

void WCenterServerDlg::InitHistory()
{
    m_nTail = m_nStart = 0;
    m_vecInputHistory.resize( INPUT_HISTORY_SIZE );
}

void WCenterServerDlg::AddHistory( CString& strInput_ )
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

int WCenterServerDlg::GetLastHistory()
{
    int nLast = m_nTail -1;

    if( nLast == -1 )
        nLast = (int)(m_vecInputHistory.size() -1);

    return nLast;
}

int WCenterServerDlg::GetNextCursor()
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

int WCenterServerDlg::GetBeforeCursor()
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

void WCenterServerDlg::ResetCursor()
{
    m_iInputCursor = -1;
}

int WCenterServerDlg::GetFreeSize()
{
    if( m_vecInputHistory.empty() ) return 0;
    return (m_nStart - m_nTail + m_vecInputHistory.size() - 1) % m_vecInputHistory.size(); 
}
void WCenterServerDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.

    ReSizeWnd();
}
void WCenterServerDlg::ReSizeWnd( void )
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

    if( m_editServerName.GetSafeHwnd() )
    {
        m_editServerName.MoveWindow( DG_BOARDER_SIZE, DG_BOARDER_SIZE, DG_SERVER_NAME_WIDTH,DG_SERVER_NAME_HEIGHT, true );
    }
}

//void WCenterServerDlg::OnPatch() // 기존에 쓰던 방식
//{
//    SiKCenterServer()->ReLoadScript();
//}

LRESULT WCenterServerDlg::OnPatch( WPARAM wParam, LPARAM lParam ) // 파라미터를 사용할 수 있을지 테스트 하는 중 
{
    SiKCenterServer()->ReLoadScript();

    return 0;
}
