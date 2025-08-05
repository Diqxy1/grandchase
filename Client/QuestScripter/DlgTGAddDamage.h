#pragma once
#include "DlgTGBase.h"
#include "AddDamage.h"
#include "ACEdit.h"
#include "stdafx.h"
//#include <map>

// CDlgTGAddDamage 대화 상자입니다.
class CDlgTGOptionWaterPole;

class CDlgTGAddDamage : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGAddDamage)

public:
	CDlgTGAddDamage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGAddDamage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_ADD_DAMAGE };

    enum DAMAGE_OPTION_TYPE { DOT_WATER_POLE ,DOT_JUMP_ZONE};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonSelectFromMap();
    CEdit           m_ebEntranceX;
    CEdit           m_ebEntranceY;
    CEdit           m_ebReCycle;
    CACEdit         m_cbDamageID;
    CStatic         m_stAddDamage_1;
    CStatic         m_stAddDamage_2;

    std::map<DAMAGE_OPTION_TYPE, CDialog*> m_mapDlgOption;

    CAddDamage m_TGAddDamage;
    void InitValue(void) { m_TGAddDamage.Init(); }
    void GetValue(CBaseTGFunction *pValue);
    void SetValue(CBaseTGFunction *pValue);

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnCbnSelchangeComboDamageid();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CComboBox m_cbDir;
	CEdit m_ebStartTime;
};
