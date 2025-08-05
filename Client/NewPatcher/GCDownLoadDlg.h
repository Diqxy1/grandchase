#pragma once


// CGCDownLoadDlg 대화 상자입니다.

class CGCDownLoadDlg : public CDialog
{
	DECLARE_DYNAMIC(CGCDownLoadDlg)

public:
	CGCDownLoadDlg(CWnd* pParent /*=NULL*/, boost::shared_ptr<Komfile> imagemass,
		boost::shared_ptr<KGCStringLoader> strLoader);   // 표준 생성자입니다.
	virtual ~CGCDownLoadDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DOWNLOAD_DLG };
	
	enum {
		CBB_DOWNLOAD_WEB_VIEW_STATIC = IDC_DOWNLOAD_DLG,
		CBB_DOWNLOAD_TOTAL_PROGRESS,
		CBB_DOWNLOAD_PACKAGE,
		CBB_DOWNLOAD_SERVER_RESELECT,

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
	CBitmap*        m_pkBitMapProgress;
	int				m_iTotalProgress;
	CGCHtmlView     *m_pHtmlPR;
	CStatic         m_pkWebStatic;
	KProgressCtrl       m_kProgressTotal;

	CColorStatic	m_stServerName;
	//CColorStatic	m_stServer;
	//CColorStatic	m_stServerName;
	CColorStatic	m_stPackage;
	CColorStatic	m_stTime;
	CColorStatic	m_stSpeed;
	CFont    m_kFont;
	boost::shared_ptr<KGCStringLoader> m_spStrLoader;

	CNBitmapButton* m_pkBtnPackageDownload;
	CNBitmapButton* m_pkBtnSelectServer;

	bool m_bPackageSizeCheck;

	INT64 m_i64OldDownLoad;
	INT64 m_i64CurrentDownLoad;
	INT64 m_i64TotalDownLoad;
	KncCriticalSection m_csDownloadInfo;

	const int m_iSpeedCheckTime;
	
	bool m_bEraseBack;

public:
	virtual BOOL OnInitDialog();
	void SetLinkButtonPosition();
	afx_msg void OnPaint();
	void ReleaseAll();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	void ShowWeb( std::string url );
	void HideWeb();
	void OnStart();
	void OnProgress( SProgressInfo & progressinfo );
	void OnProgress2( SProgressInfo & progressinfo );
	void OnError( std::string filename,int errorcode );
	void OnComplete( bool success, bool bMsgBox );
	bool IsStart() { return true; }
	void OnStartDownload();
	void OnReselectServerBtn();
	void OnPackageDownBtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};
