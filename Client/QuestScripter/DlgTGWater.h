#pragma once
#include "DlgTGBase.h"
#include "Water.h"
#include "stdafx.h"

// CDlgTGWater 대화 상자입니다.

class CDlgTGWater : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGWater)

public:
	CDlgTGWater(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGWater();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_WATER };

    CWater m_TGWater;
    void InitValue(void) { m_TGWater.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CWater*)pValue = m_TGWater; }
    void SetValue(CBaseTGFunction *pValue) { m_TGWater = *(CWater*)pValue; }

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CEdit m_ebWaterHeight;
    CComboBox m_cbWaterOnOff;
    afx_msg void OnBnClickedButtonMapOnHeight();
};
