#pragma once
#include "resource.h"

// CMoveObjectDlg 대화 상자입니다.

class CMoveObjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CMoveObjectDlg)

public:
	CMoveObjectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMoveObjectDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_OBJECT_MOVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strLayerNum;
	CString m_strCurrentLayerNum;
	virtual INT_PTR DoModal();
};
