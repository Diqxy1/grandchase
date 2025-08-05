#pragma once
#include "afxwin.h"


// KEditDialog 대화 상자입니다.
struct KD3DWndClass;

class KEditDialog : public CDialog
{
	DECLARE_DYNAMIC(KEditDialog)

public:
	KEditDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KEditDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EDIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CString m_strValue;
	afx_msg void OnAddDelete();
	afx_msg void OnTreeDelete();

//------------------------------------------------------------------------
// 20091012 이동석 
public:
	int m_iBoxType;
	CString	m_strBoxType;
	CComboBox m_ComboBoxBoxType;
	
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnCbnSelchangeComboBoxtype();
//========================================================================
};
