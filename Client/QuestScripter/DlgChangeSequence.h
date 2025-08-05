#pragma once
#include "stdafx.h"
//#include <vector>
using namespace std;

// CDlgChangeSequence 대화 상자입니다.

class CDlgChangeSequence : public CDialog
{
	DECLARE_DYNAMIC(CDlgChangeSequence)

public:
	CDlgChangeSequence(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgChangeSequence();

    int m_iSequence;
    std::vector<int> m_vtChangeSequence;
    void SetChangeSequenceList(std::vector<int> vtChangeSequence);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CHANGE_SEQUENCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    CComboBox m_cbChangeSequence;
};
