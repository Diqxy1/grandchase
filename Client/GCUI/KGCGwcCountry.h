#pragma once

#define NUM_COUNTRY_MAX 7
#define MAX_GRADE 15

class KGCGwcCountry : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGwcCountry );
    /// Default constructor
    KGCGwcCountry( void );
    /// Default destructor
    virtual ~KGCGwcCountry( void );
    /// Copy constructor
    KGCGwcCountry( const KGCGwcCountry& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGwcCountry& operator=( const KGCGwcCountry& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void SetInformation( KGWCCountryInfo mapCountryInfo_ );

public:
    KD3DWnd*  m_pkFlag;
    KGCSizingThreeBox* m_pkGuageBar;
    KSafeArray<KD3DWnd*,MAX_GRADE>    m_pkLevelWnd;

};

DEFINE_WND_FACTORY( KGCGwcCountry );
DECLARE_WND_FACTORY( KGCGwcCountry );
DECLARE_WND_FACTORY_NAME( KGCGwcCountry );
