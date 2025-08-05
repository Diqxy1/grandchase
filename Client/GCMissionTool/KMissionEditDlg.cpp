// KMissionEditDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
//#include <string>
#include "GCMissionTool.h"
#include "KMissionEditDlg.h"
#include ".\kmissioneditdlg.h"
#include "GCMissionManager.h"
//#include <boost/shared_ptr.hpp>
#include "define.h"
#include "GlobalFunc.h"
#include "KSubMissionDlg.h"
#include "KMissionRewardList.h"
#include "GCMissionInSort.h"
#include "KItemListDlg.h"

// KMissionEditDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMissionEditDlg, CDialog)
KMissionEditDlg::KMissionEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KMissionEditDlg::IDD, pParent)
{
    m_bAdd = false;
}

KMissionEditDlg::~KMissionEditDlg()
{
}

void KMissionEditDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_INHERIT,          m_edit_Inherit      );
    DDX_Control(pDX, IDC_EDIT_LEVEL,            m_edit_Level        );
    DDX_Control(pDX, IDC_EDIT_PROMOTION,        m_edit_Promotion    );
    DDX_Control(pDX, IDC_EDIT_TIME,             m_edit_Time         );
    DDX_Control(pDX, IDC_EDIT_TARGET,           m_edit_TargetMission);
    DDX_Control(pDX, IDC_EDIT_OVERLAP,          m_edit_Overlap      );
    DDX_Control(pDX, IDC_EDIT_TITLE,            m_edit_Title        );
    DDX_Control(pDX, IDC_EDIT_DESC,             m_edit_Desc         );
    DDX_Control(pDX, IDC_LIST_SUBMISSION,       m_list_Submission   );
    DDX_Control(pDX, IDC_EDIT_GP,               m_edit_GP           );
    DDX_Control(pDX, IDC_EDIT_EXP,              m_edit_Exp          );
    DDX_Control(pDX, IDC_LIST_REWARD,           m_list_Reward       );
    DDX_Control(pDX, IDC_LIST_SELECTREWARD,     m_list_SelReward    );
    DDX_Control(pDX, IDC_EDIT_REWARDPROMOTION,  m_edit_NewPromotion );
    DDX_Control(pDX, IDC_LIST1,                 m_MissionID         );
    DDX_Control(pDX, IDC_COMBO2, m_combo_Character);
    DDX_Control(pDX, IDC_COMBO1, m_combo_NewChar);
}


BEGIN_MESSAGE_MAP(KMissionEditDlg, CDialog)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_WM_CLOSE()
    ON_WM_SYSCOMMAND()
    ON_BN_CLICKED(IDC_BUTTON_ADDSUBMISSION, OnBnClickedButtonAddsubmission)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_SUBMISSION, OnNMDblclkListSubmission)
    ON_BN_CLICKED(IDC_BUTTON_ADDITEM, OnBnClickedButtonAdditem)
    ON_BN_CLICKED(IDC_BUTTON_ADDITEM2, OnBnClickedButtonAdditem2)
    ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_REWARD, OnLvnKeydownListReward)
    ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_SELECTREWARD, OnLvnKeydownListSelectreward)
    ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_SUBMISSION, OnLvnKeydownListSubmission)
    ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
    ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BTN_CMISSION, OnBnClickedBtnCmission)
    ON_BN_CLICKED(IDC_BTN_IMISSION, OnBnClickedBtnImission)
END_MESSAGE_MAP()


// KMissionEditDlg 메시지 처리기입니다.

BOOL KMissionEditDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    //변수 초기화
    m_bAdd = false;
    m_vecSubMissionID.clear();
    m_vecReward.clear();
    m_vecSelReward.clear();
    m_list_Reward.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_list_SelReward.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_list_Submission.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    GC::FillComboBoxFromINI( m_combo_Character, "Character", "Char%d" );
    m_combo_Character.SetCurSel( 0 );
    GC::FillComboBoxFromINI( m_combo_NewChar, "NewChar", "Char%d" );
    m_combo_NewChar.SetCurSel( m_combo_NewChar.AddString( L"없음" ) );

    WCHAR strColText[MAX_PATH] = {0, };
    _itow( m_dwMissionID , strColText, 10 );
    m_MissionID.AddString( strColText );    
    InitSubMissionList();
    InitRewardList( m_list_Reward );
    InitRewardList( m_list_SelReward );
    SetMissionData();
    return TRUE;
}

int KMissionEditDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  여기에 특수화된 작성 코드를 추가합니다.

    return 0;
}

std::wstring DWORDVectorToStringW( std::vector<DWORD> arg , wchar_t* tok )
{
    std::wstring str;
    for( int i = 0 ; i < (int) arg.size() ; ++i )
    {
        wchar_t strNum[MAX_PATH];
        str += _itow( arg[i], strNum, 10 );
        str += tok;
    }

    return str;
}

void KMissionEditDlg::SetMissionData( DWORD dwMissionID )
{
    if( dwMissionID == 0 )
        dwMissionID = m_dwMissionID;

    KGCMission* pkMission = SiKGCMissionManager()->GetMissionWithID(  dwMissionID  ) ;
    if( pkMission == NULL )
    {
        m_bAdd = true;
        return;
    }

    const SMissionInfo& sMissionInfo = *pkMission->GetMissionInfo();
    
    WCHAR strText[MAX_PATH] = {0, };
    swprintf( strText, L"%d~%d", sMissionInfo.m_pairLevelLimit.first, sMissionInfo.m_pairLevelLimit.second );
    m_edit_Level.SetWindowText( strText );

    m_edit_Promotion.SetWindowText( GC::IntToWchar( sMissionInfo.m_iPromotion ).c_str() ); 
    m_edit_Time.SetWindowText( GC::IntToWchar( sMissionInfo.m_iPeriod ).c_str() );
    m_edit_TargetMission.SetWindowText( DWORDVectorToStringW( sMissionInfo.m_vecInvalidCompletMissions, L"," ).c_str());
    m_edit_Overlap.SetWindowText( DWORDVectorToStringW( sMissionInfo.m_vecInvalidMissions, L"," ).c_str());
    m_edit_Title.SetWindowText( SiKGCMissionManager()->GetString( sMissionInfo.m_iTitleID ).c_str() );
    m_edit_Desc.SetWindowText( SiKGCMissionManager()->GetString( sMissionInfo.m_iDescID ).c_str() );
    m_edit_GP.SetWindowText( GC::IntToWchar( sMissionInfo.m_iGP).c_str() );
    m_edit_Exp.SetWindowText( GC::IntToWchar( sMissionInfo.m_iExp).c_str() );
    m_edit_NewPromotion.SetWindowText( GC::IntToWchar( sMissionInfo.m_iChangeJob ).c_str() );    

    m_combo_Character.SetCurSel( sMissionInfo.m_iCharacter );
    m_combo_NewChar.SetCurSel( sMissionInfo.m_iNewCharacter - 3 );

    m_vecSubMissionID = sMissionInfo.m_vecSubMissions;
    m_vecReward = sMissionInfo.m_vecRewardItem;
    m_vecSelReward = sMissionInfo.m_vecSelectItem;
    AddRewardItem( m_list_Reward, m_vecReward );
    AddRewardItem( m_list_SelReward, m_vecSelReward );
    RefreshSubMissionList();

}

void KMissionEditDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    SMissionInfo sInfo;
    CString strText;
    m_edit_Level.GetWindowText( strText );
    GC::GetPairDataFromString( strText, sInfo.m_pairLevelLimit );
    m_edit_Promotion.GetWindowText( strText );
    GC::GetIntegerFromString( strText, sInfo.m_iPromotion );    
    m_edit_Time.GetWindowText( strText );
    GC::GetIntegerFromString( strText, sInfo.m_iPeriod );
    m_edit_TargetMission.GetWindowText( strText );
    GC::GetIntListFromString( strText, sInfo.m_vecInvalidCompletMissions, L"," );
    m_edit_Overlap .GetWindowText( strText );
    GC::GetIntListFromString( strText, sInfo.m_vecInvalidMissions, L"," );
    m_edit_Title.GetWindowText( strText ) ;
    sInfo.m_iTitleID = SiKGCMissionManager()->AddString( strText.GetBuffer() );
    m_edit_Desc.GetWindowText( strText ) ;    
    sInfo.m_iDescID = SiKGCMissionManager()->AddString( strText.GetBuffer() );
    m_edit_GP.GetWindowText( strText ) ;
    GC::GetIntegerFromString( strText, sInfo.m_iGP );
    m_edit_Exp.GetWindowText( strText ) ;
    GC::GetIntegerFromString( strText, sInfo.m_iExp );
    m_edit_NewPromotion.GetWindowText( strText ) ;
    GC::GetIntegerFromString( strText, sInfo.m_iChangeJob );
    sInfo.m_dwID = m_dwMissionID;

    sInfo.m_iCharacter = m_combo_Character.GetCurSel();
    sInfo.m_iNewCharacter = ( m_combo_NewChar.GetCurSel() == m_combo_NewChar.GetCount() - 1 ? -1 : m_combo_NewChar.GetCurSel() + 3);

    sInfo.m_vecSubMissions = m_vecSubMissionID;
    sInfo.m_vecRewardItem = m_vecReward;
    sInfo.m_vecSelectItem = m_vecSelReward;

    if( SiKGCMissionManager()->AddMissionInfo( sInfo ) == false )
    {
        SiKGCMissionManager()->UpdateMissionInfo( sInfo );
    }

    OnOK();
}

void KMissionEditDlg::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnCancel();
}

void KMissionEditDlg::OnClose()
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

    CDialog::OnClose();
}

void KMissionEditDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    CDialog::OnSysCommand(nID, lParam);
}

void KMissionEditDlg::InitSubMissionList()
{
    //일단 리스트의 타이틀을 만들어 봅시다.
    std::vector< std::wstring > vecTitleList;
    vecTitleList.push_back( L"ID");
    vecTitleList.push_back( L"Title" );
    vecTitleList.push_back( L"Count" );
    vecTitleList.push_back( L"Item" );

    int iColWidth[4] = { 30, 200, 50, 50 };
    GC::SetListColumn( m_list_Submission, vecTitleList, iColWidth, 4 );
}

void KMissionEditDlg::InitRewardList( CListCtrl& kList )
{
    //일단 리스트의 타이틀을 만들어 봅시다.
    WCHAR strColTitle[7][255] = { L"ID", L"Name", L"Type", L"Count", L"CashType", L"Char", L"Level" };
    int iColWidth[7] = { 50, 200, 50, 50, 50, 50, 50 };
    std::vector< std::wstring > vecTitleList;
    for( int i = 0 ; i < 7 ; ++i )
    {
        vecTitleList.push_back( strColTitle[i] );
    }       
    GC::SetListColumn( kList, vecTitleList, iColWidth, 7 );
}

void KMissionEditDlg::OnBnClickedButtonAddsubmission()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KSubMissionDlg kDiag;
    kDiag.DoModal();

    if( kDiag.m_iSubMissionID != -1 )
    {
        m_vecSubMissionID.push_back( kDiag.m_iSubMissionID );
    }

    RefreshSubMissionList();
}

