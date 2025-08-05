#pragma once


// CDlgQuestSequence 대화 상자입니다.

class CDlgQuestSequence : public CDialog
{
	DECLARE_DYNAMIC(CDlgQuestSequence)

public:
	CDlgQuestSequence(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgQuestSequence();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_QUEST_SEQUENCE };

    bool m_bCreated;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
};
