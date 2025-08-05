#include "afxwin.h"
#include "ChannelType.h"
#include "GCEnum.h"
#include "MainFrm.h"
#if !defined(AFX_KCHANNEL_H__FD673E3E_65C0_4B35_8BA6_B6AF470BA62D__INCLUDED_)
#define AFX_KCHANNEL_H__FD673E3E_65C0_4B35_8BA6_B6AF470BA62D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KChannel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KChannel dialog

class CGCServerManagerView;
class CMainFrame;

class KChannel : public CDialog
{
// Construction
public:
	KChannel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    enum { ID_BTN_CHANNEL_EXIT = 10000 };
    //{{AFX_DATA(KChannel)
	enum { IDD = IDD_CHANNEL };
	CButton	m_btnStop;
    CListBox    m_ctrlUserList;
    CListBox    m_ctrlRoomList;
    CListBox    m_ctrlChatList;
    CListBox    m_ctrlSquareList;
    CListBox    m_ctrlSquareUserList;
    CEdit       m_ctrlEditNormalChat;
    CEdit       m_ctrlEditAdminChat;
    //CString     m_cstrAdminChat;
    CString     m_cstrNormalChat;


    // 지역 구별 변수 추가
    // 변경시 void KChannel::OnCbnSelchangeComboArea() 부분도 함께 변경해야 한다
    enum AREA_CODE {
        AC_ALL = 0,
        AC_BERMESIAH,
        AC_SILVERLAND,
        AC_ELIA,
        AC_XENIA,
        AC_ACHAEMEDIA,
        AC_ATUM,
		AC_HERO_AND_EVENT,
        AC_MATCH
    }eArea; 

    // 게임 모드 관련 구조체 ( 지역 정보, 게임 모드 )
    class KGAME_MODE {
    public:
        KGAME_MODE( EGCGameMode _eGameMode, AREA_CODE _eArea ) : eGameMode(_eGameMode), eArea(_eArea) {}
        KGAME_MODE( EGCGameModeCategory _eGameModeCategory, AREA_CODE _eArea ) : eGameModeCategory(_eGameModeCategory), eArea(_eArea) {}

        EGCGameMode GetGameMode() { return eGameMode; }
        EGCGameModeCategory GetGameModeCategory() { return eGameModeCategory; }
        AREA_CODE GetArea() { return eArea; }

    private:
        EGCGameMode eGameMode;
        EGCGameModeCategory eGameModeCategory;
        AREA_CODE   eArea;
    };

    // pravte으로 accessor 만들자.
    CMainFrame*            m_pMainFrm;
    std::map<CString, KGAME_MODE> m_mapGameMode; // 언어 출력 설정 구조 변경으로 인해 map으로 관리
    std::map<int, CString> m_mapAreaCode; // 지역순서별로 정렬하기 위해 지역 번호를 key로...

    enum SB_CHAT_MESSAGE
    {
        SBCM_ADMIN_CHAT = 0,
        SBCM_NOTICE,
        SBCM_SERVER,
        SBCM_NORMAL_CHAT,

        NUM_OF_SBCM
    };

    CButton     m_raNotice_Message[NUM_OF_SBCM]; // 라디오 버튼

    int GetSBChatMessageType(void);


	//}}AFX_DATA
    bool        m_bStop;
    CGCServerManagerView* m_pChildView;

    int         GetGameMode(){ return m_iGameMode; }
    void        SetGameMode( int iGameMode ){ m_iGameMode = iGameMode; }
    int         GetChannelType(){ return m_iChannelType; }
    void        SetChannelType( int iChannelType ){ m_iChannelType = iChannelType; }
    void        InitGameMode();
    int         GetRoomListPage(){ return m_iRoomPage; }
    void        SetRoomListPage( int iRoomPage ){ m_iRoomPage = iRoomPage; }

    void        SetRoomPageSize( int iPageSize ){ m_iRoomPageSize = iPageSize; }
    void        MakeRoomPage();
    void        MakeUserListPage();

    int         GetUserListPage(){ return m_iUserListPage; }
    void        SetUserListPage( int iUserPage ){ m_iUserListPage = iUserPage; }
    void        SetUserListPageSize( int iPageSize ){ m_iUserListPageSize = iPageSize; }

    int         GetSquareUserListPage(){ return m_iSquareUserListPage; }
    void        SetSquareUserListPage( int iSquareUserPage ){ m_iSquareUserListPage = iSquareUserPage; }
    void        SetSquareUserListPageSize( int iPageSize ){ m_iSquareUserListPageSize = iPageSize; }
    void        MakeSquarePage();

    void        UpdateGameMode();
    void        InitArea();

    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KChannel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(KChannel)
	afx_msg void OnDefBtnAdmin();
	virtual void OnCancel();
	afx_msg void OnDblclkListUser();
	afx_msg void OnDblclkListRoom();
	afx_msg void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListChat();
	afx_msg void OnButtonRoomListUpdate();
	afx_msg void OnButtonUserListUpdate();
	afx_msg void OnButtonChatStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    int m_iGameMode;
    int m_iChannelType;
    int m_iRoomPage;
    int m_iRoomPageSize;
    int m_iUserListPageSize;
    int m_iUserListPage;
    int m_iSquareUserListPageSize;
    int m_iSquareUserListPage;
public:
    afx_msg void OnCbnSelchangeComboPage();
    CComboBox m_comboRoomPage;
    CComboBox m_comboGameMode;
    afx_msg void OnCbnSelchangeComboGamemode();
    CComboBox m_comboUserListPage;
    CComboBox m_comboSquareUserListPage;
    afx_msg void OnCbnSelchangeComboUserPage();
    afx_msg void OnLbnDblclkListSquare();
    afx_msg void OnBnClickedButtonSquareUpdate();
    afx_msg void OnCbnSelchangeComboSquareUersInfo();
    afx_msg void OnLbnDblclkListSquareUser();
    CComboBox m_comboArea;
    afx_msg void OnCbnSelchangeComboArea();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KCHANNEL_H__FD673E3E_65C0_4B35_8BA6_B6AF470BA62D__INCLUDED_)
