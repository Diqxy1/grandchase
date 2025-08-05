#pragma once
#include "afxwin.h"


// CGCPatcherBackDlg 대화 상자입니다.

class CGCPatcherDlg;
class CGCServerDlg;
class CGCMainMenuDlg;
class CGCSystemBtnDlg;
class CGCStartBtnDlg;
class CGCDownLoadDlg;

class CGCPatcherBackDlg : public CDialog
{
public:
	CGCPatcherBackDlg(boost::shared_ptr<Komfile> imagemass, 
		boost::shared_ptr<KGCStringLoader> strLoader,
		CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGCPatcherBackDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GCPATCHER_BACK };


	//void CreateDownLoadDlg(bool mannual,boost::shared_ptr<KGCStringLoader> strloader,
	//	boost::shared_ptr<Komfile> imagemass,
	//	int nMaxLang =0,
	//	CWnd* pParent =NULL);
public:
	CGCPatcherDlg *m_pkDownLoadDlg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	HICON           m_hIcon;    
	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HCURSOR OnQueryDragIcon();
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	void SetChildWindow(CDialog *pWnd);

	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	void CreateServerDlg();
	void CreateMainMenuDlg();
	void CreateSystemBtnDlg();
	void CreateStartBtnDlg();
	BOOL ModalDestroyWindow();
	void CloseServerScene();
	void CreateDownLoadDlg();
	void OnServerSelect();
	void OnDownload();
	void CreateAllDlg();
	NETSTATRT_CALLBACK GetStartFunc();
	NETPROGRESS_CALLBACK GetProgressFunc();
	NETPROGRESS_CALLBACK GetProgress2Func();
	NETERROR_CALLBACK GetErrorFunc();
	COMPLETE_CALLBACK GetCompleteFunc();
	IS_START_CALLBACK GetIsStartFunc();
	void OffServerSelect();
	void OnComplete( bool success, bool bMsgBox );
	CGCServerDlg *m_pkChildServerWindow;
	CGCMainMenuDlg *m_pkChildMainMenuWindow;
	CGCSystemBtnDlg *m_pkChildSystemBtnWindow;
	CGCStartBtnDlg *m_pkChildStartBtnWindow;
	CGCDownLoadDlg *m_pkChildDownloadWindow;
	int m_iState;

	boost::shared_ptr<Komfile> m_spMassFile;
	boost::shared_ptr<KGCStringLoader> m_spStrLoader;

    HWND GetStartBtnHandle();

};
