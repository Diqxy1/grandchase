// KAdminInsertCashDurationITem.cpp : implementation file
//

#include "stdafx.h"
#include "gcservermanager.h"
#include "KAdminInsertCashDurationITem.h"
#include "GCServerManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KAdminInsertCashDurationITem dialog


KAdminInsertCashDurationITem::KAdminInsertCashDurationITem(CWnd* pParent /*=NULL*/)
	: CDialog(KAdminInsertCashDurationITem::IDD, pParent)
{
	//{{AFX_DATA_INIT(KAdminInsertCashDurationITem)
	m_iCashInsertDurationItemGoodsID = 0;
	m_strCashInsertDurationItemUserName = _T("");
	//}}AFX_DATA_INIT
}


void KAdminInsertCashDurationITem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KAdminInsertCashDurationITem)
	DDX_Text(pDX, IDC_ADMIN_INSERT_DURATION_ITEM_GOODSID, m_iCashInsertDurationItemGoodsID);
	DDX_Text(pDX, IDC_ADMIN_INSERT_DURATION_ITEM_USER_NAME, m_strCashInsertDurationItemUserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KAdminInsertCashDurationITem, CDialog)
	//{{AFX_MSG_MAP(KAdminInsertCashDurationITem)
	ON_BN_CLICKED(IDC_ADMIN_INSERT_CASH_DURATION_ITEM, OnAdminInsertCashDurationItem)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KAdminInsertCashDurationITem message handlers



void KAdminInsertCashDurationITem::OnAdminInsertCashDurationItem() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    std::wstring strUserName;
    strUserName = m_strCashInsertDurationItemUserName;

    std::wstring strItemName;
    std::vector<KAdminUseItemInfo> tempVec;
    m_gcServerManagerView->m_dlgAdminDBAccess.GetvecAdminItemInfoDataList(tempVec);
    strItemName = L"";
    
    for ( size_t i = 0; i < tempVec.size(); i++ )
    {
        if ( tempVec[i].m_dwGoodsID  == m_iCashInsertDurationItemGoodsID ) 
        {
            strItemName = tempVec[i].m_strGoodsName;
            break;
        }
    }
    
    CString strTemp;
    strTemp.Format(_T("Will you insert Disposable cash_item  : %s\nGoodsID:%d\n to : %s\n?"), strItemName.c_str(), m_iCashInsertDurationItemGoodsID , strUserName.c_str() );
    
    if ( IDYES == MessageBox( strTemp, NULL, MB_YESNO ) )
    {
        m_gcServerManagerView->m_server.Send_InsertCashDurationItemReq( m_gcServerManagerView->m_strWideServerName.c_str(), strUserName.c_str(), m_iCashInsertDurationItemGoodsID);
		MessageBox( _T("(Disposable item ) We sent packet for cash_item insertion"), NULL, MB_OK );
    }
    else
    {
        
        MessageBox( _T("(Disposable item ) We canceled to send packet for cash_item insertion."), NULL, MB_OK );
    }	
	
}

void KAdminInsertCashDurationITem::SetGCServerManagerView( CGCServerManagerView* gcServerManagerView_ )
{
    m_gcServerManagerView = gcServerManagerView_;
}


BOOL KAdminInsertCashDurationITem::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if(pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
            return TRUE;
    }
    
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL KAdminInsertCashDurationITem::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_iCashInsertDurationItemGoodsID  = -1;
    m_strCashInsertDurationItemUserName = L"";

    UpdateData(FALSE);
    
	
	// TODO: Add extra initialization here

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void KAdminInsertCashDurationITem::OnClose() 
{
	// TODO: Add your control notification handler code here
	EndDialog(IDD_ADMIN_INSERT_CASH_DURATION_ITEM_);
}
