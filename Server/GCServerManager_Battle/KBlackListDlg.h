#if !defined(AFX_KBLACKLISTDLG_H__D257BB05_38C6_45EA_A7A5_C5CE0BFA162E__INCLUDED_)
#define AFX_KBLACKLISTDLG_H__D257BB05_38C6_45EA_A7A5_C5CE0BFA162E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KBlackListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KBlackListDlg dialog

class CMainFrame;

class KBlackListDlg : public CDialog
{
// Construction
public:
	KBlackListDlg(CWnd* pParent = NULL);   // standard constructor

public:
    void UpdateBlackList( void );

// Dialog Data
    enum { ID_BTN_CLOSE = 300 };
	//{{AFX_DATA(KBlackListDlg)
	enum { IDD = IDD_DIALOG_BLACK_LIST };
	CListBox	m_ctrlBlackList;
	//}}AFX_DATA
    CMainFrame* m_pMainFrm;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KBlackListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(KBlackListDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListBlackList();
	afx_msg void OnButtonRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KBLACKLISTDLG_H__D257BB05_38C6_45EA_A7A5_C5CE0BFA162E__INCLUDED_)
