#if !defined(AFX_KADMININSERTCASHDURATIONITEM_H__0D7AE141_D1BE_4CA9_AB0B_85F20B0CC49B__INCLUDED_)
#define AFX_KADMININSERTCASHDURATIONITEM_H__0D7AE141_D1BE_4CA9_AB0B_85F20B0CC49B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KAdminInsertCashDurationITem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KAdminInsertCashDurationITem dialog
class  CGCServerManagerView;

class KAdminInsertCashDurationITem : public CDialog
{
// Construction
public:
	KAdminInsertCashDurationITem(CWnd* pParent = NULL);   // standard constructor
    void SetGCServerManagerView( CGCServerManagerView* gcServerManagerView_ );

// Dialog Data
	//{{AFX_DATA(KAdminInsertCashDurationITem)
	enum { IDD = IDD_ADMIN_INSERT_CASH_DURATION_ITEM_ };
	int		m_iCashInsertDurationItemGoodsID;
	CString	m_strCashInsertDurationItemUserName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KAdminInsertCashDurationITem)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CGCServerManagerView*   m_gcServerManagerView;

	// Generated message map functions
	//{{AFX_MSG(KAdminInsertCashDurationITem)
	afx_msg void OnAdminInsertCashDurationItem();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KADMININSERTCASHDURATIONITEM_H__0D7AE141_D1BE_4CA9_AB0B_85F20B0CC49B__INCLUDED_)
