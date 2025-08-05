// KAdminDBAccessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gcservermanager.h"
#include "KAdminDBAccessDlg.h"

#include "GCServerManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KAdminDBAccessDlg dialog


KAdminDBAccessDlg::KAdminDBAccessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KAdminDBAccessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(KAdminDBAccessDlg)
	m_strUserName = _T("");
	m_iGoodsID = 0;
	m_iGoodsUID = 0;
	//}}AFX_DATA_INIT

}


void KAdminDBAccessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KAdminDBAccessDlg)
	DDX_Control(pDX, IDC_LIST2, m_ctrList2);
	DDX_Control(pDX, IDC_LIST1, m_ctrList1);
	DDX_Text(pDX, IDC_USER_NAME, m_strUserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KAdminDBAccessDlg, CDialog)
	//{{AFX_MSG_MAP(KAdminDBAccessDlg)
	ON_BN_CLICKED(IDC_SELECT_NORMAL_ITEM, OnSelectNormalItem)
	ON_BN_CLICKED(IDC_NORMAL_ITEM_SAVE_TEXT, OnNormalItemSaveText)
	ON_BN_CLICKED(IDC_SELECT_DEL_NORMAL_ITEM, OnSelectDelNormalItem)
	ON_BN_CLICKED(IDC_SELECT_DURATION_ITEM, OnSelectDurationItem)
	ON_BN_CLICKED(IDC_DURATION_DEL_ITEM, OnDurationDelItem)
	ON_BN_CLICKED(IDC_INSERT_ITEM, OnInsertItem)
	ON_BN_CLICKED(IDC_DELETE_ITEM, OnDeleteItem)
	ON_BN_CLICKED(IDC_DURATION_ITEM_SAVE_TEXT, OnDurationItemSaveText)
	ON_BN_CLICKED(IDC_INSERT_GP_DURATION_ITEM, OnInsertGpDurationItem)
	ON_BN_CLICKED(IDC_SELECT_USER_DATA, OnSelectUserData)
	ON_BN_CLICKED(IDC_CHANGE_STRONG_ITEM_LIST, OnChangeStrongItemList)
	ON_BN_CLICKED(IDC_ADMIN_ITEMINFO_DATA_BUTTON, OnAdminIteminfoDataButton)
	ON_BN_CLICKED(IDC_INSERT_CASH_NORMAL_ITEM, OnInsertCashNormalItem)
	ON_BN_CLICKED(IDC_INSERT_CASH_DURATION_ITEM, OnInsertCashDurationItem)
	ON_BN_CLICKED(IDC_CLOSE_BUTTON, OnCloseButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KAdminDBAccessDlg message handlers

void KAdminDBAccessDlg::SetListContrlIntial()
{
    m_ctrList1.InsertColumn( 0, _T("Item Name"), LVCFMT_LEFT, 200); 
    m_ctrList1.InsertColumn( 1, _T("BuyerLogin"), LVCFMT_LEFT, 100); 
    m_ctrList1.InsertColumn( 2, _T("OwnerLogin"), LVCFMT_LEFT, 100); 
    m_ctrList1.InsertColumn( 3, _T("Item UID"), LVCFMT_LEFT, 80); 
    m_ctrList1.InsertColumn( 4, _T("Item ID"), LVCFMT_LEFT, 60); 
    m_ctrList1.InsertColumn( 5, _T("Buy Date"), LVCFMT_LEFT, 140); 
    m_ctrList1.InsertColumn( 6, _T("Start Date"), LVCFMT_LEFT, 140);
    m_ctrList1.InsertColumn( 7, _T("End Date"), LVCFMT_LEFT, 140);
    m_ctrList1.InsertColumn( 8, _T("Period"), LVCFMT_LEFT, 30);
    m_ctrList1.InsertColumn( 9, _T("Del Date "), LVCFMT_LEFT, 140); 

    m_ctrList2.InsertColumn( 0, _T("Item Name"), LVCFMT_LEFT, 200); 
    m_ctrList2.InsertColumn( 1, _T("BuyerLogin"), LVCFMT_LEFT, 100); 
    m_ctrList2.InsertColumn( 2, _T("OwnerLogin"), LVCFMT_LEFT, 100); 
    m_ctrList2.InsertColumn( 3, _T("Item UID"), LVCFMT_LEFT, 80); 
    m_ctrList2.InsertColumn( 4, _T("Item ID"), LVCFMT_LEFT, 60); 
    m_ctrList2.InsertColumn( 5, _T("Quantity Num"), LVCFMT_LEFT, 60); 
    m_ctrList2.InsertColumn( 6, _T("Buy Date"), LVCFMT_LEFT, 140); 
    m_ctrList2.InsertColumn( 7, _T("Del Date"), LVCFMT_LEFT, 140); 
}

void KAdminDBAccessDlg::SetListContrlData( std::vector<KSelectNormalItemInfo>& vecItemInv_ )
{
    CString str;

    for ( size_t i = 0; i < vecItemInv_.size(); i++)
    {
        m_ctrList1.InsertItem( i, vecItemInv_[i].m_strGoodsName.c_str());

        m_ctrList1.SetItemText(i, 1, vecItemInv_[i].m_strBuyerLogin.c_str() );
        m_ctrList1.SetItemText(i, 2, vecItemInv_[i].m_strOwnerLogin.c_str() );

        str.Format(_T("%d"), vecItemInv_[i].m_dwGoodsUID);
        m_ctrList1.SetItemText(i, 3,  str);

        str.Format(_T("%d"), vecItemInv_[i].m_dwGoodsID);
        m_ctrList1.SetItemText(i, 4,  str);

        CTime cbuyTime = CTime( vecItemInv_[i].m_tRegDate );
        str.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), cbuyTime.GetYear(), cbuyTime.GetMonth(), cbuyTime.GetDay(), cbuyTime.GetHour(), cbuyTime.GetMinute(), cbuyTime.GetSecond());
        m_ctrList1.SetItemText(i, 5,  str);

        CTime cStartTime = CTime( vecItemInv_[i].m_tStartDate );
        str.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), cStartTime.GetYear(), cStartTime.GetMonth(), cStartTime.GetDay(), cStartTime.GetHour(), cStartTime.GetMinute(), cStartTime.GetSecond());
        m_ctrList1.SetItemText(i, 6,  str);

        CTime cEndTime = CTime( vecItemInv_[i].m_tEndDate );
        str.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), cEndTime.GetYear(), cEndTime.GetMonth(), cEndTime.GetDay(), cEndTime.GetHour(), cEndTime.GetMinute(), cEndTime.GetSecond());
        m_ctrList1.SetItemText(i, 7,  str);

        str.Format(_T("%d"), vecItemInv_[i].m_iPeriod);
        m_ctrList1.SetItemText(i, 8,  str);

        if ( vecItemInv_[i].m_tDelDate != -1 )
        {
            CTime cdelTime = CTime( vecItemInv_[i].m_tDelDate );
            str.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), cdelTime.GetYear(), cdelTime.GetMonth(), cdelTime.GetDay(), cdelTime.GetHour(), cdelTime.GetMinute(), cdelTime.GetSecond());
            m_ctrList1.SetItemText(i, 9,  str);
        }
        else
        {
            str.Format(_T("%d"), vecItemInv_[i].m_tDelDate);
            m_ctrList1.SetItemText(i, 9,  str);
        }
    }
}


