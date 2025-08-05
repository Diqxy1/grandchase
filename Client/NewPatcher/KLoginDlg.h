#pragma once

#include "resource.h"
// KLoginDlg 대화 상자입니다.

class KLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(KLoginDlg)

public:
	KLoginDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KLoginDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOGINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    char*   GetID();
    CString m_kLoginID;
    afx_msg void OnBnClickedCancel();
};
