// GCMainMenuDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GCPatcher.h"
#include "GCMainMenuDlg.h"
#include "NBitmapButton.h"
#include "patch.h"
#include "GCGlobalValue.h"



// CGCMainMenuDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGCMainMenuDlg, CDialog)

CGCMainMenuDlg::CGCMainMenuDlg( CWnd* pParent /*=NULL*/, boost::shared_ptr<Komfile> imagemass )
	: CDialog(CGCMainMenuDlg::IDD, pParent)
	, m_pMassfile(imagemass)
{
	CreateLinkButtonList();
}


CGCMainMenuDlg::~CGCMainMenuDlg()
{
	ReleaseAll();
}

void CGCMainMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGCMainMenuDlg, CDialog)
	ON_WM_ERASEBKGND()
	//ON_BN_CLICKED(CBB_LINK_BUTTON_JOIN, OnClickJoin)
	//ON_BN_CLICKED(CBB_LINK_BUTTON_HOME, OnClickHome)
	//ON_BN_CLICKED(CBB_LINK_BUTTON_BEGINNER, OnClickBeginner)
	//ON_BN_CLICKED(CBB_LINK_BUTTON_DOWNLOAD, OnClickDownload)
	//ON_BN_CLICKED(CBB_LINK_BUTTON_FORUM, OnClickForum)
	//ON_BN_CLICKED(CBB_LINK_BUTTON_ONLINE_SERVICE, OnClickService)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

BOOL CGCMainMenuDlg::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}	

BOOL CGCMainMenuDlg::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->message == WM_KEYDOWN)
	{
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

void CGCMainMenuDlg::CreateLinkButtonList()
{
	if ( m_mapLinkButton.empty() == false ) return;

	m_mapLinkButton[CBB_LINK_BUTTON_JOIN] = new CNBitmapButton();
	m_mapLinkButton[CBB_LINK_BUTTON_JOIN]->LoadBitmaps(m_pMassfile.get(), "LBJoin");

	m_mapLinkButton[CBB_LINK_BUTTON_HOME] = new CNBitmapButton();
	m_mapLinkButton[CBB_LINK_BUTTON_HOME]->LoadBitmaps(m_pMassfile.get(), "LBHome");

	m_mapLinkButton[CBB_LINK_BUTTON_BEGINNER] = new CNBitmapButton();
	m_mapLinkButton[CBB_LINK_BUTTON_BEGINNER]->LoadBitmaps(m_pMassfile.get(), "LBBeginner");

	m_mapLinkButton[CBB_LINK_BUTTON_DOWNLOAD] = new CNBitmapButton();
	m_mapLinkButton[CBB_LINK_BUTTON_DOWNLOAD]->LoadBitmaps(m_pMassfile.get(), "LBDownLoad");

	m_mapLinkButton[CBB_LINK_BUTTON_FORUM] = new CNBitmapButton();
	m_mapLinkButton[CBB_LINK_BUTTON_FORUM]->LoadBitmaps(m_pMassfile.get(), "LBForum");

	m_mapLinkButton[CBB_LINK_BUTTON_ONLINE_SERVICE] = new CNBitmapButton();
	m_mapLinkButton[CBB_LINK_BUTTON_ONLINE_SERVICE]->LoadBitmaps(m_pMassfile.get(), "LBOnlineService");
}

void CGCMainMenuDlg::SetLinkButtonPosition()
{
	if ( m_mapLinkButton.empty() == true ) return;


	CRect rect;
	int iStartX = 0;
	int iStartY = 0;

	int iWidth = 133;
	rect.SetRect(iStartX,iStartY,iStartX+iWidth,iStartY+40); 
	m_mapLinkButton[CBB_LINK_BUTTON_JOIN]->Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rect,this,CBB_LINK_BUTTON_JOIN);
	m_mapLinkButton[CBB_LINK_BUTTON_JOIN]->EnableWindow(TRUE);
	m_mapLinkButton[CBB_LINK_BUTTON_JOIN]->ShowWindow(SW_SHOW);
	m_mapLinkButton[CBB_LINK_BUTTON_JOIN]->SetNoPress(TRUE);
	m_mapLinkButton[CBB_LINK_BUTTON_JOIN]->Invalidate();

	iStartX += iWidth;
	iWidth = 103;
	rect.SetRect(iStartX,iStartY,iStartX+iWidth,iStartY+40); 
	m_mapLinkButton[CBB_LINK_BUTTON_HOME]->Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rect,this,CBB_LINK_BUTTON_HOME);
	m_mapLinkButton[CBB_LINK_BUTTON_HOME]->EnableWindow(TRUE);
	m_mapLinkButton[CBB_LINK_BUTTON_HOME]->ShowWindow(SW_SHOW);
	m_mapLinkButton[CBB_LINK_BUTTON_HOME]->SetNoPress(TRUE);
	m_mapLinkButton[CBB_LINK_BUTTON_HOME]->Invalidate();

	iStartX += iWidth;
	iWidth = 103;
	rect.SetRect(iStartX,iStartY,iStartX+iWidth,iStartY+40); 
	m_mapLinkButton[CBB_LINK_BUTTON_BEGINNER]->Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rect,this,CBB_LINK_BUTTON_BEGINNER);
	m_mapLinkButton[CBB_LINK_BUTTON_BEGINNER]->EnableWindow(TRUE);
	m_mapLinkButton[CBB_LINK_BUTTON_BEGINNER]->ShowWindow(SW_SHOW);
	m_mapLinkButton[CBB_LINK_BUTTON_BEGINNER]->SetNoPress(TRUE);
	m_mapLinkButton[CBB_LINK_BUTTON_BEGINNER]->Invalidate();

	iStartX += iWidth;
	iWidth = 103;
	rect.SetRect(iStartX,iStartY,iStartX+iWidth,iStartY+40); 
	m_mapLinkButton[CBB_LINK_BUTTON_DOWNLOAD]->Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rect,this,CBB_LINK_BUTTON_DOWNLOAD);
	m_mapLinkButton[CBB_LINK_BUTTON_DOWNLOAD]->EnableWindow(TRUE);
	m_mapLinkButton[CBB_LINK_BUTTON_DOWNLOAD]->ShowWindow(SW_SHOW);
	m_mapLinkButton[CBB_LINK_BUTTON_DOWNLOAD]->SetNoPress(TRUE);
	m_mapLinkButton[CBB_LINK_BUTTON_DOWNLOAD]->Invalidate();

	iStartX += iWidth;
	iWidth = 103;
	rect.SetRect(iStartX,iStartY,iStartX+iWidth,iStartY+40); 
	m_mapLinkButton[CBB_LINK_BUTTON_FORUM]->Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rect,this,CBB_LINK_BUTTON_FORUM);
	m_mapLinkButton[CBB_LINK_BUTTON_FORUM]->EnableWindow(TRUE);
	m_mapLinkButton[CBB_LINK_BUTTON_FORUM]->ShowWindow(SW_SHOW);
	m_mapLinkButton[CBB_LINK_BUTTON_FORUM]->SetNoPress(TRUE);
	m_mapLinkButton[CBB_LINK_BUTTON_FORUM]->Invalidate();

	iStartX += iWidth;
	iWidth = 133;
	rect.SetRect(iStartX,iStartY,iStartX+iWidth,iStartY+40); 
	m_mapLinkButton[CBB_LINK_BUTTON_ONLINE_SERVICE]->Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rect,this,CBB_LINK_BUTTON_ONLINE_SERVICE);
	m_mapLinkButton[CBB_LINK_BUTTON_ONLINE_SERVICE]->EnableWindow(TRUE);
	m_mapLinkButton[CBB_LINK_BUTTON_ONLINE_SERVICE]->ShowWindow(SW_SHOW);
	m_mapLinkButton[CBB_LINK_BUTTON_ONLINE_SERVICE]->SetNoPress(TRUE);
	m_mapLinkButton[CBB_LINK_BUTTON_ONLINE_SERVICE]->Invalidate();
}

