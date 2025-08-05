#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "GCMissionInSort.h"


// KMissionRewardList 대화 상자입니다.

class KMissionRewardList : public CDialog
{
	DECLARE_DYNAMIC(KMissionRewardList)

public:
	KMissionRewardList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMissionRewardList();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MISSIONREWARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

    afx_msg void OnBnClickedOk();
public:
    CListCtrl m_kItemList;
    CEdit m_kFind;
    CEdit m_kCount;

    std::wstring m_strFindText;

protected:
    void InitItemList();
    void RefreshItemList();

    void SetItem_GoodsID( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem );
    void SetItem_ItemName( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem );
    void SetItem_Character( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem );
    void SetItem_Level( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem );
    void SetItem_Type( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem );
    void SetItem_CashType( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem );
    void SetItem_Desc( LV_ITEM& vlitem , GCItemData& gcitem, int iSubItem );
public:

    DWORD   m_dwItemID;
    int     m_iCount;   //기간이면 (일) 수량이면 (개)
    
    afx_msg void OnHdnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
    CEdit m_kSelectedItem;
    afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnChangeEdit1();
};
