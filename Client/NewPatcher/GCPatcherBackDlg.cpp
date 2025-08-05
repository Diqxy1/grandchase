// GCPatcherBackDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GCPatcher.h"
#include "GCPatcherBackDlg.h"
#include "./PngWindow/setPngWnd.h"
#include "GCPatcherDlg.h"
#include ".\gcglobalvalue.h"
#include "GCServerDlg.h"
#include "GCMainMenuDlg.h"
#include "GCSystemBtnDlg.h"
#include "GCStartBtnDlg.h"
#include "GCDownLoadDlg.h"
#include "GCPatcherStr.h"


// CGCPatcherBackDlg 대화 상자입니다.

CGCPatcherBackDlg::CGCPatcherBackDlg(boost::shared_ptr<Komfile> imagemass, 
									 boost::shared_ptr<KGCStringLoader> strLoader,
									 CWnd* pParent /*=NULL*/)
	: CDialog(CGCPatcherBackDlg::IDD, pParent)
	, m_pkChildServerWindow(NULL)
	, m_pkChildMainMenuWindow(NULL)
	, m_pkChildSystemBtnWindow(NULL)
	, m_pkChildStartBtnWindow(NULL)
	, m_pkChildDownloadWindow(NULL)
	, m_iState(0)
	, m_spMassFile(imagemass)
	, m_spStrLoader(strLoader)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	

}

CGCPatcherBackDlg::~CGCPatcherBackDlg()
{
	SAFE_DELETE(m_pkChildServerWindow);
	SAFE_DELETE(m_pkChildDownloadWindow);
	SAFE_DELETE(m_pkChildMainMenuWindow);
	SAFE_DELETE(m_pkChildSystemBtnWindow);
	SAFE_DELETE(m_pkChildStartBtnWindow);
}

void CGCPatcherBackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGCPatcherBackDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_MOVING()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()



BOOL CGCPatcherBackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	::SetWindowText( m_hWnd, GRANDCHASE_TITLE_NAME );	

	WindowAlphaBlendFromPNG("Background.png", m_hWnd);


	return TRUE;
}

//void CGCPatcherBackDlg::CreateDownLoadDlg(bool mannual,boost::shared_ptr<KGCStringLoader> strloader,
//										  boost::shared_ptr<Komfile> imagemass,
//										  int nMaxLang /*=0*/,
//										  CWnd* pParent /*=NULL*/)
//{
//
//	m_pkDownLoadDlg = new CGCPatcherDlg(mannual,strloader,imagemass,nMaxLang,pParent);
//	m_pkDownLoadDlg->Create(IDD_GCPATCHER_DIALOG);
//	m_pkDownLoadDlg->ShowWindow(SW_SHOW);
//	m_pkDownLoadDlg->ShowWeb(KGCGlobalValue::GetBGWeb());
//
//
//}

void CGCPatcherBackDlg::CreateServerDlg()
{

	m_pkChildServerWindow = new CGCServerDlg(this, m_spMassFile);

	m_pkChildServerWindow->Create(CGCServerDlg::IDD);
	//m_pkChildServerWindow->ShowWindow(SW_SHOW);
	//m_pkChildServerWindow->EnableWindow(TRUE);

	CRect rect;
	GetWindowRect(rect);

	rect.left += 62;
	rect.top += 143;
	rect.right = rect.left +787;
	rect.bottom = rect.top +361;
	m_pkChildServerWindow->MoveWindow(rect);

 

}

void CGCPatcherBackDlg::CreateMainMenuDlg()
{

	m_pkChildMainMenuWindow = new CGCMainMenuDlg(this, m_spMassFile);

	m_pkChildMainMenuWindow->Create(CGCMainMenuDlg::IDD);
	m_pkChildMainMenuWindow->ShowWindow(SW_SHOW);
	m_pkChildMainMenuWindow->EnableWindow(TRUE);

	CRect rect;
	GetWindowRect(rect);

	rect.left += 116;
	rect.top += 84;
	rect.right = rect.left + 678;
	rect.bottom = rect.top + 40;
	m_pkChildMainMenuWindow->MoveWindow(rect);

}

void CGCPatcherBackDlg::CreateSystemBtnDlg()
{

	m_pkChildSystemBtnWindow = new CGCSystemBtnDlg(this, m_spMassFile);

	m_pkChildSystemBtnWindow->Create(CGCSystemBtnDlg::IDD);
	m_pkChildSystemBtnWindow->ShowWindow(SW_SHOW);
	m_pkChildSystemBtnWindow->EnableWindow(TRUE);

	CRect rect;
	GetWindowRect(rect);

	rect.left += 825;
	rect.top += 62;
	rect.right = rect.left +44;
	rect.bottom = rect.top +21;
	m_pkChildSystemBtnWindow->MoveWindow(rect);

}

