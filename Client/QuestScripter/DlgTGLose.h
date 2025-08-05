#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "Lose.h"

// CDlgTGLose 대화 상자입니다.

class CDlgTGLose : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGLose)

public:
	CDlgTGLose(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGLose();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_LOSE };

    CLose m_Lose;
    void InitValue(void) { m_Lose.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CLose*)pValue = m_Lose; }
    void SetValue(CBaseTGFunction *pValue) { m_Lose = *(CLose*)pValue; }    

    void CalcResult(void);
    void Refresh(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
public:
    CStatic m_stLose;
};
