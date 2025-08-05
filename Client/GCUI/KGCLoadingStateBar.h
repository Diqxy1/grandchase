#pragma once

class KGCLoadingStateBar :
    public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCLoadingStateBar );
    /// Default constructor
    KGCLoadingStateBar( void );
    /// Default destructor
    virtual ~KGCLoadingStateBar( void );
    /// Copy constructor
    KGCLoadingStateBar( const KGCLoadingStateBar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCLoadingStateBar& operator=( const KGCLoadingStateBar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        EMAX_LOADING_GUAGE_DUNGEON          = 171,
        EMAX_LOADING_GUAGE_MATCH            = 106,
        EMIN_LOADING_GUAGE                  = 6,
    };

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

private:    
    KD3DSizingBox*      m_pkLoadingGuage;
    KD3DStatic*         m_pkUserNickName;
    KD3DStatic*         m_pkLoadingPercent;

    KSafeArray< KD3DWnd*, GC_CHAR_NUM > m_pkCharIcon;
    KD3DWnd*    m_pkIconBackActive;
    KD3DWnd*    m_pkIconBack;

public:
    void SetLoadingState( DWORD dwUserUID_, int iLoadingStete_ = 0 );
};

DEFINE_WND_FACTORY( KGCLoadingStateBar );
DECLARE_WND_FACTORY( KGCLoadingStateBar );
DECLARE_WND_FACTORY_NAME( KGCLoadingStateBar );