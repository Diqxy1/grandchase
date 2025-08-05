#if !defined(AFX_KREASONDLG_H__C2F9601F_7504_4B62_8A4F_DA0F2B29F469__INCLUDED_)
#define AFX_KREASONDLG_H__C2F9601F_7504_4B62_8A4F_DA0F2B29F469__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KReasonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KReasonDlg dialog

class KReasonDlg : public CDialog
{
// Construction
public:
	KReasonDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(KReasonDlg)
	enum { IDD = IDD_DIALOG_REASON };
    CButton	m_radioCurse;
	CButton	m_radioPaper;
    CButton	m_radioImpersonation;
	//}}AFX_DATA
    int m_iSelect;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KReasonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(KReasonDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KREASONDLG_H__C2F9601F_7504_4B62_8A4F_DA0F2B29F469__INCLUDED_)
