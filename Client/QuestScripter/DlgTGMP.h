#pragma once
#include "DlgTGBase.h"
#include "MP.h"
#include "ACEdit.h"
#include "stdafx.h"

// CDlgTGMP 대화 상자입니다.
class CDlgTrigger;

class CDlgTGMP : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGMP)

public:
	CDlgTGMP(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGMP();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_MP };

    void SetMonsterInfo(bool bPlayer, int iSlot, int iType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

    CComboBox   m_cbPlayerOrMonster;
    CComboBox   m_cbSlot;
    CACEdit     m_cbMonsterID;
    CEdit       m_ebMP;
    CStatic     m_stTG_MP1;
    CStatic     m_stTG_MP2;

    CMP m_TGMP;
    void InitValue(void) { m_TGMP.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CMP*)pValue = m_TGMP; }
    void SetValue(CBaseTGFunction *pValue) { m_TGMP = *(CMP*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

    afx_msg void OnBnClickedButtonSelectFromMapSelMon();
    afx_msg void OnCbnSelchangeComboPlayerOrMonster();

    CButton m_btMonster;
    void ResetSlotByMonster(void);
    void ResetSlotByPlayer(void);
};
