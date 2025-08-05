#pragma once
#include "stdafx.h"
#include "resource.h"
#include "GateObjectGenerater.h"
//#include <vector>

// CDlgGateObject_Popup 대화 상자입니다.

class CDlgGateObject_Popup : public CDialog
{
	DECLARE_DYNAMIC(CDlgGateObject_Popup)

public:
	CDlgGateObject_Popup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgGateObject_Popup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_GATEOBJECTGENERATE_POPUP };

    bool m_bFnAction;
    CGateObjectGenerater m_GateObject;

    void SetMapObjectList(std::vector<CGateObjectGenerater*> *pvtGateObjList);
    std::vector<CGateObjectGenerater*> *m_pvtGateObjList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CComboBox       m_cbDirection;
    CComboBox       m_cbType;
    CEdit           m_ebEntranceX;
    CEdit           m_ebEntranceY;
    CEdit           m_ebDelay;
    CComboBox       m_cbRight;
    CComboBox       m_cbGenerate;
    CComboBox       m_cbFunctionType;

    // 이미지 넘버를 넣으면 콤보박스 번호로 리턴해준다.
    int ConvertToComboBoxNumber(int iGateNum);
    // 콤보박스의 0, 1, 2, 3을 기준으로 left right up down 값을 판단하여 리턴
    int ConvertToImageNumber(int iGateNum);

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedDelete();
};
