#pragma once
#include "DlgTGBase.h"
#include "ReadOnlyEdit.h"
#include "NextStage.h"

// CDlgTGNextStage 대화 상자입니다.

class CDlgTGNextStage : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGNextStage)

public:
	CDlgTGNextStage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGNextStage();

    int GetCurTotalPercent(void);
    void RefreshRemainPercent(void);

    CNextStage m_TGNextStage;
    void InitValue(void) { m_TGNextStage.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CNextStage*)pValue = m_TGNextStage; }
    void SetValue(CBaseTGFunction *pValue) { m_TGNextStage = *(CNextStage*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_NEXTSTAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_lcStageNPercent;
    CEdit m_ebStage;
    CEdit m_ebPercent;
    CReadOnlyEdit m_ebRemainPercent;
    CEdit m_ebLevel;
    afx_msg void OnBnClickedButtonStage();
    afx_msg void OnBnClickedButtonStageDel();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnLvnItemchangedListStageNPercent(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    CStatic m_stNextStage1;
    CStatic m_stNextStage2;
};
