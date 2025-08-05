#pragma once

const float MAX_ECLIPSE_SCALE = 1.4f;
const float MIN_ECLIPSE_SCALE = 0.9f;
const float FRAME_PER_REVOLUTION = 55; // 55 == 1sec



class KGCEclipseTime : public KD3DDialog    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCEclipseTime );
    /// Default constructor
    KGCEclipseTime( void );
    /// Default destructor
    virtual ~KGCEclipseTime( void );
    /// Copy constructor
    KGCEclipseTime( const KGCEclipseTime& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCEclipseTime& operator=( const KGCEclipseTime& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );

    void SetGauge();
    void SetActiveIcon();
    void Tick();
	void ClickEvent();
	void OnDestroy();
    void EclipseScaling();
        
private:
    KD3DWnd     *m_pkMoonGauge;
    KD3DWnd     *m_pkSunGauge;

    KD3DWnd     *m_pkEclipse;
    KD3DWnd     *m_pkEclipse_ac;
	CParticleEventSeqPTR m_pkParticle;

    bool        m_bBigger;
    float       m_fCurrentScale;

};

DEFINE_WND_FACTORY( KGCEclipseTime );
DECLARE_WND_FACTORY( KGCEclipseTime );
DECLARE_WND_FACTORY_NAME( KGCEclipseTime );
