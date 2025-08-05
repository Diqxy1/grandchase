#pragma once

// CDlgDefaultDirection 대화 상자입니다.

class CDlgDefaultDirection : public CDialog
{
	DECLARE_DYNAMIC(CDlgDefaultDirection)

public:
	CDlgDefaultDirection(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgDefaultDirection();

	bool    m_bDirectionMode;
	int     m_iDirection;

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DEFAULT_DIRECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	CButton m_rdDel;
	CButton m_rdDirection;

	CButton m_btRight;
	CButton m_btBottom;
	CButton m_btLeft;
	CButton m_bttop;
	afx_msg void OnBnClickedRadioDirection();
	afx_msg void OnBnClickedRadioDel();
	virtual BOOL OnInitDialog();
};
