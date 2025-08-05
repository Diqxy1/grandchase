#pragma once
#include "DlgTGBase.h"
#include "stdafx.h"
#include "Reward.h"

// CDlgTGReward 대화 상자입니다.

class CDlgTGReward : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGReward)

public:
	CDlgTGReward(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGReward();

    CReward m_TGReward;
    void InitValue(void) { m_TGReward.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CReward*)pValue = m_TGReward; }
    void SetValue(CBaseTGFunction *pValue) { m_TGReward = *(CReward*)pValue; }   

    void Refresh(void);
    void CalcResult(void);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_REWARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CEdit m_ebEXP;
    CEdit m_ebGP;
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    CStatic m_stReward1;
    CStatic m_stReward2;
    CStatic m_stReward3;
    CStatic m_stReward4;
};
