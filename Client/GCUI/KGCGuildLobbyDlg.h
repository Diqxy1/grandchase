#pragma once

class KGCRoomList;
class KGCChatBox;

class KGCGuildLobbyDlg :	public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCGuildLobbyDlg );
    /// Default constructor
    KGCGuildLobbyDlg( void );
    /// Default destructor
    virtual ~KGCGuildLobbyDlg( void );
    /// Copy constructor
    KGCGuildLobbyDlg( const KGCGuildLobbyDlg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildLobbyDlg& operator=( const KGCGuildLobbyDlg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroy();
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    KD3DWnd*            m_pkCloseBtn;
    KGCChatBox*         m_pkGuildLobbyChat;
    int                 m_iBeforeFilterSetting;

public:
    KGCRoomList*        m_pkGuildRoomList;

public:
    void OnClose();
};

DEFINE_WND_FACTORY( KGCGuildLobbyDlg );
DECLARE_WND_FACTORY( KGCGuildLobbyDlg );
DECLARE_WND_FACTORY_NAME( KGCGuildLobbyDlg );