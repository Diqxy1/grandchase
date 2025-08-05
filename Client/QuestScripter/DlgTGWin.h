#pragma once
#include "DlgTGBase.h"
#include "Win.h"
#include "stdafx.h"

// CDlgTGWin 대화 상자입니다.

class CDlgTGWin : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGWin)

public:
	CDlgTGWin(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGWin();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_WIN };

    CWin m_Win;
    void InitValue(void) { m_Win.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CWin*)pValue = m_Win; }
    void SetValue(CBaseTGFunction *pValue) { m_Win = *(CWin*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
public:
    CStatic m_stWin1;
};
