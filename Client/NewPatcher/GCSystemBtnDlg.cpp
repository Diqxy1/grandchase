// GCSystemBtnDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GCPatcher.h"
#include "GCSystemBtnDlg.h"
#include "NBitmapButton.h"
#include "patch.h"
#include "GCGlobalValue.h"

// CGCSystemBtnDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGCSystemBtnDlg, CDialog)

CGCSystemBtnDlg::CGCSystemBtnDlg( CWnd* pParent /*=NULL*/, boost::shared_ptr<Komfile> imagemass )
	: CDialog(CGCSystemBtnDlg::IDD, pParent)
	, m_pMassfile(imagemass)
{
	CreateButtonList();
}

CGCSystemBtnDlg::~CGCSystemBtnDlg()
{
	ReleaseAll();
}

void CGCSystemBtnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGCSystemBtnDlg, CDialog)
	ON_WM_ERASEBKGND()
	//ON_BN_CLICKED(CBB_SYSTEM_BUTTON_MINIMUM, OnClickMinimum)
	//ON_BN_CLICKED(CBB_SYSTEM_BUTTON_CLOSE, OnClickClose)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

BOOL CGCSystemBtnDlg::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}	

BOOL CGCSystemBtnDlg::PreTranslateMessage( MSG* pMsg )
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

void CGCSystemBtnDlg::CreateButtonList()
{
	if ( m_mapSystemButton.empty() == false ) return;

	m_mapSystemButton[CBB_SYSTEM_BUTTON_MINIMUM] = new CNBitmapButton();
	m_mapSystemButton[CBB_SYSTEM_BUTTON_MINIMUM]->LoadBitmaps(m_pMassfile.get(), "minimum");

	m_mapSystemButton[CBB_SYSTEM_BUTTON_CLOSE] = new CNBitmapButton();
	m_mapSystemButton[CBB_SYSTEM_BUTTON_CLOSE]->LoadBitmaps(m_pMassfile.get(), "close");

}

void CGCSystemBtnDlg::SetLinkButtonPosition()
{
	if ( m_mapSystemButton.empty() == true ) return;


	CRect rect;
	int iStartX = 0;
	int iStartY = 0;
	int iGap = 23;

	rect.SetRect(iStartX,iStartY,iStartX+21,iStartY+21); 
	m_mapSystemButton[CBB_SYSTEM_BUTTON_MINIMUM]->Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rect,this,CBB_SYSTEM_BUTTON_MINIMUM);
	m_mapSystemButton[CBB_SYSTEM_BUTTON_MINIMUM]->EnableWindow(TRUE);
	m_mapSystemButton[CBB_SYSTEM_BUTTON_MINIMUM]->ShowWindow(SW_SHOW);
	m_mapSystemButton[CBB_SYSTEM_BUTTON_MINIMUM]->SetNoPress(TRUE);
	m_mapSystemButton[CBB_SYSTEM_BUTTON_MINIMUM]->Invalidate();

	iStartX += iGap;
	rect.SetRect(iStartX,iStartY,iStartX+21,iStartY+21); 
	m_mapSystemButton[CBB_SYSTEM_BUTTON_CLOSE]->Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rect,this,CBB_SYSTEM_BUTTON_CLOSE);
	m_mapSystemButton[CBB_SYSTEM_BUTTON_CLOSE]->EnableWindow(TRUE);
	m_mapSystemButton[CBB_SYSTEM_BUTTON_CLOSE]->ShowWindow(SW_SHOW);
	m_mapSystemButton[CBB_SYSTEM_BUTTON_CLOSE]->SetNoPress(TRUE);
	m_mapSystemButton[CBB_SYSTEM_BUTTON_CLOSE]->Invalidate();
}


void CGCSystemBtnDlg::ReleaseAll()
{
	for ( std::map<int, CNBitmapButton*>::iterator mit = m_mapSystemButton.begin() ; mit != m_mapSystemButton.end() ; ++mit ) {
		SAFE_DELETE( mit->second);
	}

	m_mapSystemButton.clear();
}


// CGCMainMenuDlg 메시지 처리기입니다.

BOOL CGCSystemBtnDlg::OnInitDialog()
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

void CGCSystemBtnDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CGCSystemBtnDlg::OnClickMinimum()
{
	m_pParentWnd->ShowWindow(SW_MINIMIZE);
}

void CGCSystemBtnDlg::OnClickClose()
{
	m_pParentWnd->EndModalLoop(-1);
}

BOOL CGCSystemBtnDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if ( nCode == BN_CLICKED ) {
		if ( CBB_SYSTEM_BUTTON_MINIMUM == nID ) {
			OnClickMinimum();
		}
		else if ( CBB_SYSTEM_BUTTON_CLOSE == nID ) { 
			OnClickClose();
		}
	}

	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



// CGCSystemBtnDlg 메시지 처리기입니다.
