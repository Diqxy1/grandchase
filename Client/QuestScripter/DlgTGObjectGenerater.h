#pragma once
#include "DlgTGBase.h"
#include "resource.h"
#include "ObjectGenerater.h"
#include "ACEdit.h"
#include "stdafx.h"

// CDlgTGObjectGenerater 대화 상자입니다.

class CDlgTGObjectGenerater : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGObjectGenerater)

public:
	CDlgTGObjectGenerater(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGObjectGenerater();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_OBJECTGENERATER };

    CComboBox m_cbOnOff;

    CObjectGenerater m_TGObjectGenerater;
    void InitValue(void) { m_TGObjectGenerater.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CObjectGenerater*)pValue = m_TGObjectGenerater; }
    void SetValue(CBaseTGFunction *pValue) { m_TGObjectGenerater = *(CObjectGenerater*)pValue; }   

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CComboBox               m_cbSlot;
    CACEdit                 m_cbType;
    CComboBox               m_cbRight;
    CComboBox               m_cbGen;
    CEdit                   m_ebEntranceX;
    CEdit                   m_ebEntranceY;
    afx_msg void OnBnClickedButton1();
    CStatic m_stObjGen1;
    CStatic m_stObjGen2;
    CStatic m_stObjGen3;
    CStatic m_stObjGen4;
    CComboBox m_cbCrashable;
	afx_msg void OnCbnSelchangeComboObjType();
	CEdit m_edJump;
};
