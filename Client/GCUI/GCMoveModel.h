#pragma once

class KGCMoveModel : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCMoveModel );
    /// Default constructor
    KGCMoveModel( void );
    /// Default destructor
    virtual ~KGCMoveModel( void );
    /// Copy constructor
    KGCMoveModel( const KGCMoveModel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMoveModel& operator=( const KGCMoveModel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );
    virtual void FrameMoveInEnabledState( void );
	void SetModelID( int iModel);
    void SetModelImage();
	//void ClearImage();
	void OnDestroy( void );
	//void PreDraw( void );
    void InitNPCData();
    void SetMotion( int index );
    bool IsMotion( int index );
	void LoadMotion();
	void ShowEffect( bool show );
	void Set3A0(int x) { this->m_3A0 = x; }
	void CreateEffect(std::string seq, float x, float y, float z);
	void RenderEffect(bool bRender);
	void ClearEffect(CParticleEventSeqPTR particle);
	void ClearAllEffects();
private:

	enum MOUSE_EVENT_TYPE
	{
		EVENT_NONE,
		EVENT_MOUSE_CLICK,
		EVENT_NONE_CLICK
	};

    D3DVIEWPORT9				m_viewport;
    D3DXMATRIX					m_matProject;
	D3DXMATRIX					m_matOrtho;						// 0x358
	//KGC2DObject*				m_pkNPCImage;
    GCObject*						m_pModelObject;
    GCDeviceMotionFRM*		m_pModelMotion;

	int									m_3A0;								// 0x3A0
    int                 				m_cFrame;							// 0x3A4
	int									m_unk;								// 
    int                 				m_iNowModelID;				// 
    int                 				m_iNowModelMotionIndex;	// 
    //int                 				m_iMotionReplayCount;		// 
    int                 				m_iMotionCount;				// 
	int									m_3B8;								// 
	int									m_3BC;								// 

    MoveModelData				m_MoveModelData;
	std::map< int, std::pair< GCDeviceMotionFRM*, MoveModelMotion > > m_mapMotionList; //0x440
	std::map< std::string, std::pair< CParticleEventSeqPTR, MoveModelParticle > > m_mapParticleList; //0x44C
	std::vector< CParticleEventSeqPTR > m_vecEffects; //0x458
	std::vector< CParticleEventSeqPTR > m_vecHiddenEffects; //0x468
};

DEFINE_WND_FACTORY( KGCMoveModel );
DECLARE_WND_FACTORY( KGCMoveModel );
DECLARE_WND_FACTORY_NAME( KGCMoveModel );