#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "KillAllMonster.h"
#include "ACEdit.h"
#include "afxwin.h"

// CDlgTGKillMaster 대화 상자입니다.

class CDlgTGKillAllMonster : public CDlgTGBase
{
    DECLARE_DYNAMIC(CDlgTGKillAllMonster)

public:
    CDlgTGKillAllMonster(CWnd* pParent = NULL);   // 표준 생성자입니다.
    virtual ~CDlgTGKillAllMonster();

    // 대화 상자 데이터입니다.
    enum { IDD = IDD_DIALOG_TG_KILL_ALL_MONSTER };
    CKillAllMonster m_KillAllMonster;
    void InitValue(void) { m_KillAllMonster.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CKillAllMonster*)pValue = m_KillAllMonster; }
    void SetValue(CBaseTGFunction *pValue) { m_KillAllMonster = *(CKillAllMonster*)pValue; }

    void Refresh(void);
    void CalcResult(void);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    CStatic m_stKillAllMonster;
};
