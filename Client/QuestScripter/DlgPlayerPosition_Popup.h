#pragma once
#include "stdafx.h"
#include "PlayerPosition.h"
#include "resource.h"
// CDlgPlayerPosition_Popup 대화 상자입니다.

class CDlgPlayerPosition_Popup : public CDialog
{
	DECLARE_DYNAMIC(CDlgPlayerPosition_Popup)

public:
	CDlgPlayerPosition_Popup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPlayerPosition_Popup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_PLAYERPOSITION_POPUP };

    bool                            m_bFnAction;
    CPlayerPosition                 m_PlayerPosition;
    std::vector<CPlayerPosition*>        *m_pvtPlayerPosition;
    void SetPlayerListValue(std::vector<CPlayerPosition*> *pvtPlayerPosition);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

	DECLARE_MESSAGE_MAP()
public:
    CEdit           m_edEntranceX;
    CEdit           m_edEntranceY;
    CComboBox       m_cbFunctionType;
    CComboBox       m_cbPlayerIndex;
    CComboBox       m_cbRight;
    afx_msg void OnBnClickedDelete();
};
