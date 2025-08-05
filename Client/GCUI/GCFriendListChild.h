#pragma once

#include "../gcui/GCFriendListTree.h"
struct KFriendListData;

class KGCFriendListChild : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCFriendListChild );
    /// Default constructor
    KGCFriendListChild( void );
    /// Default destructor
    virtual ~KGCFriendListChild( void );
    /// Copy constructor
    KGCFriendListChild( const KGCFriendListChild& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCFriendListChild& operator=( const KGCFriendListChild& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

	void SetData( KFriendListData& kFriendData );
	void ToggleFolder( void );
	void SetSelection( bool bSelected );
	void OnCursor( bool bOnCursor );
	
	inline bool IsGroupTitle( void )
	{
		return m_bGroupTitle;
	}
	inline bool IsNameSelected( void )
	{
		return m_pkSelectBar->IsRenderOn();
	}
	inline int GetGroupId( void )
	{
		return m_iGroupId;
	}

private:
	KD3DWnd*            m_pkFolderIcon;
	KD3DWnd*            m_pkFolderBar;
	KD3DWnd*            m_pkSelectBar;
	KD3DStatic*         m_pkStaticName;

	bool				m_bGroupTitle;
	bool				m_bGroupOpen;
	bool				m_bSelected;
	bool				m_bOnline;

	int					m_iGroupId;

};

DEFINE_WND_FACTORY( KGCFriendListChild );
DECLARE_WND_FACTORY( KGCFriendListChild );
DECLARE_WND_FACTORY_NAME( KGCFriendListChild );
