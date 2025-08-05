#pragma once

#include "resource.h"
#include "stdafx.h"

// KCChooseCategoryDlg 대화 상자입니다.

class KCChooseCategoryDlg : public CDialog
{
	DECLARE_DYNAMIC(KCChooseCategoryDlg)

public:
	KCChooseCategoryDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCChooseCategoryDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_CHECK_CATEGORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listCategory;
	virtual BOOL OnInitDialog();
	afx_msg void OnHdnItemclickCheckCategoryList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemActivateCheckCategoryList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};
