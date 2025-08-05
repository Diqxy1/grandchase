// GCMissionToolDlg.cpp : 구현 파일
//

#include "stdafx.h"
//#include <string>
#include "GCMissionTool.h"
#include "GCMissionToolDlg.h"
#include ".\gcmissiontooldlg.h"
#include "GCMissionManager.h"
#include "GCMissionInSort.h"
#include "KMissionEditDlg.h"
#include "define.h"
#include "GlobalFunc.h"
#include "KGCMissionTokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

// 구현
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGCMissionToolDlg 대화 상자



CGCMissionToolDlg::CGCMissionToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGCMissionToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGCMissionToolDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO1,    sortCharKind);
    DDX_Control(pDX, IDC_COMBO2,    sortMissionKind);
    DDX_Control(pDX, IDC_EDIT1,     sortStatic);
    DDX_Control(pDX, IDC_LIST2, kMissionList);
}

BEGIN_MESSAGE_MAP(CGCMissionToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, OnLvnItemchangedList2)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnNMDblclkList2)
    ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedBtnSave)
    ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
//    ON_CBN_EDITCHANGE(IDC_COMBO1, OnCbnEditchangeCombo1)
//ON_CBN_EDITUPDATE(IDC_COMBO1, OnCbnEditupdateCombo1)
ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
ON_BN_CLICKED(IDC_LOAD_EXCEL, OnBnClickedLoadExcel)
END_MESSAGE_MAP()


// CGCMissionToolDlg 메시지 처리기

BOOL CGCMissionToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
    kMissionList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    InitListControl();
    InitComboBox();
	
	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

void CGCMissionToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CGCMissionToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CGCMissionToolDlg::InitListControl()
{
    //일단 리스트의 타이틀을 만들어 봅시다.
    WCHAR strColTitle[4][255] = { L"MissionID", L"Title", L"Char", L"Flag" };
    int iColWidth[4] = { 80, 250, 80, 30 };

    LV_COLUMNW lvcolumn;
    for( int i = 0 ; i < 4 ; ++i )
    {
        lvcolumn.mask= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH; 

        lvcolumn.fmt = LVCFMT_LEFT; 

        lvcolumn.pszText=strColTitle[i]; 

        lvcolumn.iSubItem=i; 

        lvcolumn.cx=iColWidth[i]; 

        kMissionList.InsertColumn(i,&lvcolumn); 

    }
}

