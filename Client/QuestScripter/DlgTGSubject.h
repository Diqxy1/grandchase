#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "DlgTGBase.h"
#include "ACEdit.h"
#include "Subject.h"


// CDlgTGSubject 대화 상자입니다.

class CDlgTGSubject : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGSubject)

public:
	CDlgTGSubject(CWnd* pParent = NULL);
	virtual ~CDlgTGSubject();

	enum { IDD = IDD_DIALOG_TG_SUBJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedButtonAddSubject();
    afx_msg void OnBnClickedButtonAddCharType();
    afx_msg void OnBnClickedButtonAddLevel();
    afx_msg void OnBnClickedButtonAddJobLevel();
    afx_msg void OnNmItemdblclickListSubject(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLbDBClickedCharType();
    afx_msg void OnLbDBClickedLevel();
    afx_msg void OnLbDBClickedJobLevel();

public:
    CSubject m_TGSubject;

public:
    void InitValue(void) { m_TGSubject.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CSubject*)pValue = m_TGSubject; }
    void SetValue(CBaseTGFunction *pValue) { m_TGSubject = *(CSubject*)pValue; }
    void Refresh(void);
    void CalcResult(void);

public:
    CEdit m_ebSubjectType;
    CEdit m_ebSubjectRadio;
    CButton m_bbAddSubject;
    CListCtrl m_lcSubject;

    CEdit m_ebCharType;
    CButton m_bbAddCharType;
    CListBox m_lbCharType;

    CEdit m_ebLevel;
    CButton m_bbAddLevel;
    CListBox m_lbLevel;

    CEdit m_ebJobLevel;
    CButton m_bbAddJobLevel;
    CListBox m_lbJobLevel;
};
