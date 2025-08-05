#pragma once

#include "resource.h"

// CResizingDlg 대화 상자입니다.

class CResizingDlg : public CDialog
{
	DECLARE_DYNAMIC(CResizingDlg)

public:
	CResizingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CResizingDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_RESIZING_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strWidth;
	CString m_strHeight;
	void SetData(int ResizedX, int ResizedY);
	afx_msg void OnBnClickedOk();
};
