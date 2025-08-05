#pragma once


// CDlgScriptEdit 대화 상자입니다.

class CDlgScriptEdit : public CDialog
{
	DECLARE_DYNAMIC(CDlgScriptEdit)

public:
	CDlgScriptEdit(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgScriptEdit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_SCRIPTEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	bool CheckScript();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strScript;
	afx_msg void OnBnClickedOk();
};
