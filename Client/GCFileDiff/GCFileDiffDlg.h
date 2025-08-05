// GCFileDiffDlg.h : header file
//

#include "afxwin.h"
#include "DropEdit.h"
#if !defined(AFX_GCFILEDIFFDLG_H__39BB1F2D_767F_4AF8_8528_E7762585215A__INCLUDED_)
#define AFX_GCFILEDIFFDLG_H__39BB1F2D_767F_4AF8_8528_E7762585215A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGCFileDiffDlg dialog

class CGCFileDiffDlg : public CDialog
{
// Construction
public:
	CGCFileDiffDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGCFileDiffDlg)
	enum { IDD = IDD_GCFILEDIFF_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGCFileDiffDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGCFileDiffDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnOldPath();
	afx_msg void OnBtnNewPath();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
    afx_msg void OnBnClickedBtnUpdatePath();
    CDropEdit	m_editNewPath;
    CDropEdit	m_editOldPath;
    CDropEdit   m_editUpdatePath;
    
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GCFILEDIFFDLG_H__39BB1F2D_767F_4AF8_8528_E7762585215A__INCLUDED_)
