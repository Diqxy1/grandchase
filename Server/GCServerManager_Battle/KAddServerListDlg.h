#if !defined(AFX_KADDSERVERLISTDLG_H__1F1D7D46_FEB7_4727_BDF9_A3893FB24D52__INCLUDED_)
#define AFX_KADDSERVERLISTDLG_H__1F1D7D46_FEB7_4727_BDF9_A3893FB24D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KAddServerListDlg.h : header file
//

class CMainFrame; // forward declaration

/////////////////////////////////////////////////////////////////////////////
// KAddServerListDlg dialog

class KAddServerListDlg : public CDialog
{
// Construction
public:
	KAddServerListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(KAddServerListDlg)
	enum { IDD = IDD_ADD_SERVER_LIST };
	CIPAddressCtrl	m_ctrlServerIP;
	DWORD	m_dwServerPort;
	int		m_iVersion;
	//}}AFX_DATA
    DWORD   m_dwField0;
    DWORD   m_dwField1;
    DWORD   m_dwField2;
    DWORD   m_dwField3;

    CMainFrame* m_pMainFrm;
    bool    m_bEdit;
    int     m_iEditIdx;
    CString m_cstrServerName;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KAddServerListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(KAddServerListDlg)
	afx_msg void OnOk( void );
	virtual BOOL OnInitDialog( void );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KADDSERVERLISTDLG_H__1F1D7D46_FEB7_4727_BDF9_A3893FB24D52__INCLUDED_)
