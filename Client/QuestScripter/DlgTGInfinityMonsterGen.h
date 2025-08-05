#pragma once
#include "DlgTGBase.h"
#include "InfinityMonsterGen.h"
#include "stdafx.h"

// CDlgTGInfinityMonsterGen 대화 상자입니다.

class CDlgTGInfinityMonsterGen : public CDlgTGBase
{
    DECLARE_DYNAMIC(CDlgTGInfinityMonsterGen)

public:
    CDlgTGInfinityMonsterGen(CWnd* pParent = NULL);   // 표준 생성자입니다.
    virtual ~CDlgTGInfinityMonsterGen();

    // 대화 상자 데이터입니다.
    enum { IDD = IDD_DIALOG_TG_INFINITY_MON_GEN };

    CInfinityMonsterGen m_InfinityMonsterGen;
    void InitValue(void) { m_InfinityMonsterGen.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CInfinityMonsterGen*)pValue = m_InfinityMonsterGen; }
    void SetValue(CBaseTGFunction *pValue) { m_InfinityMonsterGen = *(CInfinityMonsterGen*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
public:
    CStatic m_strInfinityGen;
};
