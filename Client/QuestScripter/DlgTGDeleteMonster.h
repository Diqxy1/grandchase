#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "ACEdit.h"
#include "DeleteMonster.h"

// CDlgTGDeleteMonster 대화 상자입니다.

class CDlgTGDeleteMonster : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGDeleteMonster)

public:
	CDlgTGDeleteMonster(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGDeleteMonster();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_DELETE_MONSTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
    CComboBox   m_cbSlot;
    CACEdit     m_cbMonsterID;
    CStatic     m_stDeleteMonster1;
    CStatic     m_stDeleteMonster2;
    CButton     m_chDeleteAll;

    CDeleteMonster m_TGDeleteMonster;
    void InitValue(void) { m_TGDeleteMonster.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CDeleteMonster*)pValue = m_TGDeleteMonster; }
    void SetValue(CBaseTGFunction *pValue) { m_TGDeleteMonster = *(CDeleteMonster*)pValue;  }    

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedCheckDeleteAll();
};