void KMissionEditDlg::RefreshSubMissionList()
{
    m_list_Submission;

    //일단 리스트에 있는 내용들을 모두 지워줍시다.
    if( m_list_Submission.DeleteAllItems() == 0 )
    {
        OKBOXW( L"Can't Clear List!!" , L"Error" );
        return;
    }
    //리스트에 채워넣을 아이템이 있으면 채워 넣어야죠
    for( int i = 0 ; i < (int)m_vecSubMissionID.size() ; ++i )
    {
        const SSubMissionInfo* pkSubMission = SiKGCMissionManager()->GetSubMission( m_vecSubMissionID[i] );
        if( pkSubMission == NULL )
        {
            m_vecSubMissionID.erase( m_vecSubMissionID.begin() + i );
            --i;
            continue;
        }

        WCHAR strColText[MAX_PATH];
        LV_ITEM lvitem; 
        lvitem.mask=LVIF_TEXT; 
        lvitem.iItem=i; 
        lvitem.iSubItem=0; 
        _itow( pkSubMission->m_iSubMissionID , strColText, 10 );
        lvitem.pszText= strColText; 
        m_list_Submission.InsertItem(&lvitem); 

        lvitem.iSubItem=1; 
        lvitem.pszText = (LPWSTR)(LPCWSTR)SiKGCMissionManager()->GetString( pkSubMission->m_iDescID ).c_str();
        m_list_Submission.SetItem(&lvitem); 

        lvitem.iSubItem=2; 
        _itow( pkSubMission->m_sCompletion.m_iCount , strColText, 10 );
        lvitem.pszText= strColText; 
        m_list_Submission.SetItem(&lvitem);

        lvitem.iSubItem=3; 
        _itow( pkSubMission->m_sCompletion.m_dwItemID , strColText, 10 );
        lvitem.pszText= strColText; 
        m_list_Submission.SetItem(&lvitem);
    }
}
void KMissionEditDlg::OnNMDblclkListSubmission(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.i
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if( pNMListView->iItem == -1 )
        return;
    KSubMissionDlg dlg;
    if( pNMListView->iItem >= 0 && pNMListView->iItem < (int) m_vecSubMissionID.size() )
        dlg.SetSubMissionID( m_vecSubMissionID[pNMListView->iItem] );
    INT_PTR result = dlg.DoModal();

    if( result != IDOK )
        return;

    if( dlg.m_iSubMissionID == -1 )
        return;

    m_vecSubMissionID.erase( m_vecSubMissionID.begin() + pNMListView->iItem );
    m_vecSubMissionID.push_back( dlg.m_iSubMissionID );

    RefreshSubMissionList();

    *pResult = 0;
}

void KMissionEditDlg::OnBnClickedButtonAdditem()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KMissionRewardList dlg;    
    dlg.DoModal();

    if( dlg.m_dwItemID == -1 )
        return;

    if( dlg.m_iCount == 0 )
        return;

    m_vecReward.push_back( std::make_pair( dlg.m_dwItemID, dlg.m_iCount ) );
    AddRewardItem( m_list_Reward, m_vecReward );
}

void KMissionEditDlg::OnBnClickedButtonAdditem2()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KMissionRewardList dlg;
    dlg.DoModal();

    if( dlg.m_dwItemID == -1 )
        return;

    if( dlg.m_iCount == 0 )
        return;

    m_vecSelReward.push_back( std::make_pair( dlg.m_dwItemID, dlg.m_iCount ) );
    AddRewardItem( m_list_SelReward, m_vecSelReward );
}

