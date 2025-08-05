#pragma once
#include "stdafx.h"

// CDlgTileRowCol 대화 상자입니다.

class CDlgTileRowCol : public CDialog
{
	DECLARE_DYNAMIC(CDlgTileRowCol)

public:
	CDlgTileRowCol(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTileRowCol();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TILE_ROW_COL };

	int m_iRow;
	int m_iCol;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_ebRow;
	CEdit m_ebCol;
};
