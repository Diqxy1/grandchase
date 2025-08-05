#pragma once

const int FRIEND_LIST_SIZE = 6;

class KGCChristmasFriendList : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCChristmasFriendList );
    /// Default constructor
    KGCChristmasFriendList( void );
    /// Default destructor
    virtual ~KGCChristmasFriendList( void );
    /// Copy constructor
    KGCChristmasFriendList( const KGCChristmasFriendList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCChristmasFriendList& operator=( const KGCChristmasFriendList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

private:
    KD3DWnd*                                        m_pkBtnClose;
    KD3DWnd*                                        m_apkBtnFriendsNameSelected[FRIEND_LIST_SIZE];

    KD3DStatic*                                     m_pkStaticFriendList;
    KD3DStatic*                                     m_apkStaticFriendsName[FRIEND_LIST_SIZE];

    KD3DWnd*                                        m_pkBackScroll;
    KD3DScrollbar*                                  m_pkScrollBar;

    KGCCheckBox*                                    m_pkOfflineCheckBox;
    KD3DWnd*                                        m_pkBtnOfflineCheckBox;

    std::vector<KInviteFriendUserInfo>              m_vecFriendList;
    int                                             m_iRealIndex;

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreateComplete( void );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    void OnClickClose();
    void OnScroll();

    void InitFriendList();
    void InitScroll( void );
    int GetViewCount(int iMemberIndex_);

    void OnToggleOfflineList();
    bool IsShowOfflineList();
    void ToggleFriendList();
};


DEFINE_WND_FACTORY( KGCChristmasFriendList );
DECLARE_WND_FACTORY( KGCChristmasFriendList );
DECLARE_WND_FACTORY_NAME( KGCChristmasFriendList );
