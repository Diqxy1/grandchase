// KAdminDelItem.cpp : implementation file
//

#include "stdafx.h"
#include "gcservermanager.h"
#include "KAdminDelItem.h"
#include "GCServerManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KAdminDelItem dialog


KAdminDelItem::KAdminDelItem(CWnd* pParent /*=NULL*/)
	: CDialog(KAdminDelItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(KAdminDelItem)
	m_DelItemGoodsID = 0;
	m_DelItemGoodsUID = 0;
	m_strDelItemUserName = _T("");
	//}}AFX_DATA_INIT
}


void KAdminDelItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KAdminDelItem)
	DDX_Text(pDX, IDC_ADMIN_DEL_ITEM_GOODSID, m_DelItemGoodsID);
	DDX_Text(pDX, IDC_ADMIN_DEL_ITEM_GOODSUID, m_DelItemGoodsUID);
	DDX_Text(pDX, IDC_ADMIN_DEL_ITEM_USER_NAME, m_strDelItemUserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KAdminDelItem, CDialog)
	//{{AFX_MSG_MAP(KAdminDelItem)
	ON_BN_CLICKED(IDC_ADMIN_DEL_ITEM, OnAdminDelItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KAdminDelItem message handlers

void KAdminDelItem::SetGCServerManagerView( CGCServerManagerView* gcServerManagerView_ )
{
    m_gcServerManagerView = gcServerManagerView_;
}

void KAdminDelItem::OnAdminDelItem() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    std::wstring strUserName;
    strUserName = m_strDelItemUserName;

    std::wstring strItemName;
    std::vector<KAdminUseItemInfo> tempVec;
    m_gcServerManagerView->m_dlgAdminDBAccess.GetvecAdminItemInfoDataList(tempVec);
    strItemName = L"";

    for ( size_t i = 0; i < tempVec.size(); i++ )
    {
        if ( tempVec[i].m_dwGoodsID  == m_DelItemGoodsID ) 
        {
            strItemName = tempVec[i].m_strGoodsName;
            break;
        }
    }


    CString strTemp;
    strTemp.Format(_T("Will you delete  User : %s \'s \n item : %s\nGoodsUID   : %d\nGoodsID     : %d\n?"),strUserName.c_str(), strItemName.c_str(), m_DelItemGoodsUID, m_DelItemGoodsID  );

    if ( IDYES == MessageBox( strTemp, NULL, MB_YESNO ) )
    {
        m_gcServerManagerView->m_server.Send_DelItemReq( m_gcServerManagerView->m_strWideServerName.c_str(), strUserName.c_str(), m_DelItemGoodsID, m_DelItemGoodsUID );
        MessageBox( _T("We sent packet for item delete."), NULL, MB_OK );
    }
    else
    {
        MessageBox( _T("We canceled to send packet for item delete."), NULL, MB_OK );
    }
}

BOOL KAdminDelItem::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if(pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
            return TRUE;
    }
    	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL KAdminDelItem::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_DelItemGoodsID = -1;
    m_DelItemGoodsUID = -1;
    UpdateData( FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
