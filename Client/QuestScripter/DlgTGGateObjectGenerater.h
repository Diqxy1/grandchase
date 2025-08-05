#pragma once

#include "DlgTGBase.h"
#include "resource.h"
#include "GateObjectGenerater.h"
#include "ACEdit.h"
#include "stdafx.h"
// CDlgTGGateObjectGenerater 대화 상자입니다.

class CDlgTGGateObjectGenerater : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGGateObjectGenerater)

public:
	CDlgTGGateObjectGenerater(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGGateObjectGenerater();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_GATEOBJECTGENERATER };

    CComboBox m_cbOnOff;

    CGateObjectGenerater m_TGGateObjectGenerater;
    void InitValue(void) { m_TGGateObjectGenerater.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CGateObjectGenerater*)pValue = m_TGGateObjectGenerater; }
    void SetValue(CBaseTGFunction *pValue) { m_TGGateObjectGenerater = *(CGateObjectGenerater*)pValue; }   

    void Refresh(void);
    void CalcResult(void);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

    CComboBox               m_cbSlot;
    CACEdit                 m_cbType;
    CComboBox               m_cbRight;
    CComboBox               m_cbGen;
    CEdit                   m_ebEntranceX;
    CEdit                   m_ebEntranceY;
    CComboBox               m_cbDirection;
    CStatic                 m_stObjGen1;
    CStatic                 m_stObjGen2;
    CStatic                 m_stObjGen3;
    CStatic                 m_stObjGen4;
    CEdit                   m_ebDelay;
    CStatic                 m_stObjGen5;
    CEdit                   m_ebUseSelect;
    CEdit                   m_ebDotaStage;
    afx_msg void OnBnClickedButton1();
    int ConvertToImageNumber(int iGateNum);
    int ConvertToComboBoxNumber(int iGateNum);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnCbnSelchangeComboDirection();
	afx_msg void OnCbnSelchangeComboObjType();
    afx_msg void OnStnClickedStaticObjgen7();
};
