#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "ACEdit.h"
#include "Dialogue.h"

// CDlgTGDialogue 대화 상자입니다.

class CDlgTGDialogue : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGDialogue)

public:
	CDlgTGDialogue(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGDialogue();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_DIALOGUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CACEdit         m_cbDialogue;
    CComboBox       m_cbOnOff;
    CEdit           m_ceChar;

    CDialogue m_TGDialogue;
    void InitValue(void) { m_TGDialogue.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CDialogue*)pValue = m_TGDialogue; }
    void SetValue(CBaseTGFunction *pValue) { m_TGDialogue = *(CDialogue*)pValue;  }    

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
};
