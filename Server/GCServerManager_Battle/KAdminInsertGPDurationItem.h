#if !defined(AFX_KADMININSERTGPDURATIONITEM_H__AEFDA726_C455_4909_B673_909C572B6898__INCLUDED_)
#define AFX_KADMININSERTGPDURATIONITEM_H__AEFDA726_C455_4909_B673_909C572B6898__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KAdminInsertGPDurationItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KAdminInsertGPDurationItem dialog

class CGCServerManagerView;

class KAdminInsertGPDurationItem : public CDialog
{
// Construction
public:
	KAdminInsertGPDurationItem(CWnd* pParent = NULL);   // standard constructor
    void SetGCServerManagerView( CGCServerManagerView* gcServerManagerView_ );

// Dialog Data
	//{{AFX_DATA(KAdminInsertGPDurationItem)
	enum { IDD = IDD_ADMIN_INSERT_GP_DURATION_ITEM_ };
	CString	m_strGPDurationItemUserName;
	int		m_iGPDurationItemGoodsID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KAdminInsertGPDurationItem)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CGCServerManagerView* m_gcServerManagerView;

	// Generated message map functions
	//{{AFX_MSG(KAdminInsertGPDurationItem)
	afx_msg void OnAdminInsertGpDurationItem();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KADMININSERTGPDURATIONITEM_H__AEFDA726_C455_4909_B673_909C572B6898__INCLUDED_)