void CGCMainMenuDlg::OnClickJoin()
{
	KGCGlobalValue::PopUpWebPage(BUTTON_WEBLINK_JOIN);
}

void CGCMainMenuDlg::OnClickHome()
{
	KGCGlobalValue::PopUpWebPage(BUTTON_WEBLINK_HOME);
}

void CGCMainMenuDlg::OnClickBeginner()
{
	KGCGlobalValue::PopUpWebPage(BUTTON_WEBLINK_BEGINNER);
}

void CGCMainMenuDlg::OnClickDownload()
{
	KGCGlobalValue::PopUpWebPage(BUTTON_WEBLINK_DOWNLOAD);
}

void CGCMainMenuDlg::OnClickForum()
{
	KGCGlobalValue::PopUpWebPage(BUTTON_WEBLINK_FORUM);
}

void CGCMainMenuDlg::OnClickService()
{
	KGCGlobalValue::PopUpWebPage(BUTTON_WEBLINK_SERVICE);
}


void CGCMainMenuDlg::ReleaseAll()
{
	for ( std::map<int, CNBitmapButton*>::iterator mit = m_mapLinkButton.begin() ; mit != m_mapLinkButton.end() ; ++mit ) {
		SAFE_DELETE( mit->second);
	}

	m_mapLinkButton.clear();
}


// CGCMainMenuDlg 메시지 처리기입니다.

BOOL CGCMainMenuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowLong( m_hWnd, GWL_EXSTYLE, ::GetWindowLong( m_hWnd, GWL_EXSTYLE ) | WS_EX_LAYERED );
	SetLayeredWindowAttributes( RGB( 255, 0, 255), 0, LWA_COLORKEY/* | LWA_ALPHA */);

	SetLinkButtonPosition();
	Invalidate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CGCMainMenuDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL CGCMainMenuDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if ( nCode == BN_CLICKED ) {
		if ( CBB_LINK_BUTTON_JOIN == nID ) {
			OnClickJoin();
		}
		else if ( CBB_LINK_BUTTON_HOME == nID ) { 
			OnClickHome();
		}
		else if ( CBB_LINK_BUTTON_BEGINNER == nID ) { 
			OnClickBeginner();
		}
		else if ( CBB_LINK_BUTTON_DOWNLOAD == nID ) { 
			OnClickDownload();
		}
		else if ( CBB_LINK_BUTTON_FORUM == nID ) { 
			OnClickForum();
		}
		else if ( CBB_LINK_BUTTON_ONLINE_SERVICE == nID ) { 
			OnClickService();
		}
	}

	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

