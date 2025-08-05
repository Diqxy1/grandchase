#pragma once
#include "stdafx.h"
#include "struct.h"
#include "ACEdit.h"
#include "DlgTileBG.h"
// CDlgQuest 대화 상자입니다.
class CQuestScripterDlg;

class CDlgQuest : public CDialog
{
	DECLARE_DYNAMIC(CDlgQuest)

public:
	CDlgQuest(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgQuest();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_QUEST };

    CDlgTileBG      m_dlgTileBG;

    bool            m_bCreated;
    CStatic         m_stRowColGroup;
    CStatic         m_stRowCol;
    CButton         m_btReSize;
    void SetValue(Quest *quest);
    void GetValue(Quest *quest);

    void SetDisableControl(bool bDisable);
    void SetRowColText(int iRow, int iCol);

    CQuestScripterDlg *m_pQuestScripter;
    void SetComboBoxValue(CQuestScripterDlg *pQuestScripter);
    void SetMainDlgPointer(CQuestScripterDlg *pQuestScripter) { m_pQuestScripter = pQuestScripter; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnBnClickedButtonResize();
    afx_msg void OnBnClickedRadioDirection();
    afx_msg void OnBnClickedRadioStage();
    CButton m_chSortDirectionStage;
};
