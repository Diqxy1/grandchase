#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"

// CDlgTGCommon 대화 상자입니다.

class CDlgTGCommon : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGCommon)

public:
	CDlgTGCommon(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGCommon();

    bool m_bCheck;
    void InitValue(void) { m_bCheck = false; }
    void GetValue(CBaseTGFunction *pValue);
    void SetValue(CBaseTGFunction *pValue);
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_COMMON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CButton m_btRepeat;
    afx_msg void OnBnClickedCheckRepeat();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
};
