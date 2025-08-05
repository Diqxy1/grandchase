#pragma once
#include "stdafx.h"
#include "resource.h"
#include "DlgTGBase.h"
#include "ACEdit.h"
#include "PlayerPosition.h"

class CDlgTrigger;
// CDlgTGPlayerPosition 대화 상자입니다.

class CDlgTGPlayerPosition : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGPlayerPosition)

public:
	CDlgTGPlayerPosition(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGPlayerPosition();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_PLAYERPOSITION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CACEdit m_cbPlayerIndex;
    CEdit m_ebX;
    CEdit m_ebY;
    CStatic m_stPlayerPosition1;
    CStatic m_stPlayerPosition2;
    CComboBox m_cbRight;
    CStatic m_stPlayerPosition3;

    CPlayerPosition m_PlayerPosition;
    void InitValue(void) { m_PlayerPosition.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CPlayerPosition*)pValue = m_PlayerPosition; }
    void SetValue(CBaseTGFunction *pValue) { m_PlayerPosition = *(CPlayerPosition*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedButton1();
};
