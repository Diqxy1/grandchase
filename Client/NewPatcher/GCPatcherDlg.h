// GCPatcherDlg.h : header file
//

#if !defined(AFX_GCPATCHERDLG_H__DEDF685C_128B_468B_BED3_DAA8D1DDCBDF__INCLUDED_)
#define AFX_GCPATCHERDLG_H__DEDF685C_128B_468B_BED3_DAA8D1DDCBDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
#include "GCHtmlView.h"
#include "KProgressCtrl.h"
#include "ColorStatic.h"
#include "NBitmapButton.h"
#include "kgcstringloader.h"
#include "afxwin.h"
#include "resource.h"
#include "patch.h"

//#pragma comment(lib, "BugTrap.lib")

/////////////////////////////////////////////////////////////////////////////
// CGCPatcherDlg dialog


struct SProgData
{
    UINT m_totallength;
    UINT m_totalreadlength;

    int m_currentlength;	    // 현재 파일 사이즈
    int m_currentreadlength;	// 현재 읽은 파일 사이즈

    SProgData()
        : m_totallength ( 0 )
        , m_totalreadlength ( 0 )
        , m_currentlength ( 0 )
        , m_currentreadlength ( 0 )
    {
    }

    void Assign( SProgressInfo& sProgressInfo )
    {
        m_totallength = sProgressInfo.totallength;
        m_totalreadlength = sProgressInfo.totalreadlength;
        m_currentlength = sProgressInfo.currentlength;
        m_currentreadlength = sProgressInfo.currentreadlength;
    }
};

class CGCPatcherDlg : public CDialog
{
// Construction
public:
	CGCPatcherDlg(bool mannual,boost::shared_ptr<KGCStringLoader> strloader,boost::shared_ptr<Komfile> imagefile,
				  int nMaxLang = 0, CWnd* pParent = NULL );	// standard constructor
    ~CGCPatcherDlg();

	enum { IDD = IDD_GCPATCHER_DIALOG };
	CColorStatic	m_StaticFile;
	CColorStatic	m_StaticFileName;
	CColorStatic	m_StaticTotal;
	CColorStatic    m_StaticMsg;
    CColorStatic    m_kStaticDownLoadTotalRate;
    CColorStatic    m_kStaticDownLoadSubRate;
    CColorStatic    m_StaticBackGround;
	CColorStatic    m_StaticDate;
	CColorStatic    m_StaticSpeed;
   
public:	
    BOOL ModalDestroyWindow();
	virtual BOOL DestroyWindow();    
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedEnd();

	void ShowWeb(std::string url);
	void HideWeb();
    
	
    void OnStart();
	void OnProgress(SProgressInfo & info);
	void OnProgress2(SProgressInfo & info);
	void OnDownSpeed(SProgressInfo & info);
	void OnError(std::string filename,int errorcode);
	void OnComplete(bool success , bool bMsgBox);
	void OnInvalidate();

    void OnLaunch();
	void OnUpdateStart();
	void OnUpdateStop();
    void ShowEndButton();

	void Stop();
	bool IsStart() { return m_bClickStart; }
	void OnClickLang( int i );
	const std::wstring LoadLastUpdateDate();

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    HINSTANCE       m_hInstance;    
	CGCHtmlView     *m_pHtmlPR;
	HICON           m_hIcon;    
    char            m_strTotalDownLoadInfo[128];        
    char            m_strSubDownLoadInfo[128];        
    CBitmap*        m_pkBitMap;

    SProgData       m_sProgressInfo;
    FLOAT           m_fTotalRate;
    FLOAT           m_fFileDownloadRate;

    bool			m_bClickStart;		//	스타트버튼을 눌렀다.
	
	int				m_nMaxLang;
	std::vector<CNBitmapButton*>	m_vecLangButton;

	    
	// Generated message map functions
	//{{AFX_MSG(CGCPatcherDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
//	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnExit();    
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CNBitmapButton      m_kStartBtn;
	CNBitmapButton      m_kEndBtn;
	CNBitmapButton      m_kUpdateStartBtn;
	CNBitmapButton      m_kUpdateStopBtn;
	
	CFont               m_kFont;
	CFont               m_kSmallFont;
	KProgressCtrl       m_kProgressFile;
	KProgressCtrl       m_kProgressTotal;        

	boost::shared_ptr<KGCStringLoader>	m_pStrLoader;
	boost::shared_ptr<Komfile>			m_pMassFile;	
	bool				m_bMainError;
	bool				m_bMannual;

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnStnClickedStaticMsg();
	afx_msg void OnStnClickedStaticHtml();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GCPATCHERDLG_H__DEDF685C_128B_468B_BED3_DAA8D1DDCBDF__INCLUDED_)
