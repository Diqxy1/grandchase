// GCHtmlView.cpp : implementation file
//

#include "stdafx.h"
#include "GCPatcher.h"
#include "GCHtmlView.h"
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern FILE *fto;
/////////////////////////////////////////////////////////////////////////////
// CGCHtmlView

//IMPLEMENT_DYNCREATE(CGCHtmlView, CHtmlView)

CGCHtmlView::CGCHtmlView()
{
	//{{AFX_DATA_INIT(CGCHtmlView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CGCHtmlView::~CGCHtmlView()
{
}

void CGCHtmlView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGCHtmlView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGCHtmlView, CHtmlView)
	//{{AFX_MSG_MAP(CGCHtmlView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCHtmlView diagnostics

#ifdef _DEBUG
void CGCHtmlView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CGCHtmlView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGCHtmlView message handlers

void CGCHtmlView::AttachToDialog(CDialog* pParentWnd, DWORD dwTargetControlID, CString strUrl, bool bGird )
{
    CRect rect;

	CWnd *pControlWnd = pParentWnd->GetDlgItem(dwTargetControlID);

    //CWnd *pControlWnd = pParentWnd;
	pControlWnd->GetClientRect(&rect);
    pControlWnd->ClientToScreen(&rect);
    pParentWnd->ScreenToClient(&rect);

    
    //m_kRect = rect;
    m_kRect.bottom = rect.bottom - 300;
    m_kRect.top = rect.top;
    m_kRect.left = rect.left;
    m_kRect.right = rect.right;

    m_iWidth = m_kRect.right - m_kRect.left;
    m_iHeight = m_kRect.bottom - m_kRect.top;


	Create(NULL, NULL, WS_CHILD | WS_VISIBLE , CRect(rect.left,rect.top,rect.right,rect.bottom),pParentWnd,dwTargetControlID + 10030 );

    CRgn rgn;
    CRect rcDialog;
    this->GetClientRect( rcDialog );

    if( bGird )
        rgn.CreateRoundRectRgn( rcDialog.TopLeft().x, rcDialog.TopLeft().y, rcDialog.BottomRight().x, rcDialog.BottomRight().y, 5, 5 );
    else
        rgn.CreateRoundRectRgn( rcDialog.TopLeft().x, rcDialog.TopLeft().y, rcDialog.BottomRight().x, rcDialog.BottomRight().y, 25, 25 );

    ::SetWindowRgn(this->GetSafeHwnd(), (HRGN) rgn, TRUE);    

	ShowWindow(SW_SHOW);

	OnInitialUpdate();

    char strPath[MAX_PATH] = {0,};
    GetCurrentDirectoryA( MAX_PATH, strPath );

    //Navigate2( L"about:blank" );
	Navigate2(strUrl);

}