void KAdminDBAccessDlg::OnSelectNormalItem() 
{
	// TODO: Add your control notification handler code here
    m_ctrList1.DeleteAllItems();

    UpdateData( TRUE );
    std::wstring strUserName;
    strUserName = m_strUserName;
    m_gcServerManagerView->m_server.Send_SelectNormalItemReq( m_gcServerManagerView->m_strWideServerName.c_str(), strUserName.c_str());
    //MessageBox( "(일반 아이템 ) 가진 아이템 목록 얻어오는 패킷을 보냈습니다.", NULL, MB_OK );
}

void KAdminDBAccessDlg::SetGCServerManagerView( CGCServerManagerView* gcServerManagerView_ )
{
    m_gcServerManagerView = gcServerManagerView_;
}

void KAdminDBAccessDlg::OnNormalItemSaveText() 
{
    CFile    cFile;
    CString  strfileName;
    CString  strData;
    CString  struserName;
    CString  strDelDate;

    CString  strtime;
    CTime    timer;
    timer = CTime::GetCurrentTime();    //현재 시간을 넣어줌 
    strtime.Format(_T("%d Mon %d day %d hour %d Min %d sec"), timer.GetMonth(), timer.GetDay(), timer.GetHour(), timer.GetMinute(), timer.GetSecond());
    

    //소유한 아이템인지 판 아이템인지 판별 
    if ( m_ctrList1.GetItemCount() > 0 )
    {
        //KSelectNormalItemInfo* pData1 = (KSelectNormalItemInfo*)m_ctrList1);
        strDelDate = m_ctrList1.GetItemText(0, 6);
        
        if ( strDelDate == "-1" )
        {
            strfileName = m_ctrList1.GetItemText(0, 2);
            strfileName += L"Normal belonging item list";
            strfileName += strtime;
            strfileName += L".txt";

            strData = L"=============================================================================";
            strData += L"\r\n                  ";
            strData += m_ctrList1.GetItemText(0, 2);
            strData += L"Normal belonging item list \r\n";
            strData += L"=============================================================================\r\n\r\n";
            CString tmeptitle;
            tmeptitle.Format(_T("%30s"), "item name" );
            strData += tmeptitle;
            tmeptitle.Format(_T("%20s"), "item buyer");
            strData += tmeptitle;
            tmeptitle.Format(_T("%30s"), "item purchase date" );
            strData += tmeptitle;
            strData += L"\r\n";
        }
        else
        {
            strfileName = m_ctrList1.GetItemText(0, 2);
            strfileName += L"Normal sold Item list";
            strfileName += strtime;
            strfileName += L".txt";

            strData = L"=============================================================================";
            strData += L"\r\n                  ";
            strData += m_ctrList1.GetItemText(0, 2);
            strData += L"Normal sold Item list \r\n";
            strData += L"=============================================================================\r\n\r\n";

            CString tmeptitle;
            tmeptitle.Format(_T("%30s"), _T("Item name") );
            strData += tmeptitle;
            tmeptitle.Format(_T("%20s"), _T("item buyer") );
            strData += tmeptitle;
            tmeptitle.Format(_T("%30s"), _T("item purchase date"));
            strData += tmeptitle;
            tmeptitle.Format(_T("%30s"), _T("item sold date") );
            strData += tmeptitle;
            strData += _T("\r\n");
        }

        CString temp;
        //실제 데이터 넣기 
        if ( strDelDate == "-1" )
        {
            for ( int i = 0; i < m_ctrList1.GetItemCount(); i++ )
            {
                temp.Format(_T("%30s"), m_ctrList1.GetItemText(i, 0) );
                strData += temp;
                temp.Format(_T("%20s"), m_ctrList1.GetItemText(i, 1) );
                strData += temp;
                temp.Format(_T("%30s"), m_ctrList1.GetItemText(i, 5) );
                strData += temp;
                strData += _T("\r\n");
                /*
                strData += m_ctrList1.GetItemText(i, 0);
                strData += "\t\t";
                strData += m_ctrList1.GetItemText(i, 1);
                strData += "\t\t";
                strData += m_ctrList1.GetItemText(i, 4);
                strData += "\r\n";
                */
            }
        }
        else //판 아이템
        {
            for ( int i = 0; i < m_ctrList1.GetItemCount(); i++ )
            {
                temp.Format(_T("%30s"), m_ctrList1.GetItemText(i, 0) );
                strData += temp;
                temp.Format(_T("%20s"), m_ctrList1.GetItemText(i, 1) );
                strData += temp;
                temp.Format(_T("%30s"), m_ctrList1.GetItemText(i, 5) );
                strData += temp;
                temp.Format(_T("%30s"), m_ctrList1.GetItemText(i, 6) );
                strData += temp;
                strData += _T("\r\n");
                /*
                strData += m_ctrList1.GetItemText(i, 0);
                strData += "\t\t";
                strData += m_ctrList1.GetItemText(i, 1);
                strData += "\t\t";
                strData += m_ctrList1.GetItemText(i, 4);
                strData += "\t\t";
                strData += m_ctrList1.GetItemText(i, 5);
                strData += "\r\n";
                */
            }
        }
        
        strData += _T("\0");
        
        cFile.Open( strfileName, CFile::modeCreate | CFile::modeWrite); //파일을 열다, 없으면 만듬 
        cFile.Write( strData, strData.GetLength());      //파일에 내용을 쓴다. 
        cFile.Close();

        MessageBox(_T(" Succeeded to save Normal Item list as text file "), NULL, MB_OK );
    }
    else
    {
        MessageBox(_T(" Failed to save Normal Item list as text file "), NULL, MB_OK );
        return;
    }
}

