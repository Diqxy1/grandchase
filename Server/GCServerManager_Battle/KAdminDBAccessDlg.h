#if !defined(AFX_KADMINDBACCESSDLG_H__08AE4730_6AEC_474F_BD5D_AB5B2BDAF255__INCLUDED_)
#define AFX_KADMINDBACCESSDLG_H__08AE4730_6AEC_474F_BD5D_AB5B2BDAF255__INCLUDED_


#include "iostream"
using namespace std;
#include <vector>
#include "UserPacket.h"
#include "ItemStructure.h"
#include "KAdminInsertGPDurationItem.h"
#include "KAdminInsertGPNormalItem.h"
#include "KAdminDelItem.h"
#include "KAdminItemInfoData.h"
#include "KAdminInsertCashDurationItem.h"
#include "KAdminInsertCashNormalItem.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KAdminDBAccessDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KAdminDBAccessDlg dialog
class CGCServerManagerView;

class KAdminDBAccessDlg : public CDialog
{
// Construction
public:
	KAdminDBAccessDlg(CWnd* pParent = NULL);   // standard constructor

    void SetListContrlIntial();
    void SetListContrlData( std::vector<KSelectNormalItemInfo>& vecItemInv_ );
    void SetGCServerManagerView( CGCServerManagerView* gcServerManagerView_ );
    void SetListContrlDataDurationItem( std::vector<KSelectDurationItemInfo>& vecItemInv_);
    void GetAdminItemInfoDataList();
    void SetAdminItemInfoDataList( std::vector<KAdminUseItemInfo>& vecItemInv_ );
    void GetvecAdminItemInfoDataList( std::vector<KAdminUseItemInfo>& vecItemInv_);
    

// Dialog Data
	//{{AFX_DATA(KAdminDBAccessDlg)
	enum { IDD = IDD_DIALOG_ADMIN_DB_ACCESS };
	CListCtrl	m_ctrList2;
	CListCtrl	m_ctrList1;
	CString	m_strUserName;
	int		m_iGoodsID;
	int		m_iGoodsUID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KAdminDBAccessDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    //CListCtrl* m_pLC1;               //리스트 컨트롤의 객체 포인터 
    CGCServerManagerView* m_gcServerManagerView;
    KAdminInsertGPNormalItem       m_kInsertGPNormalItem;
    KAdminInsertGPDurationItem     m_kInsertGPDurationItem;
    KAdminDelItem                  m_kDelItem;
    std::vector<KAdminUseItemInfo> m_vecAdminItemDataList;
    KAdminItemInfoData             m_kAdminItemInfoData;
    KAdminInsertCashNormalItem       m_kInsertCashNormalItem;
    KAdminInsertCashDurationITem     m_kInsertCashDurationItem;

    

	// Generated message map functions
	//{{AFX_MSG(KAdminDBAccessDlg)
	afx_msg void OnSelectNormalItem();
	afx_msg void OnNormalItemSaveText();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDelNormalItem();
	afx_msg void OnSelectDurationItem();
	afx_msg void OnDurationDelItem();
	afx_msg void OnInsertItem();
	afx_msg void OnDeleteItem();
	afx_msg void OnDurationItemSaveText();
	afx_msg void OnInsertGpDurationItem();
	afx_msg void OnSelectUserData();
	afx_msg void OnChangeStrongItemList();
	afx_msg void OnAdminIteminfoDataButton();
	afx_msg void OnInsertCashNormalItem();
	afx_msg void OnInsertCashDurationItem();
	afx_msg void OnCloseButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KADMINDBACCESSDLG_H__08AE4730_6AEC_474F_BD5D_AB5B2BDAF255__INCLUDED_)
