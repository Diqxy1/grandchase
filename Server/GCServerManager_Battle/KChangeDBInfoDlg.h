#include "afxcmn.h"
#if !defined(AFX_KCHANGEDBINFODLG_H__0E08F31C_4DC0_4156_BA26_9C83BD1ADC30__INCLUDED_)
#define AFX_KCHANGEDBINFODLG_H__0E08F31C_4DC0_4156_BA26_9C83BD1ADC30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KChangeDBInfoDlg.h : header file
//

#include "UserPacket.h"
#include "ReportCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// KChangeDBInfoDlg dialog

class KChangeDBInfoDlg : public CDialog
{
// Construction
public:
	KChangeDBInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(KChangeDBInfoDlg)
	enum { IDD = IDD_DIALOG_CHANGE_DB_INFO };
	CComboBox   m_ctrlCbAuthLv;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KChangeDBInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(KChangeDBInfoDlg)
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonKickUser();
	afx_msg void OnButtonDropUser();
	afx_msg void OnButtonExpReset();
	//}}AFX_MSG
    afx_msg LRESULT OnEditCommitted(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
    enum LIST_COLUMN_ID {
        COLUMN_ID = 0,
        COLUMN_JOB,
        COLUMN_CHAR_NAME,
        COLUMN_PROMOTION,
        COLUMN_EXP,
        COLUMN_LEVEL,
        COLUMN_WIN,
        COLUMN_LOSE,
        COLUMN_MAX_NUM,
    };
    CReportCtrl m_list;
    KUserInfoForOperator m_kData;
    void SetInfo( IN const KUserInfoForOperator& kData ) { m_kData = kData; }
    void GetInfo( OUT KUserInfoForOperator& kData ) { kData = m_kData; }
    void UpdateList( BOOL bSaveAndValidate );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KCHANGEDBINFODLG_H__0E08F31C_4DC0_4156_BA26_9C83BD1ADC30__INCLUDED_)
