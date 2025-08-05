#pragma once

class KGCRitasBoxOpenFrame : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRitasBoxOpenFrame );
    /// Default constructor
    KGCRitasBoxOpenFrame( void );
    /// Default destructor
    virtual ~KGCRitasBoxOpenFrame( void );
    /// Copy constructor
    KGCRitasBoxOpenFrame( const KGCRitasBoxOpenFrame& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRitasBoxOpenFrame& operator=( const KGCRitasBoxOpenFrame& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

	void SetFrameType(int iType_ );

private:
	KD3DWnd*	m_pkBoxImg;
	KD3DStatic*	m_pkStaticExplain;



};

DEFINE_WND_FACTORY( KGCRitasBoxOpenFrame );
DECLARE_WND_FACTORY( KGCRitasBoxOpenFrame );
DECLARE_WND_FACTORY_NAME( KGCRitasBoxOpenFrame );
