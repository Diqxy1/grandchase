#pragma once
#ifndef NEW_CHAR_SELECT_SCENE
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCMainFrame;

class KGCCharInfoFrame;

class KGCCharSelectScene : public KGCScene
{
public:
    DECLARE_CLASSNAME( KGCCharSelectScene );
    /// Default constructor
    KGCCharSelectScene( void );
    /// Default destructor
    virtual ~KGCCharSelectScene( void );
    /// Copy constructor
    KGCCharSelectScene( const KGCCharSelectScene& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharSelectScene& operator=( const KGCCharSelectScene& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	
	virtual void InitScene() {}
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

	virtual void FrameMoveInEnabledState( void );

	virtual void EnterScene();

	virtual void OnLeaveScene();

	void Add_NewsFrame_Particle(std::string seq,float x=0.0f,float y=0.0f);
	void RenderEffect( bool bRender );
	void CreateEffect();
    void UpdateEmptySlot();

    KGCCharInfoFrame* GetCharInfoFrame()    { return m_pkCharInfoFrame; }

    void SetVitalityInfo();

	virtual HRESULT Render( void );
    void UpdateData();
private:

	KGCCharInfoFrame*	m_pkCharInfoFrame;

	KD3DStatic* m_pkStaticServerName;

	std::vector< CParticleEventSeqPTR > m_vecServerSceneEffect;

};

DEFINE_WND_FACTORY( KGCCharSelectScene );
DECLARE_WND_FACTORY( KGCCharSelectScene );
DECLARE_WND_FACTORY_NAME( KGCCharSelectScene );
#endif