// KMissionRewardList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GCMissionTool.h"
#include "KMissionRewardList.h"
#include "define.h"
#include ".\kmissionrewardlist.h"
#include "GlobalFunc.h"


// KMissionRewardList 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMissionRewardList, CDialog)
KMissionRewardList::KMissionRewardList(CWnd* pParent /*=NULL*/)
	: CDialog(KMissionRewardList::IDD, pParent)
{
}

KMissionRewardList::~KMissionRewardList()
{
}

void KMissionRewardList::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_kItemList);
    DDX_Control(pDX, IDC_EDIT1, m_kFind);
    DDX_Control(pDX, IDC_EDIT2, m_kCount);
    DDX_Control(pDX, IDC_EDIT3, m_kSelectedItem);
}


BEGIN_MESSAGE_MAP(KMissionRewardList, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_NOTIFY(HDN_ITEMCHANGED, 0, OnHdnItemchangedList1)
    ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
    ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
END_MESSAGE_MAP()


// KMissionRewardList 메시지 처리기입니다.

BOOL KMissionRewardList::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_kItemList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_dwItemID  = -1;
    m_iCount    = 0;
    m_kCount.SetWindowText( L"1" );
    InitItemList();
    RefreshItemList();
    return TRUE;
}

void KMissionRewardList::InitItemList()
{
    //일단 리스트의 타이틀을 만들어 봅시다.
    WCHAR strColTitle[7][255] = { L"GoodsID", L"ItemName", L"Character", L"level", L"type", L"CashType", L"Desc" };
    int iColWidth[7] = { 50, 200, 50, 50, 50, 50, 200 };
    std::vector< std::wstring > vecTitleList;
    for( int i = 0 ; i < 7 ; ++i )
    {
        vecTitleList.push_back( strColTitle[i] );
    }    
    GC::SetListColumn( m_kItemList, vecTitleList, iColWidth, 7 );
}

void KMissionRewardList::RefreshItemList()
{
    //일단 리스트에 있는 내용들을 모두 지워줍시다.
    if( m_kItemList.DeleteAllItems() == 0 )
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

        LV_ITEM lvitem; 
        lvitem.iItem=iCount; 
        int iSubItemID = 0;

        if( m_strFindText.empty() == false )
        {
            std::wstring strItemID = GC::IntToWchar( pItem.dwGoodsID );
            std::wstring strItemName = pItem.szItemName;
            if( (strItemID.find( m_strFindText ) == strItemID.npos) && 
                ( strItemName.find( m_strFindText ) == strItemName.npos ) )
            {
                continue;
            }
        }

        SetItem_GoodsID( lvitem , pItem , iSubItemID);
        m_kItemList.InsertItem(&lvitem); 

        SetItem_ItemName( lvitem , pItem , ++iSubItemID );     
        m_kItemList.SetItem(&lvitem); 

        SetItem_Character( lvitem , pItem , ++iSubItemID );    
        m_kItemList.SetItem(&lvitem); 

        SetItem_Level( lvitem , pItem , ++iSubItemID );        
        m_kItemList.SetItem(&lvitem); 

        SetItem_Type( lvitem , pItem , ++iSubItemID );    
        m_kItemList.SetItem(&lvitem); 

        SetItem_CashType( lvitem , pItem , ++iSubItemID ); 
        m_kItemList.SetItem(&lvitem); 

        SetItem_Desc( lvitem , pItem , ++iSubItemID );    
        m_kItemList.SetItem(&lvitem); 

        ++iCount;
    }
}

void KMissionRewardList::SetItem_GoodsID( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem  )
{
    static WCHAR strColText[MAX_PATH];
    vlitem.mask=LVIF_TEXT; 
    vlitem.iSubItem= iSubItem;  
    _itow( (int)gcitem.dwGoodsID , strColText, 10 );
    vlitem.pszText= strColText; 
}

void KMissionRewardList::SetItem_ItemName( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem  )
{
    vlitem.mask=LVIF_TEXT; 
    vlitem.iSubItem= iSubItem;  
    vlitem.pszText= gcitem.szItemName; 
}

void KMissionRewardList::SetItem_Character( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem  )
{
    static WCHAR strColText[MAX_PATH];
    vlitem.mask=LVIF_TEXT; 
    vlitem.iSubItem= iSubItem;  
    _itow( (int)gcitem.dwCharType , strColText, 10 );
    vlitem.pszText= strColText;
}

void KMissionRewardList::SetItem_Level( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem  )
{
    static WCHAR strColText[MAX_PATH];
    vlitem.mask=LVIF_TEXT; 
    vlitem.iSubItem= iSubItem;  
    _itow( gcitem.iNeedLevelForEquippingItem , strColText, 10 );
    vlitem.pszText= strColText;
}

void KMissionRewardList::SetItem_Type( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem  )
{
    vlitem.mask=LVIF_TEXT; 
    vlitem.iSubItem= iSubItem;  
    vlitem.pszText= (gcitem.eItemType == 1?L"기간":L"수량" );
}

void KMissionRewardList::SetItem_CashType( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem  )
{
    vlitem.mask=LVIF_TEXT; 
    vlitem.iSubItem= iSubItem;  
    vlitem.pszText= (gcitem.eMoneyType == 1 ? L"캐쉬" : L"GP");
}

void KMissionRewardList::SetItem_Desc( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem  )
{
    vlitem.mask=LVIF_TEXT; 
    vlitem.iSubItem= iSubItem;  
    vlitem.pszText= gcitem.szItemDesc;
}
void KMissionRewardList::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

    m_kItemList;
    m_dwItemID;

    CString strNum;
    m_kCount.GetWindowText( strNum );
    GC::GetIntegerFromString( strNum, m_iCount );

    OnOK();
}

void KMissionRewardList::OnHdnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    

}

void KMissionRewardList::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // TODO: Add your control notification handler code here

    int                idx = pNMListView->iItem;
    CString msg;
    msg = m_kItemList.GetItemText(idx,0);
    m_kSelectedItem.SetWindowText( msg );

    int itemp;
    GC::GetIntegerFromString( msg.GetBuffer() , itemp);

    m_dwItemID = itemp;

    *pResult = 0;
}

void KMissionRewardList::OnEnChangeEdit1()
{
    // TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
    // CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
    // CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
    // 이 알림을 보내지 않습니다.

    // TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CString strText;
    m_kFind.GetWindowText( strText );
    m_strFindText = strText.GetBuffer();
}
