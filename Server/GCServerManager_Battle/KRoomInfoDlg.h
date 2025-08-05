#if !defined(AFX_KROOMINFODLG_H__3129BF10_082C_4D15_A8AB_39C3A9195046__INCLUDED_)
#define AFX_KROOMINFODLG_H__3129BF10_082C_4D15_A8AB_39C3A9195046__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KRoomInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KRoomInfoDlg dialog

class CGCServerManagerView;

class KRoomInfoDlg : public CDialog
{
// Construction
public:
	KRoomInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(KRoomInfoDlg)
	enum { IDD = IDD_DIALOG_ROOM_INFO };
	DWORD	m_dwRoomID;
	CString	m_cstrRoomName;
	CString	m_cstrPublic;
	CString	m_cstrPasswd;
	CString	m_cstrUserNum;
	CString	m_cstrRoomStatus;
    CListBox m_ctrlRoomMembers;
	//}}AFX_DATA
    int     m_iIdxInfo;
    CGCServerManagerView* m_pChildView;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KRoomInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(KRoomInfoDlg)
	virtual BOOL OnInitDialog();
    afx_msg void OnDblclkRoomMembers();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KROOMINFODLG_H__3129BF10_082C_4D15_A8AB_39C3A9195046__INCLUDED_)