void CGCMissionToolDlg::RefreshList()
{
    //일단 리스트에 있는 내용들을 모두 지워줍시다.
    if( kMissionList.DeleteAllItems() == 0 )
    {
        OKBOXW( L"Can't Clear List!!" , L"Error" );
    }
    //리스트에 채워넣을 아이템이 있으면 채워 넣어야죠
    int iMissionNum = SiKGCMissionInSort()->GetMissionNum();
    int iItemCount = 0;
    if( iMissionNum > 0 )
    {
        for( int i = 0 ; i < iMissionNum ; ++i )
        {
            const GCItemData* pkItem = SiKGCMissionInSort()->GetMissionItem(i);
            if( pkItem == NULL )
                continue;

            if( m_strFindText.empty() == false )
            {
                std::wstring strItemID = GC::IntToWchar( pkItem->dwGoodsID );
                std::wstring strItemName = pkItem->szItemName;
                if( (strItemID.find( m_strFindText ) == strItemID.npos) && 
                    ( strItemName.find( m_strFindText ) == strItemName.npos ) )
                {
                    continue;
                }
            }

            if( sortCharKind.GetCurSel() != sortCharKind.GetCount() - 1 )
            {
                DWORD sortChar = 1<<sortCharKind.GetCurSel();
                if( (pkItem->dwCharType & sortChar) == false )
                {
                    continue;
                }
            }

            WCHAR strColText[MAX_PATH];
            LV_ITEM lvitem; 
            lvitem.mask=LVIF_TEXT; 
            lvitem.iItem=iItemCount; 
            lvitem.iSubItem=0; 
            _itow( pkItem->dwGoodsID , strColText, 10 );
            lvitem.pszText= strColText; 
            kMissionList.InsertItem(&lvitem); 

            lvitem.mask=LVIF_TEXT; 
            lvitem.iItem=iItemCount; 
            lvitem.iSubItem=1; 
            lvitem.pszText= (LPWSTR)(LPCWSTR)pkItem->szItemName; 
            kMissionList.SetItem(&lvitem); 

            lvitem.mask=LVIF_TEXT; 
            lvitem.iItem=iItemCount; 
            lvitem.iSubItem=2; 
            _itow( pkItem->dwCharType , strColText, 10 );
            lvitem.pszText= strColText; 
            kMissionList.SetItem(&lvitem);

            KGCMission* pkMission = SiKGCMissionManager()->GetMissionWithID( pkItem->dwGoodsID );
            if( pkMission == NULL )
            {
                lvitem.mask=LVIF_TEXT; 
                lvitem.iItem=iItemCount; 
                lvitem.iSubItem=3;                 
                lvitem.pszText= L"!!"; 
                kMissionList.SetItem(&lvitem);
            }

            ++iItemCount;
        }
    }
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR CGCMissionToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGCMissionToolDlg::OnBnClickedButton2()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    static CString strFileName;

    CString strFilter = L"All Script Files (*.lua, *stg)|*.lua;*.stg|";

    CFileDialog dlg(TRUE,NULL,strFileName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, strFilter);

    if( dlg.DoModal() != IDOK )
        return;

    POSITION pos = dlg.GetStartPosition();

    while(pos != NULL)
    {
        CString strSelectedFileName = dlg.GetNextPathName(pos);        
        WCHAR* strTemp = strSelectedFileName.GetBuffer();
        char strTrans[MAX_PATH];
        W2T2( strTrans, strTemp );
        std::string strFileName( strlwr( strTrans ) );
        if( strFileName.find( "sort" ) != strFileName.npos )
        {
            SiKGCMissionInSort()->LoadSortFile( strFileName );
        }
        else if( strFileName.find( "mission" ) != strFileName.npos && strFileName.find( "str" ) != strFileName.npos )
        {
            SiKGCMissionManager()->LoadMissionString( strFileName , NULL, true );
        }
        else if( strFileName.find( "mission" ) != strFileName.npos && strFileName.find( "init" ) != strFileName.npos )
        {
            SiKGCMissionManager()->LoadMissionFile( strFileName );
        }
    }

    RefreshList();
}

void CGCMissionToolDlg::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    *pResult = 0;
}

void CGCMissionToolDlg::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if( pNMListView->iItem == -1 )
        return;
    KMissionEditDlg dlg;        
    dlg.SetMissionID( SiKGCMissionInSort()->GetMissionItem( pNMListView->iItem )->dwGoodsID );
    dlg.DoModal();
    *pResult = 0;
    RefreshList();
}

void CGCMissionToolDlg::OnBnClickedBtnSave()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    //CString strFilter = "Script 파일(*.stg)|*.stg|";
    CString strFilter = L"*.*|*.*|";
    char strPath[MAX_PATH];

    OKBOXW( L"미션 스크립트를 저장합니다.", L"저장" );
    CFileDialog MissionScriptdlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
    if( MissionScriptdlg.DoModal() != IDOK )
        return;

    CString FilePath = MissionScriptdlg.GetPathName();

    if( FilePath.Right(4).CompareNoCase(L".lua") )
        FilePath += L".lua";

    WideCharToMultiByte( CP_ACP, 0, FilePath.GetBuffer(), -1, strPath, MAX_PATH, NULL, NULL);
    SiKGCMissionManager()->OutPutMissionFile( strPath );

    OKBOXW( L"미션 스트링을 저장합니다.", L"저장" );

    CFileDialog MissionStringdlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
    if( MissionStringdlg.DoModal() != IDOK )
        return;

    FilePath = MissionStringdlg.GetPathName();    
    if( FilePath.Right(4).CompareNoCase(L".stg") )
        FilePath += L".stg";

    WideCharToMultiByte( CP_ACP, 0, FilePath.GetBuffer(), -1, strPath, MAX_PATH, NULL, NULL);
    SiKGCMissionManager()->OutPutMissionString( strPath );
}