BOOL KAdminDBAccessDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetListContrlIntial();
    //GetAdminItemInfoDataList();


    //운영자의 정보를 얻어온다.
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void KAdminDBAccessDlg::GetAdminItemInfoDataList()
{
    //050607. microcat. DB에서 아이템 정보를 읽어오지 말고 파일에서 읽어오도록 한다.
    m_gcServerManagerView->GetAdminUseItemInfoData();
}

void KAdminDBAccessDlg::GetvecAdminItemInfoDataList( std::vector<KAdminUseItemInfo>& vecItemInv_)
{
    vecItemInv_ = m_vecAdminItemDataList;
}

void KAdminDBAccessDlg::OnSelectDelNormalItem() 
{
	// TODO: Add your control notification handler code here
    m_ctrList1.DeleteAllItems();
    UpdateData( TRUE );
    std::wstring strUserName;
    strUserName = m_strUserName;
    m_gcServerManagerView->m_server.Send_SelectDelNormalItemReq( m_gcServerManagerView->m_strWideServerName.c_str(), strUserName.c_str());
    //MessageBox( "(일반 아이템 ) 판 아이템 목록 얻어오는 패킷을 보냈습니다.", NULL, MB_OK );
	
}

void KAdminDBAccessDlg::OnSelectDurationItem() 
{
	// TODO: Add your control notification handler code here
    m_ctrList2.DeleteAllItems();
    UpdateData( TRUE );
    std::wstring strUserName;
    strUserName = m_strUserName;
    m_gcServerManagerView->m_server.Send_SelectDurationItemReq( m_gcServerManagerView->m_strWideServerName.c_str(), strUserName.c_str());
    //MessageBox( "(수량성 아이템 ) 가진  아이템 목록 얻어오는 패킷을 보냈습니다.", NULL, MB_OK );
}


