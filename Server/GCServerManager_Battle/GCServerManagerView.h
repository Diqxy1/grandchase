// GCServerManagerView.h : interface of the CGCServerManagerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GCSERVERMANAGERVIEW_H__16132666_DFF9_4E31_A7CF_45D488E8BBC7__INCLUDED_)
#define AFX_GCSERVERMANAGERVIEW_H__16132666_DFF9_4E31_A7CF_45D488E8BBC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "SServerInfo.h"
#include "GCServerManagerDoc.h"
#include "KChannelLobby.h"
#include "KChannel.h"
#include "GMProxy.h"
#include <vector>
#include <string>

#include "KAdminDBAccessDlg.h"
#include "GCItemManager.h"

class KUserInfoLess
{
public:
    bool operator()( const KInviteUserInfo& p, const KInviteUserInfo& q ) const
    { return p.m_strNickName < q.m_strNickName; }
};

class CMainFrame;

class CGCServerManagerView : public CView
{
protected: // create from serialization only
	CGCServerManagerView();
	DECLARE_DYNCREATE(CGCServerManagerView)

public:
    enum EUserState
    {
        US_INIT,
        US_LOGINING,
        US_CHANNELLOBBY,
        US_CHANNEL,
        US_ROOM,
        US_PLAYING,
        END_OF_STATE,
    };
public:
    enum EWaitResult
    {
        SUCCESS,
        FAIL,
        TIME_OVER,
    };

// Attributes
public:
	CGCServerManagerDoc* GetDocument();

// Operations
public:
    void ConnectServer( const SServerInfo& svrInfo );
    void CloseServer( void );
    void UpdateRoomInfoList( void );
    void UpdateUserInfoList( void );
    void ChangeRoomPage();
    void ChangeUserPage();
    void ChangeSquareInfoUpdate(); // 광장 처리 작업
    void UpdateSquareList();
    void UpdateSquareUserList();
    void ChangeSquareUserList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGCServerManagerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGCServerManagerView();
    LRESULT GetAdminUseItemInfoData();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
    EWaitResult WaitVerifyAccountAck( DWORD dwMaxTime );
    std::string GetAuthLevel( char cAuthLevel );
    void SetUserState( EUserState eState );
    void RestoreSize( void );
    void OnEnterBtn( void );
    void OnExitBtn( void );
    void OnLeaveChannel( void );
    void OnEditAdminChat( void );
    void OnEditNoticeMsg( void ); // void OnEditNormalChat( void ); 에서 바꿈

    LRESULT OnServerMsgRecv( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerVerifyAccountAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerGMToolAuthenticationAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerChannelInfoListAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerEnterChannelAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerRoomListAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerUserListNot( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerChatNot( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerCreateRoomAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerJoinRoomAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerKickUserAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerChangeUserDBInfoAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerUserDBInfoAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerDropUserDBInfoAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerCloseConnectionNot( WPARAM wParam, LPARAM lParam );



    LRESULT OnSelectNormalItemAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnSelectDelNormalItemAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnSelectDurationItemAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnSelectDelDurationItemAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnInsertGPNormalItemAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnInsertGPDurationItemAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnInsertCashNormalItemAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnInsertCashDurationItemAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnDelItemAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnSelectUserDataAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnSelectChangeStrongItemListAck( WPARAM wParam, LPARAM lParam );
    //LRESULT OnSelectAdminUseItemInfoDataAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerSquareListAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerSquareUserListAck( WPARAM wParam, LPARAM lParam );
    LRESULT OnServerUserChatChangeAck( WPARAM wParam, LPARAM lParam );

public:
    CMainFrame*                  m_pMainFrm;
    std::wstring                 m_strLogin;
    std::wstring                 m_strNickname;
    std::wstring                 m_strPasswd;
    std::wstring                 m_strWideServerName;
    std::string                 m_strServerName;
    std::wstring                 m_strChangeDBLogin;
    std::vector<KChannelInfo>   m_vecChannelList;
    std::vector<KSimpleRoomInfo>      m_vecRoomInfo;
    std::vector<KSimpleRoomInfo>      m_vecRoomInfoUpdate;
    std::vector<KInviteUserInfo>      m_vecUsrInfo;       // KUserInfo -> KInviteUserInfo
    std::vector<KInviteUserInfo>      m_vecUsrInfoUpdate; // KUserInfo -> KInviteUserInfo
    std::vector<std::wstring>    m_vecChatBuf;
    KGMProxy                        m_server;
    EUserState                  m_eUsrStat;
    SServerInfo                 m_svrInfo;
    KChannelLobby               m_channelLobby;
    KChannel                    m_channel;
    bool                        m_bEndWait;
    bool                        m_bError;

	bool                        m_bScrollLock;

    KAdminDBAccessDlg           m_dlgAdminDBAccess;
    KGCItemManager              m_kItemManager;

    std::vector<KSquareInfo>    m_vecSquareList;
    std::vector<KSquareInfo>    m_vecSquareListUpdate;
    std::vector<KInviteUserInfo> m_vecSquareUserList;
    std::vector<KInviteUserInfo> m_vecSquareUserListUpdate;

// Generated message map functions
protected:
	//{{AFX_MSG(CGCServerManagerView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GCServerManagerView.cpp
inline CGCServerManagerDoc* CGCServerManagerView::GetDocument()
   { return (CGCServerManagerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GCSERVERMANAGERVIEW_H__16132666_DFF9_4E31_A7CF_45D488E8BBC7__INCLUDED_)
