#pragma once
#include "DlgTGBase.h"
#include "TitleText.h"
#include "stdafx.h"

// CDlgTGTitleText 대화 상자입니다.

class CDlgTGTitleText : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGTitleText)

public:
	CDlgTGTitleText(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGTitleText();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_TITLETEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    DECLARE_MESSAGE_MAP()

public:
    CEdit m_ebTitleText;
    CStatic m_stTileText1;

    CTitleText m_TGTitleText;
    void InitValue(void) { m_TGTitleText.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CTitleText*)pValue = m_TGTitleText; }
    void SetValue(CBaseTGFunction *pValue) { m_TGTitleText = *(CTitleText*)pValue; }

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnEnChangeEditTitletext();
	CStatic m_strTitleText2;
};
