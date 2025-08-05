#pragma once
#include "stdafx.h"
#include "DialogueState.h"
#include "DlgTGBase.h"

// CDlgTGDialogueState 대화 상자입니다.

class CDlgTGDialogueState : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGDialogueState)

public:
	CDlgTGDialogueState(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGDialogueState();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_DIALOGUE_STATE };

    CDialogueState m_DialogueState;
    void InitValue(void) { m_DialogueState.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CDialogueState*)pValue = m_DialogueState; }
    void SetValue(CBaseTGFunction *pValue) { m_DialogueState = *(CDialogueState*)pValue;  }    

    void Refresh(void);
    void CalcResult(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CComboBox m_cbDialogueState;
};
