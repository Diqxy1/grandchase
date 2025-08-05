// KAdminInsertGPDurationItem.cpp : implementation file
//

#include "stdafx.h"
#include "gcservermanager.h"
#include "KAdminInsertGPDurationItem.h"
#include "GCServerManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KAdminInsertGPDurationItem dialog


KAdminInsertGPDurationItem::KAdminInsertGPDurationItem(CWnd* pParent /*=NULL*/)
	: CDialog(KAdminInsertGPDurationItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(KAdminInsertGPDurationItem)
	m_strGPDurationItemUserName = _T("");
	m_iGPDurationItemGoodsID = 0;
	//}}AFX_DATA_INIT
}


void KAdminInsertGPDurationItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KAdminInsertGPDurationItem)
	DDX_Text(pDX, IDC_ADMIN_INSERT_DURATION_ITEM_USER_NAME, m_strGPDurationItemUserName);
	DDX_Text(pDX, IDC_ADMIN_INSERT_DURATION_ITEM_GOODSID, m_iGPDurationItemGoodsID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KAdminInsertGPDurationItem, CDialog)
	//{{AFX_MSG_MAP(KAdminInsertGPDurationItem)
	ON_BN_CLICKED(IDC_ADMIN_INSERT_GP_DURATION_ITEM, OnAdminInsertGpDurationItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KAdminInsertGPDurationItem message handlers

void KAdminInsertGPDurationItem::OnAdminInsertGpDurationItem() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    std::wstring strUserName;
    strUserName = m_strGPDurationItemUserName;

    std::wstring strItemName;
    std::vector<KAdminUseItemInfo> tempVec;
    m_gcServerManagerView->m_dlgAdminDBAccess.GetvecAdminItemInfoDataList(tempVec);
    strItemName = L"";
    
    for ( size_t i = 0; i < tempVec.size(); i++ )
    {
        if ( tempVec[i].m_dwGoodsID  == m_iGPDurationItemGoodsID ) 
        {
            strItemName = tempVec[i].m_strGoodsName;
            break;
        }
    }
    
    CString strTemp;
    strTemp.Format(_T("Will you insert Disposable item  : %s\n GoodsID        : %d\n  to : %s\n?"), strItemName.c_str(), m_iGPDurationItemGoodsID ,strUserName.c_str() );
    
    if ( IDYES == MessageBox( strTemp, NULL, MB_YESNO ) )
    {
        m_gcServerManagerView->m_server.Send_InsertGPDurationItemReq( m_gcServerManagerView->m_strWideServerName.c_str(), strUserName.c_str(), m_iGPDurationItemGoodsID);
        MessageBox( _T("(Disposable item ) We sent packet for item insertion."), NULL, MB_OK );
    }
    else
    {
        
        MessageBox( _T("(Disposable item ) We canceled to send packet for item insertion."), NULL, MB_OK );
    }	
}

void KAdminInsertGPDurationItem::SetGCServerManagerView( CGCServerManagerView* gcServerManagerView_ )
{
    m_gcServerManagerView = gcServerManagerView_;
}

BOOL KAdminInsertGPDurationItem::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if(pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
            return TRUE;
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL KAdminInsertGPDurationItem::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_iGPDurationItemGoodsID = -1;

    UpdateData( FALSE );
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
