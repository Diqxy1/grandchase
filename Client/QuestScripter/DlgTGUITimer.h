#pragma once


// CDlgTGUITimer 대화 상자입니다.

class CDlgTGUITimer : public CDialog
{
	DECLARE_DYNAMIC(CDlgTGUITimer)

public:
	CDlgTGUITimer(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGUITimer();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_TIMER_WITH_UI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
