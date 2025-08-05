#pragma once
#include "afxcmn.h"


// CDlgFindParticle 대화 상자입니다.

class CDlgFindParticle : public CDialog
{
	DECLARE_DYNAMIC(CDlgFindParticle)

public:
	CDlgFindParticle(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgFindParticle();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_FIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	CString m_strFind;
	afx_msg void OnEnChangeEdit1();
};
