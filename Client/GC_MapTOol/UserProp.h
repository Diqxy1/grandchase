#pragma once
#include "stdafx.h"

// CUserProp 대화 상자입니다.

class CUserProp : public CDialog
{
	DECLARE_DYNAMIC(CUserProp)

public:
	CUserProp(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserProp();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_USER_PROP };

	void DisableAllProperty(BOOL bDisable);
	void UpdateControlValue();
	void SetControlValue(CLayerPart* pPart);
	void UpdateSelectPart(CLayerPart* pPart);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_eObjectName;
	CString m_strObjectName;
	CButton m_btnObjectNameVerify;
	CEdit m_eUserValue;
	CString m_strUserValue;
	int     m_iUserValue;
	CEdit m_eUserString;
	CString m_csUserString;
	std::string m_strUserString;
	afx_msg void OnBnClickedObjectNameVerify();
	afx_msg void OnBnClickedApply();
};
