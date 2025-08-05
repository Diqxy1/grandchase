#pragma once
#include "DlgTGBase.h"

#include "Timer.h"
// CDlgTGTimer 대화 상자입니다.

class CDlgTGTimer : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGTimer)

public:
	CDlgTGTimer(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGTimer();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_TIMER };

    CTimer m_Timer;
    void InitValue(void) { m_Timer.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CTimer*)pValue = m_Timer; }
    void SetValue(CBaseTGFunction *pValue) { m_Timer = *(CTimer*)pValue;  }    

    void Refresh(void);
    void CalcResult(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CSpinButtonCtrl m_SpinButton;
    CEdit m_ebTime;
    afx_msg void OnDeltaposSpinTimer(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    CStatic m_stTimer1;
    CComboBox m_cbTimerUI;
};
