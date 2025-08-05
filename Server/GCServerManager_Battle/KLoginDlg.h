#if !defined(AFX_KLOGINDLG_H__2B6317F1_A282_4AA8_9063_D3BBEE8083F7__INCLUDED_)
#define AFX_KLOGINDLG_H__2B6317F1_A282_4AA8_9063_D3BBEE8083F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KLoginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KLoginDlg dialog

#include <string>
#include <vector>

class KLoginDlg : public CDialog
{
// Construction
public:
	KLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(KLoginDlg)
	enum { IDD = IDD_DIALOG_LOGIN };
	CString	m_cstrLoginID;
	CString	m_cstrLoginPW;
    std::vector<CString> m_vecKoreaLoginIDList; // 로그인 시 국내 운영자인지 판별하기 위해... (국내면 한글로 출력) 
    bool m_bLangStateKorean; // 한국어냐?
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(KLoginDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KLOGINDLG_H__2B6317F1_A282_4AA8_9063_D3BBEE8083F7__INCLUDED_)
