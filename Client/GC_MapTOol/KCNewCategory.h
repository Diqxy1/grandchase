#pragma once

// KCNewCategory 대화 상자입니다.

class KCNewCategory : public CDialog
{
	DECLARE_DYNCREATE(KCNewCategory)
public:
	KCNewCategory(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCNewCategory();
	// 재정의
	afx_msg void OnButtonOK();
	afx_msg void OnButtonCancel();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEW_CATEGORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNewCategoryName;
	afx_msg void OnBnClickedNewNameVerify();
	CButton m_btnOK;
	afx_msg void OnEnChangeNewCategoryname();
};
