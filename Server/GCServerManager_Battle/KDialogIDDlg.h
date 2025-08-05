#if !defined(AFX_KDIALOGIDDLG_H__32B899E7_8A17_4453_9148_88CDF66B6BC8__INCLUDED_)
#define AFX_KDIALOGIDDLG_H__32B899E7_8A17_4453_9148_88CDF66B6BC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KDialogIDDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KDialogIDDlg dialog

class KDialogIDDlg : public CDialog
{
// Construction
public:
	KDialogIDDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(KDialogIDDlg)
	enum { IDD = IDD_DIALOG_NEWID };
	CString	m_cstrUserID;
    int m_iNickname;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KDialogIDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(KDialogIDDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KDIALOGIDDLG_H__32B899E7_8A17_4453_9148_88CDF66B6BC8__INCLUDED_)
