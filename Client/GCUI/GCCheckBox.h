#ifndef _GCCHECKBOX_H_
#define _GCCHECKBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "../uifx/D3DStatic.h"
//#include <UserPacket.h>

class KGCCheckBox: public KD3DWnd,          // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCheckBox );
    /// Default constructor
    KGCCheckBox( void );
    /// Default destructor
    virtual ~KGCCheckBox( void );
    /// Copy constructor
    KGCCheckBox( const KGCCheckBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCheckBox& operator=( const KGCCheckBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    void ToggleCheck();
    bool GetCheck() const { return m_bCheck; }
    void SetCheck( bool bCheck );
    void SetText( std::wstring wstr_ );
    void SetFontColor( D3DCOLOR dwColor_ );
    void SetFontOutline( bool bFontOutline = false, D3DCOLOR OutlineColor = 0xFF000000 );
    void InitInfo();
    void SetLock( bool bLock_ );

protected:
    virtual void OnCreate( void );

    KD3DStatic* m_pkStatic;
    KD3DWnd*    m_pkBox;
    KD3DWnd*    m_pkCheck;
    bool m_bCheck;
};

DEFINE_WND_FACTORY( KGCCheckBox );
DECLARE_WND_FACTORY( KGCCheckBox );
DECLARE_WND_FACTORY_NAME( KGCCheckBox );

#endif // _GCCHECKBOX_H_