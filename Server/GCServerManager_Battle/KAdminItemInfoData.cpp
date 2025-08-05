// KAdminItemInfoData.cpp : implementation file
//

#include "stdafx.h"
#include "gcservermanager.h"
#include "KAdminItemInfoData.h"
#include "UserPacket.h"
#include "ItemStructure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KAdminItemInfoData dialog


KAdminItemInfoData::KAdminItemInfoData(CWnd* pParent /*=NULL*/)
	: CDialog(KAdminItemInfoData::IDD, pParent)
{
	//{{AFX_DATA_INIT(KAdminItemInfoData)
	m_iGoodsID = 0;
	m_strGoodsName = _T("");
	//}}AFX_DATA_INIT
}


void KAdminItemInfoData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KAdminItemInfoData)
	DDX_Control(pDX, IDC_ITEMINFOLIST, m_crlItemInfoList);
	DDX_Text(pDX, IDC_ITEM_GOODSID, m_iGoodsID);
	DDX_Text(pDX, IDC_ITEM_SUB_NAME, m_strGoodsName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KAdminItemInfoData, CDialog)
	//{{AFX_MSG_MAP(KAdminItemInfoData)
	ON_BN_CLICKED(IDC_SELECT_ADMIN_ITEM_INFO_DATA, OnSelectAdminItemInfoData)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KAdminItemInfoData message handlers

BOOL KAdminItemInfoData::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if(pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
            return TRUE;
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}

void KAdminItemInfoData::OnSelectAdminItemInfoData() 
{
    UpdateData( TRUE );

    m_crlItemInfoList.DeleteAllItems();
    CString str;
    CString strTemp;
    int CountNum;
    CountNum = 0;
    std::vector<KAdminUseItemInfo>::iterator vit;

    if ( m_iGoodsID != -1 )
    {
        for( vit = m_vecAdminItemInfoData.begin(); vit != m_vecAdminItemInfoData.end(); vit++ )
        {
            if ( vit->m_dwGoodsID == m_iGoodsID )
            {
                m_crlItemInfoList.InsertItem( CountNum, vit->m_strGoodsName.c_str());
            
                str.Format(_T("%d"),  vit->m_dwGoodsID );
                m_crlItemInfoList.SetItemText(CountNum, 1, str );

                str.Format(_T("%d"),  vit->m_bCash );
                m_crlItemInfoList.SetItemText(CountNum, 2, str );

                str.Format(_T("%d"),  vit->m_iStrongLevel );
                m_crlItemInfoList.SetItemText(CountNum, 3, str );

                str.Format(_T("%d"),  vit->m_iDurationNum );
                m_crlItemInfoList.SetItemText(CountNum, 4, str );

                str.Format(_T("%d"),  vit->m_iCharType );
                m_crlItemInfoList.SetItemText(CountNum, 5, str );

                str.Format(_T("%d"),  vit->m_bShowWeb );
                m_crlItemInfoList.SetItemText(CountNum, 6, str );

                str.Format(_T("%d"),  vit->m_iGem );
                m_crlItemInfoList.SetItemText(CountNum, 7, str );
            
                str.Format(_T("%d"),  vit->m_iCrystal );
                m_crlItemInfoList.SetItemText(CountNum, 8, str );

                CountNum++;
                break;
            }
        }
    }

    if ( m_strGoodsName.GetLength() != 0  )
    {
        for( vit = m_vecAdminItemInfoData.begin(); vit != m_vecAdminItemInfoData.end(); vit++ )
        {
            strTemp = vit->m_strGoodsName.c_str();
            if ( -1 != strTemp.Find( m_strGoodsName ) )
            {
                m_crlItemInfoList.InsertItem( CountNum, vit->m_strGoodsName.c_str());
        
                str.Format(_T("%d"),  vit->m_dwGoodsID );
                m_crlItemInfoList.SetItemText(CountNum, 1, str );
        
                str.Format(_T("%d"),  vit->m_bCash );
                m_crlItemInfoList.SetItemText(CountNum, 2, str );
        
                str.Format(_T("%d"),  vit->m_iStrongLevel );
                m_crlItemInfoList.SetItemText(CountNum, 3, str );
        
                str.Format(_T("%d"),  vit->m_iDurationNum );
                m_crlItemInfoList.SetItemText(CountNum, 4, str );
        
                str.Format(_T("%d"),  vit->m_iCharType );
                m_crlItemInfoList.SetItemText(CountNum, 5, str );
        
                str.Format(_T("%d"),  vit->m_bShowWeb );
                m_crlItemInfoList.SetItemText(CountNum, 6, str );
        
                str.Format(_T("%d"),  vit->m_iGem );
                m_crlItemInfoList.SetItemText(CountNum, 7, str );
        
                str.Format(_T("%d"),  vit->m_iCrystal );
                m_crlItemInfoList.SetItemText(CountNum, 8, str );
        
                CountNum++;
            }
        }
    }
    
    UpdateData( FALSE );
    // TODO: Add your control notification handler code here
	
}

void KAdminItemInfoData::SetAdminItemInfoData( std::vector<KAdminUseItemInfo> vecAdminItemInfoData_ )
{
    m_vecAdminItemInfoData = vecAdminItemInfoData_;
}

void KAdminItemInfoData::OnClose() 
{
	// TODO: Add your control notification handler code here
    DestroyWindow();
}

BOOL KAdminItemInfoData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_crlItemInfoList.InsertColumn( 0, _T("Item Name"), LVCFMT_LEFT, 190); 
    m_crlItemInfoList.InsertColumn( 1, _T("Item GoodsID"), LVCFMT_LEFT, 80); 
    m_crlItemInfoList.InsertColumn( 2, _T(" Cash "), LVCFMT_LEFT, 80); 
    m_crlItemInfoList.InsertColumn( 3, _T("Strong Level"), LVCFMT_LEFT, 88); 
    m_crlItemInfoList.InsertColumn( 4, _T("Duration"), LVCFMT_LEFT, 80); 
    m_crlItemInfoList.InsertColumn( 5, _T("Char Type"), LVCFMT_LEFT, 80); 
    m_crlItemInfoList.InsertColumn( 6, _T("Show Web"), LVCFMT_LEFT, 88); 
    m_crlItemInfoList.InsertColumn( 7, _T("Need Gem"), LVCFMT_LEFT, 80); 
    m_crlItemInfoList.InsertColumn( 8, _T("Need Crystal"), LVCFMT_LEFT, 80); 

    m_iGoodsID  = -1;
    m_strGoodsName = L"";
    UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
