#pragma once
#include "stdafx.h"

class CToolObject;

// CFootHoldValue 대화 상자입니다.

class CFootHoldValue : public CDialog
{
	DECLARE_DYNAMIC(CFootHoldValue)

public:
	CFootHoldValue(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFootHoldValue();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_FHVALUE };

	void DisableAllProperty(BOOL bDisable = TRUE);
	void SetValue(const std::vector< CToolObject* >& vecObj);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_eFHValue;
	CString m_strFHValue;
	afx_msg void OnBnClickedApply();
	CButton m_btnApply;
};