void KAdminDBAccessDlg::OnDurationDelItem() 
{
	// TODO: Add your control notification handler code here
    m_ctrList2.DeleteAllItems();
    UpdateData( TRUE );
    std::wstring strUserName;
    strUserName = m_strUserName;
    m_gcServerManagerView->m_server.Send_SelectDelDurationItemReq( m_gcServerManagerView->m_strWideServerName.c_str(), strUserName.c_str());
    //MessageBox( "(수량 아이템 ) 판 아이템 목록 얻어오는 패킷을 보냈습니다.", NULL, MB_OK );
}

void KAdminDBAccessDlg::OnInsertItem() 
{
	// TODO: Add your control notification handler code here
    m_kInsertGPNormalItem.SetGCServerManagerView( m_gcServerManagerView );
    m_kInsertGPNormalItem.DoModal();
}

void KAdminDBAccessDlg::OnDeleteItem() 
{
	// TODO: Add your control notification handler code here
    m_kDelItem.SetGCServerManagerView( m_gcServerManagerView );
    m_kDelItem.DoModal();
}

void KAdminDBAccessDlg::OnDurationItemSaveText() 
{
	// TODO: Add your control notification handler code here
    CFile       cFile;
    CString     strfileName;
    CString     strData;
    CString     struserName;
    CString     strDelDate;

    CString  strtime;
    CTime    timer;
    timer = CTime::GetCurrentTime();    //현재 시간을 넣어줌 
    strtime.Format(_T("%d Mon %d day %d hour %d Min %d Sec"), timer.GetMonth(), timer.GetDay(), timer.GetHour(), timer.GetMinute(), timer.GetSecond());
    
    
    
    //소유한 아이템인지 판 아이템인지 판별 
    if ( m_ctrList2.GetItemCount() > 0 )
    {
        //KSelectNormalItemInfo* pData1 = (KSelectNormalItemInfo*)m_ctrList1);
        strDelDate = m_ctrList2.GetItemText(0, 6);
        
        if ( strDelDate == "-1" )
        {
            strfileName = m_ctrList2.GetItemText(0, 2);
            strfileName += _T("Disposable belonging item list");
            strfileName += strtime;
            strfileName += _T(".txt");
            
            strData = _T("=============================================================================");
            strData += _T("\r\n                  ");
            strData += m_ctrList2.GetItemText(0, 2);
            strData += _T("Disposable belonging item list \r\n");
            strData += _T("=============================================================================\r\n\r\n");
            
            CString tmeptitle;
			tmeptitle.Format(_T("%30s"), _T("item name") );
            strData += tmeptitle;
			tmeptitle.Format(_T("%20s"), _T("item buyer") );
            strData += tmeptitle;            
            tmeptitle.Format(_T("%20s"), _T("Item quantity"));
            strData += tmeptitle;
			tmeptitle.Format(_T("%30s"), _T("item purchase date"));
            strData += tmeptitle;
            strData += _T("\r\n");
        }
        else
        {
            strfileName = m_ctrList2.GetItemText(0, 2);
            strfileName += _T("Disposable sold item list");
            strfileName += strtime;
            strfileName += _T(".txt");
            
            strData = _T("=============================================================================");
            strData += _T("\r\n                  ");
            strData += m_ctrList2.GetItemText(0, 2);
            strData += _T("Disposable sold item list \r\n");
            strData += _T("=============================================================================\r\n\r\n");

            CString tmeptitle;
			tmeptitle.Format(_T("%30s"), _T("item name") );
            strData += tmeptitle;
			tmeptitle.Format(_T("%20s"), _T("item buyer") );
            strData += tmeptitle;
			tmeptitle.Format(_T("%20s"), _T("Item quantity"));
            strData += tmeptitle;            
			tmeptitle.Format(_T("%30s"), _T("item purchase date"));
            strData += tmeptitle;
			tmeptitle.Format(_T("%30s"), _T("item sold date") );
            strData += tmeptitle;
            strData += _T("\r\n");

        }
        
        CString temp;
        //실제 데이터 넣기 
        if ( strDelDate == _T("-1") )
        {
            for ( int i = 0; i < m_ctrList2.GetItemCount(); i++ )
            {
                temp.Format(_T("%30s"), m_ctrList2.GetItemText(i, 0) );
                strData += temp;
                temp.Format(_T("%20s"), m_ctrList2.GetItemText(i, 1) );
                strData += temp;
                temp.Format(_T("%20s"), m_ctrList2.GetItemText(i, 5) );
                strData += temp;
                temp.Format(_T("%30s"), m_ctrList2.GetItemText(i, 6) );
                strData += temp;
                strData += _T("\r\n");
            }
        }
        else //판 아이템
        {
            for ( int i = 0; i < m_ctrList2.GetItemCount(); i++ )
            {
                temp.Format(_T("%30s"), m_ctrList2.GetItemText(i, 0));
                strData += temp;
                temp.Format(_T("%20s"), m_ctrList2.GetItemText(i, 1));
                strData += temp;
                temp.Format(_T("%20s"), m_ctrList2.GetItemText(i, 5));
                strData += temp;
                temp.Format(_T("%30s"), m_ctrList2.GetItemText(i, 6));
                strData += temp;
                temp.Format(_T("%30s"), m_ctrList2.GetItemText(i, 7));
                strData += temp;
                strData += _T("\r\n");
            }
        }
        
        strData += _T("\0");
        
        cFile.Open( strfileName, CFile::modeCreate | CFile::modeWrite); //파일을 열다, 없으면 만듬 
        cFile.Write( strData, strData.GetLength());      //파일에 내용을 쓴다. 
        cFile.Close();

        MessageBox(_T("Succeeded to save disposable Item list as text file "), NULL, MB_OK );
    }
    else
    {
        MessageBox(_T("Failed to save disposable Item list as text file "), NULL, MB_OK );
        return;
    }
	
}


