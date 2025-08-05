#pragma once

class KGCGwcCountry;

class KGCGwcEvent : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCGwcEvent );
    /// Default constructor
    KGCGwcEvent( void );
    /// Default destructor
    virtual ~KGCGwcEvent( void );
    /// Copy constructor
    KGCGwcEvent( const KGCGwcEvent& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGwcEvent& operator=( const KGCGwcEvent& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum EXCEPTION_COUNTRY{
        COUNTRY_LA = 13,
    };


public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );

public:
    KD3DWnd*        m_pkCloseBtn;
    KD3DWnd*        m_pkOkBtn;    
    std::map<DWORD, KGWCCountryInfo > m_mapCountryInfo;
    std::map<DWORD, KGCGwcCountry* > m_mapCountryBar;

public:
    void OnClickClose();
    void OnClickOK();
    void SetCountryInfo( std::map<DWORD, KGWCCountryInfo>& mapCountryInfo_ );
    void UpdateCountryInfo();
};

DEFINE_WND_FACTORY( KGCGwcEvent );
DECLARE_WND_FACTORY( KGCGwcEvent );
DECLARE_WND_FACTORY_NAME( KGCGwcEvent );
