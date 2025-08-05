#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "ACEdit.h"
#include "UnitParticle.h"

// CDlgTGUnitParticle 대화 상자입니다.

class CDlgTGUnitParticle : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGUnitParticle)

public:
	CDlgTGUnitParticle(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGUnitParticle();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_UNIT_PARTICLE };

    void SetMonsterInfo(bool bPlayer, int iSlot, int iType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CComboBox       m_cbPlayerOrMonster;
    CComboBox       m_cbSlot;
    CACEdit         m_cbMonsterID;
    CACEdit         m_cbParticleID;

    CUnitParticle m_TGUnitParticle;
    void InitValue(void) { m_TGUnitParticle.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CUnitParticle*)pValue = m_TGUnitParticle; }
    void SetValue(CBaseTGFunction *pValue) { m_TGUnitParticle = *(CUnitParticle*)pValue; }   

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    CStatic m_stUnitParticle1;
    CStatic m_stUnitParticle2;

    void ResetSlotByMonster(void);
    void ResetSlotByPlayer(void);

    afx_msg void OnBnClickedButtonSelectFromMapSelMon();
    afx_msg void OnCbnSelchangeComboPlayerOrMonster();
    CButton m_btMonster;
};