void CGCPatcherBackDlg::CreateStartBtnDlg()
{

	m_pkChildStartBtnWindow = new CGCStartBtnDlg(this, m_spMassFile);

	m_pkChildStartBtnWindow->Create(CGCStartBtnDlg::IDD);
	m_pkChildStartBtnWindow->ShowWindow(SW_SHOW);
	m_pkChildStartBtnWindow->EnableWindow(TRUE);

	CRect rect;
	GetWindowRect(rect);

	rect.left += 405;
	rect.top += 487;
	rect.right = rect.left +98;
	rect.bottom = rect.top +98;
	m_pkChildStartBtnWindow->MoveWindow(rect);
}

void CGCPatcherBackDlg::CreateDownLoadDlg()
{

	m_pkChildDownloadWindow = new CGCDownLoadDlg(this, m_spMassFile, m_spStrLoader);

	m_pkChildDownloadWindow->Create(CGCDownLoadDlg::IDD);
	//m_pkChildDownloadWindow->ShowWindow(SW_SHOW);
	//m_pkChildDownloadWindow->EnableWindow(TRUE);

	CRect rect;
	GetWindowRect(rect);

	rect.left += 87;
	rect.top += 143;
	rect.right = rect.left +735;
	rect.bottom = rect.top +361;
	m_pkChildDownloadWindow->MoveWindow(rect);

}

void CGCPatcherBackDlg::OnServerSelect()
{
	CRect rect;
	GetWindowRect(rect);

	rect.left += 62;
	rect.top += 143;
	rect.right = rect.left +787;
	rect.bottom = rect.top +362;
	m_pkChildServerWindow->MoveWindow(rect);


	if ( m_pkChildDownloadWindow ) {
		m_pkChildDownloadWindow->ShowWindow(SW_HIDE);
		m_pkChildDownloadWindow->EnableWindow(FALSE);
	}

	if ( m_pkChildServerWindow ) {
		m_pkChildServerWindow->EnableWindow(TRUE);
		m_pkChildServerWindow->ShowWindow(SW_SHOW);
	}

	m_pkChildStartBtnWindow->OnServerSelectBtn();
	m_iState = 1;
	Invalidate();
}

void CGCPatcherBackDlg::OffServerSelect()
{
	if ( m_pkChildStartBtnWindow ) {
		m_pkChildStartBtnWindow->ShowWindow(SW_HIDE);
	}
	Invalidate();

}

void CGCPatcherBackDlg::OnDownload()
{
	CRect rect;
	GetWindowRect(rect);

	rect.left += 87;
	rect.top += 143;
	rect.right = rect.left +735;
	rect.bottom = rect.top +361;
	m_pkChildDownloadWindow->MoveWindow(rect);


	if ( m_pkChildServerWindow ) {
		m_pkChildServerWindow->ShowWindow(SW_HIDE);
		m_pkChildServerWindow->EnableWindow(FALSE);
	}

	if ( m_pkChildDownloadWindow ) {
		m_pkChildDownloadWindow->EnableWindow(TRUE);
		m_pkChildDownloadWindow->ShowWindow(SW_SHOW);
	}


	m_pkChildStartBtnWindow->OnStartGameBtn();
	m_pkChildStartBtnWindow->ShowWindow(SW_SHOW);
	m_pkChildDownloadWindow->OnStartDownload();
	m_iState = 2;
	m_pkChildSystemBtnWindow->Invalidate();
	Invalidate();


}



// CGCPatcherBackDlg 메시지 처리기입니다.

void CGCPatcherBackDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
}

BOOL CGCPatcherBackDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return FALSE;
}

HCURSOR CGCPatcherBackDlg::OnQueryDragIcon()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return (HCURSOR) m_hIcon;
}

//int CGCPatcherBackDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CDialog::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
//
//	return 0;
//}

