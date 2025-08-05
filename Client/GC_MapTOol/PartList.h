#pragma once
#include "stdafx.h"

// CPartList 대화 상자입니다.

class CPartList : public CDialog
{
	DECLARE_DYNAMIC(CPartList)

public:
	CPartList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPartList();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_PARTLIST };

	void SetCategoryMember(std::string CategoryName);
	void ResetContent();
	void UpdateCategoryList();
	void EnableWindow(BOOL bEnable = TRUE);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_lbCategory;
	afx_msg void OnLbnSelchangeCategory();
	CListBox m_lbParts;
	afx_msg void OnLbnSelchangeParts();
};
