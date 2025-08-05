#pragma once
#include "stdafx.h"
#include "resource.h"
#include "DlgTGBase.h"
#include "MonsterGenerater.h"
#include "ACEdit.h"
#include "afxwin.h"

// CDlgTGMonsterGenerater 대화 상자입니다.

class CDlgTGMonsterGenerater : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGMonsterGenerater)

public:
	CDlgTGMonsterGenerater(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGMonsterGenerater();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_MONSTERGENERATER };

    CMonsterGenerater m_MonsterGenerater;
    void InitValue(void) { m_MonsterGenerater.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CMonsterGenerater*)pValue = m_MonsterGenerater; }
    void SetValue(CBaseTGFunction *pValue) { m_MonsterGenerater = *(CMonsterGenerater*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
public:
    CComboBox m_cbSlot;
    CACEdit m_cbMonID;
    CComboBox m_cbBoss;
    CComboBox m_cbRight;
    CEdit m_ebX;
    CEdit m_ebY;
    CEdit m_ebPercent;
    afx_msg void OnBnClickedButton1();
    CStatic m_stMonGen1;
    CStatic m_stMonGen2;
    CStatic m_stMonGen3;
    CStatic m_stMonGen4;
    CStatic m_stMonGen5;
    CStatic m_stMonGen6;
    CStatic m_stMonGen7;
    CEdit m_edMonsterLevel;
	CButton m_chChapion;
    CButton m_rdRandom;
    CButton m_rdPosition;
	afx_msg void OnBnClickedCheckChampion();
public:
    afx_msg void OnBnClickedRadioMongen1();
public:
    afx_msg void OnBnClickedRadioMongen2();
};
