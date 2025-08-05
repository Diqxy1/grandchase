// WGameServerDlg.h : header file
//

#pragma once

//#include "ListBoxLog.h"
#include "afxwin.h"
#include "./ui/EditStreamBuf.h"
#include "ReadOnlyEdit.h"

#include <vector>

#include <KncLua.h>
#include <boost/shared_ptr.hpp>

// CWGameServerDlg dialog
class CWGameServerDlg : public CDialog
{
// Construction
public:
	CWGameServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WGAMESERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	//afx_msg void OnLbnSelchangeOutput();

	void StartupServer();
	void ShutdownServer();

    void InitHistory();
    void AddHistory( CString& strInput );
    bool IsHistoryEmpty() { return (m_nTail == m_nStart); };
    int GetFreeSize();
    int GetNextCursor();
    int GetBeforeCursor();
    int GetLastHistory();
    void ResetCursor();

	
//	CWinThread*	m_pThreadDisplay;	
	CWinThread*	m_pThreadStdIO;
	
//	CString m_strStdIO;
	CString m_strInput;	
	
    CEdit m_editInput;
    CReadOnlyEdit m_editLog;
    // The logger object
    CEditLog m_editLogger;
	
    virtual BOOL DestroyWindow();
	
//	CString m_strOutput;

    // ANSI and UNICODE stream buffers for the EditLogger
    EditStream::editstreambuf   m_editStrBuf;
    EditStream::weditstreambuf  m_editStrBufW;
    EditStream::editstreambuf   m_editStrBufError;
    EditStream::weditstreambuf  m_editStrBufWError;

    // Used to save the previos values for cout and wcout
    std::basic_streambuf<char>*     m_pOldBuf;
    std::basic_streambuf<wchar_t>*  m_pOldBufW;
    std::basic_streambuf<char>*     m_pOldBufError;
    std::basic_streambuf<wchar_t>*  m_pOldBufWError;

private:
	boost::shared_ptr<lua_State>    m_sptlua;

	std::vector<CString>			m_vecInputHistory;
    int                             m_nTail;
    int                             m_nStart;
	int								m_iInputCursor;
	
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

};
