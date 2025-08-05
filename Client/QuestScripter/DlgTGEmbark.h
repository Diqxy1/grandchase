#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "DlgTGBase.h"
#include "ACEdit.h"
#include "Embark.h"


// CDlgTGEmbark 대화 상자입니다.

class CDlgTGEmbark : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGEmbark)

public:
	CDlgTGEmbark(CWnd* pParent = NULL);
	virtual ~CDlgTGEmbark();

	enum { IDD = IDD_DIALOG_TG_EMBARK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
    CEmbark m_TGEmbark;

public:
    void InitValue(void) { m_TGEmbark.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CEmbark*)pValue = m_TGEmbark; }
    void SetValue(CBaseTGFunction *pValue) { m_TGEmbark = *(CEmbark*)pValue; }
    void Refresh(void);
    void CalcResult(void);

    //afx_msg void OnCbnSelchangeComboEmbarkType();
    //afx_msg void OnCbnSelchangeComboEmbarkRight();
    //afx_msg void OnCbnSelchangeComboEmbarkationCharSlot();

public:
    CComboBox m_cbEmbarkType;
    CComboBox m_cbIsRight;
    CEdit m_ebPosX;
    CEdit m_ebPosY;
    CEdit m_ebEmbarkSlot;
    CComboBox m_cbEmbarkationCharSlot;
};