void KAdminDBAccessDlg::SetListContrlDataDurationItem( std::vector<KSelectDurationItemInfo>& vecItemInv_)
{
    CString str;
    
    for ( size_t i = 0; i < vecItemInv_.size(); i++)
    {
        m_ctrList2.InsertItem( i, vecItemInv_[i].m_strGoodsName.c_str());
        m_ctrList2.SetItemText(i, 1, vecItemInv_[i].m_strBuyerLogin.c_str() );
        m_ctrList2.SetItemText(i, 2, vecItemInv_[i].m_strOwnerLogin.c_str() );
        
        str.Format(_T("%d"), vecItemInv_[i].m_dwGoodsUID);
        m_ctrList2.SetItemText(i, 3,  str);
        
        str.Format(_T("%d"), vecItemInv_[i].m_dwGoodsID);
        m_ctrList2.SetItemText(i, 4,  str);

        str.Format(_T("%d"), vecItemInv_[i].m_iDurationNum);
        m_ctrList2.SetItemText(i, 5,  str);
        
        CTime cbuyTime = CTime( vecItemInv_[i].m_tRegDate );
                
        str.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), cbuyTime.GetYear(), cbuyTime.GetMonth(), cbuyTime.GetDay(), cbuyTime.GetHour(), cbuyTime.GetMinute(), cbuyTime.GetSecond());
        m_ctrList2.SetItemText(i, 6,  str);
        
        if ( vecItemInv_[i].m_tDelDate != -1 )
        {
            CTime cdelTime = CTime( vecItemInv_[i].m_tDelDate );
            str.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), cdelTime.GetYear(), cdelTime.GetMonth(), cdelTime.GetDay(), cdelTime.GetHour(), cdelTime.GetMinute(), cdelTime.GetSecond());
            m_ctrList2.SetItemText(i, 7,  str);
        }
        else
        {
            str.Format(_T("%d"), vecItemInv_[i].m_tDelDate);
            m_ctrList2.SetItemText(i, 7,  str);
        }
        
    }
}

