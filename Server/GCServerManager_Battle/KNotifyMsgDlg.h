#if !defined(AFX_KNOTIFYMSGDLG_H__3C301711_12D5_4E98_9E67_A9739B3F8968__INCLUDED_)
#define AFX_KNOTIFYMSGDLG_H__3C301711_12D5_4E98_9E67_A9739B3F8968__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KNotifyMsgDlg.h : header file
//

#include "GCServerManager.h"

class CMainFrame;

/////////////////////////////////////////////////////////////////////////////
// KNotifyMsgDlg dialog

class KNotifyMsgDlg : public CDialog
{
// Construction
public:
	KNotifyMsgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    enum { ID_BTN_CLOSE = 200 };
	//{{AFX_DATA(KNotifyMsgDlg)
	enum { IDD = IDD_DIALOG_NOTIFY };
	CString 	m_cstrNotifyMsg;

    enum SB_CHAT_MESSAGE
    {
        SBCM_ADMIN_CHAT = 0,
        SBCM_NOTICE,
        SBCM_SERVER,
        SBCM_NORMAL,

        NUM_OF_SBCM
    };

    CButton     m_raNotice_Message_S[NUM_OF_SBCM]; // 라디오 버튼
	//}}AFX_DATA
    CMainFrame* m_pMainFrm;

    int GetSBChatMessageTypeS(void);
// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KNotifyMsgDlg)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(KNotifyMsgDlg)
    virtual BOOL OnInitDialog( void );
	virtual void OnOK( void );
    virtual void OnCancel( void );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KNOTIFYMSGDLG_H__3C301711_12D5_4E98_9E67_A9739B3F8968__INCLUDED_)
