// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "GCServerManager.h"
#include "KNotifyMsgDlg.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENU_NOTIFY_MSG, OnMenuNotifyMsg)
    ON_COMMAND(ID_MENU_BLACK_LIST, OnMenuBlackList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	m_pMainFrm = static_cast<CMainFrame*>( ::AfxGetMainWnd() );
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
    cs.cx = 830;
    cs.cy = 630;

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnFileNew()
{
	m_pMainFrm->OnFileNew();
}

void CChildFrame::OnFileOpen() 
{
	m_pMainFrm->OnFileOpen();
}

void CChildFrame::OnClose() 
{
    static_cast<CGCServerManagerView*>( GetActiveView() )->CloseServer();

	CMDIChildWnd::OnClose();
}

void CChildFrame::OnMenuNotifyMsg()
{
    m_pMainFrm->m_dlgNotifyMsg.Create( IDD_DIALOG_NOTIFY, m_pMainFrm );
}

void CChildFrame::OnMenuBlackList()
{
    m_pMainFrm->m_dlgBlackList.Create( IDD_DIALOG_BLACK_LIST, m_pMainFrm );
}