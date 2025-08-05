// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__82A6008B_AB56_4A86_B218_A2A9C72573FB__INCLUDED_)
#define AFX_MAINFRM_H__82A6008B_AB56_4A86_B218_A2A9C72573FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "SServerInfo.h"
#include "KNotifyMsgDlg.h"
#include "KBlackListDlg.h"
#include "GCServerManagerView.h"
#include <vector>
#include <string>
#include <fstream>

class CGCServerManagerView;

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

public:  // control bar embedded members
	CStatusBar                          m_wndStatusBar;
    KNotifyMsgDlg                       m_dlgNotifyMsg;
    KBlackListDlg                       m_dlgBlackList;
    std::vector<SServerInfo>            m_vecSvrList;
    std::vector<CGCServerManagerView*>  m_vecpChildView;
    std::vector<std::wstring>           m_vecBlackList;
    std::wstring                        m_strLogin;
    std::wstring                        m_strPasswd;
    std::wofstream                      m_logFile;

    bool                                m_bKorean; // 한글로 출력할건지...

private:
    void ReadServerListFile( void );
    void ReadBlackListFile( void );
    void WriteServerListFile( void );
    void WriteBlackListFile( void );

public:
    void AddBlackList( const std::wstring& strName, const std::wstring& strReason );

// Generated message map functions
public:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnFileNew( void );
	afx_msg void OnFileOpen( void );
	afx_msg void OnDestroy( void );
	//}}AFX_MSG
    void OnCloseNotifyMsgDlg( void );
    void OnCloseBlackListDlg( void );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__82A6008B_AB56_4A86_B218_A2A9C72573FB__INCLUDED_)
