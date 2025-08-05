#pragma once

class KGCMainFrame;

class KGCCharInfoFrame_New;

class KGCCharSelectScene_New : public KGCScene
{
public:
    DECLARE_CLASSNAME( KGCCharSelectScene_New );
    /// Default constructor
    KGCCharSelectScene_New( void );
    /// Default destructor
    virtual ~KGCCharSelectScene_New( void );
    /// Copy constructor
    KGCCharSelectScene_New( const KGCCharSelectScene_New& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharSelectScene_New& operator=( const KGCCharSelectScene_New& )
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

    KGCCharInfoFrame_New* GetCharInfoFrame()    { return m_pkCharInfoFrame; }
    void SetVitalityInfo();

	virtual HRESULT Render( void );
    void UpdateData();
private:
    std::vector<KD3DWnd*> m_vecSlotList;

	KGCCharInfoFrame_New*	m_pkCharInfoFrame;
	KD3DStatic* m_pkStaticServerName;

	std::vector< CParticleEventSeqPTR > m_vecServerSceneEffect;

};

DEFINE_WND_FACTORY( KGCCharSelectScene_New );
DECLARE_WND_FACTORY( KGCCharSelectScene_New );
DECLARE_WND_FACTORY_NAME( KGCCharSelectScene_New );
