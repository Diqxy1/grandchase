#pragma once

class KGCVipCollection : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCVipCollection );
    /// Default constructor
    KGCVipCollection( void );
    /// Default destructor
    virtual ~KGCVipCollection( void );
    /// Copy constructor
    KGCVipCollection( const KGCVipCollection& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCVipCollection& operator=( const KGCVipCollection& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void SetVIPImage( int iIndex, int iFontID );
    void ResetCharPlates();

private:
	KSafeArray< KD3DWnd*, VIP_COLLECTION_SIZE > m_vecVipContainer;
};

DEFINE_WND_FACTORY( KGCVipCollection );
DECLARE_WND_FACTORY( KGCVipCollection );
DECLARE_WND_FACTORY_NAME( KGCVipCollection );
