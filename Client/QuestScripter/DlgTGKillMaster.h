#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "KillMaster.h"
#include "ACEdit.h"
#include "afxwin.h"

// CDlgTGKillMaster 대화 상자입니다.

class CDlgTGKillMaster : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGKillMaster)

public:
	CDlgTGKillMaster(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGKillMaster();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_KILLMASTER };
    CKillMaster m_KillMaster;
    void InitValue(void) { m_KillMaster.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CKillMaster*)pValue = m_KillMaster; }
    void SetValue(CBaseTGFunction *pValue) { m_KillMaster = *(CKillMaster*)pValue; }
    void Refresh(void);
    void CalcResult(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    CComboBox m_cbSlot;
    CACEdit m_cbMonID;
    CStatic m_stKillMaster1;
    CStatic m_stKillMaster2;
	CComboBox m_cbMonsterIsDie;
};
