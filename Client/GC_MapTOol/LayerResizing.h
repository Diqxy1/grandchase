#pragma once

#include "resource.h"

// CLayerResizing 대화 상자입니다.

class CLayerResizing : public CDialog
{
	DECLARE_DYNAMIC(CLayerResizing)

public:
	CLayerResizing(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLayerResizing();

	CString GetWidth() { return m_strWidth; }
	CString GetHeight() { return m_strHeight; }
	BOOL GetCheckAll() { return m_bCheckAll; }

	void SetSize(int Width, int Height);

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_LAYER_RESIZING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strWidth;
	CString m_strHeight;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CButton m_btnAllLayer;
	BOOL m_bCheckAll;
};
