#ifndef _GCSEARCHBAR_H_
#define _GCSEARCHBAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DEdit;
class KD3DStatic;

class KGCSearchBar : public KD3DWnd,
                     public KActionListener

{
public:
    DECLARE_CLASSNAME( KGCSearchBar );
    /// Default constructor
    KGCSearchBar( void );
    /// Default destructor
    virtual ~KGCSearchBar( void );
    /// Copy constructor
    KGCSearchBar( const KGCSearchBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSearchBar& operator=( const KGCSearchBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    void Init();
    void SetFocus( bool bFocus );
    KD3DWnd* m_pkSearchButton;
    KD3DEdit* m_pkEditSearch;
    KD3DStatic* m_pkSearchDesc;

protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();

};

DEFINE_WND_FACTORY( KGCSearchBar );
DECLARE_WND_FACTORY( KGCSearchBar );
DECLARE_WND_FACTORY_NAME( KGCSearchBar );

#endif // _GCSEARCHBAR_H_
