// KSubMissionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "define.h"
#include "GCMissionTool.h"
#include "KSubMissionDlg.h"
#include ".\ksubmissiondlg.h"
#include "KConditionDlg.h"
#include "GCMissionManager.h"
#include "GlobalFunc.h"
#include "KItemListDlg.h"

// KSubMissionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSubMissionDlg, CDialog)
KSubMissionDlg::KSubMissionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KSubMissionDlg::IDD, pParent)
{
    m_iSubMissionID = -1;
}

KSubMissionDlg::~KSubMissionDlg()
{
}

void KSubMissionDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_CONDITION, m_Conditions);
    DDX_Control(pDX, IDC_EDIT_A_COUNT, m_Action_Count);
    DDX_Control(pDX, IDC_EDIT_A_ITEM, m_Action_Item);
    DDX_Control(pDX, IDC_EDIT_C_STR, m_Completion);
    DDX_Control(pDX, IDC_EDIT_C_COUNT, m_C_Count);
    DDX_Control(pDX, IDC_EDIT_C_ITEM, m_C_Item);
    DDX_Control(pDX, IDC_CHECK_MONDROP, m_bMonDrop);
    DDX_Control(pDX, IDC_EDIT_SUB_DESC, m_edit_desc);
}


BEGIN_MESSAGE_MAP(KSubMissionDlg, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_CONDITION, OnLvnKeydownListCondition)
    ON_BN_CLICKED(IDC_BTN_A_ITEM, OnBnClickedBtnAItem)
    ON_BN_CLICKED(IDC_BTN_C_ITEM, OnBnClickedBtnCItem)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_CONDITION, OnNMDblclkListCondition)
END_MESSAGE_MAP()


// KSubMissionDlg 메시지 처리기입니다.
BOOL KSubMissionDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_Conditions.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    InitConditionList();
    RefreshControls();
    return TRUE;
}

void KSubMissionDlg::OnBnClickedButton1()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KConditionDlg kDiag;
    kDiag.DoModal();

    if( kDiag.GetConditionID() == -1 )
        return;

    for( int i = 0 ; i < (int)m_vecConditions.size() ; ++i )
    {
        if( m_vecConditions[i] == kDiag.GetConditionID() )
            return;
    }

    m_vecConditions.push_back( kDiag.GetConditionID() );
    RefreshConditionList();
}

void KSubMissionDlg::InitConditionList()
{
    //일단 리스트의 타이틀을 만들어 봅시다.
    WCHAR strColTitle[13][255] = { L"ID", L"Monster", L"GameMode", L"Stage" , L"Difficult", L"MapID", L"Score", L"ClearTime", L"PlayerKill", L"Die", L"ExLife", L"Win", L"Ratio" };
    int iColWidth[13] = { 30, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 };
    std::vector< std::wstring > vecTitleList;
    for( int i = 0 ; i < 13 ; ++i )
    {
        vecTitleList.push_back( strColTitle[i] );
    }    
    GC::SetListColumn( m_Conditions, vecTitleList, iColWidth, 13 );
}