void CGCMissionToolDlg::InitComboBox()
{
    GC::FillComboBoxFromINI( sortCharKind, "Character", "Char%d" );
    sortCharKind.SetCurSel( sortCharKind.AddString( L"모두" ) );
    GC::FillComboBoxFromINI( sortMissionKind, "MissionKind", "MK%d" );    
    sortMissionKind.SetCurSel( sortMissionKind.AddString( L"모두" ) );
}
void CGCMissionToolDlg::OnEnChangeEdit1()
{
    // TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
    // CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
    // CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
    // 이 알림을 보내지 않습니다.

    // TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString strText;
    sortStatic.GetWindowText( strText );
    m_strFindText = strText.GetBuffer();

    RefreshList();
}

//void CGCMissionToolDlg::OnCbnEditchangeCombo1()
//{
//    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//    RefreshList();
//}

//void CGCMissionToolDlg::OnCbnEditupdateCombo1()
//{
//    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//    RefreshList();
//}

void CGCMissionToolDlg::OnCbnSelchangeCombo1()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    RefreshList();
}

void CGCMissionToolDlg::OnBnClickedLoadExcel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    static CString strFileName;

    CString strFilter = L"All Files (*.*)|*.*|";

    CFileDialog dlg(TRUE,NULL,strFileName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, strFilter);

    if( dlg.DoModal() != IDOK )
        return;

    POSITION pos = dlg.GetStartPosition();

    while(pos != NULL)
    {
        CString strSelectedFileName = dlg.GetNextPathName(pos);        
        WCHAR* strTemp = strSelectedFileName.GetBuffer();
        char strTrans[MAX_PATH];
        W2T2( strTrans, strTemp );
        std::string strFileName( strlwr( strTrans ) );
        if( strFileName.find( "sub" ) != strFileName.npos )
        {
			AddSubMissions( strFileName );
        }
        else
        {
            AddMissions( strFileName );
        }
    }
}

void CGCMissionToolDlg::AddMissions( std::string strFileName )
{
    KGCMissionTokenizer token;
    token.LoadFile( strFileName );

    for( int i = 0 ; ; ++i )
    {
        if( token[i] == NULL )
            break;

        SMissionInfo sInfo;
        sInfo.m_dwID = token.GetInteger(i);
        sInfo.m_iTitleID = SiKGCMissionManager()->AddString( token.GetString(i) );
        sInfo.m_iDescID = SiKGCMissionManager()->AddString( token.GetString(i) );
        sInfo.m_iCharacter = token.GetInteger(i) - 1;   //엘리시스를 1부터 넣어버렸다.
        int iBeginLevel = token.GetInteger(i);
        int iEndLevel = token.GetInteger(i);
        sInfo.m_pairLevelLimit = std::make_pair( iBeginLevel, iEndLevel );
        sInfo.m_iPromotion = token.GetInteger(i);
        sInfo.m_iPeriod = token.GetInteger(i);
        sInfo.m_iGP = token.GetInteger(i);
        sInfo.m_iExp = token.GetInteger(i);
        sInfo.m_iChangeJob = token.GetInteger(i);
        sInfo.m_iNewCharacter = token.GetInteger(i);
        SiKGCMissionManager()->AddMissionInfo( sInfo );
    }
    RefreshList();
}

void CGCMissionToolDlg::AddSubMissions( std::string strFileName )
{
    KGCMissionTokenizer token;
    token.LoadFile( strFileName );

    for( int i = 0 ; ; ++i )
    {
        if( token[i] == NULL )
            break;       

        SSubMissionInfo sInfo;
        DWORD dwMissionID = token.GetInteger(i);

        const SMissionInfo* pMission = SiKGCMissionManager()->GetMissionInfo( dwMissionID );
        SMissionInfo sMission = *pMission;
        if( pMission == NULL )
            continue;

        sInfo.m_iTitleID = SiKGCMissionManager()->AddString( token.GetString(i) );
        sInfo.m_iDescID = SiKGCMissionManager()->AddString( token.GetString(i) );
        sInfo.m_sAction.m_iCount = token.GetInteger(i);
        sInfo.m_sAction.m_dwItemID = token.GetInteger(i);
        sInfo.m_sAction.m_bMonDrop = token.GetInteger(i);
        sInfo.m_sCompletion.m_iCount = token.GetInteger(i);
        sInfo.m_sCompletion.m_dwItemID = token.GetInteger(i);

        int iSubMissionID = SiKGCMissionManager()->AddSubMissionInfo( sInfo );
        sMission.m_vecSubMissions.push_back( iSubMissionID );
        SiKGCMissionManager()->UpdateMissionInfo( sMission );
    }
}