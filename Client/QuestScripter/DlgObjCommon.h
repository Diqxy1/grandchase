#pragma once
#include "afxwin.h"
#include "struct.h"
#include "ReadOnlyEdit.h"
#include "ACEdit.h"

class CQuestScripterDlg;
// CDlgObjCommon 대화 상자입니다.

class CDlgObjCommon : public CDialog
{
	DECLARE_DYNAMIC(CDlgObjCommon)

public:
	CDlgObjCommon(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgObjCommon();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_OBJ_COMMON };

    enum CommonType { STAGE_MON };
    void SetType(CommonType Type);
    CommonType m_CurType;

    CQuestScripterDlg *m_pQuestScripter;
    bool m_bCreated;
    void SetValue(Stage_mon stage_mon);
    Stage_mon GetValue(void);
    void SetComboBoxValue(CQuestScripterDlg *pQuestScripter);
    void SetMainDlgPointer(CQuestScripterDlg *pQuestScripter) { m_pQuestScripter = pQuestScripter; }

    CReadOnlyEdit m_edType;
    CReadOnlyEdit m_edName;
    CReadOnlyEdit m_edLevel;
    CReadOnlyEdit m_edEntranceX;
    CReadOnlyEdit m_edEntranceY;
    CComboBox m_cbDirection;
    CComboBox m_cbActive;
    CACEdit m_cbID;
    void SetDisableControl(bool bDisable);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

    virtual BOOL OnInitDialog();
};
