#pragma once
#include "afxcmn.h"
#include "D3DWndClass.h"
#include "KWndDataPackage.h"

// KInsertDialog 대화 상자입니다.

class KInsertDialog : public CDialog
{
	DECLARE_DYNAMIC(KInsertDialog)

public:
	KInsertDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInsertDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INSERT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemActivateList1(NMHDR *pNMHDR, LRESULT *pResult);
	int OnInitDialog();
	void GetScriptWindow(KD3DWndClass* pkWnd);
	CListCtrl m_CtrlValueList;
	KWndDataPackage m_WndPackager;
//	afx_msg void OnInitMenu(CMenu* pMenu);
protected:
	virtual void OnOK();
};