void KSubMissionDlg::RefreshConditionList()
{
    //일단 리스트에 있는 내용들을 모두 지워줍시다.
    if( m_Conditions.DeleteAllItems() == 0 )
    {        
        OKBOXW( L"Can't Clear List!!" , L"Error" );
        return;
    }
    //리스트에 채워넣을 아이템이 있으면 채워 넣어야죠
    for( int i = 0 ; i < (int)m_vecConditions.size() ; ++i )
    {
        const SConditionInfo* pkCondition = SiKGCMissionManager()->GetConditionInfo( m_vecConditions[i] );
        if( pkCondition == NULL )
        {

            OKBOXW( L"존재하지 않는 조건 submission->condition : "<<m_iSubMissionID<<L"->"<<m_vecConditions[i],L"Error" );
            m_vecConditions.erase( m_vecConditions.begin() + i );
            --i;
            continue;
        }

        WCHAR strColText[MAX_PATH];
        LV_ITEM lvitem; 
        lvitem.mask=LVIF_TEXT; 
        lvitem.iItem=i; 
        lvitem.iSubItem=0; 
        _itow( pkCondition->m_dwID , strColText, 10 );
        lvitem.pszText= strColText; 
        m_Conditions.InsertItem(&lvitem); 

        ++lvitem.iSubItem; 
        _itow( pkCondition->m_iMonsterID , strColText, 10 );
        lvitem.pszText= strColText; 
        m_Conditions.SetItem(&lvitem);

        ++lvitem.iSubItem; 
        _itow( pkCondition->m_iGameMode , strColText, 10 );
        lvitem.pszText= strColText; 
        m_Conditions.SetItem(&lvitem);

        ++lvitem.iSubItem; 
        _itow( pkCondition->m_iStage , strColText, 10 );
        lvitem.pszText= strColText; 
        m_Conditions.SetItem(&lvitem);

        ++lvitem.iSubItem; 
        wsprintf( strColText, L"%d~%d", pkCondition->m_pairDifficultRange.first, pkCondition->m_pairDifficultRange.second );
        lvitem.pszText= strColText; 
        m_Conditions.SetItem(&lvitem);

        ++lvitem.iSubItem; 
        _itow( pkCondition->m_iMapID, strColText, 10 );
        lvitem.pszText= strColText; 
        m_Conditions.SetItem(&lvitem);

        ++lvitem.iSubItem; 
        wsprintf( strColText, L"%d~%d", pkCondition->m_pairScoreRange.first, pkCondition->m_pairScoreRange.second );
        lvitem.pszText= strColText; 
        m_Conditions.SetItem(&lvitem);

        ++lvitem.iSubItem; 
        _itow( pkCondition->m_iTime , strColText, 10 );
        lvitem.pszText= strColText; 
        m_Conditions.SetItem(&lvitem);

        ++lvitem.iSubItem; 
        _itow( pkCondition->m_iKill , strColText, 10 );
        lvitem.pszText= strColText; 
        m_Conditions.SetItem(&lvitem);

        ++lvitem.iSubItem; 
        _itow( pkCondition->m_iDie , strColText, 10 );
        lvitem.pszText= strColText; 
        m_Conditions.SetItem(&lvitem);

        ++lvitem.iSubItem; 
        _itow( pkCondition->m_iExLife , strColText, 10 );
        lvitem.pszText= strColText; 
        m_Conditions.SetItem(&lvitem);

        ++lvitem.iSubItem; 
        lvitem.pszText= (pkCondition->m_bWin?L"TRUE":L"FALSE" );
        m_Conditions.SetItem(&lvitem);

        ++lvitem.iSubItem; 
        swprintf( strColText, L"%f", pkCondition->m_fConditionRatio );
        lvitem.pszText= strColText; 
        m_Conditions.SetItem(&lvitem);
    }
}

void KSubMissionDlg::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_iSubMissionID = -1;
    OnCancel();
}

void KSubMissionDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnOK();
    SSubMissionInfo sInfo;
    if( m_bMonDrop.GetState() & 0x0003 )
        sInfo.m_sAction.m_bMonDrop = true;
    else
        sInfo.m_sAction.m_bMonDrop = false;

    int iTemp;
    CString strText;
    m_Action_Item.GetWindowText( strText );
    GC::GetIntegerFromString( strText , iTemp);
    sInfo.m_sAction.m_dwItemID = iTemp;
    m_Action_Count.GetWindowText( strText );
    GC::GetIntegerFromString( strText, sInfo.m_sAction.m_iCount );
    m_C_Count.GetWindowText( strText );
    GC::GetIntegerFromString( strText, sInfo.m_sCompletion.m_iCount  );
    m_C_Item.GetWindowText( strText );
    GC::GetIntegerFromString( strText, iTemp );
    sInfo.m_sCompletion.m_dwItemID = iTemp; 
    m_Completion.GetWindowText( strText );
    sInfo.m_iTitleID = SiKGCMissionManager()->AddString( strText.GetBuffer() );
    m_edit_desc.GetWindowText( strText );
    sInfo.m_iDescID = SiKGCMissionManager()->AddString( strText.GetBuffer() );
    sInfo.m_vecConditionID = m_vecConditions;

    m_iSubMissionID = SiKGCMissionManager()->AddSubMissionInfo( sInfo );
}

