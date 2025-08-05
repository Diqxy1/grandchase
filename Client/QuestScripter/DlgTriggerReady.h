#pragma once
#include "DlgTGBase.h"
#include "TriggerReady.h"
#include "ACEdit.h"
#include "stdafx.h"

// CDlgTriggerReady 대화 상자입니다.

class CDlgTriggerReady : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTriggerReady)

public:
	CDlgTriggerReady(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTriggerReady();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_TGREADY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
    CACEdit m_cbName;

    CTriggerReady m_TGReady;
    void InitTGName(void);
    void InitValue(void) { m_TGReady.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CTriggerReady*)pValue = m_TGReady; }
    void SetValue(CBaseTGFunction *pValue) { m_TGReady = *(CTriggerReady*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    CStatic m_stTGReady;
};
