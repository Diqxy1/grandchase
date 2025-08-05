#ifndef _GCUSERLIST_H_
#define _GCUSERLIST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KGCUserBar;
class KD3DScrollbar;

class KGCUserList : public KD3DWnd,         // extends
                    public KActionListener  // implements
{
public:
    DECLARE_CLASSNAME( KGCUserList );
    /// Default constructor
    KGCUserList( void );
    /// Default destructor
    virtual ~KGCUserList( void );
    /// Copy constructor
    KGCUserList( const KGCUserList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCUserList& operator=( const KGCUserList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        NUM_USER_LIST = 19,
    };

public:
    void ToggleButton( bool bShowAll );
    void UpdateUserList( void );
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );
    virtual void OnActivateComplete( void );
    virtual void FrameMoveInEnabledState( void );

protected:
    int             m_iTopIndex;
    bool            m_bToggleButton;
    bool            m_bShowAll;
    bool            m_bGuildChannel;
    KSafeArray<KGCUserBar*,NUM_USER_LIST> m_apkUserBar;
    KD3DScrollbar*  m_pkScrollbar;
};

DEFINE_WND_FACTORY( KGCUserList );
DECLARE_WND_FACTORY( KGCUserList );
DECLARE_WND_FACTORY_NAME( KGCUserList );

#endif // _GCUSERLIST_H_