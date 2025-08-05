#if !defined(AFX_KCHANNELLOBBY_H__A0841449_4D3E_4D94_9A72_A474DBEA79C1__INCLUDED_)
#define AFX_KCHANNELLOBBY_H__A0841449_4D3E_4D94_9A72_A474DBEA79C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KChannelLobby.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KChannelLobby dialog

#include "codex/cdxCDynamicDialog.h"

class KChannelLobby : public cdxCDynamicDialog
{
// Construction
public:
	KChannelLobby(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(KChannelLobby)
	enum { IDD = IDD_CHANNEL_LOBBY };
	CListBox	m_ctrlChannelList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KChannelLobby)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(KChannelLobby)
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnDblclkChannelList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KCHANNELLOBBY_H__A0841449_4D3E_4D94_9A72_A474DBEA79C1__INCLUDED_)
