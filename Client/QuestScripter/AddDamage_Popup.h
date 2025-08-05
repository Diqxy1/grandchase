#pragma once
#include "stdafx.h"
#include "resource.h"
#include "AddDamage.h"
//#include <vector>
//#include <map>
#include "DlgTGAddDamage.h"
        
// CAddDamage_Popup 대화 상자입니다.

class CAddDamage_Popup : public CDialog
{
	DECLARE_DYNAMIC(CAddDamage_Popup)

public:
	CAddDamage_Popup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddDamage_Popup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ADDDAMAGE_POPUP };

    bool m_bFnAction;
    CAddDamage m_AddDamage;

    void SetMapDamageList(std::vector<CAddDamage*> *pvtDamageList);
    std::vector<CAddDamage*> *m_pvtDamageList;

    int m_iDefWidth;
    std::map<CDlgTGAddDamage::DAMAGE_OPTION_TYPE, CDialog*> m_mapDlgOption;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedDelete();
    CComboBox m_cbID;
    CEdit m_ebEntranceX;
    CEdit m_ebEntranceY;
    CEdit m_ebRecycleTime;
    CComboBox m_cbFunctionType;
    afx_msg void OnCbnSelchangeComboId();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CComboBox m_cbDir;
	CEdit m_ebStartTime;
};
