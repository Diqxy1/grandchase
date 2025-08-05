#pragma once
#include "stdafx.h"
#include "ReadOnlyEdit.h"

// CDlgHelp 대화 상자입니다.

class CDlgHelp : public CDialog
{
	DECLARE_DYNAMIC(CDlgHelp)

public:
	CDlgHelp(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgHelp();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_HELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CReadOnlyEdit m_edHelp;
	virtual BOOL OnInitDialog();
};
