#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DSizingBox;
class KD3DStatic;
class KGCInviteUserInfo;
class KD3DScrollbar;

class KGCInviteAgitReqDlg:  public KD3DDialog,
                          public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCInviteAgitReqDlg );
    /// Default constructor
    KGCInviteAgitReqDlg( void );
    /// Default destructor
    virtual ~KGCInviteAgitReqDlg( void );
    /// Copy constructor
    KGCInviteAgitReqDlg( const KGCInviteAgitReqDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCInviteAgitReqDlg& operator=( const KGCInviteAgitReqDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum{
        EBUDDY_LIST_NUM = 6,  
    };    

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );    

private:
    KSafeArray<KGCCheckBox*,EBUDDY_LIST_NUM>        m_pkBuddyInfo;
    KD3DScrollbar*                                  m_pkScrollBar;
    KD3DStatic*                                     m_pkStaticConnectBuddy;
    KD3DStatic*                                     m_pkStaticAllCheck;
    KD3DWnd*                                        m_pkInviteBtn;
    KD3DWnd*                                        m_pkCloseBtn;
    KD3DWnd*                                        m_pkBackScroll;
    KGCCheckBox*                                    m_pkCheckBoxAll;
    std::vector<KInviteFriendUserInfo>              m_vecBuddyList;

    int                                             m_iRealIndex;

public:
    void OnInviteBtn();
    void OnScroll();
    void OnClose();
    void OnCheckAll( KD3DWnd* pWnd, DWORD dwCode );

    void InitInfo();
    void InitBuddyList();
    void InitScroll( void );    

    int GetViewCount(int iMemberIndex_);
};

DEFINE_WND_FACTORY( KGCInviteAgitReqDlg );
DECLARE_WND_FACTORY( KGCInviteAgitReqDlg );
DECLARE_WND_FACTORY_NAME( KGCInviteAgitReqDlg );