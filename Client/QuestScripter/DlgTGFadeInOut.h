#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "ACEdit.h"
#include "FadeInOut.h"
//#include <map>

// CDlgTGFadeInOut 대화 상자입니다.

class CDlgTGFadeInOut : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGFadeInOut)

public:
	CDlgTGFadeInOut(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGFadeInOut();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_FADE_IN_OUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CComboBox   m_cbFadeInOut;
    std::map<CString, int> m_mapQuestEnum;

    CFadeInOut m_TGFadeInOut;
    void InitValue(void) { m_TGFadeInOut.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CFadeInOut*)pValue = m_TGFadeInOut; }
    void SetValue(CBaseTGFunction *pValue) { m_TGFadeInOut = *(CFadeInOut*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
public:
	CEdit editRed;
public:
	CEdit editGreen;
public:
	CEdit editBlue;
public:
	CComboBox comboSpeed;
};
