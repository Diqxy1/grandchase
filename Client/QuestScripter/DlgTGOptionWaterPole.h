#pragma once
#include "stdafx.h"


// CDlgTGOptionWaterPole 대화 상자입니다.

class CDlgTGOptionWaterPole : public CDialog
{
	DECLARE_DYNAMIC(CDlgTGOptionWaterPole)

public:
	CDlgTGOptionWaterPole(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGOptionWaterPole();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_ADDDAMAGE_OPTION_WATERPOLE };

    int     m_iLife;    
    int     m_iCrashStart;
    int     m_iMySpeed;
    int     m_iJumpFrame;
    int     m_iContinueance;

    void GetValue(void);
    void SetValue(int iLife, int iStartLife, int iCrashStart, int iMySpeed, int iJumpFrame, int iContinueance);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

    CEdit m_edLife;
    
    CEdit m_ebCrashStart;
    CEdit m_edMySpeed;
    CEdit m_edJumpFrame;
    CEdit m_ebContinueance;
};
