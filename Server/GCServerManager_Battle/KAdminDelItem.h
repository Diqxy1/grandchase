#if !defined(AFX_KADMINDELITEM_H__B478D9D3_27F8_4909_87AD_2E3CAB720881__INCLUDED_)
#define AFX_KADMINDELITEM_H__B478D9D3_27F8_4909_87AD_2E3CAB720881__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KAdminDelItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KAdminDelItem dialog
class CGCServerManagerView;

class KAdminDelItem : public CDialog
{
// Construction
public:
	KAdminDelItem(CWnd* pParent = NULL);   // standard constructor
    void SetGCServerManagerView( CGCServerManagerView* gcServerManagerView_ );
// Dialog Data
	//{{AFX_DATA(KAdminDelItem)
	enum { IDD = IDD_ADMIN_DEL_ITEM_ };
	int		m_DelItemGoodsID;
	int		m_DelItemGoodsUID;
	CString	m_strDelItemUserName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KAdminDelItem)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CGCServerManagerView*  m_gcServerManagerView;

	// Generated message map functions
	//{{AFX_MSG(KAdminDelItem)
	afx_msg void OnAdminDelItem();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KADMINDELITEM_H__B478D9D3_27F8_4909_87AD_2E3CAB720881__INCLUDED_)
