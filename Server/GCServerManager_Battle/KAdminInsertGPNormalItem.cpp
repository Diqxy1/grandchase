// KAdminInsertGPNormalItem.cpp : implementation file
//

#include "stdafx.h"
#include "gcservermanager.h"
#include "KAdminInsertGPNormalItem.h"
#include "GCServerManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// KAdminInsertGPNormalItem dialog


KAdminInsertGPNormalItem::KAdminInsertGPNormalItem(CWnd* pParent /*=NULL*/)
	: CDialog(KAdminInsertGPNormalItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(KAdminInsertGPNormalItem)
	m_strGPNormalItemUserName = _T("");
	m_iGPNormalItemGoodsID = 0;
	//}}AFX_DATA_INIT
}


void KAdminInsertGPNormalItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KAdminInsertGPNormalItem)
	DDX_Text(pDX, IDC_ADMIN_INSERT_NORMAL_ITEM_USER_NAME, m_strGPNormalItemUserName);
	DDX_Text(pDX, IDC_ADMIN_INSERT_NORMAL_ITEM_GOODSID, m_iGPNormalItemGoodsID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KAdminInsertGPNormalItem, CDialog)
	//{{AFX_MSG_MAP(KAdminInsertGPNormalItem)
	ON_BN_CLICKED(IDC_ADMIN_INSERT_GP_NORMAL_ITEM, OnAdminInsertGpNormalItem)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KAdminInsertGPNormalItem message handlers

void KAdminInsertGPNormalItem::OnAdminInsertGpNormalItem() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    std::wstring strUserName;
    strUserName = m_strGPNormalItemUserName;

    std::wstring strItemName;
    std::vector<KAdminUseItemInfo> tempVec;
    m_gcServerManagerView->m_dlgAdminDBAccess.GetvecAdminItemInfoDataList(tempVec);
    strItemName = L"";
    
    for ( size_t i = 0; i < tempVec.size(); i++ )
    {
        if ( tempVec[i].m_dwGoodsID  == m_iGPNormalItemGoodsID ) 
        {
            strItemName = tempVec[i].m_strGoodsName;
            break;
        }
    }
    
    CString strTemp;
    strTemp.Format(_T("will you insert Normal item : %s\n GoodsID   : %d\n to User : %s\n?"), strItemName.c_str(), m_iGPNormalItemGoodsID, strUserName.c_str() );
    
    if ( IDYES == MessageBox( strTemp, NULL, MB_YESNO ) )
    {
        m_gcServerManagerView->m_server.Send_InsertGPNormalItemReq( m_gcServerManagerView->m_strWideServerName.c_str(), strUserName.c_str(), m_iGPNormalItemGoodsID);
        MessageBox( _T("(Normal item ) Sent packet for item insertion."), NULL, MB_OK );
    }
    else
    {
        MessageBox( _T("(Normal item ) We canceled item insertion."), NULL, MB_OK );
    }	
}

void KAdminInsertGPNormalItem::SetGCServerManagerView( CGCServerManagerView* gcServerManagerView_ )
{
    m_gcServerManagerView = gcServerManagerView_;
}

BOOL KAdminInsertGPNormalItem::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if(pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
            return TRUE;
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL KAdminInsertGPNormalItem::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_iGPNormalItemGoodsID = -1;

    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void KAdminInsertGPNormalItem::OnClose() 
{
	// TODO: Add your control notification handler code here
    EndDialog(IDD_ADMIN_INSERT_GP_NORMAL_ITEM_);
}
