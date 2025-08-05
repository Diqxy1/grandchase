#pragma once
#include "stdafx.h"

// CLayerProp 대화 상자입니다.

class CLayerProp : public CDialog
{
	DECLARE_DYNAMIC(CLayerProp)

public:
	CLayerProp(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLayerProp();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_LAYER_SIZE };

	void SetControlValue(CLayerPart* pPart);
	void UpdateControlValue();
	void UpdateSelectPart(CLayerPart* pPart);
	void DisableAllProperty(BOOL bDisable);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_eLayerX;
	CEdit m_eLayerY;
	CEdit m_eLayerWidth;
	CEdit m_eLayerHeight;
	CString m_strLayerX;
	CString m_strLayerY;
	CString m_strLayerWidth;
	CString m_strLayerHeight;
	float m_fLayerX;
	float m_fLayerY;
	float m_fLayerWidth;
	float m_fLayerHeight;
	afx_msg void OnBnClickedApply();
};
