#pragma once

#include "resource.h"

// CChangeLayerDlg 대화 상자입니다.

class CChangeLayerDlg : public CDialog
{
	DECLARE_DYNAMIC(CChangeLayerDlg)

public:
	CChangeLayerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CChangeLayerDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_LAYER_CHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strLayerNum;
	CString m_strCurrentLayerNum;
	virtual INT_PTR DoModal();
	afx_msg void OnBnClickedOk();
};
