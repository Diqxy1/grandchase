#pragma once


class KGCAgitMoneyInfo : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAgitMoneyInfo );
    /// Default constructor
    KGCAgitMoneyInfo( void );
    /// Default destructor
    virtual ~KGCAgitMoneyInfo( void );
    /// Copy constructor
    KGCAgitMoneyInfo( const KGCAgitMoneyInfo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAgitMoneyInfo& operator=( const KGCAgitMoneyInfo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

private:
    KD3DSizingBox*  m_pkFrame;
    KD3DStatic* m_pkStaticSeed;
    KD3DStatic* m_pkStaticStar;

public:
    void UpdateAgitMoney( void );
};

DEFINE_WND_FACTORY( KGCAgitMoneyInfo );
DECLARE_WND_FACTORY( KGCAgitMoneyInfo );
DECLARE_WND_FACTORY_NAME( KGCAgitMoneyInfo );
