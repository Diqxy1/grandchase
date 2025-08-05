#pragma once

#include "DlgQuest.h"
#include "DlgStage.h"
#include "struct.h"
// CDlgInfo 대화 상자입니다.

class CDlgInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgInfo)

public:
	CDlgInfo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgInfo();

    bool m_bCreated;

    CDlgQuest           m_dlgQuest;
    CDlgStage           m_dlgStage;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_POPUP_BOARD };

    NamesOfLevel m_CurType;
    void SetDlgType(NamesOfLevel Type, struct_base *pBase);
    void SetQuest(Quest quest);
    void SetStage(Stage stage);

    void AllDlgHide(void);

    Quest m_Quest;
    Stage m_Stage;
    
    HTREEITEM m_hCurTreeItem;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedButton1();
};
