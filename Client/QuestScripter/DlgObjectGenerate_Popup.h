#pragma once
#include "stdafx.h"
#include "resource.h"
#include "ObjectGenerater.h"
//#include <vector>

// CDlgObjectGenerate_Popup 대화 상자입니다.

class CDlgObjectGenerate_Popup : public CDialog
{
	DECLARE_DYNAMIC(CDlgObjectGenerate_Popup)

public:
	CDlgObjectGenerate_Popup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgObjectGenerate_Popup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_OBJECTGENERATE_POPUP };

    bool m_bFnAction;
    CObjectGenerater objectgen;

    void SetMapObjectList(std::vector<CObjectGenerater*> *pvtObjList);
    std::vector<CObjectGenerater*> *m_pvtObjList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CComboBox m_cbType;
    CComboBox m_cbCrashable;
    CEdit m_ebEntranceX;
    CEdit m_ebEntranceY;
    CComboBox m_cbRight;
    CComboBox m_cbGenerate;
    CComboBox m_cbFunctionType;

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedDelete();
};