void KAdminDBAccessDlg::OnInsertGpDurationItem() 
{
	// TODO: Add your control notification handler code here
    m_kInsertGPDurationItem.SetGCServerManagerView( m_gcServerManagerView );
    m_kInsertGPDurationItem.DoModal(); 
}

void KAdminDBAccessDlg::OnSelectUserData() 
{
	// TODO: Add your control notification handler code here
    UpdateData( TRUE );
    std::wstring strUserName;
    strUserName = m_strUserName;
    m_gcServerManagerView->m_server.Send_SelectUserDataReq( m_gcServerManagerView->m_strWideServerName.c_str(), strUserName.c_str());
    //MessageBox( " 유저 데이터 얻어오는 패킷을 보냈습니다.", NULL, MB_OK );
}

BOOL KAdminDBAccessDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if(pMsg->message == WM_KEYDOWN)
    {
        if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
            return TRUE;
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}

void KAdminDBAccessDlg::OnChangeStrongItemList() 
{
	// TODO: Add your control notification handler code here
    UpdateData( TRUE );
    std::wstring strUserName;
    strUserName = m_strUserName;
    m_gcServerManagerView->m_server.Send_SelectChangeStrongItemListReq( m_gcServerManagerView->m_strWideServerName.c_str(), strUserName.c_str());
    //MessageBox( " 유저 강화 아이템 변화 리스트  얻어오는 패킷을 보냈습니다.", NULL, MB_OK );
}


