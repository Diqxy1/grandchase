#pragma once

class KGCHarvestGauge : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
    enum { LEVEL0 = 4, LEVEL1 = 3, LEVEL2 = 2, LEVEL3 = 1 };
    enum { GAUGE_MIN = 12, GAUGE_HIT_START = 87, GAUGE_HIT_END = 113, GAUGE_MAX = 131 };

public:
    DECLARE_CLASSNAME( KGCHarvestGauge );
    /// Default constructor
    KGCHarvestGauge( void );
    /// Default destructor
    virtual ~KGCHarvestGauge( void );
    /// Copy constructor
    KGCHarvestGauge( const KGCHarvestGauge& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHarvestGauge& operator=( const KGCHarvestGauge& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DSizingBox*  m_pkGauge;

    int         m_iLevel;
    int         m_iFrameCount;
    int         m_iLengthCount;
    float       m_fGaugeStep;

public:
    void Init( D3DXVECTOR2& dxvPos_ );
    bool Stop( void );

private:
    int GetSpeedLevel( int iRate_ );
    void SetHarvestParticle( int iNumStar, bool bBonus );
};

DEFINE_WND_FACTORY( KGCHarvestGauge );
DECLARE_WND_FACTORY( KGCHarvestGauge );
DECLARE_WND_FACTORY_NAME( KGCHarvestGauge );
