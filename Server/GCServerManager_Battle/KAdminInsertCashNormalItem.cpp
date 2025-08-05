// KAdminInsertCashNormalItem.cpp : implementation file
//

#include "stdafx.h"
#include "gcservermanager.h"
#include "KAdminInsertCashNormalItem.h"
#include "GCServerManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KAdminInsertCashNormalItem dialog


KAdminInsertCashNormalItem::KAdminInsertCashNormalItem(CWnd* pParent /*=NULL*/)
	: CDialog(KAdminInsertCashNormalItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(KAdminInsertCashNormalItem)
	m_strCashInsertNormalItemGoodsID = 0;
	m_strCashInsertNormalItemUserName = _T("");
	//}}AFX_DATA_INIT
}


void KAdminInsertCashNormalItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KAdminInsertCashNormalItem)
	DDX_Text(pDX, IDC_ADMIN_INSERT_NORMAL_ITEM_GOODSID, m_strCashInsertNormalItemGoodsID);
	DDX_Text(pDX, IDC_ADMIN_INSERT_NORMAL_ITEM_USER_NAME, m_strCashInsertNormalItemUserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KAdminInsertCashNormalItem, CDialog)
	//{{AFX_MSG_MAP(KAdminInsertCashNormalItem)
	ON_BN_CLICKED(IDC_ADMIN_INSERT_CASH_NORMAL_ITEM, OnAdminInsertCashNormalItem)
	ON_BN_CLICKED(IDC_CLOSE_BUTTON, OnCloseButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KAdminInsertCashNormalItem message handlers



void KAdminInsertCashNormalItem::OnAdminInsertCashNormalItem() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    std::wstring strUserName;
    strUserName = m_strCashInsertNormalItemUserName;

    std::wstring strItemName;
    std::vector<KAdminUseItemInfo> tempVec;
    m_gcServerManagerView->m_dlgAdminDBAccess.GetvecAdminItemInfoDataList(tempVec);
    strItemName = L"";
    
    for ( size_t i = 0; i < tempVec.size(); i++ )
    {
        if ( tempVec[i].m_dwGoodsID  == m_strCashInsertNormalItemGoodsID ) 
        {
            strItemName = tempVec[i].m_strGoodsName;
            break;
        }
    }
    
    CString strTemp;
    strTemp.Format(_T("Will you insert cash_normal_item : %s\n GoodsID:%d\n to : %s\n?"), strItemName.c_str(), m_strCashInsertNormalItemGoodsID ,strUserName.c_str());
    
    if ( IDYES == MessageBox( strTemp, NULL, MB_YESNO ) )
    {
        m_gcServerManagerView->m_server.Send_InsertCashNormalItemReq( m_gcServerManagerView->m_strWideServerName.c_str(), strUserName.c_str(), m_strCashInsertNormalItemGoodsID);
		MessageBox( _T("(Normal item ) Sent packet for cash_item insertion."), NULL, MB_OK );
    }
    else
    {
		MessageBox( _T("(Normal item ) We canceled cash_item insertion."), NULL, MB_OK );
    }	
	
}

void KAdminInsertCashNormalItem::SetGCServerManagerView( CGCServerManagerView* gcServerManagerView_ )
{
    m_gcServerManagerView = gcServerManagerView_;
}

BOOL KAdminInsertCashNormalItem::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if(pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
            return TRUE;
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL KAdminInsertCashNormalItem::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_strCashInsertNormalItemGoodsID = -1;
    m_strCashInsertNormalItemUserName = L"";
    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void KAdminInsertCashNormalItem::OnCloseButton() 
{
	// TODO: Add your control notification handler code here
    EndDialog(IDD_ADMIN_INSERT_CASH_NORMAL_ITEM_);
}
