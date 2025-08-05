// KItemListDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GCMissionTool.h"
#include "KItemListDlg.h"
#include ".\kitemlistdlg.h"
#include "GlobalFunc.h"
#include "define.h"


// KItemListDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KItemListDlg, CDialog)
KItemListDlg::KItemListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KItemListDlg::IDD, pParent)
    , m_bIsMissionList( false )
{
    m_dwItemID = 0;
}

KItemListDlg::~KItemListDlg()
{
}

void KItemListDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_list_Items);
    DDX_Control(pDX, IDC_EDIT1, m_edit_Selected);
    DDX_Control(pDX, IDC_EDIT_FIND, m_edit_Find);
}

BOOL KItemListDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_list_Items.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    InitItemList();
    RefreshItemList();

    return TRUE;
}

BEGIN_MESSAGE_MAP(KItemListDlg, CDialog)
//    ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
ON_EN_CHANGE(IDC_EDIT_FIND, OnEnChangeEditFind)
END_MESSAGE_MAP()


// KItemListDlg 메시지 처리기입니다.

//void KItemListDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//    *pResult = 0;
//}

void KItemListDlg::InitItemList()
{
    //일단 리스트의 타이틀을 만들어 봅시다.
    WCHAR strColTitle[7][255] = { L"GoodsID", L"ItemName", L"Character", L"level", L"type", L"CashType", L"Desc" };
    int iColWidth[7] = { 50, 200, 50, 50, 50, 50, 200 };
    std::vector< std::wstring > vecTitleList;
    for( int i = 0 ; i < 7 ; ++i )
    {
        vecTitleList.push_back( strColTitle[i] );
    }    
    GC::SetListColumn( m_list_Items, vecTitleList, iColWidth, 7 );
}

void KItemListDlg::RefreshItemList()
{
    //일단 리스트에 있는 내용들을 모두 지워줍시다.
    if( m_list_Items.DeleteAllItems() == 0 )
    {
        OKBOXW( L"Can't Clear List!!" , L"Error" );
        return;
    }
    //리스트에 채워넣을 아이템이 있으면 채워 넣어야죠

    std::map< DWORD, GCItemData >::iterator mit;
    int iCount = 0;
    for( mit = SiKGCMissionInSort()->Begin() ; mit != SiKGCMissionInSort()->End() ; ++mit )
    {
        GCItemData& pItem = mit->second;

        if( m_bIsMissionList == true && pItem.eItemKind != GIK_MISSION_SCROLL)
        {
            continue;
        }

        if( m_StrFindName.empty() == false )
        {
            std::wstring strItemID = GC::IntToWchar( pItem.dwGoodsID );
            std::wstring strItemName = pItem.szItemName;
            if( (strItemID.find( m_StrFindName ) == strItemID.npos) && 
                ( strItemName.find( m_StrFindName ) == strItemName.npos ) )
            {
                continue;
            }
        }


        LV_ITEM lvitem; 
        lvitem.iItem=iCount; 
        int iSubItemID = 0;

        SetItem_GoodsID( lvitem , pItem , iSubItemID);
        m_list_Items.InsertItem(&lvitem); 

        SetItem_ItemName( lvitem , pItem , ++iSubItemID );     
        m_list_Items.SetItem(&lvitem); 

        SetItem_Character( lvitem , pItem , ++iSubItemID );    
        m_list_Items.SetItem(&lvitem); 

        SetItem_Level( lvitem , pItem , ++iSubItemID );        
        m_list_Items.SetItem(&lvitem); 

        SetItem_Type( lvitem , pItem , ++iSubItemID );    
        m_list_Items.SetItem(&lvitem); 

        SetItem_CashType( lvitem , pItem , ++iSubItemID ); 
        m_list_Items.SetItem(&lvitem); 

        SetItem_Desc( lvitem , pItem , ++iSubItemID );    
        m_list_Items.SetItem(&lvitem); 

        ++iCount;
    }
}

void KItemListDlg::SetItem_GoodsID( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem  )
{
    static WCHAR strColText[MAX_PATH];
    vlitem.mask=LVIF_TEXT; 
    vlitem.iSubItem= iSubItem;  
    _itow( (int)gcitem.dwGoodsID , strColText, 10 );
    vlitem.pszText= strColText; 
}

void KItemListDlg::SetItem_ItemName( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem  )
{
    vlitem.mask=LVIF_TEXT; 
    vlitem.iSubItem= iSubItem;  
    vlitem.pszText= gcitem.szItemName; 
}

void KItemListDlg::SetItem_Character( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem  )
{
    static WCHAR strColText[MAX_PATH];
    vlitem.mask=LVIF_TEXT; 
    vlitem.iSubItem= iSubItem;  
    _itow( (int)gcitem.dwCharType , strColText, 10 );
    vlitem.pszText= strColText;
}

void KItemListDlg::SetItem_Level( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem  )
{
    static WCHAR strColText[MAX_PATH];
    vlitem.mask=LVIF_TEXT; 
    vlitem.iSubItem= iSubItem;  
    _itow( gcitem.iNeedLevelForEquippingItem , strColText, 10 );
    vlitem.pszText= strColText;
}

void KItemListDlg::SetItem_Type( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem  )
{
    vlitem.mask=LVIF_TEXT; 
    vlitem.iSubItem= iSubItem;  
    vlitem.pszText= (gcitem.eItemType == 1?L"기간":L"수량" );
}

void KItemListDlg::SetItem_CashType( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem  )
{
    vlitem.mask=LVIF_TEXT; 
    vlitem.iSubItem= iSubItem;  
    vlitem.pszText= (gcitem.eMoneyType == 1 ? L"캐쉬" : L"GP");
}

void KItemListDlg::SetItem_Desc( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem  )
{
    vlitem.mask=LVIF_TEXT; 
    vlitem.iSubItem= iSubItem;  
    vlitem.pszText= gcitem.szItemDesc;
}
void KItemListDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // TODO: Add your control notification handler code here

    int                idx = pNMListView->iItem;
    CString msg;
    msg = m_list_Items.GetItemText(idx,0);
    m_edit_Selected.SetWindowText( msg );

    int itemp;
    GC::GetIntegerFromString( msg.GetBuffer() , itemp);

    m_dwItemID = itemp;

    *pResult = 0;
}

void KItemListDlg::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnCancel();
    m_dwItemID = 0;
}

bool KItemListDlg::FindItemAndSetEditBOX( CEdit& kEdit )
{
    if( m_dwItemID == 0 )
        return false;

    kEdit.SetWindowText( GC::IntToWchar( m_dwItemID ).c_str() );
    return true;
}
void KItemListDlg::OnEnChangeEditFind()
{
    // TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
    // CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
    // CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
    // 이 알림을 보내지 않습니다.

    // TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString strText;
    m_edit_Find.GetWindowText( strText );
    m_StrFindName = strText.GetBuffer();

    RefreshItemList();
}
