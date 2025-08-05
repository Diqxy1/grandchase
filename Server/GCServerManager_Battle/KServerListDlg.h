#if !defined(AFX_KSERVERLISTDLG_H__EB775E52_06C0_4F67_B49F_77275F9041F9__INCLUDED_)
#define AFX_KSERVERLISTDLG_H__EB775E52_06C0_4F67_B49F_77275F9041F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KServerListDlg.h : header file
//

#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
// KServerListDlg dialog

class KServerListDlg : public CDialog
{
// Construction
public:
	KServerListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(KServerListDlg)
	enum { IDD = IDD_SERVER_LIST };
	CListBox    m_ctrlSvrListBox;
	//}}AFX_DATA

    CGCServerManagerApp*    m_pApp;
    CMainFrame*             m_pMainFrm;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KServerListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void AddListBox( SServerInfo svrInfo );
    void RefreshListBox( void );

	// Generated message map functions
	//{{AFX_MSG(KServerListDlg)
	afx_msg void OnAddList( void );
    afx_msg void OnEditList( void );
	afx_msg void OnRemoveList( void );
	virtual BOOL OnInitDialog( void );
	afx_msg void OnOK( void );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KSERVERLISTDLG_H__EB775E52_06C0_4F67_B49F_77275F9041F9__INCLUDED_)