BOOL CGCPatcherBackDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) {
		if(pMsg->wParam == VK_RETURN)
			return TRUE;
		else if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	else if ( pMsg->message == WM_SYSKEYDOWN ) {
		if ( pMsg->wParam == VK_F4 ) {
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CGCPatcherBackDlg::OnNcHitTest(CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return HTCAPTION;//CDialog::OnNcHitTest(point);
}

void CGCPatcherBackDlg::SetChildWindow( CDialog *pWnd )
{
	//CRect rect;
	//GetWindowRect(rect);
	//if ( m_pkChildServerWindow && m_iState == 1 ) {
	//	m_pkChildServerWindow->SetWindowPos(&wndTop, rect.left + 150, rect.right + 80, 0, 0, SWP_NOSIZE);
	//	m_pkChildServerWindow->Invalidate();
	//}

}

void CGCPatcherBackDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialog::OnMoving(fwSide, pRect);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CGCPatcherBackDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanged(lpwndpos);

	if ( m_pkChildServerWindow && m_iState == 1 ) {
		m_pkChildServerWindow->SetWindowPos(&wndTop, lpwndpos->x +62, lpwndpos->y + 143, 0, 0, SWP_NOSIZE);
		//m_pkChildServerWindow->Invalidate();
	}

	if ( m_pkChildMainMenuWindow ) {
		m_pkChildMainMenuWindow->SetWindowPos(&wndTop, lpwndpos->x +116, lpwndpos->y + 84, 0, 0, SWP_NOSIZE);
		//m_pkChildMainMenuWindow->Invalidate();
	}

	if ( m_pkChildSystemBtnWindow ) {
		m_pkChildSystemBtnWindow->SetWindowPos(&wndTop, lpwndpos->x +825, lpwndpos->y + 62, 0, 0, SWP_NOSIZE);
		//m_pkChildSystemBtnWindow->Invalidate();
	}

	if ( m_pkChildStartBtnWindow ) {
		m_pkChildStartBtnWindow->SetWindowPos(&wndTop, lpwndpos->x +405, lpwndpos->y + 487, 0, 0, SWP_NOSIZE);
		//m_pkChildSystemBtnWindow->Invalidate();
	}

	if ( m_pkChildDownloadWindow ) {
		m_pkChildDownloadWindow->SetWindowPos(&wndTop, lpwndpos->x +87, lpwndpos->y + 143, 0, 0, SWP_NOSIZE);

	}

	Invalidate();


	

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CGCPatcherBackDlg::ModalDestroyWindow()
{
	if ( m_pkChildServerWindow != NULL )
		m_pkChildServerWindow->DestroyWindow();
	
	m_pkChildMainMenuWindow->DestroyWindow();
	m_pkChildSystemBtnWindow->DestroyWindow();
	m_pkChildStartBtnWindow->DestroyWindow();
	return DestroyWindow();
}

void CGCPatcherBackDlg::CloseServerScene()
{
	if ( m_pkChildServerWindow != NULL ){
		m_pkChildServerWindow->DestroyWindow();
		SAFE_DELETE(m_pkChildServerWindow);
	}
}

void CGCPatcherBackDlg::CreateAllDlg()
{
	CreateMainMenuDlg();
	CreateSystemBtnDlg();
	CreateServerDlg();
	CreateDownLoadDlg();
	CreateStartBtnDlg();

    if ( m_pkChildServerWindow && m_pkChildStartBtnWindow )
    {
        m_pkChildServerWindow->SetHWNDStartBtn( m_pkChildStartBtnWindow->GetSafeHwnd() );
    }
}

NETSTATRT_CALLBACK CGCPatcherBackDlg::GetStartFunc()
{
	return boost::bind(&CGCDownLoadDlg::OnStart,m_pkChildDownloadWindow);
}

NETPROGRESS_CALLBACK CGCPatcherBackDlg::GetProgressFunc()
{
	return boost::bind(&CGCDownLoadDlg::OnProgress,m_pkChildDownloadWindow,_1);
}

NETPROGRESS_CALLBACK CGCPatcherBackDlg::GetProgress2Func()
{
	return boost::bind(&CGCDownLoadDlg::OnProgress2,m_pkChildDownloadWindow,_1);
}

NETERROR_CALLBACK CGCPatcherBackDlg::GetErrorFunc()
{
	return boost::bind(&CGCDownLoadDlg::OnError,m_pkChildDownloadWindow,_1,_2);
}

COMPLETE_CALLBACK CGCPatcherBackDlg::GetCompleteFunc()
{
	return boost::bind(&CGCPatcherBackDlg::OnComplete,this,_1,_2);
}

void CGCPatcherBackDlg::OnComplete( bool success, bool bMsgBox )
{
	m_pkChildStartBtnWindow->OnComplete(success,bMsgBox);
	m_pkChildDownloadWindow->OnComplete(success,bMsgBox);
}


IS_START_CALLBACK CGCPatcherBackDlg::GetIsStartFunc()
{
	return boost::bind(&CGCDownLoadDlg::IsStart,m_pkChildDownloadWindow);
}

HWND CGCPatcherBackDlg::GetStartBtnHandle()
{
    return m_pkChildStartBtnWindow->GetSafeHwnd();
}
