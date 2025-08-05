// WCenterServerDlg.h : 헤더 파일
//

#pragma once

#include "afxwin.h"
#include "./ui/EditStreamBuf.h"
#include "./ui/ReadOnlyEdit.h"

#include <vector>

#include <KncLua.h>
#include <boost/shared_ptr.hpp>

// WCenterServerDlg 대화 상자
class WCenterServerDlg : public CDialog
{
// 생성
public:
	WCenterServerDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_WCENTERSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원

    enum
    {
        DG_BOARDER_SIZE = 10,
        DG_OK_BUTTON_SIZE = 30,
        DG_SERVER_NAME_HEIGHT = 20,
        DG_SERVER_NAME_WIDTH = 100,
    };

// Implementation
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose(UINT nForceClose);
    //afx_msg void OnPatch();
    afx_msg LRESULT OnPatch( WPARAM wParam, LPARAM lParam );

	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

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

    CWinThread*	m_pThreadStdIO;

    //	CString m_strStdIO;
    CString m_strInput;	

    CEdit m_editInput;
    CReadOnlyEdit m_editLog;
    // The logger object
    CEditLog m_editLogger;
    CButton m_btOK;

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

    void ReSizeWnd( void );
private:
    boost::shared_ptr<lua_State>    m_sptlua;

    std::vector<CString>			m_vecInputHistory;
    int                             m_nTail;
    int                             m_nStart;
    int								m_iInputCursor;

public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    afx_msg void OnSize(UINT nType, int cx, int cy);
    CEdit m_editServerName;
    CString m_strServerName;
};
