#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "ACEdit.h"
#include "UnitMovement.h"

// CDlgTGUnitMovement 대화 상자입니다.

class CDlgTGUnitMovement : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGUnitMovement)

public:
	CDlgTGUnitMovement(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGUnitMovement();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_UNIT_MOVEMENT };

    void SetMonsterInfo(bool bPlayer, int iSlot, int iType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CComboBox   m_cbSlot;
    CComboBox   m_cbPlayerOrMonster;
    CACEdit     m_cbMonsterID;
    CComboBox   m_cbMoveMent;
    CStatic     m_stUnitMoveMent1;
    CStatic     m_stUnitMoveMent2;

    CUnitMovement m_TGUnitMovement;
    void InitValue(void) { m_TGUnitMovement.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CUnitMovement*)pValue = m_TGUnitMovement; }
    void SetValue(CBaseTGFunction *pValue) { m_TGUnitMovement = *(CUnitMovement*)pValue; }

    void Refresh(void);
    void CalcResult(void);

    void ResetSlotByMonster(void);
    void ResetSlotByPlayer(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

    afx_msg void OnBnClickedButtonSelectFromMapSelMon();
    afx_msg void OnCbnSelchangeComboPlayerOrMonster();
    CButton m_btMonster;
};
