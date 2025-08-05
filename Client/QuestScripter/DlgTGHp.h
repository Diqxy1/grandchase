#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "ACEdit.h"
#include "HP.h"

// CDlgTGHp 대화 상자입니다.

class CDlgTGHp : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGHp)

public:
	CDlgTGHp(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGHp();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_HP };

    void SetMonsterInfo(bool bPlayer, int iSlot, int iType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CComboBox   m_cbPlayerOrMonster;
    CComboBox   m_cbSlot;
    CACEdit     m_cbMonsterID;
    CEdit       m_ebHP;
    CStatic     m_stTG_HP1;
    CStatic     m_stTG_HP2;

    CHP m_TGHP;
    void InitValue(void) { m_TGHP.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CHP*)pValue = m_TGHP; }
    void SetValue(CBaseTGFunction *pValue) { m_TGHP = *(CHP*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

    afx_msg void OnBnClickedButtonSelectFromMapSelMon();
    afx_msg void OnCbnSelchangeComboPlayerOrMonster();

    void ResetSlotByMonster(void);
    void ResetSlotByPlayer(void);
    CButton m_btMonster;
};
