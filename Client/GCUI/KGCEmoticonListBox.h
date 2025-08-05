#ifndef _GC_EMOTICON_LIST_BOX_H_
#define _GC_EMOTICON_LIST_BOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
////#include <vector>
//#include <map>
//#include <string>


class KGCEmoticonListBox : public KD3DWnd,          // extends
                   public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEmoticonListBox );
    /// Default constructor
    KGCEmoticonListBox( void );
    /// Default destructor
    virtual ~KGCEmoticonListBox( void );
    /// Copy constructor
    KGCEmoticonListBox( const KGCEmoticonListBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEmoticonListBox& operator=( const KGCEmoticonListBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event );
    

    typedef void (KGCEmoticonListBox::*HANDLER)( DWORD );
    std::map<std::string, HANDLER>  m_mapEvent;
};


DEFINE_WND_FACTORY( KGCEmoticonListBox );
DECLARE_WND_FACTORY( KGCEmoticonListBox );
DECLARE_WND_FACTORY_NAME( KGCEmoticonListBox );

#endif // _GC_EMOTICON_LIST_BOX_H_
