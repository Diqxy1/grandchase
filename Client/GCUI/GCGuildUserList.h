#ifndef _GCGUILDUSERLIST_H_
#define _GCGUILDUSERLIST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KGCUserBar;
class KD3DScrollbar;

class KGCGuildUserList : public KD3DWnd,        // extends
                         public KActionListener // implements
{
public:
    DECLARE_CLASSNAME( KGCGuildUserList );
    /// Default constructor
    KGCGuildUserList( void );
    /// Default destructor
    virtual ~KGCGuildUserList( void );
    /// Copy constructor
    KGCGuildUserList( const KGCGuildUserList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildUserList& operator=( const KGCGuildUserList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        NUM_GUILD_USER_LIST = 18,
    };

public:
    void UpdateGuildUserList( void );
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

protected:
    int             m_iTopIndex;
    KSafeArray<KGCUserBar*,NUM_GUILD_USER_LIST> m_apkUserBar;
    KD3DScrollbar*  m_pkScrollbar;
};

DEFINE_WND_FACTORY( KGCGuildUserList );
DECLARE_WND_FACTORY( KGCGuildUserList );
DECLARE_WND_FACTORY_NAME( KGCGuildUserList );

#endif // _GCGUILDUSERLIST_H_