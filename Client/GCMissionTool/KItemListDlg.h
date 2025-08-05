#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "GCMissionInSort.h"


// KItemListDlg 대화 상자입니다.

class KItemListDlg : public CDialog
{
	DECLARE_DYNAMIC(KItemListDlg)

public:
	KItemListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KItemListDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ITEM_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    virtual BOOL OnInitDialog();
    void InitItemList();
    void RefreshItemList();

    void SetItem_GoodsID( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem );
    void SetItem_ItemName( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem );
    void SetItem_Character( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem );
    void SetItem_Level( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem );
    void SetItem_Type( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem );
    void SetItem_CashType( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem );
    void SetItem_Desc( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem );

protected:
    CListCtrl m_list_Items;
    CEdit m_edit_Selected;

    std::wstring m_StrFindName;
public:
    bool m_bIsMissionList;
    void SetListType( bool bIsMissionList ) { m_bIsMissionList = bIsMissionList; }

protected:
	DECLARE_MESSAGE_MAP()
    afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
public:    
    DWORD m_dwItemID;
    bool FindItemAndSetEditBOX( CEdit& kEdit );
    afx_msg void OnBnClickedCancel();
    afx_msg void OnEnChangeEditFind();
    CEdit m_edit_Find;
};
