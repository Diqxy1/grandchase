#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "Position.h"

// CDlgTGPosition 대화 상자입니다. 
class CDlgTGPosition : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGPosition)

public:
	CDlgTGPosition(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGPosition();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_POSITION };

    void ResetSlotByMonster(void);
    void ResetSlotByPlayer(void);

    CPosition m_Position;
    void InitValue(void) { m_Position.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CPosition*)pValue = m_Position; }
    void SetValue(CBaseTGFunction *pValue) { m_Position = *(CPosition*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

    void SetMonsterInfo(bool bPlayer, int iSlot, int iType);
    void RectRevision(CRect &rtArea);
    void SetArea(CRect rtArea);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CComboBox   m_cbSlot;
    CComboBox   m_cbPlayer_or_Monster;
    CComboBox   m_cbMonID;
    CComboBox   m_cbHalfMore;
    CEdit       m_ebLeft;
    CEdit       m_ebRight;
    CEdit       m_ebTop;
    CEdit       m_ebBottom;
    CButton     m_btMonster;
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedButtonSelectFromMapSelMon();
    afx_msg void OnBnClickedButtonSelectFromMapSelArea();
    afx_msg void OnCbnSelchangeComboPlayerOrMonster();
    afx_msg void OnCbnSelchangeComboHalfMore();
    CComboBox m_cbInOut;
    CStatic m_stPosition1;
    CStatic m_stPosition2;
};
