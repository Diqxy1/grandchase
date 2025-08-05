#if !defined(AFX_KADMININSERTGPNORMALITEM_H__57709E74_16E3_4606_8FEB_632341D6B8F4__INCLUDED_)
#define AFX_KADMININSERTGPNORMALITEM_H__57709E74_16E3_4606_8FEB_632341D6B8F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KAdminInsertGPNormalItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KAdminInsertGPNormalItem dialog
class  CGCServerManagerView;

class KAdminInsertGPNormalItem : public CDialog
{
// Construction
public:
	KAdminInsertGPNormalItem(CWnd* pParent = NULL);   // standard constructor
    void SetGCServerManagerView( CGCServerManagerView* gcServerManagerView_ );

// Dialog Data
	//{{AFX_DATA(KAdminInsertGPNormalItem)
	enum { IDD = IDD_ADMIN_INSERT_GP_NORMAL_ITEM_ };
	CString	m_strGPNormalItemUserName;
	int		m_iGPNormalItemGoodsID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KAdminInsertGPNormalItem)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CGCServerManagerView* m_gcServerManagerView;

	// Generated message map functions
	//{{AFX_MSG(KAdminInsertGPNormalItem)
	afx_msg void OnAdminInsertGpNormalItem();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KADMININSERTGPNORMALITEM_H__57709E74_16E3_4606_8FEB_632341D6B8F4__INCLUDED_)
