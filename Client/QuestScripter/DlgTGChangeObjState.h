#pragma once
#include "stdafx.h"
#include "DlgTGBase.h"
#include "ChangeObjState.h"
#include "ACEdit.h"
#include "afxwin.h"

// CDlgTGChangeObjState 대화 상자입니다.

class CDlgTGChangeObjState : public CDlgTGBase
{
	DECLARE_DYNAMIC(CDlgTGChangeObjState)

public:
	CDlgTGChangeObjState(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTGChangeObjState();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TG_CHANGE_OBJ_STATE };

    CChangeObjState m_TGChangeObjState;
    void InitTGName(void);
    void InitValue(void) { m_TGChangeObjState.Init(); }
    void GetValue(CBaseTGFunction *pValue) { *(CChangeObjState*)pValue = m_TGChangeObjState; }
    void SetValue(CBaseTGFunction *pValue) { m_TGChangeObjState = *(CChangeObjState*)pValue; }    

    void Refresh(void);
    void CalcResult(void);

    void SetStateByObjType(int iObjType);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CComboBox    m_cbSlot;
    CACEdit      m_cbType;
    CACEdit      m_cbState;
    CStatic      m_stChangeObjState_1;
    CStatic      m_stChangeObjState_2;
	CComboBox m_cbObjType;
	afx_msg void OnCbnSelendokComboChangeObjStateObj2();
	CStatic m_stChangeObjState_3;
};
