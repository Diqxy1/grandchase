#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "CheckWall.h"


// CDlgTGCheckWall 대화 상자입니다.

class CDlgTGCheckWall : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGCheckWall)

public:
	CDlgTGCheckWall(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGCheckWall();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_CHECK_WALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	void InitValue(void);
	void GetValue(CBaseTGFunction *pValue) { *(CCheckWall*)pValue = m_TGCheckWall; }
	void SetValue(CBaseTGFunction *pValue) { m_TGCheckWall = *(CCheckWall*)pValue; }

	void Refresh(void);
	void CalcResult(void);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_ebWallSlot;
	CComboBox m_cbIsLive;
	CCheckWall	m_TGCheckWall;

public:
	afx_msg void OnCbnSelchangeComboSlotType();
};
