#pragma once

class KGCSongkranWaterEffect : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSongkranWaterEffect );
    /// Default constructor
    KGCSongkranWaterEffect( void );
    /// Default destructor
    virtual ~KGCSongkranWaterEffect( void );
    /// Copy constructor
    KGCSongkranWaterEffect( const KGCSongkranWaterEffect& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSongkranWaterEffect& operator=( const KGCSongkranWaterEffect& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void PostDraw( void );
	virtual void FrameMoveInEnabledState();
    virtual void OnDestroy();


    void ADD_WORLDMAP_PARTICLE(std::string seq,float x=0.0f,float y=0.0f);
    void ClearMapEffect();
    void RenderMapEffect( bool bRender );

public:
	GCDeviceTexture*        m_pTexWater;
	GCDeviceTexture*	    m_pWaterBack;
	GCDeviceTexture*	    m_pWaterFront;

	float					m_fBackVertex[2];
	float					m_fFrontVertex[2];

    std::vector< CParticleEventSeqPTR > m_vecMapEffect;
};

DEFINE_WND_FACTORY( KGCSongkranWaterEffect );
DECLARE_WND_FACTORY( KGCSongkranWaterEffect );
DECLARE_WND_FACTORY_NAME( KGCSongkranWaterEffect );
