#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCMainFrame;

class KGCCharIntroFrame;

class KGCCharGainScene : public KGCScene
{
public:
    DECLARE_CLASSNAME( KGCCharGainScene );
    /// Default constructor
    KGCCharGainScene( void );
    /// Default destructor
    virtual ~KGCCharGainScene( void );
    /// Copy constructor
    KGCCharGainScene( const KGCCharGainScene& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharGainScene& operator=( const KGCCharGainScene& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	
	virtual void InitScene() {}
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );

	virtual void FrameMoveInEnabledState( void );

	virtual void EnterScene();
	virtual void OnLeaveScene();


	void Add_NewsFrame_Particle(std::string seq,float x=0.0f,float y=0.0f);
	void RenderEffect( bool bRender );
	void CreateEffect();

    KGCCharIntroFrame* GetCharIntroFrame()      { return m_pkCharIntroFrame; }

	virtual HRESULT Render( void );
private:

	KGCCharIntroFrame*	m_pkCharIntroFrame;

	std::vector< CParticleEventSeqPTR > m_vecServerSceneEffect;

};

DEFINE_WND_FACTORY( KGCCharGainScene );
DECLARE_WND_FACTORY( KGCCharGainScene );
DECLARE_WND_FACTORY_NAME( KGCCharGainScene );
