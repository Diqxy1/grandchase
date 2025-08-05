#pragma once


// CGCServerDlg 대화 상자입니다.
class CNBitmapButton;
class Komfile;

class CGCServerDlg : public CDialog
{
	DECLARE_DYNAMIC(CGCServerDlg)

public:
	CGCServerDlg(CWnd* pParent /*=NULL*/, boost::shared_ptr<Komfile> imagemass);
	virtual ~CGCServerDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SERVER };

	enum {
        CBB_SEVER_BUTTON_NONE = 0,
		CBB_SEVER_BUTTON_N_1 = IDC_SERVER_BUTTON,
		CBB_SEVER_BUTTON_N_2,
		CBB_SEVER_BUTTON_N_3,
		CBB_SEVER_BUTTON_N_4,
		CBB_SEVER_BUTTON_N_5,
		CBB_SEVER_BUTTON_N_6,
		CBB_SEVER_BUTTON_S_1,
		CBB_SEVER_BUTTON_S_2,
		CBB_SEVER_BUTTON_S_3,
		CBB_SEVER_BUTTON_MAX
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

protected:
	std::map<int, CNBitmapDecoButton*> m_mapServerButton;
	void CreateLinkButtonList();
	boost::shared_ptr<Komfile> m_pMassfile;
	
	CBitmap*        m_pkBitMap;
	CColorStatic	m_stServerName;
	CFont    m_kFont;

public:
	virtual BOOL OnInitDialog();
	void SetLinkButtonPosition();
	afx_msg void OnPaint();
	void ReleaseAll();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	void SetLinkButtonInfo();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

    HWND m_hStartBtn;
    void SetHWNDStartBtn( HWND starBtn_ ) { m_hStartBtn = starBtn_; }
};
