#pragma once

#include "GCFriendListChild.h"
class KGCFriendListChild;

struct KFriendListData
{
	bool m_bGroupTitle;
	bool m_bGroupOpen;
	int m_iGroupID;
	DWORD m_dwUserUID;
	bool m_bOnline;
	bool m_bSelected;

	std::wstring m_strNickname;
	std::wstring m_strNickColor;

	KFriendListData()
		: m_bGroupTitle(false)
		, m_bGroupOpen(false)
		, m_iGroupID(-1)
		, m_dwUserUID(-1)
		, m_bOnline(false)
		, m_bSelected(false)
		, m_strNickname(L"")
		, m_strNickColor(L"FFFFFF")
	{
	}

	bool operator==(const KRecommenderInfo& right) const
	{
		return ( m_dwUserUID == right.m_dwLoginUID );
	}
};

class KGCFriendListTree : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCFriendListTree );
    /// Default constructor
    KGCFriendListTree( void );
    /// Default destructor
    virtual ~KGCFriendListTree( void );
    /// Copy constructor
    KGCFriendListTree( const KGCFriendListTree& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCFriendListTree& operator=( const KGCFriendListTree& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum{
		MAX_FRIEND_LIST_CHILD = 13,
	};

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void OnCreateComplete( void );

	void UpdateData( void );
	void UpdateDisplayData( int iScrollPos = -1 );
	void SetSelection( int iSelectedChildIndex );
	void RefreshSelection( void );

	void SetMaxScrollSize( int iMaxScrollSize );
	std::wstring GetSelectedNickname( void );

private:
	KSafeArray<KGCFriendListChild*,MAX_FRIEND_LIST_CHILD> m_pkChild;

	std::vector< KFriendListData >	m_vecFriendListData;

	KD3DScrollbar*		m_pkScrollbar;

	int					m_iMaxScrollSize;
	int					m_iScrollPos;

	int					m_iSelectedIndex;
	int					m_iSelectedUserUID;
};

DEFINE_WND_FACTORY( KGCFriendListTree );
DECLARE_WND_FACTORY( KGCFriendListTree );
DECLARE_WND_FACTORY_NAME( KGCFriendListTree );


