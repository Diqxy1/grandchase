#pragma once


// CGCStartBtnDlg 대화 상자입니다.

class CGCStartBtnDlg : public CDialog
{
	DECLARE_DYNAMIC(CGCStartBtnDlg)

public:
	CGCStartBtnDlg( CWnd* pParent /*=NULL*/, boost::shared_ptr<Komfile> imagemass );    // 표준 생성자입니다.
	virtual ~CGCStartBtnDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_START_BTN };

	enum {
		CBB_START_BUTTON_SERVER_SELECT = IDC_START_BUTTON,
		CBB_START_BUTTON_DOWNLOAD,
		CBB_START_BUTTON_MAX
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
	void ReleaseAll();

	std::map<int, CNBitmapButton*> m_mapButton;

	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	
	void OnServerSelectBtn();
	void OnStartGameBtn();
	void OnClickServerSelect();
	void OnClickStartGame();
	void OnComplete( bool success, bool bMsgBox );
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

    void SetDisbleServerSelectBtn(BOOL bDisable);
    afx_msg LRESULT SetServerSelectBtn( WPARAM wParam, LPARAM lParam );
};
