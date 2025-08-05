#if !defined(AFX_KUSERINFODLG_H__2C78377A_DEE2_40B7_94DB_FA026BC09D03__INCLUDED_)
#define AFX_KUSERINFODLG_H__2C78377A_DEE2_40B7_94DB_FA026BC09D03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KUserInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KUserInfoDlg dialog

class CGCServerManagerView;

class KUserInfoDlg : public CDialog
{
// Construction
public:
	KUserInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(KUserInfoDlg)
	enum { IDD = IDD_DIALOG_USER_INFO };
	CString	m_cstrStatus;
	//CString	m_cstrZender;
	CString	m_cstrLogin;
    CString m_cstrLoginNickname;
	//}}AFX_DATA
    int     m_iIdxInfo;
    CGCServerManagerView* m_pChildView;
    DWORD m_dwUserUIDInfo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KUserInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(KUserInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonKick();
	afx_msg void OnButtonChangeDbInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnEnChangeEditUserLoginId();
    afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KUSERINFODLG_H__2C78377A_DEE2_40B7_94DB_FA026BC09D03__INCLUDED_)
