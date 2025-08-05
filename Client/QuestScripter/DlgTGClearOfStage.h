#pragma once
#include "DlgTGBase.h"
#include "ClearOfStage.h"
#include "stdafx.h"

// CDlgTGClearOfStage 대화 상자입니다.

class CDlgTGClearOfStage : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGClearOfStage)

public:
	CDlgTGClearOfStage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGClearOfStage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_CLEAROFSTAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
    CClearOfStage m_TGClearOfStage;
    void InitValue(void) { m_TGClearOfStage.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CClearOfStage*)pValue = m_TGClearOfStage; }
    void SetValue(CBaseTGFunction *pValue) { m_TGClearOfStage = *(CClearOfStage*)pValue;  }    

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    CStatic m_stStageClear;
};
