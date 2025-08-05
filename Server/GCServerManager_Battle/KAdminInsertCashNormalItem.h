#if !defined(AFX_KADMININSERTCASHNORMALITEM_H__27403988_BDF0_4526_8DB7_88518A7BE0BA__INCLUDED_)
#define AFX_KADMININSERTCASHNORMALITEM_H__27403988_BDF0_4526_8DB7_88518A7BE0BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KAdminInsertCashNormalItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KAdminInsertCashNormalItem dialog
class  CGCServerManagerView;

class KAdminInsertCashNormalItem : public CDialog
{
// Construction
public:
	KAdminInsertCashNormalItem(CWnd* pParent = NULL);   // standard constructor
    void SetGCServerManagerView( CGCServerManagerView* gcServerManagerView_ );

// Dialog Data
	//{{AFX_DATA(KAdminInsertCashNormalItem)
	enum { IDD = IDD_ADMIN_INSERT_CASH_NORMAL_ITEM_ };
	int		m_strCashInsertNormalItemGoodsID;
	CString	m_strCashInsertNormalItemUserName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KAdminInsertCashNormalItem)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CGCServerManagerView*   m_gcServerManagerView;

	// Generated message map functions
	//{{AFX_MSG(KAdminInsertCashNormalItem)
	afx_msg void OnAdminInsertCashNormalItem();
	virtual BOOL OnInitDialog();
	afx_msg void OnCloseButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KADMININSERTCASHNORMALITEM_H__27403988_BDF0_4526_8DB7_88518A7BE0BA__INCLUDED_)