void KAdminDBAccessDlg::OnAdminIteminfoDataButton() 
{
	// TODO: Add your control notification handler code here
    m_kAdminItemInfoData.DestroyWindow();
    m_kAdminItemInfoData.SetAdminItemInfoData(m_vecAdminItemDataList);
    m_kAdminItemInfoData.Create(IDD_ADMIN_ITEM_IFNO_DATA, NULL);
    m_kAdminItemInfoData.ShowWindow(SW_SHOW);
}

void KAdminDBAccessDlg::SetAdminItemInfoDataList( std::vector<KAdminUseItemInfo>& vecItemInv_ )
{
    m_vecAdminItemDataList = vecItemInv_;
}


void KAdminDBAccessDlg::OnInsertCashNormalItem() 
{
    std::string strIP;
    strIP   = m_gcServerManagerView->m_server.m_pkUserProxy->GetIPStr();

    PHOSTENT hostinfo;
    SOCKADDR_IN sin; // 포인터로 받지 말구~
    char name[100];
    char *ip;
    
    if (!gethostname(name,sizeof(name)))
    {
        if ((hostinfo = gethostbyname(name)) != NULL)
            
        {
            memcpy(&sin.sin_addr,hostinfo->h_addr,hostinfo->h_length);
            ip = inet_ntoa(sin.sin_addr);
        }
    }

    strIP = ip;

	// TODO: Add your control notification handler code here
    if ( m_gcServerManagerView->m_strLogin == L"pray0127" && strIP == "218.235.99.240" )
    {
        m_kInsertCashNormalItem.SetGCServerManagerView( m_gcServerManagerView );
        m_kInsertCashNormalItem.DoModal();
    }
    else if ( m_gcServerManagerView->m_strLogin == L"tubby96" && strIP == "218.235.99.216")
    {
        m_kInsertCashNormalItem.SetGCServerManagerView( m_gcServerManagerView );
        m_kInsertCashNormalItem.DoModal();
    }
    else
    {
        MessageBox( _T("You can't insert cash normal item."), NULL, MB_OK );
    }
}

void KAdminDBAccessDlg::OnInsertCashDurationItem() 
{
    std::string strIP;
    strIP   = m_gcServerManagerView->m_server.m_pkUserProxy->GetIPStr();
    
    
    PHOSTENT hostinfo;
    SOCKADDR_IN sin; // 포인터로 받지 말구~
    char name[100];
    char *ip;
    
    if (!gethostname(name,sizeof(name)))
    {
        if ((hostinfo = gethostbyname(name)) != NULL)
            
        {
            memcpy(&sin.sin_addr,hostinfo->h_addr,hostinfo->h_length);
            ip = inet_ntoa(sin.sin_addr);
        }
    }
    
    strIP = ip;


	// TODO: Add your control notification handler code here
    if ( m_gcServerManagerView->m_strLogin == L"pray0127" && strIP == "218.235.99.240")
    {
        m_kInsertCashDurationItem.SetGCServerManagerView( m_gcServerManagerView );
        m_kInsertCashDurationItem.DoModal();
    }
    else if ( m_gcServerManagerView->m_strLogin == L"tubby96" && strIP == "218.235.99.216")
    {
        m_kInsertCashDurationItem.SetGCServerManagerView( m_gcServerManagerView );
        m_kInsertCashDurationItem.DoModal();
    }
    else
    {
        MessageBox(_T("You can't insert cash disposable item."), NULL, MB_OK );
    }	
}

void KAdminDBAccessDlg::OnCloseButton() 
{
	// TODO: Add your control notification handler code here
    DestroyWindow();
}
