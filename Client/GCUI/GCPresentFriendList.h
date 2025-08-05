#pragma once

class KGCFriendListTree;

class KGCPresentFriendList : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCPresentFriendList );
    /// Default constructor
    KGCPresentFriendList( void );
    /// Default destructor
    virtual ~KGCPresentFriendList( void );
    /// Copy constructor
    KGCPresentFriendList( const KGCPresentFriendList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPresentFriendList& operator=( const KGCPresentFriendList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		MAX_FRIEND_LIST = 13,
	};

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

	void OnClickCloseBtn( void );
	void OnClickOkBtn( void );
	void OnClickRefreshBtn( void );

private:
	KD3DWnd*            m_pkCloseBtn;
	KD3DWnd*            m_pkOkBtn;
	KD3DWnd*            m_pkRefreshBtn;

	KGCFriendListTree*	m_pkFriendTree;
	
};

DEFINE_WND_FACTORY( KGCPresentFriendList );
DECLARE_WND_FACTORY( KGCPresentFriendList );
DECLARE_WND_FACTORY_NAME( KGCPresentFriendList );
