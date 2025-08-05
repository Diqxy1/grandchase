#pragma once


// CGCMainMenuDlg 대화 상자입니다.
class CNBitmapButton;
class Komfile;

class CGCMainMenuDlg : public CDialog
{
	DECLARE_DYNAMIC(CGCMainMenuDlg)

public:
	CGCMainMenuDlg(CWnd* pParent /*=NULL*/, boost::shared_ptr<Komfile> imagemass);   // 표준 생성자입니다.
	virtual ~CGCMainMenuDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MAIN_MENU };

	enum {
		CBB_LINK_BUTTON_JOIN = IDC_LINK_BUTTON,
		CBB_LINK_BUTTON_HOME,
		CBB_LINK_BUTTON_BEGINNER,
		CBB_LINK_BUTTON_DOWNLOAD,
		CBB_LINK_BUTTON_FORUM,
		CBB_LINK_BUTTON_ONLINE_SERVICE,
		CBB_LINK_BUTTON_MAX,
	};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	boost::shared_ptr<Komfile> m_pMassfile;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetLinkButtonPosition();
	void CreateLinkButtonList();
	void OnClickJoin();
	void ReleaseAll();
	std::map<int, CNBitmapButton*> m_mapLinkButton;

	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	void OnClickHome();
	void OnClickBeginner();
	void OnClickDownload();
	void OnClickForum();
	void OnClickService();
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};
