#pragma once
#include "stdafx.h"

// CChangeGridGuideTerm 대화 상자입니다.

class CChangeGridGuideTerm : public CDialog
{
	DECLARE_DYNAMIC(CChangeGridGuideTerm)

public:
	CChangeGridGuideTerm(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CChangeGridGuideTerm();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_CHANGE_GRIDGUIDE_TERM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CString m_strChangeTerm;
	CEdit m_eWantChange;
	CString m_strWantChange;
	virtual BOOL OnInitDialog();
};
