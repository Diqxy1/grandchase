#pragma once

class KGCCharIcon;

class KGCCharIconWnd : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCCharIconWnd );
    /// Default constructor
    KGCCharIconWnd( void );
    /// Default destructor
    virtual ~KGCCharIconWnd( void );
    /// Copy constructor
    KGCCharIconWnd( const KGCCharIconWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharIconWnd& operator=( const KGCCharIconWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KSafeArray< KGCCharIcon*, GC_CHAR_NUM >     m_apkIcon;
    int  m_iCurIndex;

public:
    void ShowCharIcon( int iIndex, bool bLock );

};

DEFINE_WND_FACTORY( KGCCharIconWnd );
DECLARE_WND_FACTORY( KGCCharIconWnd );
DECLARE_WND_FACTORY_NAME( KGCCharIconWnd );
