#pragma once

class GCResultDamage : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( GCResultDamage );
    /// Default constructor
    GCResultDamage( void );
    /// Default destructor
    virtual ~GCResultDamage( void );
    /// Copy constructor
    GCResultDamage( const GCResultDamage& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const GCResultDamage& operator=( const GCResultDamage& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();

public:
    void Init();
    void SetDamage(int iDamage,int iTeam);
protected:
    KD3DWnd* m_pkSerdinBack;
    KD3DWnd* m_pkCanabanBack;
    KD3DWnd* m_pkTotalDmgImg;
    KD3DStatic* m_pkDamageStatic;
    KD3DStatic* m_pkHealStatic;
};

DEFINE_WND_FACTORY( GCResultDamage );
DECLARE_WND_FACTORY( GCResultDamage );
DECLARE_WND_FACTORY_NAME( GCResultDamage );
