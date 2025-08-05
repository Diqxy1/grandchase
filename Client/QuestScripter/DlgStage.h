#pragma once
#include "stdafx.h"
#include "struct.h"
#include "ACEdit.h"
#include "Resource.h"

// CDlgStage 대화 상자입니다.
class CQuestScripterDlg;
class CCategory;

class CDlgStage : public CDialog
{
	DECLARE_DYNAMIC(CDlgStage)

public:
	CDlgStage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgStage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_STAGE };

    bool m_bCreated;
    CButton m_btStageMap;
    CButton m_btOpenTGEditor;
    std::vector<CCategory*> m_vtCategory;
    std::vector<CCategory*> *m_pvtCategory;
    void CopyToCategorys(std::vector<CCategory*> *pvtDest, std::vector<CCategory*> *pvtSource);
    void ClearCategorys(std::vector<CCategory*> *pvtCategory);
    void SetValue(Stage *stage);
    void GetValue(Stage *stage);

    void SetDisableControl(bool bDisable);

    CQuestScripterDlg *m_pQuestScripter;
    void SetComboBoxValue(CQuestScripterDlg *pQuestScripter);
    void SetMainDlgPointer(CQuestScripterDlg *pQuestScripter) { m_pQuestScripter = pQuestScripter; }
    void RefreshMapDriectory(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedButtonStagemap();
    afx_msg void OnBnClickedButtonOpenTgeditor();
    CACEdit m_cbStageMap;
};
