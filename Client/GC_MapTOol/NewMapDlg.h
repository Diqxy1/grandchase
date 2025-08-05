#pragma once

#include "resource.h"

// CNewMapDlg 대화 상자입니다.

class CNewMapDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewMapDlg)

public:
	CNewMapDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNewMapDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEW_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strWidth;
	CString m_strHeight;
};
