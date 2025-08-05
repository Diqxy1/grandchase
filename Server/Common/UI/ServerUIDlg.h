// WGameServerDlg.h : header file
//

#pragma once

#include "afxwin.h"
#include "EditStreamBuf.h"

#include <vector>

#include <KncLua.h>
#include <boost/shared_ptr.hpp>

// CServerUIDlg dialog
class CServerUIDlg : public CDialog
{
// Construction
public:
    CServerUIDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
    enum { IDD = IDD_WGAMESERVER_DIALOG };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose(UINT nForceClose);
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

    void StartupServer();
    void ShutdownServer();

    CWinThread* m_pThreadStdIO;
    
    CString m_strInput; 
    CEdit m_editInput;
    virtual BOOL DestroyWindow();

private:
    std::vector<CString>            m_vecInputHistory;
    int                             m_iInputCursor;
    
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    
    // The logger object
    CEditLog            m_EditLogger;
    CEdit m_EditLog;

    // ANSI and UNICODE stream buffers for the EditLogger
    EditStream::editstreambuf   m_EditStrBuf;
    EditStream::weditstreambuf  m_EditStrBufW;
    EditStream::editstreambuf   m_EditStrBufError;
    EditStream::weditstreambuf  m_EditStrBufWError;

    // Used to save the previos values for cout and wcout
    std::basic_streambuf<char>*     m_pOldBuf;
    std::basic_streambuf<wchar_t>*  m_pOldBufW;
    std::basic_streambuf<char>*     m_pOldBufError;
    std::basic_streambuf<wchar_t>*  m_pOldBufWError;

protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
