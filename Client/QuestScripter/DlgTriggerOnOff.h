#pragma once
#include "stdafx.h"
#include "QuestScripter.h"
#include "DlgTrigger.h"
#include "QuestScripterDlg.h"
#include "DlgTGBase.h"
#include "TriggerOnOff.h"
#include "ACEdit.h"

// CDlgTriggerOnOff 대화 상자입니다.

class CDlgTriggerOnOff : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTriggerOnOff)

public:
	CDlgTriggerOnOff(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTriggerOnOff();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_TGONOFF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CACEdit m_cbName;
    CComboBox m_cbOnOff;

    CTriggerOnOff m_TGOnOff;
    void InitTGName(void);
    void InitValue(void) { m_TGOnOff.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CTriggerOnOff*)pValue = m_TGOnOff; }
    void SetValue(CBaseTGFunction *pValue) { m_TGOnOff = *(CTriggerOnOff*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedAddCharType();
    afx_msg void OnLbDBClickedCharType();
    CStatic m_stTGOnOff1;

public:
    CEdit m_ebCharType;
    CButton m_bbAddCharType;
    CListBox m_lbCharType;
    CComboBox m_cbHasChar;
    afx_msg void OnStnClickedStaticTriggerOnoff3();
};
