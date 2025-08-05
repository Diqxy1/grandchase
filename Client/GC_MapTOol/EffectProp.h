#pragma once
#include "stdafx.h"

// CEffectProp 대화 상자입니다.

class CEffectProp : public CDialog
{
	DECLARE_DYNAMIC(CEffectProp)

public:
	CEffectProp(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffectProp();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_EFFECT_PROP };

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
	CEdit m_eEffectName;
	CString m_strEffectName;
	CEdit m_eEffectX;
	CEdit m_eEffectY;
	CString m_strEffectX;
	CString m_strEffectY;
	int m_iEffectX;
	int m_iEffectY;
	CButton m_btnUseEffect;
	BOOL m_bUseEffect;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedApply();
	CLayerPart* m_pPartClone;
	void OnOK();
};