bool KSubMissionDlg::SetSubMissionID( int iSubMissionID )
{
    const SSubMissionInfo* sInfo = SiKGCMissionManager()->GetSubMission( iSubMissionID );

    if( sInfo == NULL )
        return false;

    m_iSubMissionID = iSubMissionID;    

    return true;
}

void KSubMissionDlg::RefreshControls()
{
    if( m_iSubMissionID == -1 )
        return;

    const SSubMissionInfo* sInfo = SiKGCMissionManager()->GetSubMission( m_iSubMissionID );

    if( sInfo == NULL )
    {
        return;
    }

    m_vecConditions.insert( m_vecConditions.end(), sInfo->m_vecConditionID.begin(), sInfo->m_vecConditionID.end() );    
    RefreshConditionList();

    m_Action_Count.SetWindowText( GC::IntToWchar( sInfo->m_sAction.m_iCount ).c_str() );
    m_Action_Item.SetWindowText( GC::IntToWchar( sInfo->m_sAction.m_dwItemID ).c_str() );
    m_Completion.SetWindowText( SiKGCMissionManager()->GetString( sInfo->m_iTitleID ).c_str() );
    m_edit_desc.SetWindowText( SiKGCMissionManager()->GetString( sInfo->m_iDescID ).c_str() );
    m_C_Item.SetWindowText( GC::IntToWchar( sInfo->m_sCompletion.m_dwItemID ).c_str() );
    m_C_Count.SetWindowText( GC::IntToWchar( sInfo->m_sCompletion.m_iCount ).c_str() );
    m_bMonDrop.SetState( (sInfo->m_sAction.m_bMonDrop?TRUE:FALSE) );
    
    Invalidate();
}
void KSubMissionDlg::OnLvnKeydownListCondition(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    switch( pLVKeyDow->wVKey )
    {
    case VK_DELETE:
        int iSelected = m_Conditions.GetNextItem( -1, LVNI_SELECTED );
        if( iSelected != -1 )
        {
            m_vecConditions.erase( m_vecConditions.begin() + iSelected );
        }
        RefreshConditionList();
        break;
    }
    *pResult = 0;
}

void KSubMissionDlg::OnBnClickedBtnAItem()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    KItemListDlg dlg;
    dlg.DoModal();
    dlg.FindItemAndSetEditBOX( m_Action_Item );
}

void KSubMissionDlg::OnBnClickedBtnCItem()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.KItemListDlg dlg;
    KItemListDlg dlg;
    dlg.DoModal();
    dlg.FindItemAndSetEditBOX( m_C_Item );
}

void KSubMissionDlg::OnNMDblclkListCondition(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.i
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if( pNMListView->iItem == -1 )
        return;
    KConditionDlg dlg;
    if( pNMListView->iItem >= 0 && pNMListView->iItem < (int) m_vecConditions.size() )
        dlg.SetContidionID( m_vecConditions[pNMListView->iItem] );
    INT_PTR result = dlg.DoModal();

    if( dlg.GetConditionID() == -1 )
        return;

    for( int i = 0 ; i < (int)m_vecConditions.size() ; ++i )
    {
        if( m_vecConditions[i] == dlg.GetConditionID() )
            return;
    }

    m_vecConditions.erase( m_vecConditions.begin() + pNMListView->iItem );

    m_vecConditions.push_back( dlg.GetConditionID() );
    RefreshConditionList();

    *pResult = 0;
}
