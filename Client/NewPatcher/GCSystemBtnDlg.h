#pragma once


// CGCSystemBtnDlg 대화 상자입니다.
class CNBitmapButton;
class Komfile;

class CGCSystemBtnDlg : public CDialog
{
	DECLARE_DYNAMIC(CGCSystemBtnDlg)

public:
	CGCSystemBtnDlg( CWnd* pParent /*=NULL*/, boost::shared_ptr<Komfile> imagemass );   // 표준 생성자입니다.
	virtual ~CGCSystemBtnDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SYSTEM_DLG };

	enum {
		CBB_SYSTEM_BUTTON_MINIMUM = IDC_SYSTEM_BUTTON,
		CBB_SYSTEM_BUTTON_CLOSE,
		CBB_SYSTEM_BUTTON_MAX
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	boost::shared_ptr<Komfile> m_pMassfile;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetLinkButtonPosition();
	void CreateButtonList();
	void OnClickMinimum();
	void OnClickClose();
	void ReleaseAll();

	std::map<int, CNBitmapButton*> m_mapSystemButton;

	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

};
