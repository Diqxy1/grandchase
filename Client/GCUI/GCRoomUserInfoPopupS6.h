#pragma once

class KGCRoomUserInfoViewS6;
class KGCRoomCharWndS6;

class KGCRoomUserInfoPopupS6 : public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCRoomUserInfoPopupS6 );
    /// Default constructor
    KGCRoomUserInfoPopupS6( void );
    /// Default destructor
    virtual ~KGCRoomUserInfoPopupS6( void );
    /// Copy constructor
    KGCRoomUserInfoPopupS6( const KGCRoomUserInfoPopupS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomUserInfoPopupS6& operator=( const KGCRoomUserInfoPopupS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );

public:
    void OnClose();
    void OnClickUserInfo();
    void OnClickUserStatus();

    void OnAddFriend();
    void OnBlockFriend();
    void OnBadGuy();

public:
    void SetInfoButtonState(bool bUserInfo_, bool bStatusInfo_);

    bool ValidPlayerCheck();

public:
    void UpdateData();
    void UpdatePlayerData();
    void UpdateMyChar( PLAYER* pPlayer );

public:
    int m_nPlayerIndex;

public:
    KD3DWnd*                m_pkBtnClose;
    KD3DWnd*                m_pkBntUserInfo;
    KD3DWnd*                m_pkBntUserStatus;
    KD3DWnd*                m_pkBtnAddFriend;
    KD3DWnd*                m_pkBtnBlockFriend;
    KD3DWnd*                m_pkBtnBadGuy;
    KGCRoomUserInfoViewS6*  m_pkInfoPanel;
    KGCStatusInfoPanel*     m_pkStatusPanel;
    KGCRoomCharWndS6*       m_pkCharWnd;
    PLAYER                  m_Player;
public:
    std::wstring        m_strNickName;
};

DEFINE_WND_FACTORY( KGCRoomUserInfoPopupS6 );
DECLARE_WND_FACTORY( KGCRoomUserInfoPopupS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomUserInfoPopupS6 );
