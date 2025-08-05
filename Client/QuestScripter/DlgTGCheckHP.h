#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "CheckHP.h"


// CDlgTGCheckHP 대화 상자입니다.

class CDlgTGCheckHP : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGCheckHP)

public:
	CDlgTGCheckHP(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGCheckHP();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_CHECK_HP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	void InitValue(void);
	void GetValue(CBaseTGFunction *pValue) { *(CCheckHP*)pValue = m_CheckHP; }
	void SetValue(CBaseTGFunction *pValue) { m_CheckHP = *(CCheckHP*)pValue; }

	void Refresh(void);
	void CalcResult(void);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CComboBox m_comboSlot;
	CComboBox m_comboType;
	CEdit m_editPercent;

	CCheckHP	m_CheckHP;
public:
	afx_msg void OnCbnSelchangeComboSlotType();
public:
	CStatic m_stCheckHP1;
public:
	CStatic m_stCheckHP2;
public:
	CStatic m_stCheckHP3;
};
