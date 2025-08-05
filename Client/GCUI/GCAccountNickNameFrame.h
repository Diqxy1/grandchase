#pragma once

#include "../GCUI/GCIntroNickNameFrame.h"

extern int              Result_ReportNickName;

class KGCAccountNickNameFrame : public KGCIntroNickNameFrame
{
public:
    DECLARE_CLASSNAME( KGCAccountNickNameFrame );
    /// Default constructor
    KGCAccountNickNameFrame( void );
    /// Default destructor
    virtual ~KGCAccountNickNameFrame( void );
    /// Copy constructor
    KGCAccountNickNameFrame( const KGCAccountNickNameFrame& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAccountNickNameFrame& operator=( const KGCAccountNickNameFrame& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();

    void OnConfirm();

private:

    KD3DWnd*			m_pkBtnConfirm;

};

DEFINE_WND_FACTORY( KGCAccountNickNameFrame );
DECLARE_WND_FACTORY( KGCAccountNickNameFrame );
DECLARE_WND_FACTORY_NAME( KGCAccountNickNameFrame );
