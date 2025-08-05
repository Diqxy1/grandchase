#pragma once
#include "DlgTGBase.h"
#include "OpenDGate.h"
#include "stdafx.h"

// CDlgTGOpenDGate 대화 상자입니다.

class CDlgTGOpenDGate : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGOpenDGate)

public:
	CDlgTGOpenDGate(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGOpenDGate();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_OPEN_DGATE };

    COpenDGate m_TGOpenDGate;
    void InitValue(void) { m_TGOpenDGate.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(COpenDGate*)pValue = m_TGOpenDGate; }
    void SetValue(CBaseTGFunction *pValue) { m_TGOpenDGate = *(COpenDGate*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

    CComboBox m_cbDirection;    // 0 : left, 1 : top, 2 : right, 3 : bottom
public:
    CStatic m_stOpenGate1;
};