void KMissionEditDlg::AddRewardItem( CListCtrl& klist, std::vector< std::pair< DWORD, int > >& vecRewards )
{
    //일단 리스트에 있는 내용들을 모두 지워줍시다.
    if( klist.DeleteAllItems() == 0 )
    {
        OKBOXW( L"Can't Clear List!!" , L"Error" );
        return;
    }
    //리스트에 채워넣을 아이템이 있으면 채워 넣어야죠
    for( int i = 0 ; i < (int)vecRewards.size() ; ++i )
    {
        const GCItemData* kItem = SiKGCMissionInSort()->GetItemData( vecRewards[i].first );

        if( kItem == NULL )
        {
            OKBOXW( L"Sort에 없는 아이템입니다."<<kItem->dwGoodsID , L"에러" );
            vecRewards.erase( vecRewards.begin() + i );
            --i;
            continue;
        }

        WCHAR strColText[MAX_PATH];
        LV_ITEM lvitem; 
        lvitem.mask=LVIF_TEXT; 
        lvitem.iItem=i; 
        lvitem.iSubItem=0; 
        _itow( kItem->dwGoodsID , strColText, 10 );
        lvitem.pszText= strColText; 
        klist.InsertItem(&lvitem); 

        lvitem.iSubItem=1; 
        lvitem.pszText = (LPWSTR)(LPCWSTR)kItem->szItemName;
        klist.SetItem(&lvitem); 

        lvitem.iSubItem= 2;  
        lvitem.pszText= (kItem->eItemType == 1?L"기간":L"수량" );
        klist.SetItem(&lvitem);

        lvitem.iSubItem=3; 
        _itow( vecRewards[i].second , strColText, 10 );
        lvitem.pszText= strColText; 
        klist.SetItem(&lvitem);

        lvitem.mask=LVIF_TEXT; 
        lvitem.iSubItem= 4;  
        lvitem.pszText= (kItem->eMoneyType == 1 ? L"캐쉬" : L"GP");
        klist.SetItem(&lvitem);

        lvitem.mask=LVIF_TEXT; 
        lvitem.iSubItem= 5;  
        _itow( (int)kItem->dwCharType , strColText, 10 );
        lvitem.pszText= strColText;
        klist.SetItem(&lvitem);

        lvitem.mask=LVIF_TEXT; 
        lvitem.iSubItem= 6;  
        _itow( kItem->iNeedLevelForEquippingItem , strColText, 10 );

        klist.SetItem(&lvitem);
    }
}

namespace{
    template< class T >
    void DeleteItem( CListCtrl& klist, T& vecList )
    {
        int iSelected = klist.GetNextItem( -1, LVNI_SELECTED );
        if( iSelected != -1 )
        {
            vecList.erase( vecList.begin() + iSelected );
        }
    }
}
void KMissionEditDlg::OnLvnKeydownListReward(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    switch( pLVKeyDow->wVKey )
    {
    case VK_DELETE:
        DeleteItem( m_list_Reward, m_vecReward );
        AddRewardItem( m_list_Reward, m_vecReward );
        break;
    }
    *pResult = 0;
}

void KMissionEditDlg::OnLvnKeydownListSelectreward(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    switch( pLVKeyDow->wVKey )
    {
    case VK_DELETE:
        DeleteItem( m_list_SelReward, m_vecSelReward );
        AddRewardItem( m_list_SelReward, m_vecSelReward );
        break;
    }
    *pResult = 0;
}

void KMissionEditDlg::OnLvnKeydownListSubmission(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    switch( pLVKeyDow->wVKey )
    {
    case VK_DELETE:
        DeleteItem( m_list_Submission, m_vecSubMissionID );
        RefreshSubMissionList();
        break;
    }
    *pResult = 0;
}

void KMissionEditDlg::OnBnClickedButton6()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString strText;
    m_edit_Inherit.GetWindowText( strText );
    int iMissionID;
    GC::GetIntegerFromString( strText, iMissionID );
    SetMissionData( iMissionID );
}

void KMissionEditDlg::OnBnClickedButton1()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KItemListDlg dlg;
    dlg.SetListType( true );
    dlg.DoModal();
    dlg.FindItemAndSetEditBOX( m_edit_Inherit );
}

void KMissionEditDlg::OnBnClickedBtnCmission()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KItemListDlg dlg;
    dlg.SetListType( true );
    dlg.DoModal();

    if( dlg.m_dwItemID == 0 )
        return;

    CString strText;
    m_edit_TargetMission.GetWindowText( strText );
    strText += GC::IntToWchar( dlg.m_dwItemID ).c_str();
    strText += L",";
    m_edit_TargetMission.SetWindowText( strText );
    
}

void KMissionEditDlg::OnBnClickedBtnImission()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KItemListDlg dlg;
    dlg.SetListType( true );
    dlg.DoModal();

    if( dlg.m_dwItemID == 0 )
        return;

    CString strText;
    m_edit_Overlap.GetWindowText( strText );
    strText += GC::IntToWchar( dlg.m_dwItemID ).c_str();
    strText += L",";
    m_edit_Overlap.SetWindowText( strText );
}
