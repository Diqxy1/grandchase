#pragma once

class KGCNumberControl;

class KGCHeroDungeonLevelBox : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCHeroDungeonLevelBox );
    /// Default constructor
    KGCHeroDungeonLevelBox( void );
    /// Default destructor
    virtual ~KGCHeroDungeonLevelBox( void );
    /// Copy constructor
    KGCHeroDungeonLevelBox( const KGCHeroDungeonLevelBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroDungeonLevelBox& operator=( const KGCHeroDungeonLevelBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate();
    virtual void FrameMoveInEnabledState( void );

    enum{
        EMAX_DIGITS_NUM = 5,
        EMAX_DIGITS     = 10000,
    };

private:
    KSafeArray< KGCNumberControl*, EMAX_DIGITS_NUM > m_pkHeroCoinDigits;
    KGCNumberControl*   m_pkLowLevelLeft;
    KGCNumberControl*   m_pkLowLevelRight;

public:
    void SetHeroDungeonLevel( int iLevel_ = 0 );
    void SetHeroCoin(int iCoin_ = 0 );
};

DEFINE_WND_FACTORY( KGCHeroDungeonLevelBox );
DECLARE_WND_FACTORY( KGCHeroDungeonLevelBox );
DECLARE_WND_FACTORY_NAME( KGCHeroDungeonLevelBox );
